/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "Overlay.h"
#include "User.h"
#include "Channel.h"
#include "Global.h"
#include "Message.h"
#include "Database.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "GlobalShortcut.h"

static ConfigWidget *OverlayConfigDialogNew(Settings &st) {
	return new OverlayConfig(st);
}

static ConfigRegistrar registrar(6000, OverlayConfigDialogNew);

OverlayConfig::OverlayConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	qcbShow->addItem(tr("Show no one"), Settings::Nothing);
	qcbShow->addItem(tr("Show only talking"), Settings::Talking);
	qcbShow->addItem(tr("Show everyone"), Settings::All);
}

void OverlayConfig::load(const Settings &r) {
	loadCheckBox(qcbEnable, r.bOverlayEnable);
	loadCheckBox(qcbAlwaysSelf, r.bOverlayAlwaysSelf);
	loadComboBox(qcbShow, r.osOverlay);

	loadSlider(qsX, iroundf(r.os.fX * 100.0f));
	loadSlider(qsY, 100 - iroundf(r.os.fY * 100.0f));
}

bool OverlayConfig::expert(bool) {
	return true;
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

QIcon OverlayConfig::icon() const {
	return QIcon(QLatin1String("skin:config_osd.png"));
}

void OverlayConfig::save() const {
	s.bOverlayEnable = qcbEnable->isChecked();
	s.osOverlay = static_cast<Settings::OverlayShow>(qcbShow->currentIndex());
	s.bOverlayAlwaysSelf = qcbAlwaysSelf->isChecked();
	s.os.fX = static_cast<float>(qsX->value()) / 100.0f;
	s.os.fY = 1.0f - static_cast<float>(qsY->value()) / 100.0f;
}

void OverlayConfig::accept() const {
	g.o->forceSettings();
	g.o->setActive(s.bOverlayEnable);
}



OverlayEditorScene::OverlayEditorScene(QObject *p) : QGraphicsScene(p) {
	os = g.s.os;
	
	tsColor = Settings::Talking;
	uiZoom = 2;
	
	if (g.ocIntercept)
		uiSize = g.ocIntercept->uiHeight;
	else
		uiSize = 1080.f;

	qgiGroup = new OverlayGroup();
	qgiGroup->setAcceptHoverEvents(true);
	qgiGroup->setPos(0.0f, 0.0f);
	addItem(qgiGroup);

	qgpiMuted = new QGraphicsPixmapItem(qgiGroup);
	qgpiMuted->hide();

	qgpiAvatar = new QGraphicsPixmapItem(qgiGroup);
	qgpiAvatar->hide();

	qgpiName = new QGraphicsPixmapItem(qgiGroup);
	qgpiName->hide();

	qgpiChannel = new QGraphicsPixmapItem(qgiGroup);
	qgpiChannel->hide();
	
	qgpiBox = new QGraphicsPathItem(qgiGroup);
	qgpiBox->hide();
	
	qgpiSelected = NULL;

	qgriSelected = new QGraphicsRectItem;
	qgriSelected->hide();

	qgriSelected->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
	qgriSelected->setOpacity(1.0f);
	qgriSelected->setBrush(Qt::NoBrush);
	qgriSelected->setPen(QPen(Qt::black, 4.0f));
	qgriSelected->setZValue(5.0f);
	
	addItem(qgriSelected);

	qgpiChannel->setZValue(2.0f);
	qgpiName->setZValue(1.0f);
	qgpiMuted->setZValue(3.0f);

	qgpiBox->setZValue(-1.0f);

	resync();
}

#define SCALESIZE(var) iroundf(uiSize * uiZoom * os.qrf##var .height()), iroundf(uiSize * uiZoom * os.qrf##var .width())

void OverlayEditorScene::updateMuted() {
	QImageReader qir(QLatin1String("skin:muted_self.svg"));
	QSize sz = qir.size();
	sz.scale(SCALESIZE(MutedDeafened), Qt::KeepAspectRatio);
	qir.setScaledSize(sz);
	qgpiMuted->setPixmap(QPixmap::fromImage(qir.read()));

	moveMuted();
}

void OverlayEditorScene::moveMuted() {
	qgpiMuted->setVisible(os.bMutedDeafened);
	qgpiMuted->setPos(OverlayUser::alignedPosition(OverlayUser::scaledRect(os.qrfMutedDeafened, uiSize * uiZoom), qgpiMuted->boundingRect(), os.qaMutedDeafened));
	qgpiMuted->setOpacity(os.fMutedDeafened);
}

void OverlayEditorScene::updateUserName() {
	QString qsName;
	
	switch (tsColor) {
		case Settings::Passive:
			qsName = Overlay::tr("Silent");
			break;
		case Settings::Talking:
			qsName = Overlay::tr("Talking");
			break;
		case Settings::WhisperPrivate:
			qsName = Overlay::tr("Whisper");
			break;
		case Settings::WhisperChannel:
			qsName = Overlay::tr("Shout");
			break;
	}

	QPainterPath pp;

	const QPixmap &pm = OverlayUser::createPixmap(qsName, SCALESIZE(UserName), os.qcUserName[tsColor], os.qfUserName, pp);
	qgpiName->setPixmap(pm);
	
	moveUserName();
}

void OverlayEditorScene::moveUserName() {
	qgpiName->setVisible(os.bUserName);
	qgpiName->setPos(OverlayUser::alignedPosition(OverlayUser::scaledRect(os.qrfUserName, uiSize * uiZoom), qgpiName->boundingRect(), os.qaUserName));
	qgpiName->setOpacity(os.fUserName);
}

void OverlayEditorScene::updateChannel() {
	QPainterPath pp;
	const QPixmap &pm = OverlayUser::createPixmap(Overlay::tr("Channel"), SCALESIZE(Channel), os.qcChannel, os.qfChannel, pp);
	qgpiChannel->setPixmap(pm);

	moveChannel();
}

void OverlayEditorScene::moveChannel() {
	qgpiChannel->setVisible(os.bChannel);
	qgpiChannel->setPos(OverlayUser::alignedPosition(OverlayUser::scaledRect(os.qrfChannel, uiSize * uiZoom), qgpiChannel->boundingRect(), os.qaChannel));
	qgpiChannel->setOpacity(os.fChannel);
}

void OverlayEditorScene::updateAvatar() {
	QImage img;
	QImageReader qir(QLatin1String("skin:default_avatar.svg"));
	QSize sz = qir.size();
	sz.scale(SCALESIZE(Avatar), Qt::KeepAspectRatio);
	qir.setScaledSize(sz);
	img = qir.read();
	qgpiAvatar->setPixmap(QPixmap::fromImage(img));

	moveAvatar();
}

void OverlayEditorScene::moveAvatar() {
	qgpiAvatar->setVisible(os.bAvatar);
	qgpiAvatar->setPos(OverlayUser::alignedPosition(OverlayUser::scaledRect(os.qrfAvatar, uiSize * uiZoom), qgpiAvatar->boundingRect(), os.qaAvatar));
	qgpiAvatar->setOpacity(os.fAvatar);
}

void OverlayEditorScene::moveBox() {
	QRectF children = os.qrfAvatar | os.qrfChannel | os.qrfMutedDeafened | os.qrfUserName;
	
	bool haspen = (os.qcBoxPen != os.qcBoxFill) && (! qFuzzyCompare(os.qcBoxPen.alphaF(), static_cast<qreal>(0.0f)));
	qreal pw = haspen ? qMax<qreal>(1.0f, os.fBoxPenWidth * uiSize * uiZoom) : 0.0f;
	qreal pad = os.fBoxPad * uiSize * uiZoom;

	QPainterPath pp;
	pp.addRoundedRect(children.x() * uiSize * uiZoom + -pw / 2.0f - pad, children.y() * uiSize * uiZoom + -pw / 2.0f - pad, children.width() * uiSize * uiZoom + pw + 2.0f * pad, children.height() * uiSize * uiZoom + pw + 2.0f * pad, 2.0f * pw, 2.0f * pw);
	qgpiBox->setPath(pp);
	qgpiBox->setPos(0.0f, 0.0f);
	qgpiBox->setPen(haspen ? QPen(os.qcBoxPen, pw) : Qt::NoPen);
	qgpiBox->setBrush(qFuzzyCompare(os.qcBoxFill.alphaF(), static_cast<qreal>(0.0f)) ? Qt::NoBrush : os.qcBoxFill);
	qgpiBox->setOpacity(1.0f);

	qgpiBox->setVisible(os.bBox);
}

void OverlayEditorScene::updateSelected() {
	if (qgpiSelected == qgpiAvatar)
		updateAvatar();
	else if (qgpiSelected == qgpiName)
		updateUserName();
	else if (qgpiSelected == qgpiAvatar)
		updateAvatar();
	else if (qgpiSelected == qgpiMuted)
		updateMuted();
}

void OverlayEditorScene::resync() {
 	QRadialGradient gradient(0, 0, 10 * uiZoom);
 	gradient.setSpread(QGradient::ReflectSpread);
 	gradient.setColorAt(0.0f, QColor(255, 255, 255, 64));
 	gradient.setColorAt(0.2f, QColor(0, 0, 0, 64));
 	gradient.setColorAt(0.4f, QColor(255, 128, 0, 64));
 	gradient.setColorAt(0.6f, QColor(0, 0, 0, 64));
 	gradient.setColorAt(0.8f, QColor(0, 128, 255, 64));
 	gradient.setColorAt(1.0f, QColor(0, 0, 0, 64));
 	setBackgroundBrush(gradient);

	updateMuted();
	updateUserName();
	updateChannel();
	updateAvatar();
	
	moveMuted();
	moveUserName();
	moveChannel();
	moveAvatar();
	
	moveBox();

	qgiGroup->setOpacity(os.fUser[tsColor]);
	
	qgpiSelected = NULL;
	qgriSelected->setVisible(false);
}

void OverlayEditorScene::drawBackground(QPainter *p, const QRectF &rect) {
	p->setBrushOrigin(0, 0);
	p->fillRect(rect, backgroundBrush());
	
	QRectF upscaled = OverlayUser::scaledRect(rect, 128.f / (uiSize * uiZoom) );
	
	{
		int min = iroundf(upscaled.left() - 0.5f);
		int max = iroundf(upscaled.right() + 0.5f);

		for(int i=min;i<=max;++i) {
			qreal v = (i / 128.f) * uiSize * uiZoom;

			if (i != 0)
				p->setPen(QPen(QColor(128, 128, 128, 255), 0.0f));
			else
				p->setPen(QPen(QColor(0, 0, 0, 255), 2.0f));

			p->drawLine(QPointF(v, rect.top()), QPointF(v, rect.bottom()));
		}
	}

	{
		int min = iroundf(upscaled.top() - 0.5f);
		int max = iroundf(upscaled.bottom() + 0.5f);

		for(int i=min;i<=max;++i) {
			qreal v = (i / 128.f) * uiSize * uiZoom;

			if (i != 0)
				p->setPen(QPen(QColor(128, 128, 128, 255), 0.0f));
			else
				p->setPen(QPen(QColor(0, 0, 0, 255), 2.0f));

			p->drawLine(QPointF(rect.left(), v), QPointF(rect.right(), v));
		}
	}
}

QGraphicsPixmapItem *OverlayEditorScene::childAt(const QPointF &pos) {
	QGraphicsItem *item = NULL;

	if (qgriSelected->isVisible()) {
		if (qgriSelected->rect().contains(pos)) {
			return qgpiSelected;
		}
	}

	foreach(QGraphicsItem *qgi, items(Qt::AscendingOrder)) {
		if (!qgi->isVisible() || ! qgraphicsitem_cast<QGraphicsPixmapItem *>(qgi))
			continue;

		QPointF qp = pos - qgi->pos();
		if (qgi->contains(qp)) {
			item = qgi;
		}
	}
	return static_cast<QGraphicsPixmapItem *>(item);
}

QRectF OverlayEditorScene::selectedRect() const {
	const QRectF *qrf = NULL;

	if (qgpiSelected == qgpiMuted)
		qrf = & os.qrfMutedDeafened;
	else if (qgpiSelected == qgpiAvatar)
		qrf = & os.qrfAvatar;
	else if (qgpiSelected == qgpiChannel)
		qrf = & os.qrfChannel;
	else if (qgpiSelected == qgpiName)
		qrf = & os.qrfUserName;

	if (! qrf)
		return QRectF();

	return OverlayUser::scaledRect(*qrf, uiSize * uiZoom).toAlignedRect();
}


void OverlayEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mousePressEvent(event);

	if (event->isAccepted())
		return;

	if (event->button() == Qt::LeftButton) {
		event->accept();

		if (wfsHover == Qt::NoSection) {
			qgpiSelected = childAt(event->scenePos());
			if (qgpiSelected) {
				qgriSelected->setRect(selectedRect());
				qgriSelected->show();
			} else {
				qgriSelected->hide();
			}
		}

		updateCursorShape(event->scenePos());
	}
}

void OverlayEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseReleaseEvent(event);

	if (event->isAccepted())
		return;

	if (event->button() == Qt::LeftButton) {
		event->accept();

		QRectF rect = qgriSelected->rect();

		if (! qgpiSelected || (rect == selectedRect())) {
			return;
		}
		
		QRectF scaled(rect.x() / (uiSize * uiZoom), rect.y() / (uiSize * uiZoom), rect.width() / (uiSize * uiZoom), rect.height() / (uiSize * uiZoom));

		if (qgpiSelected == qgpiMuted) {
			os.qrfMutedDeafened = scaled;
			updateMuted();
		} else if (qgpiSelected == qgpiAvatar) {
			os.qrfAvatar = scaled;
			updateAvatar();
		} else if (qgpiSelected == qgpiChannel) {
			os.qrfChannel = scaled;
			updateChannel();
		} else if (qgpiSelected == qgpiName) {
			os.qrfUserName = scaled;
			updateUserName();
		}
		
		moveBox();
	}
}

void OverlayEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseMoveEvent(event);

	if (event->isAccepted())
		return;

	if (qgpiSelected && (event->buttons() & Qt::LeftButton)) {
		event->accept();

		if (wfsHover == Qt::NoSection)
			return;

		QPointF delta = event->scenePos() - event->buttonDownScenePos(Qt::LeftButton);

		bool square = event->modifiers() & Qt::ShiftModifier;

		QRectF orig = selectedRect();
		switch (wfsHover) {
			case Qt::TitleBarArea:
				orig.translate(delta);
				break;
			case Qt::TopSection:
				orig.setTop(orig.top() + delta.y());
				if (orig.height() < 8.0f)
					orig.setTop(orig.bottom() - 8.0f);
				if (square)
					orig.setRight(orig.left() + orig.height());
				break;
			case Qt::BottomSection:
				orig.setBottom(orig.bottom() + delta.y());
				if (orig.height() < 8.0f)
					orig.setBottom(orig.top() + 8.0f);
				if (square)
					orig.setRight(orig.left() + orig.height());
				break;
			case Qt::LeftSection:
				orig.setLeft(orig.left() + delta.x());
				if (orig.width() < 8.0f)
					orig.setLeft(orig.right() - 8.0f);
				if (square)
					orig.setBottom(orig.top() + orig.width());
				break;
			case Qt::RightSection:
				orig.setRight(orig.right() + delta.x());
				if (orig.width() < 8.0f)
					orig.setRight(orig.left() + 8.0f);
				if (square)
					orig.setBottom(orig.top() + orig.width());
				break;
			case Qt::TopLeftSection:
				orig.setTopLeft(orig.topLeft() + delta);
				if (orig.height() < 8.0f)
					orig.setTop(orig.bottom() - 8.0f);
				if (orig.width() < 8.0f)
					orig.setLeft(orig.right() - 8.0f);
				if (square) {
					qreal size = qMin(orig.width(), orig.height());
					QPointF sz(-size, -size);
					orig.setTopLeft(orig.bottomRight() + sz);
				}
				break;
			case Qt::TopRightSection:
				orig.setTopRight(orig.topRight() + delta);
				if (orig.height() < 8.0f)
					orig.setTop(orig.bottom() - 8.0f);
				if (orig.width() < 8.0f)
					orig.setRight(orig.left() + 8.0f);
				if (square) {
					qreal size = qMin(orig.width(), orig.height());
					QPointF sz(size, -size);
					orig.setTopRight(orig.bottomLeft() + sz);
				}
				break;
			case Qt::BottomLeftSection:
				orig.setBottomLeft(orig.bottomLeft() + delta);
				if (orig.height() < 8.0f)
					orig.setBottom(orig.top() + 8.0f);
				if (orig.width() < 8.0f)
					orig.setLeft(orig.right() - 8.0f);
				if (square) {
					qreal size = qMin(orig.width(), orig.height());
					QPointF sz(-size, size);
					orig.setBottomLeft(orig.topRight() + sz);
				}
				break;
			case Qt::BottomRightSection:
				orig.setBottomRight(orig.bottomRight() + delta);
				if (orig.height() < 8.0f)
					orig.setBottom(orig.top() + 8.0f);
				if (orig.width() < 8.0f)
					orig.setRight(orig.left() + 8.0f);
				if (square) {
					qreal size = qMin(orig.width(), orig.height());
					QPointF sz(size, size);
					orig.setBottomRight(orig.topLeft() + sz);
				}
				break;
		}
		
		qgriSelected->setRect(orig);
	} else {
		updateCursorShape(event->scenePos());
	}
}

void OverlayEditorScene::updateCursorShape(const QPointF &point) {
	Qt::CursorShape	cs;

	if (qgriSelected->isVisible()) {
		wfsHover = rectSection(qgriSelected->rect(), point);
	} else {
		wfsHover = Qt::NoSection;
	}
	
	switch (wfsHover) {
		case Qt::TopLeftSection:
		case Qt::BottomRightSection:
			cs = Qt::SizeFDiagCursor;
			break;
		case Qt::TopRightSection:
		case Qt::BottomLeftSection:
			cs = Qt::SizeBDiagCursor;
			break;
		case Qt::TopSection:
		case Qt::BottomSection:
			cs = Qt::SizeVerCursor;
			break;
		case Qt::LeftSection:
		case Qt::RightSection:
			cs = Qt::SizeHorCursor;
			break;
		case Qt::TitleBarArea:
			cs = Qt::OpenHandCursor;
			break;
		default:
			cs = Qt::ArrowCursor;
			break;
	}


	foreach(QGraphicsView *v, views()) {
		if (v->viewport()->cursor().shape() != cs) {
			v->viewport()->setCursor(cs);

			// But an embedded, injected GraphicsView doesn't propagage mouse cursors...
			QWidget *p = v->parentWidget();
			if (p) {
				QGraphicsProxyWidget *qgpw = p->graphicsProxyWidget();
				if (qgpw) {
					qgpw->setCursor(cs);
					if (g.ocIntercept)
						g.ocIntercept->updateMouse();
				}
			}
		}
	}
}

void OverlayEditorScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	QGraphicsScene::contextMenuEvent(event);

	if (event->isAccepted())
		return;

	if (! event->widget())
		return;

	QGraphicsPixmapItem *item = childAt(event->scenePos());
	
	QMenu qm(event->widget());
	
	QMenu *qmLayout = qm.addMenu(tr("Layout preset"));
	QAction *qaLayoutLargeAvatar = qmLayout->addAction(tr("Large square avatar"));
	QAction *qaLayoutText = qmLayout->addAction(tr("Avatar and Name"));

	QMenu *qmTrans = qm.addMenu(tr("User Opacity"));
	QActionGroup *qagUser = new QActionGroup(&qm);
	QAction *userOpacity[8];
	for (int i=0;i<8;++i) {
		qreal o = (i + 1) / 8.0f;

		userOpacity[i] = new QAction(tr("%1%").arg(o * 100.0f, 0, 'f', 1), qagUser);
		userOpacity[i]->setCheckable(true);
		userOpacity[i]->setData(o);

		if (qFuzzyCompare(qgiGroup->opacity(), o))
			userOpacity[i]->setChecked(true);

		qmTrans->addAction(userOpacity[i]);
	}

	QAction *color = NULL;
	QAction *font = NULL;
	QAction *objectOpacity[8];
	for(int i=0;i<8;++i)
		objectOpacity[i] = NULL;
	QAction *boxpen[4] = { NULL, NULL, NULL, NULL};
	QAction *boxpad[4] = { NULL, NULL, NULL, NULL};
	QAction *boxpencolor = NULL;
	QAction *boxfillcolor = NULL;
	
	QAction *align[6];
	for(int i=0;i<6;++i)
		align[i] = NULL;

	if (item) {
		qm.addSeparator();
		QMenu *qmObjTrans = qm.addMenu(tr("Object Opacity"));
		QActionGroup *qagObject = new QActionGroup(&qm);
		for (int i=0;i<8;++i) {
			qreal o = (i + 1) / 8.0f;

			objectOpacity[i] = new QAction(tr("%1%").arg(o * 100.0f, 0, 'f', 1), qagObject);
			objectOpacity[i]->setCheckable(true);
			objectOpacity[i]->setData(o);
			if (qFuzzyCompare(item->opacity(), o))
				objectOpacity[i]->setChecked(true);
			qmObjTrans->addAction(objectOpacity[i]);
		}
		
		QMenu *qmObjAlign = qm.addMenu(tr("Alignment"));
		Qt::Alignment a;
		if (item == qgpiAvatar)
			a = os.qaAvatar;
		else if (item == qgpiChannel)
			a = os.qaChannel;
		else if (item == qgpiMuted)
			a = os.qaMutedDeafened;
		else
			a = os.qaUserName;

		align[0] = qmObjAlign->addAction(tr("Left"));
		align[0]->setCheckable(true);
		align[0]->setData(Qt::AlignLeft);
		if (a & Qt::AlignLeft)
			align[0]->setChecked(true);
		align[1] = qmObjAlign->addAction(tr("Center"));
		align[1]->setCheckable(true);
		align[1]->setData(Qt::AlignHCenter);
		if (a & Qt::AlignHCenter)
			align[1]->setChecked(true);
		align[2] = qmObjAlign->addAction(tr("Right"));
		align[2]->setCheckable(true);
		align[2]->setData(Qt::AlignRight);
		if (a & Qt::AlignRight)
			align[2]->setChecked(true);
			
		qmObjAlign->addSeparator();

		align[3] = qmObjAlign->addAction(tr("Top"));
		align[3]->setCheckable(true);
		align[3]->setData(Qt::AlignTop);
		if (a & Qt::AlignTop)
			align[3]->setChecked(true);
		align[4] = qmObjAlign->addAction(tr("Center"));
		align[4]->setCheckable(true);
		align[4]->setData(Qt::AlignVCenter);
		if (a & Qt::AlignVCenter)
			align[4]->setChecked(true);
		align[5] = qmObjAlign->addAction(tr("Bottom"));
		align[5]->setCheckable(true);
		align[5]->setData(Qt::AlignBottom);
		if (a & Qt::AlignBottom)
			align[5]->setChecked(true);

		if ((item != qgpiAvatar) && (item != qgpiMuted)) {
			color = qm.addAction(tr("Color..."));
			font = qm.addAction(tr("Font..."));
		}
	}
	
	if (qgpiBox->isVisible()) {
		qm.addSeparator();
		QMenu *qmBox = qm.addMenu(tr("Bounding box"));
		QMenu *qmPen = qmBox->addMenu(tr("Pen width"));
		QMenu *qmPad = qmBox->addMenu(tr("Padding"));
		boxpencolor = qmBox->addAction(tr("Pen color"));
		boxfillcolor = qmBox->addAction(tr("Fill color"));
		
		QActionGroup *qagPen = new QActionGroup(qmPen);
		QActionGroup *qagPad = new QActionGroup(qmPad);
		for(int i=0;i<4;++i) {
			qreal v = (i) ? powf(2.0f, -10 + i) : 0.0f;
			boxpen[i] = new QAction(QString::number(i), qagPen);
			boxpen[i]->setData(v);
			boxpen[i]->setCheckable(true);
			if (qFuzzyCompare(os.fBoxPenWidth, v))
				boxpen[i]->setChecked(true);
			qmPen->addAction(boxpen[i]);

			boxpad[i] = new QAction(QString::number(i), qagPad);
			boxpad[i]->setData(v);
			boxpad[i]->setCheckable(true);
			if (qFuzzyCompare(os.fBoxPad, v))
				boxpad[i]->setChecked(true);
			qmPad->addAction(boxpad[i]);
		}
	}

	QAction *act = qm.exec(event->screenPos());
	
	if (! act)
		return;

	for (int i=0;i<8;++i) {
		if (userOpacity[i] == act) {
			float o = act->data().toReal();
			os.fUser[tsColor] = o;
			
			qgiGroup->setOpacity(o);
		}
	}

	for (int i=0;i<8;++i) {
		if (objectOpacity[i] == act) {
			qreal o = act->data().toReal();

			if (item == qgpiMuted)
				os.fMutedDeafened = o;
			else if (item == qgpiAvatar)
				os.fAvatar = o;
			else if (item == qgpiChannel)
				os.fChannel = o;
			else if (item == qgpiName)
				os.fUserName = o;
				
			item->setOpacity(o);
		}
	}

	for(int i=0;i<4;++i) {
		if (boxpen[i] == act) {
			os.fBoxPenWidth = act->data().toReal();
			moveBox();
		} else if (boxpad[i] == act) {
			os.fBoxPad = act->data().toReal();
			moveBox();
		}
	}

	for(int i=0;i<6;++i) {
		if (align[i] == act) {
			Qt::Alignment *aptr;
			if (item == qgpiAvatar)
				aptr = & os.qaAvatar;
			else if (item == qgpiChannel)
				aptr = & os.qaChannel;
			else if (item == qgpiMuted)
				aptr = & os.qaMutedDeafened;
			else
				aptr = & os.qaUserName;

			Qt::Alignment a = act->data().toInt();
			if (a & Qt::AlignHorizontal_Mask) {
				*aptr = (*aptr & ~Qt::AlignHorizontal_Mask) | a;
			} else {
				*aptr = (*aptr & ~Qt::AlignVertical_Mask) | a;
			}
			
			updateSelected();
		}
	}

	if (act == boxpencolor) {
		QColor qc = QColorDialog::getColor(os.qcBoxPen, event->widget(), tr("Pick pen color"), QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
		if (! qc.isValid())
			return;
		os.qcBoxPen = qc;
		moveBox();
	} else if (act == boxfillcolor) {
		QColor qc = QColorDialog::getColor(os.qcBoxFill, event->widget(), tr("Pick fill color"), QColorDialog::DontUseNativeDialog | QColorDialog::ShowAlphaChannel);
		if (! qc.isValid())
			return;
		os.qcBoxFill = qc;
		moveBox();
	} else if (act == color) {
		QColor *col = NULL;
		if (item == qgpiChannel)
			col = & os.qcChannel;
		else if (item == qgpiName)
				col = & os.qcUserName[tsColor];
		if (! col)
			return;

		QColor qc = QColorDialog::getColor(*col, event->widget(), tr("Pick color"), QColorDialog::DontUseNativeDialog);
		if (! qc.isValid())
			return;
		qc.setAlpha(255);

		if (qc == *col)
			return;

		*col = qc;
		updateSelected();
	} else if (act == font) {
		QFont *fontptr = (item == qgpiChannel) ? &os.qfChannel : &os.qfUserName;
		
		qgpiSelected = NULL;
		qgriSelected->hide();

		// QFontDialog doesn't really like graphics view. At all.

		QFontDialog qfd;
		qfd.setOptions(QFontDialog::DontUseNativeDialog);
		qfd.setCurrentFont(*fontptr);
		qfd.setWindowTitle(tr("Pick font"));
		
		QGraphicsProxyWidget *qgpw = new QGraphicsProxyWidget(NULL, Qt::Window);
		qgpw->setWidget(&qfd);
		
		addItem(qgpw);

		qgpw->setZValue(3.0f);
		qgpw->setPanelModality(QGraphicsItem::PanelModal);
		qgpw->setPos(- qgpw->boundingRect().width() / 2.0f, - qgpw->boundingRect().height() / 2.0f);
		qgpw->show();

		int ret = qfd.exec();

		qgpw->hide();
		qgpw->setWidget(NULL);
		delete qgpw;

		if (! ret)
			return;
		*fontptr = qfd.selectedFont();

		resync();
	} else if (act == qaLayoutLargeAvatar) {
		os.fUserName = 0.75f;
		os.fChannel = 0.75f;
		os.fMutedDeafened = 0.5f;
		os.fAvatar = 1.0f;

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
		os.qfUserName = QFont(QLatin1String("Verdana"), 20);
#else
		os.qfUserName = QFont(QLatin1String("Arial"), 20);
#endif
		os.qfChannel = os.qfUserName;

		os.fUser[Settings::Passive] = 0.5f;
		os.fUser[Settings::Talking] = (7.0f / 8.0f);
		os.fUser[Settings::WhisperPrivate] = (7.0f / 8.0f);
		os.fUser[Settings::WhisperChannel] = (7.0f / 8.0f);

		os.qrfUserName = QRectF(-0.0625f, 0.101563f - 0.0625f, 0.125f, 0.023438f);
		os.qrfChannel = QRectF(-0.03125f, -0.0625f, 0.09375f, 0.015625f);
		os.qrfMutedDeafened = QRectF(-0.0625f, -0.0625f, 0.0625f, 0.0625f);
		os.qrfAvatar = QRectF(-0.0625f, -0.0625f, 0.125f, 0.125f);

		os.qcBoxPen = QColor(0, 0, 0, 224);
		os.qcBoxFill = QColor(128, 128, 128, 128);
		os.fBoxPenWidth = (1.f / 256.0f);
		os.fBoxPad = (1.f / 256.0f);

		os.bUserName = true;
		os.bChannel = true;
		os.bMutedDeafened = true;
		os.bAvatar = true;
		os.bBox = false;

		os.qaUserName = Qt::AlignCenter;
		os.qaMutedDeafened = Qt::AlignLeft | Qt::AlignTop;
		os.qaAvatar = Qt::AlignCenter;
		os.qaChannel = Qt::AlignCenter;
		
		resync();
	} else if (act == qaLayoutText) {
		os.fUserName = 1.0f;
		os.fChannel = (7.0f / 8.0f);
		os.fMutedDeafened = (7.0f / 8.0f);
		os.fAvatar = 1.0f;

#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
		os.qfUserName = QFont(QLatin1String("Verdana"), 20);
#else
		os.qfUserName = QFont(QLatin1String("Arial"), 20);
#endif
		os.qfChannel = os.qfUserName;

		os.fUser[Settings::Passive] = 0.5f;
		os.fUser[Settings::Talking] = (7.0f / 8.0f);
		os.fUser[Settings::WhisperPrivate] = (7.0f / 8.0f);
		os.fUser[Settings::WhisperChannel] = (7.0f / 8.0f);

		os.qrfUserName = QRectF(0.0f, -0.015625f, 0.250f, 0.03125f);
		os.qrfChannel = QRectF(0.03125f, -0.015625f, 0.1875f, 0.015625f);
		os.qrfMutedDeafened = QRectF(-0.03125f, -0.015625f, 0.03125f, 0.03125f);
		os.qrfAvatar = QRectF(-0.03125f, -0.015625f, 0.03125f, 0.03125f);

		os.qcBoxPen = QColor(0, 0, 0, 224);
		os.qcBoxFill = QColor(128, 128, 128, 128);
		os.fBoxPenWidth = 0.0f;
		os.fBoxPad = (1.f / 512.0f);

		os.bUserName = true;
		os.bChannel = false;
		os.bMutedDeafened = true;
		os.bAvatar = true;
		os.bBox = true;

		os.qaUserName = Qt::AlignLeft | Qt::AlignVCenter;
		os.qaMutedDeafened = Qt::AlignRight | Qt::AlignVCenter;
		os.qaAvatar = Qt::AlignRight | Qt::AlignVCenter;
		os.qaChannel = Qt::AlignLeft | Qt::AlignTop;
		
		resync();
	}
}

