CONFIG *= warn_on

win32 {
	INCLUDEPATH *= /dev/WinSDK/include /dev/dxsdk/Include /dev/Boost/include/boost-1_37
	CONFIG(intelcpp) {
		DEFINES *= RESTRICT=restrict
		DEFINES *= VAR_ARRAYS
		QMAKE_CC = icl
		QMAKE_CXX = icl
		QMAKE_LIB = xilib /nologo
		QMAKE_LINK = xilink
		QMAKE_CFLAGS *= -Qstd=c99 -Qrestrict -Qvc9
		QMAKE_CXXFLAGS *= -Qstd=c++0x -Qrestrict -Qvc9
		# Disable IPO and -GL due to bugs in Intel C++ 11.0
		QMAKE_CFLAGS_RELEASE *= -O3 -QxK -Ob0
		QMAKE_CFLAGS_RELEASE -= -GL
		QMAKE_CXXFLAGS_RELEASE *= -O3 -QxK -Ob0
		QMAKE_CXXFLAGS_RELEASE -= -GL
		QMAKE_CFLAGS_DEBUG *= -O2 -QxK -Ob0 
		QMAKE_CXXFLAGS_DEBUG *= -O2 -QxK -Ob0 

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
	
} else {
	DEFINES *= RESTRICT=__restrict__
	CONFIG(opt-gcc) {
		QMAKE_CC = /opt/gcc/bin/gcc
		QMAKE_CXX = /opt/gcc/bin/g++
		QMAKE_LINK = /opt/gcc/bin/g++
	}

	QMAKE_CFLAGS *= -Wshadow -Wconversion -Wsign-compare -fvisibility=hidden
	QMAKE_CXXFLAGS *= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare -fvisibility=hidden
	!macx {
		QMAKE_LFLAGS *= -Wl,--as-needed
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
	INCLUDEPATH *= /opt/mumble/boost

	CONFIG(cocoa) {
		QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
		QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk

		CONFIG += x86_64
		QMAKE_CFLAGS += -mmmx -msse
		QMAKE_CXXFLAGS += -mmmx -msse
	} else {
		QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
		QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk

		CONFIG(debug, debug|release) {
			CONFIG += no-universal
		}

		CONFIG(no-universal) {
			ARCH=$$system(uname -m)
			contains(ARCH, 'i386') {
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
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}
