// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_ZEROCONF_H_
#define MUMBLE_MURMUR_ZEROCONF_H_

#include "BonjourServiceRegister.h"

#include <memory>

#ifdef Q_OS_WIN
#	include <optional>

#	include <windns.h>
#endif

class Zeroconf : public QObject {
public:
	constexpr bool isOk() const { return m_ok; }

	void resetHelper();

	bool registerService(const BonjourRecord &record, const uint16_t port);
	bool unregisterService();

	Zeroconf();
	~Zeroconf();

protected:
	bool m_ok;
#ifdef Q_OS_WIN
	class Reg {
	public:
		constexpr auto isOk() const { return m_module != nullptr; }

		bool cancel();
		bool request(const BonjourRecord &record, const uint16_t port);

		Reg();
		~Reg();

	protected:
		struct InstanceDeleter {
			using FreeFn = VOID(WINAPI *)(PDNS_SERVICE_INSTANCE);

			FreeFn &m_freeFn;

			explicit InstanceDeleter(FreeFn &freeFn) noexcept : m_freeFn(freeFn) {}

			void operator()(PDNS_SERVICE_INSTANCE instance) const noexcept {
				if (instance) {
					m_freeFn(instance);
				}
			}
		};

		std::optional< DNS_SERVICE_CANCEL > m_cancel;
		std::unique_ptr< DNS_SERVICE_INSTANCE, InstanceDeleter > m_instance;

		static void WINAPI callback(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance);

		PDNS_SERVICE_INSTANCE(WINAPI *DnsServiceConstructInstance)
		(PCWSTR pServiceName, PCWSTR pHostName, PIP4_ADDRESS pIp4, PIP6_ADDRESS pIp6, WORD wPort, WORD wPriority,
		 WORD wWeight, DWORD dwPropertiesCount, PCWSTR *keys, PCWSTR *values);
		VOID(WINAPI *DnsServiceFreeInstance)(PDNS_SERVICE_INSTANCE pInstance);
		DWORD(WINAPI *DnsServiceRegister)(PDNS_SERVICE_REGISTER_REQUEST pRequest, PDNS_SERVICE_CANCEL pCancel);
		DWORD(WINAPI *DnsServiceDeRegister)(PDNS_SERVICE_REGISTER_REQUEST pRequest, PDNS_SERVICE_CANCEL pCancel);
		DWORD(WINAPI *DnsServiceRegisterCancel)(PDNS_SERVICE_CANCEL pCancelHandle);

	private:
		HMODULE m_module;
	};

	Reg m_reg;
#endif
	std::unique_ptr< BonjourServiceRegister > m_helper;

	void helperError(const DNSServiceErrorType error);

private:
	Q_OBJECT
	Q_DISABLE_COPY(Zeroconf)
};

#endif
