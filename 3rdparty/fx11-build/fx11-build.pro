include(../../compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-build,-src)

!exists(../$$SOURCEDIR/Effect.h) {
	message("The $$SOURCEDIR/ directory was not found. You need to do one of the following:")
	message("")
	message("git submodule init")
	message("git submodule update")
	message("")
	error("Aborting configuration")
}

TEMPLATE = lib
CONFIG *= debug_and_release warn_on
CONFIG *= staticlib
VPATH	= ../$$SOURCEDIR
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

DEFINES += WIN32 PROFILE _WINDOWS _LIB D3DXFX_LARGEADDRESS_HANDLE

QMAKE_CXXFLAGS_RELEASE	-= -MD
QMAKE_CXXFLAGS_DEBUG	-= -MDd

QMAKE_CXXFLAGS_RELEASE	*= -MT
QMAKE_CXXFLAGS_DEBUG	*= -MTd

INCLUDEPATH *= ../$$SOURCEDIR/Inc ../$$SOURCEDIR/Binary
INCLUDEPATH *= $(DXSDK_DIR)Include

CONFIG(release, debug|release) {
  DEFINES *= NDEBUG
}

CONFIG(debug, debug|release) {
  DEFINES *= DEBUG _DEBUG
}
