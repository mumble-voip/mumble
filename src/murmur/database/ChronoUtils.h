// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_CHRONOUTILS_H_
#define MUMBLE_SERVER_DATABASE_CHRONOUTILS_H_

#include <cassert>
#include <chrono>

namespace mumble {
namespace server {
	namespace db {

		template< typename Clock, typename Duration >
		std::size_t toEpochSeconds(const std::chrono::time_point< Clock, Duration > &tp) {
			auto duration = std::chrono::duration_cast< std::chrono::seconds >(tp.time_since_epoch()).count();
			assert(duration > 0 || duration + 1 >= 1);
			return static_cast< std::size_t >(duration);
		}

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_CHRONOUTILS_H_
