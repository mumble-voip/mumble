/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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
	a3dModel = None;
	bMute = bDeaf = false;
	bTTS = true;
	iTTSVolume = 75;
	iQuality = 6;
	iComplexity = 4;
	iMinLoudness = 4000;
	iVoiceHold = 200;
	iJitterBufferSize = 8;
	iFramesPerPacket = 2;
	bTCPCompat = false;
	bReconnect = true;
	iDXOutputDelay = 5;
	fDXMinDistance = 10.0;
	fDXMaxDistance = 50.0;
	fDXRollOff = 0.15;

	bOverlayEnable = true;
	osOverlay = All;
	bOverlayAlwaysSelf = true;
	fOverlayX = 1.0;
	fOverlayY = 0.0;
	bOverlayTop = false;
	bOverlayBottom = true;
	bOverlayLeft = true;
	bOverlayRight = false;
	bTransmitPosition = false;
	qfOverlayFont = QFont(QString::fromAscii("Arial"), 20);
	fOverlayWidth = 20.0;
	qcOverlayPlayer = QColor(255,255,255,128);
	qcOverlayTalking = QColor(255,255,196,255);
	qcOverlayAltTalking = QColor(255,128,128,255);
	qcOverlayChannel = QColor(192,192,255,192);
	qcOverlayChannelTalking = QColor(224,224,255,255);
}

