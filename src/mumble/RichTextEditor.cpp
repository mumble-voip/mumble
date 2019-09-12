// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "RichTextEditor.h"

#include "Log.h"
#include "MainWindow.h"
#include "XMLTools.h"

#include <QtCore/QMimeData>
#include <QtGui/QImageReader>
#include <QtGui/QPainter>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolTip>

#ifdef Q_OS_WIN
# include <shlobj.h>
#endif

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

RichTextHtmlEdit::RichTextHtmlEdit(QWidget *p) : QTextEdit(p) {
	m_document = new LogDocument(this);
	m_document->setDefaultStyleSheet(qApp->styleSheet());
	setDocument(m_document);
}

/* On nix, some programs send utf8, some send wchar_t. Some zeroterminate once, some twice, some not at all.
 */

static QString decodeMimeString(const QByteArray &src) {
	if (src.isEmpty())
		return QString();

	if ((src.length() >= 4) && ((src.length() % sizeof(ushort)) == 0)) {
		const ushort *ptr = reinterpret_cast<const ushort *>(src.constData());
		int len = static_cast<int>(src.length() / sizeof(ushort));
		if ((ptr[0] > 0) && (ptr[0] < 0x7f) && (ptr[1] > 0) && (ptr[1] < 0x7f)) {
			while (len && (ptr[len - 1] == 0))
				--len;
			return QString::fromUtf16(ptr, len);
		}
	}

	if ((sizeof(wchar_t) != sizeof(ushort)) && (src.length() >= static_cast<int>(sizeof(wchar_t))) && ((src.length() % sizeof(wchar_t)) == 0)) {
		const wchar_t *ptr = reinterpret_cast<const wchar_t *>(src.constData());
		int len = static_cast<int>(src.length() / sizeof(wchar_t));
		if (*ptr < 0x7f) {
			while (len && (ptr[len - 1] == 0))
				--len;
			return QString::fromWCharArray(ptr, len);
		}
	}
	const char *ptr = src.constData();
	int len = src.length();
	while (len && (ptr[len - 1] == 0))
		--len;
	return QString::fromUtf8(ptr, len);
}

/* Try really hard to properly decode Mime into something sane.
 */

void RichTextHtmlEdit::insertFromMimeData(const QMimeData *source) {
	QString uri;
	QString title;
	QRegExp newline(QLatin1String("[\\r\\n]"));

#ifndef QT_NO_DEBUG
	qWarning() << "RichTextHtmlEdit::insertFromMimeData" << source->formats();
#endif

	if (source->hasImage()) {
		QImage img = qvariant_cast<QImage>(source->imageData());
		QString html = Log::imageToImg(img);
		if (! html.isEmpty())
			insertHtml(html);
		return;
	}

	QString mozurl = decodeMimeString(source->data(QLatin1String("text/x-moz-url")));
	if (! mozurl.isEmpty()) {
		QStringList lines = mozurl.split(newline);
		qWarning() << mozurl << lines;
		if (lines.count() >= 2) {
			uri = lines.at(0);
			title = lines.at(1);
		}
	}

	if (uri.isEmpty())
		uri = decodeMimeString(source->data(QLatin1String("text/x-moz-url-data")));
	if (title.isEmpty())
		title = decodeMimeString(source->data(QLatin1String("text/x-moz-url-desc")));

	if (uri.isEmpty()) {
		QStringList urls;
#ifdef Q_OS_WIN
		urls = decodeMimeString(source->data(QLatin1String("application/x-qt-windows-mime;value=\"UniformResourceLocatorW\""))).split(newline);
		if (urls.isEmpty())
#endif
			urls = decodeMimeString(source->data(QLatin1String("text/uri-list"))).split(newline);
		if (! urls.isEmpty())
			uri = urls.at(0).trimmed();
	}

	if (uri.isEmpty()) {
		QUrl url(source->text(), QUrl::StrictMode);
		if (url.isValid() && ! url.isRelative()) {
			uri = url.toString();
		}
	}

#ifdef Q_OS_WIN
	if (title.isEmpty() && source->hasFormat(QLatin1String("application/x-qt-windows-mime;value=\"FileGroupDescriptorW\""))) {
		QByteArray qba = source->data(QLatin1String("application/x-qt-windows-mime;value=\"FileGroupDescriptorW\""));
		if (qba.length() == sizeof(FILEGROUPDESCRIPTORW)) {
			const FILEGROUPDESCRIPTORW *ptr = reinterpret_cast<const FILEGROUPDESCRIPTORW *>(qba.constData());
			title = QString::fromWCharArray(ptr->fgd[0].cFileName);
			if (title.endsWith(QLatin1String(".url"), Qt::CaseInsensitive))
				title = title.left(title.length() - 4);
		}
	}
#endif

	if (! uri.isEmpty()) {
		if (title.isEmpty())
			title = uri;
#if QT_VERSION >= 0x050000
		uri = uri.toHtmlEscaped();
		title = title.toHtmlEscaped();
#else
		uri = Qt::escape(uri);
		title = Qt::escape(title);
#endif

		insertHtml(QString::fromLatin1("<a href=\"%1\">%2</a>").arg(uri, title));
		return;
	}

	QString html = decodeMimeString(source->data(QLatin1String("text/html")));
	if (! html.isEmpty()) {
		insertHtml(html);
		return;
	}

	QTextEdit::insertFromMimeData(source);
}

