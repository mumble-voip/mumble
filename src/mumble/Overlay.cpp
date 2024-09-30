// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Overlay.h"

#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"
#include "OverlayClient.h"
#include "OverlayText.h"
#include "RichTextEditor.h"
#include "ServerHandler.h"
#include "User.h"
#include "Utils.h"
#include "WebFetch.h"
#include "Global.h"
#include "GlobalShortcut.h"

#include <QtCore/QProcessEnvironment>
#include <QtCore/QtEndian>
#include <QtGui/QFocusEvent>
#include <QtGui/QImageReader>
#include <QtGui/QImageWriter>
#include <QtNetwork/QLocalServer>
#include <QtWidgets/QMessageBox>

#ifdef Q_OS_WIN
#	include <shellapi.h>
#endif

#ifdef Q_OS_MAC
#	include <ApplicationServices/ApplicationServices.h>
#	include <CoreFoundation/CoreFoundation.h>
#endif

QString OverlayAppInfo::applicationIdentifierForPath(const QString &path) {
#ifdef Q_OS_MAC
	QString qsIdentifier;

	QFile appBundle(QString::fromLatin1("%1/Contents/Info.plist").arg(path));
	if (!appBundle.exists()) {
		return {};
	}

	appBundle.open(QIODevice::ReadOnly);

	const QByteArray byteArray = appBundle.readAll();
	CFDataRef dataCF = CFDataCreateWithBytesNoCopy(nullptr, reinterpret_cast< const UInt8 * >(byteArray.data()),
												   static_cast< CFIndex >(byteArray.size()), kCFAllocatorNull);
	if (!dataCF) {
		return {};
	}

	auto plist = static_cast< CFDictionaryRef >(
		CFPropertyListCreateWithData(nullptr, dataCF, kCFPropertyListImmutable, nullptr, nullptr));
	CFRelease(dataCF);
	if (!plist) {
		return {};
	}

	auto identifierCF = static_cast< CFStringRef >(CFDictionaryGetValue(plist, CFSTR("CFBundleIdentifier")));
	auto identifier   = QString::fromCFString(identifierCF);
	CFRelease(plist);

	return identifier;
#else
	return QDir::toNativeSeparators(path);
#endif
}

