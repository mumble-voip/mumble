/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _USERVIEW_H
#define _USERVIEW_H

#include "mumble_pch.hpp"

#include "Timer.h"

class UserDelegate : public QStyledItemDelegate {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(UserDelegate)
	public:
		UserDelegate(QObject *parent = NULL);
		void paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	public slots:
		bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);
};

class UserView : public QTreeView {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(UserView)
	protected:
		void mouseReleaseEvent(QMouseEvent *);
		void keyPressEvent(QKeyEvent *);
		bool event(QEvent *);

		QTimer *qtSearch;
		QPersistentModelIndex qpmiSearch;
		Timer tSearch;
		QString qsSearch;
	public:
		UserView(QWidget *);
		void keyboardSearch(const QString &search);
	public slots:
		void nodeActivated(const QModelIndex &idx);
		void selectSearchResult();

};

#else
class UserView;
#endif
