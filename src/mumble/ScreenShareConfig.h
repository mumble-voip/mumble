// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREENSHARECONFIG_H_
#define MUMBLE_MUMBLE_SCREENSHARECONFIG_H_

#include "ConfigDialog.h"

#include "ui_ScreenShareConfig.h"

class ScreenShareConfig : public ConfigWidget, Ui::ScreenShareConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ScreenShareConfig)

	/// Encoder picked for manual mode in the loaded settings. Kept even if it isn't available on this system.
	QString m_manualEncoder;
	/// Whether the available encoders are known, i.e. probing them has finished
	bool m_encodersKnown = false;

	/// Fills the encoder list with the available encoders and selects m_manualEncoder.
	void fillEncoders();

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	ScreenShareConfig(Settings &st);
	QString title() const Q_DECL_OVERRIDE;
	const QString &getName() const Q_DECL_OVERRIDE;
	QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
	void on_qcbEncoderMode_currentIndexChanged(int index);
};

#endif
