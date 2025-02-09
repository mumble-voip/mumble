#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from typing import Optional

import argparse
import re
import os

varNameIdx = 0
filePathIdx = 1
displayNameIdx = 2
urlIdx = 3
defineGuardIdx = 4

repoRoot = os.path.normpath(os.path.join(os.path.dirname(__file__), ".."))

# List of (3rdparty) licenses  [<variableName>, <pathToLicenseFile>, <DisplayName>, <URL>, (<define-guard>)?]
licenses = [
    ["licenseMumble", "LICENSE", "Mumble", "https://www.mumble.info/"],
    ["licenseSPEEXDSP", "3rdparty/speexdsp/COPYING", "SpeexDSP", "http://www.speex.org/"],
    ["licenseOpenSSL", "3rdPartyLicenses/openssl_license.txt", "OpenSSL", "http://www.openssl.org/"],
    ["licenseLibsndfile", "3rdPartyLicenses/libsndfile_license.txt", "libsndfile", "http://www.mega-nerd.com/libsndfile/"],
    ["licenseOgg", "3rdPartyLicenses/libogg_license.txt", "libogg", "http://www.xiph.org/"],
    ["licenseVorbis", "3rdPartyLicenses/libvorbis_license.txt", "libvorbis", "http://www.xiph.org/"],
    ["licenseFLAC", "3rdPartyLicenses/libflac_license.txt", "libFLAC", "http://flac.sourceforge.net/"],
    ["licenseMachOverride", "3rdPartyLicenses/mach_override_license.txt", "mach_override", "https://github.com/rentzsch/mach_star", "Q_OS_MAC"],
    ["licenseMinHook", "3rdparty/minhook/LICENSE.txt", "MinHook", "https://github.com/TsudaKageyu/minhook", "Q_OS_WIN64"],
    ["licenseQtTranslations", "src/mumble/qttranslations/LICENSE",
        "Additional Qt translations", "https://www.virtualbox.org/ticket/2018", "USING_BUNDLED_QT_TRANSLATIONS"],
    ["licenseFlagIcons", "3rdparty/flag-icons/LICENSE", "flag-icons", "https://flagicons.lipis.dev/" ],
    ["licenseXInputCheck", "3rdparty/xinputcheck-src/COPYING.txt", "XInputCheck (SDL_IsXInput function)", "https://www.libsdl.org/"],
    ["licenseQQBonjour", "3rdparty/qqbonjour/LICENSE", "QQBonjour", "https://doc.qt.io/archives/qq/qq23-bonjour.html"],
    ["licenseSmallFT", "3rdparty/smallft/LICENSE", "smallft", "https://www.xiph.org"],
    ["licenseOldStyleLicenseHeaders", "3rdPartyLicenses/mumble-old-license-headers/LICENSE.txt", "Old-style Mumble license headers", "https://www.mumble.info"],
    ["licenseArc4RandomUniform", "3rdparty/arc4random/LICENSE", "arc4random_uniform", "https://www.openbsd.org"],
    ["licenseMariaDBCConnector", "3rdPartyLicenses/mariadb_c_connector_license.txt", "MariaDB Connector/C", "https://www.mariadb.org", "USE_BUILDENV"],
    ["licenseZlib", "3rdPartyLicenses/zlib_license.txt", "ZLIB", "https://www.zlib.net", "USE_BUILDENV"],
    ["licenseProtobuf", "3rdPartyLicenses/protobuf_license.txt", "Protobuf", "https://developers.google.com/protocol-buffers", "USE_BUILDENV"],
    ["licenseExpat", "3rdPartyLicenses/expat_license.txt", "Expat", "http://libexpat.org", "USE_BUILDENV"],
    ["licenseBoost", "3rdPartyLicenses/boost_license.txt", "Boost C++ Libraries", "https://www.boost.org/", "USE_BUILDENV"],
    ["licenseBzip2", "3rdPartyLicenses/bzip2_license.txt", "bzip2", "http://www.bzip.org", "USE_BUILDENV"],
    ["licenseBerkeleyDB", "3rdPartyLicenses/berkeleydb_license.txt", "BerkeleyDB", "https://www.oracle.com/database/berkeley-db", "USE_BUILDENV"],
    ["licenseFreetype", "3rdPartyLicenses/freetype_license.txt", "Freetype", "https://www.freetype.org/", "USE_BUILDENV"],
    ["licenseHarfBuzzNG", "3rdPartyLicenses/harfbuzzng_license.txt", "HarfBuzz", "https://www.freedesktop.org/wiki/Software/HarfBuzz", "USE_BUILDENV"],
    ["licenseLibPNG", "3rdPartyLicenses/libpng_license.txt", "LibPNG", "http://www.libpng.org/pub/png/libpng.html", "USE_BUILDENV"],
    ["licenseLibjpegTurbo", "3rdPartyLicenses/libjpeg_turbo_license.txt", "libjpeg-turbo", "http://www.libjpeg-turbo.org", "USE_BUILDENV"],
    ["licenseZeroCIce", "3rdPartyLicenses/zeroc_ice_license.txt", "ZeroC Ice", "https://zeroc.com/", "USE_BUILDENV"],
    ["licenseMcpp", "3rdPartyLicenses/mcpp_license.txt", "mcpp", "http://mcpp.sourceforge.net/", "USE_BUILDENV"],
    ["licenseMDNSResponder", "3rdPartyLicenses/mdnsresponder_license.txt", "mDNSResponder", "https://opensource.apple.com/tarballs/mDNSResponder/", "USE_BUILDENV"],
    ["licensePCRE", "3rdPartyLicenses/pcre_license.txt", "PCRE", "http://www.pcre.org/", "USE_BUILDENV"],
    ["licenseQt", "3rdPartyLicenses/qt_license.txt", "Qt", "https://www.qt.io/", "USE_BUILDENV"],
    ["licenseSQLite3", "3rdPartyLicenses/sqlite3_license.txt", "SQLite3", "http://sqlite.org", "USE_BUILDENV"],
    ["licenseXar", "3rdPartyLicenses/xar_license.txt", "XAR", "https://opensource.apple.com/source/xar/", "USE_BUILDENV"], # macOS only
    ["licenseAvahi", "3rdPartyLicenses/avahi_license.txt", "Avahi", "https://www.avahi.org/", "USE_BUILDENV"], # Linux only
    ["licenseAppImageRuntime", "3rdPartyLicenses/appimage_runtime_license.txt", "AppImage Runtime", "http://www.appimage.org", "USE_BUILDENV"], # Linux only
    ["licenseSquashfuse", "3rdPartyLicenses/squashfuse_license.txt", "squashfuse", "https://github.com/vasi/squashfuse", "USE_BUILDENV"], # Linux only
    ["licenseLibfuse", "3rdPartyLicenses/libfuse_license.txt", "libfuse", "https://github.com/libfuse/libfuse/", "USE_BUILDENV"], # Linux only
    ["licenseGlib", "3rdPartyLicenses/glib_license.txt", "glib", "https://developer.gnome.org/glib/", "USE_BUILDENV"], # Linux only
    ["licenseLibcap2", "3rdPartyLicenses/libcap2_license.txt", "libcap2", "https://sites.google.com/site/fullycapable/", "USE_BUILDENV"], # Linux only
    ["licenseLibdaemon", "3rdPartyLicenses/libdaemon_license.txt", "libdaemon", "http://0pointer.de/lennart/projects/libdaemon/", "USE_BUILDENV"], # Linux only
    ["licenseLibffi", "3rdPartyLicenses/libffi_license.txt", "libffi", "https://sourceware.org/libffi/", "USE_BUILDENV"] # Linux only
]


