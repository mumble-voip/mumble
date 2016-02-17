#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <http://www.mumble.info/LICENSE>.

from __future__ import (unicode_literals, print_function, division)

import sys
import subprocess
import codecs
import locale

blacklist = (
	# Unknown
	"(no author) <(no author)@05730e5d-ab1b-0410-a4ac-84af385074fa>",
	"root <root@DiskStation.(none)>",
	"unknown <LoL@.(none)>",
	"unknown <d0t@.(none)>",

	# Bots
	"MumbleTransifexBot <mumbletransifexbot@mumble.info>",

	# Aliases of Bartek "stranded" Sumowski <sumowski@gmail.com>
	"Bartek <sumowski@users.sourceforge.net>",
	"Bartek Sumowksi <sumowski@gmail.com>",

	# Alias of Benjamin Jemlich <pcgod@users.sourceforge.net>
	"Benjamin Jemlich <pcgod@gmx.net>",

	# Alias of EarlOfWenc <lorenz.schwittmann@gmail.com>
	"EarlOfWenc <user@localhost>",

	# Aliases of Jamie Fraser <fwagglechop@gmail.com> -- latest commit uses this email.a
	"Jamie Fraser <jamie.f@mumbledog.com>",
	"Jamie Fraser <jamie.f@sabrienix.com>",

	# Aliases of Álvaro Manuel Recio Pérez <naproxeno@users.sourceforge.net>
	"Álvaro M. Recio Pérez <naproxeno@users.sourceforge.net>",
	"Álvaro Manuel Recio Pérez <naproxeno@kepis.(none)>",

	# Aliases of Thorvald Natvig <slicer@users.sourceforge.net>
	"Thorvald Natvig <github@natvig.com>",
	"Thorvald Natvig <slicer@users.sourceforge.net>",
	"Thorvald Natvig <thorvald@-e false.(none)>",
	"Thorvald Natvig <thorvald@debian.localdomain>",
	"Thorvald Natvig <thorvald@natvig.com>",

	# Alias of Spaccaossi <spaccaossi@gmail.com>
	"Spaccaossi <spaccaossi@users.sourceforge.net>",

	# Aliases of Stefan Hacker <dd0t@users.sourceforge.net>
	"Stefan H <dd0t@users.sourceforge.net>",
	"Stefan Hacker <hacker.stefan@googlemail.com>",
	"Stefan Hacker <hast@hast-desktop.(none)>",

	# Aliases of Michał "Zuko" Żukowski <zuczeq@gmail.com>
	"Michał Żukowski <zuczeq@gmail.com>",
	"Zukowski Michal <zuczeq@gmail.com>",
	"Żukowski Michał <zuczeq@gmail.com>",
	"zuczeq <zuczeq@users.sourceforge.net>",
	"Zuko <zuczeq@gmail.com>",

	# Two authors?! Both are listed elsewhere, so drop.
	"Michael Ziegler and Natenom <natenom@googlemail.com>",

	# ...What?
	"Netbios Domain Administrator <admin@gameserver2.(none)>",

	# Aliases of Mikkel Krautz <mikkel@krautz.dk>
	"Mikkel <mikkel@krautz.dk>",
	"Mikkel Krautz <mkrautz@users.sourceforge.net>",

	# Alias of tkmorris <mauricioarozi@gmail.com>
	"morris <tkmorris@users.sourceforge.net>",

	# Alias of bendem <online@bendem.be>
	"bendem <bendembd@gmail.com>",

	# Alias of arrai <array.of.intellect@gmail.com>
	"Arrai <arrai@users.sourceforge.net>",

	# Alias of Joël Troch <joel.troch62@gmail.com>
	"Joël Troch <https://github.com/JoelTroch>",

	# Matthias Vogelgesang <matthias.vogelgesang@gmail.com>
	"Matthias Vogelgesang <m0ta@users.sourceforge.net>",

	# Alias of Tuck Therebelos <snares@users.sourceforge.net>
	"Snares <snares@users.sourceforge.net>",

	# Aliases of Natenom <natenom@natenom.com>
	"Natenom <natenom@googlemail.com>",
	"Natenom <natenom@natenom.name>",

	# Alias of Arcenciel <arcenciel@users.sourceforge.net>
	"arcenciel <arcenciel@users.sourceforge.net>",

	# Alias of Jonathan E. Hansen <zentriple@users.sourceforge.net>
	"Jonathan <zentriple@users.sourceforge.net>",
	"zentriple <zentriple@users.sourceforge.net>",
	"Zentriple <zentriple@users.sourceforge.net>",


	# Alias of Patrick Matthäi <pmatthaei@debian.org>
	"Patrick Matthäi <the-me88@users.sourceforge.net>",

	# Alias of Jan Klass <kissaki@gmx.de>
	"Jan Klass <kissaki0@gmail.com>",

	# Alias of Necromancer <necro3@users.sourceforge.net>
	"Necromancer <necromancer3@users.sourceforge.net>",

	# Alias of Svenne33 <svenne33@users.sourceforge.net>
	"svenne33 <svenne33@users.sourceforge.net>",

	# Potentially an alias of BAYSSE Laurent <lolo_32@users.sourceforge.net>
	"lolo_32 <Alex@.(none)>",
)

def gitAuthorsOutput():
	p = subprocess.Popen(["git", "log", "--all", "--format=%aN <%aE>"], stdout=subprocess.PIPE)
	stdout, stderr = p.communicate()
	if stdout is not None:
		stdout = stdout.decode('utf-8')
	if stderr is not None:
		stderr = stderr.decode('utf-8')
	if p.returncode != 0:
		raise Exception('"git log" failed: %s', stderr)
	return stdout

def main():
	locale.setlocale(locale.LC_ALL, "")

	authorsSet = set()
	authorsText = gitAuthorsOutput()
	for line in authorsText.split("\n"):
		if line == '':
			continue

		if line == "zapman <unknown>":
			line = "zapman"

		# Fix "=?UTF-8 Michał Żukowski?=" and possibly others like it.
		if line.startswith("=?UTF-8 "):
			line = line.replace("=?UTF-8 ", "")
			line = line.replace("?=", "")

		# Use GitHub URL instead of $user@users.noreply.github.com
		if '@users.noreply.github.com' in line:
			line = line.replace('@users.noreply.github.com', '')
			line = line.replace('<', '<https://github.com/')

		if line in blacklist:
			continue

		authorsSet.add(line)

	f = codecs.open("AUTHORS", "w", "utf-8")
	f.write('''// This is the official list of people who have contributed
// to, and hold the copyright to Mumble.
//
// The use of Mumble source code is governed by a BSD-style
// license that can be found in the LICENSE file at the root
// of the Mumble source tree or at <http://www.mumble.info/LICENSE>.
//
// Contributions made on behalf of another entity, such as a
// company are indicated with the following suffix:
//
//     John Doe <jd@mumble.info> (on behalf of $COMPANY)
//
// It is possible to mix individual contributions with company
// contributions. For example, if a contributor, over time,
// has contributed code copyrighted by the contributor, as well
// as various companies:
//
//     John Doe <jd@mumble.info> (individually, on behalf of
//                                $COMPANY1, on behalf of
//                                $COMPANY2, [...]).

''')

	# Sort alphabetically
	authors = list(authorsSet)
	if sys.version[0] == '2': # Python 2
		authors.sort(cmp=locale.strcoll)
	else:
		authors.sort(key=locale.strxfrm)

	for author in authors:
		f.write(author)
		f.write("\n")

	f.close()

if __name__ == '__main__':
	main()
