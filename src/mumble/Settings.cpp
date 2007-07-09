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

#include "Settings.h"
#include "Global.h"

Settings::Settings() {
	atTransmit = VAD;
	bTransmitPosition = false;
	bMute = bDeaf = false;
	bTTS = true;
	iTTSVolume = 75;
	iTTSThreshold = 250;
	iQuality = 6;
	iComplexity = 4;
	iMinLoudness = 4000;
	iVoiceHold = 200;
	iJitterBufferSize = 1;
	iFramesPerPacket = 2;
	bTCPCompat = false;
	bReconnect = true;
	bExpandAll = false;
	bPushClick = false;

	bPlayerTop = false;

	iDXOutputDelay = 5;

#ifdef Q_OS_UNIX
	qsFestival=QLatin1String("/usr/bin/festival --batch --pipe");
	qsALSAInput=QLatin1String("default");
	qsALSAOutput=QLatin1String("default");
#endif

#ifdef Q_OS_WIN
	a3dModel = None;
	fDXMinDistance = 10.0;
	fDXMaxDistance = 50.0;
	fDXRollOff = 0.15;
#endif

	bOverlayEnable = true;
	bOverlayUserTextures=true;
	osOverlay = All;
	bOverlayAlwaysSelf = true;
	fOverlayX = 1.0;
	fOverlayY = 0.0;
	bOverlayTop = false;
	bOverlayBottom = true;
	bOverlayLeft = true;
	bOverlayRight = false;
	qfOverlayFont = QFont(QLatin1String("Arial"), 20);
	fOverlayHeight = 30.0;
	qcOverlayPlayer = QColor(255,255,255,128);
	qcOverlayTalking = QColor(255,255,196,255);
	qcOverlayAltTalking = QColor(255,128,128,255);
	qcOverlayChannel = QColor(192,192,255,192);
	qcOverlayChannelTalking = QColor(224,224,255,255);
}