OverlayAppInfo OverlayAppInfo::applicationInfoForId(const QString &identifier) {
	QString qsAppName(identifier);
	QIcon qiAppIcon;
#if defined(Q_OS_MAC)
	CFStringRef id  = identifier.toCFString();
	CFArrayRef urls = LSCopyApplicationURLsForBundleIdentifier(id, nullptr);
	CFRelease(id);

	if (!urls) {
		return OverlayAppInfo(qsAppName, qiAppIcon);
	}

	auto url = static_cast< CFURLRef >(CFArrayGetValueAtIndex(urls, 0));
	CFRetain(url);
	CFRelease(urls);

	// Figure out the bundle name of the application.
	CFStringRef bundlePathCF = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
	const auto bundlePath    = QString::fromCFString(bundlePathCF);
	CFRelease(bundlePathCF);

	qsAppName = QFileInfo(bundlePath).bundleName();

	// Load the bundle's icon.
	CFBundleRef bundle = CFBundleCreate(nullptr, url);
	CFRelease(url);

	if (!bundle) {
		return OverlayAppInfo(qsAppName, qiAppIcon);
	}

	CFDictionaryRef info = CFBundleGetInfoDictionary(bundle);
	CFRelease(bundle);

	if (!info) {
		return OverlayAppInfo(qsAppName, qiAppIcon);
	}

	auto iconFileName = static_cast< CFStringRef >(CFDictionaryGetValue(info, CFSTR("CFBundleIconFile")));
	if (!iconFileName) {
		return OverlayAppInfo(qsAppName, qiAppIcon);
	}

	auto iconPath =
		QString::fromLatin1("%1/Contents/Resources/%2").arg(bundlePath, QString::fromCFString(iconFileName));

	if (!QFile::exists(iconPath)) {
		iconPath += QStringLiteral(".icns");
	}

	if (QFile::exists(iconPath)) {
		qiAppIcon = QIcon(iconPath);
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
	HINSTANCE qWinAppInstValue = GetModuleHandle(nullptr);
	HICON icon                 = ExtractIcon(qWinAppInstValue, identifier.toStdWString().c_str(), 0);
	if (icon) {
		extern QPixmap qt_pixmapFromWinHICON(HICON icon);
		qiAppIcon = QIcon(qt_pixmapFromWinHICON(icon));
		DestroyIcon(icon);
	}
#endif
	return OverlayAppInfo(qsAppName, qiAppIcon);
}

OverlayAppInfo::OverlayAppInfo(QString name, QIcon icon) {
	qsDisplayName = name;
	qiIcon        = icon;
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
	foreach (const QGraphicsItem *item, childItems())
		if (item->isVisible())
			qr |= item->boundingRect().translated(item->pos());

	return qr;
}

Overlay::Overlay() : QObject() {
	d = nullptr;

	m_initialized.store(false);

	qlsServer = nullptr;

	QMetaObject::connectSlotsByName(this);
}

Overlay::~Overlay() {
	setActive(false);
	if (d) {
		delete d;
	}

	// Need to be deleted first, since destructor references lingering QLocalSockets
	foreach (OverlayClient *oc, qlClients) {
		// As we're the one closing the connection, we do not need to be
		// notified of disconnects. This is important because on disconnect we
		// also remove (and 'delete') the overlay client.
		disconnect(oc->qlsSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		disconnect(oc->qlsSocket, SIGNAL(error(QLocalSocket::LocalSocketError)), this,
				   SLOT(error(QLocalSocket::LocalSocketError)));
		delete oc;
	}
}

void Overlay::initialize() {
	if (!m_initialized.load()) {
		platformInit();
		forceSettings();

		createPipe();

		m_initialized.store(true);
	}
}

void Overlay::setActive(bool act) {
	if (!act && !m_initialized.load()) {
		// Disabling when the Overlay hasn't been initialized yet, doesn't make much sense
		return;
	}

	// Make sure the Overlay is initialized
	initialize();

	setActiveInternal(act);
}

void Overlay::createPipe() {
	qlsServer = new QLocalServer(this);
	// Allow anyone to access the pipe in order to communicate with the overlay
	qlsServer->setSocketOptions(QLocalServer::WorldAccessOption);

	QString pipepath;
#ifdef Q_OS_WIN
	pipepath = QLatin1String("MumbleOverlayPipe");
#else
	{
		QString xdgRuntimePath = QProcessEnvironment::systemEnvironment().value(QLatin1String("XDG_RUNTIME_DIR"));
		QDir xdgRuntimeDir     = QDir(xdgRuntimePath);

		if (!xdgRuntimePath.isNull() && xdgRuntimeDir.exists()) {
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

	if (!qlsServer->listen(pipepath)) {
		QMessageBox::warning(nullptr, QLatin1String("Mumble"),
							 tr("Failed to create communication with overlay at %2: %1. No overlay will be available.")
								 .arg(qlsServer->errorString().toHtmlEscaped(), pipepath.toHtmlEscaped()),
							 QMessageBox::Ok, QMessageBox::NoButton);
	} else {
		qWarning() << "Overlay: Listening on" << qlsServer->fullServerName();
		connect(qlsServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}
}

void Overlay::newConnection() {
	while (qlsServer && qlsServer->hasPendingConnections()) {
		QLocalSocket *qls = qlsServer->nextPendingConnection();

		OverlayClient *oc = new OverlayClient(qls, this);
		qlClients << oc;

		connect(qls, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(qls, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
	}
}

void Overlay::disconnected() {
	QLocalSocket *qls = qobject_cast< QLocalSocket * >(sender());
	foreach (OverlayClient *oc, qlClients) {
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
	return !qlClients.isEmpty();
}

void Overlay::toggleShow() {
	if (Global::get().ocIntercept) {
		Global::get().ocIntercept->hideGui();
	} else {
		foreach (OverlayClient *oc, qlClients) {
			if (oc->uiPid) {
#if defined(Q_OS_WIN)
				HWND hwnd = GetForegroundWindow();
				DWORD pid = 0;
				GetWindowThreadProcessId(hwnd, &pid);
				if (pid != oc->uiPid)
					continue;
#elif defined(Q_OS_MAC)
				if (static_cast< quint64 >(getForegroundProcessId()) != oc->uiPid)
					continue;
#	if 0
				// Fullscreen only.
				if (! CGDisplayIsCaptured(CGMainDisplayID()))
					continue;
#	endif
#endif
				oc->showGui();
				return;
			}
		}
	}
}

void Overlay::forceSettings() {
	foreach (OverlayClient *oc, qlClients) { oc->reset(); }

	updateOverlay();
}

void Overlay::verifyTexture(ClientUser *cp, bool allowupdate) {
	qsQueried.remove(cp->uiSession);

	ClientUser *self = ClientUser::get(Global::get().uiSession);
	allowupdate      = allowupdate && self && self->cChannel->isLinked(cp->cChannel);

	if (allowupdate && !cp->qbaTextureHash.isEmpty() && cp->qbaTexture.isEmpty())
		cp->qbaTexture = Global::get().db->blob(cp->qbaTextureHash);

	if (!cp->qbaTexture.isEmpty()) {
		bool valid = true;

		if (cp->qbaTexture.length() < static_cast< int >(sizeof(unsigned int))) {
			valid = false;
		} else if (qFromBigEndian< unsigned int >(reinterpret_cast< const unsigned char * >(cp->qbaTexture.constData()))
				   == 600 * 60 * 4) {
			QByteArray qba = qUncompress(cp->qbaTexture);
			if (qba.length() != 600 * 60 * 4) {
				valid = false;
			} else {
				int width               = 0;
				int height              = 0;
				const unsigned int *ptr = reinterpret_cast< const unsigned int * >(qba.constData());

				// If we have an alpha only part on the right side of the image ignore it
				for (int y = 0; y < 60; ++y) {
					for (int x = 0; x < 600; ++x) {
						if (ptr[y * 600 + x] & 0xff000000) {
							if (x > width)
								width = x;
							if (y > height)
								height = y;
						}
					}
				}

				// Full size image? More likely image without alpha; fix it.
				if ((width == 599) && (height == 59)) {
					width  = 0;
					height = 0;
					for (int y = 0; y < 60; ++y) {
						for (int x = 0; x < 600; ++x) {
							if (ptr[y * 600 + x] & 0x00ffffff) {
								if (x > width)
									width = x;
								if (y > height)
									height = y;
							}
						}
					}
				}

				if (!width || !height) {
					valid = false;
				} else {
					QImage img = QImage(width + 1, height + 1, QImage::Format_ARGB32);
					{
						QImage srcimg(reinterpret_cast< const uchar * >(qba.constData()), 600, 60,
									  QImage::Format_ARGB32);

						QPainter imgp(&img);
						img.fill(0);
						imgp.setRenderHint(QPainter::Antialiasing);
						imgp.setRenderHint(QPainter::TextAntialiasing);
						imgp.setBackground(QColor(0, 0, 0, 0));
						imgp.setCompositionMode(QPainter::CompositionMode_Source);
						imgp.drawImage(0, 0, srcimg);
					}
					cp->qbaTexture = QByteArray();

					QBuffer qb(&cp->qbaTexture);
					qb.open(QIODevice::WriteOnly);
					QImageWriter qiw(&qb, "png");
					qiw.write(img);

					cp->qbaTextureFormat = QString::fromLatin1("png").toUtf8();
				}
			}
		} else {
			QBuffer qb(&cp->qbaTexture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir;
			qir.setAutoDetectImageFormat(false);

			QByteArray fmt;
			if (RichTextImage::isValidImage(cp->qbaTexture, fmt)) {
				qir.setFormat(fmt);
				qir.setDevice(&qb);
				if (!qir.canRead() || (qir.size().width() > 1024) || (qir.size().height() > 1024)) {
					valid = false;
				} else {
					cp->qbaTextureFormat = qir.format();
					QImage qi            = qir.read();
					valid                = !qi.isNull();
				}
			} else {
				valid = false;
			}
		}
		if (!valid) {
			cp->qbaTexture     = QByteArray();
			cp->qbaTextureHash = QByteArray();
		}
	}

	if (allowupdate)
		updateOverlay();
}

typedef QPair< QString, quint32 > qpChanCol;

void Overlay::updateOverlay() {
	if (!Global::get().uiSession)
		qsQueried.clear();

	if (qlClients.isEmpty())
		return;

	qsQuery.clear();

	foreach (OverlayClient *oc, qlClients) {
		if (!oc->update()) {
			qWarning() << "Overlay: Dead client detected. PID" << oc->uiPid << oc->qsExecutablePath;
			qlClients.removeAll(oc);
			oc->scheduleDelete();
			break;
		}
	}

	if (!qsQuery.isEmpty()) {
		MumbleProto::RequestBlob mprb;
		foreach (unsigned int session, qsQuery) {
			qsQueried.insert(session);
			mprb.add_session_texture(session);
		}
		Global::get().sh->sendMessage(mprb);
	}
}

void Overlay::requestTexture(ClientUser *cu) {
	if (cu->qbaTexture.isEmpty() && !qsQueried.contains(cu->uiSession)) {
		cu->qbaTexture = Global::get().db->blob(cu->qbaTextureHash);
		if (cu->qbaTexture.isEmpty())
			qsQuery.insert(cu->uiSession);
		else
			verifyTexture(cu, false);
	}
}
