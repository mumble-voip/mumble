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
#include "OverlayText.h"
#include "User.h"
#include "Channel.h"
#include "Global.h"
#include "Message.h"
#include "Database.h"
#include "NetworkConfig.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "GlobalShortcut.h"

OverlayClient::OverlayClient(QLocalSocket *socket, QObject *p) :
		QObject(p),
		ougUsers(&g.s.os) {
	qlsSocket = socket;
	qlsSocket->setParent(NULL);
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

	ougUsers.setZValue(-1.0f);
	qgs.addItem(&ougUsers);
	ougUsers.show();

	qgpiFPS = new QGraphicsPixmapItem();
	qgs.addItem(qgpiFPS);
	qgpiFPS->setPos(g.s.os.qrfFps.x(), g.s.os.qrfFps.y());
	qgpiFPS->show();

	qgpiLogo = NULL;

	iOffsetX = iOffsetY = 0;

	connect(&qgs, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(changed(const QList<QRectF> &)));
}

OverlayClient::~OverlayClient() {
	delete qgpiFPS;
	delete qgpiCursor;
	delete qgpiLogo;

	qlsSocket->disconnect();
	qlsSocket->abort();
	qlsSocket->deleteLater();

	ougUsers.reset();
}

bool OverlayClient::eventFilter(QObject *o, QEvent *e) {
	if (e->type() == QEvent::Paint) {
		e->accept();
		return true;
	}
	return QObject::eventFilter(o, e);
}

void OverlayClient::updateFPS() {
	if (g.s.os.bFps) {
		const BasepointPixmap &pm = OverlayTextLine(tr("FPS: %1").arg(iroundf(fFps + 0.5f)), g.s.os.qfFps).createPixmap(g.s.os.qcFps);
		qgpiFPS->setPixmap(pm);
		// offset to use basepoint
		//TODO: settings are providing a top left anchor, so shift down by ascent
		qgpiFPS->setOffset(-pm.qpBasePoint + QPoint(0, pm.iAscent));
	} else {
		qgpiFPS->setPixmap(QPixmap());
	}
}

#ifndef QT_MAC_USE_COCOA
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
#endif

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
extern bool Q_GUI_EXPORT qt_use_native_dialogs;
#endif

// Qt gets very very unhappy if we embed or unmbed the widget that an event is called from.
// This means that if any modal dialog is open, we'll be in a event loop of an object
// that we're about to reparent.

void OverlayClient::showGui() {
#if defined(QT3_SUPPORT) || defined(Q_WS_WIN)
	if (QCoreApplication::loopLevel() > 1)
		return;
#else
	int count = 0;

	{
		QWidgetList widgets = qApp->topLevelWidgets();
		foreach(QWidget *w, widgets) {
			if (w->isHidden() && (w != g.mw))
				continue;
			count++;
		}
	}
	// If there's more than one window up, we're likely deep in a message loop.
	if (count > 1)
		return;
#endif

	g.ocIntercept = this;

	bWasVisible = ! g.mw->isHidden();

	if (bWasVisible) {
		if (g.s.bMinimalView) {
			g.s.qbaMinimalViewGeometry = g.mw->saveGeometry();
			g.s.qbaMinimalViewState = g.mw->saveState();
		} else {
			g.s.qbaMainWindowGeometry = g.mw->saveGeometry();
			g.s.qbaMainWindowState = g.mw->saveState();
			g.s.qbaHeaderState = g.mw->qtvUsers->header()->saveState();
		}
	}

	{
outer:
		QWidgetList widgets = qApp->topLevelWidgets();
		widgets.removeAll(g.mw);
		widgets.prepend(g.mw);

		foreach(QWidget *w, widgets) {
			if (w->graphicsProxyWidget() == NULL) {
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
#if QT_VERSION >= 0x040600
					qgpw->setActive(true);
#endif
					goto outer;
				}
			}
		}
	}

	QEvent event(QEvent::WindowActivate);
	qApp->sendEvent(&qgs, &event);

	QPoint p = QCursor::pos();
	iMouseX = qBound<int>(0, p.x(), uiWidth-1);
	iMouseY = qBound<int>(0, p.y(), uiHeight-1);

	qgpiCursor->setPos(iMouseX, iMouseY);

	qgs.setFocus();
#ifndef Q_OS_MAC
	g.mw->qteChat->activateWindow();
#endif
	g.mw->qteChat->setFocus();

	qgv.setAttribute(Qt::WA_WState_Hidden, false);
	qApp->setActiveWindow(&qgv);
	qgv.setFocus();

	ougUsers.bShowExamples = true;

