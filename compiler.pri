CONFIG *= warn_on

win32 {
	CONFIG(intelcpp) {
		DEFINES *= VAR_ARRAYS
		QMAKE_CC = icl
		QMAKE_CXX = icl
		QMAKE_LIB = xilib /nologo
		QMAKE_LINK = xilink
		QMAKE_CFLAGS *= -Qstd=c99 -Qrestrict -Qvc9
		QMAKE_CXXFLAGS *= -Qstd=c++0x -Qrestrict -Qvc9
		QMAKE_CFLAGS_RELEASE *= -O3 -QxK -Qip -Qipo
		QMAKE_CXXFLAGS_RELEASE *= -O3 -QxK -Qip -Qipo
		QMAKE_CFLAGS_DEBUG *= -O2 -QxK -Ob0 
		#-RTCs -RTCu -RTCc
		QMAKE_CXXFLAGS_DEBUG *= -O2 -QxK -Ob0 
		#-RTCs -RTCu -RTCc

		CONFIG(optgen) {
			QMAKE_CFLAGS *= -Qprof-gen
			QMAKE_CXXFLAGS *= -Qprof-gen
		}

		CONFIG(optimize) {
			QMAKE_CFLAGS *= -Qprof-use
			QMAKE_CXXFLAGS *= -Qprof-use
		}
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS_RELEASE *= -GR -Zi -Oy-
		QMAKE_CXXFLAGS_RELEASE *= -GR -Zi -Oy-
		QMAKE_LFLAGS *= -fixed:no -debug
	}
	
}

!win32 {
	CONFIG(opt-gcc) {
		QMAKE_CC = /opt/gcc/bin/gcc
		QMAKE_CXX = /opt/gcc/bin/g++
		QMAKE_LINK = /opt/gcc/bin/g++
	}

	QMAKE_CFLAGS *= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare -fvisibility=hidden
	QMAKE_CXXFLAGS *= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare -fvisibility=hidden
	!macx {
		QMAKE_LFLAGS *= -Wl,--as-needed
	}

	CONFIG(optgen) {
		QMAKE_CFLAGS *= -O3 -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_CXXFLAGS *= -O3 -ffast-math -ftree-vectorize -fprofile-generate
		QMAKE_LFLAGS *= -fprofile-generate
	}

	CONFIG(optimize) {
		QMAKE_CFLAGS *= -O3 -ffast-math -ftree-vectorize -fprofile-use
		QMAKE_CXXFLAGS *= -O3 -ffast-math -ftree-vectorize -fprofile-use
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS *= -g
		QMAKE_CXXFLAGS *= -g
	}
}

macx {
	ARCH=$$system(uname -m) 
	X86ARCH=$$find(ARCH, i[3456]86) $$find(ARCH, x86_64)

	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
	QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk

	CONFIG(debug, debug|release) {
		CONFIG += no-universal
	}

	CONFIG(no-universal) {
		!isEmpty(X86ARCH) {
			QMAKE_CFLAGS += -mmmx -msse
			QMAKE_CXXFLAGS += -mmmx -msse
		}
	} else {
		CONFIG += x86 ppc

		# Precompiled headers are broken when using Makefiles.
		!macx-xcode {
			CONFIG += no-pch
		}
	}
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}
