// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_SAVEPOINT_H_
#define MUMBLE_DATABASE_SAVEPOINT_H_

#include "NonCopyable.h"

#include <soci/soci.h>

#include <string>

namespace mumble::db {

/**
 * RAII wrapper for managing savepoints (subtransactions). The wrapper will automatically roll back to
 * the point where the savepoint was created if it goes out of scope without explicitly having
 * been released.
 */
class Savepoint : public NonCopyable {
public:
	Savepoint(soci::session &sql, std::string name);
	~Savepoint();

	void release();
	void rollback();

	const std::string &getName() const;

private:
	bool m_active = false;
	std::string m_name;
	soci::session &m_sql;
};

} // namespace mumble::db

#endif // MUMBLE_DATABASE_SAVEPOINT_H_
