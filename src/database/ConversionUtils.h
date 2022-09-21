// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_CONVERSIONUTILS_H_
#define MUMBLE_DATABASE_CONVERSIONUTILS_H_

#include <string>

#include <nlohmann/json_fwd.hpp>

namespace soci {
class row;
}

namespace mumble {
namespace db {
	namespace utils {

		std::string to_string(const soci::row &row, std::size_t columnIndex);
		std::string to_string(const nlohmann::json &json);

		nlohmann::json to_json(const soci::row &row, std::size_t columnIndex);

	} // namespace utils
} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_CONVERSIONUTILS_H_
