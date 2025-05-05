// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef PROCESSLINUX_H_
#define PROCESSLINUX_H_

#include "ProcessBase.h"

/// Meant to be used with Linux processes.
class ProcessLinux : public ProcessBase {
public:
	procptr_t exportedSymbol(const std::string &symbol, const procptr_t module) const override;

	ProcessLinux(const procid_t id, const std::string &name);
	virtual ~ProcessLinux();
};

#endif
