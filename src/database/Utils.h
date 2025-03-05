// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_UTILS_H_
#define MUMBLE_DATABASE_UTILS_H_

#include "Backend.h"
#include "NoDataException.h"

#include <soci/soci.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace mumble {
namespace db {
	namespace utils {

		/**
		 * Tests the given SQL session to see if the last query resulted in any data having been fetched. If not,
		 * this function will throw an exception of the given type.
		 *
		 * @tparam Exception The type of the exception that is to be thrown
		 */
		template< typename Exception = NoDataException > void verifyQueryResultedInData(soci::session &sql) {
			if (!sql.got_data()) {
				std::string msg =
					"Query did not result in any data having been fetched from the database: " + sql.get_last_query();
				std::string context = sql.get_last_query_context();
				if (!context.empty()) {
					msg += " with ";
					msg += context;
				}

				throw Exception(std::move(msg));
			}
		}

		/**
		 * @returns A SQL statement that will raise an error that can be used inside a trigger to signal an error.
		 */
		std::string triggerErrorStatement(Backend backend, const std::string &errorMsg);

		struct ColAlias {
			std::string colName  = {};
			std::string colAlias = {};

			ColAlias(std::string colName);
			ColAlias(std::string colName, std::string colAlias);
		};

		std::string getLowestUnoccupiedIDStatement(Backend backend, const std::string &tableName,
												   const std::string &idColName,
												   const std::vector< ColAlias > &additionalCols = {});

		/**
		 * @returns The SQL statement for the respective backend to achieve limiting a query to a given
		 *  	size and optionally also starting at a specific offset.
		 */
		std::string limitOffset(Backend backend, const std::string &limit, const std::string &offset = "0");

		namespace details {

			/**
			 * Wrapper class that makes writing COALESCE SQL statements (with only two choices) more readable.
			 */
			struct CoalesceHelper {
			public:
				CoalesceHelper(const char *baseValue);

				/**
				 * Provide the fallback value, if the base value turns out to be NULL
				 *
				 * @returns The SQL query that does the job (COALESCE)
				 */
				std::string otherwise(const std::string &fallbackValue);

			private:
				const char *m_baseValue = nullptr;
			};
		} // namespace details

		/**
		 * Produces a query that selects the given value, if it is not NULL. The default value to
		 * use in the cases if the selected value does indeed turn out to be NULL, must be provided
		 * by calling the "otherwise" function on the returned object.
		 */
		details::CoalesceHelper nonNullOf(const std::string &value);

		/**
		 * Produces a query that transforms the given value from a date(time) representation
		 * into a unix epoch timestamp (in seconds)
		 *
		 * @param value The value (or the name of a field) to convert
		 * @param backend The currently used database backend
		 * @param autocast Whether to automatically cast the given value to a suitable data
		 * type instead of letting the backend deduce the type.
		 */
		std::string dateToEpoch(const std::string &value, Backend backend, bool autocast = false);

		namespace details {
			template< typename Container > struct Resizer {
				static void resize(Container &container, std::size_t size) { container.resize(size); }

				static bool canResize() { return true; }
			};

			template< typename Type, std::size_t size > struct Resizer< std::array< Type, size > > {
				static void resize(std::array< Type, size > &, std::size_t newSize) {
					assert(newSize == size);
					if (newSize != size) {
						throw std::runtime_error("Can't resize a std::array");
					}
				}

				static bool canResize() { return false; }
			};
		} // namespace details

#define FAIL_CONVERSION                               \
	std::fill(container.begin(), container.end(), 0); \
	if (success) {                                    \
		*success = false;                             \
	}                                                 \
	return;

		template< typename Container >
		static void hexToBinary(const std::string &hex, Container &container, bool *success = nullptr) {
			static_assert(std::is_same< typename Container::value_type, std::uint8_t >::value,
						  "Container required to store bytes");

			std::size_t offset = hex.size() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X') ? 2 : 0;

			if (hex.size() % 2 != 0) {
				FAIL_CONVERSION
			}

			std::size_t byteSize = (hex.size() - offset) / 2;

			// Ensure container has correct size
			if (details::Resizer< Container >::canResize()) {
				details::Resizer< Container >::resize(container, byteSize);
			} else if (container.size() != byteSize) {
				FAIL_CONVERSION
			}

			for (std::size_t i = 1; i <= byteSize; ++i) {
				std::size_t processedChars = 0;
				try {
					container[byteSize - i] = static_cast< typename Container::value_type >(
						std::stoi(hex.substr(hex.size() - 2 * i, 2), &processedChars, 16));
				} catch (std::exception &) {
					FAIL_CONVERSION
				}

				if (processedChars != 2) {
					FAIL_CONVERSION
				}
			}

			if (success) {
				*success = true;
			}
		}
#undef FAIL_CONVERSION

		template< typename Container > static Container hexToBinary(const std::string &hex, bool *success = nullptr) {
			Container bytes;

			hexToBinary(hex, bytes, success);

			return bytes;
		}

		template< std::size_t byteSize >
		static std::array< std::uint8_t, byteSize > hexToBinary(const std::string &hex, bool *success = nullptr) {
			return hexToBinary< std::array< std::uint8_t, byteSize > >(hex, success);
		}

		template< typename Container > std::string binaryToHex(const Container &container) {
			static_assert(std::is_same< typename Container::value_type, std::uint8_t >::value,
						  "Container required to store bytes");

			std::stringstream sstream;
			sstream << std::hex << "0x";

			for (std::uint8_t currentByte : container) {
				// Prevent interpretation as char
				sstream << std::setfill('0') << std::setw(2) << std::nouppercase
						<< static_cast< unsigned int >(currentByte);
			}

			std::string hex = sstream.str();

			// We encode every byte with two characters and prefix the whole thing with '0x'
			assert(hex.size() == container.size() * 2 + 2);

			return hex;
		}


	} // namespace utils
} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_UTILS_H_
