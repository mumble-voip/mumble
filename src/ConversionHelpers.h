#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <string>

#ifndef CONVERSION_HELPER_H
#define CONVERSION_HELPER_H

inline QString u8(const ::std::string &str) {
	return QString::fromUtf8(str.data(), static_cast<int>(str.length()));
}

inline QString u8(const ::std::wstring &str) {
	return QString::fromStdWString(str);
}

inline ::std::string u8(const QString &str) {
	const QByteArray &qba = str.toUtf8();
	return ::std::string(qba.constData(), qba.length());
}

inline QByteArray blob(const ::std::string &str) {
	return QByteArray(str.data(), static_cast<int>(str.length()));
}

inline ::std::string blob(const QByteArray &str) {
	return ::std::string(str.constData(), str.length());
}

inline QByteArray sha1(const QByteArray &blob) {
	return QCryptographicHash::hash(blob, QCryptographicHash::Sha1);
}

inline QByteArray sha1(const QString &str) {
	return QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha1);
}

#endif
