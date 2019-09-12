// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GlobalShortcut_unix.h"

#include "Settings.h"

#include <QtCore/QFileSystemWatcher>
#include <QtCore/QSocketNotifier>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

// We have to use a global 'diagnostic ignored' pragmas because
// we still support old versions of GCC. (FreeBSD 9.3 ships with GCC 4.2)
#if defined (__GNUC__)
// ScreenCount(...) and so on are macros that access the private structure and
// cast their return value using old-style-casts. Hence we suppress these warnings
// for this section of code.
# pragma GCC diagnostic ignored "-Wold-style-cast"
// XKeycodeToKeysym is deprecated.
// For backwards compatibility reasons we want to keep using the
// old function as long as possible. The replacement function
// XkbKeycodeToKeysym requires the XKB extension which isn't
// guaranteed to be present.
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

/**
 * Returns a platform specific GlobalShortcutEngine object.
 *
 * @see GlobalShortcutX
 * @see GlobalShortcutMac
 * @see GlobalShortcutWin
 */
GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutX();
}

GlobalShortcutX::GlobalShortcutX() {
	iXIopcode =  -1;
	bRunning = false;

	display = XOpenDisplay(NULL);

	if (! display) {
		qWarning("GlobalShortcutX: Unable to open dedicated display connection.");
		return;
	}

#ifdef Q_OS_LINUX
	if (g.s.bEnableEvdev) {
		QString dir = QLatin1String("/dev/input");
		QFileSystemWatcher *fsw = new QFileSystemWatcher(QStringList(dir), this);
		connect(fsw, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChanged(const QString &)));
		directoryChanged(dir);

		if (qsKeyboards.isEmpty()) {
			foreach(QFile *f, qmInputDevices)
				delete f;
			qmInputDevices.clear();

			delete fsw;
			qWarning("GlobalShortcutX: Unable to open any keyboard input devices under /dev/input, falling back to XInput");
		} else {
			return;
		}
	}
#endif

	for (int i=0; i < ScreenCount(display); ++i)
		qsRootWindows.insert(RootWindow(display, i));

#ifndef NO_XINPUT2
	int evt, error;

	if (g.s.bEnableXInput2 && XQueryExtension(display, "XInputExtension", &iXIopcode, &evt, &error)) {
		int major = XI_2_Major;
		int minor = XI_2_Minor;
		int rc = XIQueryVersion(display, &major, &minor);
		if (rc != BadRequest) {
			qWarning("GlobalShortcutX: Using XI2 %d.%d", major, minor);

			queryXIMasterList();

			XIEventMask evmask;
			unsigned char mask[(XI_LASTEVENT + 7)/8];

			memset(&evmask, 0, sizeof(evmask));
			memset(mask, 0, sizeof(mask));

			XISetMask(mask, XI_RawButtonPress);
			XISetMask(mask, XI_RawButtonRelease);
			XISetMask(mask, XI_RawKeyPress);
			XISetMask(mask, XI_RawKeyRelease);
			XISetMask(mask, XI_HierarchyChanged);

			evmask.deviceid = XIAllDevices;
			evmask.mask_len = sizeof(mask);
			evmask.mask = mask;

			foreach(Window w, qsRootWindows)
				XISelectEvents(display, w, &evmask, 1);
			XFlush(display);

			connect(new QSocketNotifier(ConnectionNumber(display), QSocketNotifier::Read, this), SIGNAL(activated(int)), this, SLOT(displayReadyRead(int)));

			return;
		}
	}
#endif
	qWarning("GlobalShortcutX: No XInput support, falling back to polled input. This wastes a lot of CPU resources, so please enable one of the other methods.");
	bRunning=true;
	start(QThread::TimeCriticalPriority);
}

GlobalShortcutX::~GlobalShortcutX() {
	bRunning = false;
	wait();

	if (display)
		XCloseDisplay(display);
}

// Tight loop polling
void GlobalShortcutX::run() {
	Window root = XDefaultRootWindow(display);
	Window root_ret, child_ret;
	int root_x, root_y;
	int win_x, win_y;
	unsigned int mask[2];
	int idx = 0;
	int next = 0;
	char keys[2][32];

	memset(keys[0], 0, 32);
	memset(keys[1], 0, 32);
	mask[0] = mask[1] = 0;

	while (bRunning) {
		if (bNeedRemap)
			remap();

		msleep(10);

		idx = next;
		next = idx ^ 1;
		if (XQueryPointer(display, root, &root_ret, &child_ret, &root_x, &root_y, &win_x, &win_y, &mask[next]) && XQueryKeymap(display, keys[next])) {
			for (int i=0;i<256;++i) {
				int index = i / 8;
				int keymask = 1 << (i % 8);
				bool oldstate = (keys[idx][index] & keymask) != 0;
				bool newstate = (keys[next][index] & keymask) != 0;
				if (oldstate != newstate) {
					handleButton(i, newstate);
				}
			}
			for (int i=8;i<=12;++i) {
				bool oldstate = (mask[idx] & (1 << i)) != 0;
				bool newstate = (mask[next] & (1 << i)) != 0;
				if (oldstate != newstate) {
					handleButton(0x110 + i, newstate);
				}
			}
		}
	}
}

