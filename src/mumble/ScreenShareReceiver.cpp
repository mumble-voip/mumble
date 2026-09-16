// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenShareReceiver.h"

#include "MumbleProtocol.h"

#ifdef USE_SCREEN_SHARING
/// Maps the protocol's Codec enum to the corresponding FFmpeg codec ID.
/// To add support for a new codec: add the proto enum value in MumbleUDP.proto,
/// then add a case here returning the appropriate AV_CODEC_ID_*.
static AVCodecID codecIdForProtoCodec(MumbleUDP::Video::Codec c) {
	switch (c) {
		case MumbleUDP::Video::H264:
			return AV_CODEC_ID_H264;
		default:
			return AV_CODEC_ID_NONE;
	}
}
#endif

ScreenShareReceiver::ScreenShareReceiver(QObject *parent) : QObject(parent) {
}

ScreenShareReceiver::~ScreenShareReceiver() {
#ifdef USE_SCREEN_SHARING
	// Tear down all decoders.
	for (std::pair< const unsigned int, DecoderState > &kv : m_decoders) {
		DecoderState &ds = kv.second;
		if (ds.swsCtx) {
			sws_freeContext(ds.swsCtx);
		}
		if (ds.frame) {
			av_frame_free(&ds.frame);
		}
		if (ds.packet) {
			av_packet_free(&ds.packet);
		}
		if (ds.codecCtx) {
			avcodec_free_context(&ds.codecCtx);
		}
	}
#endif
}


void ScreenShareReceiver::handleVideoPacket(const Mumble::Protocol::VideoData &videoData) {
#ifndef USE_SCREEN_SHARING
	Q_UNUSED(videoData);
#else
	const quint32 session   = videoData.senderSession;
	const quint64 frameNum  = videoData.frameNumber;
	const quint32 fragIdx   = videoData.fragmentIndex;
	const quint32 fragCount = videoData.fragmentCount;

	if (fragCount == 0 || fragIdx >= fragCount)
		return;

	PendingFrame &pf = m_fragmentBuffer[session][frameNum];

	// Initialize frame on first fragment
	if (pf.fragmentCount == 0) {
		pf.fragmentCount = fragCount;
		pf.fragments.resize(fragCount);
		pf.width  = videoData.width;
		pf.height = videoData.height;
		pf.codec  = videoData.codec;
	}

	// OR keyframe flag (UDP fragments may arrive out of order)
	pf.isKeyFrame |= videoData.isKeyFrame;

	// Store fragment (copy once per fragment, unavoidable unless lifetime guaranteed)
	if (pf.fragments[fragIdx].isEmpty()) {
		pf.fragments[fragIdx] = QByteArray(reinterpret_cast< const char * >(videoData.payload.data()),
										   static_cast< int >(videoData.payload.size()));
	}

	// Early exit until complete
	for (const QByteArray &frag : pf.fragments) {
		if (frag.isEmpty())
			return;
	}

	// Compute total size once
	size_t totalSize = 0;
	for (const QByteArray &frag : pf.fragments)
		totalSize += static_cast< size_t >(frag.size());

	QByteArray complete;
	complete.resize(static_cast< int >(totalSize));

	char *dst = complete.data();
	for (const QByteArray &frag : pf.fragments) {
		memcpy(dst, frag.constData(), static_cast< size_t >(frag.size()));
		dst += frag.size();
	}

	// Capture frame metadata
	const quint32 fw                    = pf.width;
	const quint32 fh                    = pf.height;
	const bool isKeyFrm                 = pf.isKeyFrame;
	const MumbleUDP::Video::Codec codec = pf.codec;

	// Cleanup old frames for this session
	std::map< unsigned long long, PendingFrame > &senderMap = m_fragmentBuffer[session];

	// Using auto here as type because this is an iterator
	for (auto it = senderMap.begin(); it != senderMap.end();) {
		if (it->first <= frameNum)
			it = senderMap.erase(it);
		else
			++it;
	}

	decodeCompleteFrame(session, complete, fw, fh, isKeyFrm, codec);
#endif
}

void ScreenShareReceiver::resetSender(quint32 senderSession) {
#ifdef USE_SCREEN_SHARING
	m_fragmentBuffer.erase(senderSession);
	destroyDecoder(senderSession);
#else
	Q_UNUSED(senderSession);
#endif
}

