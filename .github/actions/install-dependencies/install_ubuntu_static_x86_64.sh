#!/usr/bin/env bash

set -e
set -x

sudo apt update

sudo apt -y install \
	build-essential \
	g++-multilib \
	ninja-build \
	`# Still required for qtbase vcpkg package` \
	'^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev libegl1-mesa-dev \
	`# TODO: can we get rid of these by replacing with vcpkg packages?` \
	libsm-dev \
	libspeechd-dev \
	libavahi-compat-libdnssd-dev \
	libasound2-dev
	

if [[ "$MUMBLE_ENVIRONMENT_SOURCE" == "" ]]; then
	echo "MUMBLE_ENVIRONMENT_SOURCE not set!"
	exit 1
fi
if [[ "$MUMBLE_ENVIRONMENT_VERSION" == "" ]]; then
	echo "MUMBLE_ENVIRONMENT_VERSION not set!"
	exit 1
fi
if [[ "$MUMBLE_ENVIRONMENT_DIR" == ""  ]]; then
	echo "MUMBLE_ENVIRONMENT_DIR not set!"
	exit 1
fi

envDir="$MUMBLE_ENVIRONMENT_DIR"

if [[ -d "$envDir" && -n "$(ls -A "$envDir")" ]]; then
	echo "Environment is cached"
else
	sudo apt install aria2

	envArchive="$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

	aria2c "$MUMBLE_ENVIRONMENT_SOURCE/$envArchive" --out="$envArchive"

	echo "Extracting archive..."
	if [[ ! -d "$envDir" ]]; then
		mkdir -p "$envDir"
	fi

	"$(dirname $0)/extractWithProgress.sh" "$envArchive" "$envDir"

	if [[ ! -d "$envDir" || -z "$(ls -A "$envDir")" ]]; then
		echo "Environment did not follow expected form"
		ls -al "$envDir"
		exit 1
	fi
fi


