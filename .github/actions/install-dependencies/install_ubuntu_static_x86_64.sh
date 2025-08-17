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


# MySQL and PostgreSQL are pre-installed on GitHub-hosted runners. More info about the default setup can be found at
# - https://github.com/actions/virtual-environments/blob/main/images/linux/Ubuntu1804-Readme.md#databases
# - https://github.com/actions/virtual-environments/blob/main/images/linux/Ubuntu2004-Readme.md#databases

# Setup MySQL and PostgreSQL databases for the Mumble tests
echo "Configuring MySQL..."

echo -e "[mysqld]\nlog-bin-trust-function-creators = 1\nsocket=/var/run/mysqld/mysqld.sock\n\n" | sudo tee -a /etc/mysql/my.cnf

# By default the statically compiled MySQL seems to ignore local config files
# and instead search for a socket under /tmp/, which doesn't exist.
# Hence, we use this environment variable to explicitly tell it the socket location
echo "MYSQL_UNIX_PORT=/var/run/mysqld/mysqld.sock" >> "$GITHUB_ENV"

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
