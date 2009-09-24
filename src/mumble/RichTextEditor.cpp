/* copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "RichTextEditor.h"

RichTextEditor::RichTextEditor(QWidget *p) : QTabWidget(p) {
	bChanged = false;

	setupUi(this);
	
	qtbToolBar->addAction(qaBold);
	qtbToolBar->addAction(qaItalic);
	qtbToolBar->addAction(qaUnderline);
	
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
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
	bChanged = true;
}

void RichTextEditor::on_qteRichText_textChanged() {
	bChanged = true;
}

/* Recursively parse and output XHTML.
 * This will drop <head>, <html> etc, take the contents of <body> and strip out unnecesarry styles.
 * It will also change <span style=""> into matching <b>, <i> or <u>.
 */

static void recurseParse(QXmlStreamReader &reader, QXmlStreamWriter &writer, int &paragraphs, const QMap<QString, QString> &opstyle = QMap<QString, QString>(), int close = 0, bool ignore = true) {
	while(! reader.atEnd()) {
		QXmlStreamReader::TokenType tt = reader.readNext();

		QXmlStreamAttributes a = reader.attributes();
		QMap<QString, QString> style;
		QMap<QString, QString> pstyle = opstyle;

		if (a.hasAttribute(QLatin1String("style"))) {
			QString stylestring = a.value(QLatin1String("style")).toString();
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

		switch(tt) {
			case QXmlStreamReader::StartElement:
				{
					QString name = reader.name().toString();
					if (name == QLatin1String("body")) {
						close = 0;
						ignore = false;
					} else if (name == QLatin1String("span")) {
						close = 0;
						if (style.value(QLatin1String("font-weight")) == QLatin1String("600")) {
							writer.writeStartElement(QLatin1String("b"));
							close++;
							style.remove(QLatin1String("font-weight"));
						}
						if (style.value(QLatin1String("font-style")) == QLatin1String("italic")) {
							writer.writeStartElement(QLatin1String("i"));
							close++;
							style.remove(QLatin1String("font-style"));
						}
						if (style.value(QLatin1String("text-decoration")) == QLatin1String("underline")) {
							writer.writeStartElement(QLatin1String("u"));
							close++;
							style.remove(QLatin1String("text-decoration"));
						}
						if (! style.isEmpty()) {
							close++;
							writer.writeStartElement(name);
							
							QStringList qsl;
							QMap<QString, QString>::const_iterator i;
							for(i=style.constBegin(); i != style.constEnd(); ++i) {
								if (! i.value().isEmpty())
									qsl << QString::fromLatin1("%1:%2").arg(i.key(), i.value());
								else
									qsl << i.key();
							}
							
							writer.writeAttribute(QLatin1String("style"), qsl.join(QLatin1String("; ")));
						}
					} else if (name == QLatin1String("p")) {
						paragraphs++;
						if (paragraphs > 1) {
							close = 1;
							writer.writeStartElement(name);

							if (! style.isEmpty()) {
								QStringList qsl;
								QMap<QString, QString>::const_iterator i;
								for(i=style.constBegin(); i != style.constEnd(); ++i) {
									if (! i.value().isEmpty())
										qsl << QString::fromLatin1("%1:%2").arg(i.key(), i.value());
									else
										qsl << i.key();
								}

								writer.writeAttribute(QLatin1String("style"), qsl.join(QLatin1String("; ")));
							}
						} else {
							close = 0;
						}
					} else if (! ignore) {
						close = 1;
						writer.writeStartElement(name);
						writer.writeAttributes(reader.attributes());
					}
					qWarning() << reader.name();

					recurseParse(reader, writer, paragraphs, pstyle, close, ignore);
					break;
				}
			case QXmlStreamReader::EndElement:
				if (!ignore)
					for(int i=0;i<close;++i)
						writer.writeEndElement();
				return;
			case QXmlStreamReader::Characters:
				if (! ignore)
					writer.writeCharacters(reader.text().toString());
				break;
		}
	}
}

void RichTextEditor::richToPlain() {
	qWarning() << qteRichText->toHtml();
	
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
	
	qWarning() << qsOutput;

	qptePlainText->setPlainText(qsOutput);
}

void RichTextEditor::setText(const QString &txt) {
	qptePlainText->setPlainText(txt);

	if (Qt::mightBeRichText(txt))
		qteRichText->setHtml(txt);
	else
		qteRichText->setPlainText(txt);
		
	bChanged = false;
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
