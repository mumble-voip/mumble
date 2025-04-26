// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MODULE_H
#define MODULE_H

#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>

typedef uint64_t procptr_t;

struct MemoryRegion {
	procptr_t address;
	size_t size;

	bool readable;
	bool writable;
	bool executable;

	bool operator<(const MemoryRegion &region) const { return address < region.address; }

	MemoryRegion() : address(0), size(0), readable(false), writable(false), executable(false) {}
};

typedef std::set< MemoryRegion > MemoryRegions;

class Module {
protected:
	std::string m_name;
	MemoryRegions m_regions;

public:
	inline std::string name() const { return m_name; }
	inline MemoryRegions regions() const { return m_regions; }

	inline bool addRegion(const MemoryRegion &region) { return m_regions.insert(region).second; }

	procptr_t baseAddress() const;

	Module(const std::string &name);
};

typedef std::unordered_map< std::string, Module > Modules;

#endif // MODULE_H
