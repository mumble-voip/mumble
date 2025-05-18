// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_METATABLE_H_
#define MUMBLE_DATABASE_METATABLE_H_

#include "Backend.h"
#include "Table.h"

#include <optional>
#include <string>

namespace mumble {
namespace db {

	class MetaTable : public Table {
	public:
		friend class Database;

		static constexpr const char *NAME = "meta";

		struct column {
			column()                           = delete;
			static constexpr const char *key   = "meta_key";
			static constexpr const char *value = "meta_value";
		};

		MetaTable(soci::session &sql, Backend backend);

		unsigned int getSchemeVersion();
		void setSchemeVersion(unsigned int version);

		void setKey(const std::string &key, const std::string &value);
		std::optional< std::string > queryKey(const std::string &key);

		void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;

	private:
		unsigned int getSchemeVersionLegacy();
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_METATABLE_H_
