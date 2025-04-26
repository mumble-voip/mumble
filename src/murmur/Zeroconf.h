// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_ZEROCONF_H_
#define MUMBLE_MURMUR_ZEROCONF_H_

#include "BonjourServiceRegister.h"

#include <memory>

#ifdef Q_OS_WIN
#	include <windns.h>
#endif

class Zeroconf : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Zeroconf)
protected:
	bool m_ok;
	std::unique_ptr< BonjourServiceRegister > m_helper;
#ifdef Q_OS_WIN
	std::unique_ptr< DNS_SERVICE_CANCEL > m_cancel;
	std::unique_ptr< DNS_SERVICE_REGISTER_REQUEST > m_request;

	static void WINAPI callbackRegisterComplete(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance);
#endif
	void helperError(const DNSServiceErrorType error);
#ifdef Q_OS_WIN
	PDNS_SERVICE_INSTANCE(WINAPI *DnsServiceConstructInstance)
	(PCWSTR pServiceName, PCWSTR pHostName, PIP4_ADDRESS pIp4, PIP6_ADDRESS pIp6, WORD wPort, WORD wPriority,
	 WORD wWeight, DWORD dwPropertiesCount, PCWSTR *keys, PCWSTR *values);
	VOID(WINAPI *DnsServiceFreeInstance)(PDNS_SERVICE_INSTANCE pInstance);
	DWORD(WINAPI *DnsServiceRegister)(PDNS_SERVICE_REGISTER_REQUEST pRequest, PDNS_SERVICE_CANCEL pCancel);
	DWORD(WINAPI *DnsServiceDeRegister)(PDNS_SERVICE_REGISTER_REQUEST pRequest, PDNS_SERVICE_CANCEL pCancel);
	DWORD(WINAPI *DnsServiceRegisterCancel)(PDNS_SERVICE_CANCEL pCancelHandle);
#endif
public:
	inline bool isOk() const { return m_ok; }

	void resetHelper();

	bool registerService(const BonjourRecord &record, const uint16_t port);
	bool unregisterService();

	Zeroconf();
	~Zeroconf();
};

#endif
