// Copyright 2024 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ACCESSIBILITY_H_
#define MUMBLE_MUMBLE_ACCESSIBILITY_H_

#include <QLabel>
#include <QWidget>

namespace Mumble {
namespace Accessibility {

	void setDescriptionFromLabel(QWidget *widget, const QLabel *label);

} // namespace Accessibility
} // namespace Mumble

#endif // MUMBLE_MUMBLE_ACCESSIBILITY_H_
