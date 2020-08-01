# Copyright 2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

param(
	[Parameter(Mandatory=$true)]$packageName,
	[Parameter(Mandatory=$true)]$version,
	[switch]$allCultures
)

Function Build-Installer {
	[cmdletbinding()]
	Param (
		[string]$culture,
		[string]$lcid
	)

	Process {
		Write-Host "Creating installer for $culture..."
		cpack -C Release -D CPACK_PACKAGE_FILE_NAME="$installerName-$culture" -D CPACK_WIX_CULTURES=$culture -D CPACK_WIX_LCID=$lcid
	}
}

# TODO: Check whether WiX is in PATH?
if(-Not (Test-Path $env:WIX)) {
	Write-Host "Error: \"WIX\" environment variable not found, is WiX installed?"
	exit 1
}

if(-Not (Test-Path -Path "EmbedTransform.exe")) {
	Write-Host "Downloading EmbedTransform from FireGiant (WiX)..."

	try {
		Invoke-WebRequest https://www.firegiant.com/system/files/samples/EmbedTransform.zip -OutFile ".\EmbedTransform.zip"
	} catch {
		Write-Host "URL for FireGiant has been removed or is not available. Aborting..."
		exit 1
	}

	Write-Host "Extracting EmbedTransform archive..."

	try {
		Expand-Archive -Path "EmbedTransform.zip" -DestinationPath "."
	} catch {
		Write-Host "EmbedTransform archive missing or corrupt. Aborting..."
		exit 1
	}

	Remove-Item "EmbedTransform.zip"
}

$mainCulture = "en-US"
if ($allCultures.isPresent) {
	$extraCultures =
		"cs-CZ",
		"da-DK",
		"de-DE",
		"el-GR",
		"es-ES",
		"fi-FI",
		"fr-FR",
		"it-IT",
		"ja-JP",
		"nb-NO",
		"nl-NL",
		"pl-PL",
		"pt-PT",
		"ru-RU",
		"sv-SE",
		"tr-TR",
		"zh-CN",
		"zh-TW"
} else {
	# Only use a few languages in order to save time
	$extraCultures =
		"de-DE",
		"es-ES"
}

$wixBinaryDir = $env:WIX + "bin"
$installerName = "$packageName-$version"
$lcids = New-Object System.Collections.Generic.List[System.Object]

$cultureInfo = New-Object system.globalization.cultureinfo($mainCulture)
$cultureLCID = $cultureInfo.TextInfo.LCID
Build-Installer -culture $mainCulture -lcid $cultureLCID
$lcids.Add($cultureLCID)

# The main culture installer is used as base for the multi-language one.
Copy-Item -Path "$installerName-$mainCulture.msi" "$installerName.msi"

foreach($culture in $extraCultures) {
	$cultureInfo = New-Object system.globalization.cultureinfo($culture)
	$cultureLCID = $cultureInfo.TextInfo.LCID
	Build-Installer -culture $culture -lcid $cultureLCID
	Write-Host "Creating language transform for $culture..."
	& $wixBinaryDir\torch.exe -p -t language "$installerName.msi" "$installerName-$culture.msi" -out $cultureLCID
	Write-Host "Embedding transform for $culture..."
	& .\EmbedTransform.exe "$installerName.msi" $cultureLCID
	$lcids.Add($cultureLCID)
}

if ([Environment]::Is64BitProcess) {
	$platform = "x64"
} else {
	$platform = "Intel"
}

Write-Host "Updating template string..."
# Inspired by https://stackoverflow.com/a/53203626/6658002.
$windowsInstaller = New-Object -ComObject WindowsInstaller.Installer
$msi = $windowsInstaller.OpenDatabase("$PSScriptRoot\$installerName.msi", 2)
$summaryInfo = $msi.SummaryInformation(4)

Write-Host "Original: " $summaryInfo.Property(7)
$lcidsString = [system.String]::Join(",", $lcids)
$summaryInfo.Property(7) = "$platform;$lcidsString"
Write-Host "Updated: " $summaryInfo.Property(7)

$summaryInfo.Persist()
$msi.Commit()

# If we don't release the objects, we cannot interact with the file we opened until PowerShell is closed.
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($summaryInfo) | Out-Null
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($msi) | Out-Null
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($windowsInstaller) | Out-Null

Write-Host "Generating checksum (SHA1)..."
$checksum = Get-FileHash "$installerName.msi" -Algorithm SHA1 | Select-Object -ExpandProperty Hash
$checksumOutput = "$installerName - $checksum"
Out-File -InputObject $checksumOutput -FilePath "$installerName.sha1"

Write-Host "Deleting temporary files..."
foreach($lcid in $lcids) {
	$cultureInfo = New-Object system.globalization.cultureinfo($lcid)
	$culture = $cultureInfo.TextInfo.CultureName
	if (-Not ($culture -eq $mainCulture)) {
		Remove-Item $lcid
	}

	Remove-Item "$installerName-$culture.msi"
}

Write-Host "Done!"
