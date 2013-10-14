/* Copyright (C) 2013, Frank Mueller <frmimue@gmail.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
	 this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
	 this list of conditions and the following disclaimer in the documentation
	 and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
	 contributors may be used to endorse or promote products derived from this
	 software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "LogTabs.h"
#include "ClientUser.h"
#include "ui_MainWindow.h"
#include "Log.h"

#include <QtCore/QtGlobal>
#include <QSignalMapper>

LogTab::LogTab(ClientUser* user,QWidget *p) : LogTextBrowser(p){
	this->m_hash = user->qsHash;
	this->m_name = user->qsName;
	this->setFrameStyle(QFrame::NoFrame);
	this->setOpenLinks(false);
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	LogDocument* ld = new LogDocument(this);
	ld->setDefaultStyleSheet(qApp->styleSheet());
	this->setDocument(ld);
	connect(this, SIGNAL(highlighted(QUrl)), this, SLOT(onHighlighted(QUrl)));
	this->setWhatsThis(QString::fromUtf8("This shows all recent activity. Connecting to servers, errors and information messages all show up here.&lt;br /&gt;To configure exactly which messages show up here, use the &lt;b&gt;Settings&lt;/b&gt; command from the menu."));
}

LogTab::~LogTab(){
}

void LogTab::addToTabWidget(QTabWidget * tabWidget){
	tabWidget->addTab(this, m_name);
}

void LogTab::updateUser(ClientUser* user){
	this->m_name  = user->qsName;
}

void LogTab::onHighlighted(const QUrl& url){
	if (QString::fromLatin1("clientid") == url.scheme() || QString::fromLatin1("channelid") == url.scheme())
		return;
	if (! url.isValid())
		QToolTip::hideText();
	else
		QToolTip::showText(QCursor::pos(), url.toString(), this, QRect());
}

LogTabWidget::LogTabWidget(QWidget* parent) : QTabWidget(parent){
	this->setTabPosition(QTabWidget::South);
	this->setMovable(true);
	this->setUsesScrollButtons(true);
	this->setElideMode(Qt::ElideRight);
	this->setStyleSheet(QString::fromUtf8("QTabBar::tab{min-width: 1px}"));

	this->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
	connect(this->tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(onTabMoved(int, int)));
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
	connect(this->tabBar(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTabBarCustomContextMenuRequested(QPoint)));

	QString tmp = QString::fromUtf8("channel");
	ClientUser* channel =  new ClientUser();
	channel->qsHash = tmp;
	channel->qsName = tmp;
	this->m_hashMap = new QHash<QString, int>();
	this->createTab(channel);
	this->setTabText(0, QString::fromUtf8("Not connected"));
	this->setTabToolTip(0, QString::fromUtf8("Not connected"));
}

LogTabWidget::~LogTabWidget(){
}

void LogTabWidget::activateTabs(bool show){
	this->tabBar()->setVisible(show);
}

int LogTabWidget::findTab(ClientUser *user){
	QHash<QString, int>::const_iterator iter = m_hashMap->find(user->qsHash);
	return iter != m_hashMap->constEnd() ? iter.value() : createTab(user);
}

int LogTabWidget::searchTab(ClientUser *user){
	QHash<QString, int>::const_iterator iter = m_hashMap->find(user->qsHash);
	return iter != m_hashMap->constEnd() ? iter.value() : -1;
}

int LogTabWidget::createTab(ClientUser *user){
	m_hashMap->insert(user->qsHash, count());
	LogTab* lt = new LogTab(user, this);
	lt->addToTabWidget(this);
	lt->document()->setMaximumBlockCount(m_maxBlockCount);
	this->setTabToolTip(count()-1, lt->m_name);
	connect(lt, SIGNAL(anchorClicked(const QUrl&)), this, SIGNAL(anchorClick(const QUrl&)));
	connect(lt, SIGNAL(customContextMenuRequested(const QPoint&)), this, SIGNAL(customContextMenuRequest(const QPoint&)));
	return count()-1;
}

void LogTabWidget::openTab(ClientUser* user){
	this->setCurrentIndex(findTab(user));
}

int LogTabWidget::getChannelTab(){
	return this->m_hashMap->find(QString::fromUtf8("channel")).value();
}

QString LogTabWidget::getHash(int index){
	return dynamic_cast<LogTab*>(widget(index))->m_hash;
}

void LogTabWidget::updateHashMap(){
	m_hashMap->clear();
	for(int i = 0; i < count(); i++){
		m_hashMap->insert(dynamic_cast<LogTab*>(widget(i))->m_hash, i);
	}
}

void LogTabWidget::markTabAsUpdated(int index){
	if(this->currentIndex() != index)
		this->tabBar()->setTabTextColor(index, Qt::blue);
}

void LogTabWidget::markTabAsRestricted(int index){
	this->tabBar()->setTabTextColor(index, Qt::gray);
}

void LogTabWidget::updateTab(ClientUser* user){
	int index = searchTab(user);
	if(-1 == index)
		return;
	LogTab* lt = dynamic_cast<LogTab*>(widget(index));
	lt->updateUser(user);
	setTabText(index, lt->m_name);
	this->setTabToolTip(index, lt->m_name);
}

void LogTabWidget::handleDocumentsetMaximumBlockCount(int maxLogBlocks){
	for(int i = 0; i < count(); i++){
		dynamic_cast<LogTab*>(widget(i))->document()->setMaximumBlockCount(maxLogBlocks);
	}
	this->m_maxBlockCount = maxLogBlocks;
}

void LogTabWidget::handleDocumentSetDefaultStyleSheet(QString styleSheet){
	for(int i = 0; i < count(); i++){
		dynamic_cast<LogTab*>(widget(i))->document()->setDefaultStyleSheet(styleSheet);
	}
}

void LogTabWidget::onCurrentChanged(int newIndex){
	this->tabBar()->setTabTextColor(newIndex, Qt::black);
}

void LogTabWidget::onTabMoved(int, int){
	this->updateHashMap();
}

void LogTabWidget::onTabCloseRequested(int index){
	QString hashKey = dynamic_cast<LogTab*>(widget(index))->m_hash;
	m_hashMap->remove(hashKey);
	widget(index)->deleteLater();
	removeTab(index);
	updateHashMap();
}

void LogTabWidget::onTabBarCustomContextMenuRequested(const QPoint& point){
	QSignalMapper *mapper = new QSignalMapper();
	for(int i = 0; i < count(); i++){
		if(i == getChannelTab())
			continue;
		if(this->tabBar()->tabRect(i).contains(point)){
			QMenu* menu = new QMenu();
			QAction* action = menu->addAction(QString::fromUtf8("Close"));
			mapper->setMapping(action, i);
			connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
			connect(mapper, SIGNAL(mapped(int)), this, SLOT(onTabCloseRequested(int)));
			connect(mapper, SIGNAL(mapped(int)), this, SIGNAL(currentChanged(int)));
			menu->exec(QCursor::pos());
			break;
		}
	}
}
