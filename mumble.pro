include(mumble.pri)

TEMPLATE	= app
QT		+= network
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		= AudioInput.h AudioOutput.h MainWindow.h Connection.h ServerHandler.h Player.h About.h Message.h ConnectDialog.h GlobalShortcut.h
SOURCES		= AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp Message.cpp Connection.cpp ServerHandler.cpp Player.cpp About.cpp ConnectDialog.cpp
DIST		+= licenses.h mumble.ico
INCLUDEPATH	+= include /dx90sdk/include
LIBS 		+= -Llib -L.

LIBS	+= -llibspeex

win32 {
  HEADERS	+= DXAudioInput.h DXAudioOutput.h GlobalShortcut_win.h
  SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp GlobalShortcut_win.cpp
  LIBS	+= -L\\dx90sdk\\lib -ldsound -ldxguid -ldinput8
}

