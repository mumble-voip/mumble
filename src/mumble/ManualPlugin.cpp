// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QMessageBox>

#include "ManualPlugin.h"
#include "ui_ManualPlugin.h"
#include "Global.h"

#include <QPointer>

#include <cmath>
#include <float.h>

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "../../plugins/mumble_legacy_plugin.h"

static QPointer< Manual > mDlg = nullptr;
static bool bLinkable          = false;
static bool bActive            = true;

static int iAzimuth   = 0;
static int iElevation = 0;

static const QString defaultContext  = QString::fromLatin1("Mumble");
static const QString defaultIdentity = QString::fromLatin1("Agent47");

static struct {
	float avatar_pos[3];
	float avatar_front[3];
	float avatar_top[3];
	float camera_pos[3];
	float camera_front[3];
	float camera_top[3];
	std::string context;
	std::wstring identity;
} my = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, std::string(), std::wstring() };

Manual::Manual(QWidget *p) : QDialog(p) {
	setupUi(this);

	qgvPosition->viewport()->installEventFilter(this);
	qgvPosition->scale(1.0f, 1.0f);
	m_qgsScene = new QGraphicsScene(QRectF(-5.0f, -5.0f, 10.0f, 10.0f), this);

	const float indicatorDiameter = 4.0f;
	QPainterPath indicator;
	// The center of the indicator's circle will represent the current position
	indicator.addEllipse(QRectF(-indicatorDiameter / 2, -indicatorDiameter / 2, indicatorDiameter, indicatorDiameter));
	// A line will indicate the indicator's orientation (azimuth)
	indicator.moveTo(0, -indicatorDiameter / 2);
	indicator.lineTo(0, -indicatorDiameter);

	m_qgiPosition = m_qgsScene->addPath(indicator);

	qgvPosition->setScene(m_qgsScene);
	qgvPosition->fitInView(-5.0f, -5.0f, 10.0f, 10.0f, Qt::KeepAspectRatio);

	qdsbX->setRange(-FLT_MAX, FLT_MAX);
	qdsbY->setRange(-FLT_MAX, FLT_MAX);
	qdsbZ->setRange(-FLT_MAX, FLT_MAX);

	qdsbX->setValue(my.avatar_pos[0]);
	qdsbY->setValue(my.avatar_pos[1]);
	qdsbZ->setValue(my.avatar_pos[2]);

	qpbActivated->setChecked(bActive);
	qpbLinked->setChecked(bLinkable);

	qsbAzimuth->setValue(iAzimuth);
	qsbElevation->setValue(iElevation);
	updateTopAndFront(iAzimuth, iElevation);

	// Set context and identity to default values in order to
	// a) make positional audio work out of the box (needs a context)
	// b) make the user aware of what each field might contain
	qleContext->setText(defaultContext);
	qleIdentity->setText(defaultIdentity);
	my.context  = defaultContext.toStdString();
	my.identity = defaultIdentity.toStdWString();

	qsbSilentUserDisplaytime->setValue(Global::get().s.manualPlugin_silentUserDisplaytime);

	updateLoopRunning.store(false);
}

void Manual::setSpeakerPositions(const QHash< unsigned int, Position2D > &positions) {
	if (mDlg) {
		QMetaObject::invokeMethod(mDlg, "on_speakerPositionUpdate", Qt::QueuedConnection,
								  Q_ARG(PositionMap, positions));
	}
}

bool Manual::eventFilter(QObject *obj, QEvent *evt) {
	if ((evt->type() == QEvent::MouseButtonPress) || (evt->type() == QEvent::MouseMove)) {
		QMouseEvent *qme = dynamic_cast< QMouseEvent * >(evt);
		if (qme) {
			if (qme->buttons() & Qt::LeftButton) {
				QPointF qpf = qgvPosition->mapToScene(qme->pos());
				qdsbX->setValue(qpf.x());
				qdsbZ->setValue(-qpf.y());
				m_qgiPosition->setPos(qpf);
			}
		}
	}
	return QDialog::eventFilter(obj, evt);
}

void Manual::changeEvent(QEvent *e) {
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}

void Manual::on_qpbUnhinge_pressed() {
	qpbUnhinge->setEnabled(false);
	mDlg->setParent(nullptr);
	mDlg->show();
}

void Manual::on_qpbLinked_clicked(bool b) {
	bLinkable = b;
}

void Manual::on_qpbActivated_clicked(bool b) {
	bActive = b;
}

void Manual::on_qdsbX_valueChanged(double d) {
	my.avatar_pos[0] = my.camera_pos[0] = static_cast< float >(d);
	m_qgiPosition->setPos(my.avatar_pos[0], -my.avatar_pos[2]);
}

void Manual::on_qdsbY_valueChanged(double d) {
	my.avatar_pos[1] = my.camera_pos[1] = static_cast< float >(d);
}

