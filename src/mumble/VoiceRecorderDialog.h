// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VOICERECORDERDIALOG_H_
#define MUMBLE_MUMBLE_VOICERECORDERDIALOG_H_

#pragma once

#include "ui_VoiceRecorderDialog.h"

class VoiceRecorderDialog : public QDialog, private Ui::VoiceRecorderDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VoiceRecorderDialog)

	QTimer *qtTimer;

public:
	explicit VoiceRecorderDialog(QWidget *p = nullptr);
	~VoiceRecorderDialog() Q_DECL_OVERRIDE;

	void closeEvent(QCloseEvent *evt) Q_DECL_OVERRIDE;
public slots:
	void on_qpbStart_clicked();
	void on_qpbStop_clicked();
	void on_qtTimer_timeout();
	void on_qpbTargetDirectoryBrowse_clicked();

	void onRecorderStopped();
	void onRecorderStarted();
	void onRecorderError(int err, QString strerr);

	void reset(bool resettimer = true);
};

#endif // VOICERECORDERDIALOG_H_
