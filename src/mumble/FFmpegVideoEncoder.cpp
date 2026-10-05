// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "FFmpegVideoEncoder.h"

#include <QtGui/QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <algorithm>

namespace {

struct EncoderSpec {
	/// Name of the encoder in FFmpeg
	const char *name;
	const char *label;
	MumbleUDP::Video::Codec codec;
	bool hardware;
	/// Encoder specific options as "key=value" pairs separated by ';', tuned for low latency. They are applied
	/// one by one and failures are ignored, as available options differ between FFmpeg versions.
	const char *options;
};

constexpr const char *NVENC_OPTIONS = "preset=p1;tune=ull;zerolatency=1;delay=0;forced-idr=1";
constexpr const char *AMF_OPTIONS   = "usage=ultralowlatency;quality=speed";
constexpr const char *QSV_OPTIONS   = "preset=veryfast;async_depth=1;low_delay_brc=1;forced_idr=1";
constexpr const char *VAAPI_OPTIONS = "async_depth=1";
constexpr const char *MF_OPTIONS    = "scenario=display_remoting;hw_encoding=1";

/// All encoders that may be used, best first.
///
/// Hardware encoders come first, as they encode in real time with hardly any CPU load. Among those, newer codecs
/// are preferred, as they achieve a better quality at the same bit rate. Software encoders follow, with the
/// cheaper ones first. libx264 comes last since it is GPL licensed and therefore often not available.
constexpr EncoderSpec ENCODERS[] = {
	{ "av1_nvenc", "AV1 (NVENC)", MumbleUDP::Video::AV1, true, NVENC_OPTIONS },
	{ "av1_amf", "AV1 (AMF)", MumbleUDP::Video::AV1, true, AMF_OPTIONS },
	{ "av1_qsv", "AV1 (Quick Sync)", MumbleUDP::Video::AV1, true, QSV_OPTIONS },
	{ "av1_vaapi", "AV1 (VA-API)", MumbleUDP::Video::AV1, true, VAAPI_OPTIONS },
	{ "av1_mf", "AV1 (Media Foundation)", MumbleUDP::Video::AV1, true, MF_OPTIONS },
	{ "vp9_qsv", "VP9 (Quick Sync)", MumbleUDP::Video::VP9, true, QSV_OPTIONS },
	{ "vp9_vaapi", "VP9 (VA-API)", MumbleUDP::Video::VP9, true, VAAPI_OPTIONS },
	{ "h264_nvenc", "H.264 (NVENC)", MumbleUDP::Video::H264, true, NVENC_OPTIONS },
	{ "h264_amf", "H.264 (AMF)", MumbleUDP::Video::H264, true, AMF_OPTIONS },
	{ "h264_qsv", "H.264 (Quick Sync)", MumbleUDP::Video::H264, true, QSV_OPTIONS },
	{ "h264_vaapi", "H.264 (VA-API)", MumbleUDP::Video::H264, true, VAAPI_OPTIONS },
	{ "h264_videotoolbox", "H.264 (VideoToolbox)", MumbleUDP::Video::H264, true, "realtime=1;prio_speed=1;allow_sw=0" },
	{ "h264_mf", "H.264 (Media Foundation)", MumbleUDP::Video::H264, true, MF_OPTIONS },
	{ "libvpx-vp9", "VP9 (libvpx)", MumbleUDP::Video::VP9, false,
	  "deadline=realtime;cpu-used=8;lag-in-frames=0;row-mt=1" },
	{ "libvpx", "VP8 (libvpx)", MumbleUDP::Video::VP8, false, "deadline=realtime;cpu-used=8;lag-in-frames=0" },
	{ "libopenh264", "H.264 (OpenH264)", MumbleUDP::Video::H264, false, "" },
	{ "libsvtav1", "AV1 (SVT-AV1)", MumbleUDP::Video::AV1, false, "preset=10;svtav1-params=rtc=1:pred-struct=1:rc=2" },
	{ "libaom-av1", "AV1 (libaom)", MumbleUDP::Video::AV1, false,
	  "usage=realtime;cpu-used=8;lag-in-frames=0;row-mt=1" },
	{ "libx264", "H.264 (x264)", MumbleUDP::Video::H264, false, "preset=superfast;tune=zerolatency;forced-idr=1" },
};

const EncoderSpec *findSpec(const QString &id) {
	for (const EncoderSpec &spec : ENCODERS) {
		if (id == QLatin1String(spec.name))
			return &spec;
	}
	return nullptr;
}

void applyOptions(void *privData, const char *options) {
	for (const QString &option : QString::fromLatin1(options).split(QLatin1Char(';'), Qt::SkipEmptyParts)) {
		const qsizetype separator = option.indexOf(QLatin1Char('='));
		if (separator < 0)
			continue;
		av_opt_set(privData, option.left(separator).toLatin1().constData(),
				   option.mid(separator + 1).toLatin1().constData(), 0);
	}
}

/// The pixel formats the encoder takes, or an empty list if it doesn't say.
std::vector< AVPixelFormat > supportedPixelFormats(const AVCodec *codec) {
	std::vector< AVPixelFormat > formats;
#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(61, 13, 100)
	const void *configs = nullptr;
	int count           = 0;
	if (avcodec_get_supported_config(nullptr, codec, AV_CODEC_CONFIG_PIX_FORMAT, 0, &configs, &count) >= 0 && configs) {
		const AVPixelFormat *list = static_cast< const AVPixelFormat * >(configs);
		formats.assign(list, list + count);
	}
#else
	if (codec->pix_fmts) {
		for (const AVPixelFormat *format = codec->pix_fmts; *format != AV_PIX_FMT_NONE; ++format)
			formats.push_back(*format);
	}
#endif
	return formats;
}

} // namespace

