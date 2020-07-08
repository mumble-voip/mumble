#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

brew update

# As brew will set a non-zero exit status if one of the packages it is asked to install
# is installed already. Thus we have to iterate through every single one and check if it
# is installed first.
for pkg in pkg-config qt5 boost libogg libvorbis flac libsndfile protobuf openssl ice; do
	if brew ls --versions "$pkg" > /dev/null; then
		echo "Skipping installation of $pkg as it is already installed"
	else
		brew install "$pkg"
	fi
done

