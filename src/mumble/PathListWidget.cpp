// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PathListWidget.h"

#include "Overlay.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMimeData>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>

PathListWidget::PathListWidget(QWidget *parent) : QListWidget(parent), pathType(FILE_EXE) {
	setAcceptDrops(true);
}

void PathListWidget::setPathType(PathType type) {
	pathType = type;
}

void PathListWidget::addFilePath(const QString &path) {
	QString qsAppIdentifier = OverlayAppInfo::applicationIdentifierForPath(path);
	QStringList qslIdentifiers;
	for (int i = 0; i < count(); i++) {
		qslIdentifiers << item(i)->data(Qt::UserRole).toString();
	}
	if (!qslIdentifiers.contains(qsAppIdentifier)) {
		OverlayAppInfo oai               = OverlayAppInfo::applicationInfoForId(qsAppIdentifier);
		QListWidgetItem *qlwiApplication = new QListWidgetItem(oai.qiIcon, oai.qsDisplayName, this);
		qlwiApplication->setData(Qt::UserRole, QVariant(qsAppIdentifier));
		setCurrentItem(qlwiApplication);
	}
}

void PathListWidget::addFolderPath(const QString &path) {
	QString dir = QDir::toNativeSeparators(path);
	QStringList qslIdentifiers;
	for (int i = 0; i < count(); i++) {
		qslIdentifiers << item(i)->data(Qt::UserRole).toString();
	}
	if (!dir.isEmpty() && !qslIdentifiers.contains(dir)) {
		QListWidgetItem *qlwiPath = new QListWidgetItem(QIcon(), QDir(dir).path(), this);
		qlwiPath->setData(Qt::UserRole, QVariant(dir));
		setCurrentItem(qlwiPath);
	}
}

void PathListWidget::checkAcceptDragEvent(QDropEvent *event, bool store) {
	if (event->mimeData()->hasUrls()) {
		foreach (QUrl url, event->mimeData()->urls()) {
			if (url.isLocalFile()) {
				QFileInfo info(url.toLocalFile());
				switch (pathType) {
					case FILE_EXE:
						if (info.isFile() && info.isExecutable()) {
							if (store) {
								addFilePath(info.filePath());
							}
							event->setDropAction(Qt::LinkAction);
							event->accept();
						}
						break;
					case FOLDER:
						if (info.isDir()) {
							if (store) {
								addFolderPath(url.toLocalFile());
							}
							event->setDropAction(Qt::LinkAction);
							event->accept();
						}
						break;
				}
			}
		}
	}
}

void PathListWidget::dragEnterEvent(QDragEnterEvent *event) {
	checkAcceptDragEvent(event, false);
}

void PathListWidget::dragMoveEvent(QDragMoveEvent *event) {
	checkAcceptDragEvent(event, false);
}

void PathListWidget::dropEvent(QDropEvent *event) {
	checkAcceptDragEvent(event, true);
}
