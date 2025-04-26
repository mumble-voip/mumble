// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_METATABLE_H_
#define MUMBLE_DATABASE_METATABLE_H_

#include "Backend.h"
#include "Table.h"

#include <string>

#include <boost/optional.hpp>

namespace mumble {
namespace db {

	class MetaTable : public Table {
	public:
		static constexpr const char *NAME = "meta";
		MetaTable(soci::session &sql, Backend backend);

		unsigned int getSchemeVersion();
		void setSchemeVersion(unsigned int version);

		void setKey(const std::string &key, const std::string &value);
		boost::optional< std::string > queryKey(const std::string &key);
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_METATABLE_H_
