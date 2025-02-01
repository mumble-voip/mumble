// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserInformation.h"

#include "Audio.h"
#include "HostAddress.h"
#include "ProtoUtils.h"
#include "QtUtils.h"
#include "ServerHandler.h"
#include "ViewCert.h"
#include "Global.h"


UserInformation::UserInformation(const MumbleProto::UserStats &msg, QWidget *p) : QDialog(p) {
	setupUi(this);

	uiSession = msg.session();

	qtTimer = new QTimer(this);
	connect(qtTimer, SIGNAL(timeout()), this, SLOT(tick()));
	qtTimer->start(6000);

	qgbConnection->setVisible(false);

	qlOpus->setText(tr("Not Reported"));

	update(msg);
	resize(sizeHint());

	qfCertificateFont = qlCertificate->font();

	// Labels are not initialized properly here. We need to wait a tiny bit
	// to make sure its contents are actually read.
	QTimer::singleShot(0, [this]() {
		qgbConnection->updateAccessibleText();
		qgbPing->updateAccessibleText();
		qgbUDP->updateAccessibleText();
		qgbBandwidth->updateAccessibleText();
	});
}

unsigned int UserInformation::session() const {
	return uiSession;
}

void UserInformation::tick() {
	if (bRequested)
		return;

	bRequested = true;

	Global::get().sh->requestUserStats(uiSession, true);
}

void UserInformation::on_qpbCertificate_clicked() {
	ViewCert *vc = new ViewCert(qlCerts, this);
	vc->setWindowModality(Qt::WindowModal);
	vc->setAttribute(Qt::WA_DeleteOnClose, true);
	vc->show();
}

QString UserInformation::secsToString(unsigned int secs) {
	QStringList qsl;

	unsigned int weeks = secs / (60 * 60 * 24 * 7);
	secs -= weeks * (60 * 60 * 24 * 7);
	unsigned int days = secs / (60 * 60 * 24);
	secs -= days * (60 * 60 * 24);
	unsigned int hours = secs / (60 * 60);
	secs -= hours * (60 * 60);
	unsigned int minutes = secs / 60;
	unsigned int seconds = secs - minutes * 60;

	if (weeks)
		qsl << tr("%1w").arg(weeks);
	if (days)
		qsl << tr("%1d").arg(days);
	if (hours)
		qsl << tr("%1h").arg(hours);
	if (minutes || hours)
		qsl << tr("%1m").arg(minutes);
	qsl << tr("%1s").arg(seconds);

	return qsl.join(QLatin1String(" "));
}

