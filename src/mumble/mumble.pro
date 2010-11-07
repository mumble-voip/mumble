include(../mumble.pri)

DEFINES		*= MUMBLE
TEMPLATE	= app
QT		*= network sql opengl xml svg
TARGET		= mumble
HEADERS		*= BanEditor.h ACLEditor.h ConfigWidget.h Log.h AudioConfigDialog.h AudioStats.h AudioInput.h AudioOutput.h CustomElements.h MainWindow.h ServerHandler.h About.h ConnectDialog.h GlobalShortcut.h TextToSpeech.h Settings.h Database.h VersionCheck.h Global.h UserModel.h Audio.h ConfigDialog.h Plugins.h LookConfig.h Overlay.h OverlayText.h SharedMemory.h AudioWizard.h ViewCert.h TextMessage.h NetworkConfig.h LCD.h Usage.h Cert.h ClientUser.h UserEdit.h Tokens.h UserView.h RichTextEditor.h UserInformation.h FileEngine.h SocketRPC.h VoiceRecorder.h VoiceRecorderDialog.h
SOURCES		*= BanEditor.cpp ACLEditor.cpp ConfigWidget.cpp Log.cpp AudioConfigDialog.cpp AudioStats.cpp AudioInput.cpp AudioOutput.cpp main.cpp CustomElements.cpp MainWindow.cpp ServerHandler.cpp About.cpp ConnectDialog.cpp Settings.cpp Database.cpp VersionCheck.cpp Global.cpp UserModel.cpp Audio.cpp ConfigDialog.cpp Plugins.cpp LookConfig.cpp OverlayClient.cpp OverlayConfig.cpp OverlayEditor.cpp OverlayEditorScene.cpp OverlayUser.cpp OverlayUserGroup.cpp Overlay.cpp OverlayText.cpp SharedMemory.cpp AudioWizard.cpp ViewCert.cpp Messages.cpp TextMessage.cpp GlobalShortcut.cpp NetworkConfig.cpp LCD.cpp Usage.cpp Cert.cpp ClientUser.cpp UserEdit.cpp Tokens.cpp UserView.cpp RichTextEditor.cpp UserInformation.cpp FileEngine.cpp SocketRPC.cpp VoiceRecorder.cpp VoiceRecorderDialog.cpp
SOURCES *= smallft.cpp
DIST		*= ../../icons/mumble.ico licenses.h smallft.h ../../icons/mumble.xpm murmur_pch.h mumble.plist
RESOURCES	*= mumble.qrc mumble_flags.qrc
FORMS	*= ConfigDialog.ui MainWindow.ui ConnectDialog.ui ConnectDialogEdit.ui BanEditor.ui ACLEditor.ui Plugins.ui Overlay.ui OverlayEditor.ui LookConfig.ui AudioInput.ui AudioOutput.ui Log.ui TextMessage.ui AudioStats.ui NetworkConfig.ui LCD.ui GlobalShortcut.ui GlobalShortcutTarget.ui Cert.ui UserEdit.ui AudioWizard.ui Tokens.ui RichTextEditor.ui RichTextEditorLink.ui UserInformation.ui VoiceRecorderDialog.ui
TRANSLATIONS	= mumble_en.ts mumble_es.ts mumble_de.ts mumble_fr.ts mumble_pl.ts mumble_ru.ts mumble_cs.ts mumble_it.ts mumble_ja.ts mumble_pt_BR.ts mumble_zh_CN.ts mumble_zh_TW.ts mumble_da.ts mumble_he.ts mumble_sv.ts
PRECOMPILED_HEADER = mumble_pch.hpp
INCLUDEPATH *= ../bonjour

isEmpty(QMAKE_LRELEASE) {
  QMAKE_QMAKE_BASE = $$basename(QMAKE_QMAKE)
  QMAKE_LRELEASE = $$dirname(QMAKE_QMAKE)/$$replace(QMAKE_QMAKE_BASE,qmake,lrelease)
}

unix:!CONFIG(bundled-speex):system(pkg-config --atleast-version=1.2 speexdsp) {
  CONFIG	*= no-bundled-speex
}

