# Ensures we have downloaded and extracted a build environment
# into our Azure Pipelines VM before we attempt to build. If the
# environment archive is already present, this script will just extract it.
#
# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  AGENT_TOOLSDIRECTORY       - The directory used by tasks such as
#                               Node Tool Installer and Use Python Version
#                               to switch between multiple versions of a tool.
#                               We store our build environment there, in the
#                               folder specified by MUMBLE_ENVIRONMENT_DIR.
#
# Defined in the visual designer on Azure Pipelines:
#
#  MUMBLE_ENVIRONMENT_DIR     - Folder where the build environment will be stored
#                               (e.g. "MumbleBuild").
#  MUMBLE_ENVIRONMENT_SOURCE  - Build environment web source folder URL
#                               (e.g. https://somehost/folder).
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-no-ltcg-1.3.x-2017-02-02-ec94ddb-790).
#                               Must match .7z and extracted folder name.
#

$MUMBLE_ENVIRONMENT_VERSION = $env:MUMBLE_ENVIRONMENT_VERSION
$MUMBLE_ENVIRONMENT_SOURCE = $env:MUMBLE_ENVIRONMENT_SOURCE
$MUMBLE_ENVIRONMENT_PATH = Join-Path $env:AGENT_TOOLSDIRECTORY $env:MUMBLE_ENVIRONMENT_DIR
$MUMBLE_ENVIRONMENT_STORE = Join-Path $MUMBLE_ENVIRONMENT_PATH "cache"

if (-Not (Test-Path $MUMBLE_ENVIRONMENT_STORE)) {
    New-Item $MUMBLE_ENVIRONMENT_STORE -ItemType Directory | Out-Null
 }

$env_7z = Join-Path $MUMBLE_ENVIRONMENT_STORE "$MUMBLE_ENVIRONMENT_VERSION.7z";

if (-Not (Test-Path $env_7z)) {
    Write-Host "Environment not cached. Downloading..."
    $env_url = "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.7z"
	try {
		# According to https://github.com/PowerShell/PowerShell/issues/2138 disabling the
		# progressbar can significantly increase the speed of Invoke-Web-Request. 
		$ProgressPreference = 'SilentlyContinue'

		Invoke-WebRequest -Uri $env_url -OutFile $env_7z

		# Reset progressbar
		$ProgressPreference = 'Continue'
	} catch {
		Write-Host "Failed at downloading build-environment: $PSItem"
		exit 1
	}
}

if (-Not (Test-Path (Join-Path $MUMBLE_ENVIRONMENT_PATH $MUMBLE_ENVIRONMENT_VERSION))) {
    Write-Host "Extracting build environment to $MUMBLE_ENVIRONMENT_PATH..."
    & C:\ProgramData\chocolatey\bin\7z.exe x $env_7z -o"$MUMBLE_ENVIRONMENT_PATH"
}

Get-ChildItem -Path $MUMBLE_ENVIRONMENT_PATH

# We have to create a symlink because unfortunately C:\MumbleBuild is hardcoded
# in our environment (e.g. qmake's configuration files' path).
New-Item -Path C:\MumbleBuild -ItemType SymbolicLink -Value $MUMBLE_ENVIRONMENT_PATH
