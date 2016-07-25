// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGIN_H_
#define MUMBLE_MUMBLE_PLUGIN_H_

#include <string>
#include <map>

// Visual Studio 2008 x86
#if _MSC_VER == 1500 && defined(_M_IX86)
# define MUMBLE_PLUGIN_MAGIC     0xd63ab7ef
# define MUMBLE_PLUGIN_MAGIC_2   0xd63ab7fe
# define MUMBLE_PLUGIN_MAGIC_QT  0xd63ab7ee
// Visual Studio 2010 x86
#elif _MSC_VER == 1600 && defined(_M_IX86)
# define MUMBLE_PLUGIN_MAGIC    0xd63ab7f0
# define MUMBLE_PLUGIN_MAGIC_2  0xd63ab7ff
# define MMUBLE_PLUGIN_MAGIC_QT 0xd63ab70f
// Visual Studio 2013 x86
#elif _MSC_VER == 1800 && defined(_M_IX86)
# define MUMBLE_PLUGIN_MAGIC    0xd63ab7c0
# define MUMBLE_PLUGIN_MAGIC_2  0xd63ab7cf
# define MUMBLE_PLUGIN_MAGIC_QT 0xd63ab7ca
// Visual Studio 2013 x64
#elif _MSC_VER == 1800 && defined(_M_X64)
# define MUMBLE_PLUGIN_MAGIC    0x9f3ed4c0
# define MUMBLE_PLUGIN_MAGIC_2  0x9f3ed4cf
# define MUMBLE_PLUGIN_MAGIC_QT 0x9f3ed4ca
// Generic plugin magic values for platforms
// where we do not officially plugins other
// than "link".
#else
# define MUMBLE_PLUGIN_MAGIC    0xf4573570
# define MUMBLE_PLUGIN_MAGIC_2  0xf457357f
# define MUMBLE_PLUGIN_MAGIC_QT 0xf457357a
#endif

#define MUMBLE_PLUGIN_VERSION 2

typedef struct _MumblePlugin {
	unsigned int magic;
	const std::wstring &description;
	const std::wstring &shortname;
	void (__cdecl *about)(HWND);
	void (__cdecl *config)(HWND);
	int (__cdecl *trylock)();
	void (__cdecl *unlock)();
	const std::wstring(__cdecl *longdesc)();
	int (__cdecl *fetch)(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity);
} MumblePlugin;

typedef struct _MumblePlugin2 {
	unsigned int magic;
	unsigned int version;
	int (__cdecl *trylock)(const std::multimap<std::wstring, unsigned long long int> &);
} MumblePlugin2;

/// MumblePluginQt provides an extra set of functions that will
/// provide a plugin with a pointer to a QWidget that should be
/// used as the parent for any dialogs Qt widgets shown by the
/// plugin.
///
/// This interface should only be used if a plugin intends to
/// present Qt-based dialogs to the user.
///
/// This interface is most useful for plugins that are internal
/// to Mumble. This is because plugins can't integrate with Mumble's
/// QWidgets unless they're built into Mumble itself.
typedef struct _MumblePluginQt {
	unsigned int magic;

	/// about is called when Mumble requests the plugin
	/// to show an about dialog.
	///
	/// The ptr argument is a pointer to a QWidget that
	/// should be used as the parent for a Qt-based
	/// about dialog.
	void (__cdecl *about)(void *ptr);

	/// config is called when Mumble requests the plugin
	/// to show its configuration dialog.
	///
	/// The ptr argument is a pointer to a QWidget that
	/// should be used as the parent for a Qt-based
	/// configuration dialog.
	void (__cdecl *config)(void *ptr);
} MumblePluginQt;

typedef MumblePlugin *(__cdecl *mumblePluginFunc)();
typedef MumblePlugin2 *(__cdecl *mumblePlugin2Func)();
typedef MumblePluginQt *(__cdecl *mumblePluginQtFunc)();

/*
 * All plugins must implement one function called mumbleGetPlugin(), which
 * follows the mumblePluginFunc type and returns a MumblePlugin struct.
 *
 * magic should be initialized to MUMBLE_PLUGIN_MAGIC. description is the
 * name of the plugin shown in the GUI, while shortname is used for TTS.
 *
 * The individual functions are:
 * about(HWND parent) - Player clicked the about button over plugin
 * config(HWND parent) - Player clicked the config button
 * trylock() - Search system for likely process and try to lock on.
 *      The parameter is a set of process names and associated PIDs.
 *		Return 1 if succeeded, else 0. Note that once a plugin is locked on,
 *		no other plugins will be queried.
 * unlock() - Unlock from process. Either from user intervention or
 *		because fetch failed.
 * fetch(...) - Fetch data from locked process. avatar_pos is position in
 *		world coordinates (1 meter per unit). avatar_front and avatar_top
 *      specify the heading of the player, as in where he is looking.
 *		You need at minimum to figure out pos and front, otherwise
 *		sounds cannot be placed. If you do not fetch top, make it the
 *		same as front but rotated 90 degrees "upwards".
 *
 *      camera_x is the same, but for the camera. Make this identical to the
 *      avatar position if you don't know (or if it's a 1st person
 *      perspective).
 *
 *		It is important that you set all fields to 0.0 if you can't
 *		fetch meaningfull values, like between rounds and such.
 *
 *      context and identity are transmitted to the server. Only players
 *      with identical context will hear positional audio from each other.
 *      Mumble will automatically prepend the shortname of the plugin to
 *      the context, so make this a representation of the game server and
 *      team the player is on.
 *
 *      identity is retained by the server and is pollable over Ice/DBus,
 *      to be used by external scripts. This should uniquiely identify the
 *      player inside the game.
 *
 *      ctx_len and id_len are initialized to the bufferspace available. Set these
 *      to -1 to keep the previous value (as parsing and optimizing can be CPU
 *      intensive)
 *
 *		The function should return 1 if it is still "locked on",
 *		otherwise it should return 0. Mumble will call unlock()
 *		if it return 0, and go back to polling with trylock()
 *		once in a while.
 */

#endif
