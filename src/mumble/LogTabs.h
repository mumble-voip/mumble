// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LOGTABS_H
#define MUMBLE_MUMBLE_LOGTABS_H

#include "CustomElements.h"

#include <QTabWidget>

class ClientUser;

/// Wrapping LogTextBrowser, to provide additional functionality for the tabs
class LogTab : public LogTextBrowser {
	Q_OBJECT
	// Access to LogTab members should only be possible for LogTabWidget
	friend class LogTabWidget;
public slots:
	void onHighlighted(const QUrl &);
private:
	QString m_name;
	QString m_hash;
	// Initialize a LogTab from a user
	LogTab(ClientUser *, QWidget *p = NULL);
	void addToTabWidget(QTabWidget *);
	// Updates the names of the related user
	void updateUser(ClientUser *);
};

/// Wrapping QTabWidget, to access protected members
class LogTabWidget : public QTabWidget {
	Q_OBJECT
public:
	LogTabWidget(QWidget *parent = 0);
	// Set the visibility of the tabbar
	void activateTabs(bool);
	int getChannelTab();
	void openTab(ClientUser *);
	int findTab(ClientUser *);
	int searchTab(ClientUser *);
	QString getHash(int);
	void updateTab(ClientUser *);
	// Adds a visible notification, indicating the related tab has been updated
	void markTabAsUpdated(int);
	// Adds a visible notification, indicating the related tab has been restricted
	void markTabAsRestricted(int);
	// Sets the maximum block count for all tabs
	void handleDocumentSetMaximumBlockCount(int);
	// Sets the default stylesheet for all tabs
	void handleDocumentSetDefaultStyleSheet(QString);
public slots:
	void onCurrentChanged(int);
	void onTabMoved(int, int);
	void onTabCloseRequested(int);
	void onTabBarCustomContextMenuRequested(const QPoint &);
signals:
	void anchorClick(const QUrl &);
	void customContextMenuRequest(const QPoint &);
	void highlighted(const QUrl &);
private:
	// Mapping a users hash to a tab index
	QHash<QString, int> *m_hashMap;
	int m_maxBlockCount;
	void updateHashMap();
	int createTab(ClientUser *);
};

#endif
