include (../../compiler.pri)

TEMPLATE = lib
CONFIG *= debug_and_release warn_on
CONFIG *= staticlib
TARGET = effects11
SOURCES = EffectAPI.cpp \
    EffectLoad.cpp \
    EffectNonRuntime.cpp \
    EffectReflection.cpp \
    EffectRuntime.cpp \
    d3dxGlobal.cpp
HEADERS = pchfx.h \
    Inc/d3dx11effect.h \
    Inc/d3dxglobal.h \
    Binary/EffectBinaryFormat.h \
    Binary/EffectStateBase11.h \
    Binary/EffectStates11.h \
    Binary/SOParser.h \
    Effect.h \
    EffectLoad.h \
    IUnknownImp.h

DEFINES -= UNICODE

QMAKE_CXXFLAGS_RELEASE	-= -MD
QMAKE_CXXFLAGS_DEBUG	-= -MDd

QMAKE_CXXFLAGS_RELEASE	*= -MT
QMAKE_CXXFLAGS_DEBUG	*= -MTd

INCLUDEPATH *= Inc Binary
INCLUDEPATH *= $(DXSDK_DIR)Include
LIBS *= "$$(DXSDK_DIR)/Lib/x86/*"

CONFIG(debug, debug|release) {
  DEFINES *= DEBUG
}
