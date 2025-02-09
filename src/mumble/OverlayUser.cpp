// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayUser.h"

#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "MainWindow.h"
#include "NetworkConfig.h"
#include "OverlayText.h"
#include "ServerHandler.h"
#include "User.h"
#include "Utils.h"
#include "Global.h"
#include "GlobalShortcut.h"

#include <QtGui/QImageReader>

OverlayUser::OverlayUser(ClientUser *cu, unsigned int height, OverlaySettings *osptr)
	: OverlayGroup(), os(osptr), uiSize(height), cuUser(cu), tsColor(Settings::Passive) {
	setup();
	updateLayout();
}

OverlayUser::OverlayUser(Settings::TalkState ts, unsigned int height, OverlaySettings *osptr)
	: OverlayGroup(), os(osptr), uiSize(height), cuUser(nullptr), tsColor(ts) {
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

	for (int i = 0; i < 4; ++i) {
		qgpiName[i] = new QGraphicsPixmapItem(this);
		qgpiName[i]->hide();
	}

	qgpiChannel = new QGraphicsPixmapItem(this);
	qgpiChannel->hide();

	qgpiBox = new QGraphicsPathItem(this);
	qgpiBox->hide();
}

template< typename T > int roundToInt(T value) {
	return static_cast< int >(value + 0.5f);
}

template< typename T > unsigned int roundToUInt(T value) {
	return static_cast< unsigned int >(value + 0.5f);
}

