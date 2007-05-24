TEMPLATE = subdirs

SUBDIRS += speexbuild
SUBDIRS += src/mumble src/murmur

win32 {
  SUBDIRS += plugins
}
