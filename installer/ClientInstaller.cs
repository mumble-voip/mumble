// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

//css_ref MumbleInstall.dll
//css_ref Wix_bin\SDK\Microsoft.Deployment.WindowsInstaller.dll;

using System;
using System.Linq;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Microsoft.Deployment.WindowsInstaller;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;
using WixSharp;
using WixSharp.Bootstrapper;
using WixSharp.CommonTasks;

public struct Features {
	public bool overlay;
	public bool g15;
	public bool rnnoise;
	public bool screenShareHelper;
}

public class ClientInstaller : MumbleInstall {

	public const string s_Name = "Mumble (client)";
	public const string s_UpgradeGuid = "D269FC55-4F2C-4285-9AA9-4D034AF305C4";

	public static bool IsVersionString(string value) {
		return !string.IsNullOrWhiteSpace(value) && Regex.IsMatch(value, @"^([0-9]+\.){2,3}[0-9]+$");
	}

	public static string GetMSIPath(string artifactVersion, string arch) {
		return System.IO.Path.GetFullPath(System.IO.Path.Combine(Environment.CurrentDirectory, "mumble_client-" + new Version(artifactVersion) + "-" + arch) + ".msi");
	}

	private static void PopulatePayloadTree(Dir targetDir, string sourceRoot, string relativePath = "") {
		string currentPath = string.IsNullOrWhiteSpace(relativePath) ? sourceRoot : System.IO.Path.Combine(sourceRoot, relativePath);
		if (!System.IO.Directory.Exists(currentPath)) {
			throw new System.IO.DirectoryNotFoundException("Payload directory was not found: " + currentPath);
		}

		var files = System.IO.Directory
			.GetFiles(currentPath)
			.OrderBy(filePath => filePath, StringComparer.OrdinalIgnoreCase)
			.Select(filePath => {
				string fileName = System.IO.Path.GetFileName(filePath);
				if (string.IsNullOrWhiteSpace(relativePath) && fileName.Equals("mumble.exe", StringComparison.OrdinalIgnoreCase)) {
					return new File(new Id("mumble.exe"), filePath, new FileAssociation("mumble_plugin", "application/mumble", "Open", "\"%1\""));
				}

				return new File(filePath);
			})
			.ToArray();

		var childDirs = System.IO.Directory
			.GetDirectories(currentPath)
			.OrderBy(dirPath => dirPath, StringComparer.OrdinalIgnoreCase)
			.Select(dirPath => {
				string dirName = System.IO.Path.GetFileName(dirPath);
				var childDir = new Dir(dirName);
				PopulatePayloadTree(childDir, sourceRoot, string.IsNullOrWhiteSpace(relativePath)
					? dirName
					: System.IO.Path.Combine(relativePath, dirName));
				return childDir;
			})
			.ToArray();

		targetDir.Files = files;
		targetDir.Dirs = childDirs;
	}

