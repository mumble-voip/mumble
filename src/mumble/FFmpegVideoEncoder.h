// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_FFMPEGVIDEOENCODER_H_
#define MUMBLE_MUMBLE_FFMPEGVIDEOENCODER_H_

#include "VideoEncoderBackend.h"

struct AVBufferRef;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;
struct SwsContext;

/// Video encoder backed by FFmpeg's libavcodec.
///
/// libavcodec wraps both software codec libraries and the platform's hardware encoders (NVENC, AMF, Quick Sync,
/// VA-API, VideoToolbox, Media Foundation), so this covers all of them. Encoders that take RGB pictures (e.g.
/// NVENC and AMF) get them as they are, without any conversion on the CPU. For all others the pictures are
/// converted to YUV, and uploaded to the GPU for encoders that only take pictures in GPU memory (e.g. VA-API).
class FFmpegVideoEncoder : public VideoEncoderBackend {
public:
	/// The encoders this build of FFmpeg contains, best first. Whether they actually work on this system is not
	/// checked; see probe().
	static std::vector< VideoEncoderInfo > candidates();
	/// Checks whether the given encoder works on this system by encoding a picture with it.
	static bool probe(const VideoEncoderInfo &info);

	/// Opens the given encoder, or returns nullptr if that fails.
	static std::unique_ptr< FFmpegVideoEncoder > open(const VideoEncoderInfo &info, const VideoEncoderConfig &config);

	~FFmpegVideoEncoder() override;

	const VideoEncoderInfo &info() const override;
	bool encode(const QImage &image, qint64 timestamp, bool keyFrame, std::vector< Packet > &packets) override;

private:
	explicit FFmpegVideoEncoder(const VideoEncoderInfo &info);
	bool init(const VideoEncoderConfig &config);

	VideoEncoderInfo m_info;

	AVCodecContext *m_codecCtx = nullptr;
	/// Picture in system memory, in the format the encoder (or the upload to the GPU) takes. When the encoder
	/// takes the pictures as they are, it only refers to the passed in picture while it is being encoded.
	AVFrame *m_frame   = nullptr;
	AVPacket *m_packet = nullptr;
	/// Converts the pictures to m_frame, unless the encoder takes them as they are
	SwsContext *m_swsCtx = nullptr;
	/// Format the pictures are passed to the encoder in when it takes them as they are, AV_PIX_FMT_NONE (-1)
	/// otherwise. Kept as int, so that the FFmpeg headers aren't needed here.
	int m_inputFormat = -1;

	/// Only set for encoders that take their input in GPU memory
	AVBufferRef *m_hwDevice = nullptr;
	AVBufferRef *m_hwFrames = nullptr;
	AVFrame *m_hwFrame      = nullptr;
};

#endif // MUMBLE_MUMBLE_FFMPEGVIDEOENCODER_H_
