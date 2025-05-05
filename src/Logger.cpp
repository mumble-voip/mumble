// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Logger.h"

#include <QString>
#include <QtGlobal>

#include <cstdlib>

#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using MasterSink = spdlog::sinks::dup_filter_sink_st;
using StdOutSink = spdlog::sinks::stdout_color_sink_st;

#ifdef Q_OS_WIN
#	include <spdlog/sinks/msvc_sink.h>

using DebuggerSink = spdlog::sinks::msvc_sink_st;
#endif

using namespace mumble;

static std::shared_ptr< MasterSink > masterSink;

static void qtMessageHandler(const QtMsgType type, const QMessageLogContext &, const QString &msg) {
	switch (type) {
		default:
			log::trace("{}", qPrintable(msg));
			break;
		case QtDebugMsg:
			log::debug("{}", qPrintable(msg));
			break;
		case QtInfoMsg:
			log::info("{}", qPrintable(msg));
			break;
		case QtWarningMsg:
			log::warn("{}", qPrintable(msg));
			break;
		case QtCriticalMsg:
			log::error("{}", qPrintable(msg));
			break;
		case QtFatalMsg:
			log::fatal("{}", qPrintable(msg));
			break;
	}
}

void log::addSink(std::shared_ptr< spdlog::sinks::sink > sink) {
	if (!masterSink) {
		log::fatal("Attempted to addSink before master sink has been initialized");
		return;
	}

	sink->set_pattern("%^<%L>%$%Y-%m-%d %H:%M:%S.%e %v");

	masterSink->add_sink(std::move(sink));
}

void log::init(spdlog::level::level_enum logLevel) {
	// Skips the message if the previous one is identical and less than 5 seconds have passed.
	masterSink = std::make_shared< MasterSink >(std::chrono::seconds(5));
#ifdef Q_OS_WIN
	addSink(std::make_shared< DebuggerSink >());
#endif
	addSink(std::make_shared< StdOutSink >());

	auto logger = std::make_shared< spdlog::logger >(MainLoggerName, masterSink);

	logger->set_level(logLevel);

	set_default_logger(std::move(logger));

	qInstallMessageHandler(qtMessageHandler);
}