void Manual::on_qdsbZ_valueChanged(double d) {
	my.avatar_pos[2] = my.camera_pos[2] = static_cast< float >(d);
	m_qgiPosition->setPos(my.avatar_pos[0], -my.avatar_pos[2]);
}

void Manual::on_qsbAzimuth_valueChanged(int i) {
	if (i > 360)
		qdAzimuth->setValue(i % 360);
	else
		qdAzimuth->setValue(i);

	updateTopAndFront(i, qsbElevation->value());
}

void Manual::on_qsbElevation_valueChanged(int i) {
	qdElevation->setValue(90 - i);
	updateTopAndFront(qsbAzimuth->value(), i);
}

void Manual::on_qdAzimuth_valueChanged(int i) {
	if (i < 0)
		qsbAzimuth->setValue(360 + i);
	else
		qsbAzimuth->setValue(i);
}

void Manual::on_qdElevation_valueChanged(int i) {
	if (i < -90)
		qdElevation->setValue(180);
	else if (i < 0)
		qdElevation->setValue(0);
	else
		qsbElevation->setValue(90 - i);
}

void Manual::on_qleContext_editingFinished() {
	my.context = qleContext->text().toStdString();
}

void Manual::on_qleIdentity_editingFinished() {
	my.identity = qleIdentity->text().toStdWString();
}

void Manual::on_buttonBox_clicked(QAbstractButton *button) {
	if (buttonBox->buttonRole(button) == buttonBox->ResetRole) {
		qpbLinked->setChecked(false);
		qpbActivated->setChecked(true);

		bLinkable = false;
		bActive   = true;

		qdsbX->setValue(0);
		qdsbY->setValue(0);
		qdsbZ->setValue(0);

		qleContext->clear();
		qleIdentity->clear();

		qsbElevation->setValue(0);
		qsbAzimuth->setValue(0);
	}
}

void Manual::on_qsbSilentUserDisplaytime_valueChanged(int value) {
	Global::get().s.manualPlugin_silentUserDisplaytime = value;
}

void Manual::on_speakerPositionUpdate(QHash< unsigned int, Position2D > positions) {
	// First iterate over the stale items to check whether one of them is actually no longer stale
	QMutableHashIterator< unsigned int, StaleEntry > staleIt(staleSpeakerPositions);
	while (staleIt.hasNext()) {
		staleIt.next();

		const unsigned int sessionID = staleIt.key();
		QGraphicsItem *staleItem     = staleIt.value().staleItem;

		if (positions.contains(sessionID)) {
			// The item is no longer stale -> restore opacity and re-insert into speakerPositions
			staleItem->setOpacity(1.0);

			staleIt.remove();
			speakerPositions.insert(sessionID, staleItem);
		} else if (!updateLoopRunning.load()) {
			QMetaObject::invokeMethod(this, "on_updateStaleSpeakers", Qt::QueuedConnection);
			updateLoopRunning.store(true);
		}
	}

	// Now iterate over all active items and check whether they have become stale or whether their
	// position can be updated
	QMutableHashIterator< unsigned int, QGraphicsItem * > speakerIt(speakerPositions);
	while (speakerIt.hasNext()) {
		speakerIt.next();

		const unsigned int sessionID = speakerIt.key();
		QGraphicsItem *speakerItem   = speakerIt.value();

		if (positions.contains(sessionID)) {
			Position2D newPos = positions.take(sessionID);

			// Update speaker's position (remember that y-axis is inverted in screen-coordinates
			speakerItem->setPos(newPos.x, -newPos.y);
		} else {
			// Remove the stale item
			speakerIt.remove();
			if (Global::get().s.manualPlugin_silentUserDisplaytime == 0) {
				// Delete it immediately
				delete speakerItem;
			} else {
				staleSpeakerPositions.insert(sessionID, { std::chrono::steady_clock::now(), speakerItem });
			}
		}
	}

	// Finally iterate over the remaining new speakers and create new items for them
	QHashIterator< unsigned int, Position2D > remainingIt(positions);
	while (remainingIt.hasNext()) {
		remainingIt.next();

		const float speakerRadius  = 1.2f;
		QGraphicsItem *speakerItem = m_qgsScene->addEllipse(-speakerRadius, -speakerRadius, 2 * speakerRadius,
															2 * speakerRadius, QPen(), QBrush(Qt::red));

		Position2D pos = remainingIt.value();

		// y-axis is inverted in screen-space
		speakerItem->setPos(pos.x, -pos.y);

		speakerPositions.insert(remainingIt.key(), speakerItem);
	}
}

