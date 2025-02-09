// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "OverlayText.h"

#include "Utils.h"

#include <QtCore/QDebug>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtGui/QTransform>

BasepointPixmap::BasepointPixmap() : qpBasePoint(0, 0), iAscent(-1), iDescent(-1) {
}

BasepointPixmap::BasepointPixmap(const QPixmap &pm)
	: QPixmap(pm), qpBasePoint(0, pm.height()), iAscent(-1), iDescent(-1) {
}

BasepointPixmap::BasepointPixmap(int w, int h) : QPixmap(w, h), qpBasePoint(0, h), iAscent(-1), iDescent(-1) {
}

BasepointPixmap::BasepointPixmap(int w, int h, const QPoint &bp)
	: QPixmap(w, h), qpBasePoint(bp), iAscent(-1), iDescent(-1) {
}

OverlayTextLine::OverlayTextLine(const QString &s, const QFont &f)
	: fEdgeFactor(0.05f), qsText(s), qfFont(f), fXCorrection(0.0), fYCorrection(0.0), iCurWidth(-1), iCurHeight(-1),
	  fBaseliningThreshold(0.5f), bElided(false) {
	QFontMetrics fm(f);
	fAscent  = static_cast< float >(fm.ascent());
	fDescent = static_cast< float >(fm.descent());
	fEdge    = qMax(static_cast< float >(f.pointSizeF()) * fEdgeFactor, 1.0f);
}

