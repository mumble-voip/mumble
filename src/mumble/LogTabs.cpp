// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LogTabs.h"

#include "Log.h"

#include <QtCore/QSignalMapper>
#include <QMenu>
#include <QToolTip>

LogTab::LogTab(QString qsHash, bool isChannel, QWidget *p)
    : LogTextBrowser(p)
    , m_isChannel(isChannel)
    , m_hash(qsHash)
{
	setOpenLinks(false);
	setContextMenuPolicy(Qt::CustomContextMenu);

	LogDocument *ld = new LogDocument(this);
	ld->setDefaultStyleSheet(qApp->styleSheet());
	setDocument(ld);

	connect(this, SIGNAL(highlighted(QUrl)), this, SLOT(onHighlighted(QUrl)));
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
	setMovable(true);
	setDocumentMode(true);
	setUsesScrollButtons(true);
	setElideMode(Qt::ElideRight);
	setTabPosition(QTabWidget::South);
	setStyleSheet(QString::fromUtf8("QTabBar::tab{min-width: 1px}"));
	setWhatsThis(tr("This shows all recent activity. Connecting to servers, errors and information messages all show up here.<br />To configure exactly which messages show up here, use the <b>Settings</b> command from the menu."));

	tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

	qiUser.addFile(QLatin1String("skin:default_avatar.svg"));

	createTab(QString::fromUtf8("general"), tr("General"), true);

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
	connect(tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(onTabMoved(int, int)));
	connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTabBarCustomContextMenuRequested(QPoint)));
}

int LogTabWidget::createTab(QString qsHash, QString qsName, bool isChannel) {
	int index = getTab(qsHash);
	if (index != -1) {
		setTabText(index, qsName);
		return index;
	}

	m_hashMap.insert(qsHash, count());

	LogTab *lt = new LogTab(qsHash, isChannel, this);
	addTab(lt, qsName);

	index = count() - 1;
	setTabName(index, qsName);

	if (!isChannel) {
		setTabIcon(index, qiUser);
	}

	lt->document()->setMaximumBlockCount(m_maxBlockCount);

	connect(lt, SIGNAL(anchorClicked(const QUrl &)), this, SIGNAL(anchorClick(const QUrl &)));
	connect(lt, SIGNAL(customContextMenuRequested(const QPoint &)), this, SIGNAL(customContextMenuRequest(const QPoint &)));

	return index;
}

void LogTabWidget::openTab(int index) {
	if (index < 0) {
		index = getGeneralTab();
	}

	setCurrentIndex(index);
}

void LogTabWidget::setTabName(int index, QString qsName) {
	LogTab *lt = dynamic_cast<LogTab *>(widget(index));
	if (!lt) {
		return;
	}

	lt->m_name = qsName;
	setTabText(index, lt->m_name);

	if (index == getGeneralTab()) {
		setTabToolTip(index, lt->m_name);
	} else if (isChannelTab(index)) {
		setTabToolTip(index, tr("Channel: ") + lt->m_name);
	} else {
		setTabToolTip(index, tr("User: ") + lt->m_name);
	}
}

int LogTabWidget::getTab(QString qsHash) {
	HashMap::const_iterator iter = m_hashMap.find(qsHash);
	return iter != m_hashMap.constEnd() ? iter.value() : -1;
}

LogTabList LogTabWidget::getTabs() {
	LogTabList tabs;
	HashMap::const_iterator iter = m_hashMap.constBegin();
	while (iter != m_hashMap.constEnd()) {
		tabs.append(iter.value());
		++iter;
	}

	return tabs;
}

int LogTabWidget::getGeneralTab() {
	return m_hashMap.find(QString::fromUtf8("general")).value();
}

int LogTabWidget::getCurrentTab() {
	return currentIndex();
}

LogTabList LogTabWidget::getChannelTabs() {
	LogTabList tabs;
	HashMap::const_iterator iter = m_hashMap.constBegin();
	while (iter != m_hashMap.constEnd()) {
		const int tab = iter.value();
		if (isChannelTab(tab) && tab != getGeneralTab()) {
			tabs.append(tab);
		}

		++iter;
	}

	return tabs;
}

QString LogTabWidget::getHash(int index) {
	return dynamic_cast<LogTab *>(widget(index))->m_hash;
}

bool LogTabWidget::isChannelTab(int index) {
	return dynamic_cast<LogTab *>(widget(index))->m_isChannel;
}

void LogTabWidget::markTabAsUpdated(int index) {
	if (currentIndex() != index) {
		tabBar()->setTabTextColor(index, Qt::blue);
	}
}

void LogTabWidget::markTabAsRestricted(int index) {
	tabBar()->setTabTextColor(index, Qt::gray);
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

void LogTabWidget::updateHashMap() {
	m_hashMap.clear();

	for (int i = 0; i < count(); i++) {
		LogTab *lt = dynamic_cast<LogTab *>(widget(i));
		m_hashMap.insert(lt->m_hash, i);
	}
}

void LogTabWidget::onTabMoved(int, int) {
	updateHashMap();
}

void LogTabWidget::onTabCloseRequested(int index) {
	QString hashKey = dynamic_cast<LogTab *>(widget(index))->m_hash;
	m_hashMap.remove(hashKey);
	widget(index)->deleteLater();
	removeTab(index);
	updateHashMap();
}

void LogTabWidget::onTabCloseAllRequested() {
	for (int i = count() - 1; i >= 0; i--) {
		if (i == getGeneralTab()) {
			continue;
		}

		QString hashKey = dynamic_cast<LogTab *>(widget(i))->m_hash;
		m_hashMap.remove(hashKey);
		widget(i)->deleteLater();
		removeTab(i);
	}

	updateHashMap();
}

void LogTabWidget::onTabBarCustomContextMenuRequested(const QPoint& point) {
	QSignalMapper *mapper = new QSignalMapper(this);
	for (int i = 0; i < count(); i++) {
		if (i == getGeneralTab()) {
			continue;
		}

		if (tabBar()->tabRect(i).contains(point)) {
			QMenu *menu = new QMenu();
			QAction *closeAction = menu->addAction(tr("Close"));
			QAction *closeAllAction = menu->addAction(tr("Close all"));

			mapper->setMapping(closeAction, i);

			connect(closeAllAction, SIGNAL(triggered()), this, SLOT(onTabCloseAllRequested()));
			connect(closeAction, SIGNAL(triggered()), mapper, SLOT(map()));
			connect(mapper, SIGNAL(mapped(int)), this, SLOT(onTabCloseRequested(int)));
			menu->exec(QCursor::pos());
			break;
		}
	}
}
