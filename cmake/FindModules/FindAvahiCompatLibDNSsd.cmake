# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include("${CMAKE_CURRENT_LIST_DIR}/find_via_pkgconf.cmake")

mumble_find_via_pkgconf("AvahiCompatLibDNSsd" "AvahiCompatLibDNSsd::AvahiCompatLibDNSsd" "avahi-compat-libdns_sd")
