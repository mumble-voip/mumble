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
#include "database/Table.h"

#include <string>

namespace soci {
class session;
};

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

			ChannelPropertyTable(soci::session &sql, ::mumble::db::Backend backend, const ChannelTable &channelTable);
			~ChannelPropertyTable() = default;

			template< typename T, bool throwOnError = true >
			T getProperty(unsigned int serverID, unsigned int channelID, ChannelProperty property,
						  const T &defaultValue = {}) {
				std::string strProp = doGetProperty(serverID, channelID, property);

				bool success = false;
				T val        = StringConverter< T >::convert(strProp, &success);

				if (!success) {
					if (throwOnError) {
						throw ::mumble::db::AccessException("Failed to perform type conversion for property "
															+ std::to_string(static_cast< int >(property))
															+ " for channel with ID " + std::to_string(channelID)
															+ " on server with ID " + std::to_string(serverID));
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
