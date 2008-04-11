include(../mumble.pri)

TEMPLATE	= app
QT		+= network sql opengl xml 
TARGET		= mumble
HEADERS		= BanEditor.h ACLEditor.h Log.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h MainWindow.h ServerHandler.h About.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h PlayerModel.h Audio.h ConfigDialog.h Plugins.h LookConfig.h Overlay.h  AudioWizard.h ViewCert.h TextMessage.h NetworkConfig.h 
SOURCES		= BanEditor.cpp ACLEditor.cpp Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp ServerHandler.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp PlayerModel.cpp Audio.cpp ConfigDialog.cpp Plugins.cpp LookConfig.cpp Overlay.cpp AudioWizard.cpp ViewCert.cpp Messages.cpp TextMessage.cpp GlobalShortcut.cpp NetworkConfig.cpp
HEADERS	+= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../Player.h
SOURCES += ../ACL.cpp ../Group.cpp ../Channel.cpp ../Message.cpp ../Connection.cpp ../Player.cpp ../Timer.cpp ../CryptState.cpp
SOURCES += smallft.cpp
DIST		+= licenses.h smallft.h mumble.ico mumble.xpm plugins/mumble_plugin.h mumble-overlay mumble.desktop mumble.protocol murmur_pch.h
RESOURCES	+= mumble.qrc
FORMS	+= ConfigDialog.ui MainWindow.ui ConnectDialog.ui BanEditor.ui ACLEditor.ui Plugins.ui Overlay.ui LookConfig.ui AudioInput.ui AudioOutput.ui Log.ui TextMessage.ui AudioStats.ui NetworkConfig.ui
TRANSLATIONS	= mumble_en.ts mumble_es.ts mumble_de.ts mumble_tr.ts mumble_id.ts mumble_fr.ts mumble_ru.ts mumble_it.ts mumble_pt.ts mumble_nb.ts mumble_nl.ts mumble_cs.ts mumble_ja.ts
PRECOMPILED_HEADER = mumble_pch.hpp

CONFIG(no-bundled-speex) {
  PKGCONFIG	+= speex speexdsp
}
  
!CONFIG(no-bundled-speex) {
  INCLUDEPATH	+= ../../speex/include ../../speex/libspeex ../../speexbuild
  LIBS 		+= -lspeex
}

!win32 {
  QMAKE_CXXFLAGS	+= -Wall -Wextra
}

!macx:!CONFIG(no-dbus) {
  CONFIG		+= dbus
}

win32 {
  RC_FILE	= mumble.rc
  HEADERS	+= GlobalShortcut_win.h
  SOURCES	+= GlobalShortcut_win.cpp TextToSpeech_win.cpp Overlay_win.cpp os_win.cpp 
  INCLUDEPATH	+= /dev/WinSDK/include /dev/dxsdk/Include /dev/Boost/include/boost-1_34_1
  LIBS		+= -ldxguid -ldinput8 -lsapi -lole32 -lws2_32 -llibeay32 -ladvapi32
  LIBPATH	+= /dev/WinSDK/Lib/i386 /dev/dxsdk/Lib/x86 /dev/OpenSSL/lib
  DEFINES	+= WIN32
  INCLUDEPATH	+= /dev/OpenSSL/include
  !CONFIG(no-asio) {
    CONFIG	+= asio
  }
  !CONFIG(no-directsound) {
    CONFIG	+= directsound
  }
  !CONFIG(no-wasapi) {
    CONFIG	+= wasapi
  }
}