static qreal distancePointLine(const QPointF &a, const QPointF &b, const QPointF &p) {
	qreal xda = a.x() - p.x();
	qreal xdb = p.x() - b.x();

	qreal xd = 0;

	if (xda > 0)
		xd = xda;
	if (xdb > 0)
		xd = qMax(xd, xdb);

	qreal yda = a.y() - p.y();
	qreal ydb = p.y() - b.y();

	qreal yd = 0;

	if (yda > 0)
		yd = yda;
	if (ydb > 0)
		yd = qMax(yd, ydb);

	return qMax(xd, yd);
}

Qt::WindowFrameSection OverlayEditorScene::rectSection(const QRectF &qrf, const QPointF &qp, qreal dist) {
	qreal left, right, top, bottom;

	top = distancePointLine(qrf.topLeft(), qrf.topRight(), qp);
	bottom = distancePointLine(qrf.bottomLeft(), qrf.bottomRight(), qp);
	left = distancePointLine(qrf.topLeft(), qrf.bottomLeft(), qp);
	right = distancePointLine(qrf.topRight(), qrf.bottomRight(), qp);

	if ((top < dist) && (top < bottom)) {
		if ((left < dist) && (left < right))
			return Qt::TopLeftSection;
		else if (right < dist)
			return Qt::TopRightSection;
		return Qt::TopSection;
	} else if (bottom < dist) {
		if ((left < dist) && (left < right))
			return Qt::BottomLeftSection;
		else if (right < dist)
			return Qt::BottomRightSection;
		return Qt::BottomSection;
	} else if (left < dist) {
		return Qt::LeftSection;
	} else if (right < dist) {
		return Qt::RightSection;
	}
	if (qrf.contains(qp))
		return Qt::TitleBarArea;

	return Qt::NoSection;
}



OverlayEditor::OverlayEditor(QWidget *p, QGraphicsItem *qgi) : QDialog(p), qgiPromote(qgi) {
	setupUi(this);
	
	connect(qdbbBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
	connect(qdbbBox->button(QDialogButtonBox::Reset), SIGNAL(clicked()), this, SLOT(reset()));
	
	QGraphicsProxyWidget *qgpw = graphicsProxyWidget();
	if (qgpw) {
		qgpw->setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
		if (g.ocIntercept) {
			qgpw->setPos(iroundf(g.ocIntercept->uiWidth / 16.0f), iroundf(g.ocIntercept->uiHeight / 16.0f));
			qgpw->resize(iroundf(g.ocIntercept->uiWidth * 14.0f / 16.0f), iroundf(g.ocIntercept->uiHeight * 14.0f / 16.0f));
		}
	}
	
	qgvView->setScene(&oes);
	
	reset();
}

OverlayEditor::~OverlayEditor() {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.9f);
	if (qgiPromote)
		qgiPromote->setZValue(-1.0f);
}

void OverlayEditor::enterEvent(QEvent *event) {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.9f);

	qgpw = graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(1.0f);

	if (qgiPromote)
		qgiPromote->setZValue(-1.0f);

	QDialog::enterEvent(event);
}

void OverlayEditor::leaveEvent(QEvent *event) {
	QGraphicsProxyWidget *qgpw = g.mw->graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.3f);

	qgpw = graphicsProxyWidget();
	if (qgpw)
		qgpw->setOpacity(0.3f);

	if (qgiPromote)
		qgiPromote->setZValue(1.0f);

	QDialog::leaveEvent(event);
}

void OverlayEditor::reset() {
	oes.os = g.s.os;
	oes.resync();

	qcbAvatar->setChecked(oes.os.bAvatar);
	qcbUser->setChecked(oes.os.bUserName);
	qcbChannel->setChecked(oes.os.bChannel);
	qcbMutedDeafened->setChecked(oes.os.bMutedDeafened);
	qcbBox->setChecked(oes.os.bBox);
}

void OverlayEditor::apply() {
	g.s.os = oes.os;
	g.o->forceSettings();
}

void OverlayEditor::accept() {
	apply();
	QDialog::accept();
}

void OverlayEditor::on_qrbPassive_clicked() {
	oes.tsColor = Settings::Passive;
	oes.resync();
}

void OverlayEditor::on_qrbTalking_clicked() {
	oes.tsColor = Settings::Talking;
	oes.resync();
}

void OverlayEditor::on_qrbWhisper_clicked() {
	oes.tsColor = Settings::WhisperPrivate;
	oes.resync();
}

void OverlayEditor::on_qrbShout_clicked() {
	oes.tsColor = Settings::WhisperChannel;
	oes.resync();
}

void OverlayEditor::on_qcbAvatar_clicked() {
	oes.os.bAvatar = qcbAvatar->isChecked();
	oes.updateAvatar();
}

void OverlayEditor::on_qcbUser_clicked() {
	oes.os.bUserName = qcbUser->isChecked();
	oes.updateUserName();
}

void OverlayEditor::on_qcbChannel_clicked() {
	oes.os.bChannel = qcbChannel->isChecked();
	oes.updateChannel();
}

void OverlayEditor::on_qcbMutedDeafened_clicked() {
	oes.os.bMutedDeafened = qcbMutedDeafened->isChecked();
	oes.updateMuted();
}

void OverlayEditor::on_qcbBox_clicked() {
	oes.os.bBox = qcbBox->isChecked();
	oes.moveBox();
}

void OverlayEditor::on_qsZoom_valueChanged(int zoom) {
	oes.uiZoom = zoom;
	oes.resync();
}

OverlayUser::OverlayUser(ClientUser *cu, unsigned int height) : OverlayGroup(), cuUser(cu), uiSize(height), tsColor(Settings::Passive) {
	setup();
	updateLayout();
}

OverlayUser::OverlayUser(Settings::TalkState ts, unsigned int height) : OverlayGroup(), cuUser(NULL), uiSize(height), tsColor(ts) {
	qsChannelName = Overlay::tr("Channel");

	setup();
	updateLayout();
}

int OverlayUser::type() const {
	return Type;
}

void OverlayUser::setup() {
	qgpiMuted = new QGraphicsPixmapItem(this);
	qgpiMuted->hide();

	qgpiDeafened = new QGraphicsPixmapItem(this);
	qgpiDeafened->hide();

	qgpiAvatar = new QGraphicsPixmapItem(this);

	for (int i=0;i<4;++i) {
		qgpiName[i] = new QGraphicsPixmapItem(this);
		qgpiName[i]->hide();
	}

	qgpiChannel = new QGraphicsPixmapItem(this);
	qgpiChannel->hide();
	
	qgpiBox = new QGraphicsPathItem(this);
	qgpiBox->hide();
}

#undef SCALESIZE
#define SCALESIZE(var) iroundf(uiSize * g.s.os.fHeight * g.s.os.qrf##var .height()), iroundf(uiSize * g.s.os.fHeight * g.s.os.qrf##var .width())