std::vector< VideoEncoderInfo > FFmpegVideoEncoder::candidates() {
	std::vector< VideoEncoderInfo > encoders;
	for (const EncoderSpec &spec : ENCODERS) {
		if (!avcodec_find_encoder_by_name(spec.name))
			continue;

		VideoEncoderInfo info;
		info.id       = QLatin1String(spec.name);
		info.name     = QLatin1String(spec.label);
		info.codec    = spec.codec;
		info.hardware = spec.hardware;
		encoders.push_back(info);
	}
	return encoders;
}

bool FFmpegVideoEncoder::probe(const VideoEncoderInfo &info) {
	VideoEncoderConfig config;
	config.width            = 1280;
	config.height           = 720;
	config.bitrate          = 1'000'000;
	config.fps              = 15;
	config.keyFrameInterval = 60;

	std::unique_ptr< FFmpegVideoEncoder > encoder = open(info, config);
	if (!encoder)
		return false;

	// Some encoders only fail once they get the first picture
	QImage image(config.width, config.height, QImage::Format_RGBA8888);
	image.fill(Qt::gray);
	std::vector< Packet > packets;
	return encoder->encode(image, 0, true, packets);
}

std::unique_ptr< FFmpegVideoEncoder > FFmpegVideoEncoder::open(const VideoEncoderInfo &info,
															   const VideoEncoderConfig &config) {
	std::unique_ptr< FFmpegVideoEncoder > encoder(new FFmpegVideoEncoder(info));
	if (!encoder->init(config))
		return nullptr;
	return encoder;
}

FFmpegVideoEncoder::FFmpegVideoEncoder(const VideoEncoderInfo &info) : m_info(info) {
}

FFmpegVideoEncoder::~FFmpegVideoEncoder() {
	if (m_swsCtx)
		sws_freeContext(m_swsCtx);
	av_frame_free(&m_hwFrame);
	av_frame_free(&m_frame);
	av_packet_free(&m_packet);
	avcodec_free_context(&m_codecCtx);
	av_buffer_unref(&m_hwFrames);
	av_buffer_unref(&m_hwDevice);
}

const VideoEncoderInfo &FFmpegVideoEncoder::info() const {
	return m_info;
}

