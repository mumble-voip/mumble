TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link
DIST = plugins.pri

win32 {
	SUBDIRS += aoc bf2 cod2 cod4 cod5 css gmod hl2dm insurgency l4d tf2 wolfet wow
}
