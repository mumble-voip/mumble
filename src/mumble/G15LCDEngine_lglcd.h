// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_G15LCDENGINE_LGLCD_H_
#	define MUMBLE_MUMBLE_G15LCDENGINE_LGLCD_H_
#	include "g15helper/g15helper.h"
#	include "LCD.h"

class G15LCDDeviceLGLCD;

class G15LCDEngineLGLCD : public LCDEngine {
	friend class G15LCDDeviceLGLCD;

private:
	Q_OBJECT
	Q_DISABLE_COPY(G15LCDEngineLGLCD)
protected:
	lgLcdConnectContextEx llcceConnect;
	lgLcdOpenByTypeContext llcContext;

public:
	G15LCDEngineLGLCD();
	~G15LCDEngineLGLCD();
	QList< LCDDevice * > devices() const;
};

class G15LCDDeviceLGLCD : public LCDDevice {
protected:
	G15LCDEngineLGLCD *engine;
	bool bEnabled;

public:
	G15LCDDeviceLGLCD(G15LCDEngineLGLCD *e);
	~G15LCDDeviceLGLCD();
	bool enabled();
	void setEnabled(bool e);
	void blitImage(QImage *img, bool alert);
	QString name() const;
	QSize size() const;
};

#else
class G15LCDEngineLGLCD;
class G15LCDDeviceLGLCD;
#endif
