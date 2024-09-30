// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// See scripts/generate-ApplicationPalette-class.py

// clang-format off

%(warning)s
#ifndef APPLICATIONPALETTE_H
#define APPLICATIONPALETTE_H

#include <QTimer>
#include <QWidget>
#ifndef Q_MOC_RUN
#	include <boost/optional.hpp>
#endif
#include <QApplication>
#include <QDebug>

	///
	/// Class enabling theming of QApplication::palette from stylesheets.
	///
	/// QPalette cannot be styled which creates issues as not all
	/// GUI elements in Qt can be styled. This class works around
	/// that by offering a QPROPERTY for each color role and group
	/// combination in QPalette. As you can set custom QPROPERTYs
	/// from stylesheet this allows the user to set all relevant
	/// palette brushes from the stylesheet.
	///
	/// Due to restrictions on allowed property names as well as a
	/// mandatory prefix the attributes are exposed as lower cased:
	/// "qproperty-<role>_<group>".
	///
	/// So a group of QPalette::Active and QPalette::Text role
	/// would be styled by:
	///
	/// ApplicationPalette {
	///     qproperty-text_active: #ff0000; /* Set color for active group */
	/// }
	///
	/// See http://qt-project.org/doc/qt-4.8/qpalette.html#ColorGroup-enum
	/// for the available groups and roles.
	///
	/// You can also use the shorthand "qproperty-<role>" to set all groups
	/// to the same brush.
	///
	/// The class will automatically pick up style changes on itself
	/// and update the application palette accordingly. To use the class
	/// simply instantiate it before setting the theme and keep it around
	/// till the application terminates.
	///
	class ApplicationPalette : public QWidget {
		Q_OBJECT
		%(properties)s
	public:
		explicit ApplicationPalette(QWidget *p = 0) : QWidget(p), m_originalPalette(QApplication::palette()) {
			// Empty
		}

		%(getterssetters)s

	private slots:
		void updateApplicationPalette() {
			qWarning() << "Updating application palette";

			QPalette newPalette = m_originalPalette; // Do not re-use potentially already styled palette. Might not pick up
													 // system style changes though.

			%(paletteupdates)s

			QApplication::setPalette(newPalette);
			resetAllProperties();
	}

	void resetAllProperties() {
		%(propertyresets)s
	}

protected:
	bool event(QEvent *e) Q_DECL_OVERRIDE {
		bool result = QWidget::event(e);

		if (e->type() == QEvent::StyleChange) {
			// Update global palette. Have to defer it
			// as property updates are also signals.
			QTimer::singleShot(0, this, SLOT(updateApplicationPalette()));
		}

		return result;
	}

private:
	const QPalette m_originalPalette;

	%(variables)s
};

// clang-format on

#endif // APPLICATIONPALETTE_H