void OverlayUser::updateLayout() {
	QPixmap pm;

	prepareGeometryChange();

	for (int i=0;i<4;++i)
		qgpiName[i]->setPixmap(pm);

	qgpiAvatar->setPixmap(pm);
	qgpiChannel->setPixmap(pm);

	{
		QImageReader qir(QLatin1String("skin:muted_self.svg"));
		QSize sz = qir.size();
		sz.scale(SCALESIZE(MutedDeafened), Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiMuted->setPixmap(QPixmap::fromImage(qir.read()));
	}

	{
		QImageReader qir(QLatin1String("skin:deafened_self.svg"));
		QSize sz = qir.size();
		sz.scale(SCALESIZE(MutedDeafened), Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiDeafened->setPixmap(QPixmap::fromImage(qir.read()));
	}

	qgpiMuted->setPos(alignedPosition(scaledRect(g.s.os.qrfMutedDeafened, uiSize * g.s.os.fHeight), qgpiMuted->boundingRect(), g.s.os.qaMutedDeafened));
	qgpiMuted->setZValue(1.0f);
	qgpiMuted->setOpacity(g.s.os.fMutedDeafened);

	qgpiDeafened->setPos(alignedPosition(scaledRect(g.s.os.qrfMutedDeafened, uiSize * g.s.os.fHeight), qgpiDeafened->boundingRect(), g.s.os.qaMutedDeafened));
	qgpiDeafened->setZValue(1.0f);
	qgpiDeafened->setOpacity(g.s.os.fMutedDeafened);

	qgpiAvatar->setPos(0.0f, 0.0f);
	qgpiAvatar->setOpacity(g.s.os.fAvatar);

	for (int i=0;i<4;++i) {
		qgpiName[i]->setPos(0.0f, 0.0f);
		qgpiName[i]->setZValue(2.0f);
		qgpiName[i]->setOpacity(g.s.os.fUserName);
	}
	qgpiChannel->setPos(0.0f, 0.0f);
	qgpiChannel->setZValue(3.0f);
	qgpiChannel->setOpacity(g.s.os.fChannel);

	QRectF children = g.s.os.qrfAvatar | g.s.os.qrfChannel | g.s.os.qrfMutedDeafened | g.s.os.qrfUserName;
	
	bool haspen = (g.s.os.qcBoxPen != g.s.os.qcBoxFill) && (! qFuzzyCompare(g.s.os.qcBoxPen.alphaF(), static_cast<qreal>(0.0f)));
	qreal pw = haspen ? qMax<qreal>(1.0f, g.s.os.fBoxPenWidth * uiSize * g.s.os.fHeight) : 0.0f;
	qreal pad = g.s.os.fBoxPad * uiSize * g.s.os.fHeight;
	QPainterPath pp;
	pp.addRoundedRect(-pw / 2.0f - pad, -pw / 2.0f - pad, children.width() * uiSize * g.s.os.fHeight + pw + 2.0f * pad, children.height() * uiSize * g.s.os.fHeight + pw + 2.0f * pad, 2.0f * pw, 2.0f * pw);
	qgpiBox->setPath(pp);
	qgpiBox->setPos(0.0f, 0.0f);
	qgpiBox->setZValue(-1.0f);
	qgpiBox->setPen(haspen ? QPen(g.s.os.qcBoxPen, pw) : Qt::NoPen);
	qgpiBox->setBrush(qFuzzyCompare(g.s.os.qcBoxFill.alphaF(), static_cast<qreal>(0.0f)) ? Qt::NoBrush : g.s.os.qcBoxFill);
	qgpiBox->setOpacity(1.0f);
	
	if (! cuUser) {
		switch (tsColor) {
			case Settings::Passive:
				qsName = Overlay::tr("Silent");
				break;
			case Settings::Talking:
				qsName = Overlay::tr("Talking");
				break;
			case Settings::WhisperPrivate:
				qsName = Overlay::tr("Whisper");
				break;
			case Settings::WhisperChannel:
				qsName = Overlay::tr("Shout");
				break;
		}
	}
}

QPixmap OverlayUser::createPixmap(const QString &string, unsigned int height, unsigned int maxwidth, QColor col, const QFont &font, QPainterPath &pp) {
	float edge = height * 0.05f;

	if (pp.isEmpty()) {
		QFont f = font;

		QRectF r;
		{
			QPainterPath qp;
			qp.addText(0.0f, 0.0f, f, string);
			r = qp.controlPointRect();
		}

		float fs = f.pointSizeF();
		float ds = fs * ((height - 2.0f * edge) / r.height());

		f.setPointSizeF(ds);

		pp.addText(0.0f, 0.0f, f, string);
		r = pp.controlPointRect();

		QString str;

		if (r.width() < maxwidth) {
			str = string;
		} else {
			QFontMetrics qfm(f);
			str = qfm.elidedText(string, Qt::ElideRight, iroundf(maxwidth - 2 * edge));
			if (str.trimmed().isEmpty())
				str = QLatin1String("...");

			pp = QPainterPath();
			pp.addText(0.0f, 0.0f, f, str);
		}

		QRectF qr = pp.controlPointRect();

#if QT_VERSION >= 0x040600
		pp.translate(- qr.x() + edge, - qr.y() + edge);
#else
		pp = QPainterPath();
		pp.addText(- qr.x() + edge, - qr.y() + edge, f, str);
#endif
	}

	QRectF qr = pp.controlPointRect();
	int w = iroundf(qr.width() + 2 * edge + 0.5f);
	int h = iroundf(qr.height() + 2 * edge + 0.5f);

	QImage img(w, h, QImage::Format_ARGB32);
	img.fill(0);

	QPainter imgp(&img);
	imgp.setRenderHint(QPainter::Antialiasing);
	imgp.setRenderHint(QPainter::TextAntialiasing);
	imgp.setBackground(QColor(0,0,0,0));
	imgp.setCompositionMode(QPainter::CompositionMode_Source);

	QColor qc(col);
	qc.setAlpha(255);

	imgp.setBrush(qc);
	imgp.setPen(QPen(Qt::black, edge, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	imgp.drawPath(pp);

	imgp.setPen(Qt::NoPen);
	imgp.drawPath(pp);

	return QPixmap::fromImage(img);
}

void OverlayUser::updateUser() {
	prepareGeometryChange();
	
	if (g.s.os.bUserName && (qgpiName[0]->pixmap().isNull() || (cuUser && (qsName != cuUser->qsName)))) {
		if (cuUser)
			qsName = cuUser->qsName;

		QPainterPath pp;
		for (int i=0; i<4; ++i) {
			const QPixmap &pm = createPixmap(qsName, SCALESIZE(UserName), g.s.os.qcUserName[i], g.s.os.qfUserName, pp);
			qgpiName[i]->setPixmap(pm);

			if (i == 0)
				qgpiName[0]->setPos(alignedPosition(scaledRect(g.s.os.qrfUserName, uiSize * g.s.os.fHeight), qgpiName[0]->boundingRect(), g.s.os.qaUserName));
			else
				qgpiName[i]->setPos(qgpiName[0]->pos());
		}
	}

	if (g.s.os.bChannel && (qgpiChannel->pixmap().isNull() || (cuUser && (qsChannelName != cuUser->cChannel->qsName)))) {
		if (cuUser)
			qsChannelName = cuUser->cChannel->qsName;

		QPainterPath pp;
		const QPixmap &pm = createPixmap(qsChannelName, SCALESIZE(Channel), g.s.os.qcChannel, g.s.os.qfChannel, pp);
		qgpiChannel->setPixmap(pm);
		qgpiChannel->setPos(alignedPosition(scaledRect(g.s.os.qrfChannel, uiSize * g.s.os.fHeight), qgpiChannel->boundingRect(), g.s.os.qaChannel));
	}

	if (g.s.os.bAvatar && (qgpiAvatar->pixmap().isNull() || (cuUser && (qbaAvatar != cuUser->qbaTextureHash)))) {
		if (cuUser)
			qbaAvatar = cuUser->qbaTextureHash;

		QImage img;
		if (qbaAvatar.isNull()) {
			QImageReader qir(QLatin1String("skin:default_avatar.svg"));
			QSize sz = qir.size();
			sz.scale(SCALESIZE(Avatar), Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		} else {
			QBuffer qb(& cuUser->qbaTexture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir(&qb, cuUser->qbaTextureFormat);
			QSize sz = qir.size();
			sz.scale(SCALESIZE(Avatar), Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		}

		qgpiAvatar->setPixmap(QPixmap::fromImage(img));
		qgpiAvatar->setPos(alignedPosition(scaledRect(g.s.os.qrfAvatar, uiSize * g.s.os.fHeight), qgpiAvatar->boundingRect(), g.s.os.qaAvatar));
	}

	qgpiAvatar->setVisible(g.s.os.bAvatar);

	if (cuUser) {
		ClientUser *self = ClientUser::get(g.uiSession);

		if (g.s.os.bMutedDeafened) {
			if (cuUser->bDeaf || cuUser->bSelfDeaf) {
				qgpiMuted->hide();
				qgpiDeafened->show();
			} else if (cuUser->bMute || cuUser->bSelfMute || cuUser->bLocalMute || cuUser->bSuppress) {
				qgpiMuted->show();
				qgpiDeafened->hide();
			} else {
				qgpiMuted->hide();
				qgpiDeafened->hide();
			}
		} else {
			qgpiMuted->hide();
			qgpiDeafened->hide();
		}

		bool samechannel = self && (self->cChannel == cuUser->cChannel);
		qgpiChannel->setVisible(g.s.os.bChannel && ! samechannel);

		tsColor = Settings::Passive;
		switch (cuUser->tsState) {
			case ClientUser::Talking:
				tsColor = Settings::Talking;
				break;
			case ClientUser::TalkingWhisper:
				tsColor = Settings::WhisperPrivate;
				break;
			case ClientUser::TalkingWhisperChannel:
				tsColor = Settings::WhisperChannel;
				break;
			default:
				break;
		}
	} else {
		qgpiChannel->setVisible(g.s.os.bChannel && (tsColor != Settings::Passive) && (tsColor != Settings::Talking));
		qgpiMuted->setVisible(g.s.os.bChannel);
		qgpiDeafened->hide();
	}

	if (g.s.os.bUserName)
		for (int i=0;i<4;++i)
			qgpiName[i]->setVisible(i == tsColor);
	else
		for (int i=0;i<4;++i)
			qgpiName[i]->setVisible(false);
			
	qgpiBox->setVisible(g.s.os.bBox);

	setOpacity(g.s.os.fUser[tsColor]);
}

QRectF OverlayUser::scaledRect(const QRectF &qr, qreal scale) {
	return QRectF(qr.x() * scale, qr.y() * scale, qr.width() * scale, qr.height() * scale);
}

QPointF OverlayUser::alignedPosition(const QRectF &box, const QRectF &item, Qt::Alignment a) {
	qreal boxw = box.width();
	qreal boxh = box.height();
	
	qreal itemw = item.width();
	qreal itemh = item.height();
	
	qreal wdiff = boxw - itemw;
	qreal hdiff = boxh - itemh;
	
	qreal xofs = box.x() - item.x();
	qreal yofs = box.y() - item.y();
	
	if (a & Qt::AlignRight)
		xofs += wdiff;
	else if (a & Qt::AlignHCenter)
		xofs += wdiff * 0.5f;
		
	if (a & Qt::AlignBottom)
		yofs += hdiff;
	else if (a & Qt::AlignVCenter)
		yofs += hdiff * 0.5f;
		
	return QPointF(iroundf(xofs), iroundf(yofs));
}



OverlayMouse::OverlayMouse(QGraphicsItem *p) : QGraphicsPixmapItem(p) {
}

bool OverlayMouse::contains(const QPointF &) const {
	return false;
}

bool OverlayMouse::collidesWithPath(const QPainterPath &, Qt::ItemSelectionMode) const {
	return false;
}



OverlayGroup::OverlayGroup() : QGraphicsItem() {
}

int OverlayGroup::type() const {
	return Type;
}

void OverlayGroup::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) {
}

QRectF OverlayGroup::boundingRect() const {
	QRectF qr;
	foreach(const QGraphicsItem *item, childItems())
		if (item->isVisible())
			qr |= item->boundingRect().translated(item->pos());

	return qr;
}


OverlayUserGroup::OverlayUserGroup() : OverlayGroup() {
}

int OverlayUserGroup::type() const {
	return Type;
}

void OverlayUserGroup::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	event->accept();

	QMenu qm(g.mw);
	QAction *qaEdit = qm.addAction(OverlayClient::tr("Edit..."));

	QAction *act = qm.exec(event->screenPos());
	
	if (! act)
		return;

	if (act == qaEdit) {
		QMetaObject::invokeMethod(g.ocIntercept, "openEditor", Qt::QueuedConnection);
	}
}

void OverlayUserGroup::wheelEvent(QGraphicsSceneWheelEvent *event) {
	event->accept();

	qreal scale = 0.875f;

	if (event->delta() > 0)
		scale = 1.0f / 0.875f;

	if ((scale < 1.0f) && (g.s.os.fHeight <= (1.0f / 16.0f)))
		return;
	else if ((scale > 1.0f) && (g.s.os.fHeight >= 16.0f))
		return;

	prepareGeometryChange();
	
	g.s.os.fHeight *= scale;

	g.o->forceSettings();
}


OverlayClient::OverlayClient(QLocalSocket *socket, QObject *p) : QObject(p) {
	qlsSocket = socket;
	connect(qlsSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	omMsg.omh.iLength = -1;
	smMem = NULL;
	uiWidth = uiHeight = 0;

	uiPid = ~0ULL;

	bWasVisible = false;
	bDelete = false;

	qgv.setScene(&qgs);
	qgv.installEventFilter(this);
	qgv.viewport()->installEventFilter(this);

	// Make sure it has a native window id
	qgv.winId();

	qgpiCursor = new OverlayMouse();
	qgpiCursor->hide();
	qgpiCursor->setZValue(10.0f);

	qgs.addItem(qgpiCursor);
	
	ougUsers.setZValue(-1.0f);
	qgs.addItem(&ougUsers);
	ougUsers.show();

	qgpiLogo = NULL;

	iOffsetX = iOffsetY = 0;

	connect(&qgs, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(changed(const QList<QRectF> &)));
}

OverlayClient::~OverlayClient() {
	qlsSocket->abort();

	foreach(OverlayUser *ou, qlExampleUsers)
		delete ou;
	qlExampleUsers.clear();

	foreach(OverlayUser *ou, qmUsers)
		delete ou;
	qmUsers.clear();
}

bool OverlayClient::eventFilter(QObject *o, QEvent *e) {
	if (e->type() == QEvent::Paint) {
		e->accept();
		return true;
	}
	return QObject::eventFilter(o, e);
}

void OverlayClient::updateMouse() {
#ifdef Q_OS_WIN
	QPixmap pm;

	HICON c = ::GetCursor();
	if (c == NULL)
		c = qgv.viewport()->cursor().handle();

	ICONINFO info;
	ZeroMemory(&info, sizeof(info));
	if (::GetIconInfo(c, &info)) {
		if (info.hbmColor) {
			pm = QPixmap::fromWinHBITMAP(info.hbmColor);
			pm.setMask(QBitmap(QPixmap::fromWinHBITMAP(info.hbmMask)));
		} else {
			QBitmap orig(QPixmap::fromWinHBITMAP(info.hbmMask));
			QImage img = orig.toImage();

			int h = img.height() / 2;
			int w = img.bytesPerLine() / sizeof(quint32);

			QImage out(img.width(), h, QImage::Format_MonoLSB);
			QImage outmask(img.width(), h, QImage::Format_MonoLSB);

			for (int i=0;i<h; ++i) {
				const quint32 *srcimg = reinterpret_cast<const quint32 *>(img.scanLine(i + h));
				const quint32 *srcmask = reinterpret_cast<const quint32 *>(img.scanLine(i));

				quint32 *dstimg = reinterpret_cast<quint32 *>(out.scanLine(i));
				quint32 *dstmask = reinterpret_cast<quint32 *>(outmask.scanLine(i));

				for (int j=0;j<w;++j) {
					dstmask[j] = srcmask[j];
					dstimg[j] = srcimg[j];
				}
			}
			pm = QBitmap::fromImage(out);
		}

		if (info.hbmMask)
			::DeleteObject(info.hbmMask);
		if (info.hbmColor)
			::DeleteObject(info.hbmColor);

		iOffsetX = info.xHotspot;
		iOffsetY = info.yHotspot;
	}

	qgpiCursor->setPixmap(pm);
#else
#endif

	qgpiCursor->setPos(iMouseX - iOffsetX, iMouseY - iOffsetY);
}

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
extern bool Q_GUI_EXPORT qt_use_native_dialogs;
#endif

// Qt gets very very unhappy if we embed or unmbed the widget that an event is called from.
// This means that if any modal dialog is open, we'll be in a event loop of an object
// that we're about to reparent.

void OverlayClient::showGui() {
	QWidgetList widgets = qApp->topLevelWidgets();
	widgets.removeAll(g.mw);
	widgets.prepend(g.mw);

#if defined(QT3_SUPPORT) || defined(Q_WS_WIN)
	if (QCoreApplication::loopLevel() > 1)
		return;
#else
	int count = 0;

	foreach(QWidget *w, widgets) {
		if (w->isHidden() && (w != g.mw))
			continue;
		count++;
	}

	// If there's more than one window up, we're likely deep in a message loop.
	if (count > 1)
		return;
#endif

	g.ocIntercept = this;

	bWasVisible = ! g.mw->isHidden();

	foreach(QWidget *w, widgets) {
		if ((w == g.mw) || (! w->isHidden())) {
			QGraphicsProxyWidget *qgpw = new QGraphicsProxyWidget(NULL, Qt::Window);

			qgpw->setOpacity(0.90f);
			qgpw->setWidget(w);


			if (w == g.mw) {
				qgpw->setPos(uiWidth / 10, uiHeight / 10);
				qgpw->resize((uiWidth * 8) / 10, (uiHeight * 8) / 10);
			}

			qgs.addItem(qgpw);
			qgpw->show();
			qgpw->setActive(true);
		}
	}

	QEvent event(QEvent::WindowActivate);
	qApp->sendEvent(&qgs, &event);

	QPoint p = QCursor::pos();
	iMouseX = qBound<int>(0, p.x(), uiWidth-1);
	iMouseY = qBound<int>(0, p.y(), uiHeight-1);

	qgpiCursor->setPos(iMouseX, iMouseY);
	qgpiCursor->show();

	qgs.setFocus();
	g.mw->qleChat->activateWindow();
	g.mw->qleChat->setFocus();

	qgv.setAttribute(Qt::WA_WState_Hidden, false);
	qApp->setActiveWindow(&qgv);
	qgv.setFocus();

	setupScene();

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
	qt_use_native_dialogs = false;
#endif
	g.o->updateOverlay();
}

void OverlayClient::hideGui() {
	qgpiCursor->hide();
#if defined(QT3_SUPPORT) || defined(Q_WS_WIN)
	if (QCoreApplication::loopLevel() > 1) {
		QCoreApplication::exit_loop();
		QMetaObject::invokeMethod(this, "hideGui", Qt::QueuedConnection);
		return;
	}
#endif

	QList<QWidget *> widgetlist;

	foreach(QGraphicsItem *qgi, qgs.items(Qt::DescendingOrder)) {
		QGraphicsProxyWidget *qgpw = qgraphicsitem_cast<QGraphicsProxyWidget *>(qgi);
		if (qgpw && qgpw->widget()) {
			QWidget *w = qgpw->widget();

			qgpw->setVisible(false);
			widgetlist << w;
		}
	}

	foreach(QWidget *w, widgetlist) {
		QGraphicsProxyWidget *qgpw = w->graphicsProxyWidget();
		if (qgpw) {
			qgpw->setVisible(false);
			qgpw->setWidget(NULL);
			delete qgpw;
		}
	}

	if (g.ocIntercept == this)
		g.ocIntercept = NULL;

	g.mw->bNoHide = true;
	foreach(QWidget *w, widgetlist) {
		if (bWasVisible)
			w->show();
	}
	g.mw->bNoHide = false;

	setupScene();

	qgv.setAttribute(Qt::WA_WState_Hidden, true);
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
	qt_use_native_dialogs = true;
#endif

	g.o->updateOverlay();

	if (bDelete)
		deleteLater();
}

void OverlayClient::scheduleDelete() {
	bDelete = true;
	hideGui();
}

void OverlayClient::readyRead() {
	while (1) {
		int ready = qlsSocket->bytesAvailable();

		if (omMsg.omh.iLength == -1) {
			if (ready < sizeof(OverlayMsgHeader))
				break;
			else {
				qlsSocket->read(omMsg.headerbuffer, sizeof(OverlayMsgHeader));
				if ((omMsg.omh.uiMagic != OVERLAY_MAGIC_NUMBER) || (omMsg.omh.iLength < 0) || (omMsg.omh.iLength > sizeof(OverlayMsgShmem))) {
					disconnect();
					return;
				}
				ready -= sizeof(OverlayMsgHeader);
			}
		}

		if (ready >= omMsg.omh.iLength) {
			int length = qlsSocket->read(omMsg.msgbuffer, omMsg.omh.iLength);

			if (length != omMsg.omh.iLength) {
				disconnect();
				return;
			}

			switch (omMsg.omh.uiType) {
				case OVERLAY_MSGTYPE_INIT: {
						if (length != sizeof(OverlayMsgInit))
							break;

						OverlayMsgInit *omi = & omMsg.omi;

						uiWidth = omi->uiWidth;
						uiHeight = omi->uiHeight;
						qrLast = QRect();

						if (smMem)
							delete smMem;

						smMem = new SharedMemory2(this, uiWidth * uiHeight * 4);
						if (! smMem->data()) {
							qWarning() << "OverlayClient: Failed to create shared memory" << uiWidth << uiHeight;
							delete smMem;
							smMem = NULL;
							break;
						}
						QByteArray key = smMem->name().toUtf8();
						key.append(static_cast<char>(0));

						OverlayMsg om;
						om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
						om.omh.uiType = OVERLAY_MSGTYPE_SHMEM;
						om.omh.iLength = key.length();
						memcpy(om.oms.a_cName, key.constData(), key.length());
						qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + om.omh.iLength);

						setupRender();

						Overlay *o = static_cast<Overlay *>(parent());
						QTimer::singleShot(0, o, SLOT(updateOverlay()));
					}
					break;
				case OVERLAY_MSGTYPE_SHMEM: {
						if (smMem)
							smMem->systemRelease();
					}
					break;
				case OVERLAY_MSGTYPE_PID: {
						if (length != sizeof(OverlayMsgPid))
							break;

						OverlayMsgPid *omp = & omMsg.omp;
						uiPid = omp->pid;
					}
					break;
				default:
					break;
			}
			omMsg.omh.iLength = -1;
		} else {
			break;
		}
	}
}

void OverlayClient::reset() {
	if (! uiWidth || ! uiHeight || ! smMem)
		return;

	foreach(OverlayUser *ou, qmUsers)
		delete ou;
	qmUsers.clear();

	if (qgpiLogo) {
		delete qgpiLogo;
		qgpiLogo = NULL;
	}

	foreach(OverlayUser *ou, qlExampleUsers)
		delete ou;
	qlExampleUsers.clear();

	setupScene();
}

void OverlayClient::setupScene() {
	if (qgpiCursor->isVisible()) {
		qgs.setBackgroundBrush(QColor(0,0,0,64));

		if (! qgpiLogo) {
			qgpiLogo = new OverlayMouse();
			qgpiLogo->hide();
			qgpiLogo->setOpacity(0.8f);
			qgpiLogo->setZValue(-5.0f);


			QImageReader qir(QLatin1String("skin:mumble.svg"));
			QSize sz = qir.size();
			sz.scale(uiWidth, uiHeight, Qt::KeepAspectRatio);
			qir.setScaledSize(sz);

			qgpiLogo->setPixmap(QPixmap::fromImage(qir.read()));

			QRectF qrf = qgpiLogo->boundingRect();
			qgpiLogo->setPos(iroundf((uiWidth - qrf.width()) / 2.0f), iroundf((uiHeight - qrf.height()) / 2.0f));

			qgs.addItem(qgpiLogo);
		}
		qgpiLogo->show();

		if (qlExampleUsers.isEmpty()) {
			qlExampleUsers << new OverlayUser(Settings::Passive, uiHeight);
			qlExampleUsers << new OverlayUser(Settings::Talking, uiHeight);
			qlExampleUsers << new OverlayUser(Settings::WhisperPrivate, uiHeight);
			qlExampleUsers << new OverlayUser(Settings::WhisperChannel, uiHeight);
		}
	} else {
		qgs.setBackgroundBrush(Qt::NoBrush);

		if (qgpiLogo)
			qgpiLogo->hide();
	}
}

void OverlayClient::setupRender() {
	qgs.setSceneRect(0, 0, uiWidth, uiHeight);
	qgv.setScene(NULL);
	qgv.setGeometry(-2, -2, uiWidth + 2, uiHeight + 2);
	qgv.viewport()->setGeometry(0, 0, uiWidth, uiHeight);
	qgv.setScene(&qgs);

	smMem->erase();

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType = OVERLAY_MSGTYPE_BLIT;
	om.omh.iLength = sizeof(OverlayMsgBlit);
	om.omb.x = 0;
	om.omb.y = 0;
	om.omb.w = uiWidth;
	om.omb.h = uiHeight;
	qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgBlit));

	reset();
}

bool OverlayClient::setTexts(const QList<OverlayTextLine> &lines) {
	if (! uiWidth || ! uiHeight || ! smMem)
		return true;

	QList<QGraphicsItem *> items;
	foreach(QGraphicsItem *qgi, ougUsers.childItems())
		items << qgi;

	QList<OverlayUser *> users;
	if (qgpiCursor->isVisible()) {
		users = qlExampleUsers;
		foreach(OverlayUser *ou, users)
			items.removeAll(ou);
	}

	foreach(const OverlayTextLine &e, lines) {
		if (e.uiSession != 0) {
			ClientUser *cu = ClientUser::get(e.uiSession);

			OverlayUser *ou = qmUsers.value(cu);
			if (! ou) {
				ou = new OverlayUser(cu, uiHeight);
				connect(cu, SIGNAL(destroyed(QObject *)), this, SLOT(userDestroyed(QObject *)));
				qmUsers.insert(cu, ou);
				ou->hide();
			} else {
				items.removeAll(ou);
			}
			users << ou;
		}
	}

	foreach(QGraphicsItem *qgi, items) {
		qgs.removeItem(qgi);
		qgi->hide();
	}

	QRectF children = g.s.os.qrfAvatar | g.s.os.qrfChannel | g.s.os.qrfMutedDeafened | g.s.os.qrfUserName;

	int width = iroundf(children.width() * uiHeight * g.s.os.fHeight);
	int height = iroundf(children.height() * uiHeight * g.s.os.fHeight);

	int y = 0;
	int x = 0;

	foreach(OverlayUser *ou, users) {
		if (ou->parentItem() == NULL)
			ou->setParentItem(&ougUsers);

		ou->setPos(x * (width+2), y * (height + 2));
		ou->updateUser();
		ou->show();

		if (x) {
			x = 0;
			++y;
		} else {
			x = 1;
		}
	}
	
	QRectF br = ougUsers.boundingRect();

	int basex = qBound<int>(0, iroundf(uiWidth * g.s.os.fX), iroundf(uiWidth - br.width()));
	int basey = qBound<int>(0, iroundf(uiHeight * g.s.os.fY), iroundf(uiHeight - br.height()));
	
	ougUsers.setPos(basex, basey);

	if (qlsSocket->bytesToWrite() > 1024) {
		return (t.elapsed() <= 5000000ULL);
	} else {
		t.restart();
		return true;
	}
}

void OverlayClient::changed(const QList<QRectF> &region) {
	if (region.isEmpty())
		return;

	qlDirty.append(region);
	QMetaObject::invokeMethod(this, "render", Qt::QueuedConnection);
}

void OverlayClient::render() {
	const QList<QRectF> region = qlDirty;
	qlDirty.clear();

	if (! uiWidth || ! uiHeight || ! smMem)
		return;
		
	QRect active;
	QRectF dirtyf;

	if (region.isEmpty())
		return;

	foreach(const QRectF &r, region) {
		dirtyf |= r;
	}


	QRect dirty = dirtyf.toAlignedRect();
	dirty = dirty.intersected(QRect(0,0,uiWidth, uiHeight));

	QRect target = dirty;
	target.moveTo(0,0);

	QImage img(reinterpret_cast<unsigned char *>(smMem->data()), uiWidth, uiHeight, QImage::Format_ARGB32_Premultiplied);
	QImage qi(target.size(), QImage::Format_ARGB32_Premultiplied);
	qi.fill(0);

	QPainter p;
	p.begin(&qi);
	p.setRenderHints(p.renderHints(), false);
	p.setCompositionMode(QPainter::CompositionMode_SourceOver);
	qgs.render(&p, target, dirty, Qt::IgnoreAspectRatio);
	p.end();

	p.begin(&img);
	p.setRenderHints(p.renderHints(), false);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.drawImage(dirty.x(), dirty.y(), qi);
	p.end();

	if (dirty.isValid()) {
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_BLIT;
		om.omh.iLength = sizeof(OverlayMsgBlit);
		om.omb.x = dirty.x();
		om.omb.y = dirty.y();
		om.omb.w = dirty.width();
		om.omb.h = dirty.height();
		qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgBlit));
	}

	if (qgpiCursor->isVisible()) {
		active = QRect(0,0,uiWidth,uiHeight);
	} else {
		active = qgs.itemsBoundingRect().toAlignedRect();
		if (active.isEmpty())
			active = QRect(0,0,0,0);
		active = active.intersected(QRect(0,0,uiWidth,uiHeight));
	}

	if (active != qrLast) {
		qrLast = active;

		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_ACTIVE;
		om.omh.iLength = sizeof(OverlayMsgActive);
		om.oma.x = qrLast.x();
		om.oma.y = qrLast.y();
		om.oma.w = qrLast.width();
		om.oma.h = qrLast.height();
		qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgActive));
	}

	qlsSocket->flush();
}

