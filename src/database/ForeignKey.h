// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_FOREIGNKEY_H_
#define MUMBLE_DATABASE_FOREIGNKEY_H_

#include <string>
#include <vector>

namespace mumble {
namespace db {

	class Table;
	class Column;

	class ForeignKey {
	public:
		ForeignKey();
		ForeignKey(const Table &foreignTable, const std::vector< const Column * > &columns);
		ForeignKey(const Table &foreignTable, const std::vector< Column > &columns);
		~ForeignKey() = default;

		const std::vector< std::string > &getForeignColumnNames() const;
		const std::vector< std::string > &getColumnNames() const;
		const std::string &getForeignTableName() const;

		void setColumns(const Table &foreignTable, const std::vector< Column > &columns);
		void setColumns(const Table &foreignTable, const std::vector< const Column * > &columns);

		std::string sql() const;

		friend bool operator==(const ForeignKey &lhs, const ForeignKey &rhs);
		friend bool operator!=(const ForeignKey &lhs, const ForeignKey &rhs);

	protected:
		std::vector< std::string > m_foreignColumnNames;
		std::vector< std::string > m_columnNames;
		std::string m_foreignTableName;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_FOREIGNKEY_H_