unix {
  UNAME=$$system(uname -s)

  HAVE_PULSEAUDIO=$$system(pkg-config --modversion --silence-errors libpulse)
  HAVE_PORTAUDIO=$$system(pkg-config --modversion --silence-errors portaudio-2.0)

  !isEmpty(HAVE_PORTAUDIO):!CONFIG(no-portaudio) {
    CONFIG += portaudio
  }

  !isEmpty(HAVE_PULSEAUDIO):!CONFIG(no-pulseaudio) {
    CONFIG -= portaudio
    CONFIG += pulseaudio
  }

  !CONFIG(no-bundled-speex) {
    QMAKE_CFLAGS += -I../../speex/include -I../../speexbuild
    QMAKE_CXXFLAGS += -I../../speex/include -I../../speexbuild
    QMAKE_CXXFLAGS_RELEASE += -I../../speex/include -I../../speexbuild
    QMAKE_CXXFLAGS_DEBUG += -I../../speex/include -I../../speexbuild
  }

  CONFIG += link_pkgconfig

  PKGCONFIG += openssl

  contains(UNAME, Linux) {
    !CONFIG(no-oss) {
      CONFIG  += oss
    }

    !CONFIG(no-alsa) {
      CONFIG += alsa
    }

    !CONFIG(no-speechd) {
      CONFIG += speechd
    }

    HEADERS += GlobalShortcut_unix.h
    SOURCES += GlobalShortcut_unix.cpp TextToSpeech_unix.cpp Overlay_unix.cpp
    PKGCONFIG += xevie 
  }

  macx {
    TARGET = Mumble

    # Common include paths for boost 1.34
    INCLUDEPATH += /usr/local/include/boost-1_34_1/
    INCLUDEPATH += /opt/local/include/boost-1_34_1/
    HEADERS += GlobalShortcut_macx.h
    SOURCES += TextToSpeech_macx.cpp 
    SOURCES += Overlay_macx.cpp
    SOURCES += GlobalShortcut_macx.cpp

    ICON = ../../icons/mumble.icns
  }
}

alsa {
	PKGCONFIG += alsa
	HEADERS += ALSAAudio.h
	SOURCES += ALSAAudio.cpp
}

oss {
	HEADERS += OSS.h
	SOURCES += OSS.cpp
	INCLUDEPATH += /usr/lib/oss/include
}

pulseaudio {
	PKGCONFIG += libpulse
	HEADERS += PulseAudio.h
	SOURCES += PulseAudio.cpp
}

portaudio {
	PKGCONFIG += portaudio-2.0
	HEADERS += PAAudio.h
	SOURCES += PAAudio.cpp
}

asio {
	HEADERS += ASIOInput.h
	SOURCES	+= ASIOInput.cpp
	FORMS += ASIOInput.ui
	INCLUDEPATH += ../../asio/common ../../asio/host ../../asio/host/pc
}

dbus {
	CONFIG += qdbus
	HEADERS += DBus.h
	SOURCES += DBus.cpp
	DEFINES += USE_DBUS
}

speechd {
	DEFINES += USE_SPEECHD
	LIBS += -lspeechd
}

directsound {
	HEADERS	+= DXAudioInput.h DXAudioOutput.h
	SOURCES	+= DXAudioInput.cpp DXAudioOutput.cpp
	LIBS	+= -ldsound
}

wasapi {
	HEADERS	+= WASAPI.h
	SOURCES	+= WASAPI.cpp
	LIBS	+= -ldelayimp -lAVRT -delayload:AVRT.DLL
}

CONFIG(no-update) {
	DEFINES += NO_UPDATE_CHECK
}

QT_TRANSDIR = $$[QT_INSTALL_TRANSLATIONS]/
QT_TRANSDIR = $$replace(QT_TRANSDIR,/,$${DIR_SEPARATOR})

copytrans.target = qt_de.qm
copytrans.commands  = $$QMAKE_COPY $${QT_TRANSDIR}qt_de.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_fr.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_es.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_ru.qm . &&
copytrans.commands += $$QMAKE_COPY $${QT_TRANSDIR}qt_ja_jp.qm .

lrel.target = mumble_en.qm
lrel.commands = lrelease mumble.pro
lrel.depends = mumble_en.ts

lupd.target = mumble_en.ts
lupd.commands = lupdate mumble.pro

QMAKE_EXTRA_TARGETS += lrel lupd copytrans

PRE_TARGETDEPS = mumble_en.qm qt_de.qm
