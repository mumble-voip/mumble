// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_ABOUT_H_
#define MUMBLE_MURMUR_ABOUT_H_

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#if QT_VERSION >= 0x050000
# include <QtWidgets/QDialog>
#else
# include <QtGui/QDialog>
#endif

enum AboutDialogOptions {
	AboutDialogOptionsShowAbout,
	AboutDialogOptionsShowLicense,
	AboutDialogOptionsShowAuthors,
	AboutDialogOptionsShowThirdPartyLicenses,

	AboutDialogOptionsDefault = AboutDialogOptionsShowAbout
};

class AboutDialog : public QDialog {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AboutDialog)
	public:
		AboutDialog(QWidget *parent, AboutDialogOptions options = AboutDialogOptionsDefault);
};

#endif
