// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYUSERGROUP_H_
#define MUMBLE_MUMBLE_OVERLAYUSERGROUP_H_

#include "Overlay.h"

class OverlayUser;

class OverlayUserGroup : public QObject, public OverlayGroup {
private:
	Q_OBJECT
	Q_DISABLE_COPY(OverlayUserGroup)

public:
	enum { Type = UserType + 3 };

protected:
	OverlaySettings *os;

	QMap< QObject *, OverlayUser * > qmUsers;
	QList< OverlayUser * > qlExampleUsers;

	QGraphicsEllipseItem *qgeiHandle;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *e) Q_DECL_OVERRIDE;
	void wheelEvent(QGraphicsSceneWheelEvent *e) Q_DECL_OVERRIDE;
	bool sceneEventFilter(QGraphicsItem *, QEvent *e) Q_DECL_OVERRIDE;
protected slots:
	void userDestroyed(QObject *);
	void moveUsers();

public:
	bool bShowExamples;

	OverlayUserGroup(OverlaySettings *);
	~OverlayUserGroup() Q_DECL_OVERRIDE;

	int type() const Q_DECL_OVERRIDE;
public slots:
	void reset();
	void updateUsers();
	void updateLayout();
};

#endif
