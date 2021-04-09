// Copyright 2013-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtSvg>

void svg2png(const QString &svgpath, const QList< int > &sizes, QStringList &qslImages) {
	QSvgRenderer svg(svgpath);
	QImage original(512, 512, QImage::Format_ARGB32);
	original.fill(Qt::transparent);

	QPainter painter(&original);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);
	svg.render(&painter);

	foreach (int size, sizes) {
		QImage img  = original.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		QString png = QDir::temp().absoluteFilePath(QString::fromLatin1("mumble.%1.png").arg(size));

		QImageWriter qiw(png);
		qiw.write(img);

		qslImages << png;
	}
}

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);


	QStringList qslImages;

	QString svgSmallPath = QLatin1String("../../icons/mumble_small.svg");
	QList< int > sizesSmall;
	sizesSmall << 16;
	sizesSmall << 24;
	svg2png(svgSmallPath, sizesSmall, qslImages);

	QLatin1String svgPath("../../icons/mumble.svg");
	QList< int > sizes;
	sizes << 32;
	sizes << 48;
	sizes << 64;
	sizes << 96;
	sizes << 128;
	sizes << 256;
	svg2png(svgPath, sizes, qslImages);


	QStringList args;
	args << qslImages;
	args << QDir::current().absoluteFilePath("../../icons/mumble.ico");

	qWarning() << args;

	QProcess qp;
	qp.setProcessChannelMode(QProcess::ForwardedChannels);
	qp.start("/usr/bin/convert", args);
	if (!qp.waitForFinished())
		qWarning() << "No finish";
	foreach (const QString &png, qslImages)
		QDir::temp().remove(png);
}
