// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_PRIMARYKEY_H_
#define MUMBLE_DATABASE_PRIMARYKEY_H_

#include <string>
#include <vector>

namespace mumble {
namespace db {

	class Column;

	class PrimaryKey {
	public:
		PrimaryKey();
		PrimaryKey(const Column &column);
		PrimaryKey(const std::vector< const Column * > &columns);
		PrimaryKey(const std::vector< std::string > &columnNames);
		~PrimaryKey() = default;

		const std::vector< std::string > &getColumnNames() const;
		void setColumnNames(const std::vector< std::string > &columnNames);

		bool isCompositeKey() const;

		bool isValid() const;

		std::string sql() const;

	protected:
		std::vector< std::string > m_columnNames;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_PRIMARYKEY_H_
