CONFIG *= warn_on

win32 {
	INCLUDEPATH *= /dev/Boost/include/boost-1_42
	LIBPATH	*= /dev/OpenSSL/lib /dev/libsndfile
	INCLUDEPATH *= /dev/OpenSSL/include /dev/libsndfile/include

	CONFIG(intelcpp) {
		DEFINES *= USE_INTEL_IPP
		DEFINES *= RESTRICT=restrict
		DEFINES *= VAR_ARRAYS
		QMAKE_CC = icl
		QMAKE_CXX = icl
		QMAKE_LIB = xilib /nologo
		QMAKE_LINK = xilink
		QMAKE_CFLAGS *= -Qstd=c99 -Qrestrict -Qvc9
		QMAKE_CXXFLAGS *= -Qstd=c++0x -Qrestrict -Qvc9

		QMAKE_CFLAGS_LTCG =
		QMAKE_CXXFLAGS_LTCG =
		QMAKE_LFLAGS_LTCG =

		QMAKE_CFLAGS_RELEASE *= -O3 -Ot -QxSSE2 -Qprec-div-
		QMAKE_CFLAGS_RELEASE -=-arch:SSE
		QMAKE_CFLAGS_RELEASE -= -GL

		QMAKE_CXXFLAGS_RELEASE *= -O3 -Ot -QxSSE2 -Qprec-div-
		QMAKE_CXXFLAGS_RELEASE -=-arch:SSE
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
		QMAKE_CFLAGS_RELEASE *= -Ox -Ot /fp:fast /Qfast_transcendentals -Ob2
		QMAKE_CXXFLAGS_RELEASE *= -Ox -Ot /fp:fast /Qfast_transcendentals -Ob2
		CONFIG(analyze) {
			QMAKE_CFLAGS_DEBUG *= /analyze
			QMAKE_CXXFLAGS_DEBUG *= /analyze
		}
		DEFINES *= RESTRICT=
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS_RELEASE *= -GR -Zi -Oy-
		QMAKE_CXXFLAGS_RELEASE *= -GR -Zi -Oy-

		QMAKE_CFLAGS_RELEASE -= -Oy
		QMAKE_CXXFLAGS_RELEASE -= -Oy

		QMAKE_LFLAGS *= /debug
		QMAKE_LFLAGS *= /OPT:REF /OPT:ICF
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
}

unix {
	DEFINES *= RESTRICT=__restrict__
	QMAKE_CFLAGS *= -Wfatal-errors -Wshadow -Wconversion -Wsign-compare -fvisibility=hidden
	QMAKE_CXXFLAGS *= -Wfatal-errors -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare -fvisibility=hidden

	CONFIG(opt-gcc) {
		QMAKE_CC = /opt/gcc/bin/gcc
		QMAKE_CXX = /opt/gcc/bin/g++
		QMAKE_LINK = /opt/gcc/bin/g++
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
}

unix:!macx {
	CONFIG(debug, debug|release) {
		QMAKE_CFLAGS *= -fstack-protector -fPIE -pie
		QMAKE_CXXFLAGS *= -fstack-protector -fPIE -pie
		QMAKE_LFLAGS = -Wl,--no-add-needed
	}

	DEFINES *= _FORTIFY_SOURCE=2
	QMAKE_LFLAGS *= -Wl,-z,relro -Wl,-z,now

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
	}

	INCLUDEPATH *= /opt/mumble-1.2/boost-1_41_0/include
	INCLUDEPATH *= /opt/mumble-1.2/include
	LIBPATH *= /opt/mumble-1.2/lib

	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
	XCODE_PATH=$$system(xcode-select -print-path)
	QMAKE_MAC_SDK = $${XCODE_PATH}/SDKs/MacOSX10.5.sdk

	QMAKE_CC = $${XCODE_PATH}/usr/bin/gcc-4.2
	QMAKE_CXX = $${XCODE_PATH}/usr/bin/g++-4.2
	QMAKE_LINK = $${XCODE_PATH}/usr/bin/g++-4.2

	QMAKE_CFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
	QMAKE_CXXFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
	QMAKE_LFLAGS += -Wl,-dead_strip -framework Cocoa -framework Carbon

	CONFIG(no-cocoa) {
		# The no-cocoa option basically means that we should build without depending on
		# a Cocoa build of Qt. However, for the most part, the reason we do this is to
		# run on older versions of Mac OS X. We 'abuse' that to disable features we don't
		# want enabled on Mac OS X 10.4.
		QMAKE_CFLAGS -= -fno-stack-protector -fno-stack-protector-all
		QMAKE_CXXFLAGS -= -fno-stack-protector -fno-stack-protector-all
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS *= -gfull -gdwarf-2
		QMAKE_CXXFLAGS *= -gfull -gdwarf-2
	}
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}
