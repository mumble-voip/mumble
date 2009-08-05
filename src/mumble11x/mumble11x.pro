include(../../compiler.pri)

TEMPLATE	= app
QT		*= network sql opengl xml
TARGET		= mumble11x
HEADERS		= BanEditor.h ACLEditor.h Log.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h MainWindow.h ServerHandler.h About.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h PlayerModel.h Audio.h ConfigDialog.h Plugins.h LookConfig.h Overlay.h  AudioWizard.h ViewCert.h TextMessage.h NetworkConfig.h LCD.h Usage.h ConfigWidget.h
SOURCES		= BanEditor.cpp ACLEditor.cpp Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp MainWindow.cpp ServerHandler.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp PlayerModel.cpp Audio.cpp ConfigDialog.cpp Plugins.cpp LookConfig.cpp Overlay.cpp AudioWizard.cpp ViewCert.cpp Messages.cpp TextMessage.cpp GlobalShortcut.cpp NetworkConfig.cpp LCD.cpp Usage.cpp ConfigWidget.cpp
SOURCES *= ../Timer.cpp ../CryptState.cpp ../OSInfo.cpp
HEADERS *= Player.h  Channel.h ACL.h Connection.h Group.cpp
SOURCES *= Message.cpp Player.cpp Channel.cpp ACL.cpp Connection.cpp
SOURCES *= smallft.cpp
DIST		*= ../../icons/mumble.ico licenses.h smallft.h ../../icons/mumble.xpm murmur_pch.h mumble.plist
RESOURCES	*= mumble.qrc
FORMS	*= ConfigDialog.ui MainWindow.ui ConnectDialog.ui BanEditor.ui ACLEditor.ui Plugins.ui Overlay.ui LookConfig.ui AudioInput.ui AudioOutput.ui Log.ui TextMessage.ui AudioStats.ui NetworkConfig.ui LCD.ui GlobalShortcut.ui
TRANSLATIONS	= mumble_en.ts mumble_es.ts mumble_de.ts mumble_fr.ts mumble_ru.ts mumble_cs.ts mumble_ja.ts mumble_pl.ts
PRECOMPILED_HEADER = mumble_pch.hpp

MUMBLE_COPY = ASIOInput.h ASIOInput.cpp DirectSound.h DirectSound.cpp WASAPI.h WASAPI.cpp AudioStats.h AudioStats.cpp AudioStats.ui ConfigDialog.h ConfigDialog.cpp ConfigDialog.ui DBus.h GlobalShortcut_win.cpp GlobalShortcut_win.h licenses.h mumble_pch.hpp os_macx.cpp smallft.h smallft.cpp TextMessage.h TextMessage.cpp TextMessage.ui TextToSpeech.h TextToSpeech_win.cpp Usage.h Usage.cpp VersionCheck.h VersionCheck.cpp ViewCert.h ViewCert.cpp ConfigWidget.h ConfigWidget.cpp

VERSION		= 1.1.8
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=1.1.8
DEFINES 	*= MUMBLE_VERSION="1.1.x"
DEFINES		*= COMPAT_CLIENT

INCLUDEPATH	+= ..

CONFIG(debug, debug|release) {
  CONFIG += console
  LIBPATH	+= ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  LIBPATH	+= ../../release
  DESTDIR	= ../../release
}


isEmpty(QMAKE_LRELEASE) {
  QMAKE_QMAKE_BASE = $$basename(QMAKE_QMAKE)
  QMAKE_LRELEASE = $$dirname(QMAKE_QMAKE)/$$replace(QMAKE_QMAKE_BASE,qmake,lrelease)
}

unix:!CONFIG(bundled-speex):system(pkg-config --atleast-version=1.2 speexdsp) {
  CONFIG	*= no-bundled-speex
}

CONFIG(no-bundled-speex) {
  PKGCONFIG	*= speex speexdsp
}

!CONFIG(no-bundled-speex) {
  INCLUDEPATH	*= ../../speex/include ../../speex/libspeex ../../speexbuild
  LIBS 		*= -lspeexdsp
}

!win32 {
  QMAKE_CXXFLAGS	*= -Wall -Wextra
}

win32 {
  RC_FILE	= mumble.rc
  HEADERS	*= GlobalShortcut_win.h
  SOURCES	*= GlobalShortcut_win.cpp TextToSpeech_win.cpp Overlay_win.cpp os_win.cpp
  LIBS		*= -ldxguid -ldinput8 -lsapi -lole32 -lws2_32 -llibeay32 -ladvapi32 -llibogg
  LIBS		*= -ldelayimp -lQwave -delayload:Qwave.DLL

  LIBPATH	*= /dev/WinSDK/Lib/i386 /dev/dxsdk/Lib/x86 /dev/OpenSSL/lib /dev/libogg/lib
  DEFINES	*= WIN32
  INCLUDEPATH	*= /dev/OpenSSL/include /dev/libogg/include
  !CONFIG(no-asio) {
    CONFIG	*= asio
  }
  !CONFIG(no-directsound) {
    CONFIG	*= directsound
  }
  !CONFIG(no-wasapi) {
    CONFIG	*= wasapi
  }
}

