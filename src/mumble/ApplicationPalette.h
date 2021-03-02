// Copyright 2014-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// See scripts/generate-ApplicationPalette-class.py

// Auto-generated from ApplicationPaletteTemplate.h . Do not edit manually.

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
	Q_PROPERTY(QBrush windowtext READ get_windowtext WRITE set_windowtext)
	Q_PROPERTY(QBrush windowtext_active READ get_windowtext_active WRITE set_windowtext_active)
	Q_PROPERTY(QBrush windowtext_disabled READ get_windowtext_disabled WRITE set_windowtext_disabled)
	Q_PROPERTY(QBrush windowtext_inactive READ get_windowtext_inactive WRITE set_windowtext_inactive)
	Q_PROPERTY(QBrush button READ get_button WRITE set_button)
	Q_PROPERTY(QBrush button_active READ get_button_active WRITE set_button_active)
	Q_PROPERTY(QBrush button_disabled READ get_button_disabled WRITE set_button_disabled)
	Q_PROPERTY(QBrush button_inactive READ get_button_inactive WRITE set_button_inactive)
	Q_PROPERTY(QBrush light READ get_light WRITE set_light)
	Q_PROPERTY(QBrush light_active READ get_light_active WRITE set_light_active)
	Q_PROPERTY(QBrush light_disabled READ get_light_disabled WRITE set_light_disabled)
	Q_PROPERTY(QBrush light_inactive READ get_light_inactive WRITE set_light_inactive)
	Q_PROPERTY(QBrush midlight READ get_midlight WRITE set_midlight)
	Q_PROPERTY(QBrush midlight_active READ get_midlight_active WRITE set_midlight_active)
	Q_PROPERTY(QBrush midlight_disabled READ get_midlight_disabled WRITE set_midlight_disabled)
	Q_PROPERTY(QBrush midlight_inactive READ get_midlight_inactive WRITE set_midlight_inactive)
	Q_PROPERTY(QBrush dark READ get_dark WRITE set_dark)
	Q_PROPERTY(QBrush dark_active READ get_dark_active WRITE set_dark_active)
	Q_PROPERTY(QBrush dark_disabled READ get_dark_disabled WRITE set_dark_disabled)
	Q_PROPERTY(QBrush dark_inactive READ get_dark_inactive WRITE set_dark_inactive)
	Q_PROPERTY(QBrush mid READ get_mid WRITE set_mid)
	Q_PROPERTY(QBrush mid_active READ get_mid_active WRITE set_mid_active)
	Q_PROPERTY(QBrush mid_disabled READ get_mid_disabled WRITE set_mid_disabled)
	Q_PROPERTY(QBrush mid_inactive READ get_mid_inactive WRITE set_mid_inactive)
	Q_PROPERTY(QBrush text READ get_text WRITE set_text)
	Q_PROPERTY(QBrush text_active READ get_text_active WRITE set_text_active)
	Q_PROPERTY(QBrush text_disabled READ get_text_disabled WRITE set_text_disabled)
	Q_PROPERTY(QBrush text_inactive READ get_text_inactive WRITE set_text_inactive)
	Q_PROPERTY(QBrush brighttext READ get_brighttext WRITE set_brighttext)
	Q_PROPERTY(QBrush brighttext_active READ get_brighttext_active WRITE set_brighttext_active)
	Q_PROPERTY(QBrush brighttext_disabled READ get_brighttext_disabled WRITE set_brighttext_disabled)
	Q_PROPERTY(QBrush brighttext_inactive READ get_brighttext_inactive WRITE set_brighttext_inactive)
	Q_PROPERTY(QBrush buttontext READ get_buttontext WRITE set_buttontext)
	Q_PROPERTY(QBrush buttontext_active READ get_buttontext_active WRITE set_buttontext_active)
	Q_PROPERTY(QBrush buttontext_disabled READ get_buttontext_disabled WRITE set_buttontext_disabled)
	Q_PROPERTY(QBrush buttontext_inactive READ get_buttontext_inactive WRITE set_buttontext_inactive)
	Q_PROPERTY(QBrush base READ get_base WRITE set_base)
	Q_PROPERTY(QBrush base_active READ get_base_active WRITE set_base_active)
	Q_PROPERTY(QBrush base_disabled READ get_base_disabled WRITE set_base_disabled)
	Q_PROPERTY(QBrush base_inactive READ get_base_inactive WRITE set_base_inactive)
	Q_PROPERTY(QBrush window READ get_window WRITE set_window)
	Q_PROPERTY(QBrush window_active READ get_window_active WRITE set_window_active)
	Q_PROPERTY(QBrush window_disabled READ get_window_disabled WRITE set_window_disabled)
	Q_PROPERTY(QBrush window_inactive READ get_window_inactive WRITE set_window_inactive)
	Q_PROPERTY(QBrush shadow READ get_shadow WRITE set_shadow)
	Q_PROPERTY(QBrush shadow_active READ get_shadow_active WRITE set_shadow_active)
	Q_PROPERTY(QBrush shadow_disabled READ get_shadow_disabled WRITE set_shadow_disabled)
	Q_PROPERTY(QBrush shadow_inactive READ get_shadow_inactive WRITE set_shadow_inactive)
	Q_PROPERTY(QBrush highlight READ get_highlight WRITE set_highlight)
	Q_PROPERTY(QBrush highlight_active READ get_highlight_active WRITE set_highlight_active)
	Q_PROPERTY(QBrush highlight_disabled READ get_highlight_disabled WRITE set_highlight_disabled)
	Q_PROPERTY(QBrush highlight_inactive READ get_highlight_inactive WRITE set_highlight_inactive)
	Q_PROPERTY(QBrush highlightedtext READ get_highlightedtext WRITE set_highlightedtext)
	Q_PROPERTY(QBrush highlightedtext_active READ get_highlightedtext_active WRITE set_highlightedtext_active)
	Q_PROPERTY(QBrush highlightedtext_disabled READ get_highlightedtext_disabled WRITE set_highlightedtext_disabled)
	Q_PROPERTY(QBrush highlightedtext_inactive READ get_highlightedtext_inactive WRITE set_highlightedtext_inactive)
	Q_PROPERTY(QBrush link READ get_link WRITE set_link)
	Q_PROPERTY(QBrush link_active READ get_link_active WRITE set_link_active)
	Q_PROPERTY(QBrush link_disabled READ get_link_disabled WRITE set_link_disabled)
	Q_PROPERTY(QBrush link_inactive READ get_link_inactive WRITE set_link_inactive)
	Q_PROPERTY(QBrush linkvisited READ get_linkvisited WRITE set_linkvisited)
	Q_PROPERTY(QBrush linkvisited_active READ get_linkvisited_active WRITE set_linkvisited_active)
	Q_PROPERTY(QBrush linkvisited_disabled READ get_linkvisited_disabled WRITE set_linkvisited_disabled)
	Q_PROPERTY(QBrush linkvisited_inactive READ get_linkvisited_inactive WRITE set_linkvisited_inactive)
	Q_PROPERTY(QBrush alternatebase READ get_alternatebase WRITE set_alternatebase)
	Q_PROPERTY(QBrush alternatebase_active READ get_alternatebase_active WRITE set_alternatebase_active)
	Q_PROPERTY(QBrush alternatebase_disabled READ get_alternatebase_disabled WRITE set_alternatebase_disabled)
	Q_PROPERTY(QBrush alternatebase_inactive READ get_alternatebase_inactive WRITE set_alternatebase_inactive)
	Q_PROPERTY(QBrush tooltipbase READ get_tooltipbase WRITE set_tooltipbase)
	Q_PROPERTY(QBrush tooltipbase_active READ get_tooltipbase_active WRITE set_tooltipbase_active)
	Q_PROPERTY(QBrush tooltipbase_disabled READ get_tooltipbase_disabled WRITE set_tooltipbase_disabled)
	Q_PROPERTY(QBrush tooltipbase_inactive READ get_tooltipbase_inactive WRITE set_tooltipbase_inactive)
	Q_PROPERTY(QBrush tooltiptext READ get_tooltiptext WRITE set_tooltiptext)
	Q_PROPERTY(QBrush tooltiptext_active READ get_tooltiptext_active WRITE set_tooltiptext_active)
	Q_PROPERTY(QBrush tooltiptext_disabled READ get_tooltiptext_disabled WRITE set_tooltiptext_disabled)
	Q_PROPERTY(QBrush tooltiptext_inactive READ get_tooltiptext_inactive WRITE set_tooltiptext_inactive)

