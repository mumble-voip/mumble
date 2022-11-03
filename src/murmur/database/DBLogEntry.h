// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBLOGENTRY_H_
#define MUMBLE_SERVER_DATABASE_DBLOGENTRY_H_

#include <chrono>
#include <string>

namespace mumble {
namespace server {
	namespace db {

		/**
		 * A small struct representing what the database needs to know/knows about a log entry
		 */
		struct DBLogEntry {
			using timestamp_type = std::chrono::time_point< std::chrono::steady_clock >;

			DBLogEntry() = default;
			DBLogEntry(std::string msg, timestamp_type timestamp = std::chrono::steady_clock::now())
				: message(std::move(msg)), timestamp(std::move(timestamp)) {}

			std::string message;
			timestamp_type timestamp = std::chrono::steady_clock::now();
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBLOGENTRY_H_