// Find XI2 master devices so they can be ignored.
void GlobalShortcutX::queryXIMasterList() {
#ifndef NO_XINPUT2
	XIDeviceInfo *info, *dev;
	int ndevices;

	qsMasterDevices.clear();

	dev = info = XIQueryDevice(display, XIAllDevices, &ndevices);
	for (int i=0;i<ndevices;++i) {
		switch (dev->use) {
			case XIMasterPointer:
			case XIMasterKeyboard:
				qsMasterDevices.insert(dev->deviceid);
				break;
			default:
				break;
		}

		++dev;
	}
	XIFreeDeviceInfo(info);
#endif
}

// XInput2 event is ready on socketnotifier.
void GlobalShortcutX::displayReadyRead(int) {
#ifndef NO_XINPUT2
	XEvent evt;

	if (bNeedRemap)
		remap();

	while (XPending(display)) {
		XNextEvent(display, &evt);
		XGenericEventCookie *cookie = & evt.xcookie;

		if ((cookie->type != GenericEvent) || (cookie->extension != iXIopcode) || !XGetEventData(display, cookie))
			continue;

		XIDeviceEvent *xide = reinterpret_cast<XIDeviceEvent *>(cookie->data);

		switch (cookie->evtype) {
			case XI_RawKeyPress:
			case XI_RawKeyRelease:
				if (! qsMasterDevices.contains(xide->deviceid))
					handleButton(xide->detail, cookie->evtype == XI_RawKeyPress);
				break;
			case XI_RawButtonPress:
			case XI_RawButtonRelease:
				if (! qsMasterDevices.contains(xide->deviceid))
					handleButton(xide->detail + 0x117, cookie->evtype == XI_RawButtonPress);
				break;
			case XI_HierarchyChanged:
				queryXIMasterList();
		}

		XFreeEventData(display, cookie);
	}
#endif
}

// One of the raw /dev/input devices has ready input
void GlobalShortcutX::inputReadyRead(int) {
#ifdef Q_OS_LINUX
	if (!g.s.bEnableEvdev) {
		return;
	}

	struct input_event ev;

	if (bNeedRemap)
		remap();

	QFile *f=qobject_cast<QFile *>(sender()->parent());
	if (!f)
		return;

	bool found = false;

	while (f->read(reinterpret_cast<char *>(&ev), sizeof(ev)) == sizeof(ev)) {
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

	if (! found) {
		int fd = f->handle();
		int version = 0;
		if ((ioctl(fd, EVIOCGVERSION, &version) < 0) || (((version >> 16) & 0xFF) < 1)) {
			qWarning("GlobalShortcutX: Removing dead input device %s", qPrintable(f->fileName()));
			qmInputDevices.remove(f->fileName());
			qsKeyboards.remove(f->fileName());
			delete f;
		}
	}
#endif
}

#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))

// The /dev/input directory changed
void GlobalShortcutX::directoryChanged(const QString &dir) {
#ifdef Q_OS_LINUX
	if (!g.s.bEnableEvdev) {
		return;
	}

	QDir d(dir, QLatin1String("event*"), 0, QDir::System);
	foreach(QFileInfo fi, d.entryInfoList()) {
		QString path = fi.absoluteFilePath();
		if (! qmInputDevices.contains(path)) {
			QFile *f = new QFile(path, this);
			if (f->open(QIODevice::ReadOnly)) {
				int fd = f->handle();
				int version;
				char name[256];
				uint8_t events[EV_MAX/8 + 1];
				memset(events, 0, sizeof(events));
				if ((ioctl(fd, EVIOCGVERSION, &version) >= 0) && (ioctl(fd, EVIOCGNAME(sizeof(name)), name)>=0) && (ioctl(fd, EVIOCGBIT(0,sizeof(events)), &events) >= 0) && test_bit(EV_KEY, events) && (((version >> 16) & 0xFF) > 0)) {
					name[255]=0;
					qWarning("GlobalShortcutX: %s: %s", qPrintable(f->fileName()), name);
					// Is it grabbed by someone else?
					if ((ioctl(fd, EVIOCGRAB, 1) < 0)) {
						qWarning("GlobalShortcutX: Device exclusively grabbed by someone else (X11 using exclusive-mode evdev?)");
						delete f;
					} else {
						ioctl(fd, EVIOCGRAB, 0);
						uint8_t keys[KEY_MAX/8 + 1];
						if ((ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keys)), &keys) >= 0) && test_bit(KEY_SPACE, keys))
							qsKeyboards.insert(f->fileName());

						fcntl(f->handle(), F_SETFL, O_NONBLOCK);
						connect(new QSocketNotifier(f->handle(), QSocketNotifier::Read, f), SIGNAL(activated(int)), this, SLOT(inputReadyRead(int)));

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
#else
	Q_UNUSED(dir);
#endif
}

QString GlobalShortcutX::buttonName(const QVariant &v) {
	bool ok;
	unsigned int key=v.toUInt(&ok);
	if (!ok)
		return QString();
	if ((key < 0x118) || (key >= 0x128)) {

		// For backwards compatibility reasons we want to keep using the
		// old function as long as possible. The replacement function
		// XkbKeycodeToKeysym requires the XKB extension which isn't
		// guaranteed to be present.
		KeySym ks=XKeycodeToKeysym(display, static_cast<KeyCode>(key), 0);
		if (ks == NoSymbol) {
			return QLatin1String("0x")+QString::number(key,16);
		} else {
			const char *str=XKeysymToString(ks);
			if (*str == '\0') {
				return QLatin1String("KS0x")+QString::number(ks, 16);
			} else {
				return QLatin1String(str);
			}
		}
	} else {
		return tr("Mouse %1").arg(key-0x118);
	}
}
