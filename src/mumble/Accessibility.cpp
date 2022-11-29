// Copyright 2024 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Accessibility.h"

#include <QAbstractButton>
#include <QObject>

namespace Mumble {
namespace Accessibility {

	void setDescriptionFromLabel(QWidget *widget, const QLabel *label) {
		widget->setAccessibleDescription(label->text().remove(QRegExp("<[^>]*>")));
	}

	void fixWizardButtonLabels(QWizard *wizard) {
		wizard->button(QWizard::NextButton)->setAccessibleName(QObject::tr("Next"));
		wizard->button(QWizard::BackButton)->setAccessibleName(QObject::tr("Back"));
	}

} // namespace Accessibility
} // namespace Mumble
