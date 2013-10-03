include (../../compiler.pri)

TEMPLATE = lib
CONFIG *= debug_and_release warn_on
#CONFIG *= staticlib
//CONFIG -= embed_manifest_dll
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

INCLUDEPATH *= $(DXSDK_DIR)Include
#INCLUDEPATH *= "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
INCLUDEPATH *= Inc Binary
#LIBS *= "$(DXSDK_DIR)/Lib/x86/*"
LIBS *= "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/*"

CONFIG(release, debug|release) {
  #DESTDIR       = ../../release
}

CONFIG(debug, debug|release) {
  #DESTDIR       = ../../debug
  DEFINES	*= DEBUG
}
