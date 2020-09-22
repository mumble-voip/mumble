// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_ZEROCONF_H_
#define MUMBLE_MURMUR_ZEROCONF_H_

#include "BonjourServiceRegister.h"

#include <memory>

#ifdef Q_OS_WIN64
#	include <windns.h>
#endif

class Zeroconf : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Zeroconf)
protected:
	bool m_ok;
	std::unique_ptr< BonjourServiceRegister > m_helper;
#ifdef Q_OS_WIN64
	std::unique_ptr< DNS_SERVICE_CANCEL > m_cancel;
	std::unique_ptr< DNS_SERVICE_REGISTER_REQUEST > m_request;

	static void WINAPI callbackRegisterComplete(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance);
#endif
	void helperError(const DNSServiceErrorType error);

public:
	inline bool isOk() const { return m_ok; }

	void resetHelper();

	bool registerService(const BonjourRecord &record, const uint16_t port);
	bool unregisterService();

	Zeroconf();
	~Zeroconf();
};

#endif
