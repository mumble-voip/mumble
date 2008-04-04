TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link

win32 {
	SUBDIRS += bf2 wow
}

