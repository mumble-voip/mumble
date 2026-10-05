// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOENCODERBACKEND_H_
#define MUMBLE_MUMBLE_VIDEOENCODERBACKEND_H_

#include "MumbleUDP.pb.h"

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <memory>
#include <vector>

class QImage;

/// Describes a video encoder that is available on this system.
struct VideoEncoderInfo {
	/// Stable identifier, e.g. for storing the user's choice in the settings
	QString id;
	/// Human readable name, e.g. "AV1 (NVENC)"
	QString name;
	MumbleUDP::Video::Codec codec = MumbleUDP::Video::H264;
	/// Whether the encoder runs on dedicated hardware rather than on the CPU
	bool hardware = false;
};

/// Parameters for opening an encoder.
struct VideoEncoderConfig {
	int width  = 0;
	int height = 0;
	/// Target bit rate in bits per second
	int bitrate = 0;
	/// Nominal frame rate. The actual frame spacing is given by the timestamps passed to encode().
	int fps = 0;
	/// Maximum distance between key frames, in frames
	int keyFrameInterval = 0;
};

/// An opened video encoder for a fixed picture size.
///
/// This is the interface between the screen sharing pipeline and the libraries that implement the codecs, so that
/// these can be replaced (e.g. by platform encoders used directly) without touching the rest of the pipeline.
class VideoEncoderBackend {
public:
	struct Packet {
		QByteArray data;
		/// Timestamp of the picture this packet belongs to, in microseconds
		qint64 timestamp = 0;
		bool isKeyFrame  = false;
	};

	virtual ~VideoEncoderBackend() = default;

	virtual const VideoEncoderInfo &info() const = 0;

	/// Encodes the given picture, which has to be of the size the encoder was opened with.
	/// @param timestamp  Capture time in microseconds. Has to increase with every call.
	/// @param keyFrame  Whether this picture has to be encoded as a key frame that decoders can start at.
	/// @param[out] packets  Packets that came out of the encoder are appended to this. Encoders may delay
	///     packets, so these do not necessarily belong to the picture just passed in.
	/// @returns Whether the picture was accepted
	virtual bool encode(const QImage &image, qint64 timestamp, bool keyFrame, std::vector< Packet > &packets) = 0;
};

namespace VideoEncoders {

/// All encoders that work on this system, best first.
///
/// Encoders are probed by actually opening them, since an encoder library may well support e.g. a GPU that isn't
/// there. This takes a while (up to a few seconds), so the first call should not happen on the GUI thread; see
/// startProbing(). The result is cached, and the function may be called from any thread.
const std::vector< VideoEncoderInfo > &available();

/// Starts probing the available encoders in the background, so that a later call to available() does not block.
void startProbing();

/// Opens the encoder with the given ID, or returns nullptr if that fails.
std::unique_ptr< VideoEncoderBackend > create(const QString &id, const VideoEncoderConfig &config);

} // namespace VideoEncoders

#endif // MUMBLE_MUMBLE_VIDEOENCODERBACKEND_H_