bool FFmpegVideoEncoder::init(const VideoEncoderConfig &config) {
	const EncoderSpec *spec = findSpec(m_info.id);
	if (!spec)
		return false;

	const AVCodec *codec = avcodec_find_encoder_by_name(spec->name);
	if (!codec)
		return false;

	// Prefer feeding pictures from system memory, which nearly all encoders (including most hardware ones) take.
	// Only if the encoder doesn't, the pictures are uploaded to the GPU.
	const std::vector< AVPixelFormat > formats = supportedPixelFormats(codec);
	auto supports                              = [&formats](AVPixelFormat format) {
        return formats.empty() || std::find(formats.begin(), formats.end(), format) != formats.end();
	};

	AVPixelFormat swFormat = AV_PIX_FMT_NONE;
	AVPixelFormat hwFormat = AV_PIX_FMT_NONE;
	if (supports(AV_PIX_FMT_YUV420P)) {
		swFormat = AV_PIX_FMT_YUV420P;
	} else if (supports(AV_PIX_FMT_NV12)) {
		swFormat = AV_PIX_FMT_NV12;
	} else {
		for (int i = 0;; ++i) {
			const AVCodecHWConfig *hwConfig = avcodec_get_hw_config(codec, i);
			if (!hwConfig)
				break;
			if (!(hwConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_FRAMES_CTX))
				continue;

			if (av_hwdevice_ctx_create(&m_hwDevice, hwConfig->device_type, nullptr, nullptr, 0) < 0)
				return false;

			hwFormat = hwConfig->pix_fmt;
			swFormat = AV_PIX_FMT_NV12;
			break;
		}
		if (hwFormat == AV_PIX_FMT_NONE)
			return false;
	}

	m_codecCtx = avcodec_alloc_context3(codec);
	if (!m_codecCtx)
		return false;

	m_codecCtx->width     = config.width;
	m_codecCtx->height    = config.height;
	m_codecCtx->time_base = { 1, 1'000'000 }; // timestamps are in microseconds
	m_codecCtx->framerate = { config.fps, 1 };
	m_codecCtx->pix_fmt   = hwFormat != AV_PIX_FMT_NONE ? hwFormat : swFormat;
	m_codecCtx->bit_rate  = config.bitrate;
	m_codecCtx->gop_size  = config.keyFrameInterval;
	// B-frames add latency, as frames have to wait for later ones
	m_codecCtx->max_b_frames = 0;

	applyOptions(m_codecCtx->priv_data, spec->options);

	if (m_hwDevice) {
		m_hwFrames = av_hwframe_ctx_alloc(m_hwDevice);
		if (!m_hwFrames)
			return false;

		AVHWFramesContext *framesCtx = reinterpret_cast< AVHWFramesContext * >(m_hwFrames->data);
		framesCtx->format            = hwFormat;
		framesCtx->sw_format         = swFormat;
		framesCtx->width             = config.width;
		framesCtx->height            = config.height;
		framesCtx->initial_pool_size = 4;
		if (av_hwframe_ctx_init(m_hwFrames) < 0)
			return false;

		m_codecCtx->hw_frames_ctx = av_buffer_ref(m_hwFrames);
		m_hwFrame                 = av_frame_alloc();
		if (!m_codecCtx->hw_frames_ctx || !m_hwFrame)
			return false;
	}

	if (avcodec_open2(m_codecCtx, codec, nullptr) < 0)
		return false;

	m_frame  = av_frame_alloc();
	m_packet = av_packet_alloc();
	if (!m_frame || !m_packet)
		return false;

	m_frame->format = swFormat;
	m_frame->width  = config.width;
	m_frame->height = config.height;
	if (av_frame_get_buffer(m_frame, 0) < 0)
		return false;

	return true;
}

bool FFmpegVideoEncoder::encode(const QImage &srcImage, qint64 timestamp, bool keyFrame,
								std::vector< Packet > &packets) {
	if (srcImage.width() != m_codecCtx->width || srcImage.height() != m_codecCtx->height)
		return false;

	const QImage image = srcImage.convertToFormat(QImage::Format_RGBA8888);

	m_swsCtx =
		sws_getCachedContext(m_swsCtx, image.width(), image.height(), AV_PIX_FMT_RGBA, m_frame->width, m_frame->height,
							 static_cast< AVPixelFormat >(m_frame->format), SWS_BICUBIC, nullptr, nullptr, nullptr);
	if (!m_swsCtx)
		return false;

	if (av_frame_make_writable(m_frame) < 0)
		return false;

	const uint8_t *srcData[1] = { image.constBits() };
	const int srcLinesize[1]  = { static_cast< int >(image.bytesPerLine()) };
	sws_scale(m_swsCtx, srcData, srcLinesize, 0, image.height(), m_frame->data, m_frame->linesize);

	AVFrame *input = m_frame;
	if (m_hwFrames) {
		av_frame_unref(m_hwFrame);
		if (av_hwframe_get_buffer(m_hwFrames, m_hwFrame, 0) < 0
			|| av_hwframe_transfer_data(m_hwFrame, m_frame, 0) < 0) {
			return false;
		}
		input = m_hwFrame;
	}

	input->pts = timestamp;
	// Frames are reused, so the picture type has to be reset after a forced key frame
	input->pict_type = keyFrame ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_NONE;

	if (avcodec_send_frame(m_codecCtx, input) < 0)
		return false;

	while (avcodec_receive_packet(m_codecCtx, m_packet) == 0) {
		Packet packet;
		packet.data       = QByteArray(reinterpret_cast< const char * >(m_packet->data), m_packet->size);
		packet.timestamp  = m_packet->pts != AV_NOPTS_VALUE ? m_packet->pts : timestamp;
		packet.isKeyFrame = (m_packet->flags & AV_PKT_FLAG_KEY) != 0;
		packets.push_back(std::move(packet));
		av_packet_unref(m_packet);
	}

	return true;
}
