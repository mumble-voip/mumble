// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_G15LCDENGINE_UNIX_H_
#	define MUMBLE_MUMBLE_G15LCDENGINE_UNIX_H_
#	include "LCD.h"

#	include <g15daemon_client.h>

class G15LCDDeviceUnix;

class G15LCDEngineUnix : public LCDEngine {
	friend class G15LCDDeviceUnix;

protected:
	int sock;

public:
	G15LCDEngineUnix();
	~G15LCDEngineUnix() Q_DECL_OVERRIDE;
	QList< LCDDevice * > devices() const Q_DECL_OVERRIDE;
};

class G15LCDDeviceUnix : public LCDDevice {
protected:
	bool bEnabled;
	G15LCDEngineUnix *engine;

public:
	G15LCDDeviceUnix(G15LCDEngineUnix *e);
	~G15LCDDeviceUnix() Q_DECL_OVERRIDE;
	bool enabled() Q_DECL_OVERRIDE;
	void setEnabled(bool e) Q_DECL_OVERRIDE;
	void blitImage(QImage *img, bool alert) Q_DECL_OVERRIDE;
	QString name() const Q_DECL_OVERRIDE;
	QSize size() const Q_DECL_OVERRIDE;
};

#else
class G15LCDEngineUnix;
class G15LCDDeviceUnix;
#endif
