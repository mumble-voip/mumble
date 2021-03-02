#! /bin/bash
#
# Copyright 2009-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

cp murmur.ini murmur.ini.win32
cp murmur.ini murmur.ini.osx
cp murmur.ini murmur.ini.system

perl -pi.bak -e 's/^(#|;|)dbus=.*$/\1dbus=system/' murmur.ini.system
perl -pi.bak -e 's/^(#|;|)logfile=.*$/logfile=\/var\/log\/mumble-server\/mumble-server.log/' murmur.ini.system
perl -pi.bak -e 's/^(#|;|)pidfile=.*$/pidfile=\/var\/run\/mumble-server\/mumble-server.pid/' murmur.ini.system
perl -pi.bak -e 's/^(#|;|)database=.*$/database=\/var\/lib\/mumble-server\/mumble-server.sqlite/' murmur.ini.system
perl -pi.bak -e 's/^(#|;|)uname=.*$/uname=mumble-server/' murmur.ini.system

perl -pi.bak -e 'BEGIN{undef $/;} s/\n/\r\n/smg' murmur.ini.win32
