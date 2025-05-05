// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_MULTISTYLEWIDGETWRAPPER_H_
#define MUMBLE_MUMBLE_WIDGETS_MULTISTYLEWIDGETWRAPPER_H_

#include <cstdint>

#include <QString>

class QWidget;

/// Purpose of this class is to work around the fact that normally it is not possible to
/// set single properties within a widget's stylesheet without potentially overwriting other
/// properties set through it.
class MultiStyleWidgetWrapper {
public:
	void setFontSize(uint32_t fontSize, bool isPixels = true);
	void setFontSizeSelector(const QString &selector);
	void setBackgroundColor(const QString &color);
	void setBackgroundColorSelector(const QString &selector);

	void clearFontSize();
	void clearFontSizeSelector();
	void clearBackgroundColor();
	void clearBackgroundColorSelector();

	QWidget *operator->();

	MultiStyleWidgetWrapper(QWidget *widget = nullptr);

protected:
	static const uint32_t UNSET_FONTSIZE;
	static const QString UNSET_COLOR;
	static const QString UNSET_SELECTOR;
	/// The wrapped widget
	QWidget *m_widget;
	/// The current font size
	uint32_t m_fontSize = UNSET_FONTSIZE;
	/// Whether the font size is currently set in pixels (as opposed
	/// to points)
	bool m_fontSizeInPixels = true;
	/// The CSS selector to be applied to the font size
	QString m_fontSizeSelector = UNSET_SELECTOR;
	/// The current background color
	QString m_backgroundColor = UNSET_COLOR;
	/// The CSS selector to be applied to the background color
	QString m_backgroundColorSelector = UNSET_SELECTOR;

	void updateStyleSheet();
};

#endif // MUMBLE_MUMBLE_WIDGETS_MULTISTYLEWIDGETWRAPPER_H_
