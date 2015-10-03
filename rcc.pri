# Use a custom tool to query for RCC dependencies.
# Qt's regular invocation of "rcc -list" doesn't work with
# .qrc files that contain references to non-existent files.
# Our .qrc files do that, because we auto-generate some of
# the files at make-time.

# Override RCC_DIR to be 'debug' or 'release'.
# We have to do this because we're loading the
# 'resources' feature a bit earlier than usual.
# By default, Qt would have set these itself.
CONFIG(debug, debug|release) {
	RCC_DIR = debug
}
CONFIG(release, debug|release) {
	RCC_DIR = release
}

load(resources)

win32 {
	rcc.depend_command = python $${PWD}\scripts\rcc-depends.py ${QMAKE_FILE_IN}
} else {
	rcc.depend_command = $${PWD}/scripts/rcc-depends.py ${QMAKE_FILE_IN}
}
