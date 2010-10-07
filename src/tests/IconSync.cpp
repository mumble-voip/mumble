#include <QtCore>
#include <QtNetwork>
#include <QtGui>
#include <QtSvg>

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	QSvgRenderer svg(QLatin1String("../../icons/mumble.svg"));
	QImage original(512,512,QImage::Format_ARGB32);
	original.fill(Qt::transparent);

	QPainter painter(&original);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.setRenderHint(QPainter::HighQualityAntialiasing);
	svg.render(&painter);

	QList<int> sizes;
	sizes << 16;
	sizes << 24;
	sizes << 32;
	sizes << 48;
	sizes << 64;
	sizes << 96;
	sizes << 128;

	QStringList qslImages;

	foreach(int size, sizes) {
		QImage img = original.scaled(size,size,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		QString png = QDir::temp().absoluteFilePath(QString::fromLatin1("mumble.%1.png").arg(size));

		QImageWriter qiw(png);
		qiw.write(img);

		qslImages << png;
	}
	QStringList args;

	args << qslImages;
	args << QDir::current().absoluteFilePath("../../icons/mumble.ico");

	qWarning() << args;

	QProcess qp;
	qp.setProcessChannelMode(QProcess::ForwardedChannels);
	qp.start("/usr/bin/convert", args);
	if (! qp.waitForFinished())
		qWarning() << "No finish";
	foreach(const QString &png, qslImages)
		QDir::temp().remove(png);
}
