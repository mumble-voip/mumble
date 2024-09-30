// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLUGINS_LINK_SHAREDMEMORY_H_
#define MUMBLE_PLUGINS_LINK_SHAREDMEMORY_H_

#include <cstddef>

#ifdef _WIN32
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include "windows.h"
#else
#	include <string>
#endif

struct LinkedMem;

class SharedMemory {
public:
	explicit SharedMemory();
	~SharedMemory();

	void close();

	int lastError() const;

	bool mapMemory(const char *name);

	bool isMemoryMapped() const;

	LinkedMem read() const;

	void write(const LinkedMem &source);

	void reset();

private:
	void *m_data;
	int m_error;

#ifdef _WIN32
	HANDLE m_handle;
#else
	std::string m_name;
#endif
};

#endif // MUMBLE_PLUGINS_LINK_SHAREDMEMORY_H_