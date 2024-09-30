:: Copyright The Mumble Developers. All rights reserved.
:: Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file at the root of the
:: Mumble source tree or at <https://www.mumble.info/LICENSE>.
::
:: Builds the installer and moves all binaries to the correct
:: directory in order for the publishing of artifacts to work
:: as expected.

@echo on

cd /d %BUILD_BINARIESDIRECTORY%

cmake --install .

if errorlevel 1 (
	exit /b %errorlevel%
)

copy installer\client\mumble_client*.msi %BUILD_ARTIFACTSTAGINGDIRECTORY%

copy installer\server\mumble_server*.msi %BUILD_ARTIFACTSTAGINGDIRECTORY%

7z a PDBs.7z *.pdb plugins\*.pdb
copy PDBs.7z %BUILD_ARTIFACTSTAGINGDIRECTORY%
