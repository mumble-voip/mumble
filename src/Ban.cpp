// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Ban.h"

bool Ban::isExpired() const {
	return (iDuration > 0) && static_cast< int >(iDuration - qdtStart.secsTo(QDateTime::currentDateTime().toUTC())) < 0;
}

bool Ban::operator<(const Ban &other) const {
	// Compare username primarily and address secondarily
	const int unameDifference = qsUsername.localeAwareCompare(other.qsUsername);
	if (unameDifference == 0)
		return haAddress < other.haAddress;
	else
		return unameDifference < 0;
}

bool Ban::operator==(const Ban &other) const {
	return (haAddress == other.haAddress) && (iMask == other.iMask) && (qsUsername == other.qsUsername)
		   && (qsHash == other.qsHash) && (qsReason == other.qsReason) && (qdtStart == other.qdtStart)
		   && (iDuration == other.iDuration);
}

bool Ban::isValid() const {
	return haAddress.isValid() && (iMask >= 8) && (iMask <= 128);
}

QString Ban::toString() const {
	return QString(QLatin1String("Hash: \"%1\", Host: \"%2\", Mask: \"%3\", Username: \"%4\", Reason: \"%5\", "
								 "BanStart: \"%6\", BanEnd: \"%7\" %8"))
		.arg(qsHash, haAddress.toString(), haAddress.isV6() ? QString::number(iMask) : QString::number(iMask - 96),
			 qsUsername, qsReason, qdtStart.toLocalTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss")),
			 qdtStart.toLocalTime().addSecs(iDuration).toString(QLatin1String("yyyy-MM-dd hh:mm:ss")),
			 iDuration == 0 ? QLatin1String("(permanent)") : QLatin1String("(temporary)"));
}

std::size_t qHash(const Ban &b) {
	return qHash(b.qsHash) ^ qHash(b.haAddress) ^ qHash(b.qsUsername) ^ qHash(b.iMask);
}
