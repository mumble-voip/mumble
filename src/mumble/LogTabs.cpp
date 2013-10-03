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

LogTabWidget::LogTabWidget(QWidget* parent) : QTabWidget(parent){
	this->setTabPosition(QTabWidget::South);
	this->mHashIndex = new QHash<QString, int>();
	this->mIndexHash = new QList<QString>();
	QString tmp = QString::fromUtf8("channel");
	this->mIndexHash->append(tmp);
	this->mHashIndex->insert(tmp, 0);
	this->setTabsClosable(true);
	this->setMovable(true);
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
	connect(this->tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(onTabMoved(int, int)));
	//this->setStyleSheet(QString::fromUtf8("QTabBar::tab:!selected{max-width: 100px;}"));
}

LogTabWidget::~LogTabWidget(){
}

void LogTabWidget::showTabs(bool show){
	this->tabBar()->setVisible(show);
}

void LogTabWidget::newTab(QString hash, QString name){
	this->mHashIndex->insert(hash, this->count());
	this->mIndexHash->append(hash);
	LogTextBrowser* ltb = new LogTextBrowser();
	ltb->setOpenLinks(false);
	ltb->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ltb, SIGNAL(anchorClicked(const QUrl&)), this, SIGNAL(anchorClick(const QUrl&)));
	connect(ltb, SIGNAL(customContextMenuRequested(const QPoint&)), this, SIGNAL(customContextMenuRequest(const QPoint&)));
	LogDocument *ld = new LogDocument(ltb);
	ltb->setDocument(ld);
	ltb->document()->setMaximumBlockCount(g.s.iMaxLogBlocks);
	ltb->document()->setDefaultStyleSheet(qApp->styleSheet());
	QString tabName = name;
	if(tabName.size() > 8)
		tabName.resize(8);
	this->QTabWidget::addTab(ltb, tabName);
	this->setTabToolTip(this->count(), name);
}

void LogTabWidget::validateTab(ClientUser* user){
	if(mHashIndex->find(user->qsHash) == mHashIndex->end())
		newTab(user->qsHash, user->qsName);
}

void LogTabWidget::openTab(ClientUser* user){
	validateTab(user);
	this->setCurrentIndex(mHashIndex->find(user->qsHash).value());
}

int LogTabWidget::getTabIndex(ClientUser* user){
	validateTab(user);
	return this->mHashIndex->find(user->qsHash).value();
}

int LogTabWidget::getChannelTab(){
	return this->mHashIndex->find(QString::fromUtf8("channel")).value(); 
}

QString LogTabWidget::getHash(int index){
	return this->mIndexHash->at(index);
}

int	LogTabWidget::addTab(QWidget* page, const QString& label){
	int retVal = QTabWidget::addTab(page, label);
	this->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
	return retVal;
} 

void LogTabWidget::closeTab(int index){
	QString hashKey = mIndexHash->at(index);
	mIndexHash->removeOne(hashKey);
	mHashIndex->remove(hashKey);
	widget(index)->deleteLater();
	update();
	removeTab(index);
}

void LogTabWidget::update(){
	mHashIndex->clear();
	for(int i = 0; i < mIndexHash->count(); i++)
		mHashIndex->insert(mIndexHash->at(i), i);
}

int	LogTabWidget::markTabAsUpdated(int index){
	if(this->currentIndex() != index)
		this->tabBar()->setTabTextColor(index, Qt::blue); 
}

int LogTabWidget::markTabAsRestricted(int index){
	this->tabBar()->setTabTextColor(index, Qt::gray);
}

void LogTabWidget::userUpdate(ClientUser* user){
	QString tabName = user->qsName;
	if(tabName.size() > 8)
		tabName.resize(8);
	this->setTabText(mHashIndex->find(user->qsHash).value(), tabName);
	this->setTabToolTip(this->count(), user->qsName);
}

void LogTabWidget::onCurrentChanged(int index){
	this->tabBar()->setTabTextColor(index, Qt::black); 
}

void LogTabWidget::onTabMoved(int from, int to){
	this->mIndexHash->swap(from, to);
	this->update();
}
