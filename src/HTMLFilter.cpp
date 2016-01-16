/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2016, Mikkel Krautz <mikkel@krautz.dk>

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

#include "murmur_pch.h"

#include "HTMLFilter.h"

QString HTMLFilter::escapeTags(const QString &in) {
	QString out;
	for (int i = 0; i < in.size(); i++) {
		if (in.at(i) == QLatin1Char('<')) {
			out += QLatin1String("&lt;");
		} else if (in.at(i) == QLatin1Char('>')) {
			out += QLatin1String("&gt;");
		} else {
			out += in.at(i);
		}
	}
	return out;
}

bool HTMLFilter::filter(const QString &in, QString &out) {
	if (! in.contains(QLatin1Char('<'))) {
		out = in.simplified();
	} else {
		QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(in));
		QString qs;
		while (! qxsr.atEnd()) {
			switch (qxsr.readNext()) {
				case QXmlStreamReader::Invalid:
					return false;
				case QXmlStreamReader::Characters:
					qs += qxsr.text();
					break;
				case QXmlStreamReader::EndElement:
					if ((qxsr.name() == QLatin1String("br")) || (qxsr.name() == QLatin1String("p")))
						qs += QLatin1Char('\n');
					break;
				default:
					break;
			}
		}
		out = escapeTags(qs.simplified());
	}
	return true;
}