CONFIG(no-crash-report) {
  DEFINES	*= NO_CRASH_REPORT
} else:unix:!macx {
  DEFINES	*= NO_CRASH_REPORT
} else {
  HEADERS	*= CrashReporter.h
  SOURCES	*= CrashReporter.cpp
}

CONFIG(no-bundled-speex) {
  PKGCONFIG	*= speex speexdsp
}

!CONFIG(no-bundled-speex) {
  INCLUDEPATH	*= ../../speex/include ../../speex/libspeex ../../speexbuild
  LIBS 		*= -lspeex
}

unix:!CONFIG(bundled-celt):system(pkg-config --atleast-version=0.7.0 celt) {
  CONFIG	*= no-bundled-celt
}

CONFIG(no-bundled-celt) {
  INCLUDEPATH	*= /usr/include/celt
}

!CONFIG(no-bundled-celt) {
  INCLUDEPATH	*= ../../celt-0.7.0-src/libcelt
}

!win32 {
  QMAKE_CXXFLAGS	*= -Wall -Wextra
}

!win32:!macx:!CONFIG(no-dbus) {
  CONFIG		*= dbus
}

!CONFIG(no-g15) {
  CONFIG *= g15
}

!CONFIG(no-bonjour) {
  CONFIG *= bonjour
}

CONFIG(no-vorbis-recording) {
  DEFINES *= NO_VORBIS_RECORDING
}

win32 {
  RC_FILE	= mumble.rc
  HEADERS	*= GlobalShortcut_win.h TaskList.h
  SOURCES	*= GlobalShortcut_win.cpp TextToSpeech_win.cpp Overlay_win.cpp SharedMemory_win.cpp os_win.cpp TaskList.cpp ../../overlay/HardHook.cpp
  LIBS		*= -l"$$(DXSDK_DIR)Lib/x86/dxguid" -l"$$(DXSDK_DIR)Lib/x86/dinput8" -lsapi -lole32 -lws2_32 -ladvapi32 -lwintrust -ldbghelp -llibsndfile-1 -lshell32 -lshlwapi
  LIBS		*= -ldelayimp -delayload:speex.dll -delayload:shell32.dll

  DEFINES	*= WIN32
  !CONFIG(no-asio) {
    CONFIG	*= asio
  }
  !CONFIG(no-directsound) {
    CONFIG	*= directsound
  }
  !CONFIG(no-wasapi) {
    CONFIG	*= wasapi
  }

  CONFIG(release, debug|release) {
    QMAKE_LFLAGS *= /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"
  }
}

