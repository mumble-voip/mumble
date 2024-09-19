#!/usr/bin/env bash

. /etc/os-release

sudo apt update

sudo apt -y install \
	build-essential \
	g++-multilib \
	ninja-build \
	pkg-config \
	qt6-base-dev \
	qt6-tools-dev \
	libboost-dev \
	libssl-dev \
	libprotobuf-dev \
	protobuf-compiler \
	libprotoc-dev \
	libcap-dev \
	libxi-dev \
	libasound2-dev \
	libasound2-plugins \
	libasound2-plugins-extra \
	libogg-dev \
	libsndfile1-dev \
	libopus-dev \
	libspeechd-dev \
	libavahi-compat-libdnssd-dev \
	libzeroc-ice-dev \
	zsync \
	appstream \
	libpoco-dev

# The package was initially called libqt6svg6-dev.
# Choose correct name based on the Ubuntu version.
if [[ $VERSION_ID = "22.04" ]]
then
	# libgl-dev is required in order for GUI-related components to be found by CMake.
	# qt6-l10n-tools and qt6-tools-dev-tools are required in order for LinguistTools to be found by CMake.
	sudo apt -y install libgl-dev \
						libqt6svg6-dev \
						qt6-l10n-tools \
						qt6-tools-dev-tools
else
	sudo apt -y install qt6-svg-dev
fi
