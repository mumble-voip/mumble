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

#ifndef _OVERLAYTEXT_H
#define _OVERLAYTEXT_H

#include "mumble_pch.hpp"

//! Annotated QPixmap supplying a basepoint.
class BasepointPixmap : public QPixmap {
	public:
		//! Local coordinates of the base point.
		QPoint qpBasePoint;
		//@{
		/*! Font ascent and descent.
		 * The pixmap may exceed those font metrics, so if you need to
		 * transform rendered text properly, use these attributes.
		 */
		int iAscent;
		int iDescent;
		//@}

		BasepointPixmap();
		//! Create from QPixmap, basepoint is bottom left.
		BasepointPixmap(const QPixmap&);
		//! Empty pixmap, basepoint is bottom left.
		BasepointPixmap(int, int);
		//! Empty pixmap with specified basepoint.
		BasepointPixmap(int, int, const QPoint&);
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

		BasepointPixmap render(int, int, const QColor&, const QPoint&) const;
	public:
		OverlayTextLine(const QString&, const QFont&);

		void setFont(const QFont&);
		void setEdge(float);

		//! Render text with current font.
		BasepointPixmap createPixmap(QColor col);
		//! Render text to fit a bounding box.
		BasepointPixmap createPixmap(unsigned int maxwidth, unsigned int height, QColor col);
};

#endif //_OVERLAYTEXT_H
