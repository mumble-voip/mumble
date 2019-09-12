// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Overlay.h"

#include "OverlayClient.h"
#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "GlobalShortcut.h"
#include "MainWindow.h"
#include "Message.h"
#include "OverlayText.h"
#include "RichTextEditor.h"
#include "ServerHandler.h"
#include "User.h"
#include "Utils.h"
#include "WebFetch.h"

#include <QtCore/QProcessEnvironment>
#include <QtCore/QtEndian>
#include <QtGui/QFocusEvent>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>
#include <QtNetwork/QLocalServer>
#include <QtWidgets/QMessageBox>

#ifdef Q_OS_WIN
# include <shellapi.h>
#endif

#ifdef Q_OS_MAC
# include <ApplicationServices/ApplicationServices.h>
# include <CoreFoundation/CoreFoundation.h>
#endif

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

QString OverlayAppInfo::applicationIdentifierForPath(const QString &path) {
#ifdef Q_OS_MAC
	QString qsIdentifier;
	CFDictionaryRef plist = NULL;
	CFDataRef data = NULL;

	QFile qfAppBundle(QString::fromLatin1("%1/Contents/Info.plist").arg(path));
	if (qfAppBundle.exists()) {
		qfAppBundle.open(QIODevice::ReadOnly);
		QByteArray qbaPlistData = qfAppBundle.readAll();

		data = CFDataCreateWithBytesNoCopy(NULL, reinterpret_cast<UInt8 *>(qbaPlistData.data()), qbaPlistData.count(), kCFAllocatorNull);
		plist = static_cast<CFDictionaryRef>(CFPropertyListCreateFromXMLData(NULL, data, kCFPropertyListImmutable, NULL));
		if (plist) {
			CFStringRef ident = static_cast<CFStringRef>(CFDictionaryGetValue(plist, CFSTR("CFBundleIdentifier")));
			if (ident) {
				char buf[4096];
				CFStringGetCString(ident, buf, 4096, kCFStringEncodingUTF8);
				qsIdentifier = QString::fromUtf8(buf);
			}
		}
	}

	if (data) {
		CFRelease(data);
	}
	if (plist) {
		CFRelease(plist);
	}

	return qsIdentifier;
#else
	return QDir::toNativeSeparators(path);
#endif
}

OverlayAppInfo OverlayAppInfo::applicationInfoForId(const QString &identifier) {
	QString qsAppName(identifier);
	QIcon qiAppIcon;
#if defined(Q_OS_MAC)
	CFStringRef bundleId = NULL;
	CFURLRef bundleUrl = NULL;
	CFBundleRef bundle = NULL;
	OSStatus err = noErr;
	char buf[4096];

	bundleId = CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast<const UniChar *>(identifier.unicode()), identifier.length());
	err = LSFindApplicationForInfo(kLSUnknownCreator, bundleId, NULL, NULL, &bundleUrl);
	if (err == noErr) {
		// Figure out the bundle name of the application.
		CFStringRef absBundlePath = CFURLCopyFileSystemPath(bundleUrl, kCFURLPOSIXPathStyle);
		CFStringGetCString(absBundlePath, buf, 4096, kCFStringEncodingUTF8);
		QString qsBundlePath = QString::fromUtf8(buf);
		CFRelease(absBundlePath);
		qsAppName = QFileInfo(qsBundlePath).bundleName();

		// Load the bundle's icon.
		bundle = CFBundleCreate(NULL, bundleUrl);
		if (bundle) {
			CFDictionaryRef info = CFBundleGetInfoDictionary(bundle);
			if (info) {
				CFStringRef iconFileName = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(info, CFSTR("CFBundleIconFile")));
				if (iconFileName) {
					CFStringGetCString(iconFileName, buf, 4096, kCFStringEncodingUTF8);
					QString qsIconPath = QString::fromLatin1("%1/Contents/Resources/%2")
					                     .arg(qsBundlePath, QString::fromUtf8(buf));
					if (! QFile::exists(qsIconPath)) {
						qsIconPath += QString::fromLatin1(".icns");
					}
					if (QFile::exists(qsIconPath)) {
						qiAppIcon = QIcon(qsIconPath);
					}
				}
			}
		}
	}

	if (bundleId) {
		CFRelease(bundleId);
	}
	if (bundleUrl) {
		CFRelease(bundleUrl);
	}
	if (bundle) {
		CFRelease(bundle);
	}

