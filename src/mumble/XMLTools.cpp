// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "XMLTools.h"

#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

void XMLTools::recurseParse(QXmlStreamReader &reader, QXmlStreamWriter &writer, int &paragraphs,
							const QMap< QString, QString > &opstyle, const int close, bool ignore) {
	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType tt = reader.readNext();

		QXmlStreamAttributes a = reader.attributes();
		QMap< QString, QString > style;
		QMap< QString, QString > pstyle = opstyle;

		const auto styleref = a.value(QLatin1String("style"));
		if (!styleref.isNull()) {
			QString stylestring = styleref.toString();
			QStringList styles  = stylestring.split(QLatin1String(";"), Qt::SkipEmptyParts);
			foreach (QString s, styles) {
				s                 = s.simplified();
				const auto idx    = s.indexOf(QLatin1Char(':'));
				const QString key = (idx > 0) ? s.left(idx).simplified() : s;
				const QString val = (idx > 0) ? s.mid(idx + 1).simplified() : QString();

				if (!pstyle.contains(key) || (pstyle.value(key) != val)) {
					style.insert(key, val);
					pstyle.insert(key, val);
				}
			}
		}

		switch (tt) {
			case QXmlStreamReader::StartElement: {
				QString name = reader.name().toString();
				int rclose   = 1;
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
					if (!style.isEmpty()) {
						rclose++;
						writer.writeStartElement(name);

						QStringList qsl;
						QMap< QString, QString >::const_iterator i;
						for (i = style.constBegin(); i != style.constEnd(); ++i) {
							if (!i.value().isEmpty())
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
					} else {
						rclose = 1;
						writer.writeStartElement(name);

						if (!style.isEmpty()) {
							QStringList qsl;
							QMap< QString, QString >::const_iterator i;
							for (i = style.constBegin(); i != style.constEnd(); ++i) {
								if (!i.value().isEmpty())
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
				} else if (!ignore) {
					rclose = 1;
					writer.writeCurrentToken(reader);
				}

				recurseParse(reader, writer, paragraphs, pstyle, rclose, ignore);
				break;
			}
			case QXmlStreamReader::EndElement:
				if (!ignore)
					for (int i = 0; i < close; ++i)
						writer.writeEndElement();
				return;
			case QXmlStreamReader::Characters:
				if (!ignore)
					writer.writeCharacters(reader.text().toString());
				break;
			default:
				break;
		}
	}
}

bool XMLTools::unduplicateTags(QXmlStreamReader &reader, QXmlStreamWriter &writer) {
	bool changed   = false;
	bool needclose = false;

	QStringList qslConcat;
	qslConcat << QLatin1String("b");
	qslConcat << QLatin1String("i");
	qslConcat << QLatin1String("u");
	qslConcat << QLatin1String("a");

	QList< QString > qlNames;
	QList< QXmlStreamAttributes > qlAttributes;

	while (!reader.atEnd()) {
		QXmlStreamReader::TokenType tt = reader.readNext();
		QString name                   = reader.name().toString();
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
			} break;
			case QXmlStreamReader::EndElement: {
				if (name == QLatin1String("unduplicate"))
					break;
				if (needclose) {
					qlNames.takeLast();
					qlAttributes.takeLast();
					writer.writeCurrentToken(reader);
				}
				needclose = true;
			} break;
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
