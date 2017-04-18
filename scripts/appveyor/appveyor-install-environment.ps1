#
# Ensures we have downloaded and extracted a build environment
# into our appveyor VM before we attempt to build. If the
# environment was already cached and hence already present
# this script will detect that.
#
# Configuration variables used from environment:
#
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-no-ltcg-1.3.x-2017-02-02-ec94ddb-790).
#                               Must match .7z and extracted folder name.
#  MUMBLE_ENVIRONMENT_SOURCE  - Build environment web source folder URL
#                               (e.g. https://somehost/folder)
#
$ErrorActionPreference = "Stop"

$MUMBLE_ENVIRONMENT_VERSION = $env:MUMBLE_ENVIRONMENT_VERSION     # Env. name (must match .7z and extracted folder name)
$MUMBLE_ENVIRONMENT_SOURCE = $env:MUMBLE_ENVIRONMENT_SOURCE       # Env. web source folder URL
$MUMBLE_BUILD_DIR = "C:\MumbleBuild"
$MUMBLE_ENVIRONMENT_STORE = Join-Path $MUMBLE_BUILD_DIR "\cache"

if (-Not (Test-Path $MUMBLE_ENVIRONMENT_STORE)) {
    New-Item $MUMBLE_ENVIRONMENT_STORE -ItemType Directory | Out-Null
 }

$cached_env = Join-Path $MUMBLE_ENVIRONMENT_STORE "$MUMBLE_ENVIRONMENT_VERSION.7z";

if (-Not (Test-Path $cached_env)) {
    Write-Host "Environment not cached. Downloading"
    $env_url = "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.7z"
    Invoke-WebRequest -Uri $env_url -OutFile $cached_env
}

if (-Not (Test-Path (Join-Path $MUMBLE_BUILD_DIR $MUMBLE_ENVIRONMENT_VERSION))) {
    Write-Host "Extracting build environment to $MUMBLE_BUILD_DIR"
    & 7z x $cached_env -o"$MUMBLE_BUILD_DIR"
}

Get-ChildItem -Path $MUMBLE_BUILD_DIR
