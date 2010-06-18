/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include "AudioInput.h"

bool Shortcut::operator <(const Shortcut &other) const {
	return (iIndex < other.iIndex);
}

Settings::Settings() {
	bHighContrast = false;
	atTransmit = VAD;
	bTransmitPosition = false;
	bMute = bDeaf = false;
	bTTS = true;
	iTTSVolume = 75;
	iTTSThreshold = 250;
	iQuality = 6;
	fVolume = 1.0f;
	fOtherVolume = 0.5f;
	bAttenuateOthersOnTalk = false;
	bAttenuateOthers = false;
	iMinLoudness = 1000;
	iVoiceHold = 50;
	iJitterBufferSize = 1;
	iFramesPerPacket = 2;
	iNoiseSuppress = -30;
	iIdleTime = 0;
	vsVAD = SignalToNoise;
	fVADmin = 0.80f;
	fVADmax = 0.98f;
	bPushClick = false;

	bExclusiveInput = bExclusiveOutput = false;

	bPlayerTop = false;

	uiDoublePush = 0;
	bExpert = false;

	// Update checks will always fail.
	bUpdateCheck = false;

	bFirstTime = true;
	ceExpand = ChannelsWithPlayers;
	ceChannelDrag = Ask;
	bMinimalView = false;
	bHideFrame = false;
	bAlwaysOnTop = false;
	bAskOnQuit = true;
	bHideTray = true;
	bUsage = true;

	iOutputDelay = 5;

	qsALSAInput=QLatin1String("default");
	qsALSAOutput=QLatin1String("default");

	bEcho = false;

	iPortAudioInput = -1; // default device
	iPortAudioOutput = -1; // default device

	bPositionalAudio = false;
	bPositionalHeadphone = false;
	fAudioMinDistance = 1.0f;
	fAudioMaxDistance = 15.0f;
	fAudioMaxDistVolume = 0.15f;
	fAudioBloom = 0.5f;

	bOverlayEnable = true;
	bOverlayUserTextures=true;
	osOverlay = All;
	bOverlayAlwaysSelf = true;
	fOverlayX = 0.8f;
	fOverlayY = 0.0f;
	bOverlayTop = false;
	bOverlayBottom = true;
	bOverlayLeft = true;
	bOverlayRight = false;
#ifdef Q_OS_WIN
	qfOverlayFont = QFont(QLatin1String("Comic Sans MS"), 20);
#else
	qfOverlayFont = QFont(QLatin1String("Arial"), 20);
#endif
	fOverlayHeight = .03f;
	qcOverlayPlayer = QColor(255,255,255,128);
	qcOverlayTalking = QColor(255,255,196,255);
	qcOverlayAltTalking = QColor(255,128,128,255);
	qcOverlayChannel = QColor(192,192,255,192);
	qcOverlayChannelTalking = QColor(224,224,255,255);

	iLCDPlayerViewMinColWidth = 50;
	iLCDPlayerViewSplitterWidth = 2;

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
		qmMessages.insert(i, Settings::LogConsole | Settings::LogBalloon | Settings::LogTTS);

	for (int i=Log::firstMsgType;i<=Log::lastMsgType;++i)
		qmMessageSounds.insert(i, QString());

	qmMessages[Log::DebugInfo] = Settings::LogConsole;
	qmMessages[Log::Information] = Settings::LogConsole;

	iServerRow = -1;
}

bool Settings::doEcho() const {
	if (! bEcho)
		return false;

	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qsAudioInput);
		if (air) {
			if (air->canEcho(qsAudioOutput))
				return true;
		}
	}
	return false;
}

bool Settings::doPositionalAudio() const {
	return bPositionalAudio;
}

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()


