// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LOGTABS_H
#define MUMBLE_MUMBLE_LOGTABS_H

#include "CustomElements.h"

#include <QTabWidget>

typedef QList<int> LogTabList;

// Mapping a tab's hash to an index
typedef QHash<QString, int> HashMap;

/// Wrapping LogTextBrowser, to provide additional functionality for the tabs
class LogTab : public LogTextBrowser {
	Q_OBJECT
	// Access to LogTab members should only be possible for LogTabWidget
	friend class LogTabWidget;
public slots:
	void onHighlighted(const QUrl &);
private:
	bool m_isChannel;
	QString m_name;
	QString m_hash;

	LogTab(QString qsHash, bool isChannel, QWidget *p = NULL);
};

/// Wrapping QTabWidget, to access protected members
class LogTabWidget : public QTabWidget {
	Q_OBJECT
public:
	LogTabWidget(QWidget *parent = 0);
	int createTab(QString, QString, bool);
	void openTab(int);
	void setTabName(int, QString);
	int getTab(QString);
	int getGeneralTab();
	int getCurrentTab();
	LogTabList getChannelTabs();
	QString getHash(int);
	bool isChannelTab(int);
	// Adds a visible notification, indicating the related tab has been updated
	void markTabAsUpdated(int);
	// Adds a visible notification, indicating the related tab has been restricted
	void markTabAsRestricted(int);
	// Sets the maximum block count for all tabs
	void handleDocumentSetMaximumBlockCount(int);
	// Sets the default stylesheet for all tabs
	void handleDocumentSetDefaultStyleSheet(QString);
public slots:
	void onTabMoved(int, int);
	void onTabCloseRequested(int);
	void onTabBarCustomContextMenuRequested(const QPoint &);
signals:
	void anchorClick(const QUrl &);
	void customContextMenuRequest(const QPoint &);
private:
	QIcon qiUser;
	HashMap m_hashMap;
	int m_maxBlockCount;
	void updateHashMap();
};

#endif
