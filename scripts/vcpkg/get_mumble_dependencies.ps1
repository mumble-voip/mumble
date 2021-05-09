# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

$profiledir = $Env:USERPROFILE 
$vcpkgdir = $profiledir + "\vcpkg"

$mumble_deps = "qt5-base",
               "qt5-svg",
               "qt5-tools",
               "qt5-translations",
               "grpc",
               "boost-accumulators",
               "poco",
               "libvorbis",
               "libogg",
               "libflac",
               "libsndfile",
               "libmariadb",
               "mdnsresponder",
               "zlib", 
               "zeroc-ice"

$ErrorActionPreference = 'Stop'

function vcpkg_install {
	Param(
		[string[]] $packages,

		[string] $targetTriplet,
		[switch] $cleanAfterBuild = $false
	)
	
	if ($cleanAfterBuild) {
		./vcpkg.exe install $packages --triplet $targetTriplet --clean-after-build
	} else {
		./vcpkg.exe install $packages --triplet $targetTriplet
	}
	
	if (-not $?) {
		Write-Error("Failed at installing package $package ($targetTriplet)")
	}
}

$prevDir=pwd

try {
	Write-Host "Setting triplets for $Env:PROCESSOR_ARCHITECTURE"
	if ($Env:PROCESSOR_ARCHITECTURE -eq "AMD64") {
		$triplet = "x64-windows-static-md"
		$xcompile_triplet = "x86-windows-static-md"
	} else {
		$triplet = "x86-windows-static-md"
	}

	Write-Host "Checking for $vcpkgdir..."
	if (-not (Test-Path $vcpkgdir)) {
		git clone https://github.com/Microsoft/vcpkg.git $vcpkgdir
	}

	if (Test-Path $vcpkgdir) {
		if (-not (Test-Path $vcpkgdir/ports/zeroc-ice)) {
			Write-Host "Adding ports for ZeroC Ice..."
			Copy-Item -Path $PSScriptRoot/../../helpers/vcpkg/ports/zeroc-ice -Destination $vcpkgdir/ports -Recurse
		}
		
		cd $vcpkgdir

		if (-not (Test-Path -LiteralPath $vcpkgdir/vcpkg.exe)) {
			Write-Host "Installing vcpkg..."
			./bootstrap-vcpkg.bat -disableMetrics
		}

		if ($Env:PROCESSOR_ARCHITECTURE -eq "AMD64") {
			Write-Host "Installing cross compile packages..."
			vcpkg_install -package boost-optional -targetTriplet $xcompile_triplet -cleanAfterBuild
		}

		Write-Host "Beginning package install..."

		vcpkg_install -package $mumble_deps -targetTriplet $triplet -cleanAfterBuild
	}
} catch {
	# rethrow
	throw $_
} finally {
	# restore previous directory
	cd $prevDir
}
