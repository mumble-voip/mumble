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
#include "iostream"

LogTabs::LogTabs(QWidget* parent) : QTabWidget(parent){
	this->mHashIndex = new QHash<QString, int>();
	this->mIndexHash = new QList<QString>();
	QString tmp;
	this->mIndexHash->append(tmp);
	this->setTabsClosable(true);
}

LogTabs::~LogTabs(){
}

void LogTabs::showTabs(bool show){
	this->tabBar()->setVisible(show);
}

void LogTabs::newTab(QString hash){
	this->mHashIndex->insert(hash, this->count());
	this->mIndexHash->append(hash);
	this->QTabWidget::addTab(new LogTextBrowser(), g.mw->pmModel->getUser(hash)->qsName);
}

void LogTabs::openTab(QString hash){
	if(mHashIndex->find(hash) == mHashIndex->end())
		newTab(hash);
	this->setCurrentIndex(mHashIndex->find(hash).value());
}

int LogTabs::getTabIndex(QString hash){
	if(mHashIndex->find(hash) == mHashIndex->end())
		newTab(hash);
	return this->mHashIndex->find(hash).value();
}

QString LogTabs::getHash(int index){
	return this->mIndexHash->at(index);
}

int	LogTabs::addTab(QWidget* page, const QString& label){
	QTabWidget::addTab(page, label);
	this->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
} 

void LogTabs::closeTab(int index){
	QString hashKey = mIndexHash->at(index);
	mIndexHash->removeOne(hashKey);
	mHashIndex->remove(hashKey);
	widget(index)->deleteLater();
	removeTab(index);
	update();
}

void LogTabs::update(){
	mHashIndex->clear();
	for(int i = 0; i < mIndexHash->count(); i++)
		mHashIndex->insert(mIndexHash->at(i), i);
}
