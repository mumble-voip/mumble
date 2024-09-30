:: Copyright The Mumble Developers. All rights reserved.
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
::  MUMBLE_USE_ELEVATION         - Whether to build the Mumble client with elevated permissions (required to make shortcuts work with privileged
::                                 applications)
::

@echo on

:: The method we use to store a command's output into a variable:
:: https://stackoverflow.com/a/6362922
for /f "tokens=* USEBACKQ" %%g in (`python "scripts\mumble-version.py"`) do (set "VERSION=%%g")

for /f "tokens=* USEBACKQ" %%g in (`python "scripts\mumble-build-number.py" --commit "%MUMBLE_SOURCE_COMMIT%" --version "%VERSION%" ^
	--password "%MUMBLE_BUILD_NUMBER_TOKEN%" --default 0`) do (set "BUILD_NUMBER=%%g")

:: Create build directory if it doesn't exist.
if not exist "%MUMBLE_BUILD_DIRECTORY%" mkdir "%MUMBLE_BUILD_DIRECTORY%

cd "%MUMBLE_BUILD_DIRECTORY%"

for /f "tokens=* USEBACKQ" %%g in (`vswhere -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
	-property installationPath`) do ( set "VCVARS_PATH=%%g\VC\Auxiliary\Build\vcvars64.bat" )

call "%VCVARS_PATH%"

set PATH=%PATH%;C:\WixSharp

if not defined MUMBLE_USE_ELEVATION (
	:: If the variable is not set, default to OFF (using elevation requires the binary to be signed - otherwise
	:: it will simply crash)
	set MUMBLE_USE_ELEVATION=OFF
)

cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE="%MUMBLE_ENVIRONMENT_TOOLCHAIN%" -DVCPKG_TARGET_TRIPLET=%MUMBLE_ENVIRONMENT_TRIPLET% ^
      -DIce_HOME="%MUMBLE_ENVIRONMENT_PATH%\installed\%MUMBLE_ENVIRONMENT_TRIPLET%" ^
      -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe ^
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_UNITY_BUILD=ON -DBUILD_NUMBER=%BUILD_NUMBER% ^
      -Dpackaging=ON -Dtests=ON -Dstatic=ON -Dsymbols=ON -Dasio=ON -Dg15=ON ^
      -Ddisplay-install-paths=ON -Delevation=%MUMBLE_USE_ELEVATION% %MUMBLE_SOURCE_REPOSITORY%

if errorlevel 1 (
	exit /b %errorlevel%
)

cmake --build .

if errorlevel 1 (
	exit /b %errorlevel%
)
