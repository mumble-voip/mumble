// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DataType.h"
#include "Backend.h"

#include <boost/algorithm/string.hpp>

#include <cassert>

namespace mumble {
namespace db {

	constexpr const std::size_t DataType::Unsized;


	DataType::DataType(DataType::Type type, std::size_t size) : m_type(type), m_size(size) {
		assert((size == Unsized && canBeUnsized()) || (size != Unsized && canBeSized()));
	}

	DataType::Type DataType::getType() const { return m_type; }

	void DataType::setType(DataType::Type type) { m_type = type; }

	std::size_t DataType::getSize() const { return m_size; }

	void DataType::setSize(std::size_t size) { m_size = size; }

	bool DataType::isSized() const { return m_size != DataType::Unsized; }

	bool DataType::canBeSized() const { return canBeSized(m_type); }

	bool DataType::canBeSized(DataType::Type type) {
		switch (type) {
			case DataType::Integer:
				return false;
			case DataType::SmallInteger:
				return false;
			case DataType::Double:
				return false;
			case DataType::FixedSizeString:
				return true;
			case DataType::String:
				return true;
		}

		// This code should be unreachable
		assert(false);
		return false;
	}

	bool DataType::canBeUnsized() const { return canBeUnsized(m_type); }

	bool DataType::canBeUnsized(DataType::Type type) {
		switch (type) {
			case DataType::Integer:
				return true;
			case DataType::SmallInteger:
				return true;
			case DataType::Double:
				return true;
			case DataType::FixedSizeString:
				return false;
			case DataType::String:
				return false;
		}

		// This code should be unreachable
		assert(false);
		return false;
	}

	std::string DataType::sqlRepresentation() const {
		std::string sqlRepr;

		switch (m_type) {
			case DataType::Integer:
				sqlRepr = "INTEGER";
				break;
			case DataType::SmallInteger:
				sqlRepr = "SMALLINT";
				break;
			case DataType::Double:
				sqlRepr = "DOUBLE PRECISION";
				break;
			case DataType::FixedSizeString:
				sqlRepr = "CHAR";
				break;
			case DataType::String:
				sqlRepr = "VARCHAR";
				break;
		}

		assert(!sqlRepr.empty());

		if (isSized()) {
			sqlRepr += "(" + std::to_string(m_size) + ")";
		}

		return sqlRepr;
	}

	DataType DataType::fromSQLRepresentation(const std::string &strRepr) {
		std::string name;

		std::size_t i = 0;
		for (; i < strRepr.size(); ++i) {
			if (strRepr[i] == '(') {
				break;
			}

			name += strRepr[i];
		}

		std::size_t size = DataType::Unsized;
		if (strRepr[i] == '(') {
			if (strRepr[strRepr.size() - 1] != ')' || strRepr.size() - i < 3) {
				// If we found a '(', we expect at least something in the form of "(1)"
				throw UnknownDataTypeException("Malformed data type size in \"" + strRepr + "\"");
			}

			try {
				size = std::stoi(strRepr.substr(i + 1, strRepr.size() - 1));
			} catch (const std::invalid_argument &e) {
				throw UnknownDataTypeException("Size of data type \"" + strRepr
											   + "\" could not be parsed: " + e.what());
			}
		}

		DataType::Type type;
		if (boost::iequals(name, "INTEGER")) {
			type = DataType::Integer;
		} else if (boost::iequals(name, "SMALLINT")) {
			type = DataType::SmallInteger;
		} else if (boost::iequals(name, "DOUBLE PRECISION")) {
			type = DataType::Double;
		} else if (boost::iequals(name, "CHAR")) {
			type = DataType::FixedSizeString;
		} else if (boost::iequals(name, "VARCHAR")) {
			type = DataType::String;
		} else {
			throw UnknownDataTypeException("Unknown data type \"" + name + "\"");
		}

		if (!canBeSized(type) && size != DataType::Unsized) {
			throw UnknownDataTypeException("Data type \"" + name + "\" can't be sized");
		}
		if (!canBeUnsized(type) && size == DataType::Unsized) {
			throw UnknownDataTypeException("Data type \"" + name + "\" has to be sized");
		}

		return DataType(type, size);
	}

} // namespace db
} // namespace mumble
