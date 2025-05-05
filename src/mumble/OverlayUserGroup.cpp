// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayUserGroup.h"

#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"
#include "NetworkConfig.h"
#include "OverlayClient.h"
#include "OverlayEditor.h"
#include "OverlayText.h"
#include "OverlayUser.h"
#include "ServerHandler.h"
#include "User.h"
#include "Utils.h"
#include "Global.h"
#include "GlobalShortcut.h"

#include <QtGui/QImageReader>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>
#include <QtWidgets/QGraphicsSceneWheelEvent>
#include <QtWidgets/QInputDialog>

template< typename T > QRectF OverlayGroup::boundingRect() const {
	QRectF qr;
	foreach (const QGraphicsItem *item, childItems())
		if (item->isVisible() && (item->type() == T::Type))
			qr |= item->boundingRect().translated(item->pos());

	return qr;
}

OverlayUserGroup::OverlayUserGroup(OverlaySettings *osptr)
	: OverlayGroup(), os(osptr), qgeiHandle(nullptr), bShowExamples(false) {
}

OverlayUserGroup::~OverlayUserGroup() {
	reset();
}

void OverlayUserGroup::reset() {
	foreach (OverlayUser *ou, qlExampleUsers)
		delete ou;
	qlExampleUsers.clear();

	foreach (OverlayUser *ou, qmUsers)
		delete ou;
	qmUsers.clear();

	delete qgeiHandle;
	qgeiHandle = nullptr;
}

int OverlayUserGroup::type() const {
	return Type;
}

void OverlayUserGroup::contextMenuEvent(QGraphicsSceneContextMenuEvent *e) {
	e->accept();

#ifdef Q_OS_MAC
	bool embed = Global::get().ocIntercept;
	QMenu qm(embed ? nullptr : e->widget());
	if (embed) {
		QGraphicsScene *scene = Global::get().ocIntercept->qgv.scene();
		scene->addWidget(&qm);
	}
#else
	QMenu qm(Global::get().ocIntercept ? Global::get().mw : e->widget());
#endif

	QMenu *qmShow = qm.addMenu(OverlayClient::tr("Filter"));

	QAction *qaShowTalking = qmShow->addAction(OverlayClient::tr("Only talking"));
	qaShowTalking->setCheckable(true);
	if (os->osShow == OverlaySettings::Talking)
		qaShowTalking->setChecked(true);

	QAction *qaShowActive = qmShow->addAction(OverlayClient::tr("Talking and recently active"));
	qaShowActive->setCheckable(true);
	if (os->osShow == OverlaySettings::Active)
		qaShowActive->setChecked(true);

	QAction *qaShowHome = qmShow->addAction(OverlayClient::tr("All in current channel"));
	qaShowHome->setCheckable(true);
	if (os->osShow == OverlaySettings::HomeChannel)
		qaShowHome->setChecked(true);

	QAction *qaShowLinked = qmShow->addAction(OverlayClient::tr("All in linked channels"));
	qaShowLinked->setCheckable(true);
	if (os->osShow == OverlaySettings::LinkedChannels)
		qaShowLinked->setChecked(true);

	qmShow->addSeparator();

	QAction *qaShowSelf = qmShow->addAction(OverlayClient::tr("Always show yourself"));
	qaShowSelf->setCheckable(true);
	qaShowSelf->setEnabled(os->osShow == OverlaySettings::Talking || os->osShow == OverlaySettings::Active);
	if (os->bAlwaysSelf)
		qaShowSelf->setChecked(true);

	qmShow->addSeparator();

	QAction *qaConfigureRecentlyActiveTime =
		qmShow->addAction(OverlayClient::tr("Configure recently active time (%1 seconds)...").arg(os->uiActiveTime));
	qaConfigureRecentlyActiveTime->setEnabled(os->osShow == OverlaySettings::Active);

	QMenu *qmColumns = qm.addMenu(OverlayClient::tr("Columns"));
	QAction *qaColumns[6];
	for (unsigned int i = 1; i <= 5; ++i) {
		qaColumns[i] = qmColumns->addAction(QString::number(i));
		qaColumns[i]->setCheckable(true);
		qaColumns[i]->setChecked(i == os->uiColumns);
	}

	QMenu *qmSort = qm.addMenu(OverlayClient::tr("Sort"));

	QAction *qaSortAlphabetically = qmSort->addAction(OverlayClient::tr("Alphabetically"));
	qaSortAlphabetically->setCheckable(true);
	if (os->osSort == OverlaySettings::Alphabetical)
		qaSortAlphabetically->setChecked(true);

	QAction *qaSortLastStateChange = qmSort->addAction(OverlayClient::tr("Last state change"));
	qaSortLastStateChange->setCheckable(true);
	if (os->osSort == OverlaySettings::LastStateChange)
		qaSortLastStateChange->setChecked(true);

	QAction *qaEdit = qm.addAction(OverlayClient::tr("Edit..."));
	QAction *qaZoom = qm.addAction(OverlayClient::tr("Reset Zoom"));

	QAction *act = qm.exec(e->screenPos());

	if (!act)
		return;

	if (act == qaEdit) {
		if (Global::get().ocIntercept) {
			QMetaObject::invokeMethod(Global::get().ocIntercept, "openEditor", Qt::QueuedConnection);
		} else {
			OverlayEditor oe(qApp->activeModalWidget(), nullptr, os);
			connect(&oe, SIGNAL(applySettings()), this, SLOT(updateLayout()));
			oe.exec();
		}
	} else if (act == qaZoom) {
		os->fZoom = 1.0f;
		updateLayout();
	} else if (act == qaShowTalking) {
		os->osShow = OverlaySettings::Talking;
		updateUsers();
	} else if (act == qaShowActive) {
		os->osShow = OverlaySettings::Active;
		updateUsers();
	} else if (act == qaShowHome) {
		os->osShow = OverlaySettings::HomeChannel;
		updateUsers();
	} else if (act == qaShowLinked) {
		os->osShow = OverlaySettings::LinkedChannels;
		updateUsers();
	} else if (act == qaShowSelf) {
		os->bAlwaysSelf = !os->bAlwaysSelf;
		updateUsers();
	} else if (act == qaConfigureRecentlyActiveTime) {
		// FIXME: This might not be the best place to configure this setting, but currently
		// there's not really a suitable place to put this. In the future an additional tab
		// might be added for some advanced overlay options, which could then include this
		// setting.
		bool ok;
		int newValue = QInputDialog::getInt(qm.parentWidget(), OverlayClient::tr("Configure recently active time"),
											OverlayClient::tr("Amount of seconds users remain active after talking:"),
											os->uiActiveTime, 1, 2147483647, 1, &ok);
		if (ok) {
			os->uiActiveTime = newValue;
		}
		updateUsers();
	} else if (act == qaSortAlphabetically) {
		os->osSort = OverlaySettings::Alphabetical;
		updateUsers();
	} else if (act == qaSortLastStateChange) {
		os->osSort = OverlaySettings::LastStateChange;
		updateUsers();
	} else {
		for (unsigned int i = 1; i <= 5; ++i) {
			if (act == qaColumns[i]) {
				os->uiColumns = i;
				updateLayout();
			}
		}
	}
}

