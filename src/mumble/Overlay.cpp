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

	loadCheckBox(qcbLeft, r.bOverlayLeft);
	loadCheckBox(qcbRight, r.bOverlayRight);
	loadCheckBox(qcbTop, r.bOverlayTop);
	loadCheckBox(qcbBottom, r.bOverlayBottom);

	loadSlider(qsX, iroundf(r.fOverlayX * 100.0f));
	loadSlider(qsY, 100 - iroundf(r.fOverlayY * 100.0f));
	qfFont = r.qfOverlayFont;
	qcUser = r.qcOverlayUser;
	qcTalking= r.qcOverlayTalking;
	qcWhisper= r.qcOverlayWhisper;
	qcChannel = r.qcOverlayChannel;
	qcChannelTalking = r.qcOverlayChannelTalking;
	qlCurrentFont->setText(qfFont.family());
	loadSlider(qsMaxHeight, iroundf(r.fOverlayHeight * 1000.0f));
	setColorLabel(qlUser, qcUser);
	setColorLabel(qlTalking, qcTalking);
	setColorLabel(qlWhisper, qcWhisper);
	setColorLabel(qlChannel, qcChannel);
	setColorLabel(qlChannelTalking, qcChannelTalking);
}

bool OverlayConfig::expert(bool b) {
	qcbLeft->setVisible(b);
	qcbRight->setVisible(b);
	qcbTop->setVisible(b);
	qcbBottom->setVisible(b);

	qgbFont->setVisible(b);
	return true;
}

void OverlayConfig::setColorLabel(QLabel *label, QColor col) {
	QString str = QString::fromLatin1("<span style=\"color: %1; opacity: %2\">%1</span>").arg(col.name()).arg(col.alphaF(), 0, 'f', 3);
	label->setText(str);
}

void OverlayConfig::on_qpbSetFont_clicked() {
	bool ok;
	QFont qf = QFontDialog::getFont(&ok, qfFont, this);
	if (ok) {
		qfFont = qf;
		qlCurrentFont->setText(qfFont.family());
		qsMaxHeight->setValue(qfFont.pointSize());
	}
}

void OverlayConfig::on_qsMaxHeight_valueChanged(int v) {
	qlMaxHeight->setText(QString::fromLatin1("%1%").arg(v/10.0, 0, 'f', 1));
	qfFont.setPointSize(v);
}

#if QT_VERSION >= 0x040500
#define UPDCOLOR(var, text) { QColor col = QColorDialog::getColor(qc##var, this, text, QColorDialog::ShowAlphaChannel); if (col.isValid()) { qc##var = col; setColorLabel(ql##var, qc##var); } }
#else
#define UPDCOLOR(var, text) { QColor col = QColorDialog::getColor(qc##var, this); if (col.isValid()) { qc##var = col; setColorLabel(ql##var, qc##var); } }
#endif

void OverlayConfig::on_qpbUser_clicked() {
	UPDCOLOR(User, tr("Color for users"));
}

void OverlayConfig::on_qpbTalking_clicked() {
	UPDCOLOR(Talking, tr("Color for talking users"));
}

void OverlayConfig::on_qpbWhisper_clicked() {
	UPDCOLOR(Whisper, tr("Color for whispering users"));
}

void OverlayConfig::on_qpbChannel_clicked() {
	UPDCOLOR(Channel, tr("Color for channels"));
}

void OverlayConfig::on_qpbChannelTalking_clicked() {
	UPDCOLOR(ChannelTalking, tr("Color for active channels"));
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
	s.fOverlayX = static_cast<float>(qsX->value()) / 100.0f;
	s.fOverlayY = 1.0f - static_cast<float>(qsY->value()) / 100.0f;
	s.bOverlayLeft = qcbLeft->isChecked();
	s.bOverlayRight = qcbRight->isChecked();
	s.bOverlayTop = qcbTop->isChecked();
	s.bOverlayBottom = qcbBottom->isChecked();
	s.qfOverlayFont = qfFont;
	s.fOverlayHeight = static_cast<float>(qsMaxHeight->value()) / 1000.0f;
	s.qcOverlayUser = qcUser;
	s.qcOverlayTalking = qcTalking;
	s.qcOverlayWhisper = qcWhisper;
	s.qcOverlayChannel = qcChannel;
	s.qcOverlayChannelTalking = qcChannelTalking;
}

