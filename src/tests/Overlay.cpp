#include <QtCore>
#include <QtNetwork>
#include <QtGui>

#include "../../overlay/overlay.h"

#include "Timer.h"

class OverlayWidget : public QWidget {
	Q_OBJECT
	
	protected:
		QImage img;
		OverlayMsgHeader omhHeader;
		QLocalSocket *qlsSocket;
		
		void resizeEvent(QResizeEvent *);
		void paintEvent(QPaintEvent *);
		
		void disconnect(QLocalSocket *);
	protected slots:
		void connected();
		void disconnected();
		void readyRead();
		void error(QLocalSocket::LocalSocketError);
	public:
		OverlayWidget(QWidget *p = NULL);
};

OverlayWidget::OverlayWidget(QWidget *p) : QWidget(p) {
	qlsSocket = NULL;
}

void OverlayWidget::resizeEvent(QResizeEvent *evt) {
	qWarning() << "resize";
	QWidget::resizeEvent(evt);

	if (! img.isNull())
		img = img.scaled(evt->size().width(), evt->size().height());
	
	if (qlsSocket) {
		qlsSocket->abort();
		qlsSocket->deleteLater();
	}
	
	qlsSocket = new QLocalSocket(this);
	connect(qlsSocket, SIGNAL(connected()), this, SLOT(connected()));
	connect(qlsSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(qlsSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(qlsSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
	qlsSocket->connectToServer(QLatin1String("MumbleOverlayPipe"));
}

void OverlayWidget::paintEvent(QPaintEvent *evt) {
	qWarning() << "paint";
	QPainter painter(this);
	painter.fillRect(0, 0, width(), height(), QColor(128,0,128));
	painter.drawImage(0, 0, img);
}

void OverlayWidget::connected() {
	qWarning() << "connected";
	
	omhHeader.iLength = -1;

	OverlayMsgs om;	
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType = OVERLAY_MSGTYPE_INIT;
	om.omh.iLength = sizeof(OverlayMsgInit);
	
	qlsSocket->write(om.buffer, sizeof(OverlayMsgHeader));
	
	om.omi.uiWidth = width();
	om.omi.uiHeight = height();

	qlsSocket->write(om.buffer, sizeof(OverlayMsgInit));
	
	img = QImage(width(), height(), QImage::Format_ARGB32);
}

void OverlayWidget::disconnected() {
	QLocalSocket *qls = qobject_cast<QLocalSocket *>(sender());
	if (qls == qlsSocket) {
		qWarning() << "disconnected";
	}
	qls->disconnect();
	qls->abort();
	qls->deleteLater();

}

void OverlayWidget::error(QLocalSocket::LocalSocketError) {
	disconnected();
}

void OverlayWidget::readyRead() {
	QLocalSocket *qls = qobject_cast<QLocalSocket *>(sender());
	if (qls != qlsSocket)
		return;

	while(1) {
		int ready = qlsSocket->bytesAvailable();
		
		qWarning() << ready << omhHeader.iLength;

		if (omhHeader.iLength == -1) {
			if (ready < sizeof(OverlayMsgHeader))
				break;
			else {
				qlsSocket->read(reinterpret_cast<char *>(&omhHeader), sizeof(OverlayMsgHeader));
				if ((omhHeader.uiMagic != OVERLAY_MAGIC_NUMBER) || (omhHeader.iLength < 0)) {
					qlsSocket->deleteLater();
					qlsSocket = NULL;
					return;
				}
				ready -= sizeof(OverlayMsgHeader);
			}
		}

		qWarning() << "2" << ready << omhHeader.iLength;
		
		if (ready >= omhHeader.iLength) {
			QByteArray qba = qlsSocket->read(omhHeader.iLength);
			unsigned char *data = reinterpret_cast<unsigned char *>(qba.data());
			int length = qba.length();

			qWarning() << length << omhHeader.uiType;

			if (length != omhHeader.iLength) {
					qlsSocket->deleteLater();
					qlsSocket = NULL;
					return;
			}
			switch(omhHeader.uiType) {
				case OVERLAY_MSGTYPE_BLIT:
					{
						OverlayMsgBlit *omb = reinterpret_cast<OverlayMsgBlit *>(data);
						length -= sizeof(OverlayMsgBlit);

						if ((length != 0) && (length != omb->w * omb->h * 4))
							break;
														
						if (((omb->x + omb->w) > img.width()) || 
							((omb->y + omb->h) > img.height()))
								break;
								
						qWarning() << "BLIT" << omb->x << omb->y << omb->w << omb->h;
						
						unsigned char *src = length ? data + sizeof(OverlayMsgBlit) : NULL;
						for(int y = 0; y < omb->h; ++y) {
							unsigned char *dst = img.scanLine(y + omb->y) + omb->x * 4;
							if (length == 0)
								memset(dst, 0, omb->w * 4);
							else
								memcpy(dst, src + y * omb->w * 4, omb->w * 4);
						}
						update();
					}
					break;
				default:
					break;
			}
			omhHeader.iLength = -1;
			ready -= length;
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
	QMainWindow *qmw=new QMainWindow();
	qmw->setObjectName(QLatin1String("main"));
	
	OverlayWidget *ow = new OverlayWidget();
	qmw->setCentralWidget(ow);
	
	qmw->show();
	
	
	
	QMetaObject::connectSlotsByName(this);
}

int main(int argc, char **argv) {
	QApplication a(argc, argv);
	
	TestWin t;

	return a.exec();
}

#include "Overlay.moc"
