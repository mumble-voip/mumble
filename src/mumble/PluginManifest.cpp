// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PluginManifest.h"

#include <regex>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/SAXException.h>

void PluginManifest::parse(std::istream &input) {
	Poco::XML::InputSource source(input);

	Poco::XML::DOMParser parser;
	Poco::AutoPtr< Poco::XML::Document > doc;
	try {
		doc = parser.parse(&source);
	} catch (const Poco::XML::SAXParseException &) {
		throw PluginManifestException("Plugin manifest uses malformed XML");
	}

	Poco::XML::Element *rootElement = doc->documentElement();

	if (rootElement->nodeName() != "bundle") {
		throw PluginManifestException("Plugin manifest is missing the root \"bundle\" element");
	}
	if (!rootElement->hasAttribute("version")) {
		throw PluginManifestException("Plugin manifest specifies \"bundle\" element without the \"version\" attribute");
	}

	std::string formatVersion = rootElement->getAttribute("version");
	if (formatVersion == "1.0.0") {
		parse_v1_0_0(doc);
	} else {
		throw PluginManifestException(std::string("Plugin manifest uses unknown format version \"") + formatVersion
									  + "\"");
	}
}

bool PluginManifest::specifiesPluginPath(const std::string &os, const std::string &arch) const {
	return m_pluginMap.find({ os, arch }) != m_pluginMap.end();
}

std::string PluginManifest::getPluginPath(const std::string &os, const std::string &arch) const {
	auto it = m_pluginMap.find({ os, arch });

	if (it == m_pluginMap.end()) {
		throw PluginManifestException("PluginManifest: Invalid access to getPluginPath");
	}

	return it->second;
}

const std::unordered_map< PluginRuntimeSpec, std::string > &PluginManifest::getSpecifiedPluginPaths() const {
	return m_pluginMap;
}

void PluginManifest::parse_v1_0_0(Poco::AutoPtr< Poco::XML::Document > document) {
	Poco::XML::Element *assets = document->documentElement()->getChildElement("assets");

	if (!assets) {
		throw PluginManifestException("Plugin manifest does not contain the \"assets\" element");
	}
	Poco::AutoPtr< Poco::XML::NodeList > pluginNodes = assets->getElementsByTagName("plugin");

	for (std::size_t i = 0; i < pluginNodes->length(); ++i) {
		Poco::XML::Element *current =
			dynamic_cast< Poco::XML::Element * >(pluginNodes->item(static_cast< unsigned long >(i)));
		if (!current) {
			throw PluginManifestException("Plugin manifest uses \"plugin\" node of unexpected type");
		}

		std::string binaryPath = current->innerText();
		std::string os         = current->getAttribute("os");
		std::string arch       = current->getAttribute("arch");

		if (binaryPath.empty()) {
			throw PluginManifestException("Plugin manifest declares \"plugin\" element with empty path");
		}
		if (os.empty()) {
			throw PluginManifestException("Plugin manifest declares \"plugin\" element without \"os\" attribute");
		}
		if (arch.empty()) {
			throw PluginManifestException("Plugin manifest declares \"plugin\" element without \"arch\" attribute");
		}

		PluginRuntimeSpec spec = { std::move(os), std::move(arch) };

		m_pluginMap.insert({ std::move(spec), std::move(binaryPath) });
	}

	Poco::XML::Element *nameElement = document->documentElement()->getChildElement("name");
	if (!nameElement) {
		throw PluginManifestException("Plugin manifest does not specify the \"name\" element");
	}

	Poco::XML::Element *versionElement = document->documentElement()->getChildElement("version");
	if (!versionElement) {
		throw PluginManifestException("Plugin manifest does not specify the \"version\" element");
	}

	m_name    = nameElement->innerText();
	m_version = versionElement->innerText();

	if (m_name.empty()) {
		throw PluginManifestException("Plugin manifest specifies empty name");
	}
	if (m_version.empty()) {
		throw PluginManifestException("Plugin manifest specifies empty version");
	}

	std::regex versionRegex("\\d+\\.\\d+\\.\\d+");

	if (!std::regex_match(m_version, versionRegex)) {
		throw PluginManifestException(
			"Plugin manifest specifies version that does not follow the format major.minor.path");
	}
}
