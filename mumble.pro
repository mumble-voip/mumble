include(mumble.pri)

TEMPLATE	= app
QT		+= network sql
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		= Log.h TrayIcon.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h MainWindow.h Connection.h ServerHandler.h Player.h About.h Message.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h PlayerModel.h Audio.h ConfigDialog.h
SOURCES		= Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp Message.cpp Connection.cpp ServerHandler.cpp Player.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp PlayerModel.cpp Audio.cpp ConfigDialog.cpp
DIST		+= licenses.h mumble.ico mumble.png.0 mumble.png.1 mumble.png.2
INCLUDEPATH	+= include
LIBS 		+= -Llib -L.
RESOURCES	+= mumble.qrc

TRANSLATIONS	= mumble_en.ts

LIBS	+= -llibspeex

win32 {
  HEADERS	+= DXAudioInput.h DXAudioOutput.h DXConfigDialog.h GlobalShortcut_win.h Plugins.h
  SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp DXConfigDialog.cpp GlobalShortcut_win.cpp TextToSpeech_win.cpp TrayIcon_win.cpp Plugins.cpp 
  INCLUDEPATH	+= /dx90sdk/include "/Program Files/Microsoft Speech SDK 5.1/Include"
  LIBS	+= -L\\dx90sdk\\lib -L"/Program Files/Microsoft Speech SDK 5.1/Lib/i386" -ldsound -ldxguid -ldinput8 -lsapi -lole32
  INCLUDEPATH	+= /boost/include/boost-1_33
  CONFIG	+= asio
}

unix {
  INCLUDEPATH	+= ..
}


CONFIG(asio) {
	INCLUDEPATH += asio/common asio/host asio/host/pc
	HEADERS += ASIOInput.h
	SOURCES	+= ASIOInput.cpp
	DEFINES += WIN32
}