BasepointPixmap OverlayTextLine::render(int w, int h, const QColor &col, const QPoint &bp) const {
	BasepointPixmap img(w, h, bp);
	img.fill(Qt::transparent);

	QPainter imgp(&img);
	imgp.setRenderHint(QPainter::Antialiasing);
	imgp.setRenderHint(QPainter::TextAntialiasing);
	imgp.setBackground(QColor(0, 0, 0, 0));
	imgp.setCompositionMode(QPainter::CompositionMode_SourceOver);

	QColor qc(col);
	qc.setAlpha(255);

	imgp.setBrush(qc);
	imgp.setPen(QPen(Qt::black, fEdge, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	imgp.drawPath(qpp);

	imgp.setPen(Qt::NoPen);
	imgp.drawPath(qpp);

	img.iAscent  = static_cast< int >(fAscent + 0.5f);
	img.iDescent = static_cast< int >(fDescent + 0.5f);
	return img;
}

BasepointPixmap OverlayTextLine::createPixmap(QColor col) {
	if (qsText.isEmpty()) {
		return BasepointPixmap();
	}

	QRectF qr;
	if (qpp.isEmpty()) {
		qpp.addText(0.0f, fAscent, qfFont, qsText);
		qr = qpp.controlPointRect();

		// fit into (0,0)-based coordinates
		fXCorrection = 0.0f;
		fYCorrection = 0.0f;

		if (qr.left() < fEdge) {
			fXCorrection = fEdge - static_cast< float >(qr.left());
		}

		if (qr.top() < fEdge) {
			fYCorrection = fEdge - static_cast< float >(qr.top());
		}

		QTransform correction;
		correction.translate(fXCorrection, fYCorrection);
		qpp = correction.map(qpp);
	}

	qr = qpp.controlPointRect();

	return render(static_cast< int >(qr.right() + 2.0f * fEdge + 0.5f),
				  static_cast< int >(qr.bottom() + 2.0f * fEdge + 0.5f), col,
				  QPoint(static_cast< int >(fXCorrection + 0.5f), static_cast< int >(fYCorrection + fAscent + 0.5f)));
}

BasepointPixmap OverlayTextLine::createPixmap(unsigned int maxwidth, unsigned int height, QColor col) {
	float twice_edge = 2.0f * fEdge;

	if (!height || !maxwidth)
		return BasepointPixmap();

	if (qpp.isEmpty() || iCurWidth > static_cast< int >(maxwidth) || iCurHeight != static_cast< int >(height)
		|| (static_cast< int >(maxwidth) > iCurWidth && bElided)) {
		QFont f = qfFont;
		QFontMetrics fm(f);

		// fit the font into a bounding box with padding
		float ps   = static_cast< float >(f.pointSizeF());
		float f_ad = static_cast< float >(fm.ascent() + fm.descent() + 1) / ps;

		float pointsize = static_cast< float >(height) / (f_ad + 2.0f * fEdgeFactor);

		if (fEdgeFactor * ps > 1.0f) {
			pointsize = static_cast< float >(height - 2) / f_ad;
		}

		if (pointsize <= 0.0f) {
			return BasepointPixmap();
		}

		f.setPointSizeF(pointsize);
		setFont(f);
		fm         = QFontMetrics(f);
		twice_edge = 2.0f * fEdge;

		if (!qpp.isEmpty()) {
			qpp = QPainterPath();
		}

		// calculate text metrics for eliding and scaling
		QRectF bb;
		qpp.addText(0.0f, 0.0f, f, qsText);
		bb = qpp.controlPointRect();

		qreal effective_ascent  = -bb.top();
		qreal effective_descent = bb.bottom();
		float scale             = 1.0f;
		bool keep_baseline      = true;
		if (effective_descent > fDescent || effective_ascent > fAscent) {
			qreal scale_ascent  = effective_ascent > 0.0f ? fAscent / effective_ascent : 1.0f;
			qreal scale_descent = effective_descent > 0.0f ? fDescent / effective_descent : 1.0f;
			scale               = static_cast< float >(qMin(scale_ascent, scale_descent));

			if (scale < fBaseliningThreshold) {
				float text_height = static_cast< float >(bb.height()) + twice_edge;
				scale             = static_cast< float >(height) / text_height;
				keep_baseline     = false;
			}

			qWarning() << QString(QLatin1String("Text \"%1\" did not fit (+%2/-%3): (+%4/-%5). Scaling to %6."))
							  .arg(qsText)
							  .arg(fAscent)
							  .arg(fDescent)
							  .arg(effective_ascent)
							  .arg(effective_descent)
							  .arg(scale);
		}

		// eliding by previously calculated width
		if ((bb.width() * scale) + twice_edge > maxwidth) {
			int eliding_width = static_cast< int >((static_cast< float >(maxwidth) / scale) - twice_edge + 0.5f);
			QString str       = fm.elidedText(qsText, Qt::ElideRight, eliding_width);

			// use ellipsis as shortest possible string
			if (str.trimmed().isEmpty()) {
				str = QString(QChar(0x2026));
			}

			qpp = QPainterPath();
			qpp.addText(0.0f, 0.0f, f, str);
			bb      = qpp.controlPointRect();
			bElided = true;
		} else {
			bElided = false;
		}

		// translation to "pixmap space":
		QTransform correction;
		//  * adjust left edge
		correction.translate(-bb.x() + fEdge, 0.0f);
		//  * scale overly high text (still on baseline)
		correction.scale(scale, scale);

		if (keep_baseline) {
			//  * translate down to baseline
			correction.translate(0.0f, (fAscent + fEdge) / scale);
		} else {
			//  * translate into bounding box
			correction.translate(0.0f, -bb.top() + fEdge);
		}

		qpp        = correction.map(qpp);
		iCurWidth  = static_cast< int >(bb.width() * scale + 0.5f);
		iCurHeight = static_cast< int >(height);
	}

	QRectF qr = qpp.controlPointRect();

	return render(static_cast< int >(qr.width() + twice_edge + 0.5f),
				  static_cast< int >(fAscent + fDescent + twice_edge + 0.5f), col,
				  QPoint(0, static_cast< int >(fAscent + fEdge + 0.5f)));
}

void OverlayTextLine::setFont(const QFont &font) {
	qfFont = font;
	qpp    = QPainterPath();
	QFontMetrics fm(font);
	fAscent  = static_cast< float >(fm.ascent() + 1);
	fDescent = static_cast< float >(fm.descent() + 1);
	fEdge    = qMax(static_cast< float >(font.pointSizeF()) * fEdgeFactor, 1.0f);
}

void OverlayTextLine::setEdge(float ew) {
	fEdge = ew;
	qpp   = QPainterPath();
}
