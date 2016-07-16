# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../mumble.pri)
include(../../python.pri)

DEFINES		*= MUMBLE
TEMPLATE	= app
TARGET		= mumble

!CONFIG(qt4-legacy-compat) {
  CONFIG += no-qt4-legacy-compat
}

CONFIG(no-qt4-legacy-compat):isEqual(QT_MAJOR_VERSION, 4) {
  error("$$escape_expand(\\n)$$escape_expand(\\n)"\
        "Mumble client support for Qt 4 is deprecated and will be dropped$$escape_expand(\\n)"\
        "completely in the future. We highly recommend switching to$$escape_expand(\\n)"\
        "building Mumble with Qt 5. For now CONFIG+=qt4-legacy-compat$$escape_expand(\\n)"\
        "can be used to build with Qt 4. Note that if built this way,$$escape_expand(\\n)"\
        "Mumble might lack certain bug-fixes and capabilities available$$escape_expand(\\n)"\
        "when built with Qt 5.$$escape_expand(\\n)"\
        "$$escape_expand(\\n)")
}

isEqual(QT_MAJOR_VERSION, 4) {
  warning("$$escape_expand(\\n)$$escape_expand(\\n)"\
          "Mumble client support for Qt 4 is deprecated and will be dropped$$escape_expand(\\n)"\
          "completely in the future. We highly recommend switching to$$escape_expand(\\n)"\
          "building Mumble with Qt 5. When built with Qt 4, Mumble might$$escape_expand(\\n)"\
          "lack certain bug-fixes and capabilities available when built$$escape_expand(\\n)"\
          "with Qt 5 already.$$escape_expand(\\n)"\
          "$$escape_expand(\\n)")
}

CONFIG(static) {
  # On Windows, building a static client
  # means building the main app into a DLL.
  win32 {
    TEMPLATE = lib
    TARGET = mumble_app
    VERSION =

    CONFIG -= static
    CONFIG += shared qt_static mumble_dll
    DEFINES += USE_MUMBLE_DLL
    isEqual(QT_MAJOR_VERSION, 5) {
      # Qt 5 uses an auto-generated .cpp file for importing plugins.
      # However, it is only automatically enabled for TEMPLATE = app.
      # Since we're building mumble_app.dll, we're not an app, but a library.
      # This means we'll have to explicitly ask Qt to generate and build the
      # plugin importer.
      CONFIG += force_import_plugins
    }
  }

  DEFINES *= USE_STATIC
  CONFIG += static_qt_plugins
}

QT		*= network sql xml svg
isEqual(QT_MAJOR_VERSION, 5) {
  QT *= widgets
  # Allow native widget access.
  win32:QT *= gui-private
  macx:QT *= gui-private
}

HEADERS *= BanEditor.h \
    ACLEditor.h \
    ConfigWidget.h \
    Log.h \
    AudioConfigDialog.h \
    AudioStats.h \
    AudioInput.h \
    AudioOutput.h \
    AudioOutputSample.h \
    AudioOutputSpeech.h \
    AudioOutputUser.h \
    CELTCodec.h \
    CustomElements.h \
    MainWindow.h \
    ServerHandler.h \
    About.h \
    ConnectDialog.h \
    GlobalShortcut.h \
    TextToSpeech.h \
    Settings.h \
    Database.h \
    VersionCheck.h \
    Global.h \
    UserModel.h \
    Audio.h \
    ConfigDialog.h \
    Plugins.h \
    PTTButtonWidget.h \
    LookConfig.h \
    Overlay.h \
    OverlayText.h \
    SharedMemory.h \
    AudioWizard.h \
    ViewCert.h \
    TextMessage.h \
    NetworkConfig.h \
    LCD.h \
    Usage.h \
    Cert.h \
    ClientUser.h \
    UserEdit.h \
    UserListModel.h \
    UserLocalVolumeDialog.h \
    Tokens.h \
    UserView.h \
    RichTextEditor.h \
    UserInformation.h \
    SocketRPC.h \
    VoiceRecorder.h \
    VoiceRecorderDialog.h \
    WebFetch.h \
    ../SignalCurry.h \
    OverlayClient.h \
    OverlayUser.h \
    OverlayUserGroup.h \
    OverlayConfig.h \
    OverlayEditor.h \
    OverlayEditorScene.h \
    MumbleApplication.h \
    ApplicationPalette.h \
    ThemeInfo.h \
    Themes.h \
    OverlayPositionableItem.h

