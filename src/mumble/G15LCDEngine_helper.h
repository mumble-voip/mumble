// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_G15LCDENGINE_HELPER_H_
#	define MUMBLE_MUMBLE_G15LCDENGINE_HELPER_H_

#	include "g15helper/g15helper.h"
#	include "LCD.h"

#	include <QtCore/QProcess>

class G15LCDDeviceHelper;

class G15LCDEngineHelper : public LCDEngine {
	friend class G15LCDDeviceHelper;

private:
	Q_OBJECT
	Q_DISABLE_COPY(G15LCDEngineHelper)
protected:
	bool bUnavailable;
	bool bRunning;
	QProcess *qpHelper;
	QString qsHelperExecutable;
	bool framebufferReady() const;
	void submitFrame(bool alert, uchar *buf, qint64 len);
	void setProcessStatus(bool run);

public:
	G15LCDEngineHelper();
	~G15LCDEngineHelper() Q_DECL_OVERRIDE;
	QList< LCDDevice * > devices() const Q_DECL_OVERRIDE;
public slots:
	void on_Helper_finished(int exitCode, QProcess::ExitStatus status);
};

class G15LCDDeviceHelper : public LCDDevice {
protected:
	G15LCDEngineHelper *engine;
	bool bEnabled;

public:
	G15LCDDeviceHelper(G15LCDEngineHelper *e);
	~G15LCDDeviceHelper() Q_DECL_OVERRIDE;
	bool enabled() Q_DECL_OVERRIDE;
	void setEnabled(bool e) Q_DECL_OVERRIDE;
	void blitImage(QImage *img, bool alert) Q_DECL_OVERRIDE;
	QString name() const Q_DECL_OVERRIDE;
	QSize size() const Q_DECL_OVERRIDE;
};

#else
class G15LCDEngineHelper;
class G15LCDDeviceHelper;
#endif
