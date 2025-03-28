#!/bin/bash -ex
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

sudo apt-get update

sudo apt -y install \
	build-essential \
	g++-multilib \
	ninja-build \
	pkg-config \
	qt6-base-dev \
	qt6-l10n-tools \
	qt6-tools-dev \
	qt6-tools-dev-tools \
	libqt6svg6-dev \
	libgl-dev \
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



# Setup MySQL and PostgreSQL databases for the Mumble tests
echo "Configuring MySQL..."

echo -e "[mysqld]\nlog-bin-trust-function-creators = 1" | sudo tee -a /etc/mysql/my.cnf

sudo systemctl enable mysql.service
sudo systemctl start mysql.service

echo "CREATE DATABASE mumble_test_db; "\
	"CREATE USER 'mumble_test_user'@'localhost' IDENTIFIED BY 'MumbleTestPassword'; "\
	"GRANT ALL PRIVILEGES ON mumble_test_db.* TO 'mumble_test_user'@'localhost';"  | sudo mysql --user=root --password="root"


echo "Configuring PostgreSQL..."

sudo systemctl enable postgresql.service
sudo systemctl start postgresql.service

echo "CREATE DATABASE mumble_test_db; "\
	"CREATE USER mumble_test_user ENCRYPTED PASSWORD 'MumbleTestPassword'; "\
	"ALTER DATABASE mumble_test_db OWNER TO mumble_test_user;" | sudo -u postgres psql
