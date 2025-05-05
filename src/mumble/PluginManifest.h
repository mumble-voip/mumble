// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGINMANIFEST_H_
#define MUMBLE_MUMBLE_PLUGINMANIFEST_H_

#include <istream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <Poco/DOM/AutoPtr.h>

namespace Poco {
namespace XML {
	class Document;
} // namespace XML
} // namespace Poco

struct PluginManifestException : std::runtime_error {
	PluginManifestException(const std::string &msg = "") : std::runtime_error(msg) {}
};

struct PluginRuntimeSpec {
	std::string os;
	std::string architecture;

	bool operator==(const PluginRuntimeSpec &other) const {
		return os == other.os && architecture == other.architecture;
	}
};

// Make PluginRuntimeSpec hashable
namespace std {
template<> struct hash< PluginRuntimeSpec > {
	std::size_t operator()(const PluginRuntimeSpec &spec) const {
		return std::hash< std::string >()(spec.os) ^ (std::hash< std::string >()(spec.architecture) << 1);
	}
};
} // namespace std

class PluginManifest {
public:
	PluginManifest() = default;

	void parse(std::istream &input);

	bool specifiesPluginPath(const std::string &os, const std::string &arch) const;
	std::string getPluginPath(const std::string &os, const std::string &arch) const;

	const std::unordered_map< PluginRuntimeSpec, std::string > &getSpecifiedPluginPaths() const;

	const std::string &getName() const;

	const std::string &getVersion() const;

protected:
	std::string m_version;
	std::string m_name;
	std::unordered_map< PluginRuntimeSpec, std::string > m_pluginMap;

	void parse_v1_0_0(Poco::AutoPtr< Poco::XML::Document > document);
};

#endif // MUMBLE_MUMBLE_PLUGINMANIFEST_H_
