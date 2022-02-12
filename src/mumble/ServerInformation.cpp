// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerInformation.h"
#include "CELTCodec.h"
#include "Connection.h"
#include "MainWindow.h"
#include "NetworkConfig.h"
#include "SSL.h"
#include "ServerHandler.h"
#include "UserModel.h"
#include "Version.h"
#include "ViewCert.h"
#include "Global.h"

#include <QTableWidgetItem>

#include <boost/accumulators/accumulators.hpp>

#include <cmath>

QString ServerInformation::m_unknownStr = tr("Unknown");

ServerInformation::ServerInformation(QWidget *parent) : QDialog(parent) {
	setupUi(this);

	updateFields();
}

void ServerInformation::updateFields() {
	updateServerInformation();
	updateAudioBandwidth();
	updateConnectionDetails();
}

void ServerInformation::on_okButton_clicked() {
	accept();
}

void ServerInformation::on_viewCertButton_clicked() {
	hide();

	ViewCert certViewer(Global::get().sh->qscCert, this);
	certViewer.exec();

	accept();
}

void ServerInformation::updateServerInformation() {
	QString host, userName, password;
	unsigned short port;

	Global::get().sh->getConnectionInfo(host, port, userName, password);

	const int userCount             = ModelItem::c_qhUsers.count();
	const unsigned int maxUserCount = Global::get().uiMaxUsers;

	QString release = Global::get().sh->qsRelease;
	if (release.isEmpty()) {
		release = m_unknownStr;
	}
	QString os = Global::get().sh->qsOS;
	if (os.isEmpty()) {
		os = m_unknownStr;
	} else if (!Global::get().sh->qsOSVersion.isEmpty()) {
		os += QString::fromLatin1(" (%1)").arg(Global::get().sh->qsOSVersion);
	}

	serverInfo_host->setText(host);
	serverInfo_port->setText(QString::number(port));
	serverInfo_users->setText(QString::fromLatin1("%1 / %2").arg(userCount).arg(maxUserCount));
	serverInfo_protocol->setText(Version::toString(Global::get().sh->uiVersion));
	serverInfo_release->setText(release);
	serverInfo_os->setText(os);
}

static const QString currentCodec() {
	if (Global::get().bOpus)
		return QLatin1String("Opus");

	int v         = Global::get().bPreferAlpha ? Global::get().iCodecAlpha : Global::get().iCodecBeta;
	CELTCodec *cc = Global::get().qmCodecs.value(v);
	if (cc)
		return QString::fromLatin1("CELT %1").arg(cc->version());
	else
		return QString::fromLatin1("CELT %1").arg(QString::number(v, 16));
}

void ServerInformation::updateAudioBandwidth() {
	// The bandwidths are in bit/s, so we divide by 1000 to get kBit/s
	const float maxBandwidthAllowed = Global::get().iMaxBandwidth / 1000.0f;
	const float currentBandwidth    = Global::get().iAudioBandwidth / 1000.0f;

	audio_current->setText(QString::fromLatin1("%1 kBit/s").arg(currentBandwidth, 0, 'f', 1));
	audio_allowed->setText(QString::fromLatin1("%1 kBit/s").arg(maxBandwidthAllowed, 0, 'f', 1));
	audio_codec->setText(currentCodec());
}

