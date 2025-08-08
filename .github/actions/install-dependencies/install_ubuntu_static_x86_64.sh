#!/usr/bin/env bash

sudo apt update

sudo apt -y install \
	build-essential \
	g++-multilib \
	ninja-build \
	libcap-dev \
	libdbus-1-dev \
	libxi-dev \
	libxkbcommon-dev \
	libxkbcommon-x11-dev \
	libxcb-icccm4-dev \
	libxcb-image0-dev \
	libxcb-keysyms1-dev \
	libxcb-render-util0-dev \
	libxcb-xinerama0-dev \
	libxcb-xinput-dev \
	libxcb-xkb-dev \
	libasound2-dev \
	libpulse-dev \
	libspeechd-dev \
	libgl1-mesa-dev \
	libavahi-compat-libdnssd-dev \
	libglu1-mesa-dev \
	mesa-common-dev \
	libxrandr-dev \
	libxxf86vm-dev \
	libgl-dev


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
if [[ "$MUMBLE_BUILD_ENV_PATH" == ""  ]]; then
	echo "MUMBLE_BUILD_ENV_PATH not set!"
	exit 1
fi

envDir="$MUMBLE_BUILD_ENV_PATH"

echo "$MUMBLE_BUILD_ENV_PATH"
echo "$envDir"
echo "$MUMBLE_ENVIRONMENT_PA"

ls -al "$envDir"

if [[ -d "$envDir" && -n "$(ls -A '$envDir')" ]]; then
	echo "Environment is cached"
else
	sudo apt install axel

	envArchive="$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

	axel -n 5 --output="$envArchive" "$MUMBLE_ENVIRONMENT_SOURCE/$envArchive"

	echo "Extracting archive..."
	if [[ ! -d "$MUMBLE_ENVIRONMENT_DIR" ]]; then
		mkdir -p "$envDir"
	fi

	"$(dirname $0)/extractWithProgress.sh" "$envArchive" "$MUMBLE_ENVIRONMENT_DIR"

	if [[ ! -d "$envDir" || -n "$(ls -A '$envDir')" ]]; then
		echo "Environment did not follow expected form"
		ls -al "$MUMBLE_ENVIRONMENT_PATH"
		exit 1
	fi

	ls -al "$envDir"
fi


