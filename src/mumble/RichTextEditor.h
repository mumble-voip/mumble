// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_RICHTEXTEDITOR_H_
#define MUMBLE_MUMBLE_RICHTEXTEDITOR_H_

#include <QtCore/QtGlobal>
#include <QtWidgets/QTextEdit>

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
	RichTextEditorLink(const QString &text = QString(), QWidget *p = nullptr);
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
	bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

public:
	RichTextEditor(QWidget *p = nullptr);
	QString text();
	bool isModified() const;
signals:
	/// The accept signal is emitted when Ctrl-Enter is pressed inside the RichTextEditor.
	void accept();
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