unix {
  UNAME=$$system(uname -s)

  HAVE_PULSEAUDIO=$$system(pkg-config --modversion --silence-errors libpulse)
  HAVE_PORTAUDIO=$$system(pkg-config --modversion --silence-errors portaudio-2.0)

  !isEmpty(HAVE_PORTAUDIO):!CONFIG(no-portaudio) {
    CONFIG *= portaudio
  }

  !isEmpty(HAVE_PULSEAUDIO):!CONFIG(no-pulseaudio) {
    CONFIG -= portaudio
    CONFIG *= pulseaudio
  }

  !CONFIG(no-bundled-speex) {
    QMAKE_CFLAGS *= -I../../speex/include -I../../speexbuild
    QMAKE_CXXFLAGS *= -I../../speex/include -I../../speexbuild
    QMAKE_CXXFLAGS_RELEASE *= -I../../speex/include -I../../speexbuild
    QMAKE_CXXFLAGS_DEBUG *= -I../../speex/include -I../../speexbuild
  }

  CONFIG *= link_pkgconfig

  PKGCONFIG *= openssl ogg

  contains(UNAME, Linux) {
    !CONFIG(no-oss) {
      CONFIG  *= oss
    }

    !CONFIG(no-alsa) {
      CONFIG *= alsa
    }

    !CONFIG(no-speechd) {
      CONFIG *= speechd
    }

    HEADERS *= GlobalShortcut_unix.h
    SOURCES *= GlobalShortcut_unix.cpp TextToSpeech_unix.cpp Overlay_unix.cpp
  }

  !macx {
    LIBS *= -lXi
  }

  macx {
    TARGET = Mumble11x
    ICON = ../../icons/mumble.icns
    QMAKE_INFO_PLIST = mumble.plist
    QMAKE_PKGINFO_TYPEINFO = MBLE

    HEADERS *= GlobalShortcut_macx.h
    SOURCES *= TextToSpeech_macx.cpp Overlay_unix.cpp GlobalShortcut_macx.cpp os_macx.cpp

    QT_CONF.path = Contents/Resources
    QT_CONF.files = ../../scripts/qt.conf
    QMAKE_BUNDLE_DATA += QT_CONF
  }
}

alsa {
	DEFINES *= USE_ALSA
	PKGCONFIG *= alsa
	HEADERS *= ALSAAudio.h
	SOURCES *= ALSAAudio.cpp
}

oss {
	DEFINES *= USE_OSS
	HEADERS *= OSS.h
	SOURCES *= OSS.cpp
	INCLUDEPATH *= /usr/lib/oss/include
}

pulseaudio {
	DEFINES *= USE_PULSEAUDIO
	PKGCONFIG *= libpulse
	HEADERS *= PulseAudio.h
	SOURCES *= PulseAudio.cpp
}

portaudio {
	DEFINES *= USE_PORTAUDIO
	PKGCONFIG *= portaudio-2.0
	HEADERS *= PAAudio.h
	SOURCES *= PAAudio.cpp
}

asio {
	DEFINES *= USE_ASIO
	HEADERS *= ASIOInput.h
	SOURCES	*= ASIOInput.cpp
	FORMS *= ASIOInput.ui
	INCLUDEPATH *= ../../asio/common ../../asio/host ../../asio/host/pc
}

dbus {
	DEFINES *= USE_DBUS
	CONFIG *= qdbus
	HEADERS *= DBus.h
	SOURCES *= DBus.cpp
}

speechd {
	DEFINES *= USE_SPEECHD
	LIBS *= -lspeechd
}

directsound {
	DEFINES *= USE_DIRECTSOUND
	HEADERS	*= DirectSound.h
	SOURCES	*= DirectSound.cpp
	LIBS	*= -ldsound
}

wasapi {
	DEFINES *= USE_WASAPI
	HEADERS	*= WASAPI.h
	SOURCES	*= WASAPI.cpp
	LIBS	*= -ldelayimp -lAVRT -delayload:AVRT.DLL
}

DEFINES *= NO_UPDATE_CHECK

!CONFIG(no-embed-qt-translations) {
	QT_TRANSDIR = $$[QT_INSTALL_TRANSLATIONS]/
	QT_TRANSDIR = $$replace(QT_TRANSDIR,/,$${DIR_SEPARATOR})

	QT_TRANSLATION_FILES *= qt_de.qm qt_es.qm qt_fr.qm qt_ru.qm qt_pl.qm qt_ja_JP.qm

	copytrans.output = ${QMAKE_FILE_NAME}
	copytrans.commands = $$QMAKE_COPY $${QT_TRANSDIR}${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
	copytrans.input = QT_TRANSLATION_FILES
	copytrans.CONFIG *= no_link target_predeps

	QMAKE_EXTRA_COMPILERS *= copytrans
	
	RESOURCES *= mumble_qt.qrc
}

lrel.output = ${QMAKE_FILE_BASE}.qm
lrel.commands = $${QMAKE_LRELEASE} ${QMAKE_FILE_NAME} 
lrel.input = TRANSLATIONS
lrel.CONFIG *= no_link target_predeps

QMAKE_EXTRA_COMPILERS *= lrel

winsymlink.output = ${QMAKE_FILE_NAME}
winsymlink.commands = $${QMAKE_COPY} ../mumble/${QMAKE_FILE_NAME} .
winsymlink.input = MUMBLE_COPY
winsymlink.depends = ../mumble/${QMAKE_FILE_NAME}
winsymlink.CONFIG *= no_link target_predeps

QMAKE_EXTRA_COMPILERS *= winsymlink
