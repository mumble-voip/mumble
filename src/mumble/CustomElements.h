/* Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>
   Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#ifndef CUSTOMELEMENTS_H
#define CUSTOMELEMENTS_H

#include "mumble_pch.hpp"

class ChatbarTextEdit : public QTextEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ChatbarTextEdit)
		void inFocus(bool);
	protected:
		QString qsDefaultText;
		bool bDefaultVisible;
		void focusInEvent(QFocusEvent *);
		void focusOutEvent(QFocusEvent *);
		void contextMenuEvent(QContextMenuEvent *);
		void dropEvent(QDropEvent *);
		bool event(QEvent *);
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		void resizeEvent(QResizeEvent *e);
	public:
		void setDefaultText(const QString &, bool = false);
		unsigned int completeAtCursor();
	signals:
		void tabPressed(void);
		void ctrlSpacePressed(void);
	public slots:
		void pasteAndSend_triggered();
		void doResize();
		void doScrollbar();
	public:
		ChatbarTextEdit(QWidget *p = NULL);
};

class DockTitleBar : public QLabel {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(DockTitleBar)
	protected:
		QTimer *qtTick;
		int size;
		int newsize;
	public:
		DockTitleBar();
		QSize sizeHint() const;
		QSize minimumSizeHint() const;
	public slots:
		void tick();
	protected:
		bool eventFilter(QObject *, QEvent *);
};

#endif // CUSTOMELEMENTS_H
