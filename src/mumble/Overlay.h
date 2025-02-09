// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAY_H_
#define MUMBLE_MUMBLE_OVERLAY_H_

#include <QtCore/QUrl>
#include <QtCore/QtGlobal>
#include <QtNetwork/QLocalSocket>
#include <QtWidgets/QGraphicsItem>

#include "../../overlay/overlay.h"
#include "ConfigDialog.h"
#include "OverlayText.h"

#include <atomic>

#include "ui_OverlayEditor.h"

#ifdef Q_OS_MAC
// The function is defined in Overlay_macx.mm because it uses Apple's Objective-C API.
pid_t getForegroundProcessId();
#endif

class ClientUser;
class Overlay;
class QLocalServer;
class OverlayClient;
class OverlayConfig;

struct OverlayAppInfo {
	static QString applicationIdentifierForPath(const QString &path);
	static OverlayAppInfo applicationInfoForId(const QString &identifier);

	QString qsDisplayName;
	QIcon qiIcon;

private:
	OverlayAppInfo(QString name, QIcon icon = QIcon());
};

class OverlayGroup : public QGraphicsItem {
private:
	Q_DISABLE_COPY(OverlayGroup)

public:
	enum { Type = UserType + 5 };

public:
	OverlayGroup();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
	int type() const Q_DECL_OVERRIDE;

	template< typename T > QRectF boundingRect() const;
};

class OverlayMouse : public QGraphicsPixmapItem {
private:
	Q_DISABLE_COPY(OverlayMouse)
public:
	bool contains(const QPointF &) const Q_DECL_OVERRIDE;
	bool collidesWithPath(const QPainterPath &, Qt::ItemSelectionMode = Qt::IntersectsItemShape) const Q_DECL_OVERRIDE;
	OverlayMouse(QGraphicsItem * = nullptr);
};

class OverlayPrivate : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(OverlayPrivate)
public:
	OverlayPrivate(QObject *p) : QObject(p){};
};

class Overlay : public QObject {
	friend class OverlayConfig;
	friend class OverlayClient;
	friend class OverlayUser;

private:
	Q_OBJECT
	Q_DISABLE_COPY(Overlay)
protected:
	OverlayPrivate *d;

	QSet< unsigned int > qsQueried;
	QSet< unsigned int > qsQuery;
	/// A flag indicating if the platformInit has been called already
	std::atomic< bool > m_initialized;

	/// Initializes the overlay, if it hasn't been initialized yet
	void initialize();

	// These 2 functions (among others) are implemented by the system-specific backend
	void platformInit();
	void setActiveInternal(bool act);

	void createPipe();

	QMap< QString, QString > qmOverlayHash;
	QLocalServer *qlsServer;
	QList< OverlayClient * > qlClients;
protected slots:
	void disconnected();
	void error(QLocalSocket::LocalSocketError);
	void newConnection();

public:
	Overlay();
	~Overlay() Q_DECL_OVERRIDE;
	bool isActive() const;
	void verifyTexture(ClientUser *cp, bool allowupdate = true);
	void requestTexture(ClientUser *);

public slots:
	void updateOverlay();
	void setActive(bool act);
	void toggleShow();
	void forceSettings();
};

#endif
