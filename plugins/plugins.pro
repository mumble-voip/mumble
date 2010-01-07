TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link manual
DIST = plugins.pri

win32 {
	SUBDIRS += aoc bf1942 bf2 bf2142 bfheroes cod2 cod4 cod5 codmw2 codmw2so css dods dys etqw gmod hl2dm insurgency l4d lotro tf2 ut2004 ut3 wolfet wow
}
