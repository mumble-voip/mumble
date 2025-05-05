// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_OVERLAYTEXT_H_
#define MUMBLE_MUMBLE_OVERLAYTEXT_H_

#include <QtGui/QFont>
#include <QtGui/QPainterPath>
#include <QtGui/QPixmap>

//! Annotated QPixmap supplying a basepoint.
class BasepointPixmap : public QPixmap {
public:
	//! Local coordinates of the base point.
	QPoint qpBasePoint;
	//@{
	/**
	 * Font ascent and descent.
	 * The pixmap may exceed those font metrics, so if you need to
	 * transform rendered text properly, use these attributes.
	 */
	int iAscent;
	int iDescent;
	//@}

	BasepointPixmap();
	//! Create from QPixmap, basepoint is bottom left.
	BasepointPixmap(const QPixmap &);
	//! Empty pixmap, basepoint is bottom left.
	BasepointPixmap(int, int);
	//! Empty pixmap with specified basepoint.
	BasepointPixmap(int, int, const QPoint &);
};

class OverlayTextLine {
private:
	const float fEdgeFactor;

	QString qsText;
	QFont qfFont;
	QPainterPath qpp;
	float fAscent, fDescent;
	float fXCorrection, fYCorrection;
	int iCurWidth, iCurHeight;
	float fEdge;
	float fBaseliningThreshold;
	bool bElided;

	BasepointPixmap render(int, int, const QColor &, const QPoint &) const;

public:
	OverlayTextLine(const QString &, const QFont &);

	void setFont(const QFont &);
	void setEdge(float);

	//! Render text with current font.
	BasepointPixmap createPixmap(QColor col);
	//! Render text to fit a bounding box.
	BasepointPixmap createPixmap(unsigned int maxwidth, unsigned int height, QColor col);
};

#endif //_OVERLAYTEXT_H