unix {
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

  PKGCONFIG *= openssl sndfile

  macx {
    TARGET = Mumble
    ICON = ../../icons/mumble.icns
    QMAKE_INFO_PLIST = mumble.plist
    QMAKE_PKGINFO_TYPEINFO = MBLE

    LIBS += -framework Security -framework SecurityInterface -framework ScriptingBridge -framework ApplicationServices

    HEADERS *= GlobalShortcut_macx.h ConfigDialogDelegate.h
    SOURCES *= TextToSpeech_macx.cpp SharedMemory_unix.cpp GlobalShortcut_macx.mm os_macx.mm

    !CONFIG(no-cocoa) {
        # Link against libxar so we can inspect Mac OS X installer packages.
        LIBS += -lxar
        # Native feeling config dialog.
        SOURCES += ConfigDialog_macx.mm ConfigDialogDelegate.mm Overlay_macx.mm
        HEADERS += ConfigDialog_macx.h
    } else {
        SOURCES += Overlay_unix.cpp
    }

    # CoreAudio
    LIBS += -framework CoreAudio -framework AudioUnit -framework AudioToolbox
    SOURCES += CoreAudio.cpp
    HEADERS += CoreAudio.h
  } else {
    HEADERS *= GlobalShortcut_unix.h
    SOURCES *= GlobalShortcut_unix.cpp TextToSpeech_unix.cpp Overlay_unix.cpp SharedMemory_unix.cpp
    PKGCONFIG *= x11
    LIBS *= -lrt -lXi

    !CONFIG(no-oss) {
      CONFIG  *= oss
    }

    !CONFIG(no-alsa):contains(UNAME, Linux) {
      CONFIG *= alsa
    }

    !CONFIG(no-speechd) {
      CONFIG *= speechd
    }
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
	INCLUDEPATH *= "$$ASIO_PATH/common" "$$ASIO_PATH/host" "$$ASIO_PATH/host/pc"
}

bonjour {
	DEFINES *= USE_BONJOUR

	HEADERS *= ../bonjour/bonjourrecord.h ../bonjour/bonjourserviceresolver.h ../bonjour/bonjourservicebrowser.h BonjourClient.h
	SOURCES *= ../bonjour/bonjourserviceresolver.cpp ../bonjour/bonjourservicebrowser.cpp BonjourClient.cpp
	win32 {
		INCLUDEPATH *= "$$BONJOUR_PATH/include"
		QMAKE_LIBDIR *= "$$BONJOUR_PATH/lib/win32"
		LIBS *= -lDNSSD
	}
	unix:!macx {
		system(pkg-config --exists avahi-compat-libdns_sd avahi-client) {
			PKGCONFIG *= avahi-compat-libdns_sd avahi-client
		} else {
			LIBS *= -ldns_sd
		}
	}
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
	LIBS	*= -lAVRT -delayload:AVRT.DLL
}

g15 {
	DEFINES *= USE_G15

	win32 {
		LIBS *= -llgLcd
		QMAKE_LIBDIR *= "$$G15SDK_PATH/Libs/x86"
		INCLUDEPATH *= "$$G15SDK_PATH/Src"
		DEFINES *= WIN32

		SOURCES *= G15LCDEngine_lglcd.cpp
		HEADERS *= G15LCDEngine_lglcd.h
	}

	unix:!macx {
		SOURCES *= G15LCDEngine_unix.cpp
		HEADERS *= G15LCDEngine_unix.h
		LIBS *= -lg15daemon_client
	}

	macx {
		SOURCES *= G15LCDEngine_helper.cpp
		HEADERS *= G15LCDEngine_helper.h ../../g15helper/g15helper.h
	}
}

CONFIG(no-update) {
	DEFINES *= NO_UPDATE_CHECK
}

!CONFIG(no-embed-qt-translations) {
	QT_TRANSDIR = $$[QT_INSTALL_TRANSLATIONS]/
	QT_TRANSDIR = $$replace(QT_TRANSDIR,/,$${DIR_SEPARATOR})

	QT_TRANSDIR = $$[QT_INSTALL_TRANSLATIONS]/
	QT_TRANSDIR = $$replace(QT_TRANSDIR,/,$${DIR_SEPARATOR})

	QT_TRANSLATION_FILES_SRC *= qt_de.qm qt_es.qm qt_fr.qm qt_ja.qm qt_ja_JP.qm qt_ru.qm qt_pl.qm qt_pt.qm qt_sv.qm qt_zh_CN.qm qt_zh_TW.qm

	for(lang, QT_TRANSLATION_FILES_SRC):exists($$[QT_INSTALL_TRANSLATIONS]/$${lang}) {
		QT_TRANSLATION_FILES *= $${lang}
	}

	copytrans.output = ${QMAKE_FILE_NAME}
	copytrans.commands = $$QMAKE_COPY $${QT_TRANSDIR}${QMAKE_FILE_NAME} ${QMAKE_FILE_OUT}
	copytrans.input = QT_TRANSLATION_FILES
	copytrans.CONFIG *= no_link target_predeps

	QMAKE_EXTRA_COMPILERS *= copytrans

	RESOURCES *= mumble_qt.qrc
}

!CONFIG(no-embed-tango-icons) {
	RESOURCES *= mumble_tango.qrc
}

lrel.output = ${QMAKE_FILE_BASE}.qm
lrel.commands = $${QMAKE_LRELEASE} ${QMAKE_FILE_NAME}
lrel.input = TRANSLATIONS
lrel.CONFIG *= no_link target_predeps

QMAKE_EXTRA_COMPILERS *= lrel

include(../../symbols.pri)
