// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_COLUMN_H_
#define MUMBLE_DATABASE_COLUMN_H_

#include "Constraint.h"
#include "DataType.h"

#include <string>
#include <vector>

namespace mumble {
namespace db {

	class Column {
	public:
		enum Flag {
			NONE          = 0,
			AUTOINCREMENT = 1 << 0,
		};

		Column(const std::string &name = {}, const DataType &type = {},
			   const std::vector< Constraint > &constraints = {}, const std::string &defaultValue = {},
			   Flag flags = Flag::NONE);
		Column(const std::string &name, const DataType &type,
			   const std::vector< Constraint > &constraints, Flag flags);
		Column(const std::string &name, const DataType &type, Flag flags);

		const std::string getName() const;
		void setName(const std::string &name);

		const DataType &getType() const;
		void setType(const DataType &type);

		const std::string &getDefaultValue() const;
		void setDefaultValue(const std::string &defaultValue);
		bool hasDefaultValue() const;

		const std::vector< Constraint > &getConstraints() const;
		void setConstraints(const std::vector< Constraint > &constraints);
		void addConstraint(const Constraint &constraint);
		void clearConstraints();

		Flag getFlags() const;
		bool testFlag(Flag flag) const;
		void setFlags(Flag flags);
		void clearFlags();

	protected:
		std::string m_name;
		DataType m_type;
		std::vector< Constraint > m_constraints;
		std::string m_default;
		Flag m_flags;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_COLUMN_H_
