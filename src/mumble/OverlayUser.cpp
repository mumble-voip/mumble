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

#include "mumble_pch.hpp"

#include "OverlayUser.h"

#include "OverlayText.h"
#include "User.h"
#include "Channel.h"
#include "ClientUser.h"
#include "Global.h"
#include "Message.h"
#include "Database.h"
#include "NetworkConfig.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "GlobalShortcut.h"

OverlayUser::OverlayUser(ClientUser *cu, unsigned int height, OverlaySettings *osptr) : OverlayGroup(), os(osptr), uiSize(height), cuUser(cu), tsColor(Settings::Passive) {
	setup();
	updateLayout();
}

OverlayUser::OverlayUser(Settings::TalkState ts, unsigned int height, OverlaySettings *osptr) : OverlayGroup(), os(osptr), uiSize(height), cuUser(NULL), tsColor(ts) {
	qsChannelName = Overlay::tr("Channel");

	setup();
	updateLayout();
}

int OverlayUser::type() const {
	return Type;
}

void OverlayUser::setup() {
	qgpiMuted = new QGraphicsPixmapItem(this);
	qgpiMuted->hide();

	qgpiDeafened = new QGraphicsPixmapItem(this);
	qgpiDeafened->hide();

	qgpiAvatar = new QGraphicsPixmapItem(this);

	for (int i=0;i<4;++i) {
		qgpiName[i] = new QGraphicsPixmapItem(this);
		qgpiName[i]->hide();
	}

	qgpiChannel = new QGraphicsPixmapItem(this);
	qgpiChannel->hide();

	qgpiBox = new QGraphicsPathItem(this);
	qgpiBox->hide();
}

#undef SCALESIZE
#define SCALESIZE(var) iroundf(uiSize * os->fZoom * os->qrf##var .width() + 0.5f), iroundf(uiSize * os->fZoom * os->qrf##var .height() + 0.5f)