void OverlayUser::updateLayout() {
	QPixmap pm;

	const double scaleFactor = uiSize * os->fZoom;

	if (scene())
		uiSize = static_cast< unsigned int >(scene()->sceneRect().height() + 0.5);

	prepareGeometryChange();

	for (int i = 0; i < 4; ++i)
		qgpiName[i]->setPixmap(pm);

	qgpiAvatar->setPixmap(pm);
	qgpiChannel->setPixmap(pm);

	{
		QImageReader qir(QLatin1String("skin:muted_self.svg"));
		QSize sz = qir.size();
		sz.scale(roundToInt(os->qrfMutedDeafened.width() * scaleFactor),
				 roundToInt(os->qrfMutedDeafened.height() * scaleFactor), Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiMuted->setPixmap(QPixmap::fromImage(qir.read()));
	}

	{
		QImageReader qir(QLatin1String("skin:deafened_self.svg"));
		QSize sz = qir.size();
		sz.scale(roundToInt(os->qrfMutedDeafened.width() * scaleFactor),
				 roundToInt(os->qrfMutedDeafened.height() * scaleFactor), Qt::KeepAspectRatio);
		qir.setScaledSize(sz);
		qgpiDeafened->setPixmap(QPixmap::fromImage(qir.read()));
	}

	qgpiMuted->setPos(alignedPosition(scaledRect(os->qrfMutedDeafened, uiSize * os->fZoom), qgpiMuted->boundingRect(),
									  os->qaMutedDeafened));
	qgpiMuted->setZValue(1.0f);
	qgpiMuted->setOpacity(os->fMutedDeafened);

	qgpiDeafened->setPos(alignedPosition(scaledRect(os->qrfMutedDeafened, uiSize * os->fZoom),
										 qgpiDeafened->boundingRect(), os->qaMutedDeafened));
	qgpiDeafened->setZValue(1.0f);
	qgpiDeafened->setOpacity(os->fMutedDeafened);

	qgpiAvatar->setPos(0.0f, 0.0f);
	qgpiAvatar->setOpacity(os->fAvatar);

	for (int i = 0; i < 4; ++i) {
		qgpiName[i]->setPos(0.0f, 0.0f);
		qgpiName[i]->setZValue(2.0f);
		qgpiName[i]->setOpacity(os->fUserName);
	}
	qgpiChannel->setPos(0.0f, 0.0f);
	qgpiChannel->setZValue(3.0f);
	qgpiChannel->setOpacity(os->fChannel);

	const QRectF childrenBounds = os->qrfAvatar | os->qrfChannel | os->qrfMutedDeafened | os->qrfUserName;

	const bool haspen =
		(os->qcBoxPen != os->qcBoxFill) && (!qFuzzyCompare(static_cast< float >(os->qcBoxPen.alphaF()), 0.0f));
	const qreal pw  = haspen ? qMax< qreal >(1.0f, os->fBoxPenWidth * uiSize * os->fZoom) : 0.0f;
	const qreal pad = os->fBoxPad * uiSize * os->fZoom;
	QPainterPath pp;
	pp.addRoundedRect(childrenBounds.x() * uiSize * os->fZoom + -pw / 2.0f - pad,
					  childrenBounds.y() * uiSize * os->fZoom + -pw / 2.0f - pad,
					  childrenBounds.width() * uiSize * os->fZoom + pw + 2.0f * pad,
					  childrenBounds.height() * uiSize * os->fZoom + pw + 2.0f * pad, 2.0f * pw, 2.0f * pw);
	qgpiBox->setPath(pp);
	qgpiBox->setPos(0.0f, 0.0f);
	qgpiBox->setZValue(-1.0f);
	qgpiBox->setPen(haspen ? QPen(os->qcBoxPen, pw) : Qt::NoPen);
	qgpiBox->setBrush(qFuzzyCompare(static_cast< float >(os->qcBoxFill.alphaF()), 0.0f) ? Qt::NoBrush : os->qcBoxFill);
	qgpiBox->setOpacity(1.0f);

	if (!cuUser) {
		switch (tsColor) {
			case Settings::Passive:
				qsName = Overlay::tr("Silent");
				break;
			case Settings::Talking:
				qsName = Overlay::tr("Talking");
				break;
			case Settings::MutedTalking:
				qsName = QObject::tr("Talking (muted)");
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
	const double scaleFactor = uiSize * os->fZoom;

	if (os->bUserName && (qgpiName[0]->pixmap().isNull() || (cuUser && (qsName != cuUser->qsName)))) {
		if (cuUser)
			qsName = cuUser->qsName;

		OverlayTextLine tl(qsName, os->qfUserName);
		for (unsigned int i = 0; i < 4; ++i) {
			const QPixmap &pm = tl.createPixmap(roundToUInt(os->qrfUserName.width() * scaleFactor),
												roundToUInt(os->qrfUserName.height() * scaleFactor), os->qcUserName[i]);
			qgpiName[i]->setPixmap(pm);

			if (i == 0)
				qgpiName[0]->setPos(alignedPosition(scaledRect(os->qrfUserName, uiSize * os->fZoom),
													qgpiName[0]->boundingRect(), os->qaUserName));
			else
				qgpiName[i]->setPos(qgpiName[0]->pos());
		}
	}

	if (os->bChannel && (qgpiChannel->pixmap().isNull() || (cuUser && (qsChannelName != cuUser->cChannel->qsName)))) {
		if (cuUser)
			qsChannelName = cuUser->cChannel->qsName;

		const QPixmap &pm = OverlayTextLine(qsChannelName, os->qfChannel)
								.createPixmap(roundToUInt(os->qrfChannel.width() * scaleFactor),
											  roundToUInt(os->qrfChannel.height() * scaleFactor), os->qcChannel);
		qgpiChannel->setPixmap(pm);
		qgpiChannel->setPos(alignedPosition(scaledRect(os->qrfChannel, uiSize * os->fZoom), qgpiChannel->boundingRect(),
											os->qaChannel));
	}

	if (os->bAvatar && (qgpiAvatar->pixmap().isNull() || (cuUser && (qbaAvatar != cuUser->qbaTextureHash)))) {
		if (cuUser)
			qbaAvatar = cuUser->qbaTextureHash;

		QImage img;

		if (!qbaAvatar.isNull() && cuUser->qbaTexture.isEmpty()) {
			Global::get().o->requestTexture(cuUser);
		} else if (qbaAvatar.isNull()) {
			QImageReader qir(QLatin1String("skin:default_avatar.svg"));
			QSize sz = qir.size();
			sz.scale(roundToInt(os->qrfAvatar.width() * scaleFactor), roundToInt(os->qrfAvatar.height() * scaleFactor),
					 Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		} else {
			QBuffer qb(&cuUser->qbaTexture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir(&qb, cuUser->qbaTextureFormat);
			QSize sz = qir.size();
			sz.scale(roundToInt(os->qrfAvatar.width() * scaleFactor), roundToInt(os->qrfAvatar.height() * scaleFactor),
					 Qt::KeepAspectRatio);
			qir.setScaledSize(sz);
			img = qir.read();
		}

		qgpiAvatar->setPixmap(QPixmap::fromImage(img));
		qgpiAvatar->setPos(
			alignedPosition(scaledRect(os->qrfAvatar, uiSize * os->fZoom), qgpiAvatar->boundingRect(), os->qaAvatar));
	}

	qgpiAvatar->setVisible(os->bAvatar);

	if (cuUser) {
		ClientUser *self = ClientUser::get(Global::get().uiSession);

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
		qgpiChannel->setVisible(os->bChannel && !samechannel);

		tsColor = cuUser->tsState;
	} else {
		qgpiChannel->setVisible(os->bChannel && (tsColor != Settings::Passive) && (tsColor != Settings::Talking));
		qgpiMuted->setVisible(os->bChannel);
		qgpiDeafened->hide();
	}

	if (os->bUserName)
		for (int i = 0; i < 4; ++i)
			qgpiName[i]->setVisible(i == tsColor);
	else
		for (int i = 0; i < 4; ++i)
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

	return QPointF(static_cast< int >(xofs + 0.5f), static_cast< int >(yofs + 0.5f));
}
