/* Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>
   Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_CUSTOMELEMENTS_H_
#define MUMBLE_MUMBLE_CUSTOMELEMENTS_H_

#include <QtCore/QObject>

#if QT_VERSION >= 0x050000
# include <QtWidgets/QLabel>
# include <QtWidgets/QTextBrowser>
# include <QtWidgets/QTextEdit>
#else
# include <QtGui/QLabel>
# include <QtGui/QTextBrowser>
# include <QtGui/QTextEdit>
#endif

class LogTextBrowser : public QTextBrowser {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogTextBrowser)
	protected:
		void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
		bool event(QEvent *e) Q_DECL_OVERRIDE;
	public:
		LogTextBrowser(QWidget *p = NULL);

		int getLogScroll();
		int getLogScrollMaximum();
		void setLogScroll(int pos);
		void scrollLogToBottom();
};

class ChatbarTextEdit : public QTextEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ChatbarTextEdit)
		void inFocus(bool);
		QStringList qslHistory;
		QString qsHistoryTemp;
		int iHistoryIndex;
		static const int MAX_HISTORY = 50;
	protected:
		QString qsDefaultText;
		bool bDefaultVisible;
		void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;
		void focusOutEvent(QFocusEvent *) Q_DECL_OVERRIDE;
		void contextMenuEvent(QContextMenuEvent *) Q_DECL_OVERRIDE;
		void dropEvent(QDropEvent *) Q_DECL_OVERRIDE;
		bool event(QEvent *) Q_DECL_OVERRIDE;
		QSize minimumSizeHint() const Q_DECL_OVERRIDE;
		QSize sizeHint() const Q_DECL_OVERRIDE;
		void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
	public:
		void setDefaultText(const QString &, bool = false);
		unsigned int completeAtCursor();
	signals:
		void tabPressed(void);
		void ctrlSpacePressed(void);
		void entered(QString);
	public slots:
		void pasteAndSend_triggered();
		void doResize();
		void doScrollbar();
		void addToHistory(const QString &str);
		void historyUp();
		void historyDown();
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
		QSize sizeHint() const Q_DECL_OVERRIDE;
		QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	public slots:
		void tick();
	protected:
		bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;
};

#endif // CUSTOMELEMENTS_H_
