# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Ensures we have downloaded and extracted a build environment
# into our Azure Pipelines VM before we attempt to build. If the
# environment archive is already present, this script will just extract it.
#
# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  AGENT_TEMPDIRECTORY        - A temporary folder that is cleaned after each pipeline job.
#                               This directory is used by tasks such as .NET Core CLI task
#                               to hold temporary items like test results before they are published.
#  AGENT_TOOLSDIRECTORY       - The directory used by tasks such as
#                               Node Tool Installer and Use Python Version
#                               to switch between multiple versions of a tool.
#                               We store our build environment there, in the
#                               folder specified by MUMBLE_ENVIRONMENT_DIR.
#
# Defined in .azure-pipelines.yml:
#
#  MUMBLE_ENVIRONMENT_STORE   - Path to the folder the build environment is stored in.
#                               (e.g. c:\hostedtoolcache\windows\MumbleBuild).
#  MUMBLE_ENVIRONMENT_SOURCE  - Build environment web source folder URL
#                               (e.g. https://somehost/folder).
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-1.4.x-2020-05-27-ecb3c64-1151).
#                               Must match .7z and extracted folder name.
#

Set-Location -Path $env:AGENT_TEMPDIRECTORY

$SOURCE_DIR = $env:BUILD_SOURCESDIRECTORY

$MUMBLE_ENVIRONMENT_STORE = $env:MUMBLE_ENVIRONMENT_STORE
$MUMBLE_ENVIRONMENT_VERSION = $env:MUMBLE_ENVIRONMENT_VERSION

if (-Not (Test-Path $MUMBLE_ENVIRONMENT_STORE)) {
	New-Item $MUMBLE_ENVIRONMENT_STORE -ItemType Directory | Out-Null
}

if (-Not (Test-Path (Join-Path $MUMBLE_ENVIRONMENT_STORE $MUMBLE_ENVIRONMENT_VERSION))) {
	Write-Host "Environment not cached. Downloading..."

	$env_url = "$env:MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.7z"
	$env_7z = Join-Path $MUMBLE_ENVIRONMENT_STORE "$MUMBLE_ENVIRONMENT_VERSION.7z";
	try {
		# According to https://github.com/PowerShell/PowerShell/issues/2138 disabling the
		# progress bar can significantly increase the speed of Invoke-Web-Request.
		$ProgressPreference = 'SilentlyContinue'

		Invoke-WebRequest -Uri $env_url -OutFile $env_7z

		# Reset progress bar
		$ProgressPreference = 'Continue'
	} catch {
		Write-Host "Failed to download build-environment: $PSItem"
		exit 1
	}

	Write-Host "Extracting build environment to $MUMBLE_ENVIRONMENT_STORE..."
	7z x $env_7z -o"$MUMBLE_ENVIRONMENT_STORE"
}

Write-Host "Downloading ASIO SDK..."

try {
	Invoke-WebRequest -Uri "https://dl.mumble.info/build/extra/asio_sdk.zip" -OutFile "asio_sdk.zip"
	7z x "asio_sdk.zip"
	Move-Item -Path "asiosdk_2.3.3_2019-06-14" -Destination "$SOURCE_DIR/3rdparty/asio"
} catch {
	Write-Host "Failed to download ASIO SDK: $PSItem"
	exit 1
}

Write-Host "Downloading G15 SDK..."

try {
	Invoke-WebRequest -Uri "https://dl.mumble.info/build/extra/g15_sdk.zip" -OutFile "g15_sdk.zip"
	7z x "g15_sdk.zip"
	Move-Item -Path "G15SDK/LCDSDK" -Destination "$SOURCE_DIR/3rdparty/g15"
} catch {
	Write-Host "Failed to download G15 SDK: $PSItem"
	exit 1
}