def formatLicenseVar(var_name: str, license_path: str, define_guard: Optional[str]) -> str:
    with open(os.path.join(repoRoot, license_path), "r") as file:
        content = file.read().strip().replace("\"", "\\\"")

        content = "\"" + re.sub("\\n\\s*", "\\\\n\"\n\t\"", content) + "\""

        content = "static const char *" + var_name + " = " + content + ";"

        if define_guard:
            content = "#ifdef " + define_guard + "\n" + content + "\n" + "#endif // " + define_guard + "\n"

        return content


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", help="Path to the file to which the generated code shall be written." +
                        " If none is given, the content is written to stdout", metavar="PATH")

    args = parser.parse_args()

    content = """/*
 * This file was auto-generated by scripts/mklic.pl\n
 * DO NOT EDIT IT MANUALLY
 */
#ifndef MUMBLE_LICENSES_H_
#define MUMBLE_LICENSES_H_

struct ThirdPartyLicense {
	const char* name;
	const char* url;
	const char* license;

	ThirdPartyLicense() : name(nullptr), url(nullptr), license(nullptr) {}
	ThirdPartyLicense(const char* name_, const char* url_, const char* license_)
	    : name(name_), url(url_), license(license_) {}

	bool isEmpty() const { return !name && !url && !license; }
};
"""
    
    for currentLicense in licenses:
        content += "\n" + formatLicenseVar(var_name=currentLicense[varNameIdx], license_path=currentLicense[filePathIdx],
                define_guard=currentLicense[defineGuardIdx] if len(currentLicense) > defineGuardIdx else None)


    content += "\n\n"
    content += "static const ThirdPartyLicense licenses3rdParties[] = {\n"

    for currentLicense in licenses[ 1 : ]:
        if len(currentLicense) > defineGuardIdx:
            content += "#ifdef " + currentLicense[defineGuardIdx] + "\n"

        content += "\t" + ("ThirdPartyLicense(\"%s\", \"%s\", %s)" % (currentLicense[displayNameIdx], currentLicense[urlIdx],
            currentLicense[varNameIdx])) + ",\n"

        if len(currentLicense) > defineGuardIdx:
            content += "#endif //" + currentLicense[defineGuardIdx] + "\n"

    content += "\t// Empty entry that marks the end of the array\n"
    content += "\tThirdPartyLicense()\n"
    content += "};\n"
    content += "#endif // MUMBLE_LICENSES_H_\n"

    if args.output:
        with open(args.output, "w") as outFile:
            outFile.write(content)
    else:
        print(content)


if __name__ == "__main__":
    main()