	public ClientInstaller(string artifactVersion, string installerVersion, string arch, Features features, string payloadRoot = "") {
		List<string> binaries = new List<string>();
		string[] plugins = {
			"amongus.dll",
			"aoc.dll",
			"arma2.dll",
			"bf1.dll",
			"bf2.dll",
			"bf3.dll",
			"bf4.dll",
			"bf4_x86.dll",
			"bf1942.dll",
			"bf2142.dll",
			"bfbc2.dll",
			"bfheroes.dll",
			"blacklight.dll",
			"borderlands.dll",
			"borderlands2.dll",
			"breach.dll",
			"cod2.dll",
			"cod4.dll",
			"cod5.dll",
			"codmw2.dll",
			"codmw2so.dll",
			"cs.dll",
			"dys.dll",
			"etqw.dll",
			"ffxiv.dll",
			"ffxiv_x64.dll",
			"gmod.dll",
			"grounded.dll",
			"gtaiv.dll",
			"gtasa.dll",
			"gtav.dll",
			"gw.dll",
			"insurgency.dll",
			"jc2.dll",
			"link.dll",
			"lol.dll",
			"lotro.dll",
			"ql.dll",
			"rl.dll",
			"se.dll",
			"sr.dll",
			"ut3.dll",
			"ut99.dll",
			"ut2004.dll",
			"wolfet.dll",
			"wow.dll",
			"wow_x64.dll"
		};

		string[] licenses = {
			"qt.txt",
			"portaudio.txt",
			"gpl.txt",
			"speex.txt",
			"lgpl.txt",
			"Mumble.rtf"
		};

		if (arch == "x64") {
			// 64 bit
			this.Platform = WixSharp.Platform.x64;
			binaries = new List<string>() {
				"speexdsp.dll",
				"mumble.exe",
			};

			if (features.rnnoise) {
				binaries.Add("rnnoise.dll");
			}

			if (features.screenShareHelper) {
				binaries.Add("mumble-screen-helper.exe");
			}

			if (features.overlay) {
				binaries.Add("mumble_ol.dll");
				binaries.Add("mumble_ol_helper.exe");
				binaries.Add("mumble_ol_helper_x64.exe");
				binaries.Add("mumble_ol_x64.dll");
			}

			if (features.g15) {
				binaries.Add("mumble-g15-helper.exe");
			}
		} else if (arch == "x86") {
			// 32 bit
			this.Platform = WixSharp.Platform.x86;
			binaries = new List<string>() {
				"speexdsp.dll",
				"mumble.exe",
			};

			if (features.rnnoise) {
				binaries.Add("rnnoise.dll");
			}

			if (features.screenShareHelper) {
				binaries.Add("mumble-screen-helper.exe");
			}

			if (features.overlay) {
				binaries.Add("mumble_ol.dll");
				binaries.Add("mumble_ol_helper.exe");
			}

			if (features.g15) {
				binaries.Add("mumble-g15-helper.exe");
			}
		}

		var normalizedArtifactVersion = new Version(artifactVersion);
		var normalizedInstallerVersion = new Version(installerVersion);

		this.Name = ClientInstaller.s_Name;
		this.UpgradeCode = Guid.Parse(ClientInstaller.s_UpgradeGuid);
		this.Version = normalizedInstallerVersion;
		this.OutFileName = "mumble_client-" + normalizedArtifactVersion + "-" + arch;
		this.Media.First().Cabinet = "Mumble.cab";

		var progsDir = new Dir(@"%ProgramFiles%");
		var productDir = new Dir("Mumble");
		var installDir = new Dir("client");
		var pluginDir = new Dir("plugins");
		var licenseDir = new Dir("licenses");
		var menuDir = new Dir(@"%ProgramMenu%");
		var desktopDir = new Dir(@"%Desktop%");
		var shortcutDir = new Dir("Mumble");
		var menuShortcut = new ExeFileShortcut("Mumble", "[INSTALLDIR]mumble.exe", arguments: "");
		var deskShortcut = new ExeFileShortcut("Mumble", "[INSTALLDIR]mumble.exe", arguments: "");

		deskShortcut.IconFile = @"..\icons\mumble.ico";
		menuShortcut.IconFile = @"..\icons\mumble.ico";

		shortcutDir.Shortcuts = new ExeFileShortcut[] { menuShortcut };
		desktopDir.Shortcuts = new ExeFileShortcut[] { deskShortcut };

		if (!string.IsNullOrWhiteSpace(payloadRoot)) {
			PopulatePayloadTree(installDir, System.IO.Path.GetFullPath(payloadRoot));
		} else {
			var binaryFiles = new File[binaries.Count];
			var licenseFiles = new File[licenses.Length];
			var pluginFiles = new File[plugins.Length];

			for (int i = 0; i < binaries.Count; i++) {
				if (binaries[i] == "mumble.exe") {
					binaryFiles[i] = new File(new Id("mumble.exe"), @"..\..\" + binaries[i], new FileAssociation("mumble_plugin", "application/mumble", "Open", "\"%1\""));
				} else {
					binaryFiles[i] = new File(@"..\..\" + binaries[i]);
				}
			}

			for (int i = 0; i < licenses.Length; i++) {
				licenseFiles[i] = new File(@"..\..\licenses\" + licenses[i]);
			}

			for (int i = 0; i < plugins.Length; i++) {
				pluginFiles[i] = new File(@"..\..\plugins\" + plugins[i]);
			}

			installDir.Files = binaryFiles;
			licenseDir.Files = licenseFiles;
			pluginDir.Files = pluginFiles;
		}

		menuDir.Dirs = new Dir[] { shortcutDir };
		if (string.IsNullOrWhiteSpace(payloadRoot)) {
			installDir.Dirs = new Dir[] { licenseDir, pluginDir };
		}
		productDir.Dirs = new Dir[] { installDir };
		progsDir.Dirs = new Dir[] { productDir};

		this.Dirs = new Dir[] { 
			progsDir,
			menuDir,
			desktopDir
		};
		this.RegValues = new RegValue[] {
			new RegValue(RegistryHive.ClassesRoot, "mumble", "", "URL:Mumble"),
			new RegValue(RegistryHive.ClassesRoot, "mumble", "URL Protocol", ""),
			new RegValue(RegistryHive.ClassesRoot, @"mumble\DefaultIcon", "", "[#mumble.exe]"),
			new RegValue(RegistryHive.ClassesRoot, @"mumble\shell\open\command", "", "[#mumble.exe] \"%1\"")
		};
	}
}

class BuildInstaller 
{
	public static void Main(string[] args) {
		string version = "";
		string installerVersion = "";
		string arch = "";
		string vcRedistRequired = "";
		bool isAllLangs = false;
		Features features = new Features();
		bool skipMSIRebuild = false;
		string payloadRoot = "";

		for (int i = 0; i < args.Length; i++) {
			if (args[i] == "--version" && i + 1 < args.Length && ClientInstaller.IsVersionString(args[i + 1])) {
				version = args[i + 1];
			}

			if (args[i] == "--installer-version" && i + 1 < args.Length && ClientInstaller.IsVersionString(args[i + 1])) {
				installerVersion = args[i + 1];
			}

			if (args[i] == "--arch" && (args[i + 1] == "x64" || args[i + 1] == "x86")) {
				arch = args[i + 1];
			}

			if (args[i] == "--all-languages") {
				isAllLangs = true;
			}

			if (args[i] == "--vc-redist-required") {
				vcRedistRequired = args[i + 1];
			}

			if (args[i] == "--g15") {
				features.g15 = true;
			}

			if (args[i] == "--overlay") {
				features.overlay = true;
			}

			if (args[i] == "--rnnoise") {
				features.rnnoise = true;
			}

			if (args[i] == "--screen-share-helper") {
				features.screenShareHelper = true;
			}

			if (args[i] == "--skip-msi-rebuild") {
				skipMSIRebuild = true;
			}

			if (args[i] == "--payload-root" && i + 1 < args.Length) {
				payloadRoot = args[i + 1];
			}
		}

		if (string.IsNullOrWhiteSpace(installerVersion)) {
			installerVersion = version;
		}

		if (!string.IsNullOrWhiteSpace(version) && !string.IsNullOrWhiteSpace(installerVersion) && !string.IsNullOrWhiteSpace(arch)) {
			string msiPath;

			if (!skipMSIRebuild) {
				var clInstaller = new ClientInstaller(version, installerVersion, arch, features, payloadRoot);
				msiPath = isAllLangs
							? clInstaller.BuildMultilanguageMsi()
							: clInstaller.BuildMsi();
			} else {
				Console.WriteLine("INFO - Skipping MSI rebuild.");
				msiPath = ClientInstaller.GetMSIPath(version, arch);
			}

			MumbleInstall.BundleMsi(ClientInstaller.s_Name, Guid.Parse(ClientInstaller.s_UpgradeGuid), msiPath, vcRedistRequired)
			           .Build(msiPath.PathChangeExtension(".exe"));
		} else {
			Console.WriteLine("ERROR - Values for arch or version are null or incorrect!");
			Environment.ExitCode = 0xA0; // Bad argument
		}
	}
}
