TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link manual
DIST = plugins.pri

win32 {
	SUBDIRS += aoc arma2 bf1942 bf2 bf3 bf2142 bfbc2 bfheroes blacklight borderlands breach cod2 cod4 cod5 codmw2 codmw2so cs css dods dys etqw tf2 gmod gtaiv gw hl2dm insurgency jc2 l4d l4d2 lol lotro sto ut2004 ut3 ut99 wolfet wow
}