void ServerInformation::updateConnectionDetails() {
	QString latencyString          = QString::fromUtf8("%1 ms (σ = %2 ms)");
	const ConnectionPtr connection = Global::get().sh->cConnection;

	if (!connection) {
		return;
	}

	// UDP
	if (NetworkConfig::TcpModeEnabled()) {
		connection_udp_infoMessage->show();

		connection_udp_encryption->hide();
		connection_udp_encryptionLabel->hide();
		connection_udp_latency->hide();
		connection_udp_latencyLabel->hide();
		connection_udp_statisticsGroup->hide();
	} else {
		connection_udp_infoMessage->hide();

		connection_udp_encryption->show();
		connection_udp_encryptionLabel->show();
		connection_udp_latency->show();
		connection_udp_latencyLabel->show();
		connection_udp_statisticsGroup->show();

		// Actually fill in data
		const float latency   = boost::accumulators::mean(Global::get().sh->accUDP);
		const float deviation = std::sqrt(boost::accumulators::variance(Global::get().sh->accUDP));

		connection_udp_encryption->setText("128 bit OCB-AES128");
		connection_udp_latency->setText(latencyString.arg(latency, 0, 'f', 1).arg(deviation, 0, 'f', 1));

		populateUDPStatistics(*connection);
	}


	// TCP
	const float latency   = boost::accumulators::mean(Global::get().sh->accTCP);
	const float deviation = std::sqrt(boost::accumulators::variance(Global::get().sh->accTCP));

	QSslCipher cipher = Global::get().sh->qscCipher;

	connection_tcp_tls->setText(MumbleSSL::protocolToString(connection->sessionProtocol()).toHtmlEscaped());
	connection_tcp_latency->setText(latencyString.arg(latency, 0, 'f', 1).arg(deviation, 0, 'f', 1));
	connection_tcp_cipher->setText(cipher.name().isEmpty() ? m_unknownStr : cipher.name());
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
	connection_tcp_forwardSecrecy->setText(Global::get().sh->connectionUsesPerfectForwardSecrecy ? tr("Yes")
																								 : tr("No"));
#else
	// The Qt function we use to query for PFS inside ServerHandler is only available since Qt 5.7 and if it is
	// unavailable, the respective boolean flag is never touched and is therefore meaningless.
	connection_tcp_forwardSecrecy->setText(tr("Unknown"));
#endif
}

void ServerInformation::populateUDPStatistics(const Connection &connection) {
	// statistics
	constexpr int toServerCol   = 0;
	constexpr int fromServerCol = 1;
	constexpr int goodRow       = 0;
	constexpr int lateRow       = 1;
	constexpr int lostRow       = 2;
	constexpr int resyncRow     = 3;

	QTableWidgetItem *toGoodItem     = new QTableWidgetItem(QString::number(connection.csCrypt->uiRemoteGood));
	QTableWidgetItem *fromGoodItem   = new QTableWidgetItem(QString::number(connection.csCrypt->uiGood));
	QTableWidgetItem *toLateItem     = new QTableWidgetItem(QString::number(connection.csCrypt->uiRemoteLate));
	QTableWidgetItem *fromLateItem   = new QTableWidgetItem(QString::number(connection.csCrypt->uiLate));
	QTableWidgetItem *toLostItem     = new QTableWidgetItem(QString::number(connection.csCrypt->uiRemoteLost));
	QTableWidgetItem *fromLostItem   = new QTableWidgetItem(QString::number(connection.csCrypt->uiLost));
	QTableWidgetItem *toResyncItem   = new QTableWidgetItem(QString::number(connection.csCrypt->uiRemoteResync));
	QTableWidgetItem *fromResyncItem = new QTableWidgetItem(QString::number(connection.csCrypt->uiResync));

	connection_udp_statisticsTable->setItem(goodRow, toServerCol, toGoodItem);
	connection_udp_statisticsTable->setItem(goodRow, fromServerCol, fromGoodItem);
	connection_udp_statisticsTable->setItem(lateRow, toServerCol, toLateItem);
	connection_udp_statisticsTable->setItem(lateRow, fromServerCol, fromLateItem);
	connection_udp_statisticsTable->setItem(lostRow, toServerCol, toLostItem);
	connection_udp_statisticsTable->setItem(lostRow, fromServerCol, fromLostItem);
	connection_udp_statisticsTable->setItem(resyncRow, toServerCol, toResyncItem);
	connection_udp_statisticsTable->setItem(resyncRow, fromServerCol, fromResyncItem);

	connection_udp_statisticsTable->adjustSize();
}
