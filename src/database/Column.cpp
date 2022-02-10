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
				   const std::string &defaultValue)
		: m_type(type), m_constraints(constraints), m_default(defaultValue) {
		setName(name);
	}

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


} // namespace db
} // namespace mumble
