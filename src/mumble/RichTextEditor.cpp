/* copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "mumble_pch.hpp"

#include "RichTextEditor.h"

#include "Global.h"
#include "Log.h"
#include "MainWindow.h"

RichTextHtmlEdit::RichTextHtmlEdit(QWidget *p) : QTextEdit(p) {
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
	foreach(const QString &format, source->formats())
		qWarning() << format << decodeMimeString(source->data(format));
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
			uri = urls.at(0);
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
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

	const QString &text = qptePlainText->toPlainText();

	bool over = true;

	unsigned int imagelength = text.length();


	if (g.uiMessageLength && imagelength <= g.uiMessageLength) {
		over = false;
	} else if (g.uiImageLength && imagelength > g.uiImageLength) {
		over = true;
	} else {
		QString qsOut;
		QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(text));
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

/* Recursively parse and output XHTML.
 * This will drop <head>, <html> etc, take the contents of <body> and strip out unnecesarry styles.
 * It will also change <span style=""> into matching <b>, <i> or <u>.
 */

static void recurseParse(QXmlStreamReader &reader, QXmlStreamWriter &writer, int &paragraphs, const QMap<QString, QString> &opstyle = QMap<QString, QString>(), const int close = 0, bool ignore = true) {
	while (! reader.atEnd()) {
		QXmlStreamReader::TokenType tt = reader.readNext();

		QXmlStreamAttributes a = reader.attributes();
		QMap<QString, QString> style;
		QMap<QString, QString> pstyle = opstyle;

		QStringRef styleref = a.value(QLatin1String("style"));
		if (!styleref.isNull()) {
			QString stylestring = styleref.toString();
			QStringList styles = stylestring.split(QLatin1String(";"), QString::SkipEmptyParts);
			foreach(QString s, styles) {
				s = s.simplified();
				int idx = s.indexOf(QLatin1Char(':'));
				QString key = (idx > 0) ? s.left(idx).simplified() : s;
				QString val = (idx > 0) ? s.mid(idx+1).simplified() : QString();

				if (! pstyle.contains(key) || (pstyle.value(key) != val)) {
					style.insert(key,val);
					pstyle.insert(key,val);
				}
			}
		}

		switch (tt) {
			case QXmlStreamReader::StartElement: {
					QString name = reader.name().toString();
					int rclose = 1;
					if (name == QLatin1String("body")) {
						rclose = 0;
						ignore = false;
					} else if (name == QLatin1String("span")) {
						// Substitute style with <b>, <i> and <u>

						rclose = 0;
						if (style.value(QLatin1String("font-weight")) == QLatin1String("600")) {
							writer.writeStartElement(QLatin1String("b"));
							rclose++;
							style.remove(QLatin1String("font-weight"));
						}
						if (style.value(QLatin1String("font-style")) == QLatin1String("italic")) {
							writer.writeStartElement(QLatin1String("i"));
							rclose++;
							style.remove(QLatin1String("font-style"));
						}
						if (style.value(QLatin1String("text-decoration")) == QLatin1String("underline")) {
							writer.writeStartElement(QLatin1String("u"));
							rclose++;
							style.remove(QLatin1String("text-decoration"));
						}
						if (! style.isEmpty()) {
							rclose++;
							writer.writeStartElement(name);

							QStringList qsl;
							QMap<QString, QString>::const_iterator i;
							for (i=style.constBegin(); i != style.constEnd(); ++i) {
								if (! i.value().isEmpty())
									qsl << QString::fromLatin1("%1:%2").arg(i.key(), i.value());
								else
									qsl << i.key();
							}

							writer.writeAttribute(QLatin1String("style"), qsl.join(QLatin1String("; ")));
						}
					} else if (name == QLatin1String("p")) {
						paragraphs++;
						if (paragraphs == 1) {
							// Ignore first paragraph. If it is styled empty drop its contents (e.g. <br />) too.
							if (style.value(QLatin1String("-qt-paragraph-type")) == QLatin1String("empty")) {
								reader.skipCurrentElement();
								continue;
							}
							rclose = 0;
						}
						else {
							rclose = 1;
							writer.writeStartElement(name);

							if (! style.isEmpty()) {
								QStringList qsl;
								QMap<QString, QString>::const_iterator i;
								for (i=style.constBegin(); i != style.constEnd(); ++i) {
									if (! i.value().isEmpty())
										qsl << QString::fromLatin1("%1:%2").arg(i.key(), i.value());
									else
										qsl << i.key();
								}

								writer.writeAttribute(QLatin1String("style"), qsl.join(QLatin1String("; ")));
							}
						}
					} else if (name == QLatin1String("a")) {
						// Set pstyle to include implicit blue underline.
						rclose = 1;
						writer.writeCurrentToken(reader);
						pstyle.insert(QLatin1String("text-decoration"), QLatin1String("underline"));
						pstyle.insert(QLatin1String("color"), QLatin1String("#0000ff"));
					} else if (! ignore) {
						rclose = 1;
						writer.writeCurrentToken(reader);
					}

					recurseParse(reader, writer, paragraphs, pstyle, rclose, ignore);
					break;
				}
			case QXmlStreamReader::EndElement:
				if (!ignore)
					for (int i=0;i<close;++i)
						writer.writeEndElement();
				return;
			case QXmlStreamReader::Characters:
				if (! ignore)
					writer.writeCharacters(reader.text().toString());
				break;
			default:
				break;
		}
	}
}

