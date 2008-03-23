CONFIG *= warn_on

win32 {
	CONFIG(intelcpp) {
		DEFINES *= VAR_ARRAYS
		QMAKE_CC = icl
		QMAKE_CXX = icl
		QMAKE_LIB = xilib /nologo
		QMAKE_LINK = xilink
		QMAKE_CFLAGS *= -Qstd=c99 -Qrestrict
		QMAKE_CXXFLAGS *= -Qstd=c++0x -Qrestrict
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
	QMAKE_CFLAGS *= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
	QMAKE_CXXFLAGS *= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
	!macx {
		QMAKE_LFLAGS *= -Wl,--as-needed
	}

	CONFIG(optgen) {
		QMAKE_CFLAGS *= -O3 -ffast-math -ftree-vectorize -fprofile-generate
	}

	CONFIG(optimize) {
		QMAKE_CFLAGS *= -O3 -ffast-math -ftree-vectorize -fprofile-use
	}
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}
