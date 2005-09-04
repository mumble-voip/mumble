include(mumble.pri)

TEMPLATE	= app
QT		+= network
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		= AudioInput.h AudioOutput.h MainWindow.h Connection.h ServerHandler.h DXAudioInput.h DXAudioOutput.h Player.h About.h Message.h ConnectDialog.h
SOURCES		= AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp Message.cpp Connection.cpp ServerHandler.cpp DXAudioInput.cpp DXAudioOutput.cpp Player.cpp About.cpp ConnectDialog.cpp
DIST		+= licenses.h mumble.ico
INCLUDEPATH	+= include /dx90sdk/include
LIBS 		+= -Llib -L.

LIBS	+= -llibspeex

win32 {
  LIBS	+= -L\\dx90sdk\\lib -ldsound -ldxguid -ldinput8
}