public:
	explicit ApplicationPalette(QWidget *p = 0) : QWidget(p), m_originalPalette(QApplication::palette()) {
		// Empty
	}


	QBrush get_windowtext() {
		if (property("windowtext_active") == property("windowtext_disabled")
			&& property("windowtext_active") == property("windowtext_inactive")) {
			return qvariant_cast< QBrush >(property("windowtext_active"));
		}
		return QBrush();
	}

	void set_windowtext(const QBrush &brush) {
		m_windowtext_active   = brush;
		m_windowtext_disabled = brush;
		m_windowtext_inactive = brush;
	}

	QBrush get_windowtext_active() {
		if (!m_windowtext_active)
			return QBrush();

		return *m_windowtext_active;
	}

	void set_windowtext_active(const QBrush &brush) { m_windowtext_active = brush; }

	QBrush get_windowtext_disabled() {
		if (!m_windowtext_disabled)
			return QBrush();

		return *m_windowtext_disabled;
	}

	void set_windowtext_disabled(const QBrush &brush) { m_windowtext_disabled = brush; }

	QBrush get_windowtext_inactive() {
		if (!m_windowtext_inactive)
			return QBrush();

		return *m_windowtext_inactive;
	}

	void set_windowtext_inactive(const QBrush &brush) { m_windowtext_inactive = brush; }

	QBrush get_button() {
		if (property("button_active") == property("button_disabled")
			&& property("button_active") == property("button_inactive")) {
			return qvariant_cast< QBrush >(property("button_active"));
		}
		return QBrush();
	}

	void set_button(const QBrush &brush) {
		m_button_active   = brush;
		m_button_disabled = brush;
		m_button_inactive = brush;
	}

	QBrush get_button_active() {
		if (!m_button_active)
			return QBrush();

		return *m_button_active;
	}

	void set_button_active(const QBrush &brush) { m_button_active = brush; }

	QBrush get_button_disabled() {
		if (!m_button_disabled)
			return QBrush();

		return *m_button_disabled;
	}

	void set_button_disabled(const QBrush &brush) { m_button_disabled = brush; }

	QBrush get_button_inactive() {
		if (!m_button_inactive)
			return QBrush();

		return *m_button_inactive;
	}

	void set_button_inactive(const QBrush &brush) { m_button_inactive = brush; }

	QBrush get_light() {
		if (property("light_active") == property("light_disabled")
			&& property("light_active") == property("light_inactive")) {
			return qvariant_cast< QBrush >(property("light_active"));
		}
		return QBrush();
	}

	void set_light(const QBrush &brush) {
		m_light_active   = brush;
		m_light_disabled = brush;
		m_light_inactive = brush;
	}

	QBrush get_light_active() {
		if (!m_light_active)
			return QBrush();

		return *m_light_active;
	}

	void set_light_active(const QBrush &brush) { m_light_active = brush; }

	QBrush get_light_disabled() {
		if (!m_light_disabled)
			return QBrush();

		return *m_light_disabled;
	}

	void set_light_disabled(const QBrush &brush) { m_light_disabled = brush; }

	QBrush get_light_inactive() {
		if (!m_light_inactive)
			return QBrush();

		return *m_light_inactive;
	}

	void set_light_inactive(const QBrush &brush) { m_light_inactive = brush; }

	QBrush get_midlight() {
		if (property("midlight_active") == property("midlight_disabled")
			&& property("midlight_active") == property("midlight_inactive")) {
			return qvariant_cast< QBrush >(property("midlight_active"));
		}
		return QBrush();
	}

	void set_midlight(const QBrush &brush) {
		m_midlight_active   = brush;
		m_midlight_disabled = brush;
		m_midlight_inactive = brush;
	}

	QBrush get_midlight_active() {
		if (!m_midlight_active)
			return QBrush();

		return *m_midlight_active;
	}

	void set_midlight_active(const QBrush &brush) { m_midlight_active = brush; }

	QBrush get_midlight_disabled() {
		if (!m_midlight_disabled)
			return QBrush();

		return *m_midlight_disabled;
	}

	void set_midlight_disabled(const QBrush &brush) { m_midlight_disabled = brush; }

	QBrush get_midlight_inactive() {
		if (!m_midlight_inactive)
			return QBrush();

		return *m_midlight_inactive;
	}

	void set_midlight_inactive(const QBrush &brush) { m_midlight_inactive = brush; }

	QBrush get_dark() {
		if (property("dark_active") == property("dark_disabled")
			&& property("dark_active") == property("dark_inactive")) {
			return qvariant_cast< QBrush >(property("dark_active"));
		}
		return QBrush();
	}

	void set_dark(const QBrush &brush) {
		m_dark_active   = brush;
		m_dark_disabled = brush;
		m_dark_inactive = brush;
	}

	QBrush get_dark_active() {
		if (!m_dark_active)
			return QBrush();

		return *m_dark_active;
	}

	void set_dark_active(const QBrush &brush) { m_dark_active = brush; }

	QBrush get_dark_disabled() {
		if (!m_dark_disabled)
			return QBrush();

		return *m_dark_disabled;
	}

	void set_dark_disabled(const QBrush &brush) { m_dark_disabled = brush; }

	QBrush get_dark_inactive() {
		if (!m_dark_inactive)
			return QBrush();

		return *m_dark_inactive;
	}

	void set_dark_inactive(const QBrush &brush) { m_dark_inactive = brush; }

	QBrush get_mid() {
		if (property("mid_active") == property("mid_disabled") && property("mid_active") == property("mid_inactive")) {
			return qvariant_cast< QBrush >(property("mid_active"));
		}
		return QBrush();
	}

	void set_mid(const QBrush &brush) {
		m_mid_active   = brush;
		m_mid_disabled = brush;
		m_mid_inactive = brush;
	}

	QBrush get_mid_active() {
		if (!m_mid_active)
			return QBrush();

		return *m_mid_active;
	}

	void set_mid_active(const QBrush &brush) { m_mid_active = brush; }

	QBrush get_mid_disabled() {
		if (!m_mid_disabled)
			return QBrush();

		return *m_mid_disabled;
	}

	void set_mid_disabled(const QBrush &brush) { m_mid_disabled = brush; }

	QBrush get_mid_inactive() {
		if (!m_mid_inactive)
			return QBrush();

		return *m_mid_inactive;
	}

	void set_mid_inactive(const QBrush &brush) { m_mid_inactive = brush; }

	QBrush get_text() {
		if (property("text_active") == property("text_disabled")
			&& property("text_active") == property("text_inactive")) {
			return qvariant_cast< QBrush >(property("text_active"));
		}
		return QBrush();
	}

	void set_text(const QBrush &brush) {
		m_text_active   = brush;
		m_text_disabled = brush;
		m_text_inactive = brush;
	}

	QBrush get_text_active() {
		if (!m_text_active)
			return QBrush();

		return *m_text_active;
	}

	void set_text_active(const QBrush &brush) { m_text_active = brush; }

	QBrush get_text_disabled() {
		if (!m_text_disabled)
			return QBrush();

		return *m_text_disabled;
	}

	void set_text_disabled(const QBrush &brush) { m_text_disabled = brush; }

	QBrush get_text_inactive() {
		if (!m_text_inactive)
			return QBrush();

		return *m_text_inactive;
	}

	void set_text_inactive(const QBrush &brush) { m_text_inactive = brush; }

	QBrush get_brighttext() {
		if (property("brighttext_active") == property("brighttext_disabled")
			&& property("brighttext_active") == property("brighttext_inactive")) {
			return qvariant_cast< QBrush >(property("brighttext_active"));
		}
		return QBrush();
	}

	void set_brighttext(const QBrush &brush) {
		m_brighttext_active   = brush;
		m_brighttext_disabled = brush;
		m_brighttext_inactive = brush;
	}

	QBrush get_brighttext_active() {
		if (!m_brighttext_active)
			return QBrush();

		return *m_brighttext_active;
	}

	void set_brighttext_active(const QBrush &brush) { m_brighttext_active = brush; }

	QBrush get_brighttext_disabled() {
		if (!m_brighttext_disabled)
			return QBrush();

		return *m_brighttext_disabled;
	}

	void set_brighttext_disabled(const QBrush &brush) { m_brighttext_disabled = brush; }

	QBrush get_brighttext_inactive() {
		if (!m_brighttext_inactive)
			return QBrush();

		return *m_brighttext_inactive;
	}

	void set_brighttext_inactive(const QBrush &brush) { m_brighttext_inactive = brush; }

	QBrush get_buttontext() {
		if (property("buttontext_active") == property("buttontext_disabled")
			&& property("buttontext_active") == property("buttontext_inactive")) {
			return qvariant_cast< QBrush >(property("buttontext_active"));
		}
		return QBrush();
	}

	void set_buttontext(const QBrush &brush) {
		m_buttontext_active   = brush;
		m_buttontext_disabled = brush;
		m_buttontext_inactive = brush;
	}

	QBrush get_buttontext_active() {
		if (!m_buttontext_active)
			return QBrush();

		return *m_buttontext_active;
	}

	void set_buttontext_active(const QBrush &brush) { m_buttontext_active = brush; }

	QBrush get_buttontext_disabled() {
		if (!m_buttontext_disabled)
			return QBrush();

		return *m_buttontext_disabled;
	}

	void set_buttontext_disabled(const QBrush &brush) { m_buttontext_disabled = brush; }

	QBrush get_buttontext_inactive() {
		if (!m_buttontext_inactive)
			return QBrush();

		return *m_buttontext_inactive;
	}

	void set_buttontext_inactive(const QBrush &brush) { m_buttontext_inactive = brush; }

	QBrush get_base() {
		if (property("base_active") == property("base_disabled")
			&& property("base_active") == property("base_inactive")) {
			return qvariant_cast< QBrush >(property("base_active"));
		}
		return QBrush();
	}

	void set_base(const QBrush &brush) {
		m_base_active   = brush;
		m_base_disabled = brush;
		m_base_inactive = brush;
	}

	QBrush get_base_active() {
		if (!m_base_active)
			return QBrush();

		return *m_base_active;
	}

	void set_base_active(const QBrush &brush) { m_base_active = brush; }

	QBrush get_base_disabled() {
		if (!m_base_disabled)
			return QBrush();

		return *m_base_disabled;
	}

	void set_base_disabled(const QBrush &brush) { m_base_disabled = brush; }

	QBrush get_base_inactive() {
		if (!m_base_inactive)
			return QBrush();

		return *m_base_inactive;
	}

	void set_base_inactive(const QBrush &brush) { m_base_inactive = brush; }

	QBrush get_window() {
		if (property("window_active") == property("window_disabled")
			&& property("window_active") == property("window_inactive")) {
			return qvariant_cast< QBrush >(property("window_active"));
		}
		return QBrush();
	}

	void set_window(const QBrush &brush) {
		m_window_active   = brush;
		m_window_disabled = brush;
		m_window_inactive = brush;
	}

	QBrush get_window_active() {
		if (!m_window_active)
			return QBrush();

		return *m_window_active;
	}

	void set_window_active(const QBrush &brush) { m_window_active = brush; }

	QBrush get_window_disabled() {
		if (!m_window_disabled)
			return QBrush();

		return *m_window_disabled;
	}

	void set_window_disabled(const QBrush &brush) { m_window_disabled = brush; }

	QBrush get_window_inactive() {
		if (!m_window_inactive)
			return QBrush();

		return *m_window_inactive;
	}

	void set_window_inactive(const QBrush &brush) { m_window_inactive = brush; }

	QBrush get_shadow() {
		if (property("shadow_active") == property("shadow_disabled")
			&& property("shadow_active") == property("shadow_inactive")) {
			return qvariant_cast< QBrush >(property("shadow_active"));
		}
		return QBrush();
	}

	void set_shadow(const QBrush &brush) {
		m_shadow_active   = brush;
		m_shadow_disabled = brush;
		m_shadow_inactive = brush;
	}

	QBrush get_shadow_active() {
		if (!m_shadow_active)
			return QBrush();

		return *m_shadow_active;
	}

	void set_shadow_active(const QBrush &brush) { m_shadow_active = brush; }

	QBrush get_shadow_disabled() {
		if (!m_shadow_disabled)
			return QBrush();

		return *m_shadow_disabled;
	}

	void set_shadow_disabled(const QBrush &brush) { m_shadow_disabled = brush; }

	QBrush get_shadow_inactive() {
		if (!m_shadow_inactive)
			return QBrush();

		return *m_shadow_inactive;
	}

	void set_shadow_inactive(const QBrush &brush) { m_shadow_inactive = brush; }

	QBrush get_highlight() {
		if (property("highlight_active") == property("highlight_disabled")
			&& property("highlight_active") == property("highlight_inactive")) {
			return qvariant_cast< QBrush >(property("highlight_active"));
		}
		return QBrush();
	}

	void set_highlight(const QBrush &brush) {
		m_highlight_active   = brush;
		m_highlight_disabled = brush;
		m_highlight_inactive = brush;
	}

	QBrush get_highlight_active() {
		if (!m_highlight_active)
			return QBrush();

		return *m_highlight_active;
	}

	void set_highlight_active(const QBrush &brush) { m_highlight_active = brush; }

	QBrush get_highlight_disabled() {
		if (!m_highlight_disabled)
			return QBrush();

		return *m_highlight_disabled;
	}

	void set_highlight_disabled(const QBrush &brush) { m_highlight_disabled = brush; }

	QBrush get_highlight_inactive() {
		if (!m_highlight_inactive)
			return QBrush();

		return *m_highlight_inactive;
	}

	void set_highlight_inactive(const QBrush &brush) { m_highlight_inactive = brush; }

	QBrush get_highlightedtext() {
		if (property("highlightedtext_active") == property("highlightedtext_disabled")
			&& property("highlightedtext_active") == property("highlightedtext_inactive")) {
			return qvariant_cast< QBrush >(property("highlightedtext_active"));
		}
		return QBrush();
	}

	void set_highlightedtext(const QBrush &brush) {
		m_highlightedtext_active   = brush;
		m_highlightedtext_disabled = brush;
		m_highlightedtext_inactive = brush;
	}

	QBrush get_highlightedtext_active() {
		if (!m_highlightedtext_active)
			return QBrush();

		return *m_highlightedtext_active;
	}

	void set_highlightedtext_active(const QBrush &brush) { m_highlightedtext_active = brush; }

	QBrush get_highlightedtext_disabled() {
		if (!m_highlightedtext_disabled)
			return QBrush();

		return *m_highlightedtext_disabled;
	}

	void set_highlightedtext_disabled(const QBrush &brush) { m_highlightedtext_disabled = brush; }

	QBrush get_highlightedtext_inactive() {
		if (!m_highlightedtext_inactive)
			return QBrush();

		return *m_highlightedtext_inactive;
	}

	void set_highlightedtext_inactive(const QBrush &brush) { m_highlightedtext_inactive = brush; }

	QBrush get_link() {
		if (property("link_active") == property("link_disabled")
			&& property("link_active") == property("link_inactive")) {
			return qvariant_cast< QBrush >(property("link_active"));
		}
		return QBrush();
	}

	void set_link(const QBrush &brush) {
		m_link_active   = brush;
		m_link_disabled = brush;
		m_link_inactive = brush;
	}

	QBrush get_link_active() {
		if (!m_link_active)
			return QBrush();

		return *m_link_active;
	}

	void set_link_active(const QBrush &brush) { m_link_active = brush; }

	QBrush get_link_disabled() {
		if (!m_link_disabled)
			return QBrush();

		return *m_link_disabled;
	}

	void set_link_disabled(const QBrush &brush) { m_link_disabled = brush; }

	QBrush get_link_inactive() {
		if (!m_link_inactive)
			return QBrush();

		return *m_link_inactive;
	}

	void set_link_inactive(const QBrush &brush) { m_link_inactive = brush; }

	QBrush get_linkvisited() {
		if (property("linkvisited_active") == property("linkvisited_disabled")
			&& property("linkvisited_active") == property("linkvisited_inactive")) {
			return qvariant_cast< QBrush >(property("linkvisited_active"));
		}
		return QBrush();
	}

	void set_linkvisited(const QBrush &brush) {
		m_linkvisited_active   = brush;
		m_linkvisited_disabled = brush;
		m_linkvisited_inactive = brush;
	}

	QBrush get_linkvisited_active() {
		if (!m_linkvisited_active)
			return QBrush();

		return *m_linkvisited_active;
	}

	void set_linkvisited_active(const QBrush &brush) { m_linkvisited_active = brush; }

	QBrush get_linkvisited_disabled() {
		if (!m_linkvisited_disabled)
			return QBrush();

		return *m_linkvisited_disabled;
	}

	void set_linkvisited_disabled(const QBrush &brush) { m_linkvisited_disabled = brush; }

	QBrush get_linkvisited_inactive() {
		if (!m_linkvisited_inactive)
			return QBrush();

		return *m_linkvisited_inactive;
	}

	void set_linkvisited_inactive(const QBrush &brush) { m_linkvisited_inactive = brush; }

	QBrush get_alternatebase() {
		if (property("alternatebase_active") == property("alternatebase_disabled")
			&& property("alternatebase_active") == property("alternatebase_inactive")) {
			return qvariant_cast< QBrush >(property("alternatebase_active"));
		}
		return QBrush();
	}

	void set_alternatebase(const QBrush &brush) {
		m_alternatebase_active   = brush;
		m_alternatebase_disabled = brush;
		m_alternatebase_inactive = brush;
	}

	QBrush get_alternatebase_active() {
		if (!m_alternatebase_active)
			return QBrush();

		return *m_alternatebase_active;
	}

	void set_alternatebase_active(const QBrush &brush) { m_alternatebase_active = brush; }

	QBrush get_alternatebase_disabled() {
		if (!m_alternatebase_disabled)
			return QBrush();

		return *m_alternatebase_disabled;
	}

	void set_alternatebase_disabled(const QBrush &brush) { m_alternatebase_disabled = brush; }

	QBrush get_alternatebase_inactive() {
		if (!m_alternatebase_inactive)
			return QBrush();

		return *m_alternatebase_inactive;
	}

	void set_alternatebase_inactive(const QBrush &brush) { m_alternatebase_inactive = brush; }

	QBrush get_tooltipbase() {
		if (property("tooltipbase_active") == property("tooltipbase_disabled")
			&& property("tooltipbase_active") == property("tooltipbase_inactive")) {
			return qvariant_cast< QBrush >(property("tooltipbase_active"));
		}
		return QBrush();
	}

	void set_tooltipbase(const QBrush &brush) {
		m_tooltipbase_active   = brush;
		m_tooltipbase_disabled = brush;
		m_tooltipbase_inactive = brush;
	}

	QBrush get_tooltipbase_active() {
		if (!m_tooltipbase_active)
			return QBrush();

		return *m_tooltipbase_active;
	}

	void set_tooltipbase_active(const QBrush &brush) { m_tooltipbase_active = brush; }

	QBrush get_tooltipbase_disabled() {
		if (!m_tooltipbase_disabled)
			return QBrush();

		return *m_tooltipbase_disabled;
	}

	void set_tooltipbase_disabled(const QBrush &brush) { m_tooltipbase_disabled = brush; }

	QBrush get_tooltipbase_inactive() {
		if (!m_tooltipbase_inactive)
			return QBrush();

		return *m_tooltipbase_inactive;
	}

	void set_tooltipbase_inactive(const QBrush &brush) { m_tooltipbase_inactive = brush; }

	QBrush get_tooltiptext() {
		if (property("tooltiptext_active") == property("tooltiptext_disabled")
			&& property("tooltiptext_active") == property("tooltiptext_inactive")) {
			return qvariant_cast< QBrush >(property("tooltiptext_active"));
		}
		return QBrush();
	}

	void set_tooltiptext(const QBrush &brush) {
		m_tooltiptext_active   = brush;
		m_tooltiptext_disabled = brush;
		m_tooltiptext_inactive = brush;
	}

	QBrush get_tooltiptext_active() {
		if (!m_tooltiptext_active)
			return QBrush();

		return *m_tooltiptext_active;
	}

	void set_tooltiptext_active(const QBrush &brush) { m_tooltiptext_active = brush; }

	QBrush get_tooltiptext_disabled() {
		if (!m_tooltiptext_disabled)
			return QBrush();

		return *m_tooltiptext_disabled;
	}

	void set_tooltiptext_disabled(const QBrush &brush) { m_tooltiptext_disabled = brush; }

	QBrush get_tooltiptext_inactive() {
		if (!m_tooltiptext_inactive)
			return QBrush();

		return *m_tooltiptext_inactive;
	}

	void set_tooltiptext_inactive(const QBrush &brush) { m_tooltiptext_inactive = brush; }