void Settings::load() {
	bMute = g.qs->value(QString::fromAscii("AudioMute"), false). toBool();
	bDeaf = g.qs->value(QString::fromAscii("AudioDeaf"), false). toBool();
	bTTS = g.qs->value(QString::fromAscii("TextToSpeech"), bTTS). toBool();
	iTTSVolume = g.qs->value(QString::fromAscii("TTSVolume"), iTTSVolume).toInt();
	atTransmit = static_cast<Settings::AudioTransmit>(g.qs->value(QString::fromAscii("AudioTransmit"), atTransmit).toInt());
	iQuality = g.qs->value(QString::fromAscii("AudioQuality"), iQuality).toInt();
	iComplexity = g.qs->value(QString::fromAscii("AudioComplexity"), iComplexity).toInt();
	iMinLoudness = g.qs->value(QString::fromAscii("AudioMinLoudness"), iMinLoudness).toInt();
	iVoiceHold = g.qs->value(QString::fromAscii("AudioVoiceHold"), iVoiceHold).toInt();
	iJitterBufferSize = g.qs->value(QString::fromAscii("JitterBufferSize"), iJitterBufferSize).toInt();
	iFramesPerPacket = g.qs->value(QString::fromAscii("FramesPerPacket"), iFramesPerPacket).toInt();
	bTCPCompat = g.qs->value(QString::fromAscii("TCPCompat"), bTCPCompat).toBool();
	bReconnect = g.qs->value(QString::fromAscii("Reconnect"), bReconnect).toBool();
#ifdef Q_OS_WIN
	a3dModel = static_cast<Settings::Audio3D>(g.qs->value(QString::fromAscii("Audio3D"), a3dModel).toInt());
	iDXOutputDelay = g.qs->value(QString::fromAscii("DXOutputDelay"), iDXOutputDelay).toInt();
	qbaDXInput = g.qs->value(QString::fromAscii("DXInput")).toByteArray();
	qbaDXOutput = g.qs->value(QString::fromAscii("DXOutput")).toByteArray();
	fDXMinDistance = g.qs->value(QString::fromAscii("DXMinDistance"), fDXMinDistance).toDouble();
	fDXMaxDistance = g.qs->value(QString::fromAscii("DXMaxDistance"), fDXMaxDistance).toDouble();
	fDXRollOff = g.qs->value(QString::fromAscii("DXRollOff"), fDXRollOff).toDouble();
	qsASIOclass = g.qs->value(QString::fromAscii("ASIOclass").toString();
	qlASIOmic = g.qs->value(QString::fromAscii("ASIOmic").toList();
	qlASIOspeaker = g.qs->value(QString::fromAscii("ASIOspeaker").toList();
	bOverlayEnable = g.qs->value(QString::fromAscii("OverlayEnable"),bOverlayEnable).toBool();
	osOverlay = static_cast<Settings::OverlayShow>(g.qs->value(QString::fromAscii("OverlayShow"), osOverlay).toInt());
	bOverlayAlwaysSelf = g.qs->value(QString::fromAscii("OverlayAlwaysSelf"),bOverlayAlwaysSelf).toBool();
	fOverlayX = g.qs->value(QString::fromAscii("OverlayX"),fOverlayX).toDouble();
	fOverlayY = g.qs->value(QString::fromAscii("OverlayY"),fOverlayY).toDouble();
	bTransmitPosition = g.qs->value(QString::fromAscii("PosTransmit"),bTransmitPosition).toBool();
	bOverlayTop = g.qs->value(QString::fromAscii("OverlayTop"),bOverlayTop).toBool();
	bOverlayBottom = g.qs->value(QString::fromAscii("OverlayBottom"),bOverlayBottom).toBool();
	bOverlayLeft = g.qs->value(QString::fromAscii("OverlayLeft"),bOverlayLeft).toBool();
	bOverlayRight = g.qs->value(QString::fromAscii("OverlayRight"),bOverlayRight).toBool();
	qfOverlayFont = qvariant_cast<QFont>(g.qs->value(QString::fromAscii("OverlayFont"),qfOverlayFont));
	fOverlayWidth = g.qs->value(QString::fromAscii("OverlayWidth"), fOverlayWidth).toDouble();
	qcOverlayPlayer = qvariant_cast<QColor>(g.qs->value(QString::fromAscii("OverlayPlayer"),qcOverlayPlayer));
	qcOverlayTalking = qvariant_cast<QColor>(g.qs->value(QString::fromAscii("OverlayTalking"),qcOverlayTalking));
	qcOverlayChannel = qvariant_cast<QColor>(g.qs->value(QString::fromAscii("OverlayChannel"),qcOverlayChannel));
	qcOverlayChannelTalking = qvariant_cast<QColor>(g.qs->value(QString::fromAscii("OverlayChannelTalking"),qcOverlayChannelTalking));
#endif
}

void Settings::save() {
	g.qs->setValue(QString::fromAscii("AudioMute"), g.s.bMute);
	g.qs->setValue(QString::fromAscii("AudioDeaf"), g.s.bDeaf);
	g.qs->setValue(QString::fromAscii("TextToSpeech"), g.s.bTTS);
	g.qs->setValue(QString::fromAscii("TTSVolume"), g.s.iTTSVolume);
	g.qs->setValue(QString::fromAscii("AudioTransmit"), g.s.atTransmit);
	g.qs->setValue(QString::fromAscii("AudioQuality"), iQuality);
	g.qs->setValue(QString::fromAscii("AudioComplexity"), iComplexity);
	g.qs->setValue(QString::fromAscii("AudioMinLoudness"), iMinLoudness);
	g.qs->setValue(QString::fromAscii("AudioVoiceHold"), iVoiceHold);
	g.qs->setValue(QString::fromAscii("JitterBufferSize"), iJitterBufferSize);
	g.qs->setValue(QString::fromAscii("FramesPerPacket"), iFramesPerPacket);
	g.qs->setValue(QString::fromAscii("TCPCompat"), bTCPCompat);
	g.qs->setValue(QString::fromAscii("Reconnect"), bReconnect);
#ifdef Q_OS_WIN
	g.qs->setValue(QString::fromAscii("PosTransmit"), g.s.bTransmitPosition);
	g.qs->setValue(QString::fromAscii("Audio3D"), g.s.a3dModel);
	g.qs->setValue(QString::fromAscii("DXOutputDelay"), iDXOutputDelay);
	g.qs->setValue(QString::fromAscii("DXInput"), qbaDXInput);
	g.qs->setValue(QString::fromAscii("DXOutput"), qbaDXOutput);
	g.qs->setValue(QString::fromAscii("DXMinDistance"), fDXMinDistance);
	g.qs->setValue(QString::fromAscii("DXMaxDistance"), fDXMaxDistance);
	g.qs->setValue(QString::fromAscii("DXRollOff"), fDXRollOff);
	g.qs->setValue(QString::fromAscii("ASIOclass"), qsASIOclass);
	g.qs->setValue(QString::fromAscii("ASIOmic"), qlASIOmic);
	g.qs->setValue(QString::fromAscii("ASIOspeaker"), qlASIOspeaker);
	g.qs->setValue(QString::fromAscii("OverlayEnable"), bOverlayEnable);
	g.qs->setValue(QString::fromAscii("OverlayShow"), osOverlay);
	g.qs->setValue(QString::fromAscii("OverlayAlwaysSelf"), bOverlayAlwaysSelf);
	g.qs->setValue(QString::fromAscii("OverlayX"), fOverlayX);
	g.qs->setValue(QString::fromAscii("OverlayY"), fOverlayY);
	g.qs->setValue(QString::fromAscii("OverlayTop"), bOverlayTop);
	g.qs->setValue(QString::fromAscii("OverlayBottom"), bOverlayBottom);
	g.qs->setValue(QString::fromAscii("OverlayLeft"), bOverlayLeft);
	g.qs->setValue(QString::fromAscii("OverlayRight"), bOverlayRight);
	g.qs->setValue(QString::fromAscii("OverlayFont"), qfOverlayFont);
	g.qs->setValue(QString::fromAscii("OverlayWidth"), fOverlayWidth);
	g.qs->setValue(QString::fromAscii("OverlayPlayer"), qcOverlayPlayer);
	g.qs->setValue(QString::fromAscii("OverlayTalking"), qcOverlayTalking);
	g.qs->setValue(QString::fromAscii("OverlayChannel"), qcOverlayChannel);
	g.qs->setValue(QString::fromAscii("OverlayChannelTalking"), qcOverlayChannelTalking);
#endif
}
