/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "NetworkConfig.h"
#include "Global.h"
#include "MainWindow.h"
#include "OSInfo.h"

static ConfigWidget *NetworkConfigNew(Settings &st) {
	return new NetworkConfig(st);
}

static ConfigRegistrar registrar(1300, NetworkConfigNew);

NetworkConfig::NetworkConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);
#ifdef NO_UPDATE_CHECK
	qcbAutoUpdate->hide();
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
	loadCheckBox(qcbAutoReconnect, s.bReconnect);
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

	loadCheckBox(qcbAutoUpdate, r.bUpdateCheck);
	loadCheckBox(qcbUsage, r.bUsage);
}

void NetworkConfig::save() const {
	s.bTCPCompat = qcbTcpMode->isChecked();
	s.bReconnect = qcbAutoReconnect->isChecked();

	s.ptProxyType = static_cast<Settings::ProxyType>(qcbType->currentIndex());
	s.qsProxyHost = qleHostname->text();
	s.usProxyPort = qlePort->text().toUShort();
	s.qsProxyUsername = qleUsername->text();
	s.qsProxyPassword = qlePassword->text();

	s.bUpdateCheck=qcbAutoUpdate->isChecked();
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

bool NetworkConfig::expert(bool b) {
	qcbTcpMode->setVisible(b);
	qgbProxy->setVisible(b);
	qcbUsage->setVisible(b);
	return true;
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
#if QT_VERSION >= 0x040600
	req.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
#endif
	req.setRawHeader(QString::fromLatin1("User-Agent").toUtf8(), QString::fromLatin1("Mozilla/5.0 (%1; %2) Mumble/%3 %4").arg(OSInfo::getOS(), OSInfo::getOSVersion(), QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)), QLatin1String(MUMBLE_RELEASE)).toUtf8());
}
