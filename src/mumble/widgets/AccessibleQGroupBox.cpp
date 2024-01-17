// Copyright 2024 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AccessibleQGroupBox.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>

AccessibleQGroupBox::AccessibleQGroupBox(QWidget *parent) : QGroupBox(parent) {
	setFocusPolicy(Qt::TabFocus);
	updateAccessibleText();
}

void AccessibleQGroupBox::updateAccessibleText() {
	QString text;

	// If we have a grid layout, we iterate over it top-to-bottom left-to-right
	// and concat all the label contents.
	QGridLayout *gridLayout = qobject_cast< QGridLayout * >(layout());
	if (gridLayout) {
		for (int y = 0; y < gridLayout->rowCount(); y++) {
			for (int x = 0; x < gridLayout->columnCount(); x++) {
				QLabel *label = qobject_cast< QLabel * >(gridLayout->itemAtPosition(y, x)->widget());
				if (!label) {
					continue;
				}

				text += " ";

				QString content = label->text();
				if (content.trimmed().isEmpty()) {
					content = tr("empty");
				}
				text += content;
			}
			text += ",";
		}
		setAccessibleDescription(text);
		return;
	}

	// Form layouts contain rows with 1 or 2 items. It can either contain a
	// "spanning role", or a label with a field.
	QFormLayout *formLayout = qobject_cast< QFormLayout * >(layout());
	if (formLayout) {
		for (int y = 0; y < formLayout->rowCount(); y++) {
			QLabel *label;

			label = qobject_cast< QLabel * >(formLayout->itemAt(y, QFormLayout::SpanningRole)->widget());
			if (label) {
				text += label->text() + ", ";
				continue;
			}

			label = qobject_cast< QLabel * >(formLayout->itemAt(y, QFormLayout::LabelRole)->widget());
			if (label) {
				text += label->text();
			}

			label = qobject_cast< QLabel * >(formLayout->itemAt(y, QFormLayout::FieldRole)->widget());
			if (label) {
				QString content = label->text();
				if (content.trimmed().isEmpty()) {
					content = tr("empty");
				}
				text += " " + content;
			}

			text += ", ";
		}
		setAccessibleDescription(text);
		return;
	}

	// There is no layout we can easily parse, so just concat all
	// child label texts.
	QObjectList childObjects = children();
	for (int i = 0; i < childObjects.length(); i++) {
		QLabel *label = qobject_cast< QLabel * >(childObjects[i]);
		if (!label) {
			continue;
		}

		text += label->text() + ",";
	}
	setAccessibleDescription(text);
}
