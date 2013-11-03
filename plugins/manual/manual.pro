include(../plugins.pri)

TARGET		= manual
CONFIG          += qt
isEqual(QT_MAJOR_VERSION, 5) {
    QT *= widgets
    # On Windows, we use QPlatformNativeInterface
    # to get access to HWNDs.
    win32 {
        QT *= gui-private
    }
}
HEADERS		= manual.h
SOURCES		= manual.cpp
FORMS		+= manual.ui

# Link against mumble_app.dll when
# building for win32-static.
win32:CONFIG(qt_dynamic_lookup) {
    CONFIG(debug, debug|release) {
        LIBPATH *= ../../debug
    }

    CONFIG(release, debug|release) {
        LIBPATH *= ../../release
    }

    LIBS *= -lmumble_app
}
