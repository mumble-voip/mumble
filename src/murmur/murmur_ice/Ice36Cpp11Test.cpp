// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This is small test case that can detect whether the
// Ice libraries on the system are built in C++11 mode.

#include <Ice/Ice.h>

static void response__() {}
static void exception__(const Ice::Exception &) {}
static void sent__(bool) {}

int main(int argc, char *argv[]) {
#if ICE_INT_VERSION >= 30600
	new ::IceInternal::Cpp11FnOnewayCallbackNC(response__, exception__, sent__);
#endif
}
