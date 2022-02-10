// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_CONSTRAINT_H_
#define MUMBLE_DATABASE_CONSTRAINT_H_

#include "Backend.h"

#include <string>

namespace mumble {
namespace db {

	class Table;
	class Column;

	class Constraint {
	public:
		enum Type {
			NotNull,
			Unique,
			PrimaryKey,
			ForeignKey,
		};

		Constraint(Type type, const std::string &name = {}, const Table *foreignTable = nullptr,
				   const Column *foreignColumn = nullptr);
		~Constraint() = default;

		Type getType() const;
		void setType(Type type);

		const std::string getName() const;
		void setName(const std::string &name);

		bool canInline() const;
		bool canOutOfLine() const;
		bool prefersInline() const;

		bool canBeDropped(Backend backend) const;

		std::string inlineSQL(Backend backend) const;
		std::string outOfLineSQL(const Column &column, Backend backend) const;

		std::string dropQuery(const Table &table, const Column &column, Backend backend) const;

	protected:
		Type m_type;
		std::string m_name;
		std::string m_foreignTableName;
		std::string m_foreignColumnName;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_CONSTRAINT_H_
