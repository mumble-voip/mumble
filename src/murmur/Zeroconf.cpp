// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Zeroconf.h"

#define GET_SYMBOL(symbol) (symbol = reinterpret_cast< decltype(symbol) >(GetProcAddress(handle, #symbol)))

Zeroconf::Zeroconf() : m_ok(false) {
#ifdef Q_OS_WIN
	auto handle = GetModuleHandle(L"dnsapi.dll");
	if (handle) {
		GET_SYMBOL(DnsServiceConstructInstance);
		GET_SYMBOL(DnsServiceFreeInstance);
		GET_SYMBOL(DnsServiceRegister);
		GET_SYMBOL(DnsServiceDeRegister);
		GET_SYMBOL(DnsServiceRegisterCancel);

		if (DnsServiceConstructInstance && DnsServiceFreeInstance && DnsServiceRegister && DnsServiceDeRegister
			&& DnsServiceRegisterCancel) {
			m_ok = true;
			return;
		}
	}

	qWarning("Zeroconf: Native mDNS/DNS-SD API not available, falling back to third-party API");

	handle = LoadLibrary(L"dnssd.dll");
	if (!handle) {
		qWarning("Zeroconf: Failed to load dnssd.dll, assuming third-party API is not available");
		return;
	}
	FreeLibrary(handle);
#endif
	resetHelper();

	m_ok = true;
}

Zeroconf::~Zeroconf() {
	if (!m_helper) {
		unregisterService();
	}
}

void Zeroconf::resetHelper() {
	m_helper.reset(new BonjourServiceRegister(this));
	connect(m_helper.get(), &BonjourServiceRegister::error, this, &Zeroconf::helperError);
}

bool Zeroconf::registerService(const BonjourRecord &record, const uint16_t port) {
	if (!m_ok) {
		return false;
	}

	unregisterService();

	if (m_helper) {
		m_helper->registerService(record, port);
		return true;
	}
#ifdef Q_OS_WIN
	DWORD size = 0;
	GetComputerNameEx(ComputerNameDnsHostname, nullptr, &size);
	std::vector< wchar_t > hostname(size);
	if (!GetComputerNameEx(ComputerNameDnsHostname, &hostname[0], &size)) {
		qWarning("Zeroconf: GetComputerNameEx() failed with error %u!", GetLastError());
		return false;
	}

	const auto domain            = record.replyDomain.isEmpty() ? L".local" : L"." + record.replyDomain.toStdWString();
	const auto qualifiedHostname = &hostname[0] + domain;

	auto service = record.serviceName.isEmpty() ? &hostname[0] : record.serviceName.toStdWString();
	service += L".";
	service += record.registeredType.toStdWString();
	service += domain;

	auto instance = DnsServiceConstructInstance(service.c_str(), qualifiedHostname.c_str(), nullptr, nullptr, port, 0,
												0, 0, nullptr, nullptr);
	if (!instance) {
		qWarning("Zeroconf: DnsServiceConstructInstance() returned nullptr!");
		return false;
	}

	m_request.reset(new DNS_SERVICE_REGISTER_REQUEST{});
	m_request->Version                     = DNS_QUERY_REQUEST_VERSION1;
	m_request->pServiceInstance            = instance;
	m_request->pRegisterCompletionCallback = callbackRegisterComplete;
	m_request->pQueryContext               = this;

	m_cancel.reset(new DNS_SERVICE_CANCEL{});
	const auto ret = DnsServiceRegister(m_request.get(), m_cancel.get());
	DnsServiceFreeInstance(instance);

	if (ret == DNS_REQUEST_PENDING) {
		return true;
	}

	qWarning("Zeroconf: DnsServiceRegister() failed with error %u!", ret);
	m_request.reset();
	m_cancel.reset();
#endif
	return false;
}

bool Zeroconf::unregisterService() {
	if (!m_ok) {
		return false;
	}

	if (m_helper) {
		resetHelper();
		return true;
	}
#ifdef Q_OS_WIN
	if (m_cancel) {
		const auto ret = DnsServiceRegisterCancel(m_cancel.get());
		if (ret == ERROR_SUCCESS || ret == ERROR_CANCELLED) {
			return true;
		}

		m_cancel.reset();
		qWarning("Zeroconf: DnsServiceRegisterCancel() failed with error %u!", ret);
	} else if (m_request) {
		const auto ret = DnsServiceDeRegister(m_request.get(), nullptr);
		if (ret == DNS_REQUEST_PENDING) {
			return true;
		}

		qWarning("Zeroconf: DnsServiceDeRegister() failed with error %u!", ret);
	}
#endif
	return false;
}

void Zeroconf::helperError(const DNSServiceErrorType error) {
	qWarning("Zeroconf: Third-party API reports error %d, service registration probably failed", error);
}
#ifdef Q_OS_WIN
void WINAPI Zeroconf::callbackRegisterComplete(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance) {
	auto zeroconf = static_cast< Zeroconf * >(context);

	if (instance) {
		zeroconf->DnsServiceFreeInstance(instance);
	}

	if (status == ERROR_CANCELLED) {
		return;
	}

	zeroconf->m_cancel.reset();

	if (status != ERROR_SUCCESS) {
		qWarning("Zeroconf: DnsServiceRegister() reports status code %u, service registration probably failed", status);
	}
}
#endif

#undef GET_SYMBOL
