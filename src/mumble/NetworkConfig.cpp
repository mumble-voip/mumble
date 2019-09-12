// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "NetworkConfig.h"

#include "MainWindow.h"
#include "OSInfo.h"

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkAccessManager>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

static ConfigWidget *NetworkConfigNew(Settings &st) {
	return new NetworkConfig(st);
}

static ConfigRegistrar registrar(1300, NetworkConfigNew);

NetworkConfig::NetworkConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);
#ifdef NO_UPDATE_CHECK
	qcbAutoUpdate->hide();
	qcbPluginUpdate->hide();
#endif
}

QString NetworkConfig::title() const {
	return tr("Network");
}

QIcon NetworkConfig::icon() const {
	return QIcon(QLatin1String("skin:config_network.png"));
}

void NetworkConfig::load(const Settings &r) {

	loadCheckBox(qcbTcpMode, s.bTCPCompat);
	loadCheckBox(qcbQoS, s.bQoS);
	loadCheckBox(qcbAutoReconnect, s.bReconnect);
	loadCheckBox(qcbAutoConnect, s.bAutoConnect);
	loadCheckBox(qcbSuppressIdentity, s.bSuppressIdentity);
	loadComboBox(qcbType, s.ptProxyType);

	qleHostname->setText(r.qsProxyHost);

	if (r.usProxyPort > 0) {
		QString port;
		port.setNum(r.usProxyPort);
		qlePort->setText(port);
	} else
		qlePort->setText(QString());

	qleUsername->setText(r.qsProxyUsername);
	qlePassword->setText(r.qsProxyPassword);

	loadCheckBox(qcbHideOS, s.bHideOS);

	loadCheckBox(qcbAutoUpdate, r.bUpdateCheck);
	loadCheckBox(qcbPluginUpdate, r.bPluginCheck);
	loadCheckBox(qcbUsage, r.bUsage);

#if defined(SNAPSHOT_BUILD) && defined(QT_NO_DEBUG)
	qcbAutoUpdate->setEnabled(false);
	qcbAutoUpdate->setToolTip(tr("Updates are mandatory when using snapshot releases."));
#endif
}

void NetworkConfig::save() const {
	s.bTCPCompat = qcbTcpMode->isChecked();
	s.bQoS = qcbQoS->isChecked();
	s.bReconnect = qcbAutoReconnect->isChecked();
	s.bAutoConnect = qcbAutoConnect->isChecked();
	s.bSuppressIdentity = qcbSuppressIdentity->isChecked();
	s.bHideOS = qcbHideOS->isChecked();

	s.ptProxyType = static_cast<Settings::ProxyType>(qcbType->currentIndex());
	s.qsProxyHost = qleHostname->text();
	s.usProxyPort = qlePort->text().toUShort();
	s.qsProxyUsername = qleUsername->text();
	s.qsProxyPassword = qlePassword->text();

	s.bUpdateCheck=qcbAutoUpdate->isChecked();
	s.bPluginCheck=qcbPluginUpdate->isChecked();
	s.bUsage=qcbUsage->isChecked();
}

static QNetworkProxy::ProxyType local_to_qt_proxy(Settings::ProxyType pt) {
	switch (pt) {
		case Settings::NoProxy:
			return QNetworkProxy::NoProxy;
		case Settings::HttpProxy:
			return QNetworkProxy::HttpProxy;
		case Settings::Socks5Proxy:
			return QNetworkProxy::Socks5Proxy;
	}

	return QNetworkProxy::NoProxy;
}

void NetworkConfig::SetupProxy() {
	QNetworkProxy proxy;
	proxy.setType(local_to_qt_proxy(g.s.ptProxyType));
	proxy.setHostName(g.s.qsProxyHost);
	proxy.setPort(g.s.usProxyPort);
	proxy.setUser(g.s.qsProxyUsername);
	proxy.setPassword(g.s.qsProxyPassword);
	QNetworkProxy::setApplicationProxy(proxy);
}

bool NetworkConfig::TcpModeEnabled() {
	/*
	 * We force TCP mode for both HTTP and SOCKS5 proxies, even though SOCKS5 supports UDP.
	 *
	 * This is because Qt's automatic application-wide proxying fails when we're in UDP
	 * mode since the datagram transmission code assumes that its socket is created in its
	 * own thread. Due to the automatic proxying, this assumption is incorrect, because of
	 * Qt's behind-the-scenes magic.
	 *
	 * However, TCP mode uses Qt events to make sure packets are sent off from the right
	 * thread, and this is what we utilize here.
	 *
	 * This is probably not even something that should even be taken care of, as proxying
	 * itself already is a potential latency killer.
	 */

	return g.s.ptProxyType != Settings::NoProxy || g.s.bTCPCompat;
}

void NetworkConfig::accept() const {
	NetworkConfig::SetupProxy();
}

void NetworkConfig::on_qcbType_currentIndexChanged(int v) {
	Settings::ProxyType pt = static_cast<Settings::ProxyType>(v);

	qleHostname->setEnabled(pt != Settings::NoProxy);
	qlePort->setEnabled(pt != Settings::NoProxy);
	qleUsername->setEnabled(pt != Settings::NoProxy);
	qlePassword->setEnabled(pt != Settings::NoProxy);
	qcbTcpMode->setEnabled(pt == Settings::NoProxy);

	s.ptProxyType = pt;
}

QNetworkReply *Network::get(const QUrl &url) {
	QNetworkRequest req(url);
	prepareRequest(req);
	return g.nam->get(req);
}

void Network::prepareRequest(QNetworkRequest &req) {
	req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

        // Do not send OS information if the corresponding privacy setting is enabled
        if (g.s.bHideOS) {
		req.setRawHeader(QString::fromLatin1("User-Agent").toUtf8(), QString::fromLatin1("Mozilla/5.0 Mumble/%1 %2").arg(QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)), QLatin1String(MUMBLE_RELEASE)).toUtf8());
        } else {
		req.setRawHeader(QString::fromLatin1("User-Agent").toUtf8(), QString::fromLatin1("Mozilla/5.0 (%1; %2) Mumble/%3 %4").arg(OSInfo::getOS(), OSInfo::getOSVersion(), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)), QLatin1String(MUMBLE_RELEASE)).toUtf8());
	}
}
