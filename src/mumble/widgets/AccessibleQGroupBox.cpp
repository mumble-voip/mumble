// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AccessibleQGroupBox.h"

#include "Accessibility.h"

#include <QFormLayout>
#include <QLabel>
#include <QLayoutItem>

AccessibleQGroupBox::AccessibleQGroupBox(QWidget *parent) : QGroupBox(parent) {
	setFocusPolicy(Qt::TabFocus);
	updateAccessibleText();
}

QString AccessibleQGroupBox::textAtPosition(QGridLayout *gridLayout, int y, int x) {
	QLayoutItem *item = gridLayout->itemAtPosition(y, x);
	if (!item) {
		return "";
	}

	QLabel *label = qobject_cast< QLabel * >(item->widget());
	if (!label || !label->isVisible()) {
		return "";
	}

	QString accessibleName = label->accessibleName();
	if (!accessibleName.isEmpty()) {
		return accessibleName;
	}

	QString content = Mumble::Accessibility::removeHTMLTags(label->text());
	if (content.trimmed().isEmpty()) {
		content = tr("empty");
	}

	return content;
}

void AccessibleQGroupBox::updateAccessibleText() {
	QString text;

	// If we have a grid layout, we iterate over it top-to-bottom left-to-right
	// and concat all the label contents.
	QGridLayout *gridLayout = qobject_cast< QGridLayout * >(layout());
	if (gridLayout) {
		bool tableMode = gridLayout->itemAtPosition(0, 0) == nullptr;

		for (int y = tableMode ? 1 : 0; y < gridLayout->rowCount(); y++) {
			for (int x = tableMode ? 1 : 0; x < gridLayout->columnCount(); x++) {
				if (tableMode) {
					QString cellContent = textAtPosition(gridLayout, y, x);
					if (!cellContent.isEmpty()) {
						text += textAtPosition(gridLayout, y, 0);
						text += " ";
						text += textAtPosition(gridLayout, 0, x);
						text += ", ";
						text += cellContent;
					}
				} else {
					text += textAtPosition(gridLayout, y, x);
				}

				text += ", ";
			}
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
			QLayoutItem *item;

			item = formLayout->itemAt(y, QFormLayout::SpanningRole);
			if (item) {
				label = qobject_cast< QLabel * >(item->widget());
				if (label && label->isVisible()) {
					text += Mumble::Accessibility::removeHTMLTags(label->text()) + ", ";
					continue;
				}
			}

			item = formLayout->itemAt(y, QFormLayout::LabelRole);
			if (item) {
				label = qobject_cast< QLabel * >(item->widget());
				if (label && label->isVisible()) {
					text += Mumble::Accessibility::removeHTMLTags(label->text());
				}
			}

			item = formLayout->itemAt(y, QFormLayout::FieldRole);
			if (item) {
				label = qobject_cast< QLabel * >(item->widget());
				if (label && label->isVisible()) {
					QString content = Mumble::Accessibility::removeHTMLTags(label->text());
					if (content.trimmed().isEmpty()) {
						content = "empty";
					}
					text += " " + content;
				}
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
		if (!label || !label->isVisible()) {
			continue;
		}

		text += label->text() + ", ";
	}
	setAccessibleDescription(text);
}
