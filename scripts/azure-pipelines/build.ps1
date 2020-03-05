# Builds Mumble using the specified build script.
# The path to the script is relative to the build environment's root.
# The configuration we build with is adjusted to be close to
# our release builds.
#
# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  AGENT_BUILDDIRECTORY       - The local path on the agent where all folders
#                               for a given build pipeline are created
#                               (e.g. "c:\agent_work\1").
#  BUILD_SOURCESDIRECTORY     - The local path on the agent where the
#                               repository is downloaded.
#                               (e.g. "c:\agent_work\1\s").
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
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-no-ltcg-1.3.x-2017-02-02-ec94ddb-790).
#                               Must match .7z and extracted folder name.
#  MUMBLE_BUILDSCRIPT         - Path to required build script cmd file. Relative to build
#                               environment's "mumble-releng\buildscripts" path.
#                               (e.g. "1.3.x/buildenv-win64-static.cmd").
#
# Defined in the YAML configuration:
#
#  MUMBLE_NO_PCH              - Indicates whether the build should not use PCH.
#

$MUMBLE_BUILD_DIR = $env:AGENT_BUILDDIRECTORY
$MUMBLE_SOURCE_DIR = $env:BUILD_SOURCESDIRECTORY
$MUMBLE_BUILDENV_DIR = Join-Path (Join-Path $env:AGENT_TOOLSDIRECTORY $env:MUMBLE_ENVIRONMENT_DIR) $env:MUMBLE_ENVIRONMENT_VERSION
$MUMBLE_BUILDSCRIPT = Join-Path $MUMBLE_BUILDENV_DIR "mumble-releng\buildscripts\$env:MUMBLE_BUILDSCRIPT"

$env:MUMBLE_EXTRA_QMAKE_CONFIG_FLAGS = ""

# We do not sign the Azure Pipelines CI builds, so we must disable
# uiaccess elevation. Also no intermediary signing is wanted.
$env:MUMBLE_EXTRA_QMAKE_CONFIG_FLAGS = $env:MUMBLE_EXTRA_QMAKE_CONFIG_FLAGS + " no-elevation"

# If "MUMBLE_NO_PCH" is enabled, pass "no-pch".
if ($env:MUMBLE_NO_PCH -eq 1) {
	$env:MUMBLE_EXTRA_QMAKE_CONFIG_FLAGS = $env:MUMBLE_EXTRA_QMAKE_CONFIG_FLAGS + " no-pch"
}

# Use jom to take advantage of the multiple cores we get on the builder.
$env:MUMBLE_NMAKE = "jom"
$env:MUMBLE_SKIP_COLLECT_SYMBOLS = "1"
$env:MUMBLE_SKIP_INTERNAL_SIGNING = "1"
$env:MUMBLE_BUILDENV_DIR = $MUMBLE_BUILDENV_DIR

Get-ChildItem -Path $MUMBLE_BUILD_DIR

& $MUMBLE_BUILDSCRIPT

if ($lastexitcode -ne 0) {
	Write-Output "Skipping build-artifatcs due to a build-error"
	exit $lastexitcode
}

if ($env:MUMBLE_PUBLISH_INSTALLER -eq 1) {
	Write-Output "Copying build-artifacts to destination directory '$env:BUILD_ARTIFACTSTAGINGDIRECTORY'"
	cp installer\bin\x64\Release\*.msi "$env:BUILD_ARTIFACTSTAGINGDIRECTORY"
}

exit $lastexitcode
