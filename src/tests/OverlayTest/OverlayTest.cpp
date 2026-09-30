// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * Overlay drawing test application.
 */

#include "../../overlay/overlay.h"

#include "SharedMemory.h"
#include "Timer.h"

#include "IPCUtils.h"

#ifdef Q_OS_WIN
#	include "win.h"
#else
#	include <unistd.h>
#endif

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>


class OverlayWidget : public QWidget {
	Q_OBJECT

protected:
	QImage img;
	OverlayMsg om;
	QLocalSocket *qlsSocket;
	SharedMemory2 *smMem;
	QTimer *qtTimer;
	QRect qrActive;
	QElapsedTimer qtWall;

	unsigned int iFrameCount;
	int iLastFpsUpdate;

	unsigned int uiWidth, uiHeight;

	void resizeEvent(QResizeEvent *);
	void paintEvent(QPaintEvent *);
	void init(const QSize &);
	void detach();

	void keyPressEvent(QKeyEvent *);
protected slots:
	void connected();
	void disconnected();
	void readyRead();
	void error(QLocalSocket::LocalSocketError);
	void update();

public:
	OverlayWidget(QWidget *p = nullptr);
};

OverlayWidget::OverlayWidget(QWidget *p) : QWidget(p) {
	qlsSocket = nullptr;
	smMem     = nullptr;
	uiWidth = uiHeight = 0;

	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	qtTimer = new QTimer(this);
	connect(qtTimer, SIGNAL(timeout()), this, SLOT(update()));
	qtTimer->start(100);
}

void OverlayWidget::keyPressEvent(QKeyEvent *evt) {
	evt->accept();
	qWarning("Keypress");
	detach();
}

void OverlayWidget::resizeEvent(QResizeEvent *evt) {
	QWidget::resizeEvent(evt);

	if (!img.isNull())
		img = img.scaled(evt->size().width(), evt->size().height());
	init(evt->size());
}

void OverlayWidget::paintEvent(QPaintEvent *) {
	if (!qlsSocket || qlsSocket->state() == QLocalSocket::UnconnectedState) {
		detach();

		qlsSocket = new QLocalSocket(this);
		connect(qlsSocket, SIGNAL(connected()), this, SLOT(connected()));
		connect(qlsSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(qlsSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
		connect(qlsSocket, SIGNAL(errorOccurred(QLocalSocket::LocalSocketError)), this,
				SLOT(error(QLocalSocket::LocalSocketError)));
		const QString pipepath = QString::fromStdString(Mumble::getOverlayPipePath().string());
		qWarning() << "connectToServer(" << pipepath << ")";
		qlsSocket->connectToServer(pipepath);
	}

	QPainter painter(this);
	painter.fillRect(0, 0, width(), height(), QColor(128, 0, 128));
	painter.setClipRect(qrActive);
	painter.drawImage(0, 0, img);
}

void OverlayWidget::init(const QSize &sz) {
	qWarning() << "Init" << sz.width() << sz.height();

	qtWall.start();
	iFrameCount    = 0;
	iLastFpsUpdate = 0;

	OverlayMsg m;
	m.omh.uiMagic  = OVERLAY_MAGIC_NUMBER;
	m.omh.uiType   = OVERLAY_MSGTYPE_INIT;
	m.omh.iLength  = sizeof(OverlayMsgInit);
	m.omi.uiWidth  = static_cast< unsigned int >(sz.width());
	m.omi.uiHeight = static_cast< unsigned int >(sz.height());

	if (qlsSocket && qlsSocket->state() == QLocalSocket::ConnectedState)
		qlsSocket->write(m.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgInit));

	img = QImage(sz.width(), sz.height(), QImage::Format_ARGB32_Premultiplied);
}

void OverlayWidget::detach() {
	if (qlsSocket) {
		qlsSocket->abort();
		qlsSocket->deleteLater();
		qlsSocket = nullptr;
	}
	if (smMem) {
		smMem = nullptr;
	}
}

void OverlayWidget::connected() {
	qWarning() << "connected";

	OverlayMsg m;
	m.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	m.omh.uiType  = OVERLAY_MSGTYPE_PID;
	m.omh.iLength = sizeof(OverlayMsgPid);
#ifdef Q_OS_WIN
	m.omp.pid = GetCurrentProcessId();
#else
	m.omp.pid = static_cast< unsigned int >(getpid());
#endif
	qlsSocket->write(m.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgPid));

	om.omh.iLength = -1;

	init(size());
}

void OverlayWidget::disconnected() {
	qWarning() << "disconnected";

	QLocalSocket *qls = qobject_cast< QLocalSocket * >(sender());
	if (qls == qlsSocket) {
		detach();
	}
}

