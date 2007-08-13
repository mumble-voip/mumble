include(../mumble.pri)

TEMPLATE	= app
QT		+= network sql opengl xml qdbus
CONFIG		+= qdbus
TARGET		= mumble
RC_FILE		= mumble.rc
HEADERS		+=
SOURCES		+=
HEADERS		= BanEditor.h ACLEditor.h Log.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h MainWindow.h ServerHandler.h About.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h PlayerModel.h Audio.h ConfigDialog.h Plugins.h LookConfig.h Overlay.h  AudioWizard.h ViewCert.h DBus.h
SOURCES		= BanEditor.cpp ACLEditor.cpp Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp ServerHandler.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp PlayerModel.cpp Audio.cpp ConfigDialog.cpp Plugins.cpp LookConfig.cpp Overlay.cpp AudioWizard.cpp ViewCert.cpp Messages.cpp DBus.cpp
HEADERS	+= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../Player.h
SOURCES += ../ACL.cpp ../Group.cpp ../Channel.cpp ../Message.cpp ../Connection.cpp ../Player.cpp ../Timer.cpp ../CryptState.cpp
DIST		+= licenses.h mumble.ico firmumble.inc plugins/mumble_plugin.h mumble-overlay mumble.desktop
INCLUDEPATH	+= ../../speex/include
LIBS 		+= -Llib -L.
RESOURCES	+= mumble.qrc
FORMS	+= ConfigDialog.ui MainWindow.ui

TRANSLATIONS	= mumble_en.ts mumble_es.ts mumble_de.ts mumble_tr.ts mumble_id.ts mumble_fr.ts mumble_ru.ts mumble_it.ts mumble_pt.ts mumble_nb.ts mumble_nl.ts mumble_cs.ts

PRECOMPILED_HEADER = mumble_pch.h

LIBS	+= -lspeex

SUBDIRS	= speex

QMAKE_CXXFLAGS	+= -Wall -Wextra

win32 {
  HEADERS	+= DXAudioInput.h DXAudioOutput.h DXConfigDialog.h GlobalShortcut_win.h
  SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp DXConfigDialog.cpp GlobalShortcut_win.cpp TextToSpeech_win.cpp Overlay_win.cpp os_win.cpp
  INCLUDEPATH	+= /dev/dxsdk/Include /dev/SpeechSDK/include /dev/Boost/include/boost-1_34_1
  LIBS	+= -L/dev/SpeechSDK/Lib/i386 -ldsound -ldxguid -ldinput8 -lsapi -lole32 -lws2_32
  CONFIG	+= asio
  DEFINES += WIN32
  INCLUDEPATH	+= /dev/openssl/outinc
  LIBS += -L/dev/openssl/out -leay32
  QMAKE_CXXFLAGS += -msse -mmmx
}

unix {
  UNAME=$$system(uname -s)
  ARCH=$$system(uname -m)
  X86ARCH=$$find(ARCH, i[3456]86) $$find(ARCH, x86_64)


  QMAKE_CFLAGS += -I../../speex/include -I../../speexbuild
  QMAKE_CXXFLAGS += -I../../speex/include -I../../speexbuild
  QMAKE_CXXFLAGS_RELEASE += -I../../speex/include -I../../speexbuild
  QMAKE_CXXFLAGS_DEBUG += -I../../speex/include -I../../speexbuild
  CONFIG += qdbus link_pkgconfig
  PKGCONFIG += openssl

  contains(UNAME, Linux) {
    HEADERS += ALSAAudio.h GlobalShortcut_unix.h
    SOURCES += ALSAAudio.cpp GlobalShortcut_unix.cpp TextToSpeech_unix.cpp Overlay_unix.cpp
    PKGCONFIG += xevie alsa
  }

  macx {
    CONFIG += x86 ppc
    LIBS += -framework ApplicationServices
    QMAKE_LFLAGS += -L../../speexbuild -L/System/Library/Frameworks
    INCLUDEPATH += /usr/local/include/boost-1_34
    HEADERS += GlobalShortcut_macx.h
    SOURCES += TextToSpeech_macx.cpp GlobalShortcut_macx.cpp Overlay_macx.cpp
  }

  !macx {
    !isEmpty(X86ARCH) {
      QMAKE_CXXFLAGS += -mmmx
    }
  }
}

CONFIG(asio) {
	DEFINES	+= USE_ASIO
	INCLUDEPATH += ../../asio/common ../../asio/host ../../asio/host/pc
	HEADERS += ASIOInput.h
	SOURCES	+= ASIOInput.cpp
}

QT_TRANSDIR = $$[QT_INSTALL_TRANSLATIONS]/
QT_TRANSDIR = $$replace(QT_TRANSDIR,/,$${DIR_SEPARATOR})

copytrans.target = qt_de.qm
copytrans.commands  = $$QMAKE_COPY $${QT_TRANSDIR}qt_de.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_fr.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_es.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_ru.qm . 

lrel.target = mumble_en.qm
lrel.commands = lrelease mumble.pro
lrel.depends = mumble_en.ts

lupd.target = mumble_en.ts
lupd.commands = lupdate mumble.pro

QMAKE_EXTRA_TARGETS += lrel lupd copytrans

PRE_TARGETDEPS = mumble_en.qm qt_de.qm
