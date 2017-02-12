#
# Builds mumble using the given environment and build script.
# The configuration we build with is adjusted to be close to
# our release builds.
#
# Configuration variables used from environment:
#
#  MUMBLE_ENVIRONMENT_VERSION - Full build environment version
#                               (e.g. win64-static-no-ltcg-1.3.x-2017-02-02-ec94ddb-790).
#                               Must match .7z and extracted folder name.
#  MUMBLE_BUILDSCRIPT         - Path to required build script cmd file. Relative to build
#                               environment mumble-releng\buildscripts.
#                               E.g.  1.3.x/buildenv-win64-static.cmd
#
$ErrorActionPreference = "Stop"

$MUMBLE_BUILD_DIR = "C:\MumbleBuild"
$MUMBLE_SOURCE_DIR = Join-Path $MUMBLE_BUILD_DIR "mumble"
$MUMBLE_BUILDENV_DIR = Join-Path $MUMBLE_BUILD_DIR $env:MUMBLE_ENVIRONMENT_VERSION
$MUMBLE_BUILDSCRIPT = Join-Path $MUMBLE_BUILDENV_DIR "mumble-releng\buildscripts\$env:MUMBLE_BUILDSCRIPT"

# We do not sign the appveyor CI builds so we mustn't disable
# uiaccess elevation. Also no intermediary signing is wanted.
# Also use jom to use both cores we get on the builder.
$env:MUMBLE_EXTRA_QMAKE_CONFIG_FLAGS = "no-elevation"
$env:MUMBLE_SKIP_INTERNAL_SIGNING = "1"
$env:MUMBLE_SKIP_COLLECT_SYMBOLS = "1"
$env:MUMBLE_NMAKE = "jom"
$env:MUMBLE_BUILDENV_DIR = $MUMBLE_BUILDENV_DIR

Set-Location $MUMBLE_SOURCE_DIR
& $MUMBLE_BUILDSCRIPT
exit $lastexitcode
