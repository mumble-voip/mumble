/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "ASIOInput.h"
#include "MainWindow.h"
#include "Global.h"

/*
 * This file blatanly assumes the sampling frequency is 48000Hz.
 * Anything else is an error, and to fix it all the constants
 * of 960 need to be replaced.
 * (960 == 20 ms of samples at 48000)
 */

class ASIOAudioInputRegistrar : public AudioInputRegistrar {
	public:
		ASIOAudioInputRegistrar();
		virtual AudioInput *create();
		virtual const QList<audioDevice> getDeviceChoices();
		virtual void setDeviceChoice(const QVariant &);

};

// Static singleton
static ASIOAudioInputRegistrar airASIO;

ASIOAudioInputRegistrar::ASIOAudioInputRegistrar() : AudioInputRegistrar(QLatin1String("ASIO")) {
}

AudioInput *ASIOAudioInputRegistrar::create() {
	return new ASIOInput();
}
const QList<audioDevice> ASIOAudioInputRegistrar::getDeviceChoices() {
	QList<audioDevice> qlReturn;
	return qlReturn;
}

void ASIOAudioInputRegistrar::setDeviceChoice(const QVariant &) {
	qWarning("ASIOInputRegistrar::setDeviceChoice was called");
}

static ConfigWidget *ASIOConfigDialogNew(Settings &st) {
	return new ASIOConfig(st);
}

static ConfigRegistrar registrar(22, ASIOConfigDialogNew);


ASIOInput *ASIOInput::aiSelf;

ASIOConfig::ASIOConfig(Settings &st) : ConfigWidget(st) {
	HKEY hkDevs;
	HKEY hk;
	DWORD idx = 0;
	WCHAR keyname[255];
	DWORD keynamelen = 255;
	FILETIME ft;
	HRESULT hr;

	setupUi(this);

	// List of devices known to misbehave or be totally useless
	QStringList blacklist;
	blacklist << QLatin1String("{a91eaba1-cf4c-11d3-b96a-00a0c9c7b61a}"); // ASIO DirectX
	blacklist << QLatin1String("{e3186861-3a74-11d1-aef8-0080ad153287}"); // ASIO Multimedia

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\ASIO", 0, KEY_READ, &hkDevs) == ERROR_SUCCESS) {
		while (RegEnumKeyEx(hkDevs, idx++, keyname, &keynamelen, NULL, NULL, NULL, &ft)  == ERROR_SUCCESS) {
			QString name=QString::fromUtf16(reinterpret_cast<ushort *>(keyname),keynamelen);
			if (RegOpenKeyEx(hkDevs, keyname, 0, KEY_READ, &hk) == ERROR_SUCCESS) {
				DWORD dtype = REG_SZ;
				WCHAR wclsid[255];
				DWORD datasize = 255;
				CLSID clsid;
				if (RegQueryValueEx(hk, L"CLSID", 0, &dtype, reinterpret_cast<BYTE *>(wclsid), &datasize) == ERROR_SUCCESS) {
					QString qsCls = QString::fromUtf16(reinterpret_cast<ushort *>(wclsid), datasize / 2);
					if (! blacklist.contains(qsCls) && ! FAILED(hr =CLSIDFromString(wclsid, &clsid))) {
						ASIODev ad(name, qsCls);
						qlDevs << ad;
					}
				}
				RegCloseKey(hk);
			}
			keynamelen = 255;
		}
		RegCloseKey(hkDevs);
	}

	bOk = false;

	ASIODev ad;

	foreach(ad, qlDevs) {
		qcbDevice->addItem(ad.first, QVariant(ad.second));
	}

	if (qlDevs.count() == 0) {
		qpbQuery->setEnabled(false);
		qpbConfig->setEnabled(false);
	}
}

#include "iasiothiscallresolver.h"

