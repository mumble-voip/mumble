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

#ifndef _SETTINGS_H
#define _SETTINGS_H

// Global helper class to spread variables around across threads
// especially helpfull to initialize things like the stored
// preference for audio transmission, since the GUI elements
// will be created long before the AudioInput object, and the
// latter lives in a separate thread and so cannot touch the
// GUI.

struct Settings {
	enum AudioTransmit { Continous, VAD, PushToTalk };
	AudioTransmit atTransmit;
	bool bTransmitPosition;
	bool bMute, bDeaf;
	bool bTTS;
	int iTTSVolume;
	int iQuality, iComplexity, iMinLoudness, iVoiceHold, iJitterBufferSize;
	int iFramesPerPacket;
	bool bTCPCompat, bReconnect, bExpandAll;
	QString qsAudioInput, qsAudioOutput;

	int iDXOutputDelay;
	
#ifdef Q_OS_UNIX
	QString qsFestival;
#endif

#ifdef USE_ASIO
	QString qsASIOclass;
	QList<QVariant> qlASIOmic;
	QList<QVariant> qlASIOspeaker;
#endif

#ifdef Q_OS_WIN
	enum Audio3D { None, Panning, Light, Full };
	enum OverlayShow { Nothing, Talking, All };
	Audio3D a3dModel;

	QByteArray qbaDXInput, qbaDXOutput;
	float fDXMinDistance, fDXMaxDistance, fDXRollOff;

	bool bOverlayEnable;
	OverlayShow osOverlay;
	bool bOverlayAlwaysSelf;
	float fOverlayX;
	float fOverlayY;
	bool bOverlayLeft, bOverlayRight, bOverlayTop, bOverlayBottom;
	QFont qfOverlayFont;
	float fOverlayWidth;
	QColor qcOverlayPlayer;
	QColor qcOverlayTalking;
	QColor qcOverlayAltTalking;
	QColor qcOverlayChannel;
	QColor qcOverlayChannelTalking;
#endif

	Settings();
	void load();
	void save();
};

#else
class Settings;
#endif
