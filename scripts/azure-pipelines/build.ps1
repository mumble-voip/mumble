# Builds Mumble using the specified build script.
# The path to the script is relative to the build environment's root.
# The configuration we build with is adjusted to be close to
# our release builds.
#
# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  AGENT_BUILDDIRECTORY       - Predefined variable.
#                               The local path on the agent where all folders
#                               for a given build pipeline are created
#                               (e.g. "D:\a\1")
#  BUILD_SOURCESDIRECTORY     - Predefined variable.
#                               The local path on the agent where the
#                               repository is downloaded.
#                               (e.g. "D:\a\1\s")
#
# Defined in the visual designer on Azure Pipelines:
#
#  MUMBLE_ENVIRONMENT_DIR     - The local path where the build environment
#                               is stored (e.g. "C:\MumbleBuild").
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-no-ltcg-1.3.x-2017-02-02-ec94ddb-790).
#                               Must match .7z and extracted folder name.
#  MUMBLE_BUILDSCRIPT         - Path to required build script cmd file. Relative to build
#                               environment's "mumble-releng\buildscripts" path.
#                               (e.g. "1.3.x/buildenv-win64-static.cmd")
#
# Defined in the YAML configuration:
#
#  MUMBLE_NO_PCH              - Indicates whether the build should not use PCH.
#

$MUMBLE_BUILD_DIR = $env:AGENT_BUILDDIRECTORY
$MUMBLE_SOURCE_DIR = $env:BUILD_SOURCESDIRECTORY
$MUMBLE_BUILDENV_DIR = Join-Path $env:MUMBLE_ENVIRONMENT_DIR $env:MUMBLE_ENVIRONMENT_VERSION
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
exit $lastexitcode
