// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SSLLOCKS_H_
#define MUMBLE_SSLLOCKS_H_

class SSLLocks {
public:
	static void initialize();
	static void destroy();
};

#endif
