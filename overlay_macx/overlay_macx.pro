TEMPLATE = subdirs

CONFIG += debug_and_release

!CONFIG(no-input-manager) {
	SUBDIRS = enabler overlay
} else {
	SUBDIRS = overlay
}
