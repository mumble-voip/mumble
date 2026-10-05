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

#include <algorithm>
#include <atomic>
#include <cstdarg>
#include <mutex>

namespace VideoEncoders {

static std::once_flag s_probeOnce;
// Never destroyed, as probing may still be running on its own thread while Mumble exits
static std::vector< VideoEncoderInfo > &s_available = *new std::vector< VideoEncoderInfo >();

/// Set on the thread that probes the encoders while it does so
static thread_local bool t_probing = false;

static void logCallback(void *avcl, int level, const char *fmt, va_list vl) {
	// Failing encoders (e.g. for a GPU that isn't there) tend to complain loudly, which is expected while probing
	if (t_probing && level > AV_LOG_FATAL)
		return;
	av_log_default_callback(avcl, level, fmt, vl);
}

static void probeAll() {
	// FFmpeg's log level is global, so only messages from this thread are filtered. The callback stays installed,
	// as other threads may be logging through it at any time.
	av_log_set_callback(&logCallback);
	t_probing = true;

	for (const VideoEncoderInfo &info : FFmpegVideoEncoder::candidates()) {
		if (FFmpegVideoEncoder::probe(info)) {
			s_available.push_back(info);
		}
	}

	t_probing = false;

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
	// The result is cached, so probing once is enough
	static std::atomic_bool s_started = false;
	if (s_started.exchange(true))
		return;

	QThread *thread = QThread::create([]() { available(); });
	thread->setObjectName(QLatin1String("VideoEncoderProbe"));
	QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
	thread->start(QThread::LowPriority);
}

QStringList order(const std::vector< VideoEncoderInfo > &encoders, const VideoEncoderSelection &selection) {
	std::vector< const VideoEncoderInfo * > ordered;
	for (const VideoEncoderInfo &info : encoders) {
		ordered.push_back(&info);
	}

	switch (selection.mode) {
		case VideoEncoderMode::Best:
			break;
		case VideoEncoderMode::Optimised: {
			auto viewerCount = [&selection](const VideoEncoderInfo *info) {
				const unsigned int codec = static_cast< unsigned int >(info->codec);
				return std::count_if(selection.viewerDecoders.begin(), selection.viewerDecoders.end(),
									 [codec](const std::vector< unsigned int > &decoders) {
										 return std::find(decoders.begin(), decoders.end(), codec) != decoders.end();
									 });
			};
			std::stable_sort(ordered.begin(), ordered.end(),
							 [&viewerCount](const VideoEncoderInfo *lhs, const VideoEncoderInfo *rhs) {
								 return viewerCount(lhs) > viewerCount(rhs);
							 });
			break;
		}
		case VideoEncoderMode::Manual:
			std::stable_partition(ordered.begin(), ordered.end(), [&selection](const VideoEncoderInfo *info) {
				return info->id == selection.manualEncoder;
			});
			break;
	}

	QStringList ids;
	for (const VideoEncoderInfo *info : ordered) {
		ids << info->id;
	}
	return ids;
}

std::unique_ptr< VideoEncoderBackend > create(const QString &id, const VideoEncoderConfig &config) {
	for (const VideoEncoderInfo &info : available()) {
		if (info.id == id)
			return FFmpegVideoEncoder::open(info, config);
	}
	return nullptr;
}

} // namespace VideoEncoders
