// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ABOUT_H_
#define MUMBLE_MUMBLE_ABOUT_H_

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#if QT_VERSION >= 0x050000
# include <QtWidgets/QDialog>
#else
# include <QtGui/QDialog>
#endif

class AboutDialog : public QDialog {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(AboutDialog)
	public:
		AboutDialog(QWidget *parent);
};

#endif
