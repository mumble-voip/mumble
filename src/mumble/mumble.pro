include(../mumble.pri)

TEMPLATE	= app
QT		+= network sql opengl
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		+=
SOURCES		+=
HEADERS		= BanEditor.h ACLEditor.h Log.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h MainWindow.h ServerHandler.h About.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h PlayerModel.h Audio.h ConfigDialog.h Plugins.h LookConfig.h
SOURCES		= BanEditor.cpp ACLEditor.cpp Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp ServerHandler.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp PlayerModel.cpp Audio.cpp ConfigDialog.cpp Plugins.cpp LookConfig.cpp
HEADERS	+= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../Player.h
SOURCES += ../ACL.cpp ../Group.cpp ../Channel.cpp ../Message.cpp ../Connection.cpp ../Player.cpp
DIST		+= licenses.h mumble.ico firmumble.inc plugins/mumble_plugin.h overlay/overlay.h
INCLUDEPATH	+= ../../speex/include
LIBS 		+= -Llib -L.
RESOURCES	+= mumble.qrc

TRANSLATIONS	= mumble_en.ts mumble_es.ts mumble_de.ts mumble_tr.ts mumble_id.ts mumble_fr.ts mumble_ru.ts mumble_it.ts mumble_pt.ts

PRECOMPILED_HEADER = mumble_pch.h

LIBS	+= -lspeex

SUBDIRS	= speex

win32 {
  HEADERS	+= DXAudioInput.h DXAudioOutput.h DXConfigDialog.h GlobalShortcut_win.h Overlay.h
  SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp DXConfigDialog.cpp GlobalShortcut_win.cpp TextToSpeech_win.cpp Overlay_win.cpp os_win.cpp
  INCLUDEPATH	+= /dev/dxsdk/Include /dev/SpeechSDK/include /dev/Boost/include/boost-1_33_1
  LIBS	+= -L/dev/SpeechSDK/Lib/i386 -ldsound -ldxguid -ldinput8 -lsapi -lole32 -lws2_32
  CONFIG	+= asio
  DEFINES += WIN32
}

unix {
  HEADERS	+= ALSAAudio.h GlobalShortcut_unix.h
  SOURCES	+= ALSAAudio.cpp GlobalShortcut_unix.cpp TextToSpeech_unix.cpp
  LIBS		+= -lasound -lXevie 
  QMAKE_CFLAGS += -I../../speex/include -I../../speexbuild
  QMAKE_CXXFLAGS += -I../../speex/include -I../../speexbuild
  QMAKE_CXXFLAGS_RELEASE += -I../../speex/include -I../../speexbuild
  QMAKE_CXXFLAGS_DEBUG += -I../../speex/include -I../../speexbuild
}

CONFIG(asio) {
	INCLUDEPATH += ../../asio/common ../../asio/host ../../asio/host/pc
	HEADERS += ASIOInput.h
	SOURCES	+= ASIOInput.cpp
}

lrel.target = mumble_en.qm
lrel.commands = lrelease mumble.pro
lrel.depends = mumble_en.ts

lupd.target = mumble_en.ts
lupd.commands = lupdate mumble.pro

QMAKE_EXTRA_TARGETS += lrel lupd

PRE_TARGETDEPS = mumble_en.qm

