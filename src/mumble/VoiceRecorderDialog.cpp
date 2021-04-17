// Copyright 2010-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VoiceRecorderDialog.h"

#include "AudioOutput.h"
#include "ServerHandler.h"
#include "VoiceRecorder.h"
#include "Global.h"

#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

VoiceRecorderDialog::VoiceRecorderDialog(QWidget *p) : QDialog(p), qtTimer(new QTimer(this)) {
	qtTimer->setObjectName(QLatin1String("qtTimer"));
	qtTimer->setInterval(200);
	setupUi(this);
	qcbFormat->setAccessibleName(tr("Output format"));
	qleTargetDirectory->setAccessibleName(tr("Target directory"));
	qleFilename->setAccessibleName(tr("Filename"));

	qleTargetDirectory->setText(Global::get().s.qsRecordingPath);
	qleFilename->setText(Global::get().s.qsRecordingFile);
	qrbDownmix->setChecked(Global::get().s.rmRecordingMode == Settings::RecordingMixdown);
	qrbMultichannel->setChecked(Global::get().s.rmRecordingMode == Settings::RecordingMultichannel);

	QString qsTooltip = QString::fromLatin1("%1"
											"<table>"
											"  <tr>"
											"    <td width=\"25%\">%user</td>"
											"    <td>%2</td>"
											"  </tr>"
											"  <tr>"
											"    <td>%date</td>"
											"    <td>%3</td>"
											"  </tr>"
											"  <tr>"
											"    <td>%time</td>"
											"    <td>%4</td>"
											"  </tr>"
											"  <tr>"
											"    <td>%host</td>"
											"    <td>%5</td>"
											"  </tr>"
											"</table>")
							.arg(tr("Valid variables are:"))
							.arg(tr("Inserts the user's name"))
							.arg(tr("Inserts the current date"))
							.arg(tr("Inserts the current time"))
							.arg(tr("Inserts the hostname"));

	qleTargetDirectory->setToolTip(qsTooltip);
	qleFilename->setToolTip(qsTooltip);

	// Populate available codecs
	Q_ASSERT(VoiceRecorderFormat::kEnd != 0);
	for (int fm = 0; fm < VoiceRecorderFormat::kEnd; fm++) {
		qcbFormat->addItem(VoiceRecorderFormat::getFormatDescription(static_cast< VoiceRecorderFormat::Format >(fm)));
	}

	if (Global::get().s.iRecordingFormat < 0 || Global::get().s.iRecordingFormat > VoiceRecorderFormat::kEnd)
		Global::get().s.iRecordingFormat = 0;

	qcbFormat->setCurrentIndex(Global::get().s.iRecordingFormat);
}

VoiceRecorderDialog::~VoiceRecorderDialog() {
	reset();
}

