// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Column.h"

#include <cassert>

#include <boost/algorithm/string.hpp>

namespace mumble {
namespace db {

	Column::Column(const std::string &name, const DataType &type, const std::vector< Constraint > &constraints,
				   const std::string &defaultValue, Column::Flag flags)
		: m_type(type), m_constraints(constraints), m_default(defaultValue), m_flags(flags) {
		setName(name);
	}

	Column::Column(const std::string &name, const DataType &type, const std::vector< Constraint > &constraints,
				   Column::Flag flags)
		: Column(name, type, constraints, {}, flags) {}

	Column::Column(const std::string &name, const DataType &type, Column::Flag flags)
		: Column(name, type, {}, {}, flags) {}

	const std::string Column::getName() const { return m_name; }

	void Column::setName(const std::string &name) {
		assert(!boost::contains(name, " "));

		m_name = name;
	}

	const DataType &Column::getType() const { return m_type; }

	void Column::setType(const DataType &type) { m_type = type; }

	const std::string &Column::getDefaultValue() const { return m_default; }

	void Column::setDefaultValue(const std::string &defaultValue) { m_default = defaultValue; }

	bool Column::hasDefaultValue() const { return !m_default.empty(); }

	const std::vector< Constraint > &Column::getConstraints() const { return m_constraints; }

	void Column::setConstraints(const std::vector< Constraint > &constraints) { m_constraints = constraints; }

	void Column::addConstraint(const Constraint &constraint) { m_constraints.push_back(constraint); }

	void Column::clearConstraints() { m_constraints.clear(); }

	Column::Flag Column::getFlags() const { return m_flags; }

	bool Column::testFlag(Column::Flag flag) const { return m_flags & flag; }

	void Column::setFlags(Column::Flag flags) { m_flags = flags; }

	void Column::clearFlags() { m_flags = Column::Flag::NONE; }


} // namespace db
} // namespace mumble
