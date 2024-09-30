// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_XMLTOOLS_H_
#define MUMBLE_MUMBLE_XMLTOOLS_H_

#include <QMap>
#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class XMLTools : public QObject {
	Q_OBJECT
public:
	/* Recursively parse and output XHTML.
	 * This will drop <head>, <html> etc, take the contents of <body> and strip out unnecessary styles.
	 * It will also change <span style=""> into matching <b>, <i> or <u>.
	 */
	static void recurseParse(QXmlStreamReader &reader, QXmlStreamWriter &writer, int &paragraphs,
							 const QMap< QString, QString > &opstyle, const int close = 0, bool ignore = true);

	/* Iterate XML and remove close-followed-by-open.
	 * For example, make "<b>bold with </b><b><i>italic</i></b>" into
	 * "<b>bold with <i>italic</i></b>"
	 *
	 * If the input XML is or may not be valid, a "unduplicate" tag can be used as a root element,
	 * which will be dropped and not written to writer.
	 *
	 * Input XML has to be valid XML.
	 *
	 * Works on b, i, u, and a elements.
	 */
	static bool unduplicateTags(QXmlStreamReader &reader, QXmlStreamWriter &writer);
};

#endif // XMLTOOLS_H
