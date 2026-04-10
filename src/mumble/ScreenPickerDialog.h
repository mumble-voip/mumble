// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENPICKERDIALOG_H_
#define MUMBLE_MUMBLE_SCREENPICKERDIALOG_H_

#ifdef USE_SCREEN_SHARING

#	include "CaptureSource.h"

#	include <QtCore/QList>
#	include <QtWidgets/QDialog>

class QListWidget;
class QListWidgetItem;

/// Modal dialog that lists available screens and windows for screen sharing.
/// Call exec(); if accepted, use selectedSource() to retrieve the chosen source.
class ScreenPickerDialog : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(ScreenPickerDialog)

public:
	explicit ScreenPickerDialog(QWidget *parent = nullptr);

	/// Returns the source selected by the user, or a default-constructed CaptureSource
	/// (EntireScreen, screenIndex 0) if nothing was explicitly selected.
	CaptureSource selectedSource() const;

private slots:
	void onItemDoubleClicked(QListWidgetItem *item);

private:
	QListWidget *m_list;
	QList< CaptureSource > m_sources;
};

#endif // USE_SCREEN_SHARING
#endif // MUMBLE_MUMBLE_SCREENPICKERDIALOG_H_
