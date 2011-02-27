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

OverlayAppInfo::OverlayAppInfo(QString name, QIcon icon) {
	qsDisplayName = name;
	qiIcon = icon;
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
			delete oc;
			return;
		}
	}
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
	} else {
		foreach(OverlayClient *oc, qlClients) {
			if (oc->uiPid) {
#if defined(Q_OS_WIN)
				HWND hwnd = GetForegroundWindow();
				DWORD pid = 0;
				GetWindowThreadProcessId(hwnd, &pid);
				if (pid != oc->uiPid)
					continue;
#elif defined(Q_OS_MAC)
				pid_t pid = 0;
				ProcessSerialNumber psn;
				GetFrontProcess(&psn);
				GetProcessPID(&psn, &pid);
				if (pid != oc->uiPid)
					continue;
#if 0
				// Fullscreen only.
				if (! CGDisplayIsCaptured(CGMainDisplayID()))
					continue;
#endif
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
		updateOverlay();
}

typedef QPair<QString, quint32> qpChanCol;

void Overlay::updateOverlay() {
	if (! g.uiSession)
		qsQueried.clear();

	if (qlClients.isEmpty())
		return;

	qsQuery.clear();

	foreach(OverlayClient *oc, qlClients) {
		if (! oc->update()) {
			qWarning() << "Overlay: Dead client detected";
			qlClients.removeAll(oc);
			oc->scheduleDelete();
			break;
		}
	}

	if (! qsQuery.isEmpty()) {
		MumbleProto::RequestBlob mprb;
		foreach(unsigned int session, qsQuery) {
			qsQueried.insert(session);
			mprb.add_session_texture(session);
		}
		g.sh->sendMessage(mprb);
	}
}

void Overlay::requestTexture(ClientUser *cu) {
	if (cu->qbaTexture.isEmpty() && ! qsQueried.contains(cu->uiSession)) {
		cu->qbaTexture=Database::blob(cu->qbaTextureHash);
		if (cu->qbaTexture.isEmpty())
			qsQuery.insert(cu->uiSession);
		else
			verifyTexture(cu, false);
	}
}

void Overlay::checkUpdates() {
#ifndef Q_OS_MAC
	return;
#endif

	QUrl url;
	url.setScheme(QLatin1String("http"));
	url.setHost(g.qsRegionalHost);
	url.setPath(QLatin1String("/overlay.php"));

	url.addQueryItem(QLatin1String("ver"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(MUMBLE_RELEASE))));
#if defined(Q_OS_WIN)
	url.addQueryItem(QLatin1String("os"), QLatin1String("Win32"));
#elif defined(Q_OS_MAC)
	url.addQueryItem(QLatin1String("os"), QLatin1String("MacOSX"));
#else
	url.addQueryItem(QLatin1String("os"), QLatin1String("Unix"));
#endif

#ifdef QT_NO_DEBUG
	QNetworkReply *rep = Network::get(url);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
#else
	g.mw->msgBox(tr("Skipping overlay update in debug mode."));
#endif
}

void Overlay::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
	QUrl url = rep->request().url();

	if (rep->error() == QNetworkReply::NoError) {
		const QString &path = url.path();
		if (path == QLatin1String("/overlay.php")) {
			qmOverlayHash.clear();
			QDomDocument doc;
			doc.setContent(rep->readAll());

			QDomElement root=doc.documentElement();
			QDomNode n = root.firstChild();
			while (!n.isNull()) {
				QDomElement e = n.toElement();
				if (!e.isNull()) {
					if (e.tagName() == QLatin1String("file")) {
						QString name = QFileInfo(e.attribute(QLatin1String("name"))).fileName();
						QString hash = e.attribute(QLatin1String("hash"));
						qmOverlayHash.insert(name, hash);
					}
				}
				n = n.nextSibling();
			}

#ifdef Q_OS_MAC
			QMap<QString, QString>::const_iterator i;
			for (i = qmOverlayHash.constBegin(); i != qmOverlayHash.constEnd(); ++i) {
				QString val = i.value();
				QString key = i.key();
				if (! val.isEmpty() && key.startsWith(QLatin1String("MumbleOverlay")) && key.endsWith(QLatin1String(".pkg"))) {
					bool update = true;
					QFile f(g.qdBasePath.absolutePath() + QLatin1String("/Overlay/MumbleOverlay.pkg"));
					if (f.exists() && f.open(QIODevice::ReadOnly)) {
						QString h = QLatin1String(sha1(f.readAll()).toHex());
						if (val == qmOverlayHash.value(QLatin1String("MumbleOverlay.pkg")))
							update = false;
					}
					if (update) {
						QUrl url;
						url.setScheme(QLatin1String("http"));
						url.setHost(rep->url().host());
						url.setPath(QString::fromLatin1("overlay/%1").arg(key));

						QNetworkReply *r = Network::get(url);
						connect(r, SIGNAL(finished()), this, SLOT(finished()));
					}
				}
			}
#endif
		} else {
			QString fname = QFileInfo(path).fileName();
			if (qmOverlayHash.contains(fname)) {
				QByteArray qba = rep->readAll();
				if (qmOverlayHash.value(fname) == QLatin1String(sha1(qba).toHex())) {
#ifdef Q_OS_MAC
					if (fname.startsWith(QLatin1String("MumbleOverlay")) && fname.endsWith(QLatin1String(".pkg"))) {
						QFile f;
						f.setFileName(g.qdBasePath.absolutePath() + QLatin1String("/Overlay/MumbleOverlay.pkg"));
						if (f.open(QIODevice::WriteOnly)) {
							f.write(qba);
							f.close();
							g.mw->msgBox(tr("Downloaded new or updated overlay support file to %1.").arg(f.fileName()));
						}
					}
#endif
				}
			}
		}
	} else {
		if (url.host() == g.qsRegionalHost) {
			url.setHost(QLatin1String("mumble.info"));
			QNetworkReply *nrep = Network::get(url);
			connect(nrep, SIGNAL(finished()), this, SLOT(finished()));
		}
	}

	rep->deleteLater();
}
