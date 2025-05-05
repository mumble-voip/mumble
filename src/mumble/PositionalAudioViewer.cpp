// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PositionalAudioViewer.h"
#include "ui_PositionalAudioViewer.h"

#include "PluginManager.h"
#include "Global.h"

PositionalAudioViewer::PositionalAudioViewer() : m_ui(std::make_unique< Ui::PositionalAudioViewer >()) {
	m_ui->setupUi(this);

	connect(&m_timer, &QTimer::timeout, this, &PositionalAudioViewer::update);

	m_timer.start(100);
}

PositionalAudioViewer::~PositionalAudioViewer() {
}

void PositionalAudioViewer::update() {
	auto pluginManager = Global::get().pluginManager;
	if (!pluginManager) {
		return;
	}

	pluginManager->fetchPositionalData();

	const PositionalData &posData = pluginManager->getPositionalData();

	updatePlayer(posData);
	updateCamera(posData);

	m_ui->context->setPlainText(posData.getContext());
	m_ui->identity->setPlainText(posData.getPlayerIdentity());
}

void PositionalAudioViewer::updatePlayer(const PositionalData &posData) {
	const Position3D pos = posData.getPlayerPos();
	const Vector3D dir   = posData.getPlayerDir();
	const Vector3D axis  = posData.getPlayerAxis();

	m_ui->playerPosX->setValue(pos.x);
	m_ui->playerPosY->setValue(pos.y);
	m_ui->playerPosZ->setValue(pos.z);

	m_ui->playerDirX->setValue(dir.x);
	m_ui->playerDirY->setValue(dir.y);
	m_ui->playerDirZ->setValue(dir.z);

	m_ui->playerAxisX->setValue(axis.x);
	m_ui->playerAxisY->setValue(axis.y);
	m_ui->playerAxisZ->setValue(axis.z);
}

void PositionalAudioViewer::updateCamera(const PositionalData &posData) {
	const Position3D pos = posData.getCameraPos();
	const Vector3D dir   = posData.getCameraDir();
	const Vector3D axis  = posData.getCameraAxis();

	m_ui->cameraPosX->setValue(pos.x);
	m_ui->cameraPosY->setValue(pos.y);
	m_ui->cameraPosZ->setValue(pos.z);

	m_ui->cameraDirX->setValue(dir.x);
	m_ui->cameraDirY->setValue(dir.y);
	m_ui->cameraDirZ->setValue(dir.z);

	m_ui->cameraAxisX->setValue(axis.x);
	m_ui->cameraAxisY->setValue(axis.y);
	m_ui->cameraAxisZ->setValue(axis.z);
}
