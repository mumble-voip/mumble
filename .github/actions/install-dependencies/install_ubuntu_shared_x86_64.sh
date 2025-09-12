#!/usr/bin/env bash

set -e
set -x

source "$( dirname "$0" )/common.sh"

source /etc/os-release

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
	libpoco-dev \
	libsqlite3-dev

# The package was initially called libqt6svg6-dev.
# Choose correct name based on the Ubuntu version along with some other version-specific setup
if [[ $VERSION_ID = "22.04" ]]; then
	# libgl-dev is required in order for GUI-related components to be found by CMake.
	# qt6-l10n-tools and qt6-tools-dev-tools are required in order for LinguistTools to be found by CMake.
	sudo apt -y install libgl-dev \
						libqt6svg6-dev \
						qt6-l10n-tools \
						qt6-tools-dev-tools \
						gcc-12 \
						g++-12-multilib

	# Ensure we're using GCC-12
	# https://askubuntu.com/a/26518
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 10
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 10

	sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30
	sudo update-alternatives --set cc /usr/bin/gcc

	sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30
	sudo update-alternatives --set c++ /usr/bin/g++

	sudo update-alternatives --config gcc
	sudo update-alternatives --config g++

	# There is a bug in GCC 12 (that got fixed in GCC 12.4 but we only have 12.3 in the repos)
	# that can lead to bogus -Wrestrict warnings, which our configuration treats as an error
	# Cmp. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105651
	# We want to prevent that from happening.
	echo "CXXFLAGS=-Wno-error=restrict" >> "$GITHUB_ENV"
else
	sudo apt -y install qt6-svg-dev
fi

# MySQL and PostgreSQL are pre-installed on GitHub-hosted runners.
# Set them up for the Mumble tests
echo -e "[mysqld]\nlog-bin-trust-function-creators = 1" | sudo tee -a /etc/mysql/my.cnf

sudo systemctl enable mysql.service
sudo systemctl start mysql.service

sudo systemctl enable postgresql.service
sudo systemctl start postgresql.service

configure_database_tables "mysql" "postgresql"
