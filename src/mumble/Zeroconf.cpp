// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Zeroconf.h"

#define GET_SYMBOL(symbol) (symbol = reinterpret_cast< decltype(symbol) >(GetProcAddress(handle, #symbol)))

Zeroconf::Zeroconf() : m_ok(false) {
	qRegisterMetaType< uint16_t >("uint16_t");
	qRegisterMetaType< BonjourRecord >("BonjourRecord");
	qRegisterMetaType< QList< BonjourRecord > >("QList<BonjourRecord>");
#ifdef Q_OS_WIN
	auto handle = GetModuleHandle(L"dnsapi.dll");
	if (handle) {
		GET_SYMBOL(DnsServiceBrowse);
		GET_SYMBOL(DnsServiceBrowseCancel);
		GET_SYMBOL(DnsServiceResolve);
		GET_SYMBOL(DnsServiceResolveCancel);
		GET_SYMBOL(DnsServiceFreeInstance);

		if (DnsServiceBrowse && DnsServiceBrowseCancel && DnsServiceResolve && DnsServiceResolveCancel
			&& DnsServiceFreeInstance) {
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
	resetHelperBrowser();
	resetHelperResolver();

	m_ok = true;
}

Zeroconf::~Zeroconf() {
	if (!m_helperBrowser) {
		stopBrowser();
	}

	if (!m_helperResolver) {
		cleanupResolvers();
	}
}

void Zeroconf::resetHelperBrowser() {
	m_helperBrowser.reset(new BonjourServiceBrowser(this));
	connect(m_helperBrowser.get(), &BonjourServiceBrowser::currentBonjourRecordsChanged, this,
			&Zeroconf::helperBrowserRecordsChanged);
	connect(m_helperBrowser.get(), &BonjourServiceBrowser::error, this, &Zeroconf::helperBrowserError);
}

void Zeroconf::resetHelperResolver() {
	m_helperResolver.reset(new BonjourServiceResolver(this));
	connect(m_helperResolver.get(), &BonjourServiceResolver::bonjourRecordResolved, this,
			&Zeroconf::helperResolverRecordResolved);
	connect(m_helperResolver.get(), &BonjourServiceResolver::error, this, &Zeroconf::helperResolverError);
}

bool Zeroconf::startBrowser(const QString &serviceType) {
	if (!m_ok) {
		return false;
	}

	stopBrowser();

	if (m_helperBrowser) {
		m_helperBrowser->browseForServiceType(serviceType);
		return true;
	}
#ifdef Q_OS_WIN
	const QString queryName = serviceType + QLatin1String(".local");

	DNS_SERVICE_BROWSE_REQUEST request{};
	request.Version         = DNS_QUERY_REQUEST_VERSION1;
	request.QueryName       = reinterpret_cast< LPCWSTR >(queryName.utf16());
	request.pBrowseCallback = callbackBrowseComplete;
	request.pQueryContext   = this;

	m_cancelBrowser.reset(new DNS_SERVICE_CANCEL{});
	const auto ret = DnsServiceBrowse(&request, m_cancelBrowser.get());
	if (ret == DNS_REQUEST_PENDING) {
		return true;
	}

	m_cancelBrowser.reset();
	qWarning("Zeroconf: DnsServiceBrowse() failed with error %u!", ret);
#endif
	return false;
}

bool Zeroconf::stopBrowser() {
	if (!m_ok) {
		return false;
	}

	if (m_helperBrowser) {
		resetHelperBrowser();
		return true;
	}
#ifdef Q_OS_WIN
	if (m_cancelBrowser) {
		const auto ret = DnsServiceBrowseCancel(m_cancelBrowser.get());
		if (ret == ERROR_SUCCESS || ret == ERROR_CANCELLED) {
			m_cancelBrowser.reset();
			return true;
		}

		qWarning("Zeroconf: DnsServiceBrowseCancel() failed with error %u!", ret);
		return false;
	}
#endif
	return true;
}

bool Zeroconf::startResolver(const BonjourRecord &record) {
	if (!m_ok) {
		return false;
	}

	if (m_helperResolver) {
		m_helperResolver->resolveBonjourRecord(record);
		return true;
	}
#ifdef Q_OS_WIN
	stopResolver(record);

	auto qualifiedHostname = record.serviceName.toStdWString() + L"." + record.registeredType.toStdWString()
							 + record.replyDomain.toStdWString();

	m_resolvers.append(Resolver(this, record));
	auto context = &m_resolvers.last();

	DNS_SERVICE_RESOLVE_REQUEST request{};
	request.Version                    = DNS_QUERY_REQUEST_VERSION1;
	request.QueryName                  = &qualifiedHostname[0];
	request.pResolveCompletionCallback = &Zeroconf::callbackResolveComplete;
	request.pQueryContext              = context;

	const auto ret = DnsServiceResolve(&request, &context->m_cancel);
	if (ret == DNS_REQUEST_PENDING) {
		return true;
	}

	m_resolvers.removeLast();

	qWarning("Zeroconf: DnsServiceResolve() failed with error %u!", ret);
#endif
	return false;
}
#ifdef Q_OS_WIN
bool Zeroconf::stopResolver(const BonjourRecord &record) {
	if (!m_ok) {
		return false;
	}

	Resolver tmp(this, record);
	if (!m_resolvers.contains(tmp)) {
		return true;
	}

	auto resolver = m_resolvers[m_resolvers.indexOf(tmp)];
	return stopResolver(resolver);
}

bool Zeroconf::stopResolver(Resolver &resolver) {
	if (!m_ok) {
		return false;
	}

	const auto ret = DnsServiceResolveCancel(&resolver.m_cancel);
	if (ret == ERROR_SUCCESS || ret == ERROR_CANCELLED) {
		return true;
	}

	qWarning("Zeroconf: DnsServiceResolveCancel() failed with error %u!", ret);
	return false;
}
#endif
bool Zeroconf::cleanupResolvers() {
	if (!m_ok) {
		return false;
	}

	if (m_helperResolver) {
		resetHelperResolver();
		return true;
	}

	auto result = true;
#ifdef Q_OS_WIN
	for (auto i = 0; i < m_resolvers.size(); ++i) {
		if (!stopResolver(m_resolvers[i])) {
			result = false;
		}
	}

	m_resolvers.clear();
#endif
	return result;
}

void Zeroconf::helperBrowserRecordsChanged(const QList< BonjourRecord > &records) {
	emit recordsChanged(records);
}

void Zeroconf::helperResolverRecordResolved(const BonjourRecord record, const QString hostname, const int port) {
	emit recordResolved(record, hostname, static_cast< std::uint16_t >(port));
}

void Zeroconf::helperBrowserError(const DNSServiceErrorType error) const {
	qWarning("Zeroconf: Third-party browser API reports error %d", error);
}

void Zeroconf::helperResolverError(const BonjourRecord record, const DNSServiceErrorType error) {
	qWarning("Zeroconf: Third-party resolver API reports error %d", error);
	emit resolveError(record);
}
#ifdef Q_OS_WIN
void WINAPI Zeroconf::callbackBrowseComplete(const DWORD status, void *context, DNS_RECORD *records) {
	auto zeroconf = static_cast< Zeroconf * >(context);
	zeroconf->m_cancelBrowser.reset();

	if (status != ERROR_SUCCESS) {
		if (records) {
			DnsRecordListFree(records, DnsFreeRecordList);
		}

		if (status != ERROR_CANCELLED) {
			qWarning("Zeroconf: DnsServiceBrowse() reports status code %u, ignoring results", status);
		}

		return;
	}

	if (!records) {
		return;
	}

	bool changed = false;

	for (auto cur = records; cur; cur = cur->pNext) {
		if (cur->wType != DNS_TYPE_PTR) {
			continue;
		}

		// Example: "_mumble._tcp.local".
		const auto domain = QString::fromWCharArray(cur->pName);
		// Example: "Test._mumble._tcp.local".
		const auto hostname = QString::fromWCharArray(cur->Data.PTR.pNameHost);

		BonjourRecord record;
		record.serviceName = hostname.left(hostname.lastIndexOf(domain));
		// Trim trailing ".".
		record.serviceName.resize(record.serviceName.size() - 1);

		record.replyDomain    = domain.mid(domain.lastIndexOf('.'));
		record.registeredType = domain.left(domain.lastIndexOf(record.replyDomain));

		if (!zeroconf->m_records.contains(record)) {
			zeroconf->m_records.append(record);
			changed = true;
		}
	}

	DnsRecordListFree(records, DnsFreeRecordList);

	if (changed) {
		emit zeroconf->recordsChanged(zeroconf->m_records);
	}
}

void WINAPI Zeroconf::callbackResolveComplete(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance) {
	auto resolver = static_cast< Resolver * >(context);
	auto zeroconf = resolver->m_zeroconf;

	if (status != ERROR_SUCCESS) {
		if (instance) {
			zeroconf->DnsServiceFreeInstance(instance);
		}

		if (status != ERROR_CANCELLED) {
			qWarning("Zeroconf: DnsServiceResolve() reports status code %u, ignoring result", status);
		}

		return;
	}

	if (!instance) {
		return;
	}

	emit zeroconf->recordResolved(resolver->m_record, QString::fromWCharArray(instance->pszHostName), instance->wPort);

	zeroconf->DnsServiceFreeInstance(instance);
}
#endif

#undef GET_SYMBOL
