// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_DIALOGS_POSITIONALAUDIOVIEWER_H_
#define MUMBLE_MUMBLE_DIALOGS_POSITIONALAUDIOVIEWER_H_

#include "PositionalData.h"

#include <memory>

#include <QDialog>
#include <QTimer>

namespace Ui {
class PositionalAudioViewer;
}

class PositionalAudioViewer : public QDialog {
public:
	PositionalAudioViewer();
	~PositionalAudioViewer();

protected:
	void update();
	void updatePlayer(const PositionalData &data);
	void updateCamera(const PositionalData &data);

	QTimer m_timer;
	std::unique_ptr< Ui::PositionalAudioViewer > m_ui;

private:
	Q_OBJECT
};

#endif
