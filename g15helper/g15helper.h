/* Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef __G15_HELPER_H__
#define __G15_HELPER_H__

#define G15_MAX_DEV         5
#define G15_MAX_WIDTH       160
#define G15_MAX_HEIGHT      43
#define G15_MAX_BPP         1
#define G15_MAX_FBMEM       (G15_MAX_WIDTH * G15_MAX_HEIGHT * G15_MAX_BPP)
#define G15_MAX_FBMEM_BITS  (G15_MAX_FBMEM / 8)
#if defined(WIN32)
#define G15_WIDGET_NAME     L"Mumble G15 Display"
#elif defined(APPLE)
#define G15_WIDGET_NAME     CFSTR("Mumble G15 Display")
#endif

enum {
	G15_ERR_INIT = 1,
	G15_ERR_CONNECT,
	G15_ERR_ENUMERATE,
	G15_ERR_OPEN,
	G15_ERR_READFILE,
	G15_ERR_CLOSE,
	G15_ERR_DISCONNECT,
	G15_ERR_DEINIT,
};

#endif /* __G15_HELPER_H__ */
