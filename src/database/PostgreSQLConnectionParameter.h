// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_POSTGRESQLCONNECTIONPARAMETER_H_
#define MUMBLE_DATABASE_POSTGRESQLCONNECTIONPARAMETER_H_

#include "database/Backend.h"
#include "database/ConnectionParameter.h"

#include <string>

namespace mumble {
namespace db {

	class PostgreSQLConnectionParameter : public ConnectionParameter {
	public:
		PostgreSQLConnectionParameter(const std::string &dbName);

		std::string dbName;
		std::string userName = "";
		std::string password = "";
		// Explicitly use localhost IP to circumvent peer authentication errors
		std::string host = "127.0.0.1";
		std::string port = "";

		virtual Backend applicability() const override;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_POSTGRESQLCONNECTIONPARAMETER_H_
