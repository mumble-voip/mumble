// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayClient.h"
#include "Channel.h"
#include "Database.h"
#include "MainWindow.h"
#include "NetworkConfig.h"
#include "OverlayEditor.h"
#include "OverlayPositionableItem.h"
#include "OverlayText.h"
#include "ServerHandler.h"
#include "Themes.h"
#include "User.h"
#include "Utils.h"
#include "Global.h"
#include "GlobalShortcut.h"

#ifdef Q_OS_WIN
#	include <QtGui/QBitmap>
#endif

#include <QtGui/QImageReader>
#include <QtWidgets/QGraphicsProxyWidget>

#ifdef Q_OS_WIN
#	include <psapi.h>
#endif

OverlayClient::OverlayClient(QLocalSocket *socket, QObject *p)
	: QObject(p), framesPerSecond(0), ougUsers(&Global::get().s.os), iMouseX(0), iMouseY(0) {
	qlsSocket = socket;
	qlsSocket->setParent(nullptr);
	connect(qlsSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	omMsg.omh.iLength = -1;
	smMem             = nullptr;
	iWidth = iHeight = 0;

	uiPid = ~0ULL;

	bWasVisible = false;
	bDelete     = false;

	qgv.setScene(&qgs);
	qgv.installEventFilter(this);
	qgv.viewport()->installEventFilter(this);

	// Make sure it has a native window id
	qgv.winId();

	qgpiCursor.reset(new OverlayMouse());
	qgpiCursor->hide();
	qgpiCursor->setZValue(10.0f);

	ougUsers.setZValue(-1.0f);
	qgs.addItem(&ougUsers);
	ougUsers.show();

	qgpiFPS.reset(new OverlayPositionableItem(&Global::get().s.os.qrfFps));
	qgs.addItem(qgpiFPS.data());
	qgpiFPS->setPos(Global::get().s.os.qrfFps.x(), Global::get().s.os.qrfFps.y());
	qgpiFPS->show();

	// Time
	qgpiTime.reset(new OverlayPositionableItem(&Global::get().s.os.qrfTime));
	qgs.addItem(qgpiTime.data());
	qgpiTime->setPos(Global::get().s.os.qrfTime.x(), Global::get().s.os.qrfTime.y());
	qgpiTime->show();

	iOffsetX = iOffsetY = 0;

	connect(&qgs, SIGNAL(changed(const QList< QRectF > &)), this, SLOT(changed(const QList< QRectF > &)));
}

OverlayClient::~OverlayClient() {
	qlsSocket->disconnectFromServer();
	if (!qlsSocket->waitForDisconnected(1000)) {
		qDebug() << "OverlayClient: Failed to cleanly disconnect: " << qlsSocket->errorString();
		qlsSocket->abort();
	}

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
	if (Global::get().s.os.bFps) {
		const BasepointPixmap &pm =
			OverlayTextLine(QString(QLatin1String("%1")).arg(static_cast< int >(framesPerSecond + 0.5f)),
							Global::get().s.os.qfFps)
				.createPixmap(Global::get().s.os.qcFps);
		qgpiFPS->setVisible(true);
		qgpiFPS->setPixmap(pm);
		// offset to use basepoint
		// TODO: settings are providing a top left anchor, so shift down by ascent
		qgpiFPS->setOffset(-pm.qpBasePoint + QPoint(0, pm.iAscent));
		qgpiFPS->updateRender();
	} else {
		qgpiFPS->setVisible(false);
	}
}

void OverlayClient::updateTime() {
	if (Global::get().s.os.bTime) {
		const BasepointPixmap &pm =
			OverlayTextLine(QString(QLatin1String("%1")).arg(QTime::currentTime().toString()), Global::get().s.os.qfFps)
				.createPixmap(Global::get().s.os.qcFps);
		qgpiTime->setVisible(true);
		qgpiTime->setPixmap(pm);
		qgpiTime->setOffset(-pm.qpBasePoint + QPoint(0, pm.iAscent));
		qgpiTime->updateRender();
	} else {
		qgpiTime->setVisible(false);
	}
}

#if !defined(Q_OS_MAC) || (defined(Q_OS_MAC) && defined(USE_MAC_UNIVERSAL))
void OverlayClient::updateMouse() {
#	if defined(Q_OS_WIN)
	QPixmap pm;

	HICON c = ::GetCursor();
	ICONINFO info;
	ZeroMemory(&info, sizeof(info));
	if (c && ::GetIconInfo(c, &info)) {
		extern QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int format = 0);

		if (info.hbmColor) {
			pm = QBitmap::fromPixmap(qt_pixmapFromWinHBITMAP(info.hbmColor));
			pm.setMask(QBitmap::fromPixmap(qt_pixmapFromWinHBITMAP(info.hbmMask)));
		} else {
			const auto orig  = QBitmap::fromPixmap(qt_pixmapFromWinHBITMAP(info.hbmMask));
			const QImage img = orig.toImage();

			const int h         = img.height() / 2;
			const std::size_t w = img.bytesPerLine() / sizeof(quint32);

			QImage out(img.width(), h, QImage::Format_MonoLSB);
			QImage outmask(img.width(), h, QImage::Format_MonoLSB);

			for (int i = 0; i < h; ++i) {
				const quint32 *srcimg  = reinterpret_cast< const quint32 * >(img.scanLine(i + h));
				const quint32 *srcmask = reinterpret_cast< const quint32 * >(img.scanLine(i));

				quint32 *dstimg  = reinterpret_cast< quint32 * >(out.scanLine(i));
				quint32 *dstmask = reinterpret_cast< quint32 * >(outmask.scanLine(i));

				for (int j = 0; j < w; ++j) {
					dstmask[j] = srcmask[j];
					dstimg[j]  = srcimg[j];
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
#	else
#	endif

	qgpiCursor->setPos(iMouseX - iOffsetX, iMouseY - iOffsetY);
}
#endif

// Qt gets very very unhappy if we embed or unmbed the widget that an event is called from.
// This means that if any modal dialog is open, we'll be in a event loop of an object
// that we're about to reparent.

void OverlayClient::showGui() {
	int count = 0;

	{
		QWidgetList widgets = qApp->topLevelWidgets();
		foreach (QWidget *w, widgets) {
			if (w->isHidden() && (w != Global::get().mw))
				continue;
			count++;
		}
	}
	// If there's more than one window up, we're likely deep in a message loop.
	if (count > 1)
		return;

	Global::get().ocIntercept = this;

	bWasVisible = !Global::get().mw->isHidden();

	if (bWasVisible) {
		Global::get().mw->storeState(Global::get().s.bMinimalView);
	}

	{
	outer:
		QWidgetList widgets = qApp->topLevelWidgets();
		widgets.removeAll(Global::get().mw);
		widgets.prepend(Global::get().mw);

		foreach (QWidget *w, widgets) {
			if (!w->graphicsProxyWidget()) {
				if ((w == Global::get().mw) || (!w->isHidden())) {
					QGraphicsProxyWidget *qgpw = new QGraphicsProxyWidget(nullptr, Qt::Window);
					qgpw->setOpacity(0.90f);
					qgpw->setWidget(w);
					if (w == Global::get().mw) {
						qgpw->setPos(static_cast< float >(iWidth) / 10, static_cast< float >(iHeight) / 10);
						qgpw->resize(static_cast< float >(iWidth * 8) / 10, static_cast< float >(iHeight * 8) / 10);
					}

					qgs.addItem(qgpw);
					qgpw->show();
					qgpw->setActive(true);
					goto outer;
				}
			}
		}
	}

	QEvent activateEvent(QEvent::WindowActivate);
	qApp->sendEvent(&qgs, &activateEvent);

	QPoint p = QCursor::pos();
	iMouseX  = qBound< int >(0, p.x(), static_cast< int >(iWidth) - 1);
	iMouseY  = qBound< int >(0, p.y(), static_cast< int >(iHeight) - 1);

	qgpiCursor->setPos(iMouseX, iMouseY);

	qgs.setFocus();
#ifndef Q_OS_MAC
	Global::get().mw->qteChat->activateWindow();
#endif
	Global::get().mw->qteChat->setFocus();

	qgv.setAttribute(Qt::WA_WState_Hidden, false);
	qgv.activateWindow();
	qgv.setFocus();

	ougUsers.bShowExamples = true;

#ifdef Q_OS_MAC
	qApp->setAttribute(Qt::AA_DontUseNativeMenuBar);
	Global::get().mw->setUnifiedTitleAndToolBarOnMac(false);
	if (!Global::get().s.os.qsStyle.isEmpty())
		qApp->setStyle(Global::get().s.os.qsStyle);
#endif

	setupScene(true);

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType  = OVERLAY_MSGTYPE_INTERACTIVE;
	om.omh.iLength = sizeof(struct OverlayMsgInteractive);
	om.omin.state  = true;
	qlsSocket->write(om.headerbuffer, static_cast< int >(sizeof(OverlayMsgHeader)) + om.omh.iLength);

	Global::get().o->updateOverlay();
}

void OverlayClient::hideGui() {
	ougUsers.bShowExamples = false;

	QList< QWidget * > widgetlist;

	foreach (QGraphicsItem *qgi, qgs.items(Qt::DescendingOrder)) {
		QGraphicsProxyWidget *qgpw = qgraphicsitem_cast< QGraphicsProxyWidget * >(qgi);
		if (qgpw && qgpw->widget()) {
			QWidget *w = qgpw->widget();

			qgpw->setVisible(false);
			widgetlist << w;
		}
	}

	foreach (QWidget *w, widgetlist) {
		QGraphicsProxyWidget *qgpw = w->graphicsProxyWidget();
		if (qgpw) {
			qgpw->setVisible(false);
			qgpw->setWidget(nullptr);
			delete qgpw;
		}
	}

	if (Global::get().ocIntercept == this)
		Global::get().ocIntercept = nullptr;

	foreach (QWidget *w, widgetlist) {
		if (bWasVisible)
			w->show();
	}

	if (bWasVisible) {
		Global::get().mw->loadState(Global::get().s.bMinimalView);
	}

#ifdef Q_OS_MAC
	qApp->setAttribute(Qt::AA_DontUseNativeMenuBar, false);
	Global::get().mw->setUnifiedTitleAndToolBarOnMac(true);
	Themes::apply();
#endif

	setupScene(false);

	qgv.setAttribute(Qt::WA_WState_Hidden, true);

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType  = OVERLAY_MSGTYPE_INTERACTIVE;
	om.omh.iLength = sizeof(struct OverlayMsgInteractive);
	om.omin.state  = false;
	qlsSocket->write(om.headerbuffer, static_cast< int >(sizeof(OverlayMsgHeader)) + om.omh.iLength);

	Global::get().o->updateOverlay();

	if (bDelete)
		deleteLater();
}

void OverlayClient::scheduleDelete() {
	bDelete = true;
	hideGui();
}

void OverlayClient::readyReadMsgInit(unsigned int length) {
	if (length != sizeof(OverlayMsgInit)) {
		return;
	}

	OverlayMsgInit *omi = &omMsg.omi;

	iWidth  = static_cast< int >(omi->uiWidth);
	iHeight = static_cast< int >(omi->uiHeight);
	qrLast  = QRect();

	delete smMem;

	smMem = new SharedMemory2(this, static_cast< unsigned int >(iWidth * iHeight * 4));
	if (!smMem->data()) {
		qWarning() << "OverlayClient: Failed to create shared memory" << iWidth << iHeight;
		delete smMem;
		smMem = nullptr;
		return;
	}
	QByteArray key = smMem->name().toUtf8();
	key.append(static_cast< char >(0));

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType  = OVERLAY_MSGTYPE_SHMEM;
	om.omh.iLength = static_cast< int >(key.length());
	Q_ASSERT(sizeof(om.oms.a_cName) >= static_cast< size_t >(key.length())); // Name should be auto-generated and short
	memcpy(om.oms.a_cName, key.constData(), static_cast< std::size_t >(key.length()));
	qlsSocket->write(om.headerbuffer, static_cast< int >(sizeof(OverlayMsgHeader)) + om.omh.iLength);

	setupRender();

	Overlay *o = static_cast< Overlay * >(parent());
	QTimer::singleShot(0, o, SLOT(updateOverlay()));
}

void OverlayClient::readyRead() {
	while (true) {
		quint64 ready = static_cast< quint64 >(qlsSocket->bytesAvailable());

		if (omMsg.omh.iLength == -1) {
			if (ready < sizeof(OverlayMsgHeader)) {
				break;
			} else {
				qlsSocket->read(omMsg.headerbuffer, sizeof(OverlayMsgHeader));
				if ((omMsg.omh.uiMagic != OVERLAY_MAGIC_NUMBER) || (omMsg.omh.iLength < 0)
					|| (omMsg.omh.iLength > static_cast< int >(sizeof(OverlayMsgShmem)))) {
					disconnect();
					return;
				}
				ready -= sizeof(OverlayMsgHeader);
			}
		}

		if (ready >= static_cast< unsigned int >(omMsg.omh.iLength)) {
			qint64 length = qlsSocket->read(omMsg.msgbuffer, omMsg.omh.iLength);

			if (length != omMsg.omh.iLength) {
				disconnect();
				return;
			}

			switch (omMsg.omh.uiType) {
				case OVERLAY_MSGTYPE_INIT: {
					readyReadMsgInit(static_cast< unsigned int >(length));
				} break;
				case OVERLAY_MSGTYPE_SHMEM: {
					if (smMem)
						smMem->systemRelease();
				} break;
				case OVERLAY_MSGTYPE_PID: {
					if (length != static_cast< qint64 >(sizeof(OverlayMsgPid)))
						break;

					OverlayMsgPid *omp = &omMsg.omp;
					uiPid              = omp->pid;
#ifdef Q_OS_WIN
					HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD) uiPid);
					if (h) {
						wchar_t buf[MAX_PATH];
						if (GetModuleFileNameEx(h, 0, buf, MAX_PATH) != 0) {
							qsExecutablePath = QString::fromWCharArray(buf);
						}
						CloseHandle(h);
					}
#else
					qsExecutablePath = QLatin1String("Unknown");
#endif
				} break;
				case OVERLAY_MSGTYPE_FPS: {
					if (length != sizeof(OverlayMsgFps))
						break;

					OverlayMsgFps *omf = &omMsg.omf;
					framesPerSecond    = omf->fps;
					// qWarning() << "FPS: " << omf->fps;

					Overlay *o = static_cast< Overlay * >(parent());
					QTimer::singleShot(0, o, SLOT(updateOverlay()));
				} break;
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
	if (!iWidth || !iHeight || !smMem)
		return;

	qgpiLogo.reset();

	ougUsers.reset();

	setupScene(Global::get().ocIntercept == this);
}

void OverlayClient::setupScene(bool show) {
	if (show) {
		qgs.setBackgroundBrush(QColor(0, 0, 0, 64));

		if (!qgpiLogo) {
			qgpiLogo.reset(new OverlayMouse());
			qgpiLogo->hide();
			qgpiLogo->setOpacity(0.8f);
			qgpiLogo->setZValue(-5.0f);


			QImageReader qir(QLatin1String("skin:mumble.svg"));
			QSize sz = qir.size();
			sz.scale(static_cast< int >(iWidth), static_cast< int >(iHeight), Qt::KeepAspectRatio);
			qir.setScaledSize(sz);

			qgpiLogo->setPixmap(QPixmap::fromImage(qir.read()));

			QRectF qrf = qgpiLogo->boundingRect();
			qgpiLogo->setPos(static_cast< int >((iWidth - qrf.width()) / 2.0f + 0.5f),
							 static_cast< int >((iHeight - qrf.height()) / 2.0f + 0.5f));
		}

		qgpiCursor->show();
		qgs.addItem(qgpiCursor.data());

		qgpiLogo->show();
		qgs.addItem(qgpiLogo.data());
	} else {
		qgs.setBackgroundBrush(Qt::NoBrush);

		if (qgpiCursor->scene())
			qgs.removeItem(qgpiCursor.data());
		qgpiCursor->hide();

		if (qgpiLogo) {
			if (qgpiLogo->scene())
				qgs.removeItem(qgpiLogo.data());
			qgpiLogo->hide();
		}
	}
	ougUsers.updateUsers();
	updateFPS();
	updateTime();
}

void OverlayClient::setupRender() {
	qgs.setSceneRect(0, 0, iWidth, iHeight);
	qgv.setScene(nullptr);
	qgv.setGeometry(-2, -2, static_cast< int >(iWidth) + 2, static_cast< int >(iHeight) + 2);
	qgv.viewport()->setGeometry(0, 0, static_cast< int >(iWidth), static_cast< int >(iHeight));
	qgv.setScene(&qgs);

	smMem->erase();

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType  = OVERLAY_MSGTYPE_BLIT;
	om.omh.iLength = sizeof(OverlayMsgBlit);
	om.omb.x       = 0;
	om.omb.y       = 0;
	om.omb.w       = static_cast< unsigned int >(iWidth);
	om.omb.h       = static_cast< unsigned int >(iHeight);
	qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgBlit));

	reset();
}

bool OverlayClient::update() {
	if (!iWidth || !iHeight || !smMem)
		return true;

	ougUsers.updateUsers();
	updateFPS();
	updateTime();

	if (qlsSocket->bytesToWrite() > 1024) {
		return (t.elapsed() <= 5000000ULL);
	} else {
		t.restart();
		return true;
	}
}

void OverlayClient::changed(const QList< QRectF > &region) {
	if (region.isEmpty())
		return;

	qlDirty.append(region);
	QMetaObject::invokeMethod(this, "render", Qt::QueuedConnection);
}

void OverlayClient::render() {
	const QList< QRectF > region = qlDirty;
	qlDirty.clear();

	if (!iWidth || !iHeight || !smMem)
		return;

	QRect active;
	QRectF dirtyf;

	if (region.isEmpty())
		return;

	foreach (const QRectF &r, region) { dirtyf |= r; }


	QRect dirty = dirtyf.toAlignedRect();
	dirty       = dirty.intersected(QRect(0, 0, iWidth, iHeight));

	if ((dirty.width() <= 0) || (dirty.height() <= 0))
		return;

	QRect target = dirty;
	target.moveTo(0, 0);

	QImage img(reinterpret_cast< unsigned char * >(smMem->data()), iWidth, iHeight,
			   QImage::Format_ARGB32_Premultiplied);
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
		om.omh.uiType  = OVERLAY_MSGTYPE_BLIT;
		om.omh.iLength = sizeof(OverlayMsgBlit);
		om.omb.x       = static_cast< unsigned int >(dirty.x());
		om.omb.y       = static_cast< unsigned int >(dirty.y());
		om.omb.w       = static_cast< unsigned int >(dirty.width());
		om.omb.h       = static_cast< unsigned int >(dirty.height());
		qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgBlit));
	}

	if (qgpiCursor->isVisible()) {
		active = QRect(0, 0, iWidth, iHeight);
	} else {
		active = qgs.itemsBoundingRect().toAlignedRect();
		if (active.isEmpty())
			active = QRect(0, 0, 0, 0);
		active = active.intersected(QRect(0, 0, iWidth, iHeight));
	}

	if (active != qrLast) {
		qrLast = active;

		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType  = OVERLAY_MSGTYPE_ACTIVE;
		om.omh.iLength = sizeof(OverlayMsgActive);
		om.oma.x       = static_cast< unsigned int >(qrLast.x());
		om.oma.y       = static_cast< unsigned int >(qrLast.y());
		om.oma.w       = static_cast< unsigned int >(qrLast.width());
		om.oma.h       = static_cast< unsigned int >(qrLast.height());
		qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgActive));
	}

	qlsSocket->flush();
}

void OverlayClient::openEditor() {
	OverlayEditor oe(Global::get().mw, &ougUsers);
	connect(&oe, SIGNAL(applySettings()), this, SLOT(updateLayout()));

	oe.exec();
}
