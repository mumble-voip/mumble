include(../../compiler.pri)

TEMPLATE = app
CONFIG += qt thread warn_on debug console
CONFIG -= app_bundle
QT += network gui svg
LANGUAGE = C++
TARGET = IconSync
SOURCES = IconSync.cpp