void OverlayClient::userDestroyed(QObject *obj) {
	OverlayUser *ou = qmUsers.take(obj);
	if (ou)
		delete ou;
}

void OverlayClient::openEditor() {
	OverlayEditor oe(g.mw, &ougUsers);
	
	oe.exec();
}


bool OverlayTextLine::operator <(const OverlayTextLine &other) const {
	if (iPriority < other.iPriority)
		return true;
	else if (iPriority > other.iPriority)
		return false;

	return QString::localeAwareCompare(qsText, other.qsText) < 0;
}



Overlay::Overlay() : QObject() {
	d = NULL;

	platformInit();
	forceSettings();

	qlsServer = new QLocalServer(this);
	QString pipepath;
#ifdef Q_OS_WIN
	pipepath = QLatin1String("MumbleOverlayPipe");
#else
	pipepath = QDir::home().absoluteFilePath(QLatin1String(".MumbleOverlayPipe"));
	{
		QFile f(pipepath);
		if (f.exists()) {
			qWarning() << "Overlay: Removing old socket on" << pipepath;
			f.remove();
		}
	}
#endif

	if (! qlsServer->listen(pipepath)) {
		QMessageBox::warning(NULL, QLatin1String("Mumble"), tr("Failed to create communication with overlay at %2: %1. No overlay will be available.").arg(qlsServer->errorString(),pipepath), QMessageBox::Ok, QMessageBox::NoButton);
	} else {
		qWarning() << "Overlay: Listening on" << qlsServer->fullServerName();
		connect(qlsServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}

	QMetaObject::connectSlotsByName(this);
}

Overlay::~Overlay() {
	setActive(false);
	if (d)
		delete d;

	// Need to be deleted first, since destructor references lingering QLocalSockets
	foreach(OverlayClient *oc, qlClients)
		delete oc;
}

void Overlay::newConnection() {
	while (1) {
		QLocalSocket *qls = qlsServer->nextPendingConnection();
		if (! qls)
			break;
		OverlayClient *oc = new OverlayClient(qls, this);
		qlClients << oc;

		connect(qls, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(qls, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
	}
}

void Overlay::disconnected() {
	QLocalSocket *qls = qobject_cast<QLocalSocket *>(sender());
	foreach(OverlayClient *oc, qlClients) {
		if (oc->qlsSocket == qls) {
			qlClients.removeAll(oc);
			oc->deleteLater();
			return;
		}
	}
	qls->deleteLater();
}

void Overlay::error(QLocalSocket::LocalSocketError) {
	disconnected();
}

bool Overlay::isActive() const {
	return ! qlClients.isEmpty();
}

void Overlay::toggleShow() {
/*
	OverlayEditor oe(g.mw);
	oe.exec();
	return;
*/
	if (g.ocIntercept) {
		g.ocIntercept->hideGui();
	} else {
		foreach(OverlayClient *oc, qlClients) {
			if (oc->uiPid) {
#ifdef Q_OS_WIN
				HWND hwnd = GetForegroundWindow();
				DWORD pid = 0;
				GetWindowThreadProcessId(hwnd, &pid);
				if (pid != oc->uiPid)
					continue;
#endif
				oc->showGui();
				return;
			}
		}
	}
}

void Overlay::forceSettings() {
	foreach(OverlayClient *oc, qlClients) {
		oc->reset();
	}

	updateOverlay();
}

void Overlay::verifyTexture(ClientUser *cp, bool allowupdate) {
	qsQueried.remove(cp->uiSession);

	ClientUser *self = ClientUser::get(g.uiSession);
	allowupdate = allowupdate && self && self->cChannel->isLinked(cp->cChannel);

	if (allowupdate && ! cp->qbaTextureHash.isEmpty() && cp->qbaTexture.isEmpty())
		cp->qbaTexture = Database::blob(cp->qbaTextureHash);

	if (! cp->qbaTexture.isEmpty()) {
		bool valid = true;

		if (cp->qbaTexture.length() < sizeof(unsigned int)) {
			valid = false;
		} else if (qFromBigEndian<unsigned int>(reinterpret_cast<const unsigned char *>(cp->qbaTexture.constData())) == 600 * 60 * 4) {
			QByteArray qba = qUncompress(cp->qbaTexture);
			if (qba.length() != 600 * 60 * 4) {
				valid = false;
			} else {
				int width = 0;
				int height = 0;
				const unsigned int *ptr = reinterpret_cast<const unsigned int *>(qba.constData());

				// If we have an alpha only part on the right side of the image ignore it
				for (int y=0;y<60;++y) {
					for (int x=0;x<600; ++x) {
						if (ptr[y*600+x] & 0xff000000) {
							if (x > width)
								width = x;
							if (y > height)
								height = y;
						}
					}
				}

				// Full size image? More likely image without alpha; fix it.
				if ((width == 599) && (height == 59)) {
					width = 0;
					height = 0;
					for (int y=0;y<60;++y) {
						for (int x=0;x<600; ++x) {
							if (ptr[y*600+x] & 0x00ffffff) {
								if (x > width)
									width = x;
								if (y > height)
									height = y;
							}
						}
					}
				}

				if (! width || ! height) {
					valid = false;
				} else {
					QImage img = QImage(width+1, height+1, QImage::Format_ARGB32);
					{
						QImage srcimg(reinterpret_cast<const uchar *>(qba.constData()), 600, 60, QImage::Format_ARGB32);

						QPainter imgp(&img);
						img.fill(0);
						imgp.setRenderHint(QPainter::Antialiasing);
						imgp.setRenderHint(QPainter::TextAntialiasing);
						imgp.setBackground(QColor(0,0,0,0));
						imgp.setCompositionMode(QPainter::CompositionMode_Source);
						imgp.drawImage(0, 0, srcimg);
					}
					cp->qbaTexture = QByteArray();

					QBuffer qb(& cp->qbaTexture);
					qb.open(QIODevice::WriteOnly);
					QImageWriter qiw(&qb, "png");
					qiw.write(img);

					cp->qbaTextureFormat = QString::fromLatin1("png").toUtf8();
				}
			}
		} else {
			QBuffer qb(& cp->qbaTexture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir;
			if (cp->qbaTexture.startsWith("<?xml"))
				qir.setFormat("svg");
			qir.setDevice(&qb);
			if (! qir.canRead() || (qir.size().width() > 1024) || (qir.size().height() > 1024)) {
				valid = false;
			} else {
				cp->qbaTextureFormat = qir.format();
				QImage qi = qir.read();
				valid = ! qi.isNull();
			}
		}
		if (! valid) {
			cp->qbaTexture = QByteArray();
			cp->qbaTextureHash = QByteArray();
		}
	}

	if (allowupdate)
		setTexts(qlCurrentTexts);
}

typedef QPair<QString, quint32> qpChanCol;

void Overlay::updateOverlay() {
	quint32 colUser = g.s.os.qcChannel.rgba();
	quint32 colTalking = g.s.os.qcChannel.rgba();
	quint32 colWhisper = g.s.os.qcChannel.rgba();
	quint32 colChannel = g.s.os.qcChannel.rgba();
	quint32 colChannelTalking = g.s.os.qcChannel.rgba();
	QString str;
	QList<qpChanCol> linkchans;
	QList<OverlayTextLine> lines;

	if (! g.uiSession) {
		qsQueried.clear();
		setTexts(lines);
		return;
	}

	Channel *home = ClientUser::get(g.uiSession)->cChannel;
	foreach(Channel *c, home->allLinks()) {
		if (home == c)
			continue;

		bool act = false;
		foreach(User *p, c->qlUsers) {
			ClientUser *u = static_cast<ClientUser *>(p);
			bool talking = (u->tsState != ClientUser::TalkingOff);
			act = act || talking;
			if (talking)
				linkchans << qpChanCol(p->qsName + QString::fromLatin1("[") + c->qsName + QString::fromLatin1("]"), colChannelTalking);
		}
		if (! act)
			linkchans << qpChanCol(c->qsName, colChannel);
	}
	qSort(linkchans);

	if (1) {
		foreach(qpChanCol cc, linkchans) {
			if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
				lines << OverlayTextLine(cc.first, cc.second, 0);
			}
		}
		if (linkchans.count() > 0) {
			lines << OverlayTextLine(QString(), 0, 0);
		}
	}

	foreach(User *p, ClientUser::get(g.uiSession)->cChannel->qlUsers) {
		ClientUser *u = static_cast<ClientUser *>(p);
		if ((g.s.osOverlay == Settings::All) || (u->tsState != ClientUser::TalkingOff) || ((u == ClientUser::get(g.uiSession)) && g.s.bOverlayAlwaysSelf)) {
			QString name = u->qsName;
			OverlayTextLine::Decoration dec = OverlayTextLine::None;
			if (u->bDeaf || u->bSelfDeaf)
				dec = OverlayTextLine::Deafened;
			else if (u->bMute || u->bSelfMute || u->bLocalMute)
				dec = OverlayTextLine::Muted;
			quint32 col;
			switch (u->tsState) {
				case ClientUser::TalkingOff:
					col = colUser;
					break;
				case ClientUser::Talking:
					col = colTalking;
					break;
				default:
					col = colWhisper;
					break;
			}
			lines << OverlayTextLine(name, col, 1, u->uiSession, dec);
		}
	}

	qSort(lines);
	setTexts(lines);
}

void Overlay::setTexts(const QList<OverlayTextLine> &lines) {
	qlCurrentTexts = lines;

	if (qlClients.isEmpty())
		return;

	QSet<unsigned int> query;

	foreach(const OverlayTextLine &e, lines) {
		ClientUser *cp = ClientUser::get(e.uiSession);
		if (cp && ! cp->qbaTextureHash.isEmpty()) {
			if (cp->qbaTexture.isEmpty() && ! qsQueried.contains(cp->uiSession)) {
				cp->qbaTexture=Database::blob(cp->qbaTextureHash);
				if (cp->qbaTexture.isEmpty())
					query.insert(cp->uiSession);
				else
					verifyTexture(cp, false);
			}
		}
	}

	if (! query.isEmpty()) {
		MumbleProto::RequestBlob mprb;
		foreach(unsigned int session, query) {
			qsQueried.insert(session);
			mprb.add_session_texture(session);
		}
		g.sh->sendMessage(mprb);
	}

	foreach(OverlayClient *oc, qlClients) {
		if (! oc->setTexts(lines)) {
			qWarning() << "Overlay: Dead client detected";
			qlClients.removeAll(oc);
			oc->scheduleDelete();
			break;
		}
	}
}
