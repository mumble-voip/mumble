// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ACCESSIBILITY_H_
#define MUMBLE_MUMBLE_ACCESSIBILITY_H_

#include "Channel.h"
#include "ClientUser.h"
#include "widgets/SemanticSlider.h"

#include <QLabel>
#include <QSlider>
#include <QString>
#include <QWidget>
#include <QWizard>

namespace Mumble {
namespace Accessibility {

	enum class SliderMode { NONE, READ_RELATIVE, READ_PERCENT, READ_ABSOLUTE };

	QString removeHTMLTags(QString value);

	void setDescriptionFromLabel(QWidget *widget, const QLabel *label);
	void fixWizardButtonLabels(QWizard *wizard);

	QString userToText(const ClientUser *user);
	QString userToDescription(const ClientUser *user);
	QString channelToText(const Channel *channel);
	QString channelToDescription(const Channel *channel);

	void setSliderSemanticValue(SemanticSlider *slider, QString value);
	void setSliderSemanticValue(SemanticSlider *slider, SliderMode mode, QString suffix = "");

	QWidget *getFirstFocusableChild(QObject *object);

} // namespace Accessibility
} // namespace Mumble

#endif // MUMBLE_MUMBLE_ACCESSIBILITY_H_
