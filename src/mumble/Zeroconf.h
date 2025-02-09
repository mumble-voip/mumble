// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ZEROCONF_H_
#define MUMBLE_MUMBLE_ZEROCONF_H_

#include "BonjourServiceBrowser.h"
#include "BonjourServiceResolver.h"

#include <memory>

#ifdef Q_OS_WIN
#	include <windns.h>
#endif

class Zeroconf : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Zeroconf)
protected:
#ifdef Q_OS_WIN
	struct Resolver {
		Zeroconf *m_zeroconf;
		BonjourRecord m_record;
		DNS_SERVICE_CANCEL m_cancel;

		bool operator==(const Resolver &other) const { return m_record == other.m_record; }

		Resolver(Zeroconf *zeroconf, const BonjourRecord &record) : m_zeroconf(zeroconf), m_record(record){};
	};
#endif
	bool m_ok;
	QList< BonjourRecord > m_records;
	std::unique_ptr< BonjourServiceBrowser > m_helperBrowser;
	std::unique_ptr< BonjourServiceResolver > m_helperResolver;
#ifdef Q_OS_WIN
	QList< Resolver > m_resolvers;
	std::unique_ptr< DNS_SERVICE_CANCEL > m_cancelBrowser;

	bool stopResolver(Resolver &resolver);

	static void WINAPI callbackBrowseComplete(const DWORD status, void *context, DNS_RECORD *records);
	static void WINAPI callbackResolveComplete(const DWORD status, void *context, DNS_SERVICE_INSTANCE *instance);
#endif
	void resetHelperBrowser();
	void resetHelperResolver();

	void helperBrowserRecordsChanged(const QList< BonjourRecord > &records);
	void helperResolverRecordResolved(const BonjourRecord record, const QString hostname, const int port);
	void helperBrowserError(const DNSServiceErrorType error) const;
	void helperResolverError(const BonjourRecord record, const DNSServiceErrorType error);
#ifdef Q_OS_WIN
	DNS_STATUS(WINAPI *DnsServiceBrowse)(PDNS_SERVICE_BROWSE_REQUEST pRequest, PDNS_SERVICE_CANCEL pCancel);
	DNS_STATUS(WINAPI *DnsServiceBrowseCancel)(PDNS_SERVICE_CANCEL pCancelHandle);
	DNS_STATUS(WINAPI *DnsServiceResolve)(PDNS_SERVICE_RESOLVE_REQUEST pRequest, PDNS_SERVICE_CANCEL pCancel);
	DNS_STATUS(WINAPI *DnsServiceResolveCancel)(PDNS_SERVICE_CANCEL pCancelHandle);
	VOID(WINAPI *DnsServiceFreeInstance)(PDNS_SERVICE_INSTANCE pInstance);
#endif
public:
	inline bool isOk() const { return m_ok; }
	inline QList< BonjourRecord > currentRecords() const {
		return m_helperBrowser ? m_helperBrowser->currentRecords() : m_records;
	}

	bool startBrowser(const QString &serviceType);
	bool stopBrowser();

	bool startResolver(const BonjourRecord &record);
#ifdef Q_OS_WIN
	bool stopResolver(const BonjourRecord &record);
#endif
	bool cleanupResolvers();

	Zeroconf();
	~Zeroconf();
signals:
	void recordsChanged(const QList< BonjourRecord > &records);
	void recordResolved(const BonjourRecord record, const QString hostname, const uint16_t port);
	void resolveError(const BonjourRecord record);
};

#endif
