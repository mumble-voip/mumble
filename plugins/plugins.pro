TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link manual
DIST = plugins.pri

win32 {
	SUBDIRS += aoc arma2 bf1942 bf2 bf2142 bfbc2 bfheroes borderlands cod2 cod4 cod5 codmw2 codmw2so cs css dods dys etqw tf2 gmod hl2dm insurgency l4d l4d2 lotro sto ut2004 ut3 wolfet wow
}
