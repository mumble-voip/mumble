# Setup Visual Studio

MSVC (Microsoft Visual C++) is Microsoft’s C++ compiler toolchain. It can be installed via the **Visual Studio Build Tools** or **Visual Studio Community**.
Either can be downloaded at [visualstudio.microsoft.com/downloads](https://visualstudio.microsoft.com/downloads) (any somewhat recent version should work).

In the Visual Studio Installer, make sure to select:

*  **C++ build tools** (or Development, respectively)
* "**C++ MFC for latest v14[X] build tools**" (just pick the latest version - e.g.v142).

vcpkg also requires the **English Language pack** which is found in the Language packs section of the Visual Studio Installer.

## Using MSVC

Unlike most macOS or Linux tools, the MSVC build tools don't get added to the PATH by default, so you can't simply call the compiler like `cl.exe`. Instead, you need to run a script (part of Visual Studio) to add them to your PATH.

The easiest way to do this, for a PowerShell window on a 64-bit machine, is to run `vcvars64.bat`:

```powershell
# Based on the version of Visual Studio you have installed, the VC++ tools will be in a specific location.
# 
# You can find them using `vswhere.exe`, which ships with all versions of VS since 2017
# and is always in the same location.
$vswhere = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

# e.g. Visual Studio 2015 path
# "${Env:PROGRAMFILES(X86)}\Microsoft Visual Studio 14.0\VC\"
# e.g. Vistual Studio 2019 path
# "${Env:PROGRAMFILES(X86)}\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\"
$vcvars64 = . $vswhere -latest -products * -find "VC\Auxiliary\Build\vcvars64.bat"

# Load vcvars64.bat into your PowerShell window.
cmd /c "`"$vcvars64`" && set" |
    ForEach-Object {
        if ($_ -match "^([^=]+)=(.*)$") { [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2]) }
    }
```

This updates your current PowerShell window to let you use the MSVC build tools to build for x64. You can add it to your PowerShell profile, or you can simply run it as needed.

See MSDN's [Use the Microsoft C++ Build Tools from the command line](https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170).

### Alternatives

You can also use [prebuilt shortcuts](https://learn.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=msvc-170#developer_command_prompt_shortcuts) like **x64 Native Tools Command Prompt**, which you can find in the Start menu after installing the MSVC.

These have some caveats that recommend using the `vcvars64.bat` or `vcvarsall.bat` techhnique instead: only **x64 Native Tools Command Prompt** supports building in 64-bit, but it is CMD, not PowerShell. The **Developer Powershell** shortcut is 32-bit only. If you are on a 64bit system, then you'll know that you have opened the correct prompt, if it prints `Environment initialized for: 'x64'`.

You can also use `vcvarsall.bat`, with args, instead of `vcvars64.bat`, to set up different environments:

```powershell
# Find the path for `vcvarsall.bat`, as before:
$vswhere = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
$vcvarsall = . $vswhere -latest -products * -find "VC\Auxiliary\Build\vcvarsall.bat"
$vcToolsPath = Split-Path $vcvarsall

# Shift context to VS
Push-Location $vcToolsPath

$architecture = 'x64' # amd64 and x64 resolve to the same path in vcvarsall.bat and are interchangeable.

# Have vcvarsall.bat load the 64 bit environment variables.
cmd /c "vcvarsall.bat $architecture&set" |
foreach {
	if ($_ -match "=") {
		$v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
	}
}
# Go back to where we were before.
Pop-Location
```