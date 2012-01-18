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

SOURCES *= "$$EFFECTS11DIR/EffectAPI.cpp"
SOURCES *= "$$EFFECTS11DIR/EffectLoad.cpp"
SOURCES *= "$$EFFECTS11DIR/EffectNonRuntime.cpp"
SOURCES *= "$$EFFECTS11DIR/EffectReflection.cpp"
SOURCES *= "$$EFFECTS11DIR/EffectRuntime.cpp"
SOURCES *= "$$EFFECTS11DIR/d3dx11dbg.cpp"
SOURCES *= "$$EFFECTS11DIR/d3dx11dbg.cpp"
SOURCES *= "$$EFFECTS11DIR/d3dxGlobal.cpp"
INCLUDEPATH *= "$$EFFECTS11DIR/."
INCLUDEPATH *= "$$EFFECTS11DIR/Inc"
INCLUDEPATH *= "$$EFFECTS11DIR/Binary"

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../release
  DEFINES	*= DEBUG
}
