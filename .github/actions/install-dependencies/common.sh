# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set -e
set -x

verify_required_env_variables_set() {
	if [[ -z "$MUMBLE_ENVIRONMENT_SOURCE" ]]; then
		echo "MUMBLE_ENVIRONMENT_SOURCE not set!" 1>&2
		exit 1
	fi

	if [[ -z "$MUMBLE_ENVIRONMENT_VERSION" ]]; then
		echo "MUMBLE_ENVIRONMENT_VERSION not set!" 1>&2
		exit 1
	fi

	if [[ -z "$MUMBLE_ENVIRONMENT_DIR" ]]; then
		echo "MUMBLE_ENVIRONMENT_DIR not set!" 1>&2
		exit 1
	fi
}

extract_with_progress() {
	local fromFile="$1"
	local targetDir="$2"

	if [ -z "$fromFile" ]; then
		echo "[ERROR]: Missing argument"
		exit 1
	fi

	if [ -z "$targetDir" ]; then
		targetDir="."
	fi

	echo "Extracting from \"$fromFile\" to \"$targetDir\""
	echo ""

	# Make targetDir an absolute path
	targetDir="$( realpath "$targetDir" )"

	# Use gtar and gwc if available (for MacOS compatibility)
	tar_exe="tar"
	if [ -x "$(command -v gtar)" ]; then
		tar_exe="gtar"
	fi
	wc_exe="wc"
	if [ -x "$(command -v gwc)" ]; then
		wc_exe="gwc"
	fi

	tmp_dir="__extract_root__"
	mkdir "$tmp_dir"

	if [[ "$fromFile" = *.7z || "$fromFile"  = *.zip ]]; then
		extract_cmd=( 7z x "$fromFile" -o"$tmp_dir" )

		summary="$( 7z l "$fromFile" | tail -n 1 )"
		fromSize="$( echo "$summary" | tr -s ' ' | cut -d ' ' -f 4 )"
		toSize="$( echo "$summary" | tr -s ' ' | cut -d ' ' -f 3 )"
	else
		# Get sizes in bytes
		fromSize=$(xz --robot --list "$fromFile" | tail -n -1 | cut -f 4)
		toSize=$(xz --robot --list "$fromFile" | tail -n -1 | cut -f 5)

		steps=100
		checkPointStep=$(expr "$toSize" / 1000 / "$steps" )

		extract_cmd=( "$tar_exe" -x --record-size=1K --checkpoint="$checkPointStep" --checkpoint-action="echo=%u / $toSize" --file "$fromFile" --directory "$tmp_dir" )
	fi

	# Convert sizes to KB
	local toSizeKB=$(expr "$toSize" / 1000)
	local fromSizeKB=$(expr "$fromSize" / 1000)

	echo "Compressed size:   $fromSizeKB KB"
	echo "Uncompressed size: $toSizeKB KB"


	echo ""

	"${extract_cmd[@]}"

	local num_files="$( ls -Al "$tmp_dir" | tail -n +2 | $wc_exe -l )"

	if [[ ! -d "$targetDir" ]]; then
		mkdir "$targetDir"
	fi

	if [[ "$num_files" = 1 && -d "$tmp_dir/$( ls "$tmp_dir" )" ]]; then
		# Skip top-level directory
		pushd "$(pwd)"
		cd "$tmp_dir"/*
		mv * "$targetDir"
		mv .* "$targetDir" 2> /dev/null || true
		popd
	else
		# Move all files
		mv "$tmp_dir"/* "$targetDir"
		mv "$tmp_dir"/.* "$targetDir" 2> /dev/null || true
	fi

	rm -rf "$tmp_dir"
}

make_build_env_available() {
	local env_file_extension="$1"

	if [[ -z "$env_file_extension" ]]; then
		echo "No file extension provided" 1>&2
		exit 1
	fi

	local env_dir="$MUMBLE_ENVIRONMENT_DIR"

	if [[ -d "$env_dir" && -n "$( ls -A "$env_dir" )" ]]; then
		echo "Environment is cached"
	else
		echo "Environment not cached -> downloading now"

		local env_archive="$MUMBLE_ENVIRONMENT_VERSION.$env_file_extension"

		aria2c "$MUMBLE_ENVIRONMENT_SOURCE/$env_archive" --out="$env_archive"

		echo "Extracting archive..."
		if [[ ! -d "$env_dir" ]]; then
			mkdir -p "$env_dir"
		fi

		extract_with_progress "$env_archive" "$env_dir"

		if [[ ! -d "$env_dir" || -z "$( ls -A "$env_dir" )" ]]; then
			echo "Environment did not follow expected form" 1>&2
			ls -al "$env_dir"
			exit 1
		fi

		chmod +x "$env_dir/installed/$MUMBLE_VCPKG_TRIPLET/tools/Ice/slice2cpp"
	fi
}

configure_database_tables() {
	if [[ -x "$( which sudo )" ]]; then
		sudo_cmd="sudo"
	else
		sudo_cmd=""
	fi

	while [[ "$#" -gt 0 ]]; do
		case "$1" in
			"mysql")
				local sql_statements="CREATE DATABASE mumble_test_db;"
				sql_statements+="CREATE USER 'mumble_test_user'@'localhost' IDENTIFIED BY 'MumbleTestPassword';"
				sql_statements+="GRANT ALL PRIVILEGES ON mumble_test_db.* TO 'mumble_test_user'@'localhost';"

				if $sudo_cmd mysql --user=root -e "SELECT 1" 2> /dev/null; then
					# Passwordless
					mysql_cmd=( $sudo_cmd mysql --user=root )
				else
					mysql_cmd=( $sudo_cmd mysql --user=root --password="root" )
				fi

				echo "$sql_statements" | "${mysql_cmd[@]}"
				;;

			"postgresql")
				local sql_statements="CREATE DATABASE mumble_test_db;"
				sql_statements+="CREATE USER mumble_test_user ENCRYPTED PASSWORD 'MumbleTestPassword';"
				sql_statements+="ALTER DATABASE mumble_test_db OWNER TO mumble_test_user;"

				if [[ -n "$sudo_cmd" ]] && id -u postgres > /dev/null 1>&1; then
					# User postgres exists and we can use sudo to execute commands as that user
					psql_cmd=( "$sudo_cmd" -u postgres psql )
				else
					psql_cmd=( psql -d postgres )
				fi

				echo "$sql_statements" | "${psql_cmd[@]}"
				;;

			*)
				echo "Unsupported database '$1'" 1>&2
				exit 1
				;;
		esac

		shift
	done
}
