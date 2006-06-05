include(mumble.pri)

TEMPLATE	= app
QT		+= network sql opengl
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		= ACL.h BanEditor.h Group.h ACLEditor.h Channel.h Log.h TrayIcon.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h MainWindow.h Connection.h ServerHandler.h Player.h About.h Message.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h PlayerModel.h Audio.h ConfigDialog.h Plugins.h
SOURCES		= ACL.cpp BanEditor.cpp Group.cpp ACLEditor.cpp Channel.cpp Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp Message.cpp Connection.cpp ServerHandler.cpp Player.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp PlayerModel.cpp Audio.cpp ConfigDialog.cpp Plugins.cpp
DIST		+= licenses.h mumble.ico plugins/mumble_plugin.h overlay/overlay.h
INCLUDEPATH	+= include
LIBS 		+= -Llib -L.
RESOURCES	+= mumble.qrc

TRANSLATIONS	= mumble_en.ts

PRECOMPILED_HEADER = mumble_pch.h

LIBS	+= -lspeex

win32 {
  HEADERS	+= DXAudioInput.h DXAudioOutput.h DXConfigDialog.h GlobalShortcut_win.h Overlay.h
  SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp DXConfigDialog.cpp GlobalShortcut_win.cpp TextToSpeech_win.cpp TrayIcon_win.cpp Overlay_win.cpp
  INCLUDEPATH	+= /dx90sdk/include "/Program Files/Microsoft Speech SDK 5.1/Include"
  LIBS	+= -L\\dx90sdk\\lib -L"/Program Files/Microsoft Speech SDK 5.1/Lib/i386" -ldsound -ldxguid -ldinput8 -lsapi -lole32 -lws2_32
  INCLUDEPATH	+= /boost/include/boost-1_33
  CONFIG	+= asio
  DEFINES += WIN32
}

unix {
  HEADERS	+= ALSAAudio.h
  SOURCES	+= ALSAAudio.cpp GlobalShortcut_unix.cpp TextToSpeech_unix.cpp TrayIcon_unix.cpp
  INCLUDEPATH	+= /usr/local/include ../speex/include
  LIBS		+= -L/usr/local/lib -L../speex/lib
}

CONFIG(asio) {
	INCLUDEPATH += asio/common asio/host asio/host/pc
	HEADERS += ASIOInput.h
	SOURCES	+= ASIOInput.cpp
}