void OverlayUserGroup::wheelEvent(QGraphicsSceneWheelEvent *e) {
	e->accept();

	qreal scaleFactor = 0.875f;

	if (e->delta() > 0)
		scaleFactor = 1.0f / 0.875f;

	if ((scaleFactor < 1.0f) && (os->fZoom <= (1.0f / 4.0f)))
		return;
	else if ((scaleFactor > 1.0f) && (os->fZoom >= 4.0f))
		return;

	os->fZoom *= scaleFactor;

	updateLayout();
}

bool OverlayUserGroup::sceneEventFilter(QGraphicsItem *watched, QEvent *e) {
	switch (e->type()) {
		case QEvent::GraphicsSceneMouseMove:
		case QEvent::GraphicsSceneMouseRelease:
			QMetaObject::invokeMethod(this, "moveUsers", Qt::QueuedConnection);
			break;
		default:
			break;
	}
	return OverlayGroup::sceneEventFilter(watched, e);
}

void OverlayUserGroup::moveUsers() {
	if (!qgeiHandle)
		return;

	const QRectF &sr = scene()->sceneRect();
	const QPointF &p = qgeiHandle->pos();

	os->fX = static_cast< float >(qBound(0.0, p.x() / sr.width(), 1.0));
	os->fY = static_cast< float >(qBound(0.0, p.y() / sr.height(), 1.0));

	qgeiHandle->setPos(os->fX * sr.width(), os->fY * sr.height());
	updateUsers();
}

void OverlayUserGroup::updateLayout() {
	prepareGeometryChange();
	reset();
	updateUsers();
}

