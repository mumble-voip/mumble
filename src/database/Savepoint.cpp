// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Savepoint.h"

#include <cassert>

namespace mumble::db {

Savepoint::Savepoint(soci::session &sql, std::string name) : m_active(true), m_name(std::move(name)), m_sql(sql) {
	assert(!m_name.empty());
	assert(m_name.find(' ') == std::string::npos);
	assert(m_name.find('\t') == std::string::npos);
	assert(m_name.find('\n') == std::string::npos);
	assert(m_name.find('\r') == std::string::npos);

	m_sql << "SAVEPOINT " << m_name;
}

Savepoint::~Savepoint() {
	if (m_active) {
		// Wrapper went out of scope without explicit handling -> rollback as this indicates an error has happened
		// We normally ignore any exception that might be throw here in order to not throw from the dtor
		try {
			rollback();
		} catch (...) {
			// In debug builds (or more general: builds with assertions enabled) we want to abort here as an error
			// encountered during rollback of a transaction indicates a bug of some sort, that we should
			// investigate.
			assert(false);
		}
	}
}

void Savepoint::release() {
	assert(m_active);

	m_sql << "RELEASE SAVEPOINT " << m_name;

	m_active = false;
}

void Savepoint::rollback() {
	assert(m_active);

	m_sql << "ROLLBACK TO SAVEPOINT " << m_name;

	m_active = false;
}

const std::string &Savepoint::getName() const {
	return m_name;
}

} // namespace mumble::db
