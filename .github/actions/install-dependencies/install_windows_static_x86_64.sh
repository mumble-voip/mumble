set -e
set -x

# All of these are already installed on the runner
# choco install cmake ninja vswhere

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

choco install aria2 7zip

if [[ -d "$envDir" && -n "$(ls -A '$envDir')" ]]; then
	echo "Environment is cached"
else
	envArchive="$MUMBLE_ENVIRONMENT_VERSION.7z"

	aria2c "$MUMBLE_ENVIRONMENT_SOURCE/$envArchive" --dir="$envArchive"

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

aria2c "https://dl.mumble.info/build/extra/asio_sdk.zip" --out "asio_sdk.zip"
"$(dirname $0)/extractWithProgress.sh" "asio_sdk.zip" "."
mv "asiosdk_2.3.3_2019-06-14" "${GITHUB_WORKSPACE}/3rdparty/asio"

aria2c "https://dl.mumble.info/build/extra/g15_sdk.zip" --out "g15_sdk.zip"
"$(dirname $0)/extractWithProgress.sh" "g15_sdk.zip" "."
mv "G15SDK/LCDSDK" "${GITHUB_WORKSPACE}/3rdparty/g15"

aria2c "https://github.com/oleg-shilo/wixsharp/releases/download/v1.19.0.0/WixSharp.1.19.0.0.7z" --out "WixSharp.7z"
"$(dirname $0)/extractWithProgress.sh" "WixSharp.7z" "C:/WixSharp"

git clone "https://github.com/nathan818fr/vcvars-bash.git" "C:/vcvars-bash"


echo "Configuring MySQL"
echo -e "[mysqld]\nlog-bin-trust-function-creators = 1" >> "C:/Windows/my.ini"

mysqld --initialize-insecure --console

powershell -Command "Start-Process mysqld"

# Give the MySQL daemon some time to start up
sleep 5

echo "CREATE DATABASE mumble_test_db; CREATE USER 'mumble_test_user'@'localhost' IDENTIFIED BY 'MumbleTestPassword'; GRANT ALL PRIVILEGES ON mumble_test_db.* TO 'mumble_test_user'@'localhost';" | mysql --user=root
