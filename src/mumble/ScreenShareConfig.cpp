// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ScreenShareConfig.h"

#include "MainWindow.h"
#include "VideoEncoderBackend.h"
#include "Global.h"

#include <QtCore/QPointer>
#include <QtCore/QSignalBlocker>
#include <QtCore/QThread>
#include <QtWidgets/QApplication>

const QString ScreenShareConfig::name = QLatin1String("ScreenShareConfig");

static ConfigWidget *ScreenShareConfigNew(Settings &st) {
	return new ScreenShareConfig(st);
}

static ConfigRegistrar registrarScreenShareConfig(1020, ScreenShareConfigNew);

ScreenShareConfig::ScreenShareConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	{
		const QSignalBlocker blocker(qcbEncoderMode);
		qcbEncoderMode->addItem(tr("Optimised for viewers"), static_cast< int >(VideoEncoderMode::Optimised));
		qcbEncoderMode->addItem(tr("Best available"), static_cast< int >(VideoEncoderMode::Best));
		qcbEncoderMode->addItem(tr("Manual"), static_cast< int >(VideoEncoderMode::Manual));
	}

	qcbEncoder->addItem(tr("Detecting encoders..."));
	qcbEncoder->setEnabled(false);

	// The encoders may still be being probed, which must not block the GUI
	QPointer< ScreenShareConfig > self(this);
	QThread *thread = QThread::create([self]() {
		VideoEncoders::available();
		QMetaObject::invokeMethod(
			qApp,
			[self]() {
				if (self) {
					self->m_encodersKnown = true;
					self->fillEncoders();
				}
			},
			Qt::QueuedConnection);
	});
	connect(thread, &QThread::finished, thread, &QObject::deleteLater);
	thread->start(QThread::LowPriority);
}

QString ScreenShareConfig::title() const {
	return tr("Screen Sharing");
}

const QString &ScreenShareConfig::getName() const {
	return ScreenShareConfig::name;
}

QIcon ScreenShareConfig::icon() const {
	return QIcon(QLatin1String("skin:actions/screenshare.svg"));
}

void ScreenShareConfig::load(const Settings &r) {
	const int modeIndex = qcbEncoderMode->findData(static_cast< int >(r.screenShareEncoderMode));
	qcbEncoderMode->setCurrentIndex(modeIndex >= 0 ? modeIndex : 0);
	// Not emitted if the index didn't change
	on_qcbEncoderMode_currentIndexChanged(qcbEncoderMode->currentIndex());

	m_manualEncoder = r.screenShareEncoder;
	fillEncoders();
}

void ScreenShareConfig::save() const {
	s.screenShareEncoderMode = static_cast< VideoEncoderMode >(qcbEncoderMode->currentData().toInt());
	if (m_encodersKnown && qcbEncoder->currentIndex() >= 0) {
		s.screenShareEncoder = qcbEncoder->currentData().toString();
	} else {
		s.screenShareEncoder = m_manualEncoder;
	}
}

void ScreenShareConfig::accept() const {
	// Switches an ongoing stream over to the newly preferred encoder
	Global::get().mw->updateScreenShareEncoderSelection();
}

void ScreenShareConfig::fillEncoders() {
	if (!m_encodersKnown) {
		return;
	}

	qcbEncoder->clear();

	const std::vector< VideoEncoderInfo > &encoders = VideoEncoders::available();
	bool manualEncoderAvailable                     = false;
	for (const VideoEncoderInfo &info : encoders) {
		qcbEncoder->addItem(info.hardware ? tr("%1 - hardware").arg(info.name) : tr("%1 - software").arg(info.name),
							info.id);
		manualEncoderAvailable = manualEncoderAvailable || info.id == m_manualEncoder;
	}

	if (!m_manualEncoder.isEmpty() && !manualEncoderAvailable) {
		// Keep the user's choice, e.g. for when the hardware is back
		qcbEncoder->addItem(tr("%1 - not available").arg(m_manualEncoder), m_manualEncoder);
	}

	if (qcbEncoder->count() == 0) {
		qcbEncoder->addItem(tr("No working encoder found"));
		qcbEncoder->setEnabled(false);
		return;
	}

	const int index = qcbEncoder->findData(m_manualEncoder);
	qcbEncoder->setCurrentIndex(index >= 0 ? index : 0);
	qcbEncoder->setEnabled(static_cast< VideoEncoderMode >(qcbEncoderMode->currentData().toInt())
						   == VideoEncoderMode::Manual);
}

void ScreenShareConfig::on_qcbEncoderMode_currentIndexChanged(int index) {
	const VideoEncoderMode mode = static_cast< VideoEncoderMode >(qcbEncoderMode->itemData(index).toInt());

	switch (mode) {
		case VideoEncoderMode::Optimised:
			qlEncoderModeDescription->setText(
				tr("Uses the best encoder whose video all users in your channel can watch. The encoder is chosen "
				   "anew whenever someone joins or leaves the channel while you are sharing your screen."));
			break;
		case VideoEncoderMode::Best:
			qlEncoderModeDescription->setText(
				tr("Always uses the best encoder that works on this computer. Users whose Mumble can't decode its "
				   "video won't be able to watch your screen."));
			break;
		case VideoEncoderMode::Manual:
			qlEncoderModeDescription->setText(
				tr("Uses the encoder selected below, falling back to the best other one if it doesn't work. Users "
				   "whose Mumble can't decode its video won't be able to watch your screen."));
			break;
	}

	qcbEncoder->setEnabled(m_encodersKnown && mode == VideoEncoderMode::Manual
						   && !qcbEncoder->currentData().toString().isEmpty());
}