void Settings::load() {
	bMute = g.qs->value(QLatin1String("AudioMute"), false).toBool();
	bDeaf = g.qs->value(QLatin1String("AudioDeaf"), false).toBool();
	bPlayerTop = g.qs->value(QLatin1String("PlayerTop"), bPlayerTop).toBool();
	bTTS = g.qs->value(QLatin1String("TextToSpeech"), bTTS).toBool();
	iTTSVolume = g.qs->value(QLatin1String("TTSVolume"), iTTSVolume).toInt();
	iTTSThreshold = g.qs->value(QLatin1String("TTSThreshold"), iTTSThreshold).toInt();
	atTransmit = static_cast<Settings::AudioTransmit>(g.qs->value(QLatin1String("AudioTransmit"), atTransmit).toInt());
	bPushClick = g.qs->value(QLatin1String("PushClick"), bPushClick).toBool();
	iQuality = g.qs->value(QLatin1String("AudioQuality"), iQuality).toInt();
	iComplexity = g.qs->value(QLatin1String("AudioComplexity"), iComplexity).toInt();
	iMinLoudness = g.qs->value(QLatin1String("AudioMinLoudness"), iMinLoudness).toInt();
	iVoiceHold = g.qs->value(QLatin1String("AudioVoiceHold"), iVoiceHold).toInt();
	iJitterBufferSize = g.qs->value(QLatin1String("JitterBufferSize2"), iJitterBufferSize).toInt();
	iFramesPerPacket = g.qs->value(QLatin1String("FramesPerPacket"), iFramesPerPacket).toInt();
	bTCPCompat = g.qs->value(QLatin1String("TCPCompat"), bTCPCompat).toBool();
	bReconnect = g.qs->value(QLatin1String("Reconnect"), bReconnect).toBool();
	bExpandAll = g.qs->value(QLatin1String("ExpandAll"), bExpandAll).toBool();
	iDXOutputDelay = g.qs->value(QLatin1String("DXOutputDelay"), iDXOutputDelay).toInt();
#ifdef Q_OS_UNIX
	qsFestival = g.qs->value(QLatin1String("Festival"),qsFestival).toString();
	qsALSAInput = g.qs->value(QLatin1String("ALSAInput"),qsALSAInput).toString();
	qsALSAOutput = g.qs->value(QLatin1String("ALSAOutput"),qsALSAOutput).toString();
#endif
#ifdef USE_ASIO
	qsASIOclass = g.qs->value(QLatin1String("ASIOclass")).toString();
	qlASIOmic = g.qs->value(QLatin1String("ASIOmic")).toList();
	qlASIOspeaker = g.qs->value(QLatin1String("ASIOspeaker")).toList();
#endif
#ifdef Q_OS_WIN
	a3dModel = static_cast<Settings::Audio3D>(g.qs->value(QLatin1String("Audio3D"), a3dModel).toInt());
	qbaDXInput = g.qs->value(QLatin1String("DXInput")).toByteArray();
	qbaDXOutput = g.qs->value(QLatin1String("DXOutput")).toByteArray();
	fDXMinDistance = g.qs->value(QLatin1String("DXMinDistance"), fDXMinDistance).toDouble();
	fDXMaxDistance = g.qs->value(QLatin1String("DXMaxDistance"), fDXMaxDistance).toDouble();
	fDXRollOff = g.qs->value(QLatin1String("DXRollOff"), fDXRollOff).toDouble();
#endif

	bOverlayEnable = g.qs->value(QLatin1String("OverlayEnable"),bOverlayEnable).toBool();
	osOverlay = static_cast<Settings::OverlayShow>(g.qs->value(QLatin1String("OverlayShow"), osOverlay).toInt());
	bOverlayUserTextures = g.qs->value(QLatin1String("OverlayUserTextures"),bOverlayUserTextures).toBool();
	bOverlayAlwaysSelf = g.qs->value(QLatin1String("OverlayAlwaysSelf"),bOverlayAlwaysSelf).toBool();
	fOverlayX = g.qs->value(QLatin1String("OverlayX"),fOverlayX).toDouble();
	fOverlayY = g.qs->value(QLatin1String("OverlayY"),fOverlayY).toDouble();
	bTransmitPosition = g.qs->value(QLatin1String("PosTransmit"),bTransmitPosition).toBool();
	bOverlayTop = g.qs->value(QLatin1String("OverlayTop"),bOverlayTop).toBool();
	bOverlayBottom = g.qs->value(QLatin1String("OverlayBottom"),bOverlayBottom).toBool();
	bOverlayLeft = g.qs->value(QLatin1String("OverlayLeft"),bOverlayLeft).toBool();
	bOverlayRight = g.qs->value(QLatin1String("OverlayRight"),bOverlayRight).toBool();
	qfOverlayFont = qvariant_cast<QFont>(g.qs->value(QLatin1String("OverlayFont"),qfOverlayFont));
	fOverlayHeight = g.qs->value(QLatin1String("OverlayHeight"), fOverlayHeight).toDouble();
	qcOverlayPlayer = qvariant_cast<QColor>(g.qs->value(QLatin1String("OverlayPlayer"),qcOverlayPlayer));
	qcOverlayTalking = qvariant_cast<QColor>(g.qs->value(QLatin1String("OverlayTalking"),qcOverlayTalking));
	qcOverlayChannel = qvariant_cast<QColor>(g.qs->value(QLatin1String("OverlayChannel"),qcOverlayChannel));
	qcOverlayChannelTalking = qvariant_cast<QColor>(g.qs->value(QLatin1String("OverlayChannelTalking"),qcOverlayChannelTalking));
}