RichTextEditorLink::RichTextEditorLink(const QString &txt, QWidget *p) : QDialog(p) {
	setupUi(this);

	if (! txt.isEmpty()) {
		qleText->setText(txt);
	}
}

QString RichTextEditorLink::text() const {
	QUrl url(qleUrl->text(), QUrl::StrictMode);
	QString txt = qleText->text();

#if QT_VERSION >= 0x050000
	txt = txt.toHtmlEscaped();
#else
	txt = Qt::escape(txt);
#endif

	if (url.isValid() && ! url.isRelative() && ! txt.isEmpty()) {
		return QString::fromLatin1("<a href=\"%1\">%2</a>").arg(url.toString(), txt);
	}

	return QString();
}

RichTextEditor::RichTextEditor(QWidget *p) : QTabWidget(p) {
	bChanged = false;
	bModified = false;
	bReadOnly = false;

	setupUi(this);

	qtbToolBar->addAction(qaBold);
	qtbToolBar->addAction(qaItalic);
	qtbToolBar->addAction(qaUnderline);
	qtbToolBar->addAction(qaColor);
	qtbToolBar->addSeparator();
	qtbToolBar->addAction(qaLink);
	qtbToolBar->addAction(qaImage);

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
	updateActions();

	qteRichText->setFocus();

	qteRichText->installEventFilter(this);
	qptePlainText->installEventFilter(this);
}

bool RichTextEditor::isModified() const {
	return bModified;
}

void RichTextEditor::on_qaBold_triggered(bool on) {
	qteRichText->setFontWeight(on ? QFont::Bold : QFont::Normal);
}

void RichTextEditor::on_qaItalic_triggered(bool on) {
	qteRichText->setFontItalic(on);
}

void RichTextEditor::on_qaUnderline_triggered(bool on) {
	qteRichText->setFontUnderline(on);
}

void RichTextEditor::on_qaColor_triggered() {
	QColor c = QColorDialog::getColor();
	if (c.isValid())
		qteRichText->setTextColor(c);
}

void RichTextEditor::on_qaLink_triggered() {
	QTextCursor qtc = qteRichText->textCursor();
	RichTextEditorLink *rtel = new RichTextEditorLink(qtc.selectedText(), this);
	if (rtel->exec() == QDialog::Accepted) {
		QString html = rtel->text();
		if (! html.isEmpty())
			qteRichText->insertHtml(html);
	}
	delete rtel;
}

void RichTextEditor::on_qaImage_triggered() {
	QPair<QByteArray, QImage> choice = g.mw->openImageFile();

	QByteArray &qba = choice.first;

	if (qba.isEmpty())
		return;

	if ((g.uiImageLength > 0) && (static_cast<unsigned int>(qba.length()) > g.uiImageLength)) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Image file too large to embed in document. Please use images smaller than %1 kB.").arg(g.uiImageLength /1024));
		return;
	}

	QBuffer qb(&qba);
	qb.open(QIODevice::ReadOnly);

	QByteArray format = QImageReader::imageFormat(&qb);
	qb.close();

	qteRichText->insertHtml(Log::imageToImg(format, qba));
}

void RichTextEditor::onCurrentChanged(int index) {
	if (! bChanged)
		return;

	if (index == 1)
		richToPlain();
	else
		qteRichText->setHtml(qptePlainText->toPlainText());

	bChanged = false;
}

void RichTextEditor::on_qptePlainText_textChanged() {
	bModified = true;
	bChanged = true;
}

void RichTextEditor::on_qteRichText_textChanged() {
	bModified = true;
	bChanged = true;
	updateActions();

	if (! g.uiMessageLength)
		return;

	richToPlain();

	const QString &plainText = qptePlainText->toPlainText();

	bool over = true;

	unsigned int imagelength = plainText.length();


	if (g.uiMessageLength && imagelength <= g.uiMessageLength) {
		over = false;
	} else if (g.uiImageLength && imagelength > g.uiImageLength) {
		over = true;
	} else {
		QString qsOut;
		QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(plainText));
		QXmlStreamWriter qxsw(&qsOut);
		while (! qxsr.atEnd()) {
			switch (qxsr.readNext()) {
				case QXmlStreamReader::Invalid:
					return;
				case QXmlStreamReader::StartElement: {
						if (qxsr.name() == QLatin1String("img")) {
							QXmlStreamAttributes attr = qxsr.attributes();

							qxsw.writeStartElement(qxsr.namespaceUri().toString(), qxsr.name().toString());
							foreach(const QXmlStreamAttribute &a, qxsr.attributes())
								if (a.name() != QLatin1String("src"))
									qxsw.writeAttribute(a);
						} else {
							qxsw.writeCurrentToken(qxsr);
						}
					}
					break;
				default:
					qxsw.writeCurrentToken(qxsr);
					break;
			}
		}
		over = (static_cast<unsigned int>(qsOut.length()) > g.uiMessageLength);
	}


	QString tooltip = tr("Message is too long.");

	if (!over) {
		if (QToolTip::text() == tooltip)
			QToolTip::hideText();
	} else {
		QPoint p = QCursor::pos();
		const QRect &r = qteRichText->rect();
		if (! r.contains(qteRichText->mapFromGlobal(p)))
			p = qteRichText->mapToGlobal(r.center());
		QToolTip::showText(p, tooltip, qteRichText);
	}
}

