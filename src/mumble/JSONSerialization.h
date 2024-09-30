// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_JSONSERIALIZATION_H_
#define MUMBLE_MUMBLE_JSONSERIALIZATION_H_

#include "EnumStringConversions.h"
#include "Settings.h"

#include <sstream>
#include <type_traits>

#include <boost/version.hpp>
#if BOOST_VERSION >= 106700
// The is_detected type-trait was only included in Boost 1.67
#	include <boost/type_traits/is_detected.hpp>
#endif

#include <nlohmann/json.hpp>

#include <QByteArray>
#include <QColor>
#include <QDataStream>
#include <QFlags>
#include <QFont>
#include <QHash>
#include <QList>
#include <QMap>
#include <QObject>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QString>

namespace details {
template< typename T > using has_enum_to_string_function = decltype(enumToString(std::declval< T >()));

template< typename T, bool isEnum, bool isQObject > struct StringConverter {
	static std::string to_string(const T &value) {
		std::stringstream stream;

		stream << value;

		return stream.str();
	}

	static T from_string(const std::string &str) {
		std::stringstream stream(str);

		T value;

		stream >> value;

		return value;
	}
};

template< typename T > struct StringConverter< T, true, false > {
	static std::string to_string(const T value) {
#if BOOST_VERSION >= 106700
		// boost::is_detected is only available since Boost 1.67 and since we only use it to create a better error
		// message, we can simply not make use of it, if earlier versions are used.
		static_assert(boost::is_detected< has_enum_to_string_function, T >::value,
					  "Template specialization requires corresponding enumToString function");
#endif
		return enumToString(value);
	}

	static T from_string(const std::string &str) { return ::stringToEnum< T >(str); }
};

template< typename T > struct StringConverter< T, false, true > {
	static std::string to_string(const T &value) {
		static_assert(std::is_base_of< QObject, T >::value, "Template specialization only applies to QObjects");

		QByteArray data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream.setVersion(QDataStream::Qt_5_0);

		stream << value;

		return data.toStdString();
	}

	static T from_string(const std::string &str) {
		QByteArray data = QByteArray::fromStdString(str);
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream.setVersion(QDataStream::Qt_5_0);

		T value;
		value << stream;

		return value;
	}
};

template< typename T > std::string to_string(const T &value) {
	return StringConverter< T, std::is_enum< T >::value, std::is_base_of< QObject, T >::value >::to_string(value);
}

template<> inline std::string to_string< QString >(const QString &str) {
	return str.toStdString();
}

template< typename T > T from_string(const std::string &str) {
	return StringConverter< T, std::is_enum< T >::value, std::is_base_of< QObject, T >::value >::from_string(str);
}

template<> inline QString from_string< QString >(const std::string &str) {
	return QString::fromStdString(str);
}

} // namespace details



void to_json(nlohmann::json &j, const Settings &settings);
void from_json(const nlohmann::json &j, Settings &settings);
void to_json(nlohmann::json &j, const OverlaySettings &settings);
void from_json(const nlohmann::json &j, OverlaySettings &settings);

void to_json(nlohmann::json &j, const QString &string);
void from_json(const nlohmann::json &j, QString &string);
void to_json(nlohmann::json &j, const QVariant &variant);
void from_json(const nlohmann::json &j, QVariant &variant);
void to_json(nlohmann::json &j, const QPoint &point);
void from_json(const nlohmann::json &j, QPoint &point);
void to_json(nlohmann::json &j, const Shortcut &shortcut);
void from_json(const nlohmann::json &j, Shortcut &shortcut);
void to_json(nlohmann::json &j, const ShortcutTarget &target);
void from_json(const nlohmann::json &j, ShortcutTarget &target);
void to_json(nlohmann::json &j, const PluginSetting &setting);
void from_json(const nlohmann::json &j, PluginSetting &setting);
void to_json(nlohmann::json &j, const QByteArray &byteArray);
void from_json(const nlohmann::json &j, QByteArray &byteArray);
void to_json(nlohmann::json &j, const QColor &color);
void from_json(const nlohmann::json &j, QColor &color);
void to_json(nlohmann::json &j, const QFont &font);
void from_json(const nlohmann::json &j, QFont &font);
void to_json(nlohmann::json &j, const QRect &rect);
void from_json(const nlohmann::json &j, QRect &rect);
void to_json(nlohmann::json &j, const QRectF &rect);
void from_json(const nlohmann::json &j, QRectF &rect);
void to_json(nlohmann::json &j, const QSize &size);
void from_json(const nlohmann::json &j, QSize &size);
void to_json(nlohmann::json &j, const QSizeF &size);
void from_json(const nlohmann::json &j, QSizeF &size);

template< typename T > void to_json(nlohmann::json &j, const QList< T > &list) {
	j = nlohmann::json::array();
	for (const T &current : list) {
		j.push_back(current);
	}
}
template< typename T > void from_json(const nlohmann::json &j, QList< T > &list) {
	for (auto it = j.begin(); it != j.end(); ++it) {
		list.push_back(it->get< T >());
	}
}

template< typename Key, typename Value > void to_json(nlohmann::json &j, const QMap< Key, Value > &map) {
	j = nlohmann::json::object();

	for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
		j[details::to_string(it.key())] = it.value();
	}
}

template< typename Key, typename Value > void from_json(const nlohmann::json &j, QMap< Key, Value > &map) {
	for (auto it = j.cbegin(); it != j.cend(); ++it) {
		map[details::from_string< Key >(it.key())] = it.value().get< Value >();
	}
}

template< typename Key, typename Value > void to_json(nlohmann::json &j, const QHash< Key, Value > &map) {
	j = nlohmann::json::object();

	for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
		j[details::to_string(it.key())] = it.value();
	}
}

template< typename Key, typename Value > void from_json(const nlohmann::json &j, QHash< Key, Value > &map) {
	for (auto it = j.cbegin(); it != j.cend(); ++it) {
		map[details::from_string< Key >(it.key())] = it.value().get< Value >();
	}
}

template< typename T > void to_json(nlohmann::json &j, const QFlags< T > &flags) {
	j = static_cast< typename QFlags< T >::Int >(flags);
}

template< typename T > void from_json(const nlohmann::json &j, QFlags< T > &flags) {
	flags = QFlag(j.get< typename QFlags< T >::Int >());
}

#endif // MUMBLE_MUMBLE_JSONSERIALIZATION_H_
