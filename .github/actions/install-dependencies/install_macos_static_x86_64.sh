set -e
set -x

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

brew install coreutils

envDir="$MUMBLE_ENVIRONMENT_DIR"

if [[ -d "$envDir" && -n "$(ls -A "$envDir")" ]]; then
	echo "Environment is cached"
else
	# We use gtar instead of tar as the default tar on macOS doesn't support
	# the --record-size option
	brew install aria2 gnu-tar xz

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

	chmod +x "$envDir/installed/$MUMBLE_VCPKG_TRIPLET/tools/Ice/slice2cpp"
fi



# Setup PostgreSQL database for the Mumble tests
# Note: we don't configure MySQL as that's not installed on the Azure runners for macOS
# by default and installing it via homebrew takes forever.

echo "Configuring PostgreSQL..."

brew install postgresql
brew services start postgresql

# Give the database some time to start
sleep 5

echo "CREATE DATABASE mumble_test_db; "\
	"CREATE USER mumble_test_user ENCRYPTED PASSWORD 'MumbleTestPassword'; "\
	"ALTER DATABASE mumble_test_db OWNER TO mumble_test_user;" | psql -d postgres
