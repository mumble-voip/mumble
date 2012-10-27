TEMPLATE = app
CONFIG += qt thread warn_on release qtestlib no_keywords
CONFIG -= app_bundle
LANGUAGE = C++
TARGET = ProtoBuf
HEADERS = Message.h Timer.h
PROTOS = ProtoBuf.proto
SOURCES = ProtoBuf.cpp Timer.cpp Message.cpp ProtoBuf.pb.cc
VPATH += .. proto
INCLUDEPATH += .. ../murmur ../mumble proto
LIBS += -lboost_signals -lprotobuf
QMAKE_CXXFLAGS *= -O3
DEFINES *= NDEBUG

protoc.output = ${QMAKE_FILE_BASE}.pb.cc ${QMAKE_FILE_BASE}.pb.h
protoc.commands = protoc ${QMAKE_FILE_NAME} --cpp_out=.
protoc.input = PROTOS
protoc.CONFIG *= no_link target_predeps

QMAKE_EXTRA_COMPILERS *= protoc
