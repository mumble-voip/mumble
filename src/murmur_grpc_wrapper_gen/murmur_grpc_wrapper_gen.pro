include(../mumble.pri)

TEMPLATE = app
TARGET = murmur_grpc_wrapper_gen
LANGUAGE = C++
SOURCES = main.cpp
LIBS = -lprotoc
CONFIG -= qt
CONFIG += c++11
PKGCONFIG += protobuf

include(../../symbols.pri)
