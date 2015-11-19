include(../mumble.pri)

TEMPLATE = app
TARGET = protoc-gen-grpcwrapper
LANGUAGE = C++
SOURCES = main.cpp
LIBS = -lprotoc
CONFIG -= qt
CONFIG += c++11
PKGCONFIG += protobuf

include(../../symbols.pri)