private slots:
	void updateApplicationPalette() {
		qWarning() << "Updating application palette";

		QPalette newPalette = m_originalPalette; // Do not re-use potentially already styled palette. Might not pick up
												 // system style changes though.


		if (m_windowtext_active) {
			newPalette.setBrush(QPalette::Active, QPalette::WindowText, *m_windowtext_active);
		}

		if (m_windowtext_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::WindowText, *m_windowtext_disabled);
		}

		if (m_windowtext_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::WindowText, *m_windowtext_inactive);
		}

		if (m_button_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Button, *m_button_active);
		}

		if (m_button_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Button, *m_button_disabled);
		}

		if (m_button_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Button, *m_button_inactive);
		}

		if (m_light_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Light, *m_light_active);
		}

		if (m_light_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Light, *m_light_disabled);
		}

		if (m_light_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Light, *m_light_inactive);
		}

		if (m_midlight_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Midlight, *m_midlight_active);
		}

		if (m_midlight_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Midlight, *m_midlight_disabled);
		}

		if (m_midlight_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Midlight, *m_midlight_inactive);
		}

		if (m_dark_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Dark, *m_dark_active);
		}

		if (m_dark_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Dark, *m_dark_disabled);
		}

		if (m_dark_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Dark, *m_dark_inactive);
		}

		if (m_mid_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Mid, *m_mid_active);
		}

		if (m_mid_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Mid, *m_mid_disabled);
		}

		if (m_mid_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Mid, *m_mid_inactive);
		}

		if (m_text_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Text, *m_text_active);
		}

		if (m_text_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Text, *m_text_disabled);
		}

		if (m_text_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Text, *m_text_inactive);
		}

		if (m_brighttext_active) {
			newPalette.setBrush(QPalette::Active, QPalette::BrightText, *m_brighttext_active);
		}

		if (m_brighttext_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::BrightText, *m_brighttext_disabled);
		}

		if (m_brighttext_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::BrightText, *m_brighttext_inactive);
		}

		if (m_buttontext_active) {
			newPalette.setBrush(QPalette::Active, QPalette::ButtonText, *m_buttontext_active);
		}

		if (m_buttontext_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::ButtonText, *m_buttontext_disabled);
		}

		if (m_buttontext_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::ButtonText, *m_buttontext_inactive);
		}

		if (m_base_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Base, *m_base_active);
		}

		if (m_base_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Base, *m_base_disabled);
		}

		if (m_base_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Base, *m_base_inactive);
		}

		if (m_window_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Window, *m_window_active);
		}

		if (m_window_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Window, *m_window_disabled);
		}

		if (m_window_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Window, *m_window_inactive);
		}

		if (m_shadow_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Shadow, *m_shadow_active);
		}

		if (m_shadow_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Shadow, *m_shadow_disabled);
		}

		if (m_shadow_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Shadow, *m_shadow_inactive);
		}

		if (m_highlight_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Highlight, *m_highlight_active);
		}

		if (m_highlight_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Highlight, *m_highlight_disabled);
		}

		if (m_highlight_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Highlight, *m_highlight_inactive);
		}

		if (m_highlightedtext_active) {
			newPalette.setBrush(QPalette::Active, QPalette::HighlightedText, *m_highlightedtext_active);
		}

		if (m_highlightedtext_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::HighlightedText, *m_highlightedtext_disabled);
		}

		if (m_highlightedtext_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::HighlightedText, *m_highlightedtext_inactive);
		}

		if (m_link_active) {
			newPalette.setBrush(QPalette::Active, QPalette::Link, *m_link_active);
		}

		if (m_link_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::Link, *m_link_disabled);
		}

		if (m_link_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::Link, *m_link_inactive);
		}

		if (m_linkvisited_active) {
			newPalette.setBrush(QPalette::Active, QPalette::LinkVisited, *m_linkvisited_active);
		}

		if (m_linkvisited_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::LinkVisited, *m_linkvisited_disabled);
		}

		if (m_linkvisited_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::LinkVisited, *m_linkvisited_inactive);
		}

		if (m_alternatebase_active) {
			newPalette.setBrush(QPalette::Active, QPalette::AlternateBase, *m_alternatebase_active);
		}

		if (m_alternatebase_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::AlternateBase, *m_alternatebase_disabled);
		}

		if (m_alternatebase_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::AlternateBase, *m_alternatebase_inactive);
		}

		if (m_tooltipbase_active) {
			newPalette.setBrush(QPalette::Active, QPalette::ToolTipBase, *m_tooltipbase_active);
		}

		if (m_tooltipbase_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, *m_tooltipbase_disabled);
		}

		if (m_tooltipbase_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, *m_tooltipbase_inactive);
		}

		if (m_tooltiptext_active) {
			newPalette.setBrush(QPalette::Active, QPalette::ToolTipText, *m_tooltiptext_active);
		}

		if (m_tooltiptext_disabled) {
			newPalette.setBrush(QPalette::Disabled, QPalette::ToolTipText, *m_tooltiptext_disabled);
		}

		if (m_tooltiptext_inactive) {
			newPalette.setBrush(QPalette::Inactive, QPalette::ToolTipText, *m_tooltiptext_inactive);
		}


		QApplication::setPalette(newPalette);
		resetAllProperties();
	}

	void resetAllProperties() {
		m_windowtext_active        = boost::none;
		m_windowtext_disabled      = boost::none;
		m_windowtext_inactive      = boost::none;
		m_button_active            = boost::none;
		m_button_disabled          = boost::none;
		m_button_inactive          = boost::none;
		m_light_active             = boost::none;
		m_light_disabled           = boost::none;
		m_light_inactive           = boost::none;
		m_midlight_active          = boost::none;
		m_midlight_disabled        = boost::none;
		m_midlight_inactive        = boost::none;
		m_dark_active              = boost::none;
		m_dark_disabled            = boost::none;
		m_dark_inactive            = boost::none;
		m_mid_active               = boost::none;
		m_mid_disabled             = boost::none;
		m_mid_inactive             = boost::none;
		m_text_active              = boost::none;
		m_text_disabled            = boost::none;
		m_text_inactive            = boost::none;
		m_brighttext_active        = boost::none;
		m_brighttext_disabled      = boost::none;
		m_brighttext_inactive      = boost::none;
		m_buttontext_active        = boost::none;
		m_buttontext_disabled      = boost::none;
		m_buttontext_inactive      = boost::none;
		m_base_active              = boost::none;
		m_base_disabled            = boost::none;
		m_base_inactive            = boost::none;
		m_window_active            = boost::none;
		m_window_disabled          = boost::none;
		m_window_inactive          = boost::none;
		m_shadow_active            = boost::none;
		m_shadow_disabled          = boost::none;
		m_shadow_inactive          = boost::none;
		m_highlight_active         = boost::none;
		m_highlight_disabled       = boost::none;
		m_highlight_inactive       = boost::none;
		m_highlightedtext_active   = boost::none;
		m_highlightedtext_disabled = boost::none;
		m_highlightedtext_inactive = boost::none;
		m_link_active              = boost::none;
		m_link_disabled            = boost::none;
		m_link_inactive            = boost::none;
		m_linkvisited_active       = boost::none;
		m_linkvisited_disabled     = boost::none;
		m_linkvisited_inactive     = boost::none;
		m_alternatebase_active     = boost::none;
		m_alternatebase_disabled   = boost::none;
		m_alternatebase_inactive   = boost::none;
		m_tooltipbase_active       = boost::none;
		m_tooltipbase_disabled     = boost::none;
		m_tooltipbase_inactive     = boost::none;
		m_tooltiptext_active       = boost::none;
		m_tooltiptext_disabled     = boost::none;
		m_tooltiptext_inactive     = boost::none;
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

	boost::optional< QBrush > m_windowtext_active;
	boost::optional< QBrush > m_windowtext_disabled;
	boost::optional< QBrush > m_windowtext_inactive;
	boost::optional< QBrush > m_button_active;
	boost::optional< QBrush > m_button_disabled;
	boost::optional< QBrush > m_button_inactive;
	boost::optional< QBrush > m_light_active;
	boost::optional< QBrush > m_light_disabled;
	boost::optional< QBrush > m_light_inactive;
	boost::optional< QBrush > m_midlight_active;
	boost::optional< QBrush > m_midlight_disabled;
	boost::optional< QBrush > m_midlight_inactive;
	boost::optional< QBrush > m_dark_active;
	boost::optional< QBrush > m_dark_disabled;
	boost::optional< QBrush > m_dark_inactive;
	boost::optional< QBrush > m_mid_active;
	boost::optional< QBrush > m_mid_disabled;
	boost::optional< QBrush > m_mid_inactive;
	boost::optional< QBrush > m_text_active;
	boost::optional< QBrush > m_text_disabled;
	boost::optional< QBrush > m_text_inactive;
	boost::optional< QBrush > m_brighttext_active;
	boost::optional< QBrush > m_brighttext_disabled;
	boost::optional< QBrush > m_brighttext_inactive;
	boost::optional< QBrush > m_buttontext_active;
	boost::optional< QBrush > m_buttontext_disabled;
	boost::optional< QBrush > m_buttontext_inactive;
	boost::optional< QBrush > m_base_active;
	boost::optional< QBrush > m_base_disabled;
	boost::optional< QBrush > m_base_inactive;
	boost::optional< QBrush > m_window_active;
	boost::optional< QBrush > m_window_disabled;
	boost::optional< QBrush > m_window_inactive;
	boost::optional< QBrush > m_shadow_active;
	boost::optional< QBrush > m_shadow_disabled;
	boost::optional< QBrush > m_shadow_inactive;
	boost::optional< QBrush > m_highlight_active;
	boost::optional< QBrush > m_highlight_disabled;
	boost::optional< QBrush > m_highlight_inactive;
	boost::optional< QBrush > m_highlightedtext_active;
	boost::optional< QBrush > m_highlightedtext_disabled;
	boost::optional< QBrush > m_highlightedtext_inactive;
	boost::optional< QBrush > m_link_active;
	boost::optional< QBrush > m_link_disabled;
	boost::optional< QBrush > m_link_inactive;
	boost::optional< QBrush > m_linkvisited_active;
	boost::optional< QBrush > m_linkvisited_disabled;
	boost::optional< QBrush > m_linkvisited_inactive;
	boost::optional< QBrush > m_alternatebase_active;
	boost::optional< QBrush > m_alternatebase_disabled;
	boost::optional< QBrush > m_alternatebase_inactive;
	boost::optional< QBrush > m_tooltipbase_active;
	boost::optional< QBrush > m_tooltipbase_disabled;
	boost::optional< QBrush > m_tooltipbase_inactive;
	boost::optional< QBrush > m_tooltiptext_active;
	boost::optional< QBrush > m_tooltiptext_disabled;
	boost::optional< QBrush > m_tooltiptext_inactive;
};

#endif // APPLICATIONPALETTE_H