SOURCES *= BanEditor.cpp \
    ACLEditor.cpp \
    ConfigWidget.cpp \
    Log.cpp \
    AudioConfigDialog.cpp \
    AudioStats.cpp \
    AudioInput.cpp \
    AudioOutput.cpp \
    AudioOutputSample.cpp \
    AudioOutputSpeech.cpp \
    AudioOutputUser.cpp \
    main.cpp \
    CELTCodec.cpp \
    CustomElements.cpp \
    MainWindow.cpp \
    ServerHandler.cpp \
    About.cpp \
    ConnectDialog.cpp \
    Settings.cpp \
    Database.cpp \
    VersionCheck.cpp \
    Global.cpp \
    UserModel.cpp \
    Audio.cpp \
    ConfigDialog.cpp \
    Plugins.cpp \
    PTTButtonWidget.cpp \
    LookConfig.cpp \
    OverlayClient.cpp \
    OverlayConfig.cpp \
    OverlayEditor.cpp \
    OverlayEditorScene.cpp \
    OverlayUser.cpp \
    OverlayUserGroup.cpp \
    Overlay.cpp \
    OverlayText.cpp \
    SharedMemory.cpp \
    AudioWizard.cpp \
    ViewCert.cpp \
    Messages.cpp \
    TextMessage.cpp \
    GlobalShortcut.cpp \
    NetworkConfig.cpp \
    LCD.cpp \
    Usage.cpp \
    Cert.cpp \
    ClientUser.cpp \
    UserEdit.cpp \
    UserListModel.cpp \
    UserLocalVolumeDialog.cpp \
    Tokens.cpp \
    UserView.cpp \
    RichTextEditor.cpp \
    UserInformation.cpp \
    SocketRPC.cpp \
    VoiceRecorder.cpp \
    VoiceRecorderDialog.cpp \
    WebFetch.cpp \
    MumbleApplication.cpp \
    ../../3rdparty/smallft-src/smallft.cpp \
    ThemeInfo.cpp \
    Themes.cpp \
    OverlayPositionableItem.cpp

DIST		*= ../../icons/mumble.ico licenses.h ../../icons/mumble.xpm murmur_pch.h mumble.plist
RESOURCES	*= mumble.qrc mumble_translations.qrc mumble_flags.qrc ../../themes/MumbleTheme.qrc
FORMS *= ConfigDialog.ui \
    MainWindow.ui \
    ConnectDialog.ui \
    ConnectDialogEdit.ui \
    BanEditor.ui \
    ACLEditor.ui \
    Plugins.ui \
    PTTButtonWidget.ui \
    Overlay.ui \
    OverlayEditor.ui \
    LookConfig.ui \
    AudioInput.ui \
    AudioOutput.ui \
    Log.ui \
    TextMessage.ui \
    AudioStats.ui \
    NetworkConfig.ui \
    LCD.ui \
    GlobalShortcut.ui \
    GlobalShortcutTarget.ui \
    Cert.ui \
    UserEdit.ui \
    UserLocalVolumeDialog.ui \
    AudioWizard.ui \
    Tokens.ui \
    RichTextEditor.ui \
    RichTextEditorLink.ui \
    UserInformation.ui \
    VoiceRecorderDialog.ui

# Include TRANSLATIONS variable
include(translations.pri)

PRECOMPILED_HEADER = mumble_pch.hpp
INCLUDEPATH *= ../../3rdparty/qqbonjour-src
INCLUDEPATH *= ../../3rdparty/smallft-src