void ASIOConfig::on_qpbQuery_clicked() {
	QString qsCls = qcbDevice->itemData(qcbDevice->currentIndex()).toString();
	CLSID clsid;
	IASIO *iorigasio;

	bool ok = false;

	clearQuery();

	CLSIDFromString(const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(qsCls.utf16())), &clsid);
	if (CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, clsid, reinterpret_cast<void **>(&iorigasio)) == S_OK) {
		SleepEx(10, false);
		IASIOThiscallResolver iasio(iorigasio);
		if (iasio.init(winId())) {
			SleepEx(10, false);
			char buff[512];
			memset(buff, 0, 512);

			iasio.getDriverName(buff);
			SleepEx(10, false);

			long ver = iasio.getDriverVersion();
			SleepEx(10, false);

			ASIOSampleRate srate;
			iasio.getSampleRate(&srate);
			SleepEx(10, false);
			if (fabs(srate-48000.0) > 1.0) {
				iasio.setSampleRate(48000.0);
				SleepEx(10, false);
				iasio.getSampleRate(&srate);
				SleepEx(10, false);
			}

			long minSize, maxSize, prefSize, granSize;
			iasio.getBufferSize(&minSize, &maxSize, &prefSize, &granSize);
			SleepEx(10, false);

			// We need sample sizes in ms for 2 bytes at 48khz

			bOk = false;

			if ((fabs(srate-48000.0) < 1.0) && (minSize <= 1920) && (maxSize >= 1920)) {
				if (granSize > 0) {
					long remain;
					remain = 1920 - minSize;
					if ((remain % granSize) == 0)
						bOk = true;
				} else if (granSize == -1) {
					long v = minSize;
					for (int i=0;i<100;i++) {
						if (v == 1920) {
							bOk = true;
							break;
						}
						v = v * 2;
					}
				}
			}

			long divider = lround(srate * 2.0 / 1000.0);
			if (divider < 1)
				divider = 1;

			if (granSize > 0)
				granSize /= divider;
			minSize /= divider;
			maxSize /= divider;
			prefSize /= divider;

			QString str = tr("%1 (ver %2)").arg(QLatin1String(buff)).arg(ver);
			qlName->setText(str);

			if (bOk)
				str = tr("%1 ms -> %2 ms (%3 ms resolution) %4Hz").arg(minSize).arg(maxSize).arg(granSize).arg(srate,0,'f',0);
			else
				str = tr("%1 ms -> %2 ms (%3 ms resolution) %4Hz -- Unusable").arg(minSize).arg(maxSize).arg(granSize).arg(srate,0,'f',0);

			qlBuffers->setText(str);

			if (bOk) {
				long ichannels, ochannels;
				iasio.getChannels(&ichannels, &ochannels);
				SleepEx(10, false);
				long cnum;

				bool match = (g.s.qsASIOclass == qsCls);
				for (cnum=0;cnum<ichannels;cnum++) {
					ASIOChannelInfo aci;
					aci.channel = cnum;
					aci.isInput = true;
					iasio.getChannelInfo(&aci);
					SleepEx(10, false);
					switch (aci.type) {
						case ASIOSTInt32LSB:
						case ASIOSTInt16LSB: {
								QListWidget *widget = qlwUnused;
								QVariant v = static_cast<int>(cnum);
								if (match && s.qlASIOmic.contains(v))
									widget = qlwMic;
								else if (match && s.qlASIOspeaker.contains(v))
									widget = qlwSpeaker;
								QListWidgetItem *item = new QListWidgetItem(QLatin1String(aci.name), widget);
								item->setData(Qt::UserRole, static_cast<int>(cnum));
							}
							break;
						default:
							qWarning("ASIOInput: Channel %ld %s (Unusable format %ld)", cnum, aci.name,aci.type);
					}
				}
			}

			ok = true;
		}
		if (! ok) {
			SleepEx(10, false);
			char err[255];
			iasio.getErrorMessage(err);
			SleepEx(10, false);
			QMessageBox::critical(this, tr("Mumble"), tr("ASIO Initialization failed: %1").arg(QLatin1String(err)), QMessageBox::Ok, QMessageBox::NoButton);
		}
		iasio.Release();
	} else {
		QMessageBox::critical(this, tr("Mumble"), tr("Failed to instantiate ASIO driver"), QMessageBox::Ok, QMessageBox::NoButton);
	}
}

