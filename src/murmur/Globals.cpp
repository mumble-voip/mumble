// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * This file serves as a place to initialize global variables that are declared as extern in other places in the code
 */

class QFile;
class Meta;


QFile *qfLog = nullptr;
Meta *meta   = nullptr;
