/* Copyright (C) 2015 Stefan Hacker <dd0t@users.sourceforge.net>

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
#ifndef MUMBLE_MUMBLE_THEMES_H_
#define MUMBLE_MUMBLE_THEMES_H_

#include <ThemeInfo.h>
#include <Settings.h>
#ifndef Q_MOC_RUN
#include <boost/optional.hpp>
#endif

class Themes {
public:
	/// Returns the style configured in the given settings structure
	static boost::optional<ThemeInfo::StyleInfo> getConfiguredStyle(const Settings& settings);
	
	/// Updates the given settings object to be configured to the given style
	/// 
	/// @note Does not apply the theme @see apply
	/// 
	/// @param settings Settings object to update
	/// @param style Style to set
	/// @param outChanged Will be set to true if the style in settings actually changed. Will not be changed otherwise.
	static void setConfiguredStyle(Settings& settings, boost::optional<ThemeInfo::StyleInfo> style, bool& outChanged);
	
	/// Applies the theme
	/// 
	/// @note Can only apply a theme before MainWindow etc. is opened
	static bool apply();
	
	/// Return a theme name to theme map
	static ThemeMap getThemes();

private:
	/// Applies the fallback stylesheet
	static void applyFallback();
	
	/// Tries to apply the configured theme.
	/// @return True on success. False on failure.
	static bool applyConfigured();
	
	/// Returns list of theme search directories ordered ascending by priorty (lowest first)
	static QVector<QDir> getSearchDirectories();
	
	/// Returns default style-sheet used for fall-backs
	static QString getDefaultStylesheet();
};

#endif // MUMBLE_MUMBLE_THEMES_H_
