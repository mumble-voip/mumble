include(../plugins.pri)

TARGET		= manual
CONFIG          += qt
isEqual(QT_MAJOR_VERSION, 5) {
	QT *= widgets
}
HEADERS		= manual.h
SOURCES		= manual.cpp
FORMS		+= manual.ui
