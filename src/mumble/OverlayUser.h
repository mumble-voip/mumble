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

#ifndef MUMBLE_MUMBLE_OVERLAYUSER_H_
#define MUMBLE_MUMBLE_OVERLAYUSER_H_

#include <QtCore/QtGlobal>

#include "Overlay.h"

class OverlayUser : public OverlayGroup {
	private:
		Q_DISABLE_COPY(OverlayUser)
	public:
		enum { Type = UserType + 1 };
	protected:
		QGraphicsPixmapItem *qgpiMuted, *qgpiDeafened;
		QGraphicsPixmapItem *qgpiAvatar;
		QGraphicsPixmapItem *qgpiName[4];
		QGraphicsPixmapItem *qgpiChannel;
		QGraphicsPathItem *qgpiBox;

		OverlaySettings *os;

		unsigned int uiSize;
		ClientUser *cuUser;
		Settings::TalkState tsColor;

		QString qsName;
		QString qsChannelName;
		QByteArray qbaAvatar;

		void setup();

	public:
		OverlayUser(ClientUser *cu, unsigned int uiSize, OverlaySettings *osptr);
		OverlayUser(Settings::TalkState ts, unsigned int uiSize, OverlaySettings *osptr);
		void updateUser();
		void updateLayout();

		int type() const;
		static QRectF scaledRect(const QRectF &qr, qreal scale);
		static QPointF alignedPosition(const QRectF &box, const QRectF &item, Qt::Alignment a);
};

#endif
