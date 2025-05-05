// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LCD_H_
#define MUMBLE_MUMBLE_LCD_H_

#include "ConfigDialog.h"
#include "Timer.h"

#include "ui_LCD.h"

class User;
class LCDDevice;

class LCDConfig : public ConfigWidget, public Ui::LCDConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LCDConfig)
public:
	/// The unique name of this ConfigWidget
	static const QString name;
	LCDConfig(Settings &st);
	QString title() const Q_DECL_OVERRIDE;
	const QString &getName() const Q_DECL_OVERRIDE;
	QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void on_qsMinColWidth_valueChanged(int v);
	void on_qsSplitterWidth_valueChanged(int v);
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
};

class LCDEngine : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LCDEngine)
protected:
	QList< LCDDevice * > qlDevices;

public:
	LCDEngine();
	virtual ~LCDEngine() Q_DECL_OVERRIDE;
	virtual QList< LCDDevice * > devices() const = 0;
};

class LCDDevice {
public:
	LCDDevice();
	virtual ~LCDDevice();
	virtual bool enabled()                                  = 0;
	virtual void setEnabled(bool e)                         = 0;
	virtual void blitImage(QImage *img, bool alert = false) = 0;
	virtual QString name() const                            = 0;
	virtual QSize size() const                              = 0;
};

typedef LCDEngine *(*LCDEngineNew)(void);

class LCDEngineRegistrar Q_DECL_FINAL {
protected:
	LCDEngineNew n;

public:
	static QList< LCDEngineNew > *qlInitializers;
	LCDEngineRegistrar(LCDEngineNew n);
	~LCDEngineRegistrar();
};

class LCD : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(LCD)
protected:
	QFont qfNormal, qfBold, qfItalic, qfItalicBold;
	QMap< unsigned int, Timer > qmSpeaking;
	QMap< unsigned int, Timer > qmNew;
	QMap< unsigned int, Timer > qmOld;
	QMap< unsigned int, QString > qmNameCache;

	int iFontHeight;
	int iFrameIndex;
	QHash< QSize, unsigned char * > qhImageBuffers;
	QHash< QSize, QImage * > qhImages;
	void initBuffers();
	void destroyBuffers();
	QImage qiLogo;
	QTimer *qtTimer;
public slots:
	void tick();

public:
	LCD();
	~LCD() Q_DECL_OVERRIDE;
	void updateUserView();
	bool hasDevices();
};

uint qHash(const QSize &size);

#endif