void Settings::save() {
	g.qs->setValue(QLatin1String("AudioMute"), g.s.bMute);
	g.qs->setValue(QLatin1String("AudioDeaf"), g.s.bDeaf);
	g.qs->setValue(QLatin1String("PlayerTop"), g.s.bPlayerTop);
	g.qs->setValue(QLatin1String("TextToSpeech"), g.s.bTTS);
	g.qs->setValue(QLatin1String("TTSVolume"), g.s.iTTSVolume);
	g.qs->setValue(QLatin1String("TTSThreshold"), g.s.iTTSThreshold);
	g.qs->setValue(QLatin1String("AudioTransmit"), g.s.atTransmit);
	g.qs->setValue(QLatin1String("PushClick"), g.s.bPushClick);
	g.qs->setValue(QLatin1String("AudioQuality"), iQuality);
	g.qs->setValue(QLatin1String("AudioComplexity"), iComplexity);
	g.qs->setValue(QLatin1String("AudioMinLoudness"), iMinLoudness);
	g.qs->setValue(QLatin1String("AudioVoiceHold"), iVoiceHold);
	g.qs->setValue(QLatin1String("JitterBufferSize2"), iJitterBufferSize);
	g.qs->setValue(QLatin1String("FramesPerPacket"), iFramesPerPacket);
	g.qs->setValue(QLatin1String("TCPCompat"), bTCPCompat);
	g.qs->setValue(QLatin1String("Reconnect"), bReconnect);
	g.qs->setValue(QLatin1String("ExpandAll"), bExpandAll);
	g.qs->setValue(QLatin1String("DXOutputDelay"), iDXOutputDelay);
#ifdef Q_OS_UNIX
	g.qs->setValue(QLatin1String("Festival"), qsFestival);
	g.qs->setValue(QLatin1String("ALSAInput"), qsALSAInput);
	g.qs->setValue(QLatin1String("ALSAOutput"), qsALSAOutput);
#endif
#ifdef USE_ASIO
	g.qs->setValue(QLatin1String("ASIOclass"), qsASIOclass);
	g.qs->setValue(QLatin1String("ASIOmic"), qlASIOmic);
	g.qs->setValue(QLatin1String("ASIOspeaker"), qlASIOspeaker);
#endif
#ifdef Q_OS_WIN
	g.qs->setValue(QLatin1String("PosTransmit"), g.s.bTransmitPosition);
	g.qs->setValue(QLatin1String("Audio3D"), g.s.a3dModel);
	g.qs->setValue(QLatin1String("DXInput"), qbaDXInput);
	g.qs->setValue(QLatin1String("DXOutput"), qbaDXOutput);
	g.qs->setValue(QLatin1String("DXMinDistance"), fDXMinDistance);
	g.qs->setValue(QLatin1String("DXMaxDistance"), fDXMaxDistance);
	g.qs->setValue(QLatin1String("DXRollOff"), fDXRollOff);
#endif
	g.qs->setValue(QLatin1String("OverlayEnable"), bOverlayEnable);
	g.qs->setValue(QLatin1String("OverlayShow"), osOverlay);
	g.qs->setValue(QLatin1String("OverlayAlwaysSelf"), bOverlayAlwaysSelf);
	g.qs->setValue(QLatin1String("OverlayUserTextures"), bOverlayUserTextures);
	g.qs->setValue(QLatin1String("OverlayX"), fOverlayX);
	g.qs->setValue(QLatin1String("OverlayY"), fOverlayY);
	g.qs->setValue(QLatin1String("OverlayTop"), bOverlayTop);
	g.qs->setValue(QLatin1String("OverlayBottom"), bOverlayBottom);
	g.qs->setValue(QLatin1String("OverlayLeft"), bOverlayLeft);
	g.qs->setValue(QLatin1String("OverlayRight"), bOverlayRight);
	g.qs->setValue(QLatin1String("OverlayFont"), qfOverlayFont);
	g.qs->setValue(QLatin1String("OverlayHeight"), fOverlayHeight);
	g.qs->setValue(QLatin1String("OverlayPlayer"), qcOverlayPlayer);
	g.qs->setValue(QLatin1String("OverlayTalking"), qcOverlayTalking);
	g.qs->setValue(QLatin1String("OverlayChannel"), qcOverlayChannel);
	g.qs->setValue(QLatin1String("OverlayChannelTalking"), qcOverlayChannelTalking);
}
