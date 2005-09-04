TEMPLATE	= app
QT		+= network
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		= AudioInput.h AudioOutput.h MainWindow.h Connection.h ServerHandler.h DXAudioInput.h DXAudioOutput.h Player.h About.h
SOURCES		= AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp Message.cpp Connection.cpp ServerHandler.cpp DXAudioInput.cpp DXAudioOutput.cpp Player.cpp About.cpp
INCLUDEPATH	+= include /dx90sdk/include
LIBS 		+= -Llib -L.

debug {
  CONFIG += console
  LIBS	+= -Ldebug
}
release {
  LIBS	+= -Lrelease
}

LIBS	+= -llibspeex

win32 {
  LIBS	+= -L\\dx90sdk\\lib -ldsound -ldxguid -ldinput8
}

