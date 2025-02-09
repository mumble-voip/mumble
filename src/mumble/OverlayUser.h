// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYUSER_H_
#define MUMBLE_MUMBLE_OVERLAYUSER_H_

#include <QtCore/QtGlobal>

#include "Overlay.h"

class OverlayUser : public OverlayGroup {
private:
	Q_DISABLE_COPY(OverlayUser)
public:
	enum { Type = UserType + 1 };

protected:
	QGraphicsPixmapItem *qgpiMuted, *qgpiDeafened;
	QGraphicsPixmapItem *qgpiAvatar;
	QGraphicsPixmapItem *qgpiName[4];
	QGraphicsPixmapItem *qgpiChannel;
	QGraphicsPathItem *qgpiBox;

	OverlaySettings *os;

	unsigned int uiSize;
	ClientUser *cuUser;
	Settings::TalkState tsColor;

	QString qsName;
	QString qsChannelName;
	QByteArray qbaAvatar;

	void setup();

public:
	OverlayUser(ClientUser *cu, unsigned int uiSize, OverlaySettings *osptr);
	OverlayUser(Settings::TalkState ts, unsigned int uiSize, OverlaySettings *osptr);
	void updateUser();
	void updateLayout();

	int type() const Q_DECL_OVERRIDE;
	static QRectF scaledRect(const QRectF &qr, qreal scale);
	static QPointF alignedPosition(const QRectF &box, const QRectF &item, Qt::Alignment a);
};

#endif