CONFIG(static) {
  # Ensure that static Mumble.app on Mac OS X
  # includes and exports its Qt symbols to plugins.
  #
  # Some plugins (libmanual) already make use of Qt
  # functionality, and it's not inconceivable that some
  # Mumble features will be split into plugins in the
  # future.
  macx {
    QMAKE_LFLAGS -= -Wl,-dead_strip
    QMAKE_LFLAGS += -Wl,-all_load
  }
}

isEmpty(QMAKE_LRELEASE) {
  QMAKE_QMAKE_BASE = $$basename(QMAKE_QMAKE)
  QMAKE_LRELEASE_PATH = $$dirname(QMAKE_QMAKE)/$$replace(QMAKE_QMAKE_BASE,qmake,lrelease)
  isEqual(QT_MAJOR_VERSION, 5) {
    QMAKE_LRELEASE = $$shell_path($$QMAKE_LRELEASE_PATH)
  } else {
    QMAKE_LRELEASE = $$QMAKE_LRELEASE_PATH
  }
}

!CONFIG(no-manual-plugin) {
  SOURCES *= ManualPlugin.cpp
  HEADERS *= ManualPlugin.h
  FORMS *= ManualPlugin.ui
  DEFINES *= USE_MANUAL_PLUGIN
}

unix:!CONFIG(bundled-speex):system(pkg-config --atleast-version=1.2 speexdsp):system(pkg-config --atleast-version=1.2 speex) {
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

CONFIG(no-xinput2) {
  DEFINES	*= NO_XINPUT2
}

CONFIG(no-bundled-speex) {
  must_pkgconfig(speex)
  must_pkgconfig(speexdsp)
}

!CONFIG(no-bundled-speex) {
  INCLUDEPATH	*= ../../3rdparty/speex-src/include ../../3rdparty/speex-src/libspeex ../../3rdparty/speex-build ../../3rdparty/speexdsp-src/include ../../3rdparty/speexdsp-src/libspeexdsp
  LIBS 		*= -lspeex
}

CONFIG(sbcelt) {
  SOURCES -= CELTCodec.cpp
  SOURCES += CELTCodec_sbcelt.cpp
  INCLUDEPATH *= ../../3rdparty/celt-0.7.0-src/libcelt ../../3rdparty/sbcelt-src
  LIBS *= -lcelt -lsbcelt
  DEFINES *= SBCELT_PREFIX_API SBCELT_COMPAT_API USE_SBCELT
} else {
  unix:!CONFIG(bundled-celt):system(pkg-config --atleast-version=0.7.0 celt) {
    CONFIG	*= no-bundled-celt
  }
  CONFIG(no-bundled-celt) {
    INCLUDEPATH	*= /usr/include/celt
    unix {
      QMAKE_CFLAGS *= "-isystem /usr/include/celt"
      QMAKE_CXXFLAGS *= "-isystem /usr/include/celt"
    }
  }
  !CONFIG(no-bundled-celt) {
    INCLUDEPATH *= ../../3rdparty/celt-0.7.0-src/libcelt
    unix {
      QMAKE_CFLAGS *= "-isystem ../../3rdparty/celt-0.7.0-src/libcelt"
      QMAKE_CXXFLAGS *= "-isystem ../../3rdparty/celt-0.7.0-src/libcelt"
    }
  }
}

!win32:!macx:!CONFIG(no-dbus) {
  CONFIG		*= dbus
}
!contains(UNAME, FreeBSD):!CONFIG(no-g15) {
  CONFIG *= g15
}

!CONFIG(no-bonjour) {
  CONFIG *= bonjour
}

CONFIG(no-vorbis-recording) {
  DEFINES *= NO_VORBIS_RECORDING
}

unix:!CONFIG(bundled-opus):system(pkg-config --exists opus) {
  must_pkgconfig(opus)
  DEFINES *= USE_OPUS
} else {
  !CONFIG(no-opus) {
    CONFIG *= opus
  }

  CONFIG(opus) {
    INCLUDEPATH *= ../../3rdparty/opus-src/celt ../../3rdparty/opus-src/include ../../3rdparty/opus-src/src ../../3rdparty/opus-build/src
    DEFINES *= USE_OPUS
    LIBS *= -lopus
    unix {
      QMAKE_CFLAGS *= "-isystem  ../../3rdparty/opus-src/celt" "-isystem ../../3rdparty/opus-src/include"
      QMAKE_CXXFLAGS *= "-isystem  ../../3rdparty/opus-src/celt" "-isystem ../../3rdparty/opus-src/include"
    }
  }
}

win32 {
  CONFIG(mumble_dll) {
    RC_FILE = mumble_dll.rc
  } else {
    RC_FILE = mumble.rc
  }
  HEADERS	*= GlobalShortcut_win.h Overlay_win.h TaskList.h UserLockFile.h
  SOURCES	*= GlobalShortcut_win.cpp TextToSpeech_win.cpp Overlay_win.cpp SharedMemory_win.cpp Log_win.cpp os_win.cpp TaskList.cpp ../../overlay/ods.cpp UserLockFile_win.cpp
  LIBS		*= -ldxguid -ldinput8 -lsapi -lole32 -lws2_32 -ladvapi32 -lwintrust -ldbghelp -lshell32 -lshlwapi -luser32 -lgdi32 -lpsapi
  LIBS		*= -logg -lvorbis -lvorbisfile -lFLAC -lsndfile
  LIBS		*= -ldelayimp -delayload:shell32.dll

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
  !CONFIG(no-gkey) {
    CONFIG *= gkey
  }

  CONFIG(gkey) {
    HEADERS *= GKey.h
    SOURCES *= GKey.cpp
    DEFINES *= USE_GKEY
  }

  !CONFIG(no-xboxinput) {
    CONFIG *= xboxinput
  }
  CONFIG(xboxinput) {
    HEADERS *= XboxInput.h
    SOURCES *= XboxInput.cpp
    DEFINES *= USE_XBOXINPUT
  }

  # XInputCheck (3rdparty/xinputheck-src)
  INCLUDEPATH *= ../../3rdparty/xinputcheck-src
  LIBS *= -lxinputcheck

  !CONFIG(mumble_dll) {
    !CONFIG(no-elevation) {
      CONFIG(release, debug|release) {
        QMAKE_LFLAGS *= /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"
      }
    }
    QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest mumble.appcompat.manifest)
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
    QMAKE_CFLAGS *= -I../../3rdparty/speex-src/include -I../../3rdparty/speex-build
    QMAKE_CXXFLAGS *= -I../../3rdparty/speex-src/include -I../../3rdparty/speex-build
    QMAKE_CXXFLAGS_RELEASE *= -I../../3rdparty/speex-src/include -I../../3rdparty/speex-build
    QMAKE_CXXFLAGS_DEBUG *= -I../../3rdparty/speex-src/include -I../../3rdparty/speex-build
  }

  CONFIG *= link_pkgconfig

  must_pkgconfig(sndfile)

  macx {
    TARGET = Mumble
    ICON = ../../icons/mumble.icns
    QMAKE_INFO_PLIST = mumble.plist
    QMAKE_PKGINFO_TYPEINFO = MBLE

    LIBS += -framework Security -framework SecurityInterface -framework ApplicationServices

    HEADERS *= GlobalShortcut_macx.h ConfigDialogDelegate.h AppNap.h
    SOURCES *= SharedMemory_unix.cpp
    OBJECTIVE_SOURCES *= TextToSpeech_macx.mm GlobalShortcut_macx.mm os_macx.mm Log_macx.mm AppNap.mm

    !CONFIG(no-cocoa) {
        DEFINES *= USE_COCOA
        # Native feeling config dialog.
        OBJECTIVE_SOURCES += ConfigDialog_macx.mm ConfigDialogDelegate.mm
        HEADERS += ConfigDialog_macx.h
    }

    !CONFIG(universal) {
      # Link against libxar so we can inspect Mac OS X installer packages.
      CONFIG(static) {
        LIBS += -lxml2 -lbz2 -lxar
      } else {
        LIBS += -lxar
      }

      LIBS += -framework ScriptingBridge
      OBJECTIVE_SOURCES += Overlay_macx.mm
    } else {
      SOURCES += Overlay_unix.cpp
    }

    # CoreAudio
    LIBS += -framework CoreAudio -framework AudioUnit -framework AudioToolbox
    SOURCES += CoreAudio.cpp
    HEADERS += CoreAudio.h
  } else {
    HEADERS *= GlobalShortcut_unix.h
    SOURCES *= GlobalShortcut_unix.cpp TextToSpeech_unix.cpp Overlay_unix.cpp SharedMemory_unix.cpp Log_unix.cpp
    must_pkgconfig(x11)
    LIBS *= -lrt -lXi

    # For MumbleSSL::qsslSanityCheck()
    contains(UNAME, Linux) {
      LIBS *= -ldl
    }

    !CONFIG(no-oss) {
      CONFIG  *= oss
    }

    !CONFIG(no-alsa):contains(UNAME, Linux) {
      CONFIG *= alsa
    }

    !CONFIG(no-speechd) {
      CONFIG *= speechd
    } else {
      DEFINES *= USE_NO_TTS
    }
  }
}

