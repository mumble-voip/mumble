/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2010, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "Version.h"

unsigned int MumbleVersion::getRaw(const QString &version) {
	int major, minor, patch;

	if (get(&major, &minor, &patch, version))
		return toRaw(major, minor, patch);

	return 0;
}

bool MumbleVersion::get(int *major, int *minor, int *patch, const QString &version) {
	QRegExp rx(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)"));

	if (rx.exactMatch(version)) {
		if (major)
			*major = rx.cap(1).toInt();
		if (minor)
			*minor = rx.cap(2).toInt();
		if (patch)
			*patch = rx.cap(3).toInt();

		return true;
	}
	return false;
}

unsigned int MumbleVersion::toRaw(int major, int minor, int patch) {
	return (major << 16) | (minor << 8) | patch;
}

void MumbleVersion::fromRaw(unsigned int version, int *major, int *minor, int *patch) {
	*major = (version & 0xFFFF0000) >> 16;
	*minor = (version & 0xFF00) >> 8;
	*patch = (version & 0xFF);
}
