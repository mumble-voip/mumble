/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_RICHTEXTEDITOR_H_
#define MUMBLE_MUMBLE_RICHTEXTEDITOR_H_

#include <QtCore/QtGlobal>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
# include <QtWidgets/QTextEdit>
#else
# include <QtGui/QTextEdit>
#endif

class LogDocument;

class RichTextHtmlEdit : public QTextEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(RichTextHtmlEdit)
	protected:
		void insertFromMimeData(const QMimeData *source);
	public:
		RichTextHtmlEdit(QWidget *p);
	private:
		LogDocument *m_document;
};

#include "ui_RichTextEditor.h"
#include "ui_RichTextEditorLink.h"


class RichTextEditorLink : public QDialog, Ui::RichTextEditorLink {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(RichTextEditorLink)
	public:
		RichTextEditorLink(const QString &text = QString(), QWidget *p = NULL);
		QString text() const;
};

class RichTextEditor : public QTabWidget, Ui::RichTextEditor {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(RichTextEditor)
	protected:
		bool bModified;
		bool bChanged;
		bool bReadOnly;
		void richToPlain();
		QColor qcColor;
	public:
		RichTextEditor(QWidget *p = NULL);
		QString text();
		bool isModified() const;
	public slots:
		void setText(const QString &text, bool readonly = false);
		void updateColor(const QColor &);
		void updateActions();
	protected slots:
		void on_qaBold_triggered(bool);
		void on_qaItalic_triggered(bool);
		void on_qaUnderline_triggered(bool);
		void on_qaColor_triggered();
		void on_qaLink_triggered();
		void on_qaImage_triggered();

		void on_qptePlainText_textChanged();
		void on_qteRichText_textChanged();
		void on_qteRichText_cursorPositionChanged();
		void on_qteRichText_currentCharFormatChanged();
		void onCurrentChanged(int);
};

class RichTextImage {
	public:
		static bool isValidImage(const QByteArray &buf, QByteArray &fmt);
};

#endif