void OverlayConfig::accept() const {
	g.o->forceSettings();
	g.o->setActive(s.bOverlayEnable);
}



OverlayUser::OverlayUser(ClientUser *cu, unsigned int height) : QGraphicsItemGroup(), cuUser(cu), uiSize(height), tsColor(Settings::Passive) {
	setup();
	updateLayout();
}

OverlayUser::OverlayUser(Settings::TalkState ts, unsigned int height) : QGraphicsItemGroup(), cuUser(NULL), uiSize(height), tsColor(ts) {
	setup(true);
	updateLayout();
	
	qsChannelName = tr("Channel Name");
	
	switch(ts) {
		case Settings::Passive:
			qsName = tr("Silent");
			break;
		case Settings::Talking:
			qsName = tr("Talking");
			break;
		case Settings::WhisperPrivate:
			qsName = tr("Private Whisper");
			break;
		case Settings::WhisperChannel:
			qsName = tr("Channel Whisper");
			break;
	}
}

void OverlayUser::setup(bool selectable) {
	setZValue(-4.0f);

	qgpiMuted = new QGraphicsPixmapItem();
	qgpiMuted->hide();
	addToGroup(qgpiMuted);

	qgpiDeafened = new QGraphicsPixmapItem();
	qgpiDeafened->hide();
	addToGroup(qgpiDeafened);

	qgpiAvatar = new QGraphicsPixmapItem();
	addToGroup(qgpiAvatar);

	for (int i=0;i<4;++i) {
		qgpiName[i] = new QGraphicsPixmapItem();
		qgpiName[i]->hide();
		addToGroup(qgpiName[i]);
	}

	qgpiChannel = new QGraphicsPixmapItem();
	qgpiChannel->hide();
	addToGroup(qgpiChannel);
	
	if (selectable) {
		qgpiMuted->setFlag(QGraphicsItem::ItemIsSelectable);
		qgpiDeafened->setFlag(QGraphicsItem::ItemIsSelectable);
		qgpiAvatar->setFlag(QGraphicsItem::ItemIsSelectable);
		qgpiChannel->setFlag(QGraphicsItem::ItemIsSelectable);
		for(int i=0;i<4;++i)
			qgpiName[i]->setFlag(QGraphicsItem::ItemIsSelectable);
	}
}

