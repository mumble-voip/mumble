// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SvgIcon.h"

#include <QPainter>
#include <QSvgRenderer>

void SvgIcon::addSvgPixmapsToIcon(QIcon &icon, QString fn) {
	QSvgRenderer svg(fn);

	QList< QSize > commonSizes;
	commonSizes << QSize(8, 8);
	commonSizes << QSize(16, 16);
	commonSizes << QSize(22, 22); // Plasma notification area size
	commonSizes << QSize(24, 24);
	commonSizes << QSize(32, 32);
	commonSizes << QSize(44, 44); // Plasma notification area size @x2
	commonSizes << QSize(48, 48);
	commonSizes << QSize(64, 64);
	commonSizes << QSize(96, 96);
	commonSizes << QSize(128, 128);
	commonSizes << QSize(256, 256);

	foreach (QSize size, commonSizes) {
		QPixmap pm(size);
		pm.fill(Qt::transparent);

		QPainter p(&pm);
		p.setRenderHint(QPainter::Antialiasing);
		p.setRenderHint(QPainter::TextAntialiasing);
		p.setRenderHint(QPainter::SmoothPixmapTransform);
		svg.render(&p);

		icon.addPixmap(pm);
	}
}