void OverlayUser::updateLayout() {
	QPixmap pm;

	if (scene())
		uiSize = iroundf(scene()->sceneRect().height() + 0.5);

	prepareGeometryChange();

	for (int i=0;i<4;++i)
		qgpiName[i]->setPixmap(pm);

	qgpiAvatar->setPixmap(pm);
	qgpiChannel->setPixmap(pm);

	{
		QImageReader qir(QLatin1String("skin:muted_self.svg"));
		QSize sz = qir.size();
		sz.scale(SCALESIZE(MutedDeafened), Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiMuted->setPixmap(QPixmap::fromImage(qir.read()));
	}

	{
		QImageReader qir(QLatin1String("skin:deafened_self.svg"));
		QSize sz = qir.size();
		sz.scale(SCALESIZE(MutedDeafened), Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiDeafened->setPixmap(QPixmap::fromImage(qir.read()));
	}

	qgpiMuted->setPos(alignedPosition(scaledRect(os->qrfMutedDeafened, uiSize * os->fZoom), qgpiMuted->boundingRect(), os->qaMutedDeafened));
	qgpiMuted->setZValue(1.0f);
	qgpiMuted->setOpacity(os->fMutedDeafened);

	qgpiDeafened->setPos(alignedPosition(scaledRect(os->qrfMutedDeafened, uiSize * os->fZoom), qgpiDeafened->boundingRect(), os->qaMutedDeafened));
	qgpiDeafened->setZValue(1.0f);
	qgpiDeafened->setOpacity(os->fMutedDeafened);

	qgpiAvatar->setPos(0.0f, 0.0f);
	qgpiAvatar->setOpacity(os->fAvatar);

	for (int i=0;i<4;++i) {
		qgpiName[i]->setPos(0.0f, 0.0f);
		qgpiName[i]->setZValue(2.0f);
		qgpiName[i]->setOpacity(os->fUserName);
	}
	qgpiChannel->setPos(0.0f, 0.0f);
	qgpiChannel->setZValue(3.0f);
	qgpiChannel->setOpacity(os->fChannel);

	QRectF children = os->qrfAvatar | os->qrfChannel | os->qrfMutedDeafened | os->qrfUserName;

	bool haspen = (os->qcBoxPen != os->qcBoxFill) && (! qFuzzyCompare(os->qcBoxPen.alphaF(), static_cast<qreal>(0.0f)));
	qreal pw = haspen ? qMax<qreal>(1.0f, os->fBoxPenWidth * uiSize * os->fZoom) : 0.0f;
	qreal pad = os->fBoxPad * uiSize * os->fZoom;
	QPainterPath pp;
	pp.addRoundedRect(children.x() * uiSize * os->fZoom + -pw / 2.0f - pad, children.y() * uiSize * os->fZoom + -pw / 2.0f - pad, children.width() * uiSize * os->fZoom + pw + 2.0f * pad, children.height() * uiSize * os->fZoom + pw + 2.0f * pad, 2.0f * pw, 2.0f * pw);
	qgpiBox->setPath(pp);
	qgpiBox->setPos(0.0f, 0.0f);
	qgpiBox->setZValue(-1.0f);
	qgpiBox->setPen(haspen ? QPen(os->qcBoxPen, pw) : Qt::NoPen);
	qgpiBox->setBrush(qFuzzyCompare(os->qcBoxFill.alphaF(), static_cast<qreal>(0.0f)) ? Qt::NoBrush : os->qcBoxFill);
	qgpiBox->setOpacity(1.0f);

	if (! cuUser) {
		switch (tsColor) {
			case Settings::Passive:
				qsName = Overlay::tr("Silent");
				break;
			case Settings::Talking:
				qsName = Overlay::tr("Talking");
				break;
			case Settings::Whispering:
				qsName = Overlay::tr("Whisper");
				break;
			case Settings::Shouting:
				qsName = Overlay::tr("Shout");
				break;
		}
	}
}

void OverlayUser::updateUser() {
	if (os->bUserName && (qgpiName[0]->pixmap().isNull() || (cuUser && (qsName != cuUser->qsName)))) {
		if (cuUser)
			qsName = cuUser->qsName;

		OverlayTextLine tl(qsName, os->qfUserName);
		for (int i=0; i<4; ++i) {
			const QPixmap &pm = tl.createPixmap(SCALESIZE(UserName), os->qcUserName[i]);
			qgpiName[i]->setPixmap(pm);

			if (i == 0)
				qgpiName[0]->setPos(alignedPosition(scaledRect(os->qrfUserName, uiSize * os->fZoom), qgpiName[0]->boundingRect(), os->qaUserName));
			else
				qgpiName[i]->setPos(qgpiName[0]->pos());
		}
	}

	if (os->bChannel && (qgpiChannel->pixmap().isNull() || (cuUser && (qsChannelName != cuUser->cChannel->qsName)))) {
		if (cuUser)
			qsChannelName = cuUser->cChannel->qsName;

		const QPixmap &pm = OverlayTextLine(qsChannelName, os->qfChannel).createPixmap(SCALESIZE(Channel), os->qcChannel);
		qgpiChannel->setPixmap(pm);
		qgpiChannel->setPos(alignedPosition(scaledRect(os->qrfChannel, uiSize * os->fZoom), qgpiChannel->boundingRect(), os->qaChannel));
	}

	if (os->bAvatar && (qgpiAvatar->pixmap().isNull() || (cuUser && (qbaAvatar != cuUser->qbaTextureHash)))) {
		if (cuUser)
			qbaAvatar = cuUser->qbaTextureHash;

		QImage img;

		if (! qbaAvatar.isNull() && cuUser->qbaTexture.isEmpty()) {
			g.o->requestTexture(cuUser);
		} else if (qbaAvatar.isNull()) {
			QImageReader qir(QLatin1String("skin:default_avatar.svg"));
			QSize sz = qir.size();
			sz.scale(SCALESIZE(Avatar), Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		} else {
			QBuffer qb(& cuUser->qbaTexture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir(&qb, cuUser->qbaTextureFormat);
			QSize sz = qir.size();
			sz.scale(SCALESIZE(Avatar), Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		}

		qgpiAvatar->setPixmap(QPixmap::fromImage(img));
		qgpiAvatar->setPos(alignedPosition(scaledRect(os->qrfAvatar, uiSize * os->fZoom), qgpiAvatar->boundingRect(), os->qaAvatar));
	}

	qgpiAvatar->setVisible(os->bAvatar);

	if (cuUser) {
		ClientUser *self = ClientUser::get(g.uiSession);

		if (os->bMutedDeafened) {
			if (cuUser->bDeaf || cuUser->bSelfDeaf) {
				qgpiMuted->hide();
				qgpiDeafened->show();
			} else if (cuUser->bMute || cuUser->bSelfMute || cuUser->bLocalMute || cuUser->bSuppress) {
				qgpiMuted->show();
				qgpiDeafened->hide();
			} else {
				qgpiMuted->hide();
				qgpiDeafened->hide();
			}
		} else {
			qgpiMuted->hide();
			qgpiDeafened->hide();
		}

		bool samechannel = self && (self->cChannel == cuUser->cChannel);
		qgpiChannel->setVisible(os->bChannel && ! samechannel);

		tsColor = cuUser->tsState;
	} else {
		qgpiChannel->setVisible(os->bChannel && (tsColor != Settings::Passive) && (tsColor != Settings::Talking));
		qgpiMuted->setVisible(os->bChannel);
		qgpiDeafened->hide();
	}

	if (os->bUserName)
		for (int i=0;i<4;++i)
			qgpiName[i]->setVisible(i == tsColor);
	else
		for (int i=0;i<4;++i)
			qgpiName[i]->setVisible(false);

	qgpiBox->setVisible(os->bBox);

	setOpacity(os->fUser[tsColor]);
}

QRectF OverlayUser::scaledRect(const QRectF &qr, qreal scale) {
	return QRectF(qr.x() * scale, qr.y() * scale, qr.width() * scale, qr.height() * scale);
}

QPointF OverlayUser::alignedPosition(const QRectF &box, const QRectF &item, Qt::Alignment a) {
	qreal boxw = box.width();
	qreal boxh = box.height();

	qreal itemw = item.width();
	qreal itemh = item.height();

	qreal wdiff = boxw - itemw;
	qreal hdiff = boxh - itemh;

	qreal xofs = box.x() - item.x();
	qreal yofs = box.y() - item.y();

	if (a & Qt::AlignRight)
		xofs += wdiff;
	else if (a & Qt::AlignHCenter)
		xofs += wdiff * 0.5f;

	if (a & Qt::AlignBottom)
		yofs += hdiff;
	else if (a & Qt::AlignVCenter)
		yofs += hdiff * 0.5f;

	return QPointF(iroundf(xofs + 0.5f), iroundf(yofs + 0.5f));
}

