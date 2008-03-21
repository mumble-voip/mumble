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

#include "Settings.h"
#include "Log.h"
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
	fVolume = 1.0f;
	iMinLoudness = 1000;
	iVoiceHold = 100;
	iJitterBufferSize = 1;
	iFramesPerPacket = 2;
	iNoiseSuppress = -30;
	vsVAD = SignalToNoise;
	fVADmin = 0.80f;
	fVADmax = 0.98f;
	bExpandAll = true;
	bPushClick = false;

	bPlayerTop = false;

	uiDoublePush = 0;
	bExpert = false;

#ifdef NO_UPDATE_CHECK
	bUpdateCheck = false;
#else
	bUpdateCheck = true;
#endif

	bFirstTime = true;
	bHorizontal = true;

	iOutputDelay = 5;

	qsALSAInput=QLatin1String("default");
	qsALSAOutput=QLatin1String("default");

	bWASAPIEcho = false;
	bPulseAudioEcho = false;

	iPortAudioInput = -1; // default device
	iPortAudioOutput = -1; // default device

	a3dModel = No3D;
	fDXMinDistance = 10.0f;
	fDXMaxDistance = 50.0f;
	fDXRollOff = 0.15f;

	bPositionalSoundEnable = true;
	bPositionalSoundSwap = false;
	ePositionalSoundModel = CONSTANT;
	fPositionalSoundDistance= 70;
	fPositionalSoundPreGain = 0;
	fPositionalSoundMaxAtt = 10;


	bOverlayEnable = true;
	bOverlayUserTextures=true;
	osOverlay = All;
	bOverlayAlwaysSelf = true;
	fOverlayX = 1.0f;
	fOverlayY = 0.0f;
	bOverlayTop = false;
	bOverlayBottom = true;
	bOverlayLeft = true;
	bOverlayRight = false;
	qfOverlayFont = QFont(QLatin1String("Arial"), 20);
	fOverlayHeight = 30.0f;
	qcOverlayPlayer = QColor(255,255,255,128);
	qcOverlayTalking = QColor(255,255,196,255);
	qcOverlayAltTalking = QColor(255,128,128,255);
	qcOverlayChannel = QColor(192,192,255,192);
	qcOverlayChannelTalking = QColor(224,224,255,255);

	// Network settings
	bTCPCompat = false;
	bReconnect = true;
	ptProxyType = NoProxy;
	usProxyPort = 0;

#ifdef AUDIO_TEST
	lmLoopMode = Local;
#else
	lmLoopMode = None;
#endif
	dPacketLoss = 0;
	dMaxPacketDelay = 0.0f;

	for (int i=Log::firstMsgType;i<=Log::lastMsgType;++i)
		qmMessages.insert(i, Settings::LogConsole | Settings::LogTTS);

	iServerRow = -1;
}

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()


BOOST_TYPEOF_REGISTER_TYPE(Settings::AudioTransmit);
BOOST_TYPEOF_REGISTER_TYPE(Settings::VADSource);
BOOST_TYPEOF_REGISTER_TYPE(Settings::LoopMode)
BOOST_TYPEOF_REGISTER_TYPE(Settings::OverlayShow)
BOOST_TYPEOF_REGISTER_TYPE(Settings::Audio3D)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ProxyType)
BOOST_TYPEOF_REGISTER_TYPE(Settings::PositionalSoundModels)
BOOST_TYPEOF_REGISTER_TYPE(QString)
BOOST_TYPEOF_REGISTER_TYPE(QByteArray)
BOOST_TYPEOF_REGISTER_TYPE(QColor)
BOOST_TYPEOF_REGISTER_TYPE(QVariant)
BOOST_TYPEOF_REGISTER_TYPE(QFont)
BOOST_TYPEOF_REGISTER_TEMPLATE(QList, 1)

#define SAVELOAD(var,name) var = qvariant_cast<BOOST_TYPEOF(var)>(g.qs->value(QLatin1String(name), var))
#define LOADENUM(var, name) var = static_cast<BOOST_TYPEOF(var)>(g.qs->value(QLatin1String(name), var).toInt())

