// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VideoEncoderBackend.h"

#include "FFmpegVideoEncoder.h"

#include <QtCore/QDebug>
#include <QtCore/QThread>

extern "C" {
#include <libavutil/log.h>
}

#include <mutex>

namespace VideoEncoders {

static std::once_flag s_probeOnce;
static std::vector< VideoEncoderInfo > s_available;

static void probeAll() {
	// Failing encoders (e.g. for a GPU that isn't there) tend to complain loudly, which is expected here
	const int logLevel = av_log_get_level();
	av_log_set_level(AV_LOG_FATAL);

	for (const VideoEncoderInfo &info : FFmpegVideoEncoder::candidates()) {
		if (FFmpegVideoEncoder::probe(info)) {
			s_available.push_back(info);
		}
	}

	av_log_set_level(logLevel);

	QStringList names;
	for (const VideoEncoderInfo &info : s_available) {
		names << info.name;
	}
	qInfo() << "Available video encoders:" << names;
}

const std::vector< VideoEncoderInfo > &available() {
	std::call_once(s_probeOnce, probeAll);
	return s_available;
}

void startProbing() {
	QThread *thread = QThread::create([]() { available(); });
	thread->setObjectName(QLatin1String("VideoEncoderProbe"));
	QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
	thread->start(QThread::LowPriority);
}

std::unique_ptr< VideoEncoderBackend > create(const QString &id, const VideoEncoderConfig &config) {
	for (const VideoEncoderInfo &info : available()) {
		if (info.id == id)
			return FFmpegVideoEncoder::open(info, config);
	}
	return nullptr;
}

} // namespace VideoEncoders
