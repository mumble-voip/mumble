// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_CHANNELPROPERTYTABLE_H_
#define MUMBLE_SERVER_DATABASE_CHANNELPROPERTYTABLE_H_

#include "ChannelProperty.h"

#include "StringConverter.h"

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/NoDataException.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace server {
	namespace db {

		class ChannelTable;

		/**
		 * Table for storing optional properties of channels. Optional in the sense that a given channel might or might
		 * not have a value set for the given property.
		 */
		class ChannelPropertyTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "channel_properties";

			struct column {
				column()                                = delete;
				static constexpr const char *server_id  = "server_id";
				static constexpr const char *channel_id = "channel_id";
				static constexpr const char *key        = "property_key";
				static constexpr const char *value      = "property_value";
			};

			ChannelPropertyTable(soci::session &sql, ::mumble::db::Backend backend, const ChannelTable &channelTable);
			~ChannelPropertyTable() = default;

			template< typename T, bool throwOnError = true >
			T getProperty(unsigned int serverID, unsigned int channelID, ChannelProperty property,
						  const T &defaultValue = {}) {
				bool success = false;
				bool gotData = true;
				T val        = defaultValue;
				try {
					std::string strProp = doGetProperty(serverID, channelID, property);
					val                 = StringConverter< T >::convert(strProp, &success);
				} catch (const ::mumble::db::NoDataException &) {
					gotData = false;
				}

				if (!success) {
					if (throwOnError) {
						std::string suffix = !gotData ? " (fetched property turned out to be empty/non-existent)" : "";
						throw ::mumble::db::AccessException(
							"Failed to perform type conversion for property "
							+ std::to_string(static_cast< int >(property)) + " for channel with ID "
							+ std::to_string(channelID) + " on server with ID " + std::to_string(serverID) + suffix);
					} else {
						return defaultValue;
					}
				}

				return val;
			}

			bool isPropertySet(unsigned int serverID, unsigned int channelID, ChannelProperty property);

			void setProperty(unsigned int serverID, unsigned int channelID, ChannelProperty property,
							 const std::string &value);

			void clearProperty(unsigned int serverID, unsigned int channelID, ChannelProperty property);

			void clearAllProperties(unsigned int serverID, unsigned int channelID);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;

		protected:
			std::string doGetProperty(unsigned int serverID, unsigned int channelID, ChannelProperty property);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_CHANNELPROPERTYTABLE_H_
