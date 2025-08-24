// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TextureManager.h"

#include "Database.h"
#include "RichTextEditor.h"
#include <QBuffer>
#include <QImageReader>
#include <QImageWriter>
#include <QPainter>


QByteArray TextureManager::convertTexture(QByteArray texture, QByteArray &textureFormat) {
	bool valid = true;
	if (!texture.isEmpty()) {
		if (texture.length() < static_cast< int >(sizeof(unsigned int))) {
			valid = false;
		} else if (qFromBigEndian< unsigned int >(reinterpret_cast< const unsigned char * >(texture.constData()))
				   == 600 * 60 * 4) {
			QByteArray qba = qUncompress(texture);
			if (qba.length() != 600 * 60 * 4) {
				valid = false;
			} else {
				int width               = 0;
				int height              = 0;
				const unsigned int *ptr = reinterpret_cast< const unsigned int * >(qba.constData());

				// If we have an alpha only part on the right side of the image ignore it
				for (int y = 0; y < 60; ++y) {
					for (int x = 0; x < 600; ++x) {
						if (ptr[y * 600 + x] & 0xff000000) {
							if (x > width)
								width = x;
							if (y > height)
								height = y;
						}
					}
				}

				// Full size image? More likely image without alpha; fix it.
				if ((width == 599) && (height == 59)) {
					width  = 0;
					height = 0;
					for (int y = 0; y < 60; ++y) {
						for (int x = 0; x < 600; ++x) {
							if (ptr[y * 600 + x] & 0x00ffffff) {
								if (x > width)
									width = x;
								if (y > height)
									height = y;
							}
						}
					}
				}

				if (!width || !height) {
					valid = false;
				} else {
					QImage img = QImage(width + 1, height + 1, QImage::Format_ARGB32);
					{
						QImage srcimg(reinterpret_cast< const uchar * >(qba.constData()), 600, 60,
									  QImage::Format_ARGB32);

						QPainter imgp(&img);
						img.fill(0);
						imgp.setRenderHint(QPainter::Antialiasing);
						imgp.setRenderHint(QPainter::TextAntialiasing);
						imgp.setBackground(QColor(0, 0, 0, 0));
						imgp.setCompositionMode(QPainter::CompositionMode_Source);
						imgp.drawImage(0, 0, srcimg);
					}
					texture = QByteArray();

					QBuffer qb(&texture);
					qb.open(QIODevice::WriteOnly);
					QImageWriter qiw(&qb, "png");
					qiw.write(img);

					textureFormat = QString::fromLatin1("png").toUtf8();
				}
			}
		} else {
			QBuffer qb(&texture);
			qb.open(QIODevice::ReadOnly);

			QImageReader qir;
			qir.setAutoDetectImageFormat(false);

			QByteArray fmt;
			if (RichTextImage::isValidImage(texture, fmt)) {
				qir.setFormat(fmt);
				qir.setDevice(&qb);
				if (!qir.canRead() || (qir.size().width() > 1024) || (qir.size().height() > 1024)) {
					valid = false;
				} else {
					textureFormat = qir.format();
					QImage qi     = qir.read();
					valid         = !qi.isNull();
				}
			} else {
				valid = false;
			}
		}
	}

	if (!valid) {
		texture       = QByteArray();
		textureFormat = QByteArray();
	}

	return texture;
}