void Settings::load() {
	SAVELOAD(bMute, "audio/mute");
	SAVELOAD(bDeaf, "audio/deaf");
	LOADENUM(atTransmit, "audio/transmit");
	SAVELOAD(uiDoublePush, "audio/doublepush");
	SAVELOAD(bPushClick, "audio/pushclick");
	SAVELOAD(iQuality, "audio/quality");
	SAVELOAD(iComplexity, "audio/complexity");
	SAVELOAD(iMinLoudness, "audio/loudness");
	SAVELOAD(fVolume, "audio/volume");
	LOADENUM(vsVAD, "audio/vadsource");
	SAVELOAD(fVADmin, "audio/vadmin");
	SAVELOAD(fVADmax, "audio/vadmax");
	SAVELOAD(iNoiseSuppress, "audio/noisesupress");
	SAVELOAD(iVoiceHold, "audio/voicehold");
	SAVELOAD(iOutputDelay, "audio/outputdelay");
	LOADENUM(a3dModel, "audio/3dmode");
	SAVELOAD(fDXMinDistance, "audio/mindistance");
	SAVELOAD(fDXMaxDistance, "audio/maxdistance");
	SAVELOAD(fDXRollOff, "audio/rolloff");
	SAVELOAD(qsAudioInput, "audio/input");
	SAVELOAD(qsAudioOutput, "audio/output");

	SAVELOAD(iJitterBufferSize, "net/jitterbuffer");
	SAVELOAD(iFramesPerPacket, "net/framesperpacket");

	SAVELOAD(qsASIOclass, "asio/class");
	SAVELOAD(qlASIOmic, "asio/mic");
	SAVELOAD(qlASIOspeaker, "asio/speaker");

	SAVELOAD(qsWASAPIInput, "wasapi/input");
	SAVELOAD(qsWASAPIOutput, "wasapi/output");
	SAVELOAD(bWASAPIEcho, "wasapi/echo");

	SAVELOAD(qsALSAInput, "alsa/input");
	SAVELOAD(qsALSAOutput, "alsa/output");

	SAVELOAD(qsPulseAudioInput, "pulseaudio/input");
	SAVELOAD(qsPulseAudioOutput, "pulseaudio/output");
	SAVELOAD(bPulseAudioEcho, "pulseaudio/echo");

	SAVELOAD(qsOSSInput, "oss/input");
	SAVELOAD(qsOSSOutput, "oss/output");

	SAVELOAD(iPortAudioInput, "portaudio/input");
	SAVELOAD(iPortAudioOutput, "portaudio/output");

	SAVELOAD(qbaDXInput, "directsound/input");
	SAVELOAD(qbaDXOutput, "directsound/output");

	SAVELOAD(bPositionalSoundEnable, "positionalsound/enable");
	SAVELOAD(bPositionalSoundSwap, "positionalsound/swap");
	LOADENUM(ePositionalSoundModel, "positionalsound/model");
	SAVELOAD(fPositionalSoundDistance, "positionalsound/distance");
	SAVELOAD(fPositionalSoundPreGain, "positionalsound/pregain");
	SAVELOAD(fPositionalSoundMaxAtt, "positionalsound/maxatt");

	SAVELOAD(bTTS, "tts/enable");
	SAVELOAD(iTTSVolume, "tts/volume");
	SAVELOAD(iTTSThreshold, "tts/threshold");

	SAVELOAD(bOverlayEnable, "overlay/enable");
	LOADENUM(osOverlay, "overlay/show");
	SAVELOAD(bOverlayUserTextures, "overlay/usertextures");
	SAVELOAD(bOverlayAlwaysSelf, "overlay/alwaysself");
	SAVELOAD(fOverlayX, "overlay/x");
	SAVELOAD(fOverlayY, "overlay/y");
	SAVELOAD(bTransmitPosition, "audio/postransmit");
	SAVELOAD(bOverlayTop, "overlay/top");
	SAVELOAD(bOverlayBottom, "overlay/bottom");
	SAVELOAD(bOverlayLeft, "overlay/left");
	SAVELOAD(bOverlayRight, "overlay/right");
	SAVELOAD(qfOverlayFont, "overlay/font");
	SAVELOAD(fOverlayHeight, "overlay/height");
	SAVELOAD(qcOverlayPlayer, "overlay/player");
	SAVELOAD(qcOverlayTalking, "overlay/talking");
	SAVELOAD(qcOverlayChannel, "overlay/channel");
	SAVELOAD(qcOverlayChannelTalking, "overlay/channeltalking");

	// Network settings
	SAVELOAD(bTCPCompat, "net/tcponly");
	SAVELOAD(bReconnect, "net/reconnect");
	LOADENUM(ptProxyType, "net/proxytype");
	SAVELOAD(qsProxyHost, "net/proxyhost");
	SAVELOAD(usProxyPort, "net/proxyport");
	SAVELOAD(qsProxyUsername, "net/proxyusername");
	SAVELOAD(qsProxyPassword, "net/proxypassword");

	SAVELOAD(bExpert, "ui/expert");
	SAVELOAD(qsLanguage, "ui/language");
	SAVELOAD(qsStyle, "ui/style");
	SAVELOAD(qsSkin, "ui/skin");
	SAVELOAD(bHorizontal, "ui/horizontal");
	SAVELOAD(bExpandAll, "ui/expandall");
	SAVELOAD(bPlayerTop, "ui/playertop");
	SAVELOAD(bFirstTime, "ui/firsttime");
	SAVELOAD(qbaMainWindowGeometry, "ui/geometry");
	SAVELOAD(qbaMainWindowState, "ui/state");
	SAVELOAD(qbaSplitterState, "ui/splitter");
	SAVELOAD(qbaHeaderState, "ui/header");
	SAVELOAD(qsUsername, "ui/username");
	SAVELOAD(iServerRow, "ui/serverrow");
	SAVELOAD(bUpdateCheck, "ui/updatecheck");

	int nshorts = g.qs->beginReadArray(QLatin1String("shortcuts"));
	for (int i=0;i<nshorts;i++) {
		g.qs->setArrayIndex(i);
		SAVELOAD(qmShortcuts[i], "keys");
	}
	g.qs->endArray();

	g.qs->beginReadArray(QLatin1String("messages"));
	for (QMap<int, quint32>::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmMessages[it.key()], "log");
	}
	g.qs->endArray();
}

