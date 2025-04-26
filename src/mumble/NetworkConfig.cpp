// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "NetworkConfig.h"

#include "MainWindow.h"
#include "OSInfo.h"
#include "Global.h"

#include <QSignalBlocker>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>

#ifdef NO_UPDATE_CHECK
#	include <QMessageBox>
#endif

const QString NetworkConfig::name = QLatin1String("NetworkConfig");

static ConfigWidget *NetworkConfigNew(Settings &st) {
	return new NetworkConfig(st);
}

static ConfigRegistrar registrarNetworkConfig(1300, NetworkConfigNew);

NetworkConfig::NetworkConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);
}

QString NetworkConfig::title() const {
	return tr("Network");
}

const QString &NetworkConfig::getName() const {
	return NetworkConfig::name;
}

QIcon NetworkConfig::icon() const {
	return QIcon(QLatin1String("skin:config_network.png"));
}

void NetworkConfig::load(const Settings &r) {
	loadCheckBox(qcbTcpMode, s.bTCPCompat);
	loadCheckBox(qcbQoS, s.bQoS);
	loadCheckBox(qcbAutoReconnect, s.bReconnect);
	loadCheckBox(qcbAutoConnect, s.bAutoConnect);
	loadCheckBox(qcbDisablePublicList, s.bDisablePublicList);
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

	const QSignalBlocker blocker(qcbAutoUpdate);
	loadCheckBox(qcbAutoUpdate, r.bUpdateCheck);
	loadCheckBox(qcbPluginUpdateCheck, r.bPluginCheck);
	loadCheckBox(qcbPluginAutoUpdate, r.bPluginAutoUpdate);
	loadCheckBox(qcbUsage, r.bUsage);
}

void NetworkConfig::save() const {
	s.bTCPCompat         = qcbTcpMode->isChecked();
	s.bQoS               = qcbQoS->isChecked();
	s.bReconnect         = qcbAutoReconnect->isChecked();
	s.bAutoConnect       = qcbAutoConnect->isChecked();
	s.bDisablePublicList = qcbDisablePublicList->isChecked();
	s.bSuppressIdentity  = qcbSuppressIdentity->isChecked();
	s.bHideOS            = qcbHideOS->isChecked();

	s.ptProxyType     = static_cast< Settings::ProxyType >(qcbType->currentIndex());
	s.qsProxyHost     = qleHostname->text();
	s.usProxyPort     = qlePort->text().toUShort();
	s.qsProxyUsername = qleUsername->text();
	s.qsProxyPassword = qlePassword->text();

	s.bUpdateCheck      = qcbAutoUpdate->isChecked();
	s.bPluginCheck      = qcbPluginUpdateCheck->isChecked();
	s.bPluginAutoUpdate = qcbPluginAutoUpdate->isChecked();
	s.bUsage            = qcbUsage->isChecked();
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
	proxy.setType(local_to_qt_proxy(Global::get().s.ptProxyType));
	proxy.setHostName(Global::get().s.qsProxyHost);
	proxy.setPort(Global::get().s.usProxyPort);
	proxy.setUser(Global::get().s.qsProxyUsername);
	proxy.setPassword(Global::get().s.qsProxyPassword);
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

	return Global::get().s.ptProxyType != Settings::NoProxy || Global::get().s.bTCPCompat;
}

void NetworkConfig::accept() const {
	NetworkConfig::SetupProxy();
}

void NetworkConfig::on_qcbType_currentIndexChanged(int v) {
	Settings::ProxyType pt = static_cast< Settings::ProxyType >(v);

	qleHostname->setEnabled(pt != Settings::NoProxy);
	qlePort->setEnabled(pt != Settings::NoProxy);
	qleUsername->setEnabled(pt != Settings::NoProxy);
	qlePassword->setEnabled(pt != Settings::NoProxy);
	qcbTcpMode->setEnabled(pt == Settings::NoProxy);

	s.ptProxyType = pt;
}

#ifdef NO_UPDATE_CHECK
void NetworkConfig::on_qcbAutoUpdate_stateChanged(int state) {
	if (state == Qt::Checked) {
		QMessageBox msgBox;
		msgBox.setText(
			QObject::tr("<p>You're using a Mumble version that <b>explicitly disabled</b> update-checks.</p>"
						"<p>This means that the update notification you might receive by using this option will "
						"<b>most likely be meaningless</b> for you.</p>"));
		msgBox.setInformativeText(
			QObject::tr("<p>If you're using Linux this is most likely because you are using a "
						"version from your distribution's package repository that have their own update cycles.</p>"
						"<p>If you want to always have the most recent Mumble version, you should consider using a "
						"different method of installation.\n"
						"See <a href=\"https://github.com/mumble-voip/mumble\">the project repository</a> for what "
						"alternatives there are.</p>"));
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
}
#endif

QNetworkReply *Network::get(const QUrl &url) {
	QNetworkRequest req(url);
	prepareRequest(req);
	return Global::get().nam->get(req);
}

void Network::prepareRequest(QNetworkRequest &req) {
	req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

	// Do not send OS information if the corresponding privacy setting is enabled
	if (Global::get().s.bHideOS) {
		req.setRawHeader(QString::fromLatin1("User-Agent").toUtf8(),
						 QString::fromLatin1("Mozilla/5.0 Mumble/%1").arg(Version::getRelease()).toUtf8());
	} else {
		req.setRawHeader(QString::fromLatin1("User-Agent").toUtf8(),
						 QString::fromLatin1("Mozilla/5.0 (%1; %2) Mumble/%3")
							 .arg(OSInfo::getOS(), OSInfo::getOSVersion(), Version::getRelease())
							 .toUtf8());
	}
}
