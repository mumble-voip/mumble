:: Copyright 2020 The Mumble Developers. All rights reserved.
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
:: Predefined variables:
::
::  BUILD_BINARIESDIRECTORY      - The local path on the agent that can be used
::                                 as an output folder for compiled binaries.
::  BUILD_SOURCESDIRECTORY       - The local path on the agent where the
::                                 repository is downloaded.
::  AGENT_TOOLSDIRECTORY         - The directory used by tasks such as
::                                 Node Tool Installer and Use Python Version
::                                 to switch between multiple versions of a tool.
::                                 We store Wix# there, in "WixSharp".
::
:: Defined in .azure-pipelines.yml:
::
::  MUMBLE_ENVIRONMENT_PATH      - Path to the environment's root directory.
::  MUMBLE_ENVIRONMENT_TOOLCHAIN - Path to the vcpkg CMake toolchain, used for CMake.
::  MUMBLE_ENVIRONMENT_TRIPLET   - vcpkg triplet.
::  VCVARS_PATH                  - Path to the Visual Studio environment initialization script.
::
:: Defined on Azure Pipelines:
::
::  MUMBLE_BUILD_NUMBER_TOKEN           - Access token for the build number page on our server.
::

@echo on

if defined MUMBLE_BUILD_NUMBER_TOKEN (
	:: The method we use to store a command's output into a variable:
	:: https://stackoverflow.com/a/6362922
	for /f "tokens=* USEBACKQ" %%g in (`python "scripts\mumble-version.py" --project`) do (set "VERSION=%%g")
	for /f "tokens=* USEBACKQ" %%g in (`curl "https://mumble.info/get-build-number?version=%VERSION%_%AGENT_JOBNAME%&token=%MUMBLE_BUILD_NUMBER_TOKEN%"`) do (set "BUILD_NUMBER=%%g")
) else (
	set BUILD_NUMBER=0
)

for /f "tokens=* USEBACKQ" %%g in (`python "scripts/mumble-version.py"`) do (set "RELEASE_ID=%%g")

cd /d %BUILD_BINARIESDIRECTORY%

call "%VCVARS_PATH%"

set PATH=%PATH%;%AGENT_TOOLSDIRECTORY%\WixSharp

:: Delete MinGW, otherwise CMake picks it over MSVC.
:: We don't delete the (Chocolatey) packages because it takes ~10 minutes...
del C:\ProgramData\chocolatey\bin\gcc.exe
del C:\ProgramData\chocolatey\bin\g++.exe
del C:\ProgramData\chocolatey\bin\c++.exe
del C:\Strawberry\c\bin\gcc.exe
del C:\Strawberry\c\bin\g++.exe
del C:\Strawberry\c\bin\c++.exe

cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE="%MUMBLE_ENVIRONMENT_TOOLCHAIN%" -DVCPKG_TARGET_TRIPLET=%MUMBLE_ENVIRONMENT_TRIPLET% ^
      -DIce_HOME="%MUMBLE_ENVIRONMENT_PATH%\installed\%MUMBLE_ENVIRONMENT_TRIPLET%" ^
      -DCMAKE_BUILD_TYPE=Release -DRELEASE_ID=%RELEASE_ID% -DBUILD_NUMBER=%BUILD_NUMBER% ^
      -Dpackaging=ON -Dtests=ON -Donline-tests=ON -Dstatic=ON -Dsymbols=ON -Dgrpc=ON -Dasio=ON -Dg15=ON ^
      -Ddisplay-install-paths=ON "%BUILD_SOURCESDIRECTORY%"

if errorlevel 1 (
	exit /b %errorlevel%
)

cmake --build .

if errorlevel 1 (
	exit /b %errorlevel%
)

:: Set timeout for tests to 15min
set QTEST_FUNCTION_TIMEOUT=900000
ctest --verbose

if errorlevel 1 (
	exit /b %errorlevel%
)

cmake --install .

if errorlevel 1 (
	exit /b %errorlevel%
)

copy installer\client\mumble_client*.msi %BUILD_ARTIFACTSTAGINGDIRECTORY%

copy installer\server\mumble_server*.msi %BUILD_ARTIFACTSTAGINGDIRECTORY%

7z a PDBs.7z *.pdb plugins\*.pdb
copy PDBs.7z %BUILD_ARTIFACTSTAGINGDIRECTORY%