void RichTextEditor::on_qteRichText_cursorPositionChanged() {
	updateActions();
}

void RichTextEditor::on_qteRichText_currentCharFormatChanged() {
	updateActions();
}

void RichTextEditor::updateColor(const QColor &col) {
	if (col == qcColor)
		return;
	qcColor = col;

	QRect r(0,0,24,24);

	QPixmap qpm(r.size());
	QPainter qp(&qpm);
	qp.fillRect(r, col);
	qp.setPen(col.darker());
	qp.drawRect(r.adjusted(0, 0, -1, -1));

	qaColor->setIcon(qpm);
}

void RichTextEditor::updateActions() {
	qaBold->setChecked(qteRichText->fontWeight() == QFont::Bold);
	qaItalic->setChecked(qteRichText->fontItalic());
	qaUnderline->setChecked(qteRichText->fontUnderline());
	updateColor(qteRichText->textColor());
}

void RichTextEditor::richToPlain() {
	QXmlStreamReader reader(qteRichText->toHtml());

	QString qsOutput;
	QXmlStreamWriter writer(&qsOutput);

	int paragraphs = 0;

	QMap<QString, QString> def;

	def.insert(QLatin1String("margin-top"), QLatin1String("0px"));
	def.insert(QLatin1String("margin-bottom"), QLatin1String("0px"));
	def.insert(QLatin1String("margin-left"), QLatin1String("0px"));
	def.insert(QLatin1String("margin-right"), QLatin1String("0px"));
	def.insert(QLatin1String("-qt-block-indent"), QLatin1String("0"));
	def.insert(QLatin1String("text-indent"), QLatin1String("0px"));

	XMLTools::recurseParse(reader, writer, paragraphs, def);

	qsOutput = qsOutput.trimmed();

	bool changed;
	do {
		// Make sure the XML has a root element (would be invalid XML otherwise)
		// The "unduplicate" element will be dropped by XMLTools::unduplciateTags
		qsOutput = QString::fromLatin1("<unduplicate>%1</unduplicate>").arg(qsOutput);

		QXmlStreamReader r(qsOutput);
		qsOutput = QString();
		QXmlStreamWriter w(&qsOutput);
		changed = XMLTools::unduplicateTags(r, w);
		qsOutput = qsOutput.trimmed();
	} while (changed);

	qptePlainText->setPlainText(qsOutput);
}

void RichTextEditor::setText(const QString &txt, bool readonly) {
	qtbToolBar->setEnabled(! readonly && g.bAllowHTML);
	qtbToolBar->setVisible(! readonly && g.bAllowHTML);
	qptePlainText->setReadOnly(readonly || ! g.bAllowHTML);
	qteRichText->setReadOnly(readonly);

	qteRichText->setHtml(txt);
	qptePlainText->setPlainText(txt);

	bChanged = false;
	bModified = false;
	bReadOnly = readonly;
}

QString RichTextEditor::text() {
	if (bChanged) {
		if (currentIndex() == 0)
			richToPlain();
		else
			qteRichText->setHtml(qptePlainText->toPlainText());
	}

	bChanged = false;
	return qptePlainText->toPlainText();
}

bool RichTextEditor::eventFilter(QObject *obj, QEvent *evt) {
	if (obj != qptePlainText && obj != qteRichText)
		return false;
	if (evt->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
		if (((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) &&
		        (keyEvent->modifiers() == Qt::ControlModifier)) {
			emit accept();
			return true;
		}
	}
	return false;
}

bool RichTextImage::isValidImage(const QByteArray &ba, QByteArray &fmt) {
	QBuffer qb;
	qb.setData(ba);
	if (!qb.open(QIODevice::ReadOnly)) {
		return false;
	}

	QByteArray detectedFormat = QImageReader::imageFormat(&qb).toLower();
	if (detectedFormat == QByteArray("png") || detectedFormat == QByteArray("jpg")
            || detectedFormat == QByteArray("jpeg") || detectedFormat == QByteArray("gif")) {
		fmt = detectedFormat;
		return true;
	}

	return false;
}