void UserInformation::update(const MumbleProto::UserStats &msg) {
	bRequested = false;

	bool showcon = false;

	ClientUser *cu = ClientUser::get(uiSession);
	if (cu)
		setWindowTitle(cu->qsName);

	if (msg.certificates_size() > 0) {
		showcon = true;
		qlCerts.clear();
		for (int i = 0; i < msg.certificates_size(); ++i) {
			const std::string &s = msg.certificates(i);
			QList< QSslCertificate > certs =
				QSslCertificate::fromData(QByteArray(s.data(), static_cast< int >(s.length())), QSsl::Der);
			qlCerts << certs;
		}
		if (!qlCerts.isEmpty()) {
			qpbCertificate->setEnabled(true);

			const QSslCertificate &cert                                      = qlCerts.last();
			const QMultiMap< QSsl::AlternativeNameEntryType, QString > &alts = cert.subjectAlternativeNames();
			if (alts.contains(QSsl::EmailEntry))
				qlCertificate->setText(QStringList(alts.values(QSsl::EmailEntry)).join(tr(", ")));
			else
				qlCertificate->setText(
					Mumble::QtUtils::decode_first_utf8_qssl_string(cert.subjectInfo(QSslCertificate::CommonName)));

			if (msg.strong_certificate()) {
				QFont f = qfCertificateFont;
				f.setBold(true);
				qlCertificate->setFont(f);
			} else {
				qlCertificate->setFont(qfCertificateFont);
			}
		} else {
			qpbCertificate->setEnabled(false);
			qlCertificate->setText(QString());
		}
	}
	if (msg.has_address()) {
		showcon = true;
		HostAddress ha(msg.address());
		qlAddress->setText(ha.toString());
	}
	if (msg.has_version()) {
		showcon = true;

		const MumbleProto::Version &mpv = msg.version();
		Version::full_t version         = MumbleProto::getVersion(mpv);
		qlVersion->setText(tr("%1 (%2)").arg(Version::toString(version)).arg(u8(mpv.release())));
		qlOS->setText(tr("%1 (%2)").arg(u8(mpv.os())).arg(u8(mpv.os_version())));

		if (Version::getPatch(version) == 255) {
			// The patch level 255 might indicate that the server is incapable of parsing
			// the new version format (or the patch level is actually exactly 255).
			// Show a warning to the user just in case.
			qlVersionNote->show();
		}
	}
	if (msg.has_opus()) {
		qlOpus->setText(msg.opus() ? tr("Supported") : tr("Not Supported"));
	}
	if (showcon)
		qgbConnection->setVisible(true);

	qlTCPCount->setText(QString::number(msg.tcp_packets()));
	qlUDPCount->setText(QString::number(msg.udp_packets()));

	qlTCPAvg->setText(QString::number(msg.tcp_ping_avg(), 'f', 2));
	qlUDPAvg->setText(QString::number(msg.udp_ping_avg(), 'f', 2));

	qlTCPVar->setText(QString::number(msg.tcp_ping_var() > 0.0f ? sqrtf(msg.tcp_ping_var()) : 0.0f, 'f', 2));
	qlUDPVar->setText(QString::number(msg.udp_ping_var() > 0.0f ? sqrtf(msg.udp_ping_var()) : 0.0f, 'f', 2));

	bool hasTotalStats   = msg.has_from_client() && msg.has_from_server();
	bool hasRollingStats = msg.has_rolling_stats();

	qgbUDP->setVisible(hasTotalStats || hasRollingStats);

	if (hasTotalStats) {
		const MumbleProto::UserStats_Stats &from = msg.from_client();
		qlFromGood->setText(QString::number(from.good()));
		qlFromLate->setText(QString::number(from.late()));
		qlFromLost->setText(QString::number(from.lost()));
		qlFromResync->setText(QString::number(from.resync()));

		const MumbleProto::UserStats_Stats &to = msg.from_server();
		qlToGood->setText(QString::number(to.good()));
		qlToLate->setText(QString::number(to.late()));
		qlToLost->setText(QString::number(to.lost()));
		qlToResync->setText(QString::number(to.resync()));

		quint32 allFromPackets = from.good() + from.late() + from.lost();
		qlFromLatePercent->setText(
			QString::number(allFromPackets > 0 ? from.late() * 100.0 / allFromPackets : 0., 'f', 1));
		qlFromLostPercent->setText(
			QString::number(allFromPackets > 0 ? from.lost() * 100.0 / allFromPackets : 0., 'f', 1));

		quint32 allToPackets = to.good() + to.late() + to.lost();
		qlToLatePercent->setText(QString::number(allToPackets > 0 ? to.late() * 100.0 / allToPackets : 0., 'f', 1));
		qlToLostPercent->setText(QString::number(allToPackets > 0 ? to.lost() * 100.0 / allToPackets : 0., 'f', 1));
	}

	if (hasRollingStats) {
		const MumbleProto::UserStats_RollingStats &rolling = msg.rolling_stats();

		const MumbleProto::UserStats_Stats &from = rolling.from_client();
		qlFromGoodRolling->setText(QString::number(from.good()));
		qlFromLateRolling->setText(QString::number(from.late()));
		qlFromLostRolling->setText(QString::number(from.lost()));
		qlFromResyncRolling->setText(QString::number(from.resync()));

		const MumbleProto::UserStats_Stats &to = rolling.from_server();
		qlToGoodRolling->setText(QString::number(to.good()));
		qlToLateRolling->setText(QString::number(to.late()));
		qlToLostRolling->setText(QString::number(to.lost()));
		qlToResyncRolling->setText(QString::number(to.resync()));

		quint32 allFromPackets = from.good() + from.late() + from.lost();
		qlFromLatePercentRolling->setText(
			QString::number(allFromPackets > 0 ? from.late() * 100.0 / allFromPackets : 0., 'f', 1));
		qlFromLostPercentRolling->setText(
			QString::number(allFromPackets > 0 ? from.lost() * 100.0 / allFromPackets : 0., 'f', 1));

		quint32 allToPackets = to.good() + to.late() + to.lost();
		qlToLatePercentRolling->setText(
			QString::number(allToPackets > 0 ? to.late() * 100.0 / allToPackets : 0., 'f', 1));
		qlToLostPercentRolling->setText(
			QString::number(allToPackets > 0 ? to.lost() * 100.0 / allToPackets : 0., 'f', 1));

		uint32_t rollingSeconds = rolling.time_window();
		QString rollingText     = tr("Last %1 %2:");
		if (rollingSeconds < 120) {
			qliRolling->setText(rollingText.arg(QString::number(rollingSeconds)).arg(tr("seconds")));
		} else {
			qliRolling->setText(rollingText.arg(QString::number(rollingSeconds / 60)).arg(tr("minutes")));
		}
	}

	qlFromGoodRolling->setVisible(hasRollingStats);
	qlFromLateRolling->setVisible(hasRollingStats);
	qlFromLostRolling->setVisible(hasRollingStats);
	qlFromResyncRolling->setVisible(hasRollingStats);
	qlToGoodRolling->setVisible(hasRollingStats);
	qlToLateRolling->setVisible(hasRollingStats);
	qlToLostRolling->setVisible(hasRollingStats);
	qlToResyncRolling->setVisible(hasRollingStats);
	qlFromLatePercentRolling->setVisible(hasRollingStats);
	qlFromLostPercentRolling->setVisible(hasRollingStats);
	qlToLatePercentRolling->setVisible(hasRollingStats);
	qlToLostPercentRolling->setVisible(hasRollingStats);
	qliRolling->setVisible(hasRollingStats);
	qliRollingFrom->setVisible(hasRollingStats);
	qliRollingTo->setVisible(hasRollingStats);

	if (msg.has_onlinesecs()) {
		if (msg.has_idlesecs())
			qlTime->setText(
				tr("%1 online (%2 idle)").arg(secsToString(msg.onlinesecs()), secsToString(msg.idlesecs())));
		else
			qlTime->setText(tr("%1 online").arg(secsToString(msg.onlinesecs())));
	}
	if (msg.has_bandwidth()) {
		qlBandwidth->setVisible(true);
		qliBandwidth->setVisible(true);
		qlBandwidth->setText(tr("%1 kbit/s").arg(msg.bandwidth() / 125.0, 0, 'f', 1));
	} else {
		qlBandwidth->setVisible(false);
		qliBandwidth->setVisible(false);
		qlBandwidth->setText(QString());
	}

	qgbConnection->updateAccessibleText();
	qgbPing->updateAccessibleText();
	qgbUDP->updateAccessibleText();
	qgbBandwidth->updateAccessibleText();
}
