# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

CONFIG(symbols):macx {
	DSYM_DESTDIR = $${DESTDIR}

	contains(TEMPLATE, 'app') {
		DSYM_TARGET_FULLPATH = ${TARGET}
		DSYM_OUT = $${DSYM_DESTDIR}/symbols/${QMAKE_TARGET}.dSYM
		CONFIG(app_bundle) {
			DSYM_OUT = $${DSYM_DESTDIR}/symbols/${QMAKE_TARGET}.app.dSYM
		}
	}

	contains(TEMPLATE, 'lib') {
		DSYM_TARGET_FULLPATH = $${DESTDIR}/${TARGET}
		DSYM_OUT = $${DSYM_DESTDIR}/symbols/${TARGET}.dSYM
		CONFIG(plugin_bundle) {
			DSYM_TARGET_FULLPATH = ${DESTDIR}${TARGET}$${QMAKE_BUNDLE_EXTENSION}/Contents/MacOS/${TARGET}
			DSYM_OUT = $${DSYM_DESTDIR}/symbols/${TARGET}$${QMAKE_BUNDLE_EXTENSION}.dSYM
		}
	}

	dsym.target = $${DSYM_OUT}
	dsym.commands = $(MKDIR) $${DSYM_DESTDIR}/symbols/ && dsymutil $${DSYM_TARGET_FULLPATH} -o $${DSYM_OUT}
	dsym.depends = $${DSYM_TARGET_FULLPATH}
	dsym.CONFIG = recursive
	QMAKE_EXTRA_TARGETS *= dsym
	QMAKE_DISTCLEAN *= $${DSYM_OUT}

	strip.target = stripdebug
	strip.commands = strip -S $${DSYM_TARGET_FULLPATH}
	strip.depends = $${DSYM_OUT}
	strip.CONFIG = recursive
	QMAKE_EXTRA_TARGETS *= strip

	!CONFIG(staticlib) {
		ALL_DEPS += $${DSYM_OUT} stripdebug
	}
}