alsa {
	DEFINES *= USE_ALSA
	must_pkgconfig(alsa)
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
	must_pkgconfig(libpulse)
	HEADERS *= PulseAudio.h
	SOURCES *= PulseAudio.cpp
}

portaudio {
	DEFINES *= USE_PORTAUDIO
	must_pkgconfig(portaudio-2.0)
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

	HEADERS *= \
		../../3rdparty/qqbonjour-src/BonjourRecord.h \
		../../3rdparty/qqbonjour-src/BonjourServiceResolver.h \
		../../3rdparty/qqbonjour-src/BonjourServiceBrowser.h \
		BonjourClient.h
	SOURCES *= \
		../../3rdparty/qqbonjour-src/BonjourServiceResolver.cpp \
		../../3rdparty/qqbonjour-src/BonjourServiceBrowser.cpp \
		BonjourClient.cpp
	win32 {
		INCLUDEPATH *= "$$BONJOUR_PATH/include"
		QMAKE_LIBDIR *= "$$BONJOUR_PATH/lib/win32"
		LIBS *= -lDNSSD
	}
	unix:!macx {
		system(pkg-config --exists avahi-compat-libdns_sd avahi-client) {
			must_pkgconfig(avahi-compat-libdns_sd)
			must_pkgconfig(avahi-client)
		} else {
			LIBS *= -ldns_sd
		}
	}
}

