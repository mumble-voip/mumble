// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QTextStream>
#include <QFileInfo>

inline unsigned long long getProcessAddress() {
	QString exe = QFile::symLinkTarget("/proc/self/exe");
	QFileInfo fi(exe);
	QString firstPart = fi.baseName();
	QString completeSuffix = fi.completeSuffix();
	QString modname;

	if (completeSuffix.isEmpty()) {
		modname = firstPart;
	} else {
		modname = firstPart + QLatin1String(".") + completeSuffix;
	}

	if (modname.isEmpty()) {
		return 0;
	}

	QFile mapsFile("/proc/self/maps");
	if (!mapsFile.open(QIODevice::ReadOnly)) {
		return 0;
	}

	QString maps = mapsFile.readAll();

	mapsFile.close();

	if (maps.isEmpty()) {
		return 0;
	}

	QTextStream tsPath(&maps);
	while (!tsPath.atEnd()) {
		QString baseaddr;

		QChar ch;
		while (true) {
			tsPath >> ch;
			if (ch == '-') {
				break;
			} else if (ch == EOF) {
				return 0;
			}
			baseaddr.append(ch);
		}

		// seek to perms
		do {
			tsPath >> ch;
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to offset
		do {
			tsPath >> ch;
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to dev
		do {
			tsPath >> ch;
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to inode
		do {
			tsPath >> ch;
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// seek to pathname
		do {
			tsPath >> ch;
			if (ch == EOF) {
				return 0;
			}
		} while (ch != ' ');

		// eat spaces until we're at the beginning of pathname.
		while (ch == ' ') {
			if (ch == EOF) {
				return 0;
			}
			tsPath >> ch;
		}
		tsPath.seek(tsPath.pos() - 1);

		QString pathname;
		while (true) {
			tsPath >> ch;
			if (ch == '\n') {
				break;
			} else if (ch == EOF) {
				return 0;
			}
			pathname.append(ch);
		};

		// OK, we found 'em!
		// Only treat path as a real path if it starts with /.
		if (pathname.size() > 0 && pathname.at(0) == '/') {
			// Find the basename.
			int lastSlash = pathname.lastIndexOf('/');
			if (pathname.size() > lastSlash + 1) {
				QString basename = pathname.mid(lastSlash + 1);
				if (basename == modname) {
					bool ok = false;
					unsigned long addr = baseaddr.toULong(&ok, 16);
					if (!ok) {
						return 0;
					}

					return addr;
				}
			}
		}
	}

	return 0;
}
