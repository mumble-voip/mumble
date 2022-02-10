// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_DATATYPE_H_
#define MUMBLE_DATABASE_DATATYPE_H_

#include "Exception.h"

#include <limits>
#include <string>

namespace mumble {
namespace db {

	struct UnknownDataTypeException : public Exception {
		using Exception::Exception;
	};

	class DataType {
	public:
		enum Type {
			Integer,
			SmallInteger,
			Double,
			FixedSizeString,
			String,
		};
		static constexpr const std::size_t Unsized = std::numeric_limits< std::size_t >::max();

		DataType(Type type = {}, std::size_t size = Unsized);
		~DataType() = default;

		Type getType() const;
		void setType(Type type);

		std::size_t getSize() const;
		void setSize(std::size_t size);
		bool isSized() const;

		bool canBeSized() const;
		bool canBeUnsized() const;

		static bool canBeSized(Type type);
		static bool canBeUnsized(Type type);

		std::string sqlRepresentation() const;
		static DataType fromSQLRepresentation(const std::string &strRepr);

	protected:
		Type m_type;
		std::size_t m_size;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_DATATYPE_H_
