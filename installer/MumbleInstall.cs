// Copyright 2020-2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

using Microsoft.Win32;
using System;
using System.Diagnostics;
using WixSharp;

// base class with info across installers
public class MumbleInstall : ManagedProject {
	public string Arch {
		get; set;
	}
	
	public MumbleInstall() {
		var allUsersProp = new Property("ALLUSERS", "1");
		this.Language = "en-US,cs-CZ,da-DK,de-DE,el-GR,es-ES,fi-FI,fr-FR,it-IT,ja-JP,nb-NO,nl-NL,pl-PL,pt-PT,ru-RU,sv-SE,tr-TR,zh-CN,zh-TW";
		this.MajorUpgradeStrategy = MajorUpgradeStrategy.Default;
		this.MajorUpgradeStrategy.RemoveExistingProductAfter = Step.InstallInitialize;
		this.PreserveTempFiles = true;
		this.BackgroundImage = @"..\dlgbmp.bmp";
		this.BannerImage = @"..\bannrbmp.bmp";
		this.LicenceFile = @"..\..\licenses\Mumble.rtf";
		this.UI = WUI.WixUI_Minimal;
		this.ControlPanelInfo.Comments = "Mumble is a free, open source, low latency, high quality voice chat application.";
		this.ControlPanelInfo.Manufacturer = "Mumble VoIP";
		this.ControlPanelInfo.ProductIcon = @"..\icons\mumble.ico";
		this.ControlPanelInfo.UrlInfoAbout = "https://mumble.info";
		this.Properties = new Property[] { allUsersProp };
		this.AfterInstall += Mumblex86_AfterInstall;
		if(this.Arch == "x64") {
			this.AfterInstall += Mumblex64_AfterInstall;
		}
		this.ValidateBackgroundImage = false;
	}
	
	public static void Mumblex86_AfterInstall(SetupEventArgs e) {
		if(e.IsInstalling) {
			ExecuteFileFromWeb("https://aka.ms/vs/17/release/vc_redist.x86.exe", 
								Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\Temp\vc_redist.x86.exe");
		}
	}
	
	public static void Mumblex64_AfterInstall(SetupEventArgs e) {
		if(e.IsInstalling) {
			ExecuteFileFromWeb("https://aka.ms/vs/17/release/vc_redist.x64.exe", 
								Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData) + @"\Temp\vc_redist.x64.exe");
		}
	}
	
	static void ExecuteFileFromWeb(string uri, string path) {
		if(System.IO.File.Exists(path)) {
			System.IO.File.Delete(path);
		}
		using(var wc = new System.Net.WebClient()) {
			wc.DownloadFile(uri, path);
		}
		Process.Start(path);
	}
}
