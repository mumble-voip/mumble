// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ASIOINPUT_H_
#define MUMBLE_MUMBLE_ASIOINPUT_H_

#include "ui_ASIOInput.h"

#include "AudioInput.h"
#include "ConfigDialog.h"

#include "win.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QWaitCondition>

#include <combaseapi.h>

#include <asiodrvr.h>

typedef QPair< QString, QString > ASIODev;

class ASIOConfig : public ConfigWidget, public Ui::ASIOConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ASIOConfig)
protected:
	QList< ASIODev > qlDevs;
	bool bOk;

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	ASIOConfig(Settings &st);
	virtual QString title() const Q_DECL_OVERRIDE;
	virtual const QString &getName() const Q_DECL_OVERRIDE;
	virtual QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
	void clearQuery();
	void on_qcbDevice_activated(int index);
	void on_qpbQuery_clicked();
	void on_qpbConfig_clicked();
	void on_qpbAddMic_clicked();
	void on_qpbRemMic_clicked();
	void on_qpbAddSpeaker_clicked();
	void on_qpbRemSpeaker_clicked();
};

#define IEEE754_64FLOAT 1
#include "asio.h"

class ASIOInput : public AudioInput {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ASIOInput)
protected:
	IASIO *iasio;

	int iNumMic, iNumSpeaker;
	long lBufSize;
	ASIOBufferInfo *abiInfo;
	ASIOChannelInfo *aciInfo;

	// ASIO Callbacks
	static ASIOInput *aiSelf;

	static void sampleRateChanged(ASIOSampleRate sRate);
	static long asioMessages(long selector, long value, void *message, double *opt);
	static void bufferSwitch(long index, ASIOBool processNow);
	static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);

	void addBuffer(long sampType, int interleave, void *src, float *RESTRICT dst);
	void bufferReady(long index);
	bool initializeDriver();

	QWaitCondition qwDone;

public:
	ASIOInput();
	~ASIOInput() Q_DECL_OVERRIDE;
	void run() Q_DECL_OVERRIDE;
};

#endif