void ASIOConfig::on_qpbConfig_clicked() {
	QString qsCls = qcbDevice->itemData(qcbDevice->currentIndex()).toString();
	CLSID clsid;
	IASIO *iorigasio;

	bool ok = false;

	CLSIDFromString(const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(qsCls.utf16())), &clsid);
	if (CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, clsid, reinterpret_cast<void **>(&iorigasio)) == S_OK) {
		SleepEx(10, false);
		IASIOThiscallResolver iasio(iorigasio);
		if (iasio.init(winId())) {
			SleepEx(10, false);
			ok = (iasio.controlPanel() == ASE_OK);
			SleepEx(10, false);
		} else {
			SleepEx(10, false);
			char err[255];
			iasio.getErrorMessage(err);
			SleepEx(10, false);
			QMessageBox::critical(this, tr("Mumble"), tr("ASIO Initialization failed: %1").arg(QLatin1String(err)), QMessageBox::Ok, QMessageBox::NoButton);
		}
		iasio.Release();
	} else {
		QMessageBox::critical(this, tr("Mumble"), tr("Failed to instantiate ASIO driver"), QMessageBox::Ok, QMessageBox::NoButton);
	}
}

void ASIOConfig::on_qcbDevice_activated(int) {
	clearQuery();
}

void ASIOConfig::on_qpbAddMic_clicked() {
	int row = qlwUnused->currentRow();
	if (row < 0)
		return;
	qlwMic->addItem(qlwUnused->takeItem(row));
}

void ASIOConfig::on_qpbRemMic_clicked() {
	int row = qlwMic->currentRow();
	if (row < 0)
		return;
	qlwUnused->addItem(qlwMic->takeItem(row));
}

void ASIOConfig::on_qpbAddSpeaker_clicked() {
	int row = qlwUnused->currentRow();
	if (row < 0)
		return;
	qlwSpeaker->addItem(qlwUnused->takeItem(row));
}

void ASIOConfig::on_qpbRemSpeaker_clicked() {
	int row = qlwSpeaker->currentRow();
	if (row < 0)
		return;
	qlwUnused->addItem(qlwSpeaker->takeItem(row));
}

QString ASIOConfig::title() const {
	return tr("ASIO");
}

QIcon ASIOConfig::icon() const {
	return QIcon(QLatin1String("skin:config_asio.png"));
}

void ASIOConfig::save() const {
	if (! bOk)
		return;

	s.qsASIOclass = qcbDevice->itemData(qcbDevice->currentIndex()).toString();

	QList<QVariant> list;

	for (int i=0;i<qlwMic->count();i++) {
		QListWidgetItem *item = qlwMic->item(i);
		list << item->data(Qt::UserRole);
	}

	s.qlASIOmic = list;

	list.clear();

	for (int i=0;i<qlwSpeaker->count();i++) {
		QListWidgetItem *item = qlwSpeaker->item(i);
		list << item->data(Qt::UserRole);
	}

	s.qlASIOspeaker = list;
}

void ASIOConfig::load(const Settings &r) {
	int i = 0;
	ASIODev ad;
	foreach(ad, qlDevs) {
		if (ad.second == r.qsASIOclass) {
			qcbDevice->setCurrentIndex(i);
		}
		i++;
	}
	s.qlASIOmic = r.qlASIOmic;
	s.qlASIOspeaker = r.qlASIOspeaker;

	qlName->setText(QString());
	qlBuffers->setText(QString());
	qlwMic->clear();
	qlwUnused->clear();
	qlwSpeaker->clear();
}

void ASIOConfig::clearQuery() {
	bOk = false;
	qlName->setText(QString());
	qlBuffers->setText(QString());
	qlwMic->clear();
	qlwUnused->clear();
	qlwSpeaker->clear();
}



#include "firmumble.inc"

