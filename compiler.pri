CONFIG *= warn_on

win32 {
	INCLUDEPATH *= /dev/Boost/include/boost-1_40
	CONFIG(intelcpp) {
		DEFINES *= RESTRICT=restrict
		DEFINES *= VAR_ARRAYS
		QMAKE_CC = icl
		QMAKE_CXX = icl
		QMAKE_LIB = xilib /nologo
		QMAKE_LINK = xilink
		QMAKE_CFLAGS *= -Qstd=c99 -Qrestrict -Qvc9
		QMAKE_CXXFLAGS *= -Qstd=c++0x -Qrestrict -Qvc9
		# Disable IPO and -GL due to bugs in Intel C++ 11.0 and 11.1
		QMAKE_CFLAGS_LTCG =
		QMAKE_LFLAGS_LTCG =
		QMAKE_CFLAGS_RELEASE *= -O3 -Ob0
		QMAKE_CFLAGS_RELEASE -= -GL
		QMAKE_CXXFLAGS_RELEASE *= -O3 -Ob0
		QMAKE_CXXFLAGS_RELEASE -= -GL
		QMAKE_CFLAGS_DEBUG *= -O2 -Ob0
		QMAKE_CXXFLAGS_DEBUG *= -O2 -Ob0

		CONFIG(optgen) {
			QMAKE_CFLAGS *= -Qprof-gen
			QMAKE_CXXFLAGS *= -Qprof-gen
		}

		CONFIG(optimize) {
			QMAKE_CFLAGS *= -Qprof-use
			QMAKE_CXXFLAGS *= -Qprof-use
		}
	} else {
		DEFINES *= RESTRICT=
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS_RELEASE *= -GR -Zi -Oy-
		QMAKE_CXXFLAGS_RELEASE *= -GR -Zi -Oy-
		QMAKE_LFLAGS *= -fixed:no -debug
	}
	CONFIG(debug, debug|release) {
		exists($$(VLD_DIR)) {
			VLD_DIR = $$(VLD_DIR)
		}
		else {
			VLD_DIR = /dev/vld
		}
		INCLUDEPATH *= "$$VLD_DIR/include"
		LIBPATH *= "$$VLD_DIR/lib"
	}
} else {
	DEFINES *= RESTRICT=__restrict__
	CONFIG(opt-gcc) {
		QMAKE_CC = /opt/gcc/bin/gcc
		QMAKE_CXX = /opt/gcc/bin/g++
		QMAKE_LINK = /opt/gcc/bin/g++
	}

	CONFIG(debug, debug|release) {
		QMAKE_CFLAGS *= -fstack-protector -fPIE -pie
		QMAKE_CXXFLAGS *= -fstack-protector -fPIE -pie
	}

	QMAKE_CFLAGS *= -Wfatal-errors -Wshadow -Wconversion -Wsign-compare -fvisibility=hidden
	QMAKE_CXXFLAGS *= -Wfatal-errors -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare -fvisibility=hidden

	DEFINES *= _FORTIFY_SOURCE=2

	!macx {
		QMAKE_LFLAGS *= -Wl,-z,relro -Wl,-z,now
	}

	CONFIG(optgen) {
		QMAKE_CFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_CXXFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_LFLAGS *= -fprofile-generate
	}

	CONFIG(optimize) {
		QMAKE_CFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-use
		QMAKE_CXXFLAGS *= -O3 -march=native -ffast-math -ftree-vectorize -fprofile-use
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS *= -g
		QMAKE_CXXFLAGS *= -g
	}
}

macx {
	CONFIG(debug, debug|release) {
		CONFIG += no-universal
	}

	!CONFIG(no-universal) {
		CONFIG += x86 ppc

		# Precompiled headers are broken when using Makefiles.
		!macx-xcode {
			CONFIG += no-pch
		}
	}

	INCLUDEPATH *= /opt/mumble-1.2/boost-1_40_0/include
	INCLUDEPATH *= /opt/mumble-1.2/include
	LIBPATH *= /opt/mumble-1.2/lib

	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
	XCODE_PATH=$$system(xcode-select -print-path)
	QMAKE_MAC_SDK = $${XCODE_PATH}/SDKs/MacOSX10.5.sdk
	QMAKE_CC = $${XCODE_PATH}/usr/bin/gcc-4.2
	QMAKE_CXX = $${XCODE_PATH}/usr/bin/g++-4.2
	QMAKE_LINK = $${XCODE_PATH}/usr/bin/g++-4.2
	QMAKE_CFLAGS += -mmacosx-version-min=10.5 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
	QMAKE_CXXFLAGS += -mmacosx-version-min=10.5 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}
