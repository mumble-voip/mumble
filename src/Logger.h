// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_LOGGER_H_
#define MUMBLE_LOGGER_H_

#include <memory>

#include <spdlog/spdlog.h>

class QTextEdit;

namespace mumble {
namespace log {
	void init(QTextEdit *textBox = nullptr);
	void prepareToExit();

	template< typename... Args > static void inline trace(spdlog::format_string_t< Args... > fmt, Args &&... args) {
		spdlog::trace(fmt, std::forward< Args >(args)...);
	}

	template< typename... Args > static void inline debug(spdlog::format_string_t< Args... > fmt, Args &&... args) {
		spdlog::debug(fmt, std::forward< Args >(args)...);
	}

	template< typename... Args > static void inline info(spdlog::format_string_t< Args... > fmt, Args &&... args) {
		spdlog::info(fmt, std::forward< Args >(args)...);
	}

	template< typename... Args > static void inline warn(spdlog::format_string_t< Args... > fmt, Args &&... args) {
		spdlog::warn(fmt, std::forward< Args >(args)...);
	}

	template< typename... Args > static void inline error(spdlog::format_string_t< Args... > fmt, Args &&... args) {
		spdlog::error(fmt, std::forward< Args >(args)...);
	}

	template< typename... Args > static void inline fatal(spdlog::format_string_t< Args... > fmt, Args &&... args) {
		spdlog::critical(fmt, std::forward< Args >(args)...);
		std::exit(1);
	}
} // namespace log
} // namespace mumble

#endif