dbus {
	DEFINES *= USE_DBUS
	QT *= dbus
	HEADERS *= DBus.h
	SOURCES *= DBus.cpp
}

speechd {
	DEFINES *= USE_SPEECHD
	system(pkg-config --atleast-version=0.8 speech-dispatcher) {
		DEFINES *= USE_SPEECHD_PKGCONFIG
		must_pkgconfig(speech-dispatcher)
	} else {
		LIBS *= -lspeechd
		INCLUDEPATH	*= /usr/include/speech-dispatcher
	}
}

directsound {
	DEFINES *= USE_DIRECTSOUND
	HEADERS	*= DirectSound.h
	SOURCES	*= DirectSound.cpp
	LIBS	*= -ldsound
}

wasapi {
	DEFINES *= USE_WASAPI
	HEADERS	*= WASAPI.h WASAPINotificationClient.h
	SOURCES	*= WASAPI.cpp WASAPINotificationClient.cpp
	LIBS	*= -lAVRT -delayload:AVRT.DLL
}

g15 {
	DEFINES *= USE_G15
	win32|macx {
		SOURCES *= G15LCDEngine_helper.cpp
		HEADERS *= G15LCDEngine_helper.h ../../g15helper/g15helper.h
	}
	unix:!macx:!contains(UNAME, FreeBSD) {
		SOURCES *= G15LCDEngine_unix.cpp
		HEADERS *= G15LCDEngine_unix.h
		LIBS *= -lg15daemon_client
	}
}

CONFIG(no-update) {
	DEFINES *= NO_UPDATE_CHECK
}