ASIOInput::ASIOInput() {
	QString qsCls = g.s.qsASIOclass;
	CLSID clsid;
	IASIO *iorigasio;

	iorigasio = false;
	iasio = NULL;
	abiInfo = NULL;
	aciInfo = NULL;

	int i, idx;

	bHasSpeaker = true;

	// Allocate buffers
	pdInputBuffer = new double[960];
	pdOutputBuffer = new double[iFrameSize];
	pdMicDelayLine = new double[MUMBLE_SFP_LENGTH];
	pdSpeakerDelayLine = new double[MUMBLE_SFP_LENGTH];

	for (i=0;i<MUMBLE_SFP_LENGTH;i++)
		pdMicDelayLine[i]=pdSpeakerDelayLine[i] = 0.0;

	// Sanity check things first.

	iNumMic=g.s.qlASIOmic.count();
	iNumSpeaker=g.s.qlASIOspeaker.count();

	if ((iNumMic == 0) || (iNumSpeaker == 0)) {
		QMessageBox::warning(NULL, tr("Mumble"), tr("You need to select at least one microphone and one speaker source to use ASIO. "
		                     "If you just need microphone sampling, use DirectSound."),  QMessageBox::Ok, QMessageBox::NoButton);
		return;
	}

	CLSIDFromString(const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(qsCls.utf16())), &clsid);
	if (CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, clsid, reinterpret_cast<void **>(&iaOriginal)) == S_OK) {
		iasio = new IASIOThiscallResolver(iaOriginal);
		if (iasio->init(NULL)) {
			ASIOSampleRate srate = 0.0;
			iasio->getSampleRate(&srate);
			if ((fabs(srate - 48000.0) < 1.0) || (iasio->setSampleRate(48000.0) == ASE_OK)) {

				abiInfo = new ASIOBufferInfo[iNumMic + iNumSpeaker];
				aciInfo = new ASIOChannelInfo[iNumMic + iNumSpeaker];
				idx = 0;
				for (i=0;i<iNumMic;i++) {
					abiInfo[idx].isInput = true;
					abiInfo[idx].channelNum = g.s.qlASIOmic[i].toInt();

					aciInfo[idx].channel = abiInfo[idx].channelNum;
					aciInfo[idx].isInput = true;
					iasio->getChannelInfo(&aciInfo[idx]);
					SleepEx(10, false);

					idx++;
				}
				for (i=0;i<iNumSpeaker;i++) {
					abiInfo[idx].isInput = true;
					abiInfo[idx].channelNum = g.s.qlASIOspeaker[i].toInt();

					aciInfo[idx].channel = abiInfo[idx].channelNum;
					aciInfo[idx].isInput = true;
					iasio->getChannelInfo(&aciInfo[idx]);
					SleepEx(10, false);

					idx++;
				}

				ASIOCallbacks asioCallbacks;
				ZeroMemory(&asioCallbacks, sizeof(asioCallbacks));
				asioCallbacks.bufferSwitch = &bufferSwitch;
				asioCallbacks.sampleRateDidChange = &sampleRateChanged;
				asioCallbacks.asioMessage = &asioMessages;
				asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;

				if (iasio->createBuffers(abiInfo, idx, 960, &asioCallbacks) == ASE_OK)
					return;
			}
		}
	}
	if (abiInfo) {
		delete [] abiInfo;
		abiInfo = NULL;
	}

	if (aciInfo) {
		delete [] aciInfo;
		aciInfo = NULL;
	}

	if (iasio) {
		iaOriginal->Release();
		delete iasio;

		iasio = NULL;
		iaOriginal = NULL;
	}

	QMessageBox::critical(NULL, tr("Mumble"), tr("Opening selected ASIO device failed. No input will be done."),
	                      QMessageBox::Ok, QMessageBox::NoButton);

}

ASIOInput::~ASIOInput() {
	qwDone.wakeAll();
	wait();
	if (iasio) {
		iasio->stop();
		iasio->disposeBuffers();
		iaOriginal->Release();
		delete iasio;
		iasio = NULL;
	}
	if (abiInfo) {
		delete [] abiInfo;
		abiInfo = NULL;
	}

	delete [] pdInputBuffer;
	delete [] pdOutputBuffer;
	delete [] pdMicDelayLine;
	delete [] pdSpeakerDelayLine;
}

void ASIOInput::run() {
	QMutex m;
	m.lock();
	bRunning = true;
	if (iasio) {
		aiSelf = this;
		iasio->start();
		qwDone.wait(&m);
	}
}

ASIOTime *ASIOInput::bufferSwitchTimeInfo(ASIOTime *, long index, ASIOBool) {
	aiSelf->bufferReady(index);
	return 0L;
}

void ASIOInput::addBuffer(ASIOSampleType sampType, void *src, double *dst) {
	switch (sampType) {
		case ASIOSTInt16LSB: {
				short *buf=static_cast<short *>(src);
				for (int i=0;i<960;i++)
					dst[i]+=buf[i];
			}
			break;
		case ASIOSTInt32LSB: {
				int *buf=static_cast<int *>(src);
				for (int i=0;i<960;i++)
					dst[i]+=(buf[i] >> 16);
			}
			break;
	}
}

