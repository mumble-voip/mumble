// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MultiStyleWidgetWrapper.h"

#include <QFont>
#include <QFontMetrics>
#include <QWidget>

const uint32_t MultiStyleWidgetWrapper::UNSET_FONTSIZE = 0;
const QString MultiStyleWidgetWrapper::UNSET_COLOR     = "";
const QString MultiStyleWidgetWrapper::UNSET_SELECTOR  = "*";

MultiStyleWidgetWrapper::MultiStyleWidgetWrapper(QWidget *widget) : m_widget(widget) {
}

void MultiStyleWidgetWrapper::setFontSize(uint32_t fontSize, bool isPixels) {
	if (fontSize != m_fontSize || m_fontSizeInPixels != isPixels) {
		m_fontSize         = fontSize;
		m_fontSizeInPixels = isPixels;

		updateStyleSheet();
	}
}

void MultiStyleWidgetWrapper::setFontSizeSelector(const QString &selector) {
	if (m_fontSizeSelector != selector) {
		m_fontSizeSelector = selector;

		updateStyleSheet();
	}
}

void MultiStyleWidgetWrapper::setBackgroundColor(const QString &color) {
	if (m_backgroundColor != color) {
		m_backgroundColor = color;

		updateStyleSheet();
	}
}

void MultiStyleWidgetWrapper::setBackgroundColorSelector(const QString &selector) {
	if (m_backgroundColorSelector != selector) {
		m_backgroundColorSelector = selector;

		updateStyleSheet();
	}
}

void MultiStyleWidgetWrapper::clearFontSize() {
	setFontSize(UNSET_FONTSIZE);
}

void MultiStyleWidgetWrapper::clearFontSizeSelector() {
	setFontSizeSelector(UNSET_SELECTOR);
}

void MultiStyleWidgetWrapper::clearBackgroundColor() {
	setBackgroundColor(UNSET_COLOR);
}

void MultiStyleWidgetWrapper::clearBackgroundColorSelector() {
	setBackgroundColorSelector(UNSET_SELECTOR);
}

void MultiStyleWidgetWrapper::updateStyleSheet() {
	QString styleSheet;

	if (m_fontSize != UNSET_FONTSIZE) {
		styleSheet += QString(" %1 { font-size: %2%3; }")
						  .arg(m_fontSizeSelector)
						  .arg(m_fontSize)
						  .arg(m_fontSizeInPixels ? QStringLiteral("px") : QStringLiteral("pt"));
	}
	if (m_backgroundColor != UNSET_COLOR) {
		styleSheet += QString(" %1 { background-color: %2; }").arg(m_backgroundColorSelector).arg(m_backgroundColor);
	}

	m_widget->setStyleSheet(styleSheet);
}

QWidget *MultiStyleWidgetWrapper::operator->() {
	return m_widget;
}
