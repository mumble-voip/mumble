// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

//css_ref MumbleInstall.dll
//css_ref Wix_bin\SDK\Microsoft.Deployment.WindowsInstaller.dll;

using System;
using System.Linq;
using System.Text.RegularExpressions;
using Microsoft.Deployment.WindowsInstaller;
using WixSharp;
using WixSharp.CommonTasks;

public class ServerInstaller : MumbleInstall {
	public ServerInstaller(string version, string arch) {
		string upgradeGuid = "03E9476F-0F75-4661-BFC9-A9DAEB23D3A0";
		string[] binaries = {
			"mumble-server.exe",
			"MumbleServer.ice"
		};

		string[] licenses = {
			"qt.txt",
			"gpl.txt",
			"speex.txt",
			"lgpl.txt",
			"Mumble.rtf"
		};

		if (arch == "x64") {
			// 64 bit
			this.Platform = WixSharp.Platform.x64;
			
		} else if (arch == "x86") {
			// 32 bit
			this.Platform = WixSharp.Platform.x86;
		}

		this.Name = "Mumble Server";
		this.UpgradeCode = Guid.Parse(upgradeGuid);
		this.Version = new Version(version);
		this.OutFileName = "mumble_server-" + this.Version + "-" + arch;
		this.Media.First().Cabinet = "Mumble.cab";

		var progsDir = new Dir(@"%ProgramFiles%");
		var productDir = new Dir("Mumble");
		var installDir = new Dir("server");
		var licenseDir = new Dir("licenses");
		var menuDir = new Dir(@"%ProgramMenu%");
		var shortcutDir = new Dir("Mumble");
		var menuShortcut = new ExeFileShortcut("Mumble Server", "[INSTALLDIR]mumble-server.exe", arguments: "");
		menuShortcut.IconFile = @"..\icons\murmur.ico";
		shortcutDir.Shortcuts = new ExeFileShortcut[] { menuShortcut };
		
		var binaryFiles = new File[binaries.Length];
		var licenseFiles = new File[licenses.Length];
		
		for (int i = 0; i < binaries.Length; i++) {
			binaryFiles[i] = new File(@"..\..\" + binaries[i]);
		}
		
		for (int i = 0; i < licenses.Length; i++) {
			licenseFiles[i] = new File(@"..\..\licenses\" + licenses[i]);
		}

		installDir.Files = binaryFiles;
		licenseDir.Files = licenseFiles;
		
		menuDir.Dirs = new Dir[] { shortcutDir };
		installDir.Dirs = new Dir[] { licenseDir };
		productDir.Dirs = new Dir[] { installDir };
		progsDir.Dirs = new Dir[] { productDir };
		
		this.Dirs = new Dir[] {
			progsDir,
			menuDir
		};
	}
}

class BuildInstaller 
{
	public static void Main(string[] args) {
		string version = "";
		string arch = "";
		bool isAllLangs = false;

		for (int i = 0; i < args.Length; i++) {
			if (args[i] == "--version" && Regex.IsMatch(args[i + 1], @"^([0-9]+\.){2}[0-9]+$")) {
				version = args[i + 1];
			}

			if (args[i] == "--arch" && (args[i + 1] == "x64" || args[i + 1] == "x86")) {
				arch = args[i + 1];
			}

			if (args[i] == "--all-languages") {
				isAllLangs = true;
			}
		}

		if (version != null && arch != null) {
			var srvInstaller = new ServerInstaller(version, arch);
			srvInstaller.Version = new Version(version);

			if (isAllLangs) {
				srvInstaller.BuildMultilanguageMsi();
			} else {
				srvInstaller.BuildMsi();
			}
		} else {
			Console.WriteLine("ERROR - Values for arch or version are null or incorrect!");
			Environment.ExitCode = 0xA0; // Bad argument
		}
	}
}
