/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "mumble_pch.hpp"

// Global helper class to spread variables around across threads
// especially helpfull to initialize things like the stored
// preference for audio transmission, since the GUI elements
// will be created long before the AudioInput object, and the
// latter lives in a separate thread and so cannot touch the
// GUI.

struct Settings {
	enum AudioTransmit { Continous, VAD, PushToTalk };
	enum VADSource { Amplitude, SignalToNoise };
	enum LoopMode { None, Local, Server };
	enum OverlayShow { Nothing, Talking, All };
	AudioTransmit atTransmit;
	quint64 uiDoublePush;

	bool bExpert;

	bool bTransmitPosition;
	bool bPushClick;
	bool bMute, bDeaf;
	bool bTTS;
	bool bPlayerTop;
	int iTTSVolume, iTTSThreshold;
	int iQuality, iComplexity, iMinLoudness, iVoiceHold, iJitterBufferSize;
	int iNoiseSuppress;
	VADSource vsVAD;
	float fVADmin, fVADmax;
	int iFramesPerPacket;
	bool bExpandAll;
	QString qsAudioInput, qsAudioOutput;
	float fVolume;
	int iOutputDelay;

	QString qsALSAInput, qsALSAOutput;
	QString qsPulseAudioInput, qsPulseAudioOutput;
	QString qsOSSInput, qsOSSOutput;
	int iPortAudioInput, iPortAudioOutput;
	QString qsASIOclass;
	QList<QVariant> qlASIOmic;
	QList<QVariant> qlASIOspeaker;

	QString qsWASAPIInput, qsWASAPIOutput;

	QByteArray qbaDXInput, qbaDXOutput;

	bool bEcho;
	bool bPositionalAudio;
	float fAudioMinDistance, fAudioMaxDistance, fAudioRollOff, fAudioMinVolume, fAudioBloom;

	bool bOverlayEnable;
	bool bOverlayUserTextures;
	OverlayShow osOverlay;
	bool bOverlayAlwaysSelf;
	float fOverlayX;
	float fOverlayY;
	bool bOverlayLeft, bOverlayRight, bOverlayTop, bOverlayBottom;
	QFont qfOverlayFont;
	float fOverlayHeight;
	QColor qcOverlayPlayer;
	QColor qcOverlayTalking;
	QColor qcOverlayAltTalking;
	QColor qcOverlayChannel;
	QColor qcOverlayChannelTalking;

	typedef QMap<int, QList<QVariant > > ShortcutMap;
	ShortcutMap qmShortcuts;

	enum MessageLog { LogConsole = 0x01, LogTTS = 0x02};
	QMap<int, quint32> qmMessages;

	QString qsLanguage;
	QString qsStyle;
	QString qsSkin;
	bool bHorizontal;
	bool bFirstTime;
	QByteArray qbaMainWindowGeometry, qbaMainWindowState, qbaSplitterState, qbaHeaderState;

	QString qsUsername;
	int iServerRow;

	bool bUpdateCheck;

	// Network settings
	enum ProxyType { NoProxy, HttpProxy, Socks5Proxy };
	bool bTCPCompat, bReconnect;
	ProxyType ptProxyType;
	QString qsProxyHost, qsProxyUsername, qsProxyPassword;
	unsigned short usProxyPort;

	// Nonsaved
	LoopMode lmLoopMode;
	float dPacketLoss;
	float dMaxPacketDelay;

	bool doEcho() const;
	bool doPositionalAudio() const;

	Settings();
	void load();
	void save();
};

#else
struct Settings;
#endif