void Manual::on_updateStaleSpeakers() {
	if (staleSpeakerPositions.isEmpty()) {
		// If there are no stale speakers, this loop doesn't have to run
		updateLoopRunning.store(false);
		return;
	}

	// Iterate over all stale items and check whether they have to be removed entirely. If not, update
	// their opacity.
	QMutableHashIterator< unsigned int, StaleEntry > staleIt(staleSpeakerPositions);
	while (staleIt.hasNext()) {
		staleIt.next();

		StaleEntry entry = staleIt.value();

		double elapsedTime =
			static_cast< std::chrono::duration< double > >(std::chrono::steady_clock::now() - entry.staleSince).count();

		if (elapsedTime >= Global::get().s.manualPlugin_silentUserDisplaytime) {
			// The item has been around long enough - remove it now
			staleIt.remove();
			delete entry.staleItem;
		} else {
			// Let the item fade out
			double opacity = (Global::get().s.manualPlugin_silentUserDisplaytime - elapsedTime)
							 / static_cast< double >(Global::get().s.manualPlugin_silentUserDisplaytime);
			entry.staleItem->setOpacity(opacity);
		}
	}

	if (!staleSpeakerPositions.isEmpty()) {
		updateLoopRunning.store(true);
		// Call this function again in the next iteration of the event loop
		QMetaObject::invokeMethod(this, "on_updateStaleSpeakers", Qt::QueuedConnection);
	} else {
		updateLoopRunning.store(false);
	}
}

void Manual::updateTopAndFront(int azimuth, int elevation) {
	iAzimuth   = azimuth;
	iElevation = elevation;

	m_qgiPosition->setRotation(azimuth);

	double azim = azimuth * M_PI / 180.;
	double elev = elevation * M_PI / 180.;

	my.avatar_front[0] = static_cast< float >(cos(elev) * sin(azim));
	my.avatar_front[1] = static_cast< float >(sin(elev));
	my.avatar_front[2] = static_cast< float >(cos(elev) * cos(azim));

	my.avatar_top[0] = static_cast< float >(-sin(elev) * sin(azim));
	my.avatar_top[1] = static_cast< float >(cos(elev));
	my.avatar_top[2] = static_cast< float >(-sin(elev) * cos(azim));

	memcpy(my.camera_top, my.avatar_top, sizeof(float) * 3);
	memcpy(my.camera_front, my.avatar_front, sizeof(float) * 3);
}

static int trylock() {
	return bLinkable;
}

static void unlock() {
	if (mDlg) {
		mDlg->qpbLinked->setChecked(false);
	}
	bLinkable = false;
}

static void config(void *ptr) {
	QWidget *w = reinterpret_cast< QWidget * >(ptr);

	if (mDlg) {
		mDlg->setParent(w, Qt::Dialog);
		mDlg->qpbUnhinge->setEnabled(true);
	} else {
		mDlg = new Manual(w);
	}

	mDlg->show();
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &identity) {
	if (!bLinkable)
		return false;

	if (!bActive) {
		memset(avatar_pos, 0, sizeof(float) * 3);
		memset(camera_pos, 0, sizeof(float) * 3);
		return true;
	}

	memcpy(avatar_pos, my.avatar_pos, sizeof(float) * 3);
	memcpy(avatar_front, my.avatar_front, sizeof(float) * 3);
	memcpy(avatar_top, my.avatar_top, sizeof(float) * 3);

	memcpy(camera_pos, my.camera_pos, sizeof(float) * 3);
	memcpy(camera_front, my.camera_front, sizeof(float) * 3);
	memcpy(camera_top, my.camera_top, sizeof(float) * 3);

	context.assign(my.context);
	identity.assign(my.identity);

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"This is the manual placement plugin. It allows you to place yourself manually.");
}

static std::wstring description(L"Manual placement plugin");
static std::wstring shortname(L"Manual placement");

static void about(void *ptr) {
	QWidget *w = reinterpret_cast< QWidget * >(ptr);

	QMessageBox::about(w, QString::fromStdWString(description), QString::fromStdWString(longdesc()));
}

static MumblePlugin manual = { MUMBLE_PLUGIN_MAGIC,
							   description,
							   shortname,
							   nullptr, // About is handled by MumblePluginQt
							   nullptr, // Config is handled by MumblePluginQt
							   trylock,
							   unlock,
							   longdesc,
							   fetch };

static MumblePluginQt manualqt = { MUMBLE_PLUGIN_MAGIC_QT, about, config };

MumblePlugin *ManualPlugin_getMumblePlugin() {
	return &manual;
}

MumblePluginQt *ManualPlugin_getMumblePluginQt() {
	return &manualqt;
}


/////////// Implementation of the ManualPlugin class //////////////
ManualPlugin::ManualPlugin(QObject *p) : LegacyPlugin(QString::fromLatin1("manual.builtin"), true, p) {
}

ManualPlugin::~ManualPlugin() {
}

void ManualPlugin::resolveFunctionPointers() {
	m_mumPlug   = &manual;
	m_mumPlugQt = &manualqt;
}
