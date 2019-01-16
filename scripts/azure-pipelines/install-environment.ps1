# Ensures we have downloaded and extracted a build environment
# into our Azure Pipelines VM before we attempt to build. If the
# environment archive is already present, this script will just extract it.
#
# Configuration variables (defined in the visual designer on Azure Pipelines) used from environment:
#
#  MUMBLE_ENVIRONMENT_DIR     - The local path where the build environment
#                               will be stored (e.g. "C:\MumbleBuild").
#  MUMBLE_ENVIRONMENT_SOURCE  - Build environment web source folder URL
#                               (e.g. https://somehost/folder).
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-no-ltcg-1.3.x-2017-02-02-ec94ddb-790).
#                               Must match .7z and extracted folder name.
#

$MUMBLE_ENVIRONMENT_VERSION = $env:MUMBLE_ENVIRONMENT_VERSION
$MUMBLE_ENVIRONMENT_SOURCE = $env:MUMBLE_ENVIRONMENT_SOURCE
$MUMBLE_ENVIRONMENT_DIR = $env:MUMBLE_ENVIRONMENT_DIR
$MUMBLE_ENVIRONMENT_STORE = Join-Path $MUMBLE_ENVIRONMENT_DIR "\cache"

if (-Not (Test-Path $MUMBLE_ENVIRONMENT_STORE)) {
    New-Item $MUMBLE_ENVIRONMENT_STORE -ItemType Directory | Out-Null
 }

$env_7z = Join-Path $MUMBLE_ENVIRONMENT_STORE "$MUMBLE_ENVIRONMENT_VERSION.7z";

if (-Not (Test-Path $env_7z)) {
    Write-Host "Environment not cached. Downloading..."
    $env_url = "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.7z"
    Invoke-WebRequest -Uri $env_url -OutFile $env_7z
}

if (-Not (Test-Path (Join-Path $MUMBLE_ENVIRONMENT_DIR $MUMBLE_ENVIRONMENT_VERSION))) {
    Write-Host "Extracting build environment to $MUMBLE_ENVIRONMENT_DIR..."
    & C:\ProgramData\chocolatey\bin\7z.exe x $env_7z -o"$MUMBLE_ENVIRONMENT_DIR"
}

Get-ChildItem -Path $MUMBLE_ENVIRONMENT_DIR
