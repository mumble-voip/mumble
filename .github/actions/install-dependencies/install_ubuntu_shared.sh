#!/usr/bin/env bash

set -e
set -x

source "$( dirname "$0" )/common.sh"

source /etc/os-release

sudo apt update

case $MUMBLE_CC in
	gcc|gcc-*)
		sudo apt -y install $MUMBLE_CC-multilib $MUMBLE_CXX-multilib
		;;
esac

sudo apt -y install \
	build-essential \
	g++-multilib \
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
