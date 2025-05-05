// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Module.h"

Module::Module(const std::string &name) : m_name(name) {
}

procptr_t Module::baseAddress() const {
	const auto iter = m_regions.cbegin();
	if (iter != m_regions.cend()) {
		return iter->address;
	}

	return 0;
}
