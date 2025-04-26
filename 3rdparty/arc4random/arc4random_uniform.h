// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_3RDPARTY_ARC4RANDOM_UNIFORM_H_
#define MUMBLE_3RDPARTY_ARC4RANDOM_UNIFORM_H_

#include <stdint.h>

uint32_t mumble_arc4random(void);
uint32_t mumble_arc4random_uniform(uint32_t upper_bound);

#endif