/* Iterate XML and remove close-followed-by-open.
 * For example, make "<b>bold with </b><b><i>italic</i></b>" into
 * "<b>bold with <i>italic</i></b>"
 */

static bool unduplicateTags(QXmlStreamReader &reader, QXmlStreamWriter &writer) {
	bool changed = false;
	bool needclose = false;

	QStringList qslConcat;
	qslConcat << QLatin1String("b");
	qslConcat << QLatin1String("i");
	qslConcat << QLatin1String("u");
	qslConcat << QLatin1String("a");

	QList<QString> qlNames;
	QList<QXmlStreamAttributes> qlAttributes;

	while (! reader.atEnd()) {
		QXmlStreamReader::TokenType tt = reader.readNext();
		QString name = reader.name().toString();
		switch (tt) {
			case QXmlStreamReader::StartDocument:
			case QXmlStreamReader::EndDocument:
				break;
			case QXmlStreamReader::StartElement: {
					QXmlStreamAttributes a = reader.attributes();

					if (name == QLatin1String("unduplicate"))
						break;

					if (needclose) {
						needclose = false;

						if ((a == qlAttributes.last()) && (name == qlNames.last()) && (qslConcat.contains(name))) {
							changed = true;
							break;
						}
						qlNames.takeLast();
						qlAttributes.takeLast();
						writer.writeEndElement();
					}
					writer.writeCurrentToken(reader);
					qlNames.append(name);
					qlAttributes.append(a);
				}
				break;
			case QXmlStreamReader::EndElement: {
					if (name == QLatin1String("unduplicate"))
						break;
					if (needclose) {
						qlNames.takeLast();
						qlAttributes.takeLast();
						writer.writeCurrentToken(reader);
					} else {
						needclose = true;
					}
					needclose = true;
				}
				break;
			default:
				if (needclose) {
					writer.writeEndElement();
					needclose = false;
				}
				writer.writeCurrentToken(reader);
		}
	}
	if (needclose)
		writer.writeEndElement();
	return changed;
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

	recurseParse(reader, writer, paragraphs, def);

	qsOutput = qsOutput.trimmed();

	bool changed;
	do {
		qsOutput = QString::fromLatin1("<unduplicate>%1</unduplicate>").arg(qsOutput);

		QXmlStreamReader r(qsOutput);
		qsOutput = QString();
		QXmlStreamWriter w(&qsOutput);
		changed = unduplicateTags(r, w);
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