#undef SAVELOAD
#define SAVELOAD(var,name) if (var != def.var) g.qs->setValue(QLatin1String(name), var); else g.qs->remove(QLatin1String(name))

void Settings::save() {
	Settings def;

	SAVELOAD(bMute, "audio/mute");
	SAVELOAD(bDeaf, "audio/deaf");
	SAVELOAD(atTransmit, "audio/transmit");
	SAVELOAD(uiDoublePush, "audio/doublepush");
	SAVELOAD(bPushClick, "audio/pushclick");
	SAVELOAD(iQuality, "audio/quality");
	SAVELOAD(iComplexity, "audio/complexity");
	SAVELOAD(iMinLoudness, "audio/loudness");
	SAVELOAD(fVolume, "audio/volume");
	SAVELOAD(vsVAD, "audio/vadsource");
	SAVELOAD(fVADmin, "audio/vadmin");
	SAVELOAD(fVADmax, "audio/vadmax");
	SAVELOAD(iNoiseSuppress, "audio/noisesupress");
	SAVELOAD(iVoiceHold, "audio/voicehold");
	SAVELOAD(iOutputDelay, "audio/outputdelay");
	SAVELOAD(a3dModel, "audio/3dmode");
	SAVELOAD(fDXMinDistance, "audio/mindistance");
	SAVELOAD(fDXMaxDistance, "audio/maxdistance");
	SAVELOAD(fDXRollOff, "audio/rolloff");
	SAVELOAD(qsAudioInput, "audio/input");
	SAVELOAD(qsAudioOutput, "audio/output");

	SAVELOAD(iJitterBufferSize, "net/jitterbuffer");
	SAVELOAD(iFramesPerPacket, "net/framesperpacket");

	SAVELOAD(qsASIOclass, "asio/class");
	SAVELOAD(qlASIOmic, "asio/mic");
	SAVELOAD(qlASIOspeaker, "asio/speaker");

	SAVELOAD(qsWASAPIInput, "wasapi/input");
	SAVELOAD(qsWASAPIOutput, "wasapi/output");
	SAVELOAD(bWASAPIEcho, "wasapi/echo");

	SAVELOAD(qsALSAInput, "alsa/input");
	SAVELOAD(qsALSAOutput, "alsa/output");

	SAVELOAD(qsPulseAudioInput, "pulseaudio/input");
	SAVELOAD(qsPulseAudioOutput, "pulseaudio/output");
	SAVELOAD(bPulseAudioEcho, "pulseaudio/echo");

	SAVELOAD(qsOSSInput, "oss/input");
	SAVELOAD(qsOSSOutput, "oss/output");

	SAVELOAD(iPortAudioInput, "portaudio/input");
	SAVELOAD(iPortAudioOutput, "portaudio/output");

	SAVELOAD(qbaDXInput, "directsound/input");
	SAVELOAD(qbaDXOutput, "directsound/output");

	SAVELOAD(bPositionalSoundEnable, "positionalsound/enable");
	SAVELOAD(bPositionalSoundSwap, "positionalsound/swap");
	SAVELOAD(ePositionalSoundModel, "positionalsound/model");
	SAVELOAD(fPositionalSoundDistance, "positionalsound/distance");
	SAVELOAD(fPositionalSoundPreGain, "positionalsound/pregain");
	SAVELOAD(fPositionalSoundMaxAtt, "positionalsound/maxatt");

	SAVELOAD(bTTS, "tts/enable");
	SAVELOAD(iTTSVolume, "tts/volume");
	SAVELOAD(iTTSThreshold, "tts/threshold");

	SAVELOAD(bOverlayEnable, "overlay/enable");
	SAVELOAD(osOverlay, "overlay/show");
	SAVELOAD(bOverlayUserTextures, "overlay/usertextures");
	SAVELOAD(bOverlayAlwaysSelf, "overlay/alwaysself");
	SAVELOAD(fOverlayX, "overlay/x");
	SAVELOAD(fOverlayY, "overlay/y");
	SAVELOAD(bTransmitPosition, "audio/postransmit");
	SAVELOAD(bOverlayTop, "overlay/top");
	SAVELOAD(bOverlayBottom, "overlay/bottom");
	SAVELOAD(bOverlayLeft, "overlay/left");
	SAVELOAD(bOverlayRight, "overlay/right");
	SAVELOAD(qfOverlayFont, "overlay/font");
	SAVELOAD(fOverlayHeight, "overlay/height");
	SAVELOAD(qcOverlayPlayer, "overlay/player");
	SAVELOAD(qcOverlayTalking, "overlay/talking");
	SAVELOAD(qcOverlayChannel, "overlay/channel");
	SAVELOAD(qcOverlayChannelTalking, "overlay/channeltalking");

	// Network settings
	SAVELOAD(bTCPCompat, "net/tcponly");
	SAVELOAD(bReconnect, "net/reconnect");
	SAVELOAD(ptProxyType, "net/proxytype");
	SAVELOAD(qsProxyHost, "net/proxyhost");
	SAVELOAD(usProxyPort, "net/proxyport");
	SAVELOAD(qsProxyUsername, "net/proxyusername");
	SAVELOAD(qsProxyPassword, "net/proxypassword");

	SAVELOAD(bExpert, "ui/expert");
	SAVELOAD(qsLanguage, "ui/language");
	SAVELOAD(qsStyle, "ui/style");
	SAVELOAD(qsSkin, "ui/skin");
	SAVELOAD(bHorizontal, "ui/horizontal");
	SAVELOAD(bExpandAll, "ui/expandall");
	SAVELOAD(bPlayerTop, "ui/playertop");
	SAVELOAD(bFirstTime, "ui/firsttime");
	SAVELOAD(qbaMainWindowGeometry, "ui/geometry");
	SAVELOAD(qbaMainWindowState, "ui/state");
	SAVELOAD(qbaSplitterState, "ui/splitter");
	SAVELOAD(qbaHeaderState, "ui/header");
	SAVELOAD(qsUsername, "ui/username");
	SAVELOAD(iServerRow, "ui/serverrow");
	SAVELOAD(bUpdateCheck, "ui/updatecheck");

	g.qs->beginWriteArray(QLatin1String("shortcuts"));
	int idx = 0;
	for (ShortcutMap::const_iterator it = qmShortcuts.constBegin(); it != qmShortcuts.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmShortcuts[it.key()], "keys");
	}
	g.qs->endArray();

	g.qs->beginWriteArray(QLatin1String("messages"));
	idx = 0;
	for (QMap<int, quint32>::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmMessages[it.key()], "log");
	}
	g.qs->endArray();
}
