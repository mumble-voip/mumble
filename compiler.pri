include(qt.pri)
 
CONFIG *= warn_on

win32 {
	# Import dependency paths for windows
	include(winpaths_default.pri)

	INCLUDEPATH *= "$$BOOST_PATH/include" "$$BOOST_PATH/include/boost-1_49/"
	QMAKE_LIBDIR *= "$$OPENSSL_PATH/lib" "$$LIBSNDFILE_PATH/lib" "$$BOOST_PATH/lib"
	INCLUDEPATH *= "$$OPENSSL_PATH/include" "$$LIBSNDFILE_PATH/include"
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
		QMAKE_LFLAGS_RELEASE *= /SafeSEH /NXCOMPAT /DYNAMICBASE
		CONFIG(analyze) {
			QMAKE_CFLAGS_DEBUG *= /analyze
			QMAKE_CXXFLAGS_DEBUG *= /analyze
			QMAKE_CFLAGS_RELEASE *= /analyze
			QMAKE_CXXFLAGS_RELEASE *= /analyze
		}
		DEFINES *= RESTRICT=
		CONFIG(sse2) {
		      QMAKE_CFLAGS_RELEASE -= -arch:SSE
		      QMAKE_CFLAGS_DEBUG -= -arch:SSE
		      QMAKE_CFLAGS += -arch:SSE2
		}
	}

	CONFIG(symbols) {
		QMAKE_CFLAGS_RELEASE *= -GR -Zi -Oy-
		QMAKE_CXXFLAGS_RELEASE *= -GR -Zi -Oy-

		QMAKE_CFLAGS_RELEASE -= -Oy
		QMAKE_CXXFLAGS_RELEASE -= -Oy

		QMAKE_LFLAGS *= /debug
		QMAKE_LFLAGS *= /OPT:REF /OPT:ICF
	}

	CONFIG(vld) {
		CONFIG(debug, debug|release) {
			DEFINES *= USE_VLD
			INCLUDEPATH *= "$$VLD_PATH/include"
			QMAKE_LIBDIR *= "$$VLD_PATH/lib"
		}
	}
}

unix {
	DEFINES *= RESTRICT=__restrict__
	QMAKE_CFLAGS *= -Wfatal-errors -fvisibility=hidden
	QMAKE_CXXFLAGS *= -Wfatal-errors -fvisibility=hidden
	!CONFIG(quiet-build-log) {
		QMAKE_CFLAGS *= -Wshadow -Wconversion -Wsign-compare
		QMAKE_CXXFLAGS *= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
	}

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
	SYSTEM_INCLUDES = $$(MUMBLE_PREFIX)/include $$(MUMBLE_PREFIX)/include/boost_1_54_0 $$[QT_INSTALL_HEADERS]
	QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/lib

	for(inc, $$list($$SYSTEM_INCLUDES)) {
		QMAKE_CFLAGS += -isystem $$inc
		QMAKE_CXXFLAGS += -isystem $$inc
		QMAKE_OBJECTIVE_CFLAGS += -isystem $$inc
		QMAKE_OBJECTIVE_CXXFLAGS += -isystem $$inc
	}

	!CONFIG(universal) {
		CONFIG += no-pch

		# Qt 5.1 and greater want short-form OS X SDKs.
		isEqual(QT_MAJOR_VERSION, 5) {
			QMAKE_MAC_SDK = macosx
		} else {
			QMAKE_MAC_SDK = $$system(xcrun --sdk macosx --show-sdk-path)
		}

		QMAKE_CC = $$system(xcrun -find clang)
		QMAKE_CXX = $$system(xcrun -find clang++)
		QMAKE_LINK = $$system(xcrun -find clang++)
		QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
		QMAKE_CFLAGS += -mmacosx-version-min=10.6
		QMAKE_CXXFLAGS += -mmacosx-version-min=10.6
		QMAKE_OBJECTIVE_CFLAGS += -mmacosx-version-min=10.6
		QMAKE_OBJECTIVE_CXXFLAGS += -mmacosx-version-min=10.6
	} else {
		XCODE_PATH=$$system(xcode-select -print-path)
		CONFIG += x86 ppc no-cocoa
		QMAKE_MAC_SDK = $${XCODE_PATH}/SDKs/MacOSX10.5.sdk
		QMAKE_CC = $${XCODE_PATH}/usr/bin/gcc-4.2
		QMAKE_CXX = $${XCODE_PATH}/usr/bin/g++-4.2
		QMAKE_LINK = $${XCODE_PATH}/usr/bin/g++-4.2
		QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
		QMAKE_CFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		QMAKE_CXXFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		QMAKE_OBJECTIVE_CFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		QMAKE_OBJECTIVE_CXXFLAGS += -mmacosx-version-min=10.4 -Xarch_i386 -mmmx -Xarch_i386 -msse -Xarch_i386 -msse2
		DEFINES += USE_MAC_UNIVERSAL
	}

	QMAKE_LFLAGS += -Wl,-dead_strip -framework Cocoa -framework Carbon

	CONFIG(symbols) {
		QMAKE_CFLAGS *= -gfull -gdwarf-2
		QMAKE_CXXFLAGS *= -gfull -gdwarf-2
	}
}

CONFIG(clang-analyzer) {
	QMAKE_CC = $$(CC)
	QMAKE_CXX = $$(CXX)
}

CONFIG(no-pch) {
	CONFIG -= precompile_header
} else {
 	CONFIG *= precompile_header
}

CONFIG(debug, debug|release) {
	DEFINES *= SNAPSHOT_BUILD
}
