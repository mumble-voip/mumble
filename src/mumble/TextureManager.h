// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <qbytearray.h>

class TextureManager {
public:
	QByteArray convertTexture(QByteArray texture, QByteArray &textureFormat);
};

#endif // TEXTUREMANAGER_H
