:: Copyright 2012-2021 The Mumble Developers. All rights reserved.
:: Use of this source code is governed by a BSD-style license
:: that can be found in the LICENSE file at the root of the
:: Mumble source tree or at <https://www.mumble.info/LICENSE>.

:: Copy murmur.ini to murmur.ini.win32, and convert
:: LF -> CRLF so Windows users can edit murmur.ini in
:: Notepad.
::
:: Note that the 'more' command also expands tabs
:: to spaces. This isn't a problem right now, since
:: we don't use tabs in murmur.ini -- and even if
:: we did, they wouldn't be significant.
type murmur.ini | more /p > murmur.ini.win32
