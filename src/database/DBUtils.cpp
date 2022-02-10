// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBUtils.h"

#include <soci/soci.h>

#include <nlohmann/json.hpp>

#include <cassert>
#include <iostream>

namespace mumble {
namespace db {
	namespace utils {

		std::string to_string(const soci::row &row, std::size_t columnIndex) {
			const soci::column_properties &props = row.get_properties(columnIndex);

			switch (props.get_data_type()) {
				case soci::dt_string:
					return row.get< std::string >(columnIndex);
				case soci::dt_double:
					return std::to_string(row.get< double >(columnIndex));
				case soci::dt_integer:
					return std::to_string(row.get< int >(columnIndex));
				case soci::dt_long_long:
					return std::to_string(row.get< long long >(columnIndex));
				case soci::dt_unsigned_long_long:
					return std::to_string(row.get< unsigned long long >(columnIndex));
				case soci::dt_date:
				case soci::dt_xml:
				case soci::dt_blob:
					std::cerr << "[ERROR]: Tried to convert unsupported DB datatype to string: "
							  << props.get_data_type() << std::endl;
					std::abort();
			}

			assert(false);
			return "";
		}

		std::string to_string(const nlohmann::json &json) {
			switch (json.type()) {
				case nlohmann::json::value_t::string:
					// For strings the nlohmann::to_string method would add extra quotes around the actual string,
					// which is not what we want. Thus, we have to handle them here explicitly.
					return json.get< std::string >();
				default:
					return nlohmann::to_string(json);
			}
		}

		nlohmann::json to_json(const soci::row &row, std::size_t columnIndex) {
			const soci::column_properties &props = row.get_properties(columnIndex);

			switch (props.get_data_type()) {
				case soci::dt_string:
					return row.get< std::string >(columnIndex);
				case soci::dt_double:
					return row.get< double >(columnIndex);
				case soci::dt_integer:
					return row.get< int >(columnIndex);
				case soci::dt_long_long:
					return row.get< long long >(columnIndex);
				case soci::dt_unsigned_long_long:
					return row.get< unsigned long long >(columnIndex);
				case soci::dt_date:
				case soci::dt_xml:
				case soci::dt_blob:
					std::cerr << "[ERROR]: Tried to convert unsupported DB datatype to JSON: " << props.get_data_type()
							  << std::endl;
					std::abort();
			}

			assert(false);
			return "";
		}

	} // namespace utils
} // namespace db
} // namespace mumble
