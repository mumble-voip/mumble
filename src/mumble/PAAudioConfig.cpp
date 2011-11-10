/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2007, Stefan Gehn <mETz AT gehn DOT net>

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

#include "mumble_pch.hpp"

#include "PAAudioConfig.h"
#include "PAAudio.h"


static ConfigWidget *PAAudioConfigNew(Settings &st) {
	return new PAAudioConfig(st);
}

static ConfigRegistrar registrar(2005, PAAudioConfigNew);

PAAudioConfig::PAAudioConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);
	setupDevicesList();

	connect(lwInput->selectionModel(),
	        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	        SLOT(fixSelection(const QItemSelection &, const QItemSelection &)));
	connect(lwOutput->selectionModel(),
	        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
	        SLOT(fixSelection(const QItemSelection &, const QItemSelection &)));
}

QString PAAudioConfig::title() const {
	return tr("PortAudio");
}

QIcon PAAudioConfig::icon() const {
	return QIcon(QLatin1String("skin:config_dsound.png"));
}

void PAAudioConfig::load(const Settings &r) {
	QList<QListWidgetItem *> qlItems;

	qWarning() << "PAAudioConfig::load()";

	int idx = 0;
	QListWidgetItem *qlwi = NULL;
	do {
		qlwi = lwInput->item(idx);
		if (qlwi && (qlwi->data(Qt::UserRole).toInt() == r.iPortAudioInput)) {
			qlwi->setSelected(true);
			break;
		}
		++idx;
	} while (qlwi);

	qlwi = NULL;
	idx = 0;
	do {
		qlwi = lwOutput->item(idx);
		if (qlwi && (qlwi->data(Qt::UserRole).toInt() == r.iPortAudioOutput)) {
			qlwi->setSelected(true);
			break;
		}
		++idx;
	} while (qlwi);
}

void PAAudioConfig::save() const {
	QListWidgetItem *lwi;
	QList<QListWidgetItem *> qlSel;

	qlSel = lwInput->selectedItems();
	lwi = (qlSel.isEmpty() ? NULL : qlSel.first());
	if (lwi)
		s.iPortAudioInput = lwi->data(Qt::UserRole).toInt();

	qlSel = lwOutput->selectedItems();
	lwi = (qlSel.isEmpty() ? NULL : qlSel.first());
	if (lwi)
		s.iPortAudioOutput = lwi->data(Qt::UserRole).toInt();

	qWarning() << "PAAudioConfig::save(); saved input device index  :" << s.iPortAudioInput;
	qWarning() << "PAAudioConfig::save(); saved output device index :" << s.iPortAudioOutput;
}

bool PAAudioConfig::expert(bool) {
	return true;
}

void PAAudioConfig::setupDevicesList() {
	PortAudioSystemPtr pSys = PortAudioSystem::self();

	QHash<PaHostApiIndex, PortAudioSystem::HostApiDevices>::const_iterator apiIt;
	QHash<PaHostApiIndex, PortAudioSystem::HostApiDevices>::const_iterator apiItEnd = pSys->qhHostApis.constEnd();
	for (apiIt = pSys->qhHostApis.constBegin(); apiIt != apiItEnd; ++apiIt) {
		QHash<PaDeviceIndex, QString>::const_iterator devIt;
		QHash<PaDeviceIndex, QString>::const_iterator devItEnd;

		const PortAudioSystem::HostApiDevices devs = apiIt.value();

		if (!devs.qhInputs.isEmpty()) {
			devItEnd = devs.qhInputs.constEnd();
			for (devIt = devs.qhInputs.constBegin(); devIt != devItEnd; ++devIt) {
				QString devName;
				// special case default device, there's no real hostapi behind it
				if (apiIt.key() == -1)
					devName = devIt.value();
				else
					devName = QString("%1 (%2)").arg(devIt.value(), devs.qsHostApiName);

				QListWidgetItem *lwiInputDev = new QListWidgetItem(lwInput);
				lwiInputDev->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				lwiInputDev->setText(devName);
				lwiInputDev->setData(Qt::UserRole, devIt.key());
			}
		}

		if (!devs.qhOutputs.isEmpty()) {
			devItEnd = devs.qhOutputs.constEnd();
			for (devIt = devs.qhOutputs.constBegin(); devIt != devItEnd; ++devIt) {
				QString devName;
				// special case default device, there's no real hostapi behind it
				if (apiIt.key() == -1)
					devName = devIt.value();
				else
					devName = QString("%1 (%2)").arg(devIt.value(), devs.qsHostApiName);

				QListWidgetItem *lwiOutputDevice = new QListWidgetItem(lwOutput);
				lwiOutputDevice->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				lwiOutputDevice->setText(devName);
				lwiOutputDevice->setData(Qt::UserRole, devIt.key());
			}
		}
	}
}

void PAAudioConfig::fixSelection(const QItemSelection &, const QItemSelection &deSel) {
	QItemSelectionModel *selModel = qobject_cast<QItemSelectionModel *>(sender());
	if (!selModel)
		return;

	if (selModel->hasSelection())
		return;

	// re-select the deselected items
	selModel->select(deSel, QItemSelectionModel::Select);
}
