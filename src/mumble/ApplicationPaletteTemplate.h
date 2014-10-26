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

%(warning)s

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
class ApplicationPalette : public QWidget
{
		Q_OBJECT
%(properties)s
	public:
		explicit ApplicationPalette(QWidget *parent = 0)
		  : QWidget(parent)
		  , m_originalPalette(QApplication::palette()){
			// Empty
		}
		
%(getterssetters)s
		
	private slots:
		void updateApplicationPalette() {
			qWarning() << "Updating application palette";
			
			QPalette palette = m_originalPalette; // Do not re-use potentially already styled palette. Might not pick up system style changes though.

%(paletteupdates)s

			QApplication::setPalette(palette);
			resetAllProperties();
		}
		
		void resetAllProperties() {
%(propertyresets)s
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
		const QPalette m_originalPalette;
		
%(variables)s
};

#endif // APPLICATIONPALETTE_H
