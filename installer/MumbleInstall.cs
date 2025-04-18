// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

using System;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;
using WixSharp;
using WixSharp.Bootstrapper;
using WixSharp.CommonTasks;

// base class with info across installers
public class MumbleInstall : Project {
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
	}

	public Bundle BundleMsi(string msiPath) {
		var bootstrapper = new Bundle(
				this.Name,
				new MsiPackage(msiPath)
				{ },
				new ExePackage()
				{
					Id = "VCREDIST_EXE",
					Name = "VC_redist.x64.exe",
					SourceFile = @"..\VC_redist.x64.exe",
					DisplayName = "Microsoft Visual C++ 2015-2022 Redistributable (x64)",
					/* I couldn't find a page from Microsoft that lists permalinks to particular versions
					 * of the redistributables. I could only find a link that redirects you to a link for
					 * the latest redistributable version. The page is at [1] and the link is [2].
					 *
					 * The VC_redist.x64.exe downloaded from the `DownloadUrl` value below should be
					 * placed in the build/installer/client folder, since WixSharp will use the file
					 * to include a hash in the installer to verify the download at the time of install.
					 *
					 * The URL in DownloadUrl is for version 14.42.34438.0
					 *
					 * [1] https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version
					 * [2] https://aka.ms/vs/17/release/vc_redist.x64.exe */
					DownloadUrl = "https://download.visualstudio.microsoft.com/download/pr/285b28c7-3cf9-47fb-9be8-01cf5323a8df/8F9FB1B3CFE6E5092CF1225ECD6659DAB7CE50B8BF935CB79BFEDE1F3C895240/VC_redist.x64.exe",
					/* This condition being true should mean that it is already installed. */
					DetectCondition = "VCREDIST_INSTALLED >= VCREDIST_REQUIRED",
					InstallCommand = "/install /quiet /norestart /ChainingPackage \"[WixBundleName]\"",
					/* I don't think this has anything to do with compression.
					 * A true value means the exe is "embedded", false means its "external".
					 * Since this is a download, it is not embedded in the bundle. */
					Compressed = false,
					/* By default, if the installer does not have an internet connection, the download
					 * will fail and the entire install will fail. Setting Vital to false means that
					 * if the install for the redistributables can fail silently and Mumble's install
					 * will continue. */
					Vital = false,
					/* Permanent just means it won't uninstall this when uninstalling Mumble.
					 * This will still show up in Add/Remove Programs and can be removed there,
					 * even when Permanent is true. */
					Permanent = true,
				});
		bootstrapper.Variables = new[] {
				new Variable("VCREDIST_REQUIRED", "14.0.0.0")
				{ Type = WixSharp.VariableType.version }
				};
		// Visual C++ 14 2015-2022 Redistributable (x64)
		bootstrapper.AddWixFragment(
				"Wix/Bundle",
				new UtilProductSearch
				{
					UpgradeCode = "{36F68A90-239C-34DF-B58C-64B30153CE35}",
					Variable = "VCREDIST_INSTALLED"
				});

		bootstrapper.SetVersionFromFile(msiPath);
		bootstrapper.UpgradeCode = this.UpgradeCode ?? Guid.Empty;
		/* The options UI allows specifying an alternate install path,
		 * SuppressOptionsUI hides the button for that screen. */
		bootstrapper.Application.SuppressOptionsUI = true;
		bootstrapper.Application.LicensePath = @"..\..\licenses\Mumble.rtf";
		bootstrapper.Application.LogoFile = @"..\dlgbmp.bmp";
		bootstrapper.Application.ThemeFile = @"..\Theme.xml";
		bootstrapper.Include(WixExtension.Util);
		return bootstrapper;
	}
}
