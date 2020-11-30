// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_LINUX_H_

#include "ConfigDialog.h"
#include "Global.h"
#include "GlobalShortcut.h"

class GlobalShortcutLinux : public GlobalShortcutEngine {
public:
	GlobalShortcutLinux();
	~GlobalShortcutLinux() Q_DECL_OVERRIDE;
	QString buttonName(const QVariant &) Q_DECL_OVERRIDE;

	QMap< QString, QFile * > qmInputDevices;
	QSet< QString > qsKeyboards;
public slots:
	void inputReadyRead(int);
	void directoryChanged(const QString &);
};

#endif
