:: Copyright 2021 The Mumble Developers. All rights reserved.
:: Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file at the root of the
:: Mumble source tree or at <https://www.mumble.info/LICENSE>.
::
:: Builds Mumble using the specified build script.
:: The path to the script is relative to the build environment's root.
:: The configuration we build with is adjusted to be close to
:: our release builds.
::
:: Below is a list of configuration variables used from environment.
::
::  MUMBLE_BUILD_DIRECTORY       - Output directory.
::  MUMBLE_BUILD_NUMBER_TOKEN    - Access token for the build number page on our server.
::  MUMBLE_ENVIRONMENT_PATH      - Path to the environment's root directory.
::  MUMBLE_ENVIRONMENT_TOOLCHAIN - Path to the vcpkg CMake toolchain, used for CMake.
::  MUMBLE_ENVIRONMENT_TRIPLET   - vcpkg triplet.
::  MUMBLE_SOURCE_COMMIT         - Hash of the Git commit that is being built.
::  MUMBLE_SOURCE_REPOSITORY     - Path to the cloned repository.
::  VCVARS_PATH                  - Path to the Visual Studio environment initialization script.
::

@echo on

:: The method we use to store a command's output into a variable:
:: https://stackoverflow.com/a/6362922
for /f "tokens=* USEBACKQ" %%g in (`python "scripts\mumble-version.py" --format version`) do (set "VERSION=%%g")

:: For some really stupid reason we can't have this statement and the one where we set the VERSION variable in the same if body as
:: in that case the variable substitution of that variable in the expression below fails (is replaced with empty string)
:: Also we can't anything else inside the if body as this will cause the curl command to always be executed.
if defined MUMBLE_BUILD_NUMBER_TOKEN (
	for /f "tokens=* USEBACKQ" %%g in (`curl "https://mumble.info/get-build-number?commit=%MUMBLE_SOURCE_COMMIT%&version=%VERSION%&token=%MUMBLE_BUILD_NUMBER_TOKEN%"`) do (set "BUILD_NUMBER=%%g")
) else (
	echo Build number token not set - defaulting to 0
	set BUILD_NUMBER=0
)

for /f "tokens=* USEBACKQ" %%g in (`python "scripts\mumble-version.py"`) do (set "RELEASE_ID=%%g")

:: Create build directory if it doesn't exist.
if not exist "%MUMBLE_BUILD_DIRECTORY%" mkdir "%MUMBLE_BUILD_DIRECTORY%

cd "%MUMBLE_BUILD_DIRECTORY%"

call "%VCVARS_PATH%"

set PATH=%PATH%;C:\WixSharp

cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE="%MUMBLE_ENVIRONMENT_TOOLCHAIN%" -DVCPKG_TARGET_TRIPLET=%MUMBLE_ENVIRONMENT_TRIPLET% ^
      -DIce_HOME="%MUMBLE_ENVIRONMENT_PATH%\installed\%MUMBLE_ENVIRONMENT_TRIPLET%" ^
      -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe ^
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_UNITY_BUILD=ON -DRELEASE_ID=%RELEASE_ID% -DBUILD_NUMBER=%BUILD_NUMBER% ^
      -Dpackaging=ON -Dtests=ON -Dstatic=ON -Dsymbols=ON -Dgrpc=ON -Dasio=ON -Dg15=ON ^
      -Ddisplay-install-paths=ON %MUMBLE_SOURCE_REPOSITORY%

if errorlevel 1 (
	exit /b %errorlevel%
)

cmake --build .

if errorlevel 1 (
	exit /b %errorlevel%
)
