/* Copyright (C) 2014, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// See scripts/generate-ApplicationPalette-class.py

// Auto-generated from ApplicationPaletteTemplate.h . Do not edit manually.

#ifndef APPLICATIONPALETTE_H
#define APPLICATIONPALETTE_H

#include <QWidget>
#include <QTimer>
#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#endif
#include <QApplication>

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
/// mandatory prefix the attributes are exposed as
/// "qproperty-<group>_<role>".
/// 
/// So a group of QPalette::Active and QPalette::Text role
/// would be styled by:
/// 
/// ApplicationPalette {
///     qproperty-active_text: #ff0000;
/// }
///
/// See http://qt-project.org/doc/qt-4.8/qpalette.html#ColorGroup-enum
/// for the available groups and roles.
/// 
/// The class will automatically pick up style changes on itself
/// and update the application palette accordingly. To use the class
/// simply instantiate it before setting the theme and keep it around
/// till the application terminates.
/// 
class ApplicationPalette : public QWidget
{
		Q_OBJECT
		Q_PROPERTY(QBrush active_windowtext READ get_active_windowtext WRITE set_active_windowtext)
		Q_PROPERTY(QBrush active_button READ get_active_button WRITE set_active_button)
		Q_PROPERTY(QBrush active_light READ get_active_light WRITE set_active_light)
		Q_PROPERTY(QBrush active_midlight READ get_active_midlight WRITE set_active_midlight)
		Q_PROPERTY(QBrush active_dark READ get_active_dark WRITE set_active_dark)
		Q_PROPERTY(QBrush active_mid READ get_active_mid WRITE set_active_mid)
		Q_PROPERTY(QBrush active_text READ get_active_text WRITE set_active_text)
		Q_PROPERTY(QBrush active_brighttext READ get_active_brighttext WRITE set_active_brighttext)
		Q_PROPERTY(QBrush active_buttontext READ get_active_buttontext WRITE set_active_buttontext)
		Q_PROPERTY(QBrush active_base READ get_active_base WRITE set_active_base)
		Q_PROPERTY(QBrush active_window READ get_active_window WRITE set_active_window)
		Q_PROPERTY(QBrush active_shadow READ get_active_shadow WRITE set_active_shadow)
		Q_PROPERTY(QBrush active_highlight READ get_active_highlight WRITE set_active_highlight)
		Q_PROPERTY(QBrush active_highlightedtext READ get_active_highlightedtext WRITE set_active_highlightedtext)
		Q_PROPERTY(QBrush active_link READ get_active_link WRITE set_active_link)
		Q_PROPERTY(QBrush active_linkvisited READ get_active_linkvisited WRITE set_active_linkvisited)
		Q_PROPERTY(QBrush active_alternatebase READ get_active_alternatebase WRITE set_active_alternatebase)
		Q_PROPERTY(QBrush active_tooltipbase READ get_active_tooltipbase WRITE set_active_tooltipbase)
		Q_PROPERTY(QBrush active_tooltiptext READ get_active_tooltiptext WRITE set_active_tooltiptext)
		Q_PROPERTY(QBrush disabled_windowtext READ get_disabled_windowtext WRITE set_disabled_windowtext)
		Q_PROPERTY(QBrush disabled_button READ get_disabled_button WRITE set_disabled_button)
		Q_PROPERTY(QBrush disabled_light READ get_disabled_light WRITE set_disabled_light)
		Q_PROPERTY(QBrush disabled_midlight READ get_disabled_midlight WRITE set_disabled_midlight)
		Q_PROPERTY(QBrush disabled_dark READ get_disabled_dark WRITE set_disabled_dark)
		Q_PROPERTY(QBrush disabled_mid READ get_disabled_mid WRITE set_disabled_mid)
		Q_PROPERTY(QBrush disabled_text READ get_disabled_text WRITE set_disabled_text)
		Q_PROPERTY(QBrush disabled_brighttext READ get_disabled_brighttext WRITE set_disabled_brighttext)
		Q_PROPERTY(QBrush disabled_buttontext READ get_disabled_buttontext WRITE set_disabled_buttontext)
		Q_PROPERTY(QBrush disabled_base READ get_disabled_base WRITE set_disabled_base)
		Q_PROPERTY(QBrush disabled_window READ get_disabled_window WRITE set_disabled_window)
		Q_PROPERTY(QBrush disabled_shadow READ get_disabled_shadow WRITE set_disabled_shadow)
		Q_PROPERTY(QBrush disabled_highlight READ get_disabled_highlight WRITE set_disabled_highlight)
		Q_PROPERTY(QBrush disabled_highlightedtext READ get_disabled_highlightedtext WRITE set_disabled_highlightedtext)
		Q_PROPERTY(QBrush disabled_link READ get_disabled_link WRITE set_disabled_link)
		Q_PROPERTY(QBrush disabled_linkvisited READ get_disabled_linkvisited WRITE set_disabled_linkvisited)
		Q_PROPERTY(QBrush disabled_alternatebase READ get_disabled_alternatebase WRITE set_disabled_alternatebase)
		Q_PROPERTY(QBrush disabled_tooltipbase READ get_disabled_tooltipbase WRITE set_disabled_tooltipbase)
		Q_PROPERTY(QBrush disabled_tooltiptext READ get_disabled_tooltiptext WRITE set_disabled_tooltiptext)
		Q_PROPERTY(QBrush inactive_windowtext READ get_inactive_windowtext WRITE set_inactive_windowtext)
		Q_PROPERTY(QBrush inactive_button READ get_inactive_button WRITE set_inactive_button)
		Q_PROPERTY(QBrush inactive_light READ get_inactive_light WRITE set_inactive_light)
		Q_PROPERTY(QBrush inactive_midlight READ get_inactive_midlight WRITE set_inactive_midlight)
		Q_PROPERTY(QBrush inactive_dark READ get_inactive_dark WRITE set_inactive_dark)
		Q_PROPERTY(QBrush inactive_mid READ get_inactive_mid WRITE set_inactive_mid)
		Q_PROPERTY(QBrush inactive_text READ get_inactive_text WRITE set_inactive_text)
		Q_PROPERTY(QBrush inactive_brighttext READ get_inactive_brighttext WRITE set_inactive_brighttext)
		Q_PROPERTY(QBrush inactive_buttontext READ get_inactive_buttontext WRITE set_inactive_buttontext)
		Q_PROPERTY(QBrush inactive_base READ get_inactive_base WRITE set_inactive_base)
		Q_PROPERTY(QBrush inactive_window READ get_inactive_window WRITE set_inactive_window)
		Q_PROPERTY(QBrush inactive_shadow READ get_inactive_shadow WRITE set_inactive_shadow)
		Q_PROPERTY(QBrush inactive_highlight READ get_inactive_highlight WRITE set_inactive_highlight)
		Q_PROPERTY(QBrush inactive_highlightedtext READ get_inactive_highlightedtext WRITE set_inactive_highlightedtext)
		Q_PROPERTY(QBrush inactive_link READ get_inactive_link WRITE set_inactive_link)
		Q_PROPERTY(QBrush inactive_linkvisited READ get_inactive_linkvisited WRITE set_inactive_linkvisited)
		Q_PROPERTY(QBrush inactive_alternatebase READ get_inactive_alternatebase WRITE set_inactive_alternatebase)
		Q_PROPERTY(QBrush inactive_tooltipbase READ get_inactive_tooltipbase WRITE set_inactive_tooltipbase)
		Q_PROPERTY(QBrush inactive_tooltiptext READ get_inactive_tooltiptext WRITE set_inactive_tooltiptext)

	public:
		explicit ApplicationPalette(QWidget *parent = 0) :
		    QWidget(parent) {
			// Empty
		}
		

		QBrush get_active_windowtext() {
			if (!m_active_windowtext)
				return QBrush();

			return *m_active_windowtext;
		}

		void set_active_windowtext(const QBrush& brush) {
			m_active_windowtext = brush;
		}

		QBrush get_active_button() {
			if (!m_active_button)
				return QBrush();

			return *m_active_button;
		}

		void set_active_button(const QBrush& brush) {
			m_active_button = brush;
		}

		QBrush get_active_light() {
			if (!m_active_light)
				return QBrush();

			return *m_active_light;
		}

		void set_active_light(const QBrush& brush) {
			m_active_light = brush;
		}

		QBrush get_active_midlight() {
			if (!m_active_midlight)
				return QBrush();

			return *m_active_midlight;
		}

		void set_active_midlight(const QBrush& brush) {
			m_active_midlight = brush;
		}

		QBrush get_active_dark() {
			if (!m_active_dark)
				return QBrush();

			return *m_active_dark;
		}

		void set_active_dark(const QBrush& brush) {
			m_active_dark = brush;
		}

		QBrush get_active_mid() {
			if (!m_active_mid)
				return QBrush();

			return *m_active_mid;
		}

		void set_active_mid(const QBrush& brush) {
			m_active_mid = brush;
		}

		QBrush get_active_text() {
			if (!m_active_text)
				return QBrush();

			return *m_active_text;
		}

		void set_active_text(const QBrush& brush) {
			m_active_text = brush;
		}

		QBrush get_active_brighttext() {
			if (!m_active_brighttext)
				return QBrush();

			return *m_active_brighttext;
		}

		void set_active_brighttext(const QBrush& brush) {
			m_active_brighttext = brush;
		}

		QBrush get_active_buttontext() {
			if (!m_active_buttontext)
				return QBrush();

			return *m_active_buttontext;
		}

		void set_active_buttontext(const QBrush& brush) {
			m_active_buttontext = brush;
		}

		QBrush get_active_base() {
			if (!m_active_base)
				return QBrush();

			return *m_active_base;
		}

		void set_active_base(const QBrush& brush) {
			m_active_base = brush;
		}

		QBrush get_active_window() {
			if (!m_active_window)
				return QBrush();

			return *m_active_window;
		}

		void set_active_window(const QBrush& brush) {
			m_active_window = brush;
		}

		QBrush get_active_shadow() {
			if (!m_active_shadow)
				return QBrush();

			return *m_active_shadow;
		}

		void set_active_shadow(const QBrush& brush) {
			m_active_shadow = brush;
		}

		QBrush get_active_highlight() {
			if (!m_active_highlight)
				return QBrush();

			return *m_active_highlight;
		}

		void set_active_highlight(const QBrush& brush) {
			m_active_highlight = brush;
		}

		QBrush get_active_highlightedtext() {
			if (!m_active_highlightedtext)
				return QBrush();

			return *m_active_highlightedtext;
		}

		void set_active_highlightedtext(const QBrush& brush) {
			m_active_highlightedtext = brush;
		}

		QBrush get_active_link() {
			if (!m_active_link)
				return QBrush();

			return *m_active_link;
		}

		void set_active_link(const QBrush& brush) {
			m_active_link = brush;
		}

		QBrush get_active_linkvisited() {
			if (!m_active_linkvisited)
				return QBrush();

			return *m_active_linkvisited;
		}

		void set_active_linkvisited(const QBrush& brush) {
			m_active_linkvisited = brush;
		}

		QBrush get_active_alternatebase() {
			if (!m_active_alternatebase)
				return QBrush();

			return *m_active_alternatebase;
		}

		void set_active_alternatebase(const QBrush& brush) {
			m_active_alternatebase = brush;
		}

		QBrush get_active_tooltipbase() {
			if (!m_active_tooltipbase)
				return QBrush();

			return *m_active_tooltipbase;
		}

		void set_active_tooltipbase(const QBrush& brush) {
			m_active_tooltipbase = brush;
		}

		QBrush get_active_tooltiptext() {
			if (!m_active_tooltiptext)
				return QBrush();

			return *m_active_tooltiptext;
		}

		void set_active_tooltiptext(const QBrush& brush) {
			m_active_tooltiptext = brush;
		}

		QBrush get_disabled_windowtext() {
			if (!m_disabled_windowtext)
				return QBrush();

			return *m_disabled_windowtext;
		}

		void set_disabled_windowtext(const QBrush& brush) {
			m_disabled_windowtext = brush;
		}

		QBrush get_disabled_button() {
			if (!m_disabled_button)
				return QBrush();

			return *m_disabled_button;
		}

		void set_disabled_button(const QBrush& brush) {
			m_disabled_button = brush;
		}

		QBrush get_disabled_light() {
			if (!m_disabled_light)
				return QBrush();

			return *m_disabled_light;
		}

		void set_disabled_light(const QBrush& brush) {
			m_disabled_light = brush;
		}

		QBrush get_disabled_midlight() {
			if (!m_disabled_midlight)
				return QBrush();

			return *m_disabled_midlight;
		}

		void set_disabled_midlight(const QBrush& brush) {
			m_disabled_midlight = brush;
		}

		QBrush get_disabled_dark() {
			if (!m_disabled_dark)
				return QBrush();

			return *m_disabled_dark;
		}

		void set_disabled_dark(const QBrush& brush) {
			m_disabled_dark = brush;
		}

		QBrush get_disabled_mid() {
			if (!m_disabled_mid)
				return QBrush();

			return *m_disabled_mid;
		}

		void set_disabled_mid(const QBrush& brush) {
			m_disabled_mid = brush;
		}

		QBrush get_disabled_text() {
			if (!m_disabled_text)
				return QBrush();

			return *m_disabled_text;
		}

		void set_disabled_text(const QBrush& brush) {
			m_disabled_text = brush;
		}

		QBrush get_disabled_brighttext() {
			if (!m_disabled_brighttext)
				return QBrush();

			return *m_disabled_brighttext;
		}

		void set_disabled_brighttext(const QBrush& brush) {
			m_disabled_brighttext = brush;
		}

		QBrush get_disabled_buttontext() {
			if (!m_disabled_buttontext)
				return QBrush();

			return *m_disabled_buttontext;
		}

		void set_disabled_buttontext(const QBrush& brush) {
			m_disabled_buttontext = brush;
		}

		QBrush get_disabled_base() {
			if (!m_disabled_base)
				return QBrush();

			return *m_disabled_base;
		}

		void set_disabled_base(const QBrush& brush) {
			m_disabled_base = brush;
		}

		QBrush get_disabled_window() {
			if (!m_disabled_window)
				return QBrush();

			return *m_disabled_window;
		}

		void set_disabled_window(const QBrush& brush) {
			m_disabled_window = brush;
		}

		QBrush get_disabled_shadow() {
			if (!m_disabled_shadow)
				return QBrush();

			return *m_disabled_shadow;
		}

		void set_disabled_shadow(const QBrush& brush) {
			m_disabled_shadow = brush;
		}

		QBrush get_disabled_highlight() {
			if (!m_disabled_highlight)
				return QBrush();

			return *m_disabled_highlight;
		}

		void set_disabled_highlight(const QBrush& brush) {
			m_disabled_highlight = brush;
		}

		QBrush get_disabled_highlightedtext() {
			if (!m_disabled_highlightedtext)
				return QBrush();

			return *m_disabled_highlightedtext;
		}

		void set_disabled_highlightedtext(const QBrush& brush) {
			m_disabled_highlightedtext = brush;
		}

		QBrush get_disabled_link() {
			if (!m_disabled_link)
				return QBrush();

			return *m_disabled_link;
		}

		void set_disabled_link(const QBrush& brush) {
			m_disabled_link = brush;
		}

		QBrush get_disabled_linkvisited() {
			if (!m_disabled_linkvisited)
				return QBrush();

			return *m_disabled_linkvisited;
		}

		void set_disabled_linkvisited(const QBrush& brush) {
			m_disabled_linkvisited = brush;
		}

		QBrush get_disabled_alternatebase() {
			if (!m_disabled_alternatebase)
				return QBrush();

			return *m_disabled_alternatebase;
		}

		void set_disabled_alternatebase(const QBrush& brush) {
			m_disabled_alternatebase = brush;
		}

		QBrush get_disabled_tooltipbase() {
			if (!m_disabled_tooltipbase)
				return QBrush();

			return *m_disabled_tooltipbase;
		}

		void set_disabled_tooltipbase(const QBrush& brush) {
			m_disabled_tooltipbase = brush;
		}

		QBrush get_disabled_tooltiptext() {
			if (!m_disabled_tooltiptext)
				return QBrush();

			return *m_disabled_tooltiptext;
		}

		void set_disabled_tooltiptext(const QBrush& brush) {
			m_disabled_tooltiptext = brush;
		}

		QBrush get_inactive_windowtext() {
			if (!m_inactive_windowtext)
				return QBrush();

			return *m_inactive_windowtext;
		}

		void set_inactive_windowtext(const QBrush& brush) {
			m_inactive_windowtext = brush;
		}

		QBrush get_inactive_button() {
			if (!m_inactive_button)
				return QBrush();

			return *m_inactive_button;
		}

		void set_inactive_button(const QBrush& brush) {
			m_inactive_button = brush;
		}

		QBrush get_inactive_light() {
			if (!m_inactive_light)
				return QBrush();

			return *m_inactive_light;
		}

		void set_inactive_light(const QBrush& brush) {
			m_inactive_light = brush;
		}

		QBrush get_inactive_midlight() {
			if (!m_inactive_midlight)
				return QBrush();

			return *m_inactive_midlight;
		}

		void set_inactive_midlight(const QBrush& brush) {
			m_inactive_midlight = brush;
		}

		QBrush get_inactive_dark() {
			if (!m_inactive_dark)
				return QBrush();

			return *m_inactive_dark;
		}

		void set_inactive_dark(const QBrush& brush) {
			m_inactive_dark = brush;
		}

		QBrush get_inactive_mid() {
			if (!m_inactive_mid)
				return QBrush();

			return *m_inactive_mid;
		}

		void set_inactive_mid(const QBrush& brush) {
			m_inactive_mid = brush;
		}

		QBrush get_inactive_text() {
			if (!m_inactive_text)
				return QBrush();

			return *m_inactive_text;
		}

		void set_inactive_text(const QBrush& brush) {
			m_inactive_text = brush;
		}

		QBrush get_inactive_brighttext() {
			if (!m_inactive_brighttext)
				return QBrush();

			return *m_inactive_brighttext;
		}

		void set_inactive_brighttext(const QBrush& brush) {
			m_inactive_brighttext = brush;
		}

		QBrush get_inactive_buttontext() {
			if (!m_inactive_buttontext)
				return QBrush();

			return *m_inactive_buttontext;
		}

		void set_inactive_buttontext(const QBrush& brush) {
			m_inactive_buttontext = brush;
		}

		QBrush get_inactive_base() {
			if (!m_inactive_base)
				return QBrush();

			return *m_inactive_base;
		}

		void set_inactive_base(const QBrush& brush) {
			m_inactive_base = brush;
		}

		QBrush get_inactive_window() {
			if (!m_inactive_window)
				return QBrush();

			return *m_inactive_window;
		}

		void set_inactive_window(const QBrush& brush) {
			m_inactive_window = brush;
		}

		QBrush get_inactive_shadow() {
			if (!m_inactive_shadow)
				return QBrush();

			return *m_inactive_shadow;
		}

		void set_inactive_shadow(const QBrush& brush) {
			m_inactive_shadow = brush;
		}

		QBrush get_inactive_highlight() {
			if (!m_inactive_highlight)
				return QBrush();

			return *m_inactive_highlight;
		}

		void set_inactive_highlight(const QBrush& brush) {
			m_inactive_highlight = brush;
		}

		QBrush get_inactive_highlightedtext() {
			if (!m_inactive_highlightedtext)
				return QBrush();

			return *m_inactive_highlightedtext;
		}

		void set_inactive_highlightedtext(const QBrush& brush) {
			m_inactive_highlightedtext = brush;
		}

		QBrush get_inactive_link() {
			if (!m_inactive_link)
				return QBrush();

			return *m_inactive_link;
		}

		void set_inactive_link(const QBrush& brush) {
			m_inactive_link = brush;
		}

		QBrush get_inactive_linkvisited() {
			if (!m_inactive_linkvisited)
				return QBrush();

			return *m_inactive_linkvisited;
		}

		void set_inactive_linkvisited(const QBrush& brush) {
			m_inactive_linkvisited = brush;
		}

		QBrush get_inactive_alternatebase() {
			if (!m_inactive_alternatebase)
				return QBrush();

			return *m_inactive_alternatebase;
		}

		void set_inactive_alternatebase(const QBrush& brush) {
			m_inactive_alternatebase = brush;
		}

		QBrush get_inactive_tooltipbase() {
			if (!m_inactive_tooltipbase)
				return QBrush();

			return *m_inactive_tooltipbase;
		}

		void set_inactive_tooltipbase(const QBrush& brush) {
			m_inactive_tooltipbase = brush;
		}

		QBrush get_inactive_tooltiptext() {
			if (!m_inactive_tooltiptext)
				return QBrush();

			return *m_inactive_tooltiptext;
		}

		void set_inactive_tooltiptext(const QBrush& brush) {
			m_inactive_tooltiptext = brush;
		}

		
	public slots:
		void updateApplicationPalette() {
			qWarning() << "Updating application palette";
			
			QPalette palette = QApplication::palette();


			if (m_active_windowtext) {
				palette.setBrush(QPalette::Active, QPalette::WindowText, *m_active_windowtext);
			}

			if (m_active_button) {
				palette.setBrush(QPalette::Active, QPalette::Button, *m_active_button);
			}

			if (m_active_light) {
				palette.setBrush(QPalette::Active, QPalette::Light, *m_active_light);
			}

			if (m_active_midlight) {
				palette.setBrush(QPalette::Active, QPalette::Midlight, *m_active_midlight);
			}

			if (m_active_dark) {
				palette.setBrush(QPalette::Active, QPalette::Dark, *m_active_dark);
			}

			if (m_active_mid) {
				palette.setBrush(QPalette::Active, QPalette::Mid, *m_active_mid);
			}

			if (m_active_text) {
				palette.setBrush(QPalette::Active, QPalette::Text, *m_active_text);
			}

			if (m_active_brighttext) {
				palette.setBrush(QPalette::Active, QPalette::BrightText, *m_active_brighttext);
			}

			if (m_active_buttontext) {
				palette.setBrush(QPalette::Active, QPalette::ButtonText, *m_active_buttontext);
			}

			if (m_active_base) {
				palette.setBrush(QPalette::Active, QPalette::Base, *m_active_base);
			}

			if (m_active_window) {
				palette.setBrush(QPalette::Active, QPalette::Window, *m_active_window);
			}

			if (m_active_shadow) {
				palette.setBrush(QPalette::Active, QPalette::Shadow, *m_active_shadow);
			}

			if (m_active_highlight) {
				palette.setBrush(QPalette::Active, QPalette::Highlight, *m_active_highlight);
			}

			if (m_active_highlightedtext) {
				palette.setBrush(QPalette::Active, QPalette::HighlightedText, *m_active_highlightedtext);
			}

			if (m_active_link) {
				palette.setBrush(QPalette::Active, QPalette::Link, *m_active_link);
			}

			if (m_active_linkvisited) {
				palette.setBrush(QPalette::Active, QPalette::LinkVisited, *m_active_linkvisited);
			}

			if (m_active_alternatebase) {
				palette.setBrush(QPalette::Active, QPalette::AlternateBase, *m_active_alternatebase);
			}

			if (m_active_tooltipbase) {
				palette.setBrush(QPalette::Active, QPalette::ToolTipBase, *m_active_tooltipbase);
			}

			if (m_active_tooltiptext) {
				palette.setBrush(QPalette::Active, QPalette::ToolTipText, *m_active_tooltiptext);
			}

			if (m_disabled_windowtext) {
				palette.setBrush(QPalette::Disabled, QPalette::WindowText, *m_disabled_windowtext);
			}

			if (m_disabled_button) {
				palette.setBrush(QPalette::Disabled, QPalette::Button, *m_disabled_button);
			}

			if (m_disabled_light) {
				palette.setBrush(QPalette::Disabled, QPalette::Light, *m_disabled_light);
			}

			if (m_disabled_midlight) {
				palette.setBrush(QPalette::Disabled, QPalette::Midlight, *m_disabled_midlight);
			}

			if (m_disabled_dark) {
				palette.setBrush(QPalette::Disabled, QPalette::Dark, *m_disabled_dark);
			}

			if (m_disabled_mid) {
				palette.setBrush(QPalette::Disabled, QPalette::Mid, *m_disabled_mid);
			}

			if (m_disabled_text) {
				palette.setBrush(QPalette::Disabled, QPalette::Text, *m_disabled_text);
			}

			if (m_disabled_brighttext) {
				palette.setBrush(QPalette::Disabled, QPalette::BrightText, *m_disabled_brighttext);
			}

			if (m_disabled_buttontext) {
				palette.setBrush(QPalette::Disabled, QPalette::ButtonText, *m_disabled_buttontext);
			}

			if (m_disabled_base) {
				palette.setBrush(QPalette::Disabled, QPalette::Base, *m_disabled_base);
			}

			if (m_disabled_window) {
				palette.setBrush(QPalette::Disabled, QPalette::Window, *m_disabled_window);
			}

			if (m_disabled_shadow) {
				palette.setBrush(QPalette::Disabled, QPalette::Shadow, *m_disabled_shadow);
			}

			if (m_disabled_highlight) {
				palette.setBrush(QPalette::Disabled, QPalette::Highlight, *m_disabled_highlight);
			}

			if (m_disabled_highlightedtext) {
				palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, *m_disabled_highlightedtext);
			}

			if (m_disabled_link) {
				palette.setBrush(QPalette::Disabled, QPalette::Link, *m_disabled_link);
			}

			if (m_disabled_linkvisited) {
				palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, *m_disabled_linkvisited);
			}

			if (m_disabled_alternatebase) {
				palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, *m_disabled_alternatebase);
			}

			if (m_disabled_tooltipbase) {
				palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, *m_disabled_tooltipbase);
			}

			if (m_disabled_tooltiptext) {
				palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, *m_disabled_tooltiptext);
			}

			if (m_inactive_windowtext) {
				palette.setBrush(QPalette::Inactive, QPalette::WindowText, *m_inactive_windowtext);
			}

			if (m_inactive_button) {
				palette.setBrush(QPalette::Inactive, QPalette::Button, *m_inactive_button);
			}

			if (m_inactive_light) {
				palette.setBrush(QPalette::Inactive, QPalette::Light, *m_inactive_light);
			}

			if (m_inactive_midlight) {
				palette.setBrush(QPalette::Inactive, QPalette::Midlight, *m_inactive_midlight);
			}

			if (m_inactive_dark) {
				palette.setBrush(QPalette::Inactive, QPalette::Dark, *m_inactive_dark);
			}

			if (m_inactive_mid) {
				palette.setBrush(QPalette::Inactive, QPalette::Mid, *m_inactive_mid);
			}

			if (m_inactive_text) {
				palette.setBrush(QPalette::Inactive, QPalette::Text, *m_inactive_text);
			}

			if (m_inactive_brighttext) {
				palette.setBrush(QPalette::Inactive, QPalette::BrightText, *m_inactive_brighttext);
			}

			if (m_inactive_buttontext) {
				palette.setBrush(QPalette::Inactive, QPalette::ButtonText, *m_inactive_buttontext);
			}

			if (m_inactive_base) {
				palette.setBrush(QPalette::Inactive, QPalette::Base, *m_inactive_base);
			}

			if (m_inactive_window) {
				palette.setBrush(QPalette::Inactive, QPalette::Window, *m_inactive_window);
			}

			if (m_inactive_shadow) {
				palette.setBrush(QPalette::Inactive, QPalette::Shadow, *m_inactive_shadow);
			}

			if (m_inactive_highlight) {
				palette.setBrush(QPalette::Inactive, QPalette::Highlight, *m_inactive_highlight);
			}

			if (m_inactive_highlightedtext) {
				palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, *m_inactive_highlightedtext);
			}

			if (m_inactive_link) {
				palette.setBrush(QPalette::Inactive, QPalette::Link, *m_inactive_link);
			}

			if (m_inactive_linkvisited) {
				palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, *m_inactive_linkvisited);
			}

			if (m_inactive_alternatebase) {
				palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, *m_inactive_alternatebase);
			}

			if (m_inactive_tooltipbase) {
				palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, *m_inactive_tooltipbase);
			}

			if (m_inactive_tooltiptext) {
				palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, *m_inactive_tooltiptext);
			}


			QApplication::setPalette(palette);
		}
		
	protected:
		bool event(QEvent *event) Q_DECL_OVERRIDE {
			bool result = QWidget::event(event);
			
			if (event->type() == QEvent::StyleChange) {
				// Update global palette. Have to defer it
				// as property updates are also signals.
				QTimer::singleShot(0, this, SLOT(updateApplicationPalette()));
			}
			
			return result;
		}
	private:
		boost::optional<QBrush> m_active_windowtext;
		boost::optional<QBrush> m_active_button;
		boost::optional<QBrush> m_active_light;
		boost::optional<QBrush> m_active_midlight;
		boost::optional<QBrush> m_active_dark;
		boost::optional<QBrush> m_active_mid;
		boost::optional<QBrush> m_active_text;
		boost::optional<QBrush> m_active_brighttext;
		boost::optional<QBrush> m_active_buttontext;
		boost::optional<QBrush> m_active_base;
		boost::optional<QBrush> m_active_window;
		boost::optional<QBrush> m_active_shadow;
		boost::optional<QBrush> m_active_highlight;
		boost::optional<QBrush> m_active_highlightedtext;
		boost::optional<QBrush> m_active_link;
		boost::optional<QBrush> m_active_linkvisited;
		boost::optional<QBrush> m_active_alternatebase;
		boost::optional<QBrush> m_active_tooltipbase;
		boost::optional<QBrush> m_active_tooltiptext;
		boost::optional<QBrush> m_disabled_windowtext;
		boost::optional<QBrush> m_disabled_button;
		boost::optional<QBrush> m_disabled_light;
		boost::optional<QBrush> m_disabled_midlight;
		boost::optional<QBrush> m_disabled_dark;
		boost::optional<QBrush> m_disabled_mid;
		boost::optional<QBrush> m_disabled_text;
		boost::optional<QBrush> m_disabled_brighttext;
		boost::optional<QBrush> m_disabled_buttontext;
		boost::optional<QBrush> m_disabled_base;
		boost::optional<QBrush> m_disabled_window;
		boost::optional<QBrush> m_disabled_shadow;
		boost::optional<QBrush> m_disabled_highlight;
		boost::optional<QBrush> m_disabled_highlightedtext;
		boost::optional<QBrush> m_disabled_link;
		boost::optional<QBrush> m_disabled_linkvisited;
		boost::optional<QBrush> m_disabled_alternatebase;
		boost::optional<QBrush> m_disabled_tooltipbase;
		boost::optional<QBrush> m_disabled_tooltiptext;
		boost::optional<QBrush> m_inactive_windowtext;
		boost::optional<QBrush> m_inactive_button;
		boost::optional<QBrush> m_inactive_light;
		boost::optional<QBrush> m_inactive_midlight;
		boost::optional<QBrush> m_inactive_dark;
		boost::optional<QBrush> m_inactive_mid;
		boost::optional<QBrush> m_inactive_text;
		boost::optional<QBrush> m_inactive_brighttext;
		boost::optional<QBrush> m_inactive_buttontext;
		boost::optional<QBrush> m_inactive_base;
		boost::optional<QBrush> m_inactive_window;
		boost::optional<QBrush> m_inactive_shadow;
		boost::optional<QBrush> m_inactive_highlight;
		boost::optional<QBrush> m_inactive_highlightedtext;
		boost::optional<QBrush> m_inactive_link;
		boost::optional<QBrush> m_inactive_linkvisited;
		boost::optional<QBrush> m_inactive_alternatebase;
		boost::optional<QBrush> m_inactive_tooltipbase;
		boost::optional<QBrush> m_inactive_tooltiptext;

};

#endif // APPLICATIONPALETTE_H
