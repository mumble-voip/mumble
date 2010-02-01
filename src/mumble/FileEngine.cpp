/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "FileEngine.h"
#include "Global.h"
#include "ClientUser.h"

MumbleFileEngineHandler::MumbleFileEngineHandler() : QAbstractFileEngineHandler() {
}

QAbstractFileEngine *MumbleFileEngineHandler::create(const QString &name) const {
	QUrl url(name);

	if (url.scheme() == QLatin1String("memoryblob"))
		return new MumbleImageFileEngine(url);

	return NULL;
}

MumbleImageFileEngine::MumbleImageFileEngine(const QUrl &url) : QAbstractFileEngine(), quUrl(url) {
	const QString &domain = url.host();
	qslPath = url.path().split(QLatin1Char('/'), QString::SkipEmptyParts);

	if (domain == QLatin1String("avatar") && (qslPath.size() == 2)) {

		unsigned int session = qslPath.first().toUInt();
		ClientUser *cu = ClientUser::get(session);

		if (cu)
			qbData.setData(cu->qbaTexture);
	}
}

bool MumbleImageFileEngine::open(QIODevice::OpenMode om) {
	return qbData.open(om);
}

bool MumbleImageFileEngine::close() {
	qbData.close();
	return true;
}

bool MumbleImageFileEngine::seek(qint64 offset) {
	return qbData.seek(offset);
}

qint64 MumbleImageFileEngine::size() const {
	return qbData.size();
}

qint64 MumbleImageFileEngine::pos() const {
	return qbData.pos();
}

qint64 MumbleImageFileEngine::read(char *data, qint64 maxlen) {
	return qbData.read(data, maxlen);
}

QString MumbleImageFileEngine::fileName(QAbstractFileEngine::FileName fn) const {
	switch (fn) {
		case QAbstractFileEngine::BaseName:
			return qslPath.last();
		default:
			return quUrl.toString();
	}
}