#ifdef Q_OS_MAC
	qApp->setAttribute(Qt::AA_DontUseNativeMenuBar);
	g.mw->setUnifiedTitleAndToolBarOnMac(false);
	if (! g.s.os.qsStyle.isEmpty())
		qApp->setStyle(g.s.os.qsStyle);
#endif

	setupScene(true);

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
	qt_use_native_dialogs = false;
#endif

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType = OVERLAY_MSGTYPE_INTERACTIVE;
	om.omh.iLength = sizeof(struct OverlayMsgInteractive);
	om.omin.state = true;
	qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + om.omh.iLength);

	g.o->updateOverlay();
}

void OverlayClient::hideGui() {
#if defined(QT3_SUPPORT) || defined(Q_WS_WIN)
	if (QCoreApplication::loopLevel() > 1) {
		QCoreApplication::exit_loop();
		QMetaObject::invokeMethod(this, "hideGui", Qt::QueuedConnection);
		return;
	}
#endif

	ougUsers.bShowExamples = false;

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

	foreach(QWidget *w, widgetlist) {
		if (bWasVisible)
			w->show();
	}

	if (bWasVisible) {
		if (g.s.bMinimalView && ! g.s.qbaMinimalViewGeometry.isNull()) {
			g.mw->restoreGeometry(g.s.qbaMinimalViewGeometry);
			g.mw->restoreState(g.s.qbaMinimalViewState);
		} else if (! g.s.bMinimalView && ! g.s.qbaMainWindowGeometry.isNull()) {
			g.mw->restoreGeometry(g.s.qbaMainWindowGeometry);
			g.mw->restoreState(g.s.qbaMainWindowState);
		}
	}

#ifdef Q_OS_MAC
	qApp->setAttribute(Qt::AA_DontUseNativeMenuBar, false);
	g.mw->setUnifiedTitleAndToolBarOnMac(true);
	if (! g.qsCurrentStyle.isEmpty()) {
		qApp->setStyle(g.qsCurrentStyle);
	} else {
		// Assume that an empty qsCurrentStyle means "use the aqua theme".
		// This might not always be the case (for example, the default style
		// can be changed via the Qt command line argument "-style".
		qApp->setStyle(QLatin1String("Macintosh (aqua)"));
	}
#endif

	setupScene(false);

	qgv.setAttribute(Qt::WA_WState_Hidden, true);
#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
	qt_use_native_dialogs = true;
#endif

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType = OVERLAY_MSGTYPE_INTERACTIVE;
	om.omh.iLength = sizeof(struct OverlayMsgInteractive);
	om.omin.state = false;
	qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + om.omh.iLength);

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
				case OVERLAY_MSGTYPE_FPS: {
						if (length != sizeof(OverlayMsgFps))
							break;

						OverlayMsgFps *omf = & omMsg.omf;
						fFps = omf ->fps;
						//qWarning() << "FPS: " << omf->fps;

						Overlay *o = static_cast<Overlay *>(parent());
						QTimer::singleShot(0, o, SLOT(updateOverlay()));
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

	delete qgpiLogo;
	qgpiLogo = NULL;

	ougUsers.reset();

	setupScene(g.ocIntercept == this);
}

void OverlayClient::setupScene(bool show) {
	if (show) {
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
			qgpiLogo->setPos(iroundf((uiWidth - qrf.width()) / 2.0f + 0.5f), iroundf((uiHeight - qrf.height()) / 2.0f) + 0.5f);

		}

		qgpiCursor->show();
		qgs.addItem(qgpiCursor);

		qgpiLogo->show();
		qgs.addItem(qgpiLogo);
	} else {
		qgs.setBackgroundBrush(Qt::NoBrush);

		if (qgpiCursor->scene())
			qgs.removeItem(qgpiCursor);
		qgpiCursor->hide();

		if (qgpiLogo) {
			if (qgpiLogo->scene())
				qgs.removeItem(qgpiLogo);
			qgpiLogo->hide();
		}

	}
	ougUsers.updateUsers();
	updateFPS();
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

bool OverlayClient::update() {
	if (! uiWidth || ! uiHeight || ! smMem)
		return true;

	ougUsers.updateUsers();
	updateFPS();

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

	if ((dirty.width() <= 0) || (dirty.height() <= 0))
		return;

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

void OverlayClient::openEditor() {
	OverlayEditor oe(g.mw, &ougUsers);
	connect(&oe, SIGNAL(applySettings()), this, SLOT(updateLayout()));

	oe.exec();
}
