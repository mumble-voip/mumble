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
			case DataType::Float:
				return false;
			case DataType::Double:
				return false;
			case DataType::FixedSizeString:
				return true;
			case DataType::VarChar:
				return true;
			case DataType::Text:
				return false;
			case DataType::EpochTime:
				return false;
			case DataType::Blob:
				return false;
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
			case DataType::Float:
				return true;
			case DataType::Double:
				return true;
			case DataType::FixedSizeString:
				return false;
			case DataType::VarChar:
				return false;
			case DataType::Text:
				return true;
			case DataType::EpochTime:
				return true;
			case DataType::Blob:
				return true;
		}

		// This code should be unreachable
		assert(false);
		return false;
	}

	bool DataType::isStringType() const { return isStringType(m_type); }

	bool DataType::isStringType(DataType::Type type) {
		switch (type) {
			case DataType::Type::FixedSizeString:
			case DataType::Type::VarChar:
			case DataType::Text:
				return true;
			case DataType::Type::Integer:
			case DataType::Type::SmallInteger:
			case DataType::Type::Float:
			case DataType::Type::Double:
			case DataType::Type::EpochTime:
			case DataType::Type::Blob:
				return false;
		}

		// Code should be unreachable
		assert(false);
		return false;
	}

	std::string DataType::sqlRepresentation(Backend backend) const {
		std::string sqlRepr;

		switch (m_type) {
			case DataType::Integer:
				sqlRepr = "INTEGER";
				break;
			case DataType::SmallInteger:
				sqlRepr = "SMALLINT";
				break;
			case DataType::Float:
				if (backend == Backend::PostgreSQL) {
					sqlRepr = "REAL";
				} else {
					sqlRepr = "FLOAT";
				}
				break;
			case DataType::Double:
				sqlRepr = "DOUBLE PRECISION";
				break;
			case DataType::FixedSizeString:
				sqlRepr = "CHAR";
				break;
			case DataType::VarChar:
				sqlRepr = "VARCHAR";
				break;
			case DataType::Text:
				sqlRepr = "TEXT";
				break;
			case DataType::EpochTime:
				// Note that we explicitly do not use the TIMESTAMP type as that would be stored as a 4-byte
				// integer which means that it'd break down some time in the year 2038. At least this
				// currently (2022) applies to MySQL.
				sqlRepr = "BIGINT";
				break;
			case DataType::Blob:
				if (backend == Backend::PostgreSQL) {
					sqlRepr = "OID";
				} else {
					sqlRepr = "BLOB";
				}
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
		bool found    = false;
		for (; i < strRepr.size(); ++i) {
			if (strRepr[i] == '(') {
				found = true;
				break;
			}

			name += strRepr[i];
		}

		std::size_t size = DataType::Unsized;
		if (found) {
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
		} else if (boost::iequals(name, "FLOAT") || boost::iequals(name, "REAL")) {
			type = DataType::Float;
		} else if (boost::iequals(name, "DOUBLE PRECISION")) {
			type = DataType::Double;
		} else if (boost::iequals(name, "CHAR")) {
			type = DataType::FixedSizeString;
		} else if (boost::iequals(name, "VARCHAR")) {
			type = DataType::VarChar;
		} else if (boost::iequals(name, "TEXT")) {
			type = DataType::Text;
		} else if (boost::iequals(name, "BIGINT")) {
			type = DataType::EpochTime;
		} else if (boost::iequals(name, "BLOB") || boost::iequals(name, "OID")) {
			type = DataType::Blob;
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

	bool operator==(const DataType &lhs, const DataType &rhs) {
		return lhs.m_type == rhs.m_type && lhs.m_size == rhs.m_size;
	}

	bool operator!=(const DataType &lhs, const DataType &rhs) { return !(lhs == rhs); }

} // namespace db
} // namespace mumble
