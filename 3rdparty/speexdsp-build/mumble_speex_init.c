// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_INTEL_IPP
#include <ipp.h>
#endif


EXPORT void mumble_speex_init()
{
#ifdef USE_INTEL_IPP
	ippStaticInit();
#endif
}
