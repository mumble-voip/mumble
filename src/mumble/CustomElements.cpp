/* Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>
   Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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
#include "CustomElements.h"

void ChatbarLineEdit::focusInEvent(QFocusEvent *) {
	if (bDefaultVisible) {
		QFont f = font();
		f.setItalic(false);
		setFont(f);
		setAlignment(Qt::AlignLeft);
		setText(QString());
		bDefaultVisible = false;
	}
}

void ChatbarLineEdit::focusOutEvent(QFocusEvent *) {
	if (text().trimmed().isEmpty()) {
		QFont f = font();
		f.setItalic(true);
		setFont(f);
		setAlignment(Qt::AlignCenter);
		setText(qsDefaultText);
		bDefaultVisible = true;
	} else {
		bDefaultVisible = false;
	}
}

ChatbarLineEdit::ChatbarLineEdit(QWidget *p) : QLineEdit(p) {
	bDefaultVisible = true;
	setDefaultText(tr("Type chat message here"));
};

void ChatbarLineEdit::setDefaultText(const QString &new_default) {
	qsDefaultText = new_default;

	if (!hasFocus()) {
		QFont f = font();
		f.setItalic(true);
		setFont(f);
		setAlignment(Qt::AlignCenter);
		setText(qsDefaultText);
		bDefaultVisible = true;
	}
}


DockTitleBar::DockTitleBar() {
	qtTick = new QTimer(this);
	qtTick->setSingleShot(true);
	connect(qtTick, SIGNAL(timeout()), this, SLOT(tick()));
	size = newsize = 0;
}

QSize DockTitleBar::sizeHint() const {
	return minimumSizeHint();
}

QSize DockTitleBar::minimumSizeHint() const {
	return QSize(size,size);
}

bool DockTitleBar::eventFilter(QObject *, QEvent *evt) {
	QDockWidget *qdw = qobject_cast<QDockWidget*>(parentWidget());

	switch (evt->type()) {
		case QEvent::Leave:
		case QEvent::Enter:
		case QEvent::MouseMove:
		case QEvent::MouseButtonRelease: {
				newsize = 0;
				QPoint p = qdw->mapFromGlobal(QCursor::pos());
				if ((p.x() >= iroundf(static_cast<float>(qdw->width()) * 0.1f)) && (p.x() < iroundf(static_cast<float>(qdw->width()) * 0.9f))  && (p.y() >= 0) && (p.y() < 15))
					newsize = 15;
				if (newsize > 0 && !qtTick->isActive())
					qtTick->start(500);
				else if ((newsize == size) && qtTick->isActive())
					qtTick->stop();
				else if (newsize == 0)
					tick();
			}
		default:
			break;
	}

	return false;
}

void DockTitleBar::tick() {
	QDockWidget *qdw = qobject_cast<QDockWidget*>(parentWidget());

	if (newsize == size)
		return;

	size = newsize;
	qdw->setTitleBarWidget(this);
}
