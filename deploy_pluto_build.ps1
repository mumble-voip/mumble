$keyPrefix = Get-Date -format "yyyyMMdd"
$release = ".\release"
foreach($extension in "*.exp", "*.pdb", "*.lib") {
	get-childitem $release -include $extension -recurse | foreach ($_) {remove-item $_.fullname}
}

Write-S3Object -BucketName plutovr/mumble-releases -Folder $release -Recurse -KeyPrefix $keyPrefix\