void OverlayUser::updateLayout() {
	QPixmap pm;

	for(int i=0;i<4;++i)
		qgpiName[i]->setPixmap(pm);
		
	qgpiAvatar->setPixmap(pm);
	qgpiChannel->setPixmap(pm);

	{
		QImageReader qir(QLatin1String("skin:muted_self.svg"));
		QSize sz = qir.size();
		sz.scale(uiSize / 2, uiSize / 2, Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiMuted->setPixmap(QPixmap::fromImage(qir.read()));
	}

	{
		QImageReader qir(QLatin1String("skin:deafened_self.svg"));
		QSize sz = qir.size();
		sz.scale(uiSize / 2, uiSize / 2, Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiDeafened->setPixmap(QPixmap::fromImage(qir.read()));
	}
	
	qgpiMuted->setPos(0.0f, 0.0f);
	qgpiMuted->setZValue(1.0f);
	qgpiMuted->setOpacity(g.s.fOverlayMutedDeafened);

	qgpiDeafened->setPos(0.0f, 0.0f);
	qgpiDeafened->setZValue(1.0f);
	qgpiDeafened->setOpacity(g.s.fOverlayMutedDeafened);

	qgpiAvatar->setPos(0.0f, 0.0f);
	qgpiAvatar->setOpacity(g.s.fOverlayAvatar);

	for (int i=0;i<4;++i) {
		qgpiName[i]->setPos(0.0f, (5 * uiSize) / 6);
		qgpiName[i]->setZValue(2.0f);
		qgpiName[i]->setOpacity(g.s.fOverlayUserName[i]);
	}
	qgpiChannel->setPos(0.0f, 0.0f);
	qgpiChannel->setZValue(2.0f);
	qgpiChannel->setOpacity(g.s.fOverlayChannel);
}

QPixmap OverlayUser::createPixmap(const QString &string, unsigned int height, unsigned int maxwidth, QColor col, const QFont &font, QPainterPath &pp) {
	float edge = height * 0.05f;

	if (pp.isEmpty()) {
		QFont f = font;

		QPainterPath qp;
		qp.addText(0.0f, 0.0f, f, string);

		QRectF r = qp.controlPointRect();

		float fs = f.pointSizeF();
		float ds = fs * ((height - 2.0f * edge) / r.height());

		f.setPointSizeF(ds);

		QString str;

		if (r.width() < maxwidth) {
			str = string;
		} else {
			QFontMetrics qfm(f);
			str = qfm.elidedText(string, Qt::ElideRight, iroundf(maxwidth - 2 * edge));
		}

		pp.addText(0.0f, 0.0f, f, str);
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
	if (qgpiName[0]->pixmap().isNull() || (cuUser && (qsName != cuUser->qsName))) {
		if (cuUser)
			qsName = cuUser->qsName;

		QPainterPath pp;
		for (int i=0; i<4; ++i) {
			const QPixmap &pm = createPixmap(qsName, uiSize / 6, uiSize, g.s.qcOverlayUserName[i], g.s.qfOverlayUserName, pp);
			qgpiName[i]->setPixmap(pm);
			qgpiName[i]->setX((uiSize - pm.width()) / 2);
		}
	}

	if (qgpiChannel->pixmap().isNull() || (cuUser && (qsChannelName != cuUser->cChannel->qsName))) {
		if (cuUser)
			qsChannelName = cuUser->cChannel->qsName;

		QPainterPath pp;
		const QPixmap &pm = createPixmap(qsChannelName, uiSize / 6, uiSize, g.s.qcOverlayChannel, g.s.qfOverlayChannel, pp);
		qgpiChannel->setPixmap(pm);
		qgpiChannel->setX(uiSize - pm.width());
	}

	if (qgpiAvatar->pixmap().isNull() || (cuUser && (qbaAvatar != cuUser->qbaTextureHash))) {
		if (cuUser)
			qbaAvatar = cuUser->qbaTextureHash;

		QImage img;
		if (qbaAvatar.isNull()) {
			QImageReader qir(QLatin1String("skin:default_avatar.svg"));
			QSize sz = qir.size();
			sz.scale(uiSize, uiSize, Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		} else {
			QBuffer qb(& cuUser->qbaTexture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir(&qb, cuUser->qbaTextureFormat);
			QSize sz = qir.size();
			sz.scale(uiSize, uiSize, Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		}

		qgpiAvatar->setPixmap(QPixmap::fromImage(img));
	}

	qgpiAvatar->show();
	
	if (cuUser) {
		ClientUser *self = ClientUser::get(g.uiSession);

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

		bool samechannel = self && (self->cChannel == cuUser->cChannel);
		qgpiChannel->setVisible(! samechannel);

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
		qgpiChannel->setVisible((tsColor != Settings::Passive) && (tsColor != Settings::Talking));
		qgpiMuted->show();
		qgpiDeafened->hide();
	}		

	for (int i=0;i<4;++i)
		qgpiName[i]->setVisible(i == tsColor);

	setOpacity(g.s.fOverlayUser[tsColor]);
}

QGraphicsPixmapItem *OverlayUser::childAt(const QPointF &pos) {
	QGraphicsItem *item = NULL;

	foreach(QGraphicsItem *qgi, childItems()) {
		QPointF qp = mapToItem(qgi, pos);
		if (qgi->isVisible() && qgi->contains(qp)) {
			if (qgi->isSelected())
				return static_cast<QGraphicsPixmapItem *>(qgi);
			item = qgi;
		}
	}
	return static_cast<QGraphicsPixmapItem *>(item);
}

void OverlayUser::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
	QGraphicsPixmapItem *item = childAt(event->pos());
	if (! item || !(item->flags() & QGraphicsItem::ItemIsSelectable)) {
		QGraphicsItemGroup::contextMenuEvent(event);
		return;
	}

	event->accept();

	QMenu qm(g.mw);

	QMenu *qmTrans = qm.addMenu(tr("User Opacity"));
	QActionGroup *qagUser = new QActionGroup(&qm);
	QAction *userOpacity[8];
	for(int i=0;i<8;++i) {
		qreal o = (i + 1) / 8.0f;

		userOpacity[i] = new QAction(tr("%1%").arg(o * 100.0f, 0, 'f', 1), qagUser);
		userOpacity[i]->setCheckable(true);
		userOpacity[i]->setData(o);
		if (qFuzzyCompare(opacity(), o))
			userOpacity[i]->setChecked(true);
		qmTrans->addAction(userOpacity[i]);
	}
	
	qm.addSeparator();

	QMenu *qmObjTrans = qm.addMenu(tr("Object Opacity"));
	QActionGroup *qagObject = new QActionGroup(&qm);
	QAction *objectOpacity[10];
	for(int i=0;i<8;++i) {
		qreal o = (i + 1) / 8.0f;

		objectOpacity[i] = new QAction(tr("%1%").arg(o * 100.0f, 0, 'f', 1), qagObject);
		objectOpacity[i]->setCheckable(true);
		objectOpacity[i]->setData(o);
		if (qFuzzyCompare(item->opacity(), o))
			objectOpacity[i]->setChecked(true);
		qmObjTrans->addAction(objectOpacity[i]);
	}

	
	QAction *color = NULL;
	QAction *font = NULL;

	if ((item != qgpiAvatar) && (item != qgpiMuted) && (item != qgpiDeafened)) {
		color = qm.addAction(tr("Color..."));
		font = qm.addAction(tr("Font..."));
	}

	QAction *act = qm.exec(event->screenPos());
	if (act) {

		for(int i=0;i<8;++i) {
			if (userOpacity[i] == act) {
				float o = act->data().toReal();
				g.s.fOverlayUser[tsColor] = o;
				g.o->forceSettings();
			}
		}

		for(int i=0;i<8;++i) {
			if (objectOpacity[i] == act) {
				qreal o = act->data().toReal();

				if ((item == qgpiMuted) || (item == qgpiDeafened))
					g.s.fOverlayMutedDeafened = o;
				else if (item == qgpiAvatar)
					g.s.fOverlayAvatar = o;
				else if (item == qgpiChannel)
					g.s.fOverlayChannel = o;
				else
					for(int i=0;i<4;++i)
						if (item == qgpiName[i])
							g.s.fOverlayUserName[i] = o;

				g.o->forceSettings();
			}
		}

		if (act == color) {
			QColor *col = NULL;
			if (item == qgpiChannel)
				col = & g.s.qcOverlayChannel;
			else
				for(int i=0;i<4;++i)
					if (item == qgpiName[i])
						col = & g.s.qcOverlayUserName[i];
			
			if (col) {
				QColor qc = QColorDialog::getColor(*col, g.mw, tr("Pick color"), QColorDialog::DontUseNativeDialog);
				if (qc.isValid()) {
					qc.setAlpha(255);
					if (qc != *col) {
						*col = qc;
						g.o->forceSettings();
					}
				}
			}
		} else if (act == font) {
			QFont *fontptr = (item == qgpiChannel) ? &g.s.qfOverlayChannel : &g.s.qfOverlayUserName;
			
			// QFontDialog doesn't really like graphics view. At all.

			QFontDialog qfd;
			qfd.setOptions(QFontDialog::DontUseNativeDialog);
			qfd.setCurrentFont(*fontptr);
			qfd.setWindowTitle(tr("Pick font"));
			
			QGraphicsProxyWidget *qgpw = new QGraphicsProxyWidget(NULL, Qt::Window);
			qgpw->setWidget(&qfd);
			scene()->addItem(qgpw);
			qgpw->show();
			
			int ret = qfd.exec();
			
			qgpw->hide();
			qgpw->setWidget(NULL);
			delete qgpw;
			
			if (ret) {
				*fontptr = qfd.selectedFont();
				g.o->forceSettings();
			}
		}
	}
}

OverlayMouse::OverlayMouse(QGraphicsItem *p) : QGraphicsPixmapItem(p) {
}

bool OverlayMouse::contains(const QPointF &) const {
	return false;
}

bool OverlayMouse::collidesWithPath(const QPainterPath &, Qt::ItemSelectionMode) const {
	return false;
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
			QImage img = QPixmap::fromWinHBITMAP(info.hbmMask).toImage();
			img.invertPixels();
			QBitmap full = QBitmap::fromImage(img);
			pm = full.copy(0, 0, full.width(), full.height() / 2);
			pm.setMask(QBitmap(full.copy(0, full.height() / 2, full.width(), full.height() / 2)));
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

			w->showNormal();

			qgpw->setActive(true);
			qgs.addItem(qgpw);
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
			qlExampleUsers << new OverlayUser(Settings::Passive, iItemHeight);
			qlExampleUsers << new OverlayUser(Settings::Talking, iItemHeight);
			qlExampleUsers << new OverlayUser(Settings::WhisperPrivate, iItemHeight);
			qlExampleUsers << new OverlayUser(Settings::WhisperChannel, iItemHeight);
		}
	} else {
		qgs.setBackgroundBrush(Qt::NoBrush);

		if (qgpiLogo)
			qgpiLogo->hide();
	}
}

void OverlayClient::setupRender() {
	iItemHeight = iroundf(uiHeight * g.s.fOverlayHeight + 0.5f);

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
	foreach(QGraphicsItem *qgi, qgs.items())
		if (! qgi->parentItem() && (qgi->type() == 10))
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
				ou = new OverlayUser(cu, iItemHeight);
				connect(cu, SIGNAL(destroyed(QObject *)), this, SLOT(userDestroyed(QObject *)));
				qmUsers.insert(cu, ou);
			} else {
				items.removeAll(ou);
			}
			users << ou;
		}
	}

	foreach(QGraphicsItem *qgi, items)
		qgs.removeItem(qgi);

	int y = 0;
	int x = 0;

	foreach(OverlayUser *ou, users) {
		ou->setPos(iroundf(uiWidth * g.s.fOverlayX + x * iItemHeight * 1.05f), iroundf(uiHeight * g.s.fOverlayY + y * iItemHeight * 1.05f));
		ou->show();
		ou->updateUser();

		if (! ou->scene())
			qgs.addItem(ou);

		if (x) {
			x = 0;
			++y;
		} else {
			x = 1;
		}
	}

	if (qlsSocket->bytesToWrite() > 1024) {
		return (t.elapsed() <= 5000000ULL);
	} else {
		t.restart();
		return true;
	}
}

