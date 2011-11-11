include (../compiler.pri)

TEMPLATE = lib
CONFIG *= dll debug_and_release warn_on
CONFIG -= embed_manifest_dll
TARGET = effects11
SOURCES = effects11.cpp

DEFINES -= UNICODE

QMAKE_CXXFLAGS_RELEASE	-= -MD
QMAKE_CXXFLAGS_DEBUG	-= -MDd

QMAKE_CXXFLAGS_RELEASE	*= -MT
QMAKE_CXXFLAGS_DEBUG	*= -MTd

INCLUDEPATH *= "$$(DXSDK_DIR)Include"

EFFECTS11DIR = $$(DXSDK_DIR)Samples/C++/Effects11/
EFFECTS11PATH = . Inc Binary
EFFECTS11SOURCES = EffectAPI.cpp EffectLoad.cpp EffectNonRuntime.cpp EffectReflection.cpp EffectRuntime.cpp d3dx11dbg.cpp d3dxGlobal.cpp

SOURCES *= $$join(EFFECTS11SOURCES," $$EFFECTS11DIR","$$EFFECTS11DIR")
INCLUDEPATH *= $$join(EFFECTS11PATH," $$EFFECTS11DIR","$$EFFECTS11DIR")

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../release
  DEFINES	*= DEBUG
}