void VoiceRecorderDialog::closeEvent(QCloseEvent *evt) {
	if (Global::get().sh) {
		VoiceRecorderPtr recorder(Global::get().sh->recorder);
		if (recorder && recorder->isRunning()) {
			int ret = QMessageBox::warning(this, tr("Recorder still running"),
										   tr("Closing the recorder without stopping it will discard unwritten audio. "
											  "Do you really want to close the recorder?"),
										   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

			if (ret == QMessageBox::No) {
				evt->ignore();
				return;
			}

			recorder->stop(true);
		}
	}

	Global::get().s.qsRecordingPath = qleTargetDirectory->text();
	Global::get().s.qsRecordingFile = qleFilename->text();
	if (qrbDownmix->isChecked())
		Global::get().s.rmRecordingMode = Settings::RecordingMixdown;
	else
		Global::get().s.rmRecordingMode = Settings::RecordingMultichannel;

	int i                            = qcbFormat->currentIndex();
	Global::get().s.iRecordingFormat = (i == -1) ? 0 : i;

	reset();
	evt->accept();

	QDialog::closeEvent(evt);
}

void VoiceRecorderDialog::on_qpbStart_clicked() {
	if (!Global::get().uiSession || !Global::get().sh) {
		QMessageBox::critical(this, tr("Recorder"), tr("Unable to start recording. Not connected to a server."));
		reset();
		return;
	}

	if (Global::get().sh->uiVersion < 0x010203) {
		QMessageBox::critical(this, tr("Recorder"),
							  tr("The server you are currently connected to is version 1.2.2 or older. "
								 "For privacy reasons, recording on servers of versions older than 1.2.3 "
								 "is not possible.\nPlease contact your server administrator for further "
								 "information."));
		return;
	}

	if (Global::get().sh->recorder) {
		QMessageBox::information(this, tr("Recorder"), tr("There is already a recorder active for this server."));
		return;
	}

	// Check validity of input
	int ifm = qcbFormat->currentIndex();
	if (ifm == -1) {
		QMessageBox::critical(this, tr("Recorder"), tr("Please select a recording format."));
		return;
	}

	QString dstr = qleTargetDirectory->text();
	if (dstr.isEmpty()) {
		on_qpbTargetDirectoryBrowse_clicked();
		dstr = qleTargetDirectory->text();
		if (dstr.isEmpty())
			return;
	}
	QDir dir(dstr);

	QFileInfo fi(qleFilename->text());
	QString basename(fi.baseName());
	QString suffix(fi.completeSuffix());
	if (suffix.isEmpty())
		suffix = VoiceRecorderFormat::getFormatDefaultExtension(static_cast< VoiceRecorderFormat::Format >(ifm));


	if (basename.isEmpty()) {
		basename = QLatin1String("%user");
	}

	qleFilename->setText(basename);

	AudioOutputPtr ao(Global::get().ao);
	if (!ao)
		return;

	Global::get().sh->announceRecordingState(true);

	// Create the recorder
	VoiceRecorder::Config config;
	config.sampleRate      = ao->getMixerFreq();
	config.fileName        = dir.absoluteFilePath(basename + QLatin1Char('.') + suffix);
	config.mixDownMode     = qrbDownmix->isChecked();
	config.recordingFormat = static_cast< VoiceRecorderFormat::Format >(ifm);

	Global::get().sh->recorder.reset(new VoiceRecorder(this, config));
	VoiceRecorderPtr recorder(Global::get().sh->recorder);

	// Wire it up
	connect(&*recorder, SIGNAL(recording_started()), this, SLOT(onRecorderStarted()));
	connect(&*recorder, SIGNAL(recording_stopped()), this, SLOT(onRecorderStopped()));
	connect(&*recorder, SIGNAL(error(int, QString)), this, SLOT(onRecorderError(int, QString)));

	recorder->start();

	qpbStart->setDisabled(true);
	qpbStop->setEnabled(true);
	qgbMode->setDisabled(true);
	qgbOutput->setDisabled(true);
}

void VoiceRecorderDialog::on_qpbStop_clicked() {
	if (!Global::get().sh) {
		reset();
		return;
	}

	VoiceRecorderPtr recorder(Global::get().sh->recorder);
	if (!recorder) {
		reset();
		return;
	}

	// Stop clock and recording
	qtTimer->stop();
	recorder->stop();

	// Disable stop botton to indicate we reacted
	qpbStop->setDisabled(true);
	qpbStop->setText(tr("Stopping"));
}

void VoiceRecorderDialog::on_qtTimer_timeout() {
	if (!Global::get().sh) {
		reset();
		return;
	}

	if (!Global::get().uiSession) {
		reset(false);
		return;
	}

	VoiceRecorderPtr recorder(Global::get().sh->recorder);
	if (!Global::get().sh->recorder) {
		reset();
		return;
	}

	const QTime elapsedTime = QTime(0, 0).addMSecs(static_cast< int >(recorder->getElapsedTime() / 1000));
	qlTime->setText(elapsedTime.toString());
}

void VoiceRecorderDialog::on_qpbTargetDirectoryBrowse_clicked() {
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select target directory"), QString(),
													QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty())
		qleTargetDirectory->setText(dir);
}

void VoiceRecorderDialog::reset(bool resettimer) {
	qtTimer->stop();

	if (Global::get().sh) {
		VoiceRecorderPtr recorder(Global::get().sh->recorder);
		if (recorder) {
			Global::get().sh->recorder.reset();
			Global::get().sh->announceRecordingState(false);
		}
	}

	qpbStart->setEnabled(true);
	qpbStop->setDisabled(true);
	qpbStop->setText(tr("S&top"));

	qgbMode->setEnabled(true);
	qgbOutput->setEnabled(true);

	if (resettimer)
		qlTime->setText(QLatin1String("00:00:00"));
}

void VoiceRecorderDialog::onRecorderStopped() {
	reset(false);
}

void VoiceRecorderDialog::onRecorderStarted() {
	qlTime->setText(QLatin1String("00:00:00"));
	qtTimer->start();
}

void VoiceRecorderDialog::onRecorderError(int err, QString strerr) {
	Q_UNUSED(err);
	QMessageBox::critical(this, tr("Recorder"), strerr);
	reset(false);
}
