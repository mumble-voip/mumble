// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Constraint.h"
#include "Backend.h"
#include "Column.h"
#include "DataType.h"
#include "Table.h"

#include <algorithm>
#include <cassert>

namespace mumble {
namespace db {

	Constraint::Constraint(Constraint::Type type) : m_type(type) {}

	Constraint::Type Constraint::getType() const { return m_type; }

	void Constraint::setType(Constraint::Type type) { m_type = type; }

	std::string Constraint::sql(Backend) const {
		switch (m_type) {
			case Constraint::NotNull:
				return "NOT NULL";
			case Constraint::Unique:
				return "UNIQUE";
		}

		// This code should never be reached
		assert(false);
		return "";
	}

	bool operator==(const Constraint &lhs, const Constraint &rhs) { return lhs.m_type == rhs.m_type; }

	bool operator!=(const Constraint &lhs, const Constraint &rhs) { return !(lhs == rhs); }

} // namespace db
} // namespace mumble