BOOST_TYPEOF_REGISTER_TYPE(Settings::AudioTransmit);
BOOST_TYPEOF_REGISTER_TYPE(Settings::VADSource);
BOOST_TYPEOF_REGISTER_TYPE(Settings::LoopMode)
BOOST_TYPEOF_REGISTER_TYPE(Settings::OverlayShow)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ProxyType)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ChannelExpand)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ChannelDrag)
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
	SAVELOAD(iMinLoudness, "audio/loudness");
	SAVELOAD(fVolume, "audio/volume");
	SAVELOAD(fOtherVolume, "audio/othervolume");
	LOADENUM(vsVAD, "audio/vadsource");
	SAVELOAD(fVADmin, "audio/vadmin");
	SAVELOAD(fVADmax, "audio/vadmax");
	SAVELOAD(iNoiseSuppress, "audio/noisesupress");
	SAVELOAD(iVoiceHold, "audio/voicehold");
	SAVELOAD(iOutputDelay, "audio/outputdelay");
	SAVELOAD(iIdleTime, "audio/idletime");
	SAVELOAD(fAudioMinDistance, "audio/mindistance");
	SAVELOAD(fAudioMaxDistance, "audio/maxdistance");
	SAVELOAD(fAudioMaxDistVolume, "audio/maxdistancevolume");
	SAVELOAD(fAudioBloom, "audio/bloom");
	SAVELOAD(bEcho, "audio/echo");
	SAVELOAD(bPositionalAudio, "audio/positional");
	SAVELOAD(bPositionalHeadphone, "audio/headphone");
	SAVELOAD(qsAudioInput, "audio/input");
	SAVELOAD(qsAudioOutput, "audio/output");

	SAVELOAD(iJitterBufferSize, "net/jitterbuffer");
	SAVELOAD(iFramesPerPacket, "net/framesperpacket");

	SAVELOAD(qsASIOclass, "asio/class");
	SAVELOAD(qlASIOmic, "asio/mic");
	SAVELOAD(qlASIOspeaker, "asio/speaker");

	SAVELOAD(qsWASAPIInput, "wasapi/input");
	SAVELOAD(qsWASAPIOutput, "wasapi/output");

	SAVELOAD(qsCoreAudioInput, "coreaudio/input");
	SAVELOAD(qsCoreAudioOutput, "coreaudio/output");

	SAVELOAD(qsALSAInput, "alsa/input");
	SAVELOAD(qsALSAOutput, "alsa/output");

	SAVELOAD(qsPulseAudioInput, "pulseaudio/input");
	SAVELOAD(qsPulseAudioOutput, "pulseaudio/output");

	SAVELOAD(qsOSSInput, "oss/input");
	SAVELOAD(qsOSSOutput, "oss/output");

	SAVELOAD(iPortAudioInput, "portaudio/input");
	SAVELOAD(iPortAudioOutput, "portaudio/output");

	SAVELOAD(qbaDXInput, "directsound/input");
	SAVELOAD(qbaDXOutput, "directsound/output");

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
	LOADENUM(ceExpand, "ui/expand");
	LOADENUM(ceChannelDrag, "ui/drag");
	SAVELOAD(bAlwaysOnTop, "ui/alwaysontop");
	SAVELOAD(bAskOnQuit, "ui/askonquit");
	SAVELOAD(bMinimalView, "ui/minimalview");
	SAVELOAD(bHideFrame, "ui/hideframe");
	SAVELOAD(bPlayerTop, "ui/playertop");
	SAVELOAD(bFirstTime, "ui/firsttime");
	SAVELOAD(qbaMainWindowGeometry, "ui/geometry");
	SAVELOAD(qbaMainWindowState, "ui/state");
	SAVELOAD(qbaMinimalViewGeometry, "ui/minimalviewgeometry");
	SAVELOAD(qbaConfigGeometry, "ui/ConfigGeometry");
	SAVELOAD(qbaSplitterState, "ui/splitter");
	SAVELOAD(qbaHeaderState, "ui/header");
	SAVELOAD(qsUsername, "ui/username");
	SAVELOAD(iServerRow, "ui/serverrow");
	SAVELOAD(bUpdateCheck, "ui/updatecheck");
	SAVELOAD(bHideTray, "ui/hidetray");
	SAVELOAD(bUsage, "ui/usage");

	SAVELOAD(iLCDPlayerViewMinColWidth, "lcd/playerview/mincolwidth");
	SAVELOAD(iLCDPlayerViewSplitterWidth, "lcd/playerview/splitterwidth");

	int nshorts = g.qs->beginReadArray(QLatin1String("shortcuts"));
	for (int i=0;i<nshorts;i++) {
		g.qs->setArrayIndex(i);
		Shortcut s;

		s.iIndex = -2;

		SAVELOAD(s.iIndex, "index");
		SAVELOAD(s.qlButtons, "keys");
		SAVELOAD(s.bSuppress, "suppress");
		if (s.iIndex >= -1)
			qlShortcuts << s;
	}
	g.qs->endArray();

	g.qs->beginReadArray(QLatin1String("messages"));
	for (QMap<int, quint32>::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmMessages[it.key()], "log");
	}
	g.qs->endArray();

	g.qs->beginReadArray(QLatin1String("messagesounds"));
	for (QMap<int, QString>::const_iterator it = qmMessageSounds.constBegin(); it != qmMessageSounds.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmMessageSounds[it.key()], "logsound");
	}
	g.qs->endArray();

	g.qs->beginGroup(QLatin1String("lcd/devices"));
	foreach(const QString &d, g.qs->childKeys()) {
		qmLCDDevices.insert(d, g.qs->value(d, true).toBool());
	}
	g.qs->endGroup();
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
	SAVELOAD(iMinLoudness, "audio/loudness");
	SAVELOAD(fVolume, "audio/volume");
	SAVELOAD(fOtherVolume, "audio/othervolume");
	SAVELOAD(vsVAD, "audio/vadsource");
	SAVELOAD(fVADmin, "audio/vadmin");
	SAVELOAD(fVADmax, "audio/vadmax");
	SAVELOAD(iNoiseSuppress, "audio/noisesupress");
	SAVELOAD(iVoiceHold, "audio/voicehold");
	SAVELOAD(iOutputDelay, "audio/outputdelay");
	SAVELOAD(iIdleTime, "audio/idletime");
	SAVELOAD(fAudioMinDistance, "audio/mindistance");
	SAVELOAD(fAudioMaxDistance, "audio/maxdistance");
	SAVELOAD(fAudioMaxDistVolume, "audio/maxdistancevolume");
	SAVELOAD(fAudioBloom, "audio/bloom");
	SAVELOAD(bEcho, "audio/echo");
	SAVELOAD(bPositionalAudio, "audio/positional");
	SAVELOAD(bPositionalHeadphone, "audio/headphone");
	SAVELOAD(qsAudioInput, "audio/input");
	SAVELOAD(qsAudioOutput, "audio/output");

	SAVELOAD(iJitterBufferSize, "net/jitterbuffer");
	SAVELOAD(iFramesPerPacket, "net/framesperpacket");

	SAVELOAD(qsASIOclass, "asio/class");
	SAVELOAD(qlASIOmic, "asio/mic");
	SAVELOAD(qlASIOspeaker, "asio/speaker");

	SAVELOAD(qsWASAPIInput, "wasapi/input");
	SAVELOAD(qsWASAPIOutput, "wasapi/output");

	SAVELOAD(qsCoreAudioInput, "coreaudio/input");
	SAVELOAD(qsCoreAudioOutput, "coreaudio/output");

	SAVELOAD(qsALSAInput, "alsa/input");
	SAVELOAD(qsALSAOutput, "alsa/output");

	SAVELOAD(qsPulseAudioInput, "pulseaudio/input");
	SAVELOAD(qsPulseAudioOutput, "pulseaudio/output");

	SAVELOAD(qsOSSInput, "oss/input");
	SAVELOAD(qsOSSOutput, "oss/output");

	SAVELOAD(iPortAudioInput, "portaudio/input");
	SAVELOAD(iPortAudioOutput, "portaudio/output");

	SAVELOAD(qbaDXInput, "directsound/input");
	SAVELOAD(qbaDXOutput, "directsound/output");

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
	SAVELOAD(ceExpand, "ui/expand");
	SAVELOAD(ceChannelDrag, "ui/drag");
	SAVELOAD(bAlwaysOnTop, "ui/alwaysontop");
	SAVELOAD(bAskOnQuit, "ui/askonquit");
	SAVELOAD(bMinimalView, "ui/minimalview");
	SAVELOAD(bHideFrame, "ui/hideframe");
	SAVELOAD(bPlayerTop, "ui/playertop");
	SAVELOAD(bFirstTime, "ui/firsttime");
	SAVELOAD(qbaMainWindowGeometry, "ui/geometry");
	SAVELOAD(qbaMainWindowState, "ui/state");
	SAVELOAD(qbaMinimalViewGeometry, "ui/minimalviewgeometry");
	SAVELOAD(qbaConfigGeometry, "ui/ConfigGeometry");
	SAVELOAD(qbaSplitterState, "ui/splitter");
	SAVELOAD(qbaHeaderState, "ui/header");
	SAVELOAD(qsUsername, "ui/username");
	SAVELOAD(iServerRow, "ui/serverrow");
