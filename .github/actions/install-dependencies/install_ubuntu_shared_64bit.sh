#!/usr/bin/env bash

set -e
set -x

sudo apt update

sudo apt -y install \
	build-essential \
	g++-multilib \
	ninja-build \
	pkg-config \
	qtbase5-dev \
	qttools5-dev \
	qttools5-dev-tools \
	libqt5svg5-dev \
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

# MySQL and PostgreSQL are pre-installed on GitHub-hosted runners. More info about the default setup can be found at
# - https://github.com/actions/virtual-environments/blob/main/images/linux/Ubuntu1804-Readme.md#databases
# - https://github.com/actions/virtual-environments/blob/main/images/linux/Ubuntu2004-Readme.md#databases

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
	"GRANT ALL PRIVILEGES ON DATABASE mumble_test_db TO mumble_test_user;" | sudo -u postgres psql
