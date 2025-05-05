// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SVGICON_H
#define MUMBLE_MUMBLE_SVGICON_H

#include <QIcon>
#include <QString>

class SvgIcon {
public:
	/// addSvgPixmapsToIcon renders the SVG file at |fn| in various
	/// common sizes from 8x8 up to 256x256 and adds the resulting
	/// rasterized pixmaps to |icon|.
	///
	/// This method is only needed due to an odd interaction between
	/// Qt and Plasma/KStatusNotifierItem. See
	/// https://github.com/mumble-voip/mumble/issues/3374
	/// for more information.
	static void addSvgPixmapsToIcon(QIcon &icon, QString fn);
};

#endif
