// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_UTILS_H_
#define MUMBLE_TEST_TESTDATABASE_UTILS_H_

#include "database/Backend.h"
#include "database/ConnectionParameter.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

#include <nlohmann/json_fwd.hpp>

namespace mumble {
namespace db {
	namespace test {
		constexpr std::array< mumble::db::Backend, 0
#ifdef MUMBLE_TEST_SQLITE
													   + 1
#endif
#ifdef MUMBLE_TEST_MYSQL
													   + 1
#endif
#ifdef MUMBLE_TEST_POSTGRESQL
													   + 1
#endif
							  >
			backends = {
#ifdef MUMBLE_TEST_SQLITE
				mumble::db::Backend::SQLite,
#endif
#ifdef MUMBLE_TEST_MYSQL
				mumble::db::Backend::MySQL,
#endif
#ifdef MUMBLE_TEST_POSTGRESQL
				mumble::db::Backend::PostgreSQL,
#endif
			};

		namespace utils {

			// Implementations for find_permutation and apply_permutation taken/inspired from/by
			// https://stackoverflow.com/a/17074810
			template< typename VectorLike >
			std::vector< std::size_t > find_permutation(const VectorLike &reference, const VectorLike &target) {
				assert(reference.size() == target.size());
				assert(std::is_permutation(reference.begin(), reference.end(), target.begin()));

				std::vector< std::size_t > permutation(target.size());
				std::iota(permutation.begin(), permutation.end(), 0);

				assert(std::is_sorted(permutation.begin(), permutation.end()));

				do {
					bool matches = true;
					for (std::size_t i = 0; i < permutation.size(); ++i) {
						if (reference[i] != target[permutation[i]]) {
							matches = false;
							break;
						}
					}

					if (matches) {
						break;
					}
				} while (std::next_permutation(permutation.begin(), permutation.end()));

				return permutation;
			}

			template< typename VectorLike >
			void apply_permutation(VectorLike &vec, const std::vector< std::size_t > &permutation) {
				std::vector< bool > done(vec.size());
				for (std::size_t i = 0; i < vec.size(); ++i) {
					if (done[i]) {
						continue;
					}
					done[i]            = true;
					std::size_t prev_j = i;
					std::size_t j      = permutation[i];
					while (i != j) {
						std::swap(vec[prev_j], vec[j]);
						done[j] = true;
						prev_j  = j;
						j       = permutation[j];
					}
				}
			}

			/**
			 * Aligns the column order of the two provided JSON representation of a DB's content, so that
			 * the columns appear in the same order (if they were only a permutation of each other to begin with).
			 * All related fields (column_types and all row entries) are permuted accordingly to match the new
			 * column order.
			 *
			 * This function has to be called, before checking reference and target for equality, as the column
			 * order in our DB JSON representation is to some degree arbitrary, but the JSON objects compare
			 * them in an order-aware fashion (as they are stored as arrays).
			 */
			void alignColumnOrder(const nlohmann::json &reference, nlohmann::json &target);

			/**
			 * Same as alignColumnOrder, but this time it's the "rows" array of the different table specification
			 * that is brought into a consistent order (such that operator== returns true for tables containing the same
			 * content)
			 */
			void alignRowOrder(const nlohmann::json &reference, nlohmann::json &target);

			const mumble::db::ConnectionParameter &getConnectionParamter(mumble::db::Backend backend);

			/**
			 * Helper function to print exception messages that can also fully unfold nested exceptions
			 */
			inline void print_exception_message(const std::exception &e) {
				std::cerr << "  " << e.what() << "\n";
				try {
					std::rethrow_if_nested(e);
				} catch (const std::exception &nested) {
					print_exception_message(nested);
				}
			}


		} // namespace utils
	}     // namespace test
} // namespace db
} // namespace mumble



#define MUMBLE_BEGIN_TEST_CASE_NO_INIT                                                                              \
	try {                                                                                                           \
		for (::mumble::db::Backend currentBackend : mumble::db::test::backends) {                                   \
			qInfo() << "Current backend:" << QString::fromStdString(::mumble::db::backendToString(currentBackend)); \
			TestDB db(currentBackend);

#define MUMBLE_BEGIN_TEST_CASE     \
	MUMBLE_BEGIN_TEST_CASE_NO_INIT \
	db.init(::mumble::db::test::utils::getConnectionParamter(currentBackend));

#define MUMBLE_END_TEST_CASE                                 \
	}                                                        \
	}                                                        \
	catch (const std::exception &e) {                        \
		std::cerr << "Caught unexpected exception:\n";       \
		mumble::db::test::utils::print_exception_message(e); \
		QFAIL("Aborting due to thrown exception");           \
	}

#endif // MUMBLE_TEST_TESTDATABASE_UTILS_H_
