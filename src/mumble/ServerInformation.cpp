// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerInformation.h"
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

	// Labels are not initialized properly here. We need to wait a tiny bit
	// to make sure its contents are actually read.
	QTimer::singleShot(0, [this]() {
		qgbServerInformation->updateAccessibleText();
		qgbAudioBandwidth->updateAccessibleText();
		qgbTCPParameters->updateAccessibleText();
	});
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

	const auto userCount            = ModelItem::c_qhUsers.count();
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
	serverInfo_protocol->setText(Version::toString(Global::get().sh->m_version));
	serverInfo_release->setText(release);
	serverInfo_os->setText(os);

	qgbServerInformation->updateAccessibleText();
}

static const QString currentCodec() {
	// We now always use Opus
	return "Opus";
}

void ServerInformation::updateAudioBandwidth() {
	// The bandwidths are in bit/s, so we divide by 1000 to get kBit/s
	const float maxBandwidthAllowed = static_cast< float >(Global::get().iMaxBandwidth) / 1000.0f;
	const float currentBandwidth    = static_cast< float >(Global::get().iAudioBandwidth) / 1000.0f;

	audio_current->setText(QString::fromLatin1("%1 kBit/s").arg(currentBandwidth, 0, 'f', 1));
	audio_allowed->setText(QString::fromLatin1("%1 kBit/s").arg(maxBandwidthAllowed, 0, 'f', 1));
	audio_codec->setText(currentCodec());

	qgbAudioBandwidth->updateAccessibleText();
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
		const float latency = static_cast< float >(boost::accumulators::mean(Global::get().sh->accUDP));
		const float deviation =
			static_cast< float >(std::sqrt(boost::accumulators::variance(Global::get().sh->accUDP)));

		connection_udp_encryption->setText("128 bit OCB-AES128");
		connection_udp_latency->setText(latencyString.arg(latency, 0, 'f', 1).arg(deviation, 0, 'f', 1));

		populateUDPStatistics(*connection);
	}


	// TCP
	const float latency   = static_cast< float >(boost::accumulators::mean(Global::get().sh->accTCP));
	const float deviation = static_cast< float >(std::sqrt(boost::accumulators::variance(Global::get().sh->accTCP)));

	QSslCipher cipher = Global::get().sh->qscCipher;

	connection_tcp_tls->setText(MumbleSSL::protocolToString(connection->sessionProtocol()).toHtmlEscaped());
	connection_tcp_latency->setText(latencyString.arg(latency, 0, 'f', 1).arg(deviation, 0, 'f', 1));
	connection_tcp_cipher->setText(cipher.name().isEmpty() ? m_unknownStr : cipher.name());
	connection_tcp_forwardSecrecy->setText(Global::get().sh->connectionUsesPerfectForwardSecrecy ? tr("Yes")
																								 : tr("No"));

	qgbTCPParameters->updateAccessibleText();
}

void ServerInformation::populateUDPStatistics(const Connection &connection) {
	// statistics
	constexpr int TO_SERVER_COL   = 0;
	constexpr int FROM_SERVER_COL = 1;
	constexpr int GOOD_ROW        = 0;
	constexpr int LATE_ROW        = 1;
	constexpr int LOST_ROW        = 2;
	constexpr int RESYNC_ROW      = 3;

	QTableWidgetItem *toGoodItem     = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsRemote.good));
	QTableWidgetItem *fromGoodItem   = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsLocal.good));
	QTableWidgetItem *toLateItem     = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsRemote.late));
	QTableWidgetItem *fromLateItem   = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsLocal.late));
	QTableWidgetItem *toLostItem     = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsRemote.lost));
	QTableWidgetItem *fromLostItem   = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsLocal.lost));
	QTableWidgetItem *toResyncItem   = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsRemote.resync));
	QTableWidgetItem *fromResyncItem = new QTableWidgetItem(QString::number(connection.csCrypt->m_statsLocal.resync));

	connection_udp_statisticsTable->setItem(GOOD_ROW, TO_SERVER_COL, toGoodItem);
	connection_udp_statisticsTable->setItem(GOOD_ROW, FROM_SERVER_COL, fromGoodItem);
	connection_udp_statisticsTable->setItem(LATE_ROW, TO_SERVER_COL, toLateItem);
	connection_udp_statisticsTable->setItem(LATE_ROW, FROM_SERVER_COL, fromLateItem);
	connection_udp_statisticsTable->setItem(LOST_ROW, TO_SERVER_COL, toLostItem);
	connection_udp_statisticsTable->setItem(LOST_ROW, FROM_SERVER_COL, fromLostItem);
	connection_udp_statisticsTable->setItem(RESYNC_ROW, TO_SERVER_COL, toResyncItem);
	connection_udp_statisticsTable->setItem(RESYNC_ROW, FROM_SERVER_COL, fromResyncItem);

	connection_udp_statisticsTable->adjustSize();
}
