// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SharedMemory.h"

#ifdef _WIN32
#else
// Note: Linking to the "rt" library is required
#	include <fcntl.h>
#	include <sys/mman.h>
#	include <sys/stat.h>
#	include <unistd.h>
#	include <errno.h>
#endif

#include <cstring>

#include <iostream>

SharedMemory::SharedMemory()
	: m_data(nullptr), m_size(0), m_error(0),
#ifdef _WIN32
	  m_handle(NULL)
#else
	  m_name()
#endif
{
}

SharedMemory::~SharedMemory() {
	close();
}

std::size_t SharedMemory::size() {
	return m_size;
}

void SharedMemory::close() {
#ifdef _WIN32
	if (m_data) {
		UnmapViewOfFile(m_data);
	}
	if (m_handle != NULL) {
		CloseHandle(m_handle);
	}

	m_handle = NULL;
#else
	if (m_data) {
		munmap(m_data, m_size);
	}
	if (!m_name.empty()) {
		shm_unlink(m_name.c_str());
	}

	m_name.clear();
#endif

	m_data  = nullptr;
	m_size  = 0;
	m_error = 0;
}

int SharedMemory::lastError() const {
	return m_error;
}

void *SharedMemory::mapMemory(const char *name, std::size_t size) {
	close();

	bool created = false;

#ifdef _WIN32
	m_handle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, name);

	if (m_handle == NULL) {
		// Attaching failed, so we have to create it
		m_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, size, name);

		if (m_handle == NULL) {
			m_error = GetLastError();

			return nullptr;
		}

		created = true;
	}

	m_data = MapViewOfFile(m_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (m_data == NULL) {
		m_error = GetLastError();

		CloseHandle(m_handle);

		return nullptr;
	}
#else
	m_name.clear();

	// Create or attach to a shared memory region with the given name and make sure the user can read and write it
	int fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);

	if (fd < 0) {
		std::cout << "Attaching failed" << std::endl;

		// Attaching failed, so it seems we have to create it
		fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		if (fd == -1) {
			m_error = errno;
			return nullptr;
		}

		// Truncate to specified size
		if (ftruncate(fd, size) != 0) {
			m_error = errno;

			::close(fd);

			return nullptr;
		}

		created = true;
	}

	m_data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (m_data == reinterpret_cast< void * >(-1)) {
		m_data = nullptr;

		m_error = errno;

		::close(fd);

		return nullptr;
	}

	// Quoting from the mmap manpage:
	// After the mmap() call has returned, the file descriptor, fd, can
	// be closed immediately without invalidating the mapping.
	::close(fd);

	m_name.assign(name);
#endif

	m_size = size;

	if (created) {
		std::memset(m_data, 0, m_size);
	}

	return m_data;
}