void OverlayUserGroup::updateUsers() {
	const QRectF &sr = scene()->sceneRect();

	unsigned int uiHeight = static_cast< unsigned int >(sr.height() + 0.5f);

	QList< QGraphicsItem * > items;
	foreach (QGraphicsItem *qgi, childItems())
		items << qgi;

	QList< OverlayUser * > users;
	if (bShowExamples) {
		if (qlExampleUsers.isEmpty()) {
			qlExampleUsers << new OverlayUser(Settings::Passive, uiHeight, os);
			qlExampleUsers << new OverlayUser(Settings::Talking, uiHeight, os);
			qlExampleUsers << new OverlayUser(Settings::MutedTalking, uiHeight, os);
			qlExampleUsers << new OverlayUser(Settings::Whispering, uiHeight, os);
			qlExampleUsers << new OverlayUser(Settings::Shouting, uiHeight, os);
		}

		users = qlExampleUsers;
		foreach (OverlayUser *ou, users)
			items.removeAll(ou);

		if (!qgeiHandle) {
			qgeiHandle = new QGraphicsEllipseItem(QRectF(-4.0f, -4.0f, 8.0f, 8.0f));
			qgeiHandle->setPen(QPen(Qt::darkRed, 0.0f));
			qgeiHandle->setBrush(Qt::red);
			qgeiHandle->setZValue(0.5f);
			qgeiHandle->setFlag(QGraphicsItem::ItemIsMovable);
			qgeiHandle->setFlag(QGraphicsItem::ItemIsSelectable);
			qgeiHandle->setPos(sr.width() * os->fX, sr.height() * os->fY);
			scene()->addItem(qgeiHandle);
			qgeiHandle->show();
			qgeiHandle->installSceneEventFilter(this);
		}
	} else {
		delete qgeiHandle;
		qgeiHandle = nullptr;
	}

	ClientUser *self = ClientUser::get(Global::get().uiSession);
	if (self) {
		QList< ClientUser * > showusers;
		Channel *home = ClientUser::get(Global::get().uiSession)->cChannel;

		switch (os->osShow) {
			case OverlaySettings::LinkedChannels:
				foreach (Channel *c, home->allLinks())
					foreach (User *p, c->qlUsers)
						showusers << static_cast< ClientUser * >(p);
				foreach (ClientUser *cu, ClientUser::getTalking())
					if (!showusers.contains(cu))
						showusers << cu;
				break;
			case OverlaySettings::HomeChannel:
				foreach (User *p, home->qlUsers)
					showusers << static_cast< ClientUser * >(p);
				foreach (ClientUser *cu, ClientUser::getTalking())
					if (!showusers.contains(cu))
						showusers << cu;
				break;
			case OverlaySettings::Active:
				showusers = ClientUser::getActive();
				if (os->bAlwaysSelf && !showusers.contains(self))
					showusers << self;
				break;
			default:
				showusers = ClientUser::getTalking();
				if (os->bAlwaysSelf && (self->tsState == Settings::Passive))
					showusers << self;
				break;
		}

		ClientUser::sortUsersOverlay(showusers);

		foreach (ClientUser *cu, showusers) {
			OverlayUser *ou = qmUsers.value(cu);
			if (!ou) {
				ou = new OverlayUser(cu, uiHeight, os);
				connect(cu, SIGNAL(destroyed(QObject *)), this, SLOT(userDestroyed(QObject *)));
				qmUsers.insert(cu, ou);
				ou->hide();
			} else {
				items.removeAll(ou);
			}
			users << ou;
		}
	}

	foreach (QGraphicsItem *qgi, items) {
		scene()->removeItem(qgi);
		qgi->hide();
	}

	QRectF childrenBounds = os->qrfAvatar | os->qrfChannel | os->qrfMutedDeafened | os->qrfUserName;

	int pad    = os->bBox ? static_cast< int >(uiHeight * os->fZoom * (os->fBoxPad + os->fBoxPenWidth) + 0.5f) : 0;
	int width  = static_cast< int >(childrenBounds.width() * uiHeight * os->fZoom + 0.5f) + 2 * pad;
	int height = static_cast< int >(childrenBounds.height() * uiHeight * os->fZoom + 0.5f) + 2 * pad;

	int xOffset = -static_cast< int >(childrenBounds.left() * uiHeight * os->fZoom + 0.5f) + pad;
	int yOffset = -static_cast< int >(childrenBounds.top() * uiHeight * os->fZoom + 0.5f) + pad;

	int yPos = 0;
	int xPos = 0;

	foreach (OverlayUser *ou, users) {
		if (!ou->parentItem())
			ou->setParentItem(this);

		ou->setPos(xPos * (width + 4) + xOffset, yPos * (height + 4) + yOffset);
		ou->updateUser();
		ou->show();

		if (static_cast< unsigned int >(xPos) >= (os->uiColumns - 1)) {
			xPos = 0;
			++yPos;
		} else {
			++xPos;
		}
	}

	QRectF br = boundingRect< OverlayUser >();

	int basex = qBound< int >(0, static_cast< int >(sr.width() * os->fX + 0.5f),
							  static_cast< int >(sr.width() - br.width() + 0.5f));
	int basey = qBound< int >(0, static_cast< int >(sr.height() * os->fY + 0.5f),
							  static_cast< int >(sr.height() - br.height() + 0.5f));

	setPos(basex, basey);
}

void OverlayUserGroup::userDestroyed(QObject *obj) {
	OverlayUser *ou = qmUsers.take(obj);
	delete ou;
}
