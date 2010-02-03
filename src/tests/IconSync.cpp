#include <QtCore>
#include <QtNetwork>
#include <QtGui>

int main(int argc, char **argv) {
	QApplication a(argc, argv);

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
		QImageReader svg("../../icons/mumble.svg");
		svg.setScaledSize(QSize(size, size));
		QImage img = svg.read().convertToFormat(QImage::Format_Indexed8);

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
