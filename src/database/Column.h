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
		Column(const std::string &name = {}, const DataType &type = {},
			   const std::vector< Constraint > &constraints = {}, const std::string &defaultValue = {});

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

	protected:
		std::string m_name;
		DataType m_type;
		std::vector< Constraint > m_constraints;
		std::string m_default;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_COLUMN_H_
