// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Zeroconf.h"

#define GET_SYMBOL(symbol) (symbol = reinterpret_cast< decltype(symbol) >(GetProcAddress(m_module, #symbol)))

Zeroconf::Zeroconf() : m_ok(false) {
#ifdef Q_OS_WIN
	if (m_reg.isOk()) {
		m_ok = true;
		return;
	}

	qWarning("Zeroconf: Native mDNS/DNS-SD API not available, falling back to third-party API");

	HMODULE module = LoadLibrary(L"dnssd.dll");
	if (!module) {
		qWarning("Zeroconf: Failed to load dnssd.dll, assuming third-party API is not available");
		return;
	}

	FreeLibrary(module);
#endif
	resetHelper();

	m_ok = true;
}

Zeroconf::~Zeroconf() {
}

void Zeroconf::resetHelper() {
	m_helper.reset(new BonjourServiceRegister(this));
	connect(m_helper.get(), &BonjourServiceRegister::error, this, &Zeroconf::helperError);
}

bool Zeroconf::registerService(const BonjourRecord &record, const uint16_t port) {
	unregisterService();

	if (m_helper) {
		m_helper->registerService(record, port);
		return true;
	}
#ifdef Q_OS_WIN
	return m_reg.request(record, port);
#else
	return false;
#endif
}

bool Zeroconf::unregisterService() {
	if (m_helper) {
		resetHelper();
		return true;
	}
#ifdef Q_OS_WIN
	return m_reg.cancel();
#else
	return false;
#endif
}

void Zeroconf::helperError(const DNSServiceErrorType error) {
	qWarning("Zeroconf: Third-party API reports error %d, service registration probably failed", error);
}

#ifdef Q_OS_WIN
Zeroconf::Reg::Reg() : m_instance(nullptr, InstanceDeleter{ DnsServiceFreeInstance }) {
	m_module = GetModuleHandle(L"dnsapi.dll");
	if (!m_module) {
		return;
	}

	GET_SYMBOL(DnsServiceConstructInstance);
	GET_SYMBOL(DnsServiceFreeInstance);
	GET_SYMBOL(DnsServiceRegister);
	GET_SYMBOL(DnsServiceDeRegister);
	GET_SYMBOL(DnsServiceRegisterCancel);

	if (!DnsServiceConstructInstance || !DnsServiceFreeInstance || !DnsServiceRegister || !DnsServiceDeRegister
		|| !DnsServiceRegisterCancel) {
		FreeModule(m_module);
		m_module = nullptr;
	}
}

Zeroconf::Reg::~Reg() {
	if (!isOk()) {
		return;
	}

	cancel();
	FreeLibrary(m_module);
}

bool Zeroconf::Reg::cancel() {
	if (!isOk()) {
		return false;
	}

	if (m_cancel) {
		const auto ret = DnsServiceRegisterCancel(&m_cancel.value());
		if (ret != ERROR_SUCCESS && ret != ERROR_CANCELLED) {
			qWarning("Zeroconf: DnsServiceRegisterCancel() failed with error %u!", ret);

			m_cancel.reset();
			m_instance.reset();

			return false;
		}
	} else if (m_instance) {
		DNS_SERVICE_REGISTER_REQUEST req{};
		req.Version                     = DNS_QUERY_REQUEST_VERSION1;
		req.pServiceInstance            = m_instance.get();
		req.pRegisterCompletionCallback = callback;
		req.pQueryContext               = this;

		const auto ret = DnsServiceDeRegister(&req, nullptr);
		if (ret != DNS_REQUEST_PENDING) {
			qWarning("Zeroconf: DnsServiceDeRegister() failed with error %u!", ret);

			return false;
		}
	}

	return true;
}

bool Zeroconf::Reg::request(const BonjourRecord &record, const uint16_t port) {
	if (!isOk()) {
		return false;
	}

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

	DNS_SERVICE_REGISTER_REQUEST req{};
	req.Version                     = DNS_QUERY_REQUEST_VERSION1;
	req.pServiceInstance            = instance;
	req.pRegisterCompletionCallback = callback;
	req.pQueryContext               = this;

	m_cancel = DNS_SERVICE_CANCEL{};
	m_instance.reset(instance);

	const auto ret = DnsServiceRegister(&req, &m_cancel.value());
	if (ret != DNS_REQUEST_PENDING) {
		qWarning("Zeroconf: DnsServiceRegister() failed with error %u!", ret);

		m_instance.reset();
		m_cancel.reset();

		return false;
	}

	return true;
}

void WINAPI Zeroconf::Reg::callback(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance) {
	auto &reg = *static_cast< Reg * >(context);

	reg.m_instance.reset(instance);

	if (!reg.m_cancel) {
		// No cancel handle, which means this is a de-registration.
		return;
	}

	reg.m_cancel.reset();

	switch (status) {
		case ERROR_SUCCESS:
		case ERROR_CANCELLED:
			break;
		default:
			qWarning("Zeroconf: DnsServiceRegister() reports status code %u, service registration probably failed",
					 status);
	}
}
#endif

#undef GET_SYMBOL
