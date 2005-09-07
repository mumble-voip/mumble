include(mumble.pri)

TEMPLATE	= app
QT		+= network sql
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		= AudioInput.h AudioOutput.h MainWindow.h Connection.h ServerHandler.h Player.h About.h Message.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h
SOURCES		= AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp Message.cpp Connection.cpp ServerHandler.cpp Player.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp
DIST		+= licenses.h mumble.ico mumble.png.0 mumble.png.1 mumble.png.2
INCLUDEPATH	+= include
LIBS 		+= -Llib -L.
RESOURCES	+= mumble.qrc

LIBS	+= -llibspeex

win32 {
  HEADERS	+= DXAudioInput.h DXAudioOutput.h GlobalShortcut_win.h
  SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp GlobalShortcut_win.cpp TextToSpeech_win.cpp
  INCLUDEPATH	+= /dx90sdk/include "/Program Files/Microsoft Speech SDK 5.1/Include"
  LIBS	+= -L\\dx90sdk\\lib -L"/Program Files/Microsoft Speech SDK 5.1/Lib/i386" -ldsound -ldxguid -ldinput8 -lsapi -lole32
}