#elif defined(Q_OS_WIN)
	// qWinAppInst(), whose return value we used to pass
	// to ExtractIcon below, was removed in Qt 5.8.
	//
	// It was removed via
	// https://github.com/qt/qtbase/commit/64507c7165e42c2a5029353d8f97a0d841fa6b01
	//
	// In both Qt 4 and Qt 5, the qWinAppInst() implementation
	// simply calls GetModuleHandle(0).
	//
	// To sidestep the removal of the function, we simply
	// call through to GetModuleHandle() directly.
	HINSTANCE qWinAppInstValue = GetModuleHandle(NULL);
	HICON icon = ExtractIcon(qWinAppInstValue, identifier.toStdWString().c_str(), 0);
	if (icon) {
#if QT_VERSION >= 0x050000
		extern QPixmap qt_pixmapFromWinHICON(HICON icon);
		qiAppIcon = QIcon(qt_pixmapFromWinHICON(icon));
#else
		qiAppIcon = QIcon(QPixmap::fromWinHICON(icon));
#endif
		DestroyIcon(icon);
	}
#endif
	return OverlayAppInfo(qsAppName, qiAppIcon);
}

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
	{
		QString xdgRuntimePath = QProcessEnvironment::systemEnvironment().value(QLatin1String("XDG_RUNTIME_DIR"));
		QDir xdgRuntimeDir = QDir(xdgRuntimePath);

		if (! xdgRuntimePath.isNull() && xdgRuntimeDir.exists()) {
			pipepath = xdgRuntimeDir.absoluteFilePath(QLatin1String("MumbleOverlayPipe"));
		} else {
			pipepath = QDir::home().absoluteFilePath(QLatin1String(".MumbleOverlayPipe"));
		}
	}

	{
		QFile f(pipepath);
		if (f.exists()) {
			qWarning() << "Overlay: Removing old socket on" << pipepath;
			f.remove();
		}
	}
#endif

	if (! qlsServer->listen(pipepath)) {
		QMessageBox::warning(NULL, QLatin1String("Mumble"), tr("Failed to create communication with overlay at %2: %1. No overlay will be available.").arg(Qt::escape(qlsServer->errorString()), Qt::escape(pipepath)), QMessageBox::Ok, QMessageBox::NoButton);
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
	{
		// As we're the one closing the connection, we do not need to be
		// notified of disconnects. This is important because on disconnect we
		// also remove (and 'delete') the overlay client.
		disconnect(oc->qlsSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		disconnect(oc->qlsSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
		delete oc;
	}
}

void Overlay::newConnection() {
	while (true) {
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
				if (static_cast<quint64>(pid) != oc->uiPid)
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
		cp->qbaTexture = g.db->blob(cp->qbaTextureHash);

	if (! cp->qbaTexture.isEmpty()) {
		bool valid = true;

		if (cp->qbaTexture.length() < static_cast<int>(sizeof(unsigned int))) {
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
			qir.setAutoDetectImageFormat(false);

			QByteArray fmt;
			if (RichTextImage::isValidImage(cp->qbaTexture, fmt)) {
				qir.setFormat(fmt);
				qir.setDevice(&qb);
				if (! qir.canRead() || (qir.size().width() > 1024) || (qir.size().height() > 1024)) {
					valid = false;
				} else {
					cp->qbaTextureFormat = qir.format();
					QImage qi = qir.read();
					valid = ! qi.isNull();
				}
			} else {
				valid = false;
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
			qWarning() << "Overlay: Dead client detected. PID" << oc->uiPid << oc->qsExecutablePath;
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
		cu->qbaTexture=g.db->blob(cu->qbaTextureHash);
		if (cu->qbaTexture.isEmpty())
			qsQuery.insert(cu->uiSession);
		else
			verifyTexture(cu, false);
	}
}
