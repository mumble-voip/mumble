/*
Copyright (c) 2007, Trenton Schulz

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. The name of the author may not be used to endorse or promote products
    derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MUMBLE_BONJOUR_RECORD_H_
#define MUMBLE_BONJOUR_RECORD_H_

#include <QtCore/QMetaType>
#include <QtCore/QString>

class BonjourRecord {
	public:
		BonjourRecord() {}
		BonjourRecord(const QString &name, const QString &regType, const QString &domain)
				: serviceName(name), registeredType(regType), replyDomain(domain) {}
		BonjourRecord(const char *name, const char *regType, const char *domain) {
			serviceName = QString::fromUtf8(name);
			registeredType = QString::fromUtf8(regType);
			replyDomain = QString::fromUtf8(domain);
		}
		QString serviceName;
		QString registeredType;
		QString replyDomain;
		bool operator==(const BonjourRecord &other) const {
			return serviceName == other.serviceName
			       && registeredType == other.registeredType
			       && replyDomain == other.replyDomain;
		}
};

Q_DECLARE_METATYPE(BonjourRecord)

#endif
