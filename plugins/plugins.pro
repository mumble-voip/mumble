TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link
DIST = plugins.pri

win32 {
	SUBDIRS += aoc bf2 cod2 cod4 cod5 css hl2dm l4d tf2 wolfet wow
}
