// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MainWindow.h"
#include "ServerHandler.h"
#include "ServerLogs.h"
#include "Version.h"
#include "Global.h"

#include <QTableWidgetItem>

#include <boost/accumulators/accumulators.hpp>

QString ServerLogs::m_unknownStr = tr("Unknown");

ServerLogs::ServerLogs(QWidget *parent) : QDialog(parent) {
	setupUi(this);

	updateFields();
}

void ServerLogs::updateFields() {
	updateTimeSpan();
	updateLogs();
}

void ServerLogs::updateTimeSpan() {

}

void ServerLogs::updateLogs() {

}


void ServerLogs::on_okButton_clicked() {
	accept();
}
