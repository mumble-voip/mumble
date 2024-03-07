// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_CONNECTIONPARAMETER_H_
#define MUMBLE_DATABASE_CONNECTIONPARAMETER_H_

#include "database/Backend.h"

#include <string>

namespace mumble {
namespace db {

	struct ConnectionParameter {
		ConnectionParameter()          = default;
		virtual ~ConnectionParameter() = default;

		virtual Backend applicability() const = 0;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_CONNECTIONPARAMETER_H_
