// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GlobalShortcut_linux.h"

#include "Settings.h"

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QSocketNotifier>

#ifdef Q_OS_LINUX
#	include <fcntl.h>
#	include <linux/input.h>
#endif

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name
// (like protobuf 3.7 does). As such, for now, we have to make this our last incl
#include "Global.h"

// We have to use a global 'diagnostic ignored' pragmas because
// we still support old versions of GCC. (FreeBSD 9.3 ships with GCC 4.2)
#if defined(__GNUC__)
// ScreenCount(...) and so on are macros that access the private structure and
// cast their return value using old-style-casts. Hence we suppress these warnings
// for this section of code.
#	pragma GCC diagnostic ignored "-Wold-style-cast"
// XKeycodeToKeysym is deprecated.
// For backwards compatibility reasons we want to keep using the
// old function as long as possible. The replacement function
// XkbKeycodeToKeysym requires the XKB extension which isn't
// guaranteed to be present.
#	pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

/**
 * Returns a platform specific GlobalShortcutEngine object.
 *
 * @see GlobalShortcutX
 * @see GlobalShortcutLinux
 * @see GlobalShortcutMac
 * @see GlobalShortcutWin
 */
GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutLinux();
}

GlobalShortcutLinux::GlobalShortcutLinux() {
	if (g.s.bEnableEvdev) {
		QString dir             = QLatin1String("/dev/input");
		QFileSystemWatcher *fsw = new QFileSystemWatcher(QStringList(dir), this);
		connect(fsw, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChanged(const QString &)));
		directoryChanged(dir);

		if (qsKeyboards.isEmpty()) {
			foreach (QFile *f, qmInputDevices)
				delete f;
			qmInputDevices.clear();

			delete fsw;
			qWarning("GlobalShortcutLinux: Unable to open any keyboard input devices under /dev/input");
		} else {
			return;
		}
	} else {
		qWarning("GlobalShortcutLinux: No evdev support, global shortcuts disabled");
	}
}

GlobalShortcutLinux::~GlobalShortcutLinux() {
	wait();
}

// One of the raw /dev/input devices has ready input
void GlobalShortcutLinux::inputReadyRead(int) {
	if (!g.s.bEnableEvdev) {
		return;
	}

	struct input_event ev;

	if (bNeedRemap)
		remap();

	QFile *f = qobject_cast< QFile * >(sender()->parent());
	if (!f)
		return;

	bool found = false;

	while (f->read(reinterpret_cast< char * >(&ev), sizeof(ev)) == sizeof(ev)) {
		found = true;
		if (ev.type != EV_KEY)
			continue;
		bool down;
		switch (ev.value) {
			case 0:
				down = false;
				break;
			case 1:
				down = true;
				break;
			default:
				continue;
		}
		int evtcode = ev.code + 8;
		handleButton(evtcode, down);
	}

	if (!found) {
		int fd      = f->handle();
		int version = 0;
		if ((ioctl(fd, EVIOCGVERSION, &version) < 0) || (((version >> 16) & 0xFF) < 1)) {
			qWarning("GlobalShortcutLinux: Removing dead input device %s", qPrintable(f->fileName()));
			qmInputDevices.remove(f->fileName());
			qsKeyboards.remove(f->fileName());
			delete f;
		}
	}
}

#define test_bit(bit, array) (array[bit / 8] & (1 << (bit % 8)))

// The /dev/input directory changed
void GlobalShortcutLinux::directoryChanged(const QString &dir) {
	if (!g.s.bEnableEvdev) {
		return;
	}

	QDir d(dir, QLatin1String("event*"), 0, QDir::System);
	foreach (QFileInfo fi, d.entryInfoList()) {
		QString path = fi.absoluteFilePath();
		if (!qmInputDevices.contains(path)) {
			QFile *f = new QFile(path, this);
			if (f->open(QIODevice::ReadOnly)) {
				int fd = f->handle();
				int version;
				char name[256];
				uint8_t events[EV_MAX / 8 + 1];
				memset(events, 0, sizeof(events));
				if ((ioctl(fd, EVIOCGVERSION, &version) >= 0) && (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0)
					&& (ioctl(fd, EVIOCGBIT(0, sizeof(events)), &events) >= 0) && test_bit(EV_KEY, events)
					&& (((version >> 16) & 0xFF) > 0)) {
					name[255] = 0;
					qWarning("GlobalShortcutLinux: %s: %s", qPrintable(f->fileName()), name);
					// Is it grabbed by someone else?
					if ((ioctl(fd, EVIOCGRAB, 1) < 0)) {
						qWarning("GlobalShortcutLinux: Device exclusively grabbed by someone else (X11 using "
								 "exclusive-mode evdev?)");
						delete f;
					} else {
						ioctl(fd, EVIOCGRAB, 0);
						uint8_t keys[KEY_MAX / 8 + 1];
						if ((ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keys)), &keys) >= 0) && test_bit(KEY_SPACE, keys))
							qsKeyboards.insert(f->fileName());

						fcntl(f->handle(), F_SETFL, O_NONBLOCK);
						connect(new QSocketNotifier(f->handle(), QSocketNotifier::Read, f), SIGNAL(activated(int)),
								this, SLOT(inputReadyRead(int)));

						qmInputDevices.insert(f->fileName(), f);
					}
				} else {
					delete f;
				}
			} else {
				delete f;
			}
		}
	}
}

QString GlobalShortcutLinux::buttonName(const QVariant &v) {
	bool ok;
	unsigned int key = v.toUInt(&ok);
	//char buffer[128];

	if (!ok)
		return QString();

	if ((key < 0x118) || (key >= 0x128)) {
		return QLatin1String("0x") + QString::number(key, 16);
	} else {
		return tr("Mouse %1").arg(key - 0x118);
	}
}