void OverlayClient::changed(const QList<QRectF> &region) {
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

	QRect target = dirty;
	target.moveTo(0,0);

	QImage img(reinterpret_cast<unsigned char *>(smMem->data()), uiWidth, uiHeight, QImage::Format_ARGB32);
	QImage qi(target.width(), target.height(), QImage::Format_ARGB32);
	qi.fill(0);

	{
		QPainter p(&qi);
		p.setRenderHint(QPainter::Antialiasing);
		p.setRenderHint(QPainter::TextAntialiasing);
		p.setCompositionMode(QPainter::CompositionMode_SourceOver);
		qgs.render(&p, target, dirty, Qt::IgnoreAspectRatio);
	}

	{
		QPainter p(&img);
		p.setCompositionMode(QPainter::CompositionMode_Source);
		p.drawImage(dirty.x(), dirty.y(), qi);
	}


	if (dirty.isValid()) {
		dirty = dirty.intersected(QRect(0,0,uiWidth, uiHeight));
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
	if (g.ocIntercept) {
		g.ocIntercept->hideGui();
		updateOverlay();
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
				updateOverlay();
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
	quint32 colUser = g.s.qcOverlayUser.rgba();
	quint32 colTalking = g.s.qcOverlayTalking.rgba();
	quint32 colWhisper = g.s.qcOverlayWhisper.rgba();
	quint32 colChannel = g.s.qcOverlayChannel.rgba();
	quint32 colChannelTalking = g.s.qcOverlayChannelTalking.rgba();
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

	if (g.s.bOverlayTop) {
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

	if (! g.s.bOverlayTop) {
		if (linkchans.count() > 0) {
			lines << OverlayTextLine(QString(), 0, 2);
		}
		foreach(qpChanCol cc, linkchans) {
			if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
				lines << OverlayTextLine(cc.first, cc.second, 2);
			}
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
