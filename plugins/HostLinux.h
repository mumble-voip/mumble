// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef HOSTLINUX_H_
#define HOSTLINUX_H_

#include "Module.h"

using procid_t = uint64_t;

class HostLinux {
protected:
	procid_t m_pid;

public:
	bool peek(const procptr_t address, void *dst, const size_t size) const;
	Modules modules() const;

	static bool isWine(const procid_t id);

	HostLinux(const procid_t pid);
	virtual ~HostLinux();
};

#endif
