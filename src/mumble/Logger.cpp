// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Logger.h"

#include "Global.h"

#include <cstdlib>

#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/sinks/qt_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using MasterSink = spdlog::sinks::dup_filter_sink_st;
using QtSink     = spdlog::sinks::qt_color_sink_st;
using FileSink   = spdlog::sinks::rotating_file_sink_st;
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

#ifdef NDEBUG
static void addFileSink(MasterSink &parent) {
	constexpr auto maxFiles = 3;
	constexpr auto maxSize  = 5 * 1024 * 1024; // 5 MiB
#	ifndef Q_OS_MACOS
	const auto filePath = Global::get().qdBasePath.filePath(QLatin1String("Console.txt")).toStdString();
	parent.add_sink(std::make_shared< FileSink >(filePath, maxSize, maxFiles));
#	else
	std::string filePath = "/Library/Logs/Mumble.log";
	if (const char *homePath = std::getenv("HOME")) {
		filePath.insert(0, homePath);
		parent.add_sink(std::make_shared< FileSink >(filePath, maxSize, maxFiles));
	}
#	endif
#else
static void addFileSink(MasterSink &) {
#endif
}

void log::init(QTextEdit *textBox) {
	// Skips the message if the previous one is identical and less than 5 seconds have passed.
	masterSink = std::make_shared< MasterSink >(std::chrono::seconds(5));
#ifdef Q_OS_WIN
	masterSink->add_sink(std::make_shared< DebuggerSink >());
#endif
	masterSink->add_sink(std::make_shared< StdOutSink >());

	addFileSink(*masterSink);

	if (textBox) {
		// Only display up to 1000 lines.
		masterSink->add_sink(std::make_shared< QtSink >(textBox, 1000));
	}

	auto logger = std::make_shared< spdlog::logger >("Main", masterSink);

	// TODO: Ideally we should add a user option, perhaps along with a launch parameter, to set the log level.
	// However, the messages across the codebase are very inconsistent in terms of right now.
	// For example, there are a lot of debug messages that should be info instead.
	// Thus, for the time being let's show all messages regardless of their advertised level.
	logger->set_level(spdlog::level::trace);
	logger->set_pattern("%^<%L>%$%Y-%m-%d %H:%M:%S.%e %v");

	set_default_logger(std::move(logger));

	qInstallMessageHandler(qtMessageHandler);
}

void log::prepareToExit() {
	debug("Preparing to exit...");

	auto &sinks = masterSink->sinks();

	// We iterate through the (sub)sinks to find the Qt one (if present).
	// It must be removed because the widget it writes into is going to be deleted soon.
	for (auto iter = sinks.cbegin(); iter != sinks.cend(); ++iter) {
		auto textBoxSink = std::dynamic_pointer_cast< QtSink >(*iter);
		if (textBoxSink) {
			masterSink->sinks().erase(iter);
			return;
		}
	}
}
