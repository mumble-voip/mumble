/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _INTERNAL_OVERLAY_H
#define _INTERNAL_OVERLAY_H

#define OVERLAY_MAGIC_NUMBER 0x00000005

#define TEXT_WIDTH 600
#define TEXT_HEIGHT 60
#define NUM_TEXTS 32

#define TEXTURE_SIZE (TEXT_WIDTH*TEXT_HEIGHT*4)

#define OVERLAY_VERSION_MAJ 1
#define OVERLAY_VERSION_MIN 1
#define OVERLAY_VERSION_PATCH 7
#define OVERLAY_VERSION_SUB 0

struct TextEntry {
	unsigned int color;
	wchar_t text[128];
	unsigned int uiCounter;
	short width;
	unsigned char texture[TEXTURE_SIZE];
};

struct SharedMem {
	unsigned char version[4];
	unsigned int lastAppAlive;
	bool bHooked;
	bool bDebug;
	bool bShow;
	bool bReset;
	float fX, fY;
	bool bTop, bBottom, bLeft, bRight;
	float fFontSize;

	unsigned int uiAudioTick;
	float fPos[3];
	float fFront[3];
	float fTop[3];

	struct TextEntry texts[NUM_TEXTS];
};

#endif
