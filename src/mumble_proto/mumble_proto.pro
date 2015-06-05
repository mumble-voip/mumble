include(../../compiler.pri)

PROTOBUF	*= ../Mumble.proto

pbh.output = ${QMAKE_FILE_BASE}.pb.h
pbh.depends = ${QMAKE_FILE_BASE}.pb.cc
pbh.commands = $$escape_expand(\\n)
pbh.input = PROTOBUF
pbh.CONFIG *= no_link explicit_dependencies target_predeps
pbh.variable_out = HEADERS

pb.output = ${QMAKE_FILE_BASE}.pb.cc
pb.commands = protoc --cpp_out=. -I. -I.. ${QMAKE_FILE_NAME}
pb.input = PROTOBUF
pb.CONFIG *= no_link explicit_dependencies
pb.variable_out = SOURCES

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += staticlib

INCLUDEPATH *= "$$PROTOBUF_PATH/vsprojects/include" "$$PROTOBUF_PATH/src" protobuf

QMAKE_EXTRA_COMPILERS *= pb pbh

!CONFIG(third-party-warnings) {
    # We ignore warnings in third party builds. We won't actually look
    # at them and they clutter out our warnings.
    CONFIG -= warn_on
    CONFIG += warn_off
}

CONFIG(debug, debug|release) {
	DESTDIR = ../../debug
}

CONFIG(release, debug|release) {
	DESTDIR = ../../release
}

include(../../symbols.pri)

