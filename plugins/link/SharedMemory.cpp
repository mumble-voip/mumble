// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SharedMemory.h"

#include "LinkedMem.h"

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

static constexpr std::size_t chunkSize  = sizeof(Chunk);
static constexpr std::size_t chunkCount = sizeof(LinkedMem) / chunkSize;

static_assert(sizeof(std::atomic< Chunk >) == chunkSize, "std::atomic< Chunk > has a size different from Chunk's");
static_assert(chunkSize * chunkCount == sizeof(LinkedMem), "LinkedMem's size isn't a multiple of Chunk's size");

SharedMemory::SharedMemory()
	: m_data(nullptr), m_error(0),
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
		munmap(m_data, sizeof(LinkedMem));
	}
	if (!m_name.empty()) {
		shm_unlink(m_name.c_str());
	}

	m_name.clear();
#endif

	m_data  = nullptr;
	m_error = 0;
}

int SharedMemory::lastError() const {
	return m_error;
}

bool SharedMemory::mapMemory(const char *name) {
	close();

	bool created = false;

#ifdef _WIN32
	m_handle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, name);

	if (m_handle == NULL) {
		// Attaching failed, so we have to create it
		m_handle = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(LinkedMem), name);

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

		// Truncate to correct size
		if (ftruncate(fd, sizeof(LinkedMem)) != 0) {
			m_error = errno;

			::close(fd);

			return false;
		}

		created = true;
	}

	m_data = mmap(nullptr, sizeof(LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

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

	if (created) {
		reset();
	}

	return true;
}

bool SharedMemory::isMemoryMapped() const {
	return m_data != nullptr;
}

LinkedMem SharedMemory::read() const {
	const auto *data = static_cast< const std::atomic< Chunk > * >(m_data);

	Chunk buff[chunkCount];

	for (std::size_t i = 0; i < chunkCount; i++) {
		buff[i] = data[i].load(std::memory_order_relaxed);
	}

	// bitcast the chunks with memcpy to avoid running afoul
	// of type-based alias analysis
	LinkedMem dest;
	std::memcpy(&dest, buff, sizeof(LinkedMem));
	return dest;
}

void SharedMemory::write(const LinkedMem &source) {
	auto *data = static_cast< std::atomic< Chunk > * >(m_data);

	Chunk buff[chunkCount];

	std::memcpy(buff, &source, sizeof(LinkedMem));

	for (std::size_t i = 0; i < chunkCount; i++) {
		data[i].store(buff[i], std::memory_order_relaxed);
	}
}

void SharedMemory::reset() {
	write(LinkedMem());
}
