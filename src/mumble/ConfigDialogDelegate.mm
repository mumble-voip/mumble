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
 * This file implements a NSToolbar delegate used for the Mumble
 * preferences dialog on Mac OS X. 
 */

#import "ConfigDialogDelegate.h"

#define OSX_TOOLBAR_ICON_SIZE 32  /* Currently, Mac OS X NSToolbars use 32x32 items when in 'normal' mode. */

static const NSString *ConfigDialogDelegate_QString_to_NSString(const QString& string) {
	return reinterpret_cast<const NSString *>(CFStringCreateWithCharacters(kCFAllocatorDefault,
	                                reinterpret_cast<const UniChar *>(string.unicode()), string.length()));
}

static NSImage *ConfigDialogDelegate_QIcon_to_NSImage(const QIcon &icon) {
	CGImageRef img = icon.pixmap(OSX_TOOLBAR_ICON_SIZE, OSX_TOOLBAR_ICON_SIZE).toMacCGImageRef();
	NSBitmapImageRep *bmp = [[NSBitmapImageRep alloc] initWithCGImage:img];
	NSImage *nimg = [[NSImage alloc] init];
	[nimg addRepresentation:bmp];
	[bmp release];
	return nimg;
}

@implementation ConfigDialogDelegate

- (id)initWithConfigDialog: (ConfigDialog *)dialog andWidgetMap: (QMap<unsigned int, ConfigWidget *> *)map inExpertMode: (BOOL)flag
{
	self = [super init];

	configDialog = dialog;
	widgetMap = map;
	inExpertMode = flag;

	NSUInteger numItems = widgetMap->count();
	nameWidgetMapping = [NSMutableDictionary dictionaryWithCapacity:numItems];
	identifiers = [[NSMutableArray alloc] initWithCapacity:numItems];

	foreach (ConfigWidget *cw, *widgetMap) {
		if (cw->expert(inExpertMode) || inExpertMode) {
			NSString *str = ConfigDialogDelegate_QString_to_NSString(cw->title());
			NSValue *ptr = [NSValue valueWithPointer: cw];
			[nameWidgetMapping setObject:ptr forKey:str];
			[identifiers addObject:str];
		}
	}

	[nameWidgetMapping retain];
	[identifiers retain];

	return self;
}

- (NSArray *) toolbarAllowedItemIdentifiers: (NSToolbar *)toolbar {
	return identifiers;
}

- (NSArray *) toolbarDefaultItemIdentifiers: (NSToolbar *)toolbar {
	return identifiers;
}

- (NSArray *) toolbarSelectableItemIdentifiers: (NSToolbar *)toolbar {
	return identifiers;
}

- (void) toolbarWillAddItem: (NSNotification*)notification {
}

- (void) toolbarDidRemoveItem: (NSNotification *)notification {
	NSToolbarItem *item = [[notification userInfo] valueForKey: @"item"];
	[item release];
}

- (NSToolbarItem *) toolbar: (NSToolbar *)toolbar itemForItemIdentifier: (NSString *)identifier willBeInsertedIntoToolbar: (BOOL)flag {
	ConfigWidget *cw = reinterpret_cast<ConfigWidget *>([[nameWidgetMapping valueForKey: identifier] pointerValue]);

	NSToolbarItem *item = [[NSToolbarItem alloc] initWithItemIdentifier:identifier];
	[item setLabel: identifier];
	[item setImage: ConfigDialogDelegate_QIcon_to_NSImage(cw->icon())]; 
	[item setTarget: self];
	[item setAction: @selector(itemSelected:)];

	/* Cache our current toolbar if it isn't already. */
	if (toolbarCache == nil) {
		toolbarCache = toolbar;
	}

	return item;
}

/* Our NSToolbar delegate is also the target of our NSToolbarItems. As such we must respond to
   validation requests from the toolbar items.  We have no special needs so we just allow them
   all through. */
- (BOOL) validateToolbarItem: (NSToolbarItem *) toolbarItem {
	return YES;
}

- (void) selectItem: (ConfigWidget *)cw {
	NSString *str = ConfigDialogDelegate_QString_to_NSString(cw->title());
	[toolbarCache setSelectedItemIdentifier: str];
	[str release];
}

- (void) itemSelected: (NSToolbarItem *)toolbarItem {
	NSString *identifier = [toolbarItem itemIdentifier];
	NSValue *val = [nameWidgetMapping valueForKey: identifier];
	ConfigWidget *cw = reinterpret_cast<ConfigWidget *>([val pointerValue]);
	if (cw) {
		configDialog->on_widgetSelected(cw);
	}
}

- (void) dealloc {

	if (toolbarCache != nil) {
		int nitems = [[toolbarCache items] count];
		while (nitems > 0) {
			[toolbarCache removeItemAtIndex: 0];
			--nitems;
		}
	}

	[nameWidgetMapping release];
	[identifiers release];

	[super dealloc];
}

@end
