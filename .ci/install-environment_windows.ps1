# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Ensures we have downloaded and extracted a build environment into our AppVeyor
# VM before we attempt to build. If the environment archive is already present,
# this script will just extract it.
#
# Below is a list of configuration variables used from environment.
#
#  MUMBLE_ENVIRONMENT_STORE   - Path to the folder the build environment is stored in
#                               (e.g. C:/MumbleBuild).
#  MUMBLE_ENVIRONMENT_SOURCE  - Build environment web source folder URL
#                               (e.g. https://dl.mumble.info/build/vcpkg/windows-static-1.4.x~2021-04-26~7da4529f.x64.7z).
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. windows-static-1.4.x~2021-04-26~7da4529f.x64).
#                               Must match .7z and extracted folder name.
#  MUMBLE_SOURCE_REPOSITORY   - Path to the cloned repository.
#

# Always quit on encountered errors
$ErrorActionPreference = 'Stop'

choco install aria2

# We require a separate function that makes sure the run command's exit code
# is properly checked, so we can mimmick Bash's -e flag.
function Invoke-Command {
  $command = $args[0]
  $arguments = $args[1..($args.Length)]
  & $command @arguments

  # Check for non-zero exit code
  if ($LastExitCode -ne 0) {
    Write-Error "Exit code $LastExitCode while running $command $arguments"
  }
}

# The Download function attempts the download multiple times before
# erroring out
function Download {
	Param(
		[string] $source,
		[string] $destination
	)

	Write-Host "Downloading from '$source' to '$destination'..."

	$iterations = 0
	$maxIterations = 3

	while($iterations -lt $maxIterations) {
		try {
			$iterations += 1

			Write-Host "Attempt #$iterations"

			# Separate directory from filename (needed as we have to feed them separately
			# into aria2c)
			$destDir = Split-Path -Path "$destination" -Parent
			$destName = Split-Path -Path "$destination" -Leaf

			aria2c "$source" --dir="$destDir" --out="$destName"

			Write-Host "Download succeeded"

			break
		} catch {
			Write-Host "Download failed"

			if ($iterations -lt $maxIterations) {
				Write-Host "Retrying in 5s..."
				# We sleep in case this was a network error that might be fixed in a couple of seconds
				Start-Sleep -Seconds 5
			} else {
				Write-Error "Download failed too often - giving up"
			}
		}
	}

}

$TEMP_DIR = [System.IO.Path]::GetTempPath()
Set-Location -Path $TEMP_DIR

$SOURCE_DIR = $env:MUMBLE_SOURCE_REPOSITORY

$MUMBLE_ENVIRONMENT_STORE = $env:MUMBLE_ENVIRONMENT_STORE
$MUMBLE_ENVIRONMENT_VERSION = $env:MUMBLE_ENVIRONMENT_VERSION

if (-Not (Test-Path $MUMBLE_ENVIRONMENT_STORE)) {
	New-Item $MUMBLE_ENVIRONMENT_STORE -ItemType Directory | Out-Null
}

if (-Not (Test-Path (Join-Path $MUMBLE_ENVIRONMENT_STORE $MUMBLE_ENVIRONMENT_VERSION))) {
	Write-Host "Environment not cached. Downloading..."

	$env_url = "$env:MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.7z"
	$env_7z = Join-Path $MUMBLE_ENVIRONMENT_STORE "$MUMBLE_ENVIRONMENT_VERSION.7z";

	Download -source "$env_url" -destination "$env_7z"

	Write-Host "Extracting build environment to $MUMBLE_ENVIRONMENT_STORE..."
	Invoke-Command 7z x $env_7z -o"$MUMBLE_ENVIRONMENT_STORE"
}

Write-Host "Downloading ASIO SDK..."

Download -source "https://dl.mumble.info/build/extra/asio_sdk.zip" -destination "asio_sdk.zip"
Invoke-Command 7z x "asio_sdk.zip"
Move-Item -Path "asiosdk_2.3.3_2019-06-14" -Destination "$SOURCE_DIR/3rdparty/asio"


Write-Host "Downloading G15 SDK..."

Download -source "https://dl.mumble.info/build/extra/g15_sdk.zip" -destination "g15_sdk.zip"
Invoke-Command 7z x "g15_sdk.zip"
Move-Item -Path "G15SDK/LCDSDK" -Destination "$SOURCE_DIR/3rdparty/g15"


Write-Host "Downloading WixSharp..."

Download -source "https://github.com/oleg-shilo/wixsharp/releases/download/v1.19.0.0/WixSharp.1.19.0.0.7z" -destination "WixSharp.7z"
Write-Host "Exracting WixSharp to C:/WixSharp..."
Invoke-Command 7z x "WixSharp.7z" "-oC:/WixSharp"

choco install vswhere

Write-Host "Build environment successfully installed"
