TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link
DIST = mumble_plugin.h

win32 {
	SUBDIRS += aoc bf2 cod2 cod4 cod5 css gmod hl2dm insurgency l4d tf2 wolfet wow
}
