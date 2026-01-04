#!/usr/bin/env bash

set -e
set -x

source "$( dirname "$0" )/common.sh"

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
	libasound2-dev \
	libsystemd-dev

verify_required_env_variables_set

make_build_env_available "tar.xz"

# MySQL and PostgreSQL are pre-installed on GitHub-hosted runners.
# Set them up for the Mumble tests

echo -e "[mysqld]\nlog-bin-trust-function-creators = 1\nsocket=/var/run/mysqld/mysqld.sock\n\n" | sudo tee -a /etc/mysql/my.cnf

# By default the statically compiled MySQL seems to ignore local config files
# and instead search for a socket under /tmp/, which doesn't exist.
# Hence, we use this environment variable to explicitly tell it the socket location
echo "MYSQL_UNIX_PORT=/var/run/mysqld/mysqld.sock" >> "$GITHUB_ENV"

sudo systemctl enable mysql.service
sudo systemctl start mysql.service

sudo systemctl enable postgresql.service
sudo systemctl start postgresql.service

configure_database_tables "mysql" "postgresql"
