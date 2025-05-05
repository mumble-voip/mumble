// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef PROCESSWINDOWS_H_
#define PROCESSWINDOWS_H_

#include "ProcessBase.h"

/// Meant to be used with Windows processes.
class ProcessWindows : public ProcessBase {
public:
	procptr_t exportedSymbol(const std::string &symbol, const procptr_t module) const override;

	ProcessWindows(const procid_t id, const std::string &name);
	virtual ~ProcessWindows();
};

#endif
