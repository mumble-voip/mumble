#!/usr/bin/env bash

set -e
set -x

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
	libpoco-dev \
	libsqlite3-dev

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
	"ALTER DATABASE mumble_test_db OWNER TO mumble_test_user;" | sudo -u postgres psql
