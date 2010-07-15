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

#include "GlobalShortcut_unix.h"

GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutX();
}

GlobalShortcutX::GlobalShortcutX() {
	bRunning=false;
	bXInput = false;

	display = NULL;

	iKeyPress = iKeyRelease = iButtonPress = iButtonRelease = -1;

#ifdef Q_OS_LINUX
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
#endif

	display = XOpenDisplay(NULL);

	if (! display) {
		qWarning("GlobalShortcutX: Unable to open dedicated display connection.");
		return;
	}

	XExtensionVersion *version = XGetExtensionVersion(display, INAME);
	if (version && (version != reinterpret_cast<XExtensionVersion *>(NoSuchExtension))) {
		qWarning("GlobalShortcutX: Using XInput %d.%d", version->major_version, version->minor_version);
		bXInput = true;
		XFree(version);
		initXInput();
		if (qmXDevices.isEmpty()) {
			qWarning("GlobalShortcutX: No XInput devices");
		} else {
			connect(new QSocketNotifier(ConnectionNumber(display), QSocketNotifier::Read, this), SIGNAL(activated(int)), this, SLOT(displayReadyRead(int)));
			return;
		}
	} else {
		qWarning("GlobalShortcutX: No XInput support, falling back to polled input. This wastes a lot of CPU resources, so please enable one of the other methods.");
		bRunning=true;
		start(QThread::TimeCriticalPriority);
	}
}

GlobalShortcutX::~GlobalShortcutX() {
	bRunning = false;
	wait();
	foreach(XDevice *dev, qmXDevices)
		XCloseDevice(display, dev);
	if (display)
		XCloseDisplay(display);
}

void GlobalShortcutX::initXInput() {
	foreach(XDevice *dev, qmXDevices)
		XCloseDevice(display, dev);

	qmXDevices.clear();

	int numdev;
	XDeviceInfo *infolist = XListInputDevices(display, &numdev);
	if (! infolist)
		return;
	for (int i=0;i<numdev;++i) {
		XDeviceInfo *info = infolist + i;
		XDevice *dev = XOpenDevice(display, info->id);
		if (dev) {
			bool key = false, button = false;
			for (int j=0;j<dev->num_classes;++j) {
				XInputClassInfo *ici = dev->classes + j;
				key = key || (ici->input_class == KeyClass);
				button = button || (ici->input_class == ButtonClass);
			}

			XEventClass events[4];
			int nevents = 0;
			if (key) {
				DeviceKeyPress(dev, iKeyPress, events[nevents]);
				++nevents;
				DeviceKeyRelease(dev, iKeyRelease, events[nevents]);
				++nevents;
			}
			if (button) {
				DeviceButtonPress(dev, iButtonPress, events[nevents]);
				++nevents;
				DeviceButtonRelease(dev, iButtonRelease, events[nevents]);
				++nevents;
			}

			if ((nevents != 0) && ! XSelectExtensionEvent(display, XDefaultRootWindow(display), events, nevents)) {
				qWarning("GlobalShortcutX: XInput %ld:%s", info->id, info->name);
				qmXDevices.insert(info->id, dev);
			} else {
				XCloseDevice(display, dev);
			}
		}
	}
	XFree(infolist);
}

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

void GlobalShortcutX::displayReadyRead(int) {
	XEvent evt;

	if (bNeedRemap)
		remap();

	while (XPending(display)) {
		XNextEvent(display, &evt);
		if ((evt.type == iButtonPress) || (evt.type == iButtonRelease)) {
			XDeviceButtonEvent *be = reinterpret_cast<XDeviceButtonEvent *>(&evt);
			handleButton(be->button + 0x117, evt.type == iButtonPress);
		} else if ((evt.type == iKeyPress) || (evt.type == iKeyRelease)) {
			XDeviceKeyEvent *ke = reinterpret_cast<XDeviceKeyEvent *>(&evt);
			if ((evt.type == iKeyRelease) && XPending(display)) {
				// Is it a silly key repeat?
				XEvent nxt;
				XPeekEvent(display, &nxt);
				if (nxt.type == iKeyPress) {
					XDeviceKeyEvent *ne = reinterpret_cast<XDeviceKeyEvent *>(&nxt);
					if (ke->keycode == ne->keycode) {
						XNextEvent(display, &nxt);
						continue;
					}
				}
			}
			handleButton(ke->keycode, evt.type == iKeyPress);
		}
	}
}

void GlobalShortcutX::inputReadyRead(int) {
#ifdef Q_OS_LINUX
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

void GlobalShortcutX::directoryChanged(const QString &dir) {
#ifdef Q_OS_LINUX
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
#endif
}

QString GlobalShortcutX::buttonName(const QVariant &v) {
	bool ok;
	unsigned int key=v.toUInt(&ok);
	if (!ok)
		return QString();
	if ((key < 0x118) || (key >= 0x128)) {
		KeySym ks=XKeycodeToKeysym(QX11Info::display(), static_cast<KeyCode>(key), 0);
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