void ASIOInput::bufferReady(long buffindex) {
	int c, i;

	// Microphone inputs
	ZeroMemory(pdInputBuffer, sizeof(double) * 960);
	for (c=0;c<iNumMic;c++)
		addBuffer(aciInfo[c].type, abiInfo[c].buffers[buffindex], pdInputBuffer);

	double mul = 1.0 / (32768.0 * iNumMic);

	for (i=0;i<960;i++) {
		pdInputBuffer[i] *= mul;
	}

	decim(pdInputBuffer, pdOutputBuffer, pdMicDelayLine);

	for (i=0;i<320;i++)
		psMic[i] = static_cast<short>(pdOutputBuffer[i] * 32768.0);


	// Speaker inputs
	ZeroMemory(pdInputBuffer, sizeof(double) * 960);
	for (c=0;c<iNumMic;c++)
		addBuffer(aciInfo[iNumMic+c].type, abiInfo[iNumMic+c].buffers[buffindex], pdInputBuffer);

	mul = 1.0 / (32768.0 * iNumSpeaker);

	for (i=0;i<960;i++) {
		pdInputBuffer[i] *= mul;
	}

	decim(pdInputBuffer, pdOutputBuffer, pdSpeakerDelayLine);

	for (i=0;i<320;i++)
		psSpeaker[i] = static_cast<short>(pdOutputBuffer[i] * 32768.0);

	encodeAudioFrame();
}

void ASIOInput::bufferSwitch(long index, ASIOBool processNow) {
	ASIOTime  timeInfo;
	memset(&timeInfo, 0, sizeof(timeInfo));

	if (aiSelf->iasio->getSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

	bufferSwitchTimeInfo(&timeInfo, index, processNow);
}

void ASIOInput::sampleRateChanged(ASIOSampleRate) {
	qFatal("ASIOInput: sampleRateChanged");
}

long ASIOInput::asioMessages(long selector, long value, void*, double*) {
	long ret = 0;
	switch (selector) {
		case kAsioSelectorSupported:
			if (value == kAsioResetRequest
			        || value == kAsioEngineVersion
			        || value == kAsioResyncRequest
			        || value == kAsioLatenciesChanged
			        || value == kAsioSupportsTimeInfo
			        || value == kAsioSupportsTimeCode
			        || value == kAsioSupportsInputMonitor)
				ret = 1L;
			break;
		case kAsioResetRequest:
			qFatal("ASIOInput: kAsioResetRequest");
			ret = 1L;
			break;
		case kAsioResyncRequest:
			ret = 1L;
			break;
		case kAsioLatenciesChanged:
			ret = 1L;
			break;
		case kAsioEngineVersion:
			ret = 2L;
			break;
		case kAsioSupportsTimeInfo:
			ret = 1;
			break;
		case kAsioSupportsTimeCode:
			ret = 0;
			break;
	}
	return ret;
}

/****************************************************************************
*
* The following subroutine (ASIOInput::decim()) was adopted from decim.c
* found at dspguru.
* Original copyright notice as follows:
*
* Name: decim.c
*
* Synopsis: Decimates a real or complex signal.
*
* Description: See decim.h.
*
* by Grant R. Griffin
* Provided by Iowegian's "dspGuru" service (http://www.dspguru.com).
* Copyright 2001, Iowegian International Corporation (http://www.iowegian.com)
*
*                          The Wide Open License (WOL)
*
* Permission to use, copy, modify, distribute and sell this software and its
* documentation for any purpose is hereby granted without fee, provided that
* the above copyright notice and this license appear in all source copies.
* THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF
* ANY KIND. See http://www.dspguru.com/wol.htm for more information.
*
*****************************************************************************/


#define factor_M 3
#define H_size MUMBLE_SFP_LENGTH
#define p_H mumble_sfp

void ASIOInput::decim(const double *p_inp, double *p_out, double *p_Z) {
	int tap;
	double sum;
	int num_inp = 960;

	while (num_inp >= factor_M) {
		/* shift Z delay line up to make room for next samples */
		for (tap = H_size - 1; tap >= factor_M; tap--) {
			p_Z[tap] = p_Z[tap - factor_M];
		}

		/* copy next samples from input buffer to bottom of Z delay line */
		for (tap = factor_M - 1; tap >= 0; tap--) {
			p_Z[tap] = *p_inp++;
		}
		num_inp -= factor_M;

		/* calculate FIR sum */
		sum = 0.0;
		for (tap = 0; tap < H_size; tap++) {
			sum += p_H[tap] * p_Z[tap];
		}
		*p_out++ = sum;     /* store sum and point to next output */
	}
}
