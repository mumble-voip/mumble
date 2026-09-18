#!/usr/bin/env bash

set -e
set -x

source "$( dirname "$0" )/common.sh"

source /etc/os-release

sudo apt update

if [ -z "$MUMBLE_ARCH" ]; then
	MUMBLE_ARCH=x86_64
fi

if [ -z "$MUMBLE_CC" ]; then
	MUMBLE_CC=gcc
	MUMBLE_CXX=g++
fi

case $MUMBLE_CC in
	gcc|gcc-*)
		if [[ "$MUMBLE_ARCH" = "x86_64" ]]; then
			sudo apt -y install $MUMBLE_CC-multilib $MUMBLE_CXX-multilib
		else
			sudo apt -y install $MUMBLE_CC $MUMBLE_CXX
		fi
		;;
	clang|clang-*)
		# Unfortunately there is no package that pulls in the default libstdc++ alone.
		if [[ "$MUMBLE_ARCH" = "x86_64" ]]; then
			sudo apt -y install $MUMBLE_CC g++-multilib
		else
			sudo apt -y install $MUMBLE_CC g++
		fi
		;;
esac

sudo apt -y install \
	build-essential \
	ninja-build \
	pkg-config \
	qt6-base-dev \
	qt6-svg-dev \
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
	libpoco-dev \
	libsqlite3-dev

# MySQL and PostgreSQL are pre-installed on GitHub-hosted runners.
# Set them up for the Mumble tests
echo -e "[mysqld]\nlog-bin-trust-function-creators = 1" | sudo tee -a /etc/mysql/my.cnf

sudo systemctl enable mysql.service
sudo systemctl start mysql.service

sudo systemctl enable postgresql.service
sudo systemctl start postgresql.service

configure_database_tables "mysql" "postgresql"
