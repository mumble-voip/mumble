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

#ifndef MUMBLE_MUMBLE_LOGTABS_H
#define MUMBLE_MUMBLE_LOGTABS_H

#include <QTabWidget>

#include "ui_Log.h"
#include "CustomElements.h"

class ClientUser;

///Wrapping LogTextBrowser, to provide additional functionality for the tabs
class LogTab : public LogTextBrowser{
	Q_OBJECT
	///Access to LogTab members should only be possible for LogTabWidget
	friend class LogTabWidget;
public slots:
	void onHighlighted(const QUrl&);
private:
	QString m_name;
	QString m_hash;
	///Initialize a LogTab from a user
	LogTab(ClientUser*, QWidget *p = NULL);
	~LogTab();
	void    addToTabWidget(QTabWidget*);
	///Updates the names of the related user
	void    updateUser(ClientUser*);
};

///Wrapping QTabWidget, to access protected members
class LogTabWidget : public QTabWidget {
	Q_OBJECT
public:
	LogTabWidget(QWidget* parent = 0);
	~LogTabWidget();
	///Set the visibility of the tabbar
	void    activateTabs(bool);
	int     getChannelTab();
	void    openTab(ClientUser*);
	int     findTab(ClientUser*);
	int     searchTab(ClientUser*);
	QString getHash(int);
	void    updateTab(ClientUser*);
	///Adds a visible notification, indicating the related tab has been updated
	void     markTabAsUpdated(int);
	///Adds a visible notification, indicating the related tab has been restricted
	void     markTabAsRestricted(int);
	///Sets the maximum block count for all tabs
	void    handleDocumentsetMaximumBlockCount(int);
	///Sets the default stylesheet for all tabs
	void    handleDocumentSetDefaultStyleSheet(QString);
public slots:
	void 	onCurrentChanged(int);
	void    onTabMoved(int, int);
	void    onTabCloseRequested(int);
	void    onTabBarCustomContextMenuRequested(const QPoint&);
signals:
	void 	anchorClick(const QUrl&);
	void    customContextMenuRequest(const QPoint&);
	void    highlighted(const QUrl&);
private:
	///Mapping a users hash to a tab index
	QHash<QString, int>* m_hashMap;
	int     m_maxBlockCount;
	void    updateHashMap();
	int     createTab(ClientUser*);
};

#endif
