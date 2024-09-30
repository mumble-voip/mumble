// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SERVERINFORMATION_H_
#define MUMBLE_MUMBLE_SERVERINFORMATION_H_

#include <QDialog>

#include "ui_ServerInformation.h"

class Connection;

class ServerInformation : public QDialog, private Ui::ServerInformation {
	Q_OBJECT
	Q_DISABLE_COPY(ServerInformation)

public:
	ServerInformation(QWidget *parent = nullptr);

	void updateFields();

public slots:
	void on_okButton_clicked();
	void on_viewCertButton_clicked();

private:
	static QString m_unknownStr;

	void updateServerInformation();
	void updateAudioBandwidth();
	void updateConnectionDetails();
	void populateUDPStatistics(const Connection &connection);
};

#endif // MUMBLE_MUMBLE_SERVERINFORMATION_H_
