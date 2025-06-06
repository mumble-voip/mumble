// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_USERPROPERTY_H_
#define MUMBLE_SERVER_DATABASE_USERPROPERTY_H_

namespace mumble {
namespace server {
	namespace db {

		/**
		 * An enum for all existing user properties
		 */
		enum class UserProperty : unsigned int {
			Name,
			Email,
			Comment,
			CertificateHash,
			Password,
			LastActive,
			kdfIterations,
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_USERPROPERTY_H_
