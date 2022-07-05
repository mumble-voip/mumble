// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_INDEX_H_
#define MUMBLE_DATABASE_INDEX_H_

#include "Backend.h"

#include <string>
#include <vector>

namespace mumble {
namespace db {

	class Table;

	class Index {
	public:
		enum Flag {
			NONE = 0,
			UNIQUE = 1 << 0,
		};

		Index(const std::string &name, const std::vector< std::string > &columns, int flags = NONE);
		Index(const std::string &name, std::vector< std::string > &&columns, int flags);

		const std::string &getName() const;
		const std::vector< std::string > &getColumnNames() const;

		std::string creationQuery(const Table &table, Backend backend) const;
		std::string dropQuery(const Table &table, Backend backend) const;

		friend bool operator==(const Index &lhs, const Index &rhs);
		friend bool operator!=(const Index &lhs, const Index &rhs);

	protected:
		std::string m_name;
		std::vector< std::string > m_columns;
		int m_flags = 0;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_INDEX_H_
