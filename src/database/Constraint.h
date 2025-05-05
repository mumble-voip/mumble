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
		};

		Constraint(Type type);
		~Constraint() = default;

		Type getType() const;
		void setType(Type type);

		std::string sql(Backend backend) const;

		friend bool operator==(const Constraint &lhs, const Constraint &rhs);
		friend bool operator!=(const Constraint &lhs, const Constraint &rhs);

	protected:
		Type m_type;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_CONSTRAINT_H_
