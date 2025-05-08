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

	public Bundle BundleMsi(string msiPath, string vcRedistUrl, string vcRedistRequired) {
		var bootstrapper = new Bundle(
				this.Name,
				new MsiPackage(msiPath)
				{ },
				new ExePackage()
				{
					Id = "VCREDIST_EXE",
					Name = "VC_redist.x64.exe",
					/* The SourceFile is required used by wix to generate a RemotePayload object for
					 * this ExePackage. It inludes a sha1 hash of the SourceFile in the installer we
					 * build to validate the download at install time on the user's machine. */
					SourceFile = @"..\VC_redist.x64.exe",
					DisplayName = "Microsoft Visual C++ 2015-2022 Redistributable (x64)",
					DownloadUrl = vcRedistUrl,
					/* This condition being true should mean that it is already installed. */
					DetectCondition = "VCREDIST_INSTALLED >= VCREDIST_REQUIRED",
					InstallCommand = "/install /quiet /norestart /ChainingPackage \"[WixBundleName]\"",
					/* I don't think this has anything to do with compression.
					 * A true value means the exe is "embedded", false means its "external".
					 * Since this is a download, it is not embedded in the bundle. */
					Compressed = false,
					/* By default, if the installer does not have an internet connection, the download
					 * will fail and the entire install will fail. Setting Vital to false means that
					 * the install for the redistributables can fail silently and Mumble's install
					 * will continue. */
					Vital = false,
					/* Permanent just means it won't uninstall this when uninstalling Mumble.
					 * This will still show up in Add/Remove Programs and can be removed there,
					 * even when Permanent is true. */
					Permanent = true,
				});
		bootstrapper.Variables = new[] {
				/* Version.ToString() is here to validating the input when the installer is built.
				 * Otherwise, the installer can fail silently at runtime and that'harder to debug. */
				new Variable("VCREDIST_REQUIRED", new Version(vcRedistRequired).ToString())
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
