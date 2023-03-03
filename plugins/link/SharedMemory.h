// Copyright 2021-2023 The Mumble Developers. All rights reserved.
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

class SharedMemory {
public:
	explicit SharedMemory();
	~SharedMemory();

	std::size_t size();

	void close();

	int lastError() const;

	bool mapMemory(const char *name, std::size_t size);

	bool isMemoryMapped();

	void read(void *dest, std::size_t size);

	void write(const void *source, std::size_t size);

	void fillWithZero();

private:
	void *m_data;
	std::size_t m_size;
	int m_error;

#ifdef _WIN32
	HANDLE m_handle;
#else
	std::string m_name;
#endif
};

#endif // MUMBLE_PLUGINS_LINK_SHAREDMEMORY_H_