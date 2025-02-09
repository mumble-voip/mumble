// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PATHLISTWIDGET_H_
#define MUMBLE_MUMBLE_PATHLISTWIDGET_H_

#include <QListWidget>

class QGraphicsSceneDragDropEvent;
class QDropEvent;

/**
 * ListWidget that adds functionality for being able to drop files or folders to add them to the list.
 *
 * It makes use of OverlayAppInfo to display file/app information (e.g. the appropriate icon).
 */
class PathListWidget : public QListWidget {
public:
	enum PathType { FILE_EXE, FOLDER };

	PathListWidget(QWidget *parent = 0);
	void setPathType(PathType type);
	virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
	virtual void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
	virtual void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private:
	Q_OBJECT
	Q_DISABLE_COPY(PathListWidget)

	PathType pathType;

	void addFilePath(const QString &path);
	void addFolderPath(const QString &path);
	void checkAcceptDragEvent(QDropEvent *event, bool store);
};

#endif