void OverlayWidget::error(QLocalSocket::LocalSocketError) {
	perror("error");
	disconnected();
}

void OverlayWidget::update() {
	++iFrameCount;

	float elapsed = static_cast< float >(qtWall.elapsed() - iLastFpsUpdate) / 1000.0f;

	if (elapsed > OVERLAY_FPS_INTERVAL) {
		struct OverlayMsg omFps;
		omFps.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		omFps.omh.uiType  = OVERLAY_MSGTYPE_FPS;
		omFps.omh.iLength = sizeof(struct OverlayMsgFps);
		omFps.omf.fps     = static_cast< float >(iFrameCount) / elapsed;

		if (qlsSocket && qlsSocket->state() == QLocalSocket::ConnectedState) {
			qlsSocket->write(reinterpret_cast< char * >(&omFps),
							 static_cast< int >(sizeof(OverlayMsgHeader)) + omFps.omh.iLength);
		}

		iFrameCount    = 0;
		iLastFpsUpdate = 0;
		qtWall.start();
	}

	QWidget::update();
}

void OverlayWidget::readyRead() {
	QLocalSocket *qls = qobject_cast< QLocalSocket * >(sender());
	if (qls != qlsSocket)
		return;

	while (true) {
		quint64 ready = static_cast< quint64 >(qlsSocket->bytesAvailable());

		if (om.omh.iLength == -1) {
			if (ready < sizeof(OverlayMsgHeader))
				break;
			else {
				qlsSocket->read(reinterpret_cast< char * >(om.headerbuffer), sizeof(OverlayMsgHeader));
				if ((om.omh.uiMagic != OVERLAY_MAGIC_NUMBER) || (om.omh.iLength < 0)
					|| ((size_t) om.omh.iLength > sizeof(OverlayMsgShmem))) {
					detach();
					return;
				}
				ready -= sizeof(OverlayMsgHeader);
			}
		}

		if (ready >= static_cast< unsigned int >(om.omh.iLength)) {
			qint64 length = qlsSocket->read(om.msgbuffer, om.omh.iLength);

			qWarning() << length << om.omh.uiType;

			if (length != om.omh.iLength) {
				detach();
				return;
			}

			switch (om.omh.uiType) {
				case OVERLAY_MSGTYPE_SHMEM: {
					OverlayMsgShmem *oms = &om.oms;
					QString key          = QString::fromUtf8(oms->a_cName, length);
					qWarning() << "SHMAT" << key;
					if (smMem)
						delete smMem;
					smMem = new SharedMemory2(this, static_cast< unsigned int >(width() * height() * 4), key);
					if (!smMem->data()) {
						qWarning() << "SHMEM FAIL";
						delete smMem;
						smMem = nullptr;
					} else {
						qWarning() << "SHMEM" << smMem->size();
					}
				} break;
				case OVERLAY_MSGTYPE_BLIT: {
					OverlayMsgBlit *omb = &om.omb;

					qWarning() << "BLIT" << omb->x << omb->y << omb->w << omb->h;

					if (!smMem)
						break;

					if (((omb->x + omb->w) > (unsigned int) img.width())
						|| ((omb->y + omb->h) > (unsigned int) img.height()))
						break;


					for (unsigned int y = 0; y < omb->h; ++y) {
						unsigned char *src = reinterpret_cast< unsigned char * >(smMem->data())
											 + 4 * (static_cast< unsigned int >(width()) * (y + omb->y) + omb->x);
						unsigned char *dst = img.scanLine(static_cast< int >(y + omb->y)) + omb->x * 4;
						memcpy(dst, src, omb->w * 4);
					}

					update();
				} break;
				case OVERLAY_MSGTYPE_ACTIVE: {
					OverlayMsgActive *oma = &om.oma;

					qWarning() << "ACTIVE" << oma->x << oma->y << oma->w << oma->h;

					qrActive = QRect(static_cast< int >(oma->x), static_cast< int >(oma->y), static_cast< int >(oma->w),
									 static_cast< int >(oma->h));
					update();
				}; break;
				default:
					break;
			}
			om.omh.iLength = -1;
		} else {
			break;
		}
	}
}

class TestWin : public QObject {
	Q_OBJECT

protected:
	QWidget *qw;

public:
	TestWin();
};

TestWin::TestWin() {
	QMainWindow *qmw = new QMainWindow();
	qmw->setObjectName(QLatin1String("main"));

	OverlayWidget *ow = new OverlayWidget();
	qmw->setCentralWidget(ow);

	qmw->show();
	qmw->resize(1280, 720);

	QMetaObject::connectSlotsByName(this);
}

int main(int argc, char **argv) {
	QApplication a(argc, argv);

	TestWin t;

	return a.exec();
}

#include "OverlayTest.moc"
