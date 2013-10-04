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
#include "CustomElements.h"
#include "UserModel.h"
#include "ClientUser.h"
#include "ui_MainWindow.h"
#include "Global.h"
#include "MainWindow.h"
#include "Log.h"

#include <QtCore/QtGlobal>

LogTab::LogTab(ClientUser* user,QWidget *p) : LogTextBrowser(p){
    this->hash      = user->qsHash;
    this->fullname  = user->qsName;
    this->shortName = user->qsName;
    if(shortName.size() > 8)
        shortName.resize(8);
    this->initTab();
}

LogTab::LogTab(QWidget *p) : LogTextBrowser(p){
    this->hash      = QString::fromUtf8("channel");
    this->fullname  = QString::fromUtf8("channel");
    this->shortName = QString::fromUtf8("channel");
    if(shortName.size() > 8)
        shortName.resize(8);
    this->initTab();
}

void LogTab::initTab(){
    this->setFrameStyle(QFrame::NoFrame);
    this->setOpenLinks(false);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    LogDocument* ld = new LogDocument(this);
    ld->setMaximumBlockCount(g.s.iMaxLogBlocks);
    ld->setDefaultStyleSheet(qApp->styleSheet());
    this->setDocument(ld);
    connect(this, SIGNAL(highlighted(QUrl)), this, SLOT(onHighlighted(QUrl)));
}

LogTab::~LogTab(){
}

void LogTab::addToTabWidget(QTabWidget * tabWidget){
    tabWidget->addTab(this, shortName);
}

void LogTab::updateUser(ClientUser* user){
    this->fullname  = user->qsName;
    this->shortName = user->qsName;
    if(shortName.size() > 8)
        shortName.resize(8);
}

void LogTab::onHighlighted(const QUrl& url){
    if (url.scheme() == QString::fromLatin1("clientid") || url.scheme() == QString::fromLatin1("channelid"))
        return;
    if (! url.isValid())
        QToolTip::hideText();
    else
        QToolTip::showText(QCursor::pos(), url.toString(), this, QRect());
}

LogTabWidget::LogTabWidget(QWidget* parent) : QTabWidget(parent){
	this->setTabPosition(QTabWidget::South);
    this->mHashIndex = new QHash<QString, int>();
    QString tmp = QString::fromUtf8("channel");
	this->mHashIndex->insert(tmp, 0);
	this->setTabsClosable(true);
	this->setMovable(true);
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
    connect(this->tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(onTabMoved(int, int)));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
}

LogTabWidget::~LogTabWidget(){
}

void LogTabWidget::showTabs(bool show){
	this->tabBar()->setVisible(show);
}

int LogTabWidget::findTab(ClientUser *user){
    QHash<QString, int>::const_iterator iter = mHashIndex->find(user->qsHash);
    return iter != mHashIndex->constEnd() ? iter.value() : createTab(user);
}

int LogTabWidget::searchTab(ClientUser *user){
    QHash<QString, int>::const_iterator iter = mHashIndex->find(user->qsHash);
    return iter != mHashIndex->constEnd() ? iter.value() : -1;
}

int LogTabWidget::createTab(ClientUser *user){
    mHashIndex->insert(user->qsHash, count());
    LogTab* lt = new LogTab(user);
    //lt->setOpenLinks(false);
    //lt->setContextMenuPolicy(Qt::CustomContextMenu);
    //LogDocument* ld = new LogDocument(lt);
    //ld->setMaximumBlockCount(g.s.iMaxLogBlocks);
    //ld->setDefaultStyleSheet(qApp->styleSheet());
    //lt->setDocument(ld);
    lt->addToTabWidget(this);
    connect(lt, SIGNAL(anchorClicked(const QUrl&)), this, SIGNAL(anchorClick(const QUrl&)));
    connect(lt, SIGNAL(customContextMenuRequested(const QPoint&)), this, SIGNAL(customContextMenuRequest(const QPoint&)));
    //connect(lt, SIGNAL(highlighted(QUrl&)), this, SIGNAL(highlighted(QUrl&)));
    return count()-1;
}

void LogTabWidget::openTab(ClientUser* user){
    this->setCurrentIndex(findTab(user));
}

int LogTabWidget::getChannelTab(){
	return this->mHashIndex->find(QString::fromUtf8("channel")).value(); 
}

QString LogTabWidget::getHash(int index){
    return dynamic_cast<LogTab*>(widget(index))->hash;
}

int	LogTabWidget::addTab(QWidget* page, const QString& label){
	int retVal = QTabWidget::addTab(page, label);
	this->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
	return retVal;
}

void LogTabWidget::update(){
	mHashIndex->clear();
    for(int i = 0; i < count(); i++){
        mHashIndex->insert(dynamic_cast<LogTab*>(widget(i))->hash, i);
    }
}

int	LogTabWidget::markTabAsUpdated(int index){
	if(this->currentIndex() != index)
		this->tabBar()->setTabTextColor(index, Qt::blue); 
}

int LogTabWidget::markTabAsRestricted(int index){
	this->tabBar()->setTabTextColor(index, Qt::gray);
}

void LogTabWidget::updateTab(ClientUser* user){
    int index = searchTab(user);
    if(index == -1)
        return;
    dynamic_cast<LogTab*>(widget(index))->updateUser(user);
    if(index == currentIndex())
        setTabText(index, dynamic_cast<LogTab*>(widget(index))->fullname);
    else
        setTabText(index, dynamic_cast<LogTab*>(widget(index))->shortName);
}

void LogTabWidget::handleDocumentsetMaximumBlockCount(int maxLogBlocks){
    for(int i = 0; i < count(); i++){
        dynamic_cast<LogTab*>(widget(i))->document()->setMaximumBlockCount(maxLogBlocks);
    }
}

void LogTabWidget::handleDocumentSetDefaultStyleSheed(QString styleSheet){
    for(int i = 0; i < count(); i++){
        dynamic_cast<LogTab*>(widget(i))->document()->setDefaultStyleSheet(styleSheet);
    }
}

void LogTabWidget::onCurrentChanged(int newIndex){
    this->tabBar()->setTabTextColor(newIndex, Qt::black);
    setTabText(newIndex, dynamic_cast<LogTab*>(widget(newIndex))->fullname);
    setTabText(oldIndex, dynamic_cast<LogTab*>(widget(oldIndex))->shortName);
    oldIndex = newIndex;
}

void LogTabWidget::onTabMoved(int to, int from){
    oldIndex = to;
	this->update();
}

void LogTabWidget::onTabCloseRequested(int index){
    QString hashKey = dynamic_cast<LogTab*>(widget(index))->hash;
    mHashIndex->remove(hashKey);
    widget(index)->deleteLater();
    removeTab(index);
}
