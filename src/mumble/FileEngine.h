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

#ifndef _FILEENGINE_H
#define _FILEENGINE_H

#include "mumble_pch.hpp"

class MumbleFileEngineHandler : public QAbstractFileEngineHandler {
	private:
		Q_DISABLE_COPY(MumbleFileEngineHandler)
	public:
		MumbleFileEngineHandler();
		QAbstractFileEngine *create(const QString &) const;
};

class MumbleImageFileEngine : public QAbstractFileEngine {
	private:
		Q_DISABLE_COPY(MumbleImageFileEngine)
	protected:
		QBuffer qbData;
		QUrl quUrl;
		QStringList qslPath;
	public:
		MumbleImageFileEngine(const QUrl &);
		bool open(QIODevice::OpenMode);
		bool close();
		bool seek(qint64 offset);
		qint64 pos() const;
		qint64 size() const;
		qint64 read(char *data, qint64 maxlen);
		QString fileName(QAbstractFileEngine::FileName) const;
};

#endif