#ifndef NO_UPDATE_CHECK
	SAVELOAD(bUpdateCheck, "ui/updatecheck");
#endif
	SAVELOAD(bHideTray, "ui/hidetray");
	SAVELOAD(bUsage, "ui/usage");

	SAVELOAD(iLCDPlayerViewMinColWidth, "lcd/playerview/mincolwidth");
	SAVELOAD(iLCDPlayerViewSplitterWidth, "lcd/playerview/splitterwidth");

	g.qs->beginWriteArray(QLatin1String("shortcuts"));
	int idx = 0;
	foreach(const Shortcut &s, qlShortcuts) {
		g.qs->setArrayIndex(idx++);
		g.qs->setValue(QLatin1String("index"), s.iIndex);
		g.qs->setValue(QLatin1String("keys"), s.qlButtons);
		g.qs->setValue(QLatin1String("suppress"), s.bSuppress);
	}
	g.qs->endArray();

	g.qs->beginWriteArray(QLatin1String("messages"));
	for (QMap<int, quint32>::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmMessages[it.key()], "log");
	}
	g.qs->endArray();

	g.qs->beginWriteArray(QLatin1String("messagesounds"));
	for (QMap<int, QString>::const_iterator it = qmMessageSounds.constBegin(); it != qmMessageSounds.constEnd(); ++it) {
		g.qs->setArrayIndex(it.key());
		SAVELOAD(qmMessageSounds[it.key()], "logsound");
	}
	g.qs->endArray();

	g.qs->beginGroup(QLatin1String("lcd/devices"));
	foreach(const QString &d, qmLCDDevices.keys()) {
		bool v = qmLCDDevices.value(d);
		if (!v)
			g.qs->setValue(d, v);
		else
			g.qs->remove(d);
	}
	g.qs->endGroup();
}
