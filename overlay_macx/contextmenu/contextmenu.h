/* Copyright (C) 2009, Mikkel Krautz <mikkel@krautz.dk>

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

#ifndef __CONTEXTMENU_H__
#define __CONTEXTMENU_H__

#include <stdlib.h>
#include <unistd.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFPlugInCOM.h>

#include <CoreServices/CoreServices.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

/*
 * UUID for context menu:
 * 27630515-2D3E-4103-8AD7-32F8272C4C2D
 */
#define kContextMenuFactoryId  CFUUIDGetConstantUUIDWithBytes(NULL, \
                                                              0x27, 0x63, 0x05, 0x15, \
                                                              0x2D, 0x3E, 0x41, 0x03, \
                                                              0x8A, 0xD7, 0x32, 0xF8, \
                                                              0x27, 0x2C, 0x4C, 0x2D)

typedef struct _ContextMenuPlugin {
	ContextualMenuInterfaceStruct *cmInterface;
	CFUUIDRef factoryId;
	ULONG refCount;
} ContextMenuPlugin;

void *ContextMenuFactory(CFAllocatorRef alloc, CFUUIDRef typeID);

static HRESULT ContextMenuQueryInterface(ContextMenuPlugin *p, REFIID iid, LPVOID *ppv);
static ULONG ContextMenuAddRef(ContextMenuPlugin *p);
static ULONG ContextMenuRelease(ContextMenuPlugin *p);
static OSStatus ContextMenuExamineContext(ContextMenuPlugin *p, const AEDesc *ctx, AEDescList *cmds);
static OSStatus ContextMenuHandleSelection(ContextMenuPlugin *p, AEDesc *ctx, SInt32 commandId);
static void ContextMenuPostMenuCleanup(ContextMenuPlugin *p);

static ContextMenuPlugin *ContextMenuAllocateInstance(CFUUIDRef factoryId);
static void ContextMenuDeallocateInstance(ContextMenuPlugin *p);

static CFBundleRef ContextMenuGetAppBundleFromDescList(const AEDesc *desc);
static bool ContextMenuAEDescIsAppBundle(const AEDesc *desc);

#endif /* __CONTEXTMENU_H__ */
