// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LogTabs.h"

#include "ClientUser.h"
#include "Log.h"

#include <QtCore/QSignalMapper>
#include <QMenu>
#include <QToolTip>

LogTab::LogTab(ClientUser *user, QWidget *p) : LogTextBrowser(p) {
	m_hash = user->qsHash;
	m_name = user->qsName;

	setFrameStyle(QFrame::NoFrame);
	setOpenLinks(false);
	setContextMenuPolicy(Qt::CustomContextMenu);

	LogDocument *ld = new LogDocument(this);
	ld->setDefaultStyleSheet(qApp->styleSheet());
	setDocument(ld);

	setWhatsThis(tr("This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu."));

	connect(this, SIGNAL(highlighted(QUrl)), this, SLOT(onHighlighted(QUrl)));
}

void LogTab::addToTabWidget(QTabWidget *tabWidget) {
	tabWidget->addTab(this, m_name);
}

void LogTab::updateUser(ClientUser *user) {
	m_name = user->qsName;
}

void LogTab::onHighlighted(const QUrl &url) {
	if (url.scheme() == QString::fromLatin1("clientid") || url.scheme() == QString::fromLatin1("channelid")) {
		return;
	}

	if (url.isValid()) {
		QToolTip::showText(QCursor::pos(), url.toString(), this, QRect());
	} else {
		QToolTip::hideText();
	}
}

LogTabWidget::LogTabWidget(QWidget *parent) : QTabWidget(parent) {
	setTabPosition(QTabWidget::South);
	setMovable(true);
	setUsesScrollButtons(true);
	setElideMode(Qt::ElideRight);
	setStyleSheet(QString::fromUtf8("QTabBar::tab{min-width: 1px}"));
	setTabText(0, tr("Not connected"));
	setTabToolTip(0, tr("Not connected"));

	tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

	ClientUser *channel = new ClientUser();
	channel->qsHash = QString::fromUtf8("channel");
	channel->qsName = tr("Channel");
	m_hashMap = new QHash<QString, int>();
	createTab(channel);

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
	connect(tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(onTabMoved(int, int)));
	connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTabBarCustomContextMenuRequested(QPoint)));
}

void LogTabWidget::activateTabs(bool show) {
	tabBar()->setVisible(show);
}

int LogTabWidget::findTab(ClientUser *user) {
	QHash<QString, int>::const_iterator iter = m_hashMap->find(user->qsHash);
	return iter != m_hashMap->constEnd() ? iter.value() : createTab(user);
}

int LogTabWidget::searchTab(ClientUser *user) {
	QHash<QString, int>::const_iterator iter = m_hashMap->find(user->qsHash);
	return iter != m_hashMap->constEnd() ? iter.value() : -1;
}

int LogTabWidget::createTab(ClientUser *user) {
	m_hashMap->insert(user->qsHash, count());

	LogTab *lt = new LogTab(user, this);
	lt->addToTabWidget(this);
	lt->document()->setMaximumBlockCount(m_maxBlockCount);

	setTabToolTip(count() -1 , lt->m_name);

	connect(lt, SIGNAL(anchorClicked(const QUrl &)), this, SIGNAL(anchorClick(const QUrl &)));
	connect(lt, SIGNAL(customContextMenuRequested(const QPoint &)), this, SIGNAL(customContextMenuRequest(const QPoint &)));

	return count() - 1;
}

void LogTabWidget::openTab(ClientUser *user) {
	setCurrentIndex(findTab(user));
}

int LogTabWidget::getChannelTab() {
	return m_hashMap->find(QString::fromUtf8("channel")).value();
}

QString LogTabWidget::getHash(int index) {
	return dynamic_cast<LogTab *>(widget(index))->m_hash;
}

void LogTabWidget::updateHashMap() {
	m_hashMap->clear();

	for (int i = 0; i < count(); i++) {
		m_hashMap->insert(dynamic_cast<LogTab *>(widget(i))->m_hash, i);
	}
}

void LogTabWidget::markTabAsUpdated(int index) {
	if (currentIndex() != index) {
		tabBar()->setTabTextColor(index, Qt::blue);
	}
}

void LogTabWidget::markTabAsRestricted(int index) {
	tabBar()->setTabTextColor(index, Qt::gray);
}

void LogTabWidget::updateTab(ClientUser *user) {
	const int index = searchTab(user);
	if (index == -1) {
		return;
	}

	LogTab *lt = dynamic_cast<LogTab *>(widget(index));
	lt->updateUser(user);
	setTabText(index, lt->m_name);
	setTabToolTip(index, lt->m_name);
}

void LogTabWidget::handleDocumentSetMaximumBlockCount(int maxLogBlocks) {
	for (int i = 0; i < count(); i++) {
		dynamic_cast<LogTab *>(widget(i))->document()->setMaximumBlockCount(maxLogBlocks);
	}

	m_maxBlockCount = maxLogBlocks;
}

void LogTabWidget::handleDocumentSetDefaultStyleSheet(QString styleSheet) {
	for (int i = 0; i < count(); i++) {
		dynamic_cast<LogTab *>(widget(i))->document()->setDefaultStyleSheet(styleSheet);
	}
}

void LogTabWidget::onCurrentChanged(int newIndex) {
	tabBar()->setTabTextColor(newIndex, Qt::black);
}

void LogTabWidget::onTabMoved(int, int) {
	updateHashMap();
}

void LogTabWidget::onTabCloseRequested(int index) {
	QString hashKey = dynamic_cast<LogTab *>(widget(index))->m_hash;
	m_hashMap->remove(hashKey);
	widget(index)->deleteLater();
	removeTab(index);
	updateHashMap();
}

void LogTabWidget::onTabBarCustomContextMenuRequested(const QPoint& point) {
	QSignalMapper *mapper = new QSignalMapper(this);
	for (int i = 0; i < count(); i++) {
		if (i == getChannelTab()) {
			continue;
		}

		if (tabBar()->tabRect(i).contains(point)) {
			QMenu *menu = new QMenu();
			QAction *action = menu->addAction(tr("Close"));
			mapper->setMapping(action, i);
			connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
			connect(mapper, SIGNAL(mapped(int)), this, SLOT(onTabCloseRequested(int)));
			connect(mapper, SIGNAL(mapped(int)), this, SIGNAL(currentChanged(int)));
			menu->exec(QCursor::pos());
			break;
		}
	}
}
