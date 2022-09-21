// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_USERPROPERTYTABLE_H_
#define MUMBLE_SERVER_DATABASE_USERPROPERTYTABLE_H_

#include "DBUser.h"
#include "UserProperty.h"

#include "StringConverter.h"

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/NoDataException.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class UserTable;

		/**
		 * Table for storing optional properties of channels. Optional in the sense that a given channel might or might
		 * not have a value set for the given property.
		 */
		class UserPropertyTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "user_properties";

			struct column {
				column()                               = delete;
				static constexpr const char *server_id = "server_id";
				static constexpr const char *user_id   = "user_id";
				static constexpr const char *key       = "property_key";
				static constexpr const char *value     = "property_value";
			};

			UserPropertyTable(soci::session &sql, ::mumble::db::Backend backend, const UserTable &userTable);
			~UserPropertyTable() = default;

			template< typename T, bool throwOnError = true >
			T getProperty(const DBUser &user, UserProperty property, const T &defaultValue = {}) {
				bool success = false;
				bool gotData = true;
				T val        = defaultValue;
				try {
					std::string strProp = doGetProperty(user, property);
					val                 = StringConverter< T >::convert(strProp, &success);
				} catch (const ::mumble::db::NoDataException &) {
					gotData = false;
				}

				if (!success) {
					if (throwOnError) {
						std::string suffix = !gotData ? " (fetched property turned out to be non-existent)" : "";
						throw ::mumble::db::AccessException(
							"Failed to perform type conversion for property "
							+ std::to_string(static_cast< int >(property)) + " for user with ID "
							+ std::to_string(user.registeredUserID) + " on server with ID "
							+ std::to_string(user.serverID) + suffix);
					} else {
						return defaultValue;
					}
				}

				return val;
			}

			bool isPropertySet(const DBUser &user, UserProperty property);

			void setProperty(const DBUser &user, UserProperty property, const std::string &value);

			void clearProperty(const DBUser &user, UserProperty property);

			void clearAllProperties(const DBUser &user);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;

		protected:
			std::string doGetProperty(const DBUser &user, UserProperty property);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_USERPROPERTYTABLE_H_