#ifdef USE_SCREEN_SHARING
bool ScreenShareReceiver::ensureDecoder(quint32 session, MumbleUDP::Video::Codec protoCodec) {
	if (m_decoders.count(session) && m_decoders[session].codecCtx)
		return true;

	const AVCodecID avCodecId = codecIdForProtoCodec(protoCodec);
	if (avCodecId == AV_CODEC_ID_NONE)
		return false;

	const AVCodec *codec = avcodec_find_decoder(avCodecId);
	if (!codec)
		return false;

	DecoderState ds;
	ds.codec    = protoCodec;
	ds.codecCtx = avcodec_alloc_context3(codec);
	if (!ds.codecCtx)
		return false;

	if (avcodec_open2(ds.codecCtx, codec, nullptr) < 0) {
		avcodec_free_context(&ds.codecCtx);
		return false;
	}

	ds.frame            = av_frame_alloc();
	ds.packet           = av_packet_alloc();
	m_decoders[session] = ds;
	return true;
}

void ScreenShareReceiver::destroyDecoder(quint32 session) {
	auto it = m_decoders.find(session);
	if (it == m_decoders.end())
		return;

	DecoderState &ds = it->second;
	if (ds.swsCtx) {
		sws_freeContext(ds.swsCtx);
	}
	if (ds.frame) {
		av_frame_free(&ds.frame);
	}
	if (ds.packet) {
		av_packet_free(&ds.packet);
	}
	if (ds.codecCtx) {
		avcodec_free_context(&ds.codecCtx);
	}
	m_decoders.erase(it);
}

void ScreenShareReceiver::decodeCompleteFrame(quint32 session, const QByteArray &encodedData, quint32 /*width*/,
											  quint32 /*height*/, bool isKeyFrame, MumbleUDP::Video::Codec codec) {
	if (!ensureDecoder(session, codec))
		return;

	DecoderState &ds = m_decoders[session];

	// Drop non-keyframes until the decoder has seen at least one IDR.
	// Without SPS/PPS (which come with the keyframe) the decoder can't
	// reference picture parameters and emits "non-existing PPS" errors.
	if (!ds.gotKeyFrame && !isKeyFrame)
		return;

	if (isKeyFrame) {
		// Flush any buffered decoder state from a previous stream so the new
		// IDR is treated as a clean start.
		avcodec_flush_buffers(ds.codecCtx);
		ds.gotKeyFrame = true;
	}

	av_packet_unref(ds.packet);
	ds.packet->data = reinterpret_cast< uint8_t * >(const_cast< char * >(encodedData.constData()));
	ds.packet->size = static_cast< int >(encodedData.size());

	if (avcodec_send_packet(ds.codecCtx, ds.packet) < 0) {
		// Packet was rejected (e.g. corrupted reference frame from UDP loss).
		// Flush and wait for the next keyframe so we don't propagate corruption.
		avcodec_flush_buffers(ds.codecCtx);
		ds.gotKeyFrame = false;
		return;
	}

	while (avcodec_receive_frame(ds.codecCtx, ds.frame) == 0) {
		const int dw = ds.frame->width;
		const int dh = ds.frame->height;

		// (Re-)create the sws context if dimensions changed.
		if (!ds.swsCtx || ds.swsWidth != dw || ds.swsHeight != dh) {
			if (ds.swsCtx)
				sws_freeContext(ds.swsCtx);
			ds.swsCtx = sws_getContext(dw, dh, static_cast< AVPixelFormat >(ds.frame->format), dw, dh, AV_PIX_FMT_RGBA,
									   SWS_BILINEAR, nullptr, nullptr, nullptr);
			ds.swsWidth  = dw;
			ds.swsHeight = dh;
		}
		if (!ds.swsCtx)
			continue;

		QImage img(dw, dh, QImage::Format_RGBA8888);
		uint8_t *dstData[1] = { img.bits() };
		int dstStride[1]    = { static_cast< int >(img.bytesPerLine()) };
		sws_scale(ds.swsCtx, ds.frame->data, ds.frame->linesize, 0, dh, dstData, dstStride);

		emit frameDecoded(session, img);
	}
}
#endif
