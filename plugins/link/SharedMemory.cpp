// Copyright 2021-2023 The Mumble Developers. All rights reserved.
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

#include <atomic>
#include <climits>
#include <cstdint>
#include <cstring>

#include <iostream>

#include <vector>

// A Chunk is one atomically accessed chunk of shared memory.
//
// The only way to check at compile time if operations on a
// given atomic type are always lock-free before C++17 is
// using the C macros ATOMIC_*_LOCK_FREE, which aren't defined
// for fixed-width integer types. But we want the width of our
// atomic operations to be fixed, both to make the protocol
// easier to implement in other programming languages and to
// prevent interoperability problems when a game is built
// for a different platform than Mumble, e.g. if it's running
// in box86 or WoW64.
//
// int32_t is virtually always the same as int, so we check to
// make sure that's the case and that ATOMIC_INT_LOCK_FREE
// is 2, which means std::atomic< int > is *always* lock-free.
static_assert(std::is_same< int, std::int32_t >::value, "int isn't the same as std::int32_t");
static_assert(ATOMIC_INT_LOCK_FREE == 2, "std::atomic< int > may not be lock-free");
using Chunk = int;

static constexpr std::size_t chunkSize = sizeof(Chunk);
static_assert(sizeof(std::atomic< Chunk >) == chunkSize, "std::atomic< Chunk > has a size different from Chunk's");

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

bool SharedMemory::mapMemory(const char *name, std::size_t size) {
	// Round size up to the nearest multiple of the chunk size
	size = (size + chunkSize - 1) / chunkSize * chunkSize;

	close();

	bool created = false;

#ifdef _WIN32
	m_handle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, name);

	if (m_handle == NULL) {
		// Attaching failed, so we have to create it
		m_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, size, name);

		if (m_handle == NULL) {
			m_error = GetLastError();

			return false;
		}

		created = true;
	}

	m_data = MapViewOfFile(m_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (m_data == NULL) {
		m_error = GetLastError();

		CloseHandle(m_handle);

		return false;
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
			return false;
		}

		// Truncate to specified size
		if (ftruncate(fd, size) != 0) {
			m_error = errno;

			::close(fd);

			return false;
		}

		created = true;
	}

	m_data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (m_data == reinterpret_cast< void * >(-1)) {
		m_data = nullptr;

		m_error = errno;

		::close(fd);

		return false;
	}

	// Quoting from the mmap manpage:
	// After the mmap() call has returned, the file descriptor, fd, can
	// be closed immediately without invalidating the mapping.
	::close(fd);

	m_name.assign(name);
#endif

	m_size = size;

	if (created) {
		fillWithZero();
	}

	return true;
}

bool SharedMemory::isMemoryMapped() {
	return m_data != nullptr;
}

void SharedMemory::read(void *dest, std::size_t size) {
	const auto *data = static_cast< const std::atomic< Chunk > * >(m_data);

	std::vector< Chunk > buff(m_size / chunkSize, 0);

	for (std::size_t i = 0; i < m_size / chunkSize; i++) {
		buff[i] = data[i].load(std::memory_order_relaxed);
	}

	// bitcast the chunks with memcpy to avoid running afoul
	// of type-based alias analysis
	std::memcpy(dest, &buff[0], std::min(size, m_size));
}

void SharedMemory::write(const void *source, std::size_t size) {
	auto *data = static_cast< std::atomic< Chunk > * >(m_data);

	std::vector< Chunk > buff(m_size / chunkSize, 0);

	std::memcpy(&buff[0], source, std::min(size, m_size));

	for (std::size_t i = 0; i < m_size / chunkSize; i++) {
		data[i].store(buff[i], std::memory_order_relaxed);
	}
}

void SharedMemory::fillWithZero() {
	auto *data = static_cast< std::atomic< Chunk > * >(m_data);

	for (std::size_t i = 0; i < m_size / chunkSize; i++) {
		data[i].store(0, std::memory_order_relaxed);
	}
}