!CONFIG(no-embed-qt-translations):!exists($$[QT_INSTALL_TRANSLATIONS]) {
  error("$$escape_expand(\\n)$$escape_expand(\\n)"\
        "The QT_INSTALL_TRANSLATIONS directory ($$[QT_INSTALL_TRANSLATIONS])$$escape_expand(\\n)"\
	"does not exist.$$escape_expand(\\n)"\
	"$$escape_expand(\\n)"\
	"The Mumble build process is attempting to embed Qt translations into the Mumble binary,$$escape_expand(\\n)"\
	"but it cannot, because the files are missing.$$escape_expand(\\n)"\
	"$$escape_expand(\\n)"\
	"If you wish to embed Qt translations into the Mumble binary,$$escape_expand(\\n)"\
	"you will need to install the translation package for your verison of Qt.$$escape_expand(\\n)"\
	"For example, On Ubuntu with Qt 5, that package is 'qttranslations5-l10n'.$$escape_expand(\\n)"\
	"$$escape_expand(\\n)"\
	"You can also tell the Mumble build process to not embed Qt's$$escape_expand(\\n)"\
	"translations into the Mumble binary by using the 'no-embed-qt-translations'$$escape_expand(\\n)"\
	"CONFIG option when running qmake, such as:$$escape_expand(\\n)"\
	"$$escape_expand(\\n)"\
	"    $ qmake -recursive main.pro CONFIG+=$$escape_expand(\")no-embed-qt-translations$$escape_expand(\")$$escape_expand(\\n)"\
	"$$escape_expand(\\n)"\
	"Please refer to the INSTALL file at the root of the source tree for more information$$escape_expand(\\n)"\
	"about the build process.$$escape_expand(\\n)"\
        "$$escape_expand(\\n)")
}

!CONFIG(no-embed-qt-translations) {
	QT_TRANSLATIONS_FALLBACK_DIR = qttranslations
	QT_TRANSLATIONS_FALLBACK_FILES = $$files($$QT_TRANSLATIONS_FALLBACK_DIR/*.ts)
	for(fn, QT_TRANSLATIONS_FALLBACK_FILES) {
		!system($$QMAKE_LRELEASE -silent $$fn) {
			error(Failed to run lrelease for $$fn)
		}
	}
	GENQRC = $$PYTHON ../../scripts/generate-mumble_qt-qrc.py
	win32 {
		GENQRC = $$PYTHON ..\\..\\scripts\\generate-mumble_qt-qrc.py
	}
	!system($$GENQRC mumble_qt_auto.qrc $$[QT_INSTALL_TRANSLATIONS] $$QT_TRANSLATIONS_FALLBACK_DIR) {
		error(Failed to run generate-mumble_qt-qrc.py script)
	}
	RESOURCES *= mumble_qt_auto.qrc
}

!CONFIG(no-embed-tango-icons) {
	RESOURCES *= mumble_tango.qrc
}

CONFIG(static_qt_plugins) {
  DEFINES += USE_STATIC_QT_PLUGINS
  QTPLUGIN += qtaccessiblewidgets qico qsvg qsvgicon

  # The accessiblewidgets plugin is not needed for Qt 5.4 and greater,
  # where it is integrated into the QtWidgets library.
  # See QTBUG-43007 for more information.
  isEqual(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 3) {
    QTPLUGIN -= qtaccessiblewidgets
  }

  macx {
    isEqual(QT_MAJOR_VERSION, 5) {
      QTPLUGIN += qicns qcocoa
    } else {
      QTPLUGIN += qicnsicon
    }
  }

  win32 {
    isEqual(QT_MAJOR_VERSION, 5) {
      QTPLUGIN += qwindows
    }
  }

  # Icon engines are special; they don't get their lib directory
  # included automatically by mkspecs/features/qt.prf
  LIBS *= -L$$[QT_INSTALL_PLUGINS]/iconengines
}

lrel.output = ${QMAKE_FILE_BASE}.qm
lrel.commands = $${QMAKE_LRELEASE} ${QMAKE_FILE_NAME}
lrel.input = TRANSLATIONS
lrel.CONFIG *= no_link
lrel.variable_out = rcc.depends

QMAKE_EXTRA_COMPILERS *= lrel

include(../../symbols.pri)
