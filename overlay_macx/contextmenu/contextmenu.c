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

/*
 * Mac OS X context menu plugin for launching applications with the
 * Mumble overaly enabled.
 */

#include "contextmenu.h"

#define MUMBLE_OVERLAY_LAUNCHER_PATH    MUMBLE_OVERLAY_MACX_PATH "/mumble-overlay"

static ContextualMenuInterfaceStruct interfaceTable = {
	NULL,

	ContextMenuQueryInterface,
	ContextMenuAddRef,
	ContextMenuRelease,

	ContextMenuExamineContext,
	ContextMenuHandleSelection,
	ContextMenuPostMenuCleanup,
};

void *ContextMenuFactory(CFAllocatorRef alloc, CFUUIDRef typeID) {
	if (CFEqual(typeID, kContextualMenuTypeID)) {
		ContextMenuPlugin *p;
		p = ContextMenuAllocateInstance(kContextMenuFactoryId);
		return p;
	}
	return NULL;
}

static HRESULT ContextMenuQueryInterface(ContextMenuPlugin *p, REFIID iid, LPVOID *ppv) {
	CFUUIDRef interfaceId = CFUUIDCreateFromUUIDBytes(NULL, iid);
	if (CFEqual(interfaceId, kContextualMenuInterfaceID) || CFEqual(interfaceId, IUnknownUUID)) {
		ContextMenuAddRef(p);
		*ppv = p;
		CFRelease(interfaceId);
		return S_OK;
	} else {
		*ppv = NULL;
		CFRelease(interfaceId);
		return E_NOINTERFACE;
	}
}

static ULONG ContextMenuAddRef(ContextMenuPlugin *p) {
	return ++p->refCount;
}

static ULONG ContextMenuRelease(ContextMenuPlugin *p) {
	--p->refCount;

	if (p->refCount == 0) {
		ContextMenuDeallocateInstance(p);
		return 0;
	}

	return p->refCount;	
}

static ContextMenuPlugin *ContextMenuAllocateInstance(CFUUIDRef factoryId) {
	ContextMenuPlugin *p;
	p = malloc(sizeof(ContextMenuPlugin));
	p->cmInterface = &interfaceTable;
	p->factoryId = CFRetain(factoryId);
	CFPlugInAddInstanceForFactory(factoryId);
	ContextMenuAddRef(p);
	return p;
}

static void ContextMenuDeallocateInstance(ContextMenuPlugin *p) {
	CFPlugInRemoveInstanceForFactory(p->factoryId);
	CFRelease(p->factoryId);
	free(p);
}

static OSStatus ContextMenuExamineContext(ContextMenuPlugin *p, const AEDesc *desc, AEDescList *cmds) {
	AERecord commandRecord = { typeNull, NULL };
	OSStatus err;

	if (!ContextMenuAEDescIsAppBundle(desc))
		return noErr;

	err = AECreateList(NULL, 0, true, &commandRecord);
	if (err != noErr)
		goto err;

	char *str = "Launch with Mumble Overlay";
	err = AEPutKeyPtr(&commandRecord, keyAEName, typeUTF8Text, str, strlen(str)+1);
	if (err != noErr)
		goto err;

	SInt32 commandId = 'OLAY';
	if (commandId != NULL) {
		err = AEPutKeyPtr(&commandRecord, keyContextualMenuCommandID, typeLongInteger, &commandId, sizeof(commandId));
		if (err != noErr)
			goto err;
	}

	err = AEPutDesc(cmds, 0, &commandRecord);

err:
	AEDisposeDesc (&commandRecord);
	return err;
}

static OSStatus ContextMenuHandleSelection(ContextMenuPlugin *p, AEDesc *desc, SInt32 commandId) {
	CFBundleRef bundle = NULL;
	CFURLRef bundleUrl = NULL;
	CFStringRef bundleFn = NULL;
	CFArrayRef argv = NULL;
	OSStatus err = noErr;
	FSRef ref;

	if (commandId != 'OLAY')
		return noErr;

	err = FSPathMakeRef((const UInt8 *) MUMBLE_OVERLAY_LAUNCHER_PATH, &ref, NULL);
	if (err != noErr)
		return noErr;

	bundle = ContextMenuGetAppBundleFromDescList(desc);
	if (!bundle)
		goto out;

	bundleUrl = CFBundleCopyBundleURL(bundle);
	if (!bundleUrl)
		goto out;

	bundleFn = CFURLCopyFileSystemPath(bundleUrl, kCFURLPOSIXPathStyle);
	if (!bundleFn)
		goto out;

	CFStringRef array[] = { bundleFn };
	argv = CFArrayCreate(kCFAllocatorDefault, array, 1, NULL);
	if (!argv)
		goto out;

	LSApplicationParameters parm = {
		.version             = 0,
		.flags               = kLSLaunchDefaults,
		.application         = &ref,
		.asyncLaunchRefCon   = NULL,
		.environment         = NULL,
		.argv                = argv,
		.initialEvent        = NULL
	};

	err = LSOpenApplication(&parm, NULL);
	if (err != noErr)
		goto out;

out:
	if (bundle)
		CFRelease(bundle);
	if (bundleUrl)
		CFRelease(bundleUrl);
	if (bundleFn)
		CFRelease(bundleFn);
	if (argv)
		CFRelease(argv);
	return noErr;
}

static void ContextMenuPostMenuCleanup(ContextMenuPlugin *p) {
}

/* -- */

static CFBundleRef ContextMenuGetAppBundleFromDescList(const AEDesc *desc) {
	CFBundleRef bundle;
	CFURLRef fileUrl;
	AEKeyword kw;
	AEDesc item;
	OSStatus err;
	FSRef fileRef;
	int nItems;

	if (desc->descriptorType != 'list')
		return NULL;

	err = AECountItems(desc, &nItems);
	if (err != noErr)
		return NULL;

	if (nItems > 1)
		return NULL;

	err = AEGetNthDesc(desc, 1, typeWildCard, &kw, &item);
	if (err != noErr)
		return NULL;

	if (item.descriptorType != typeFSRef)
		return NULL;

	err = AEGetDescData(&item, &fileRef, AEGetDescDataSize(&item));
	if (err != noErr)
		return NULL;

	fileUrl = CFURLCreateFromFSRef(kCFAllocatorDefault, &fileRef);
	if (!fileUrl)
		return NULL;

	bundle = CFBundleCreate(kCFAllocatorDefault, fileUrl);
	if (!bundle)
		return NULL;
	CFRelease(fileUrl);

	UInt32 pkgType = 0, pkgCreator = 0;
	CFStringRef pkgIdentifier = NULL;

	pkgIdentifier = CFBundleGetIdentifier(bundle);
	CFBundleGetPackageInfo(bundle, &pkgType, &pkgCreator);

	if (!(pkgType == 'APPL' && pkgIdentifier != NULL)) {
		CFRelease(bundle);
		return NULL;
	}

	return bundle;
}

static bool ContextMenuAEDescIsAppBundle(const AEDesc *desc) {
	CFBundleRef bundle = ContextMenuGetAppBundleFromDescList(desc);
	if (bundle) {
		CFRelease(bundle);
		return true;
	}

	return false;
}
