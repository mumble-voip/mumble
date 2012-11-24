/* Copyright (C) 2009-2012, Mikkel Krautz <mikkel@krautz.dk>

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

// This file implements a NSToolbar delegate used for the Mumble
// preferences dialog on Mac OS X. 

#import "ConfigDialogDelegate.h"

#define OSX_TOOLBAR_ICON_SIZE 32  // Currently, Mac OS X NSToolbars use 32x32 items when in 'normal' mode.

static NSString *ConfigDialogDelegate_QString_to_AutoreleasedNSString(const QString& string) {
	return [const_cast<NSString *>(reinterpret_cast<const NSString *>(CFStringCreateWithCharacters(kCFAllocatorDefault,
	                                reinterpret_cast<const UniChar *>(string.unicode()), string.length()))) autorelease];
}

static NSImage *ConfigDialogDelegate_QIcon_to_AutoreleasedNSImage(const QIcon &icon) {
	CGImageRef img = icon.pixmap(OSX_TOOLBAR_ICON_SIZE, OSX_TOOLBAR_ICON_SIZE).toMacCGImageRef();
	NSBitmapImageRep *bmp = [[[NSBitmapImageRep alloc] initWithCGImage:img] autorelease];
	CGImageRelease(img);

	NSImage *nimg = [[[NSImage alloc] init] autorelease];
	[nimg addRepresentation:bmp];
	return nimg;
}

@interface ConfigDialogDelegate () {
	ConfigDialogMac                      *_configDialog;
	NSToolbar                            *_toolbar;
	QMap<unsigned int, ConfigWidget *>   *_widgetMap;
	BOOL                                 _expertMode;

   	NSMutableDictionary                  *_nameWidgetMapping;
	NSMutableArray                       *_identifiers;
}
- (NSToolbarItem *) toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)identifier willBeInsertedIntoToolbar:(BOOL)flag;
- (void) itemSelected:(NSToolbarItem *)toolbarItem;
- (void) expertSelected:(NSButton *)button;
@end

@implementation ConfigDialogDelegate

- (id) initWithConfigDialog:(ConfigDialogMac *)dialog andToolbar:(NSToolbar *)toolbar andWidgetMap:(QMap<unsigned int, ConfigWidget *> *)map inExpertMode:(BOOL)isExpertMode {
	if ((self = [super init])) {
		_configDialog = dialog;
		_toolbar = toolbar;
		_widgetMap = map;
		_expertMode = isExpertMode;

		NSUInteger numItems = _widgetMap->count() + 2;

		_nameWidgetMapping = [[NSMutableDictionary alloc] initWithCapacity:numItems];
		_identifiers = [[NSMutableArray alloc] initWithCapacity:numItems];

		foreach (ConfigWidget *cw, *_widgetMap) {
			if (cw->expert(_expertMode) || _expertMode) {
				NSString *str = ConfigDialogDelegate_QString_to_AutoreleasedNSString(cw->title());
				NSValue *ptr = [NSValue valueWithPointer:cw];
				[_nameWidgetMapping setObject:ptr forKey:str];
				[_identifiers addObject:str];
			}
		}

		[_identifiers addObject:NSToolbarFlexibleSpaceItemIdentifier];
		[_identifiers addObject:@"expertModeCheckbox"];
	}
	return self;
}

- (void) dealloc {
	[_nameWidgetMapping release];
	[_identifiers release];
	[super dealloc];
}

- (NSArray *) toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar {
	(void) toolbar;
	return _identifiers;
}

- (NSArray *) toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar {
	(void) toolbar;
	return _identifiers;
}

- (NSArray *) toolbarSelectableItemIdentifiers:(NSToolbar *)toolbar {
	(void) toolbar;
	return _identifiers;
}

- (NSToolbarItem *) toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)identifier willBeInsertedIntoToolbar:(BOOL)willBeInserted {
	(void) toolbar;
	(void) willBeInserted;

	// Special-case for the 'Expert Mode' checkbox.
	if ([identifier isEqualTo:@"expertModeCheckbox"]) {
		NSToolbarItem *item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];

		NSString *advanced = ConfigDialogDelegate_QString_to_AutoreleasedNSString(qApp->translate("ConfigDialog", "Advanced"));
		[item setLabel:advanced];

		NSButton *button = [[[NSButton alloc] init] autorelease];
		NSSize buttonSize = NSMakeSize(OSX_TOOLBAR_ICON_SIZE/2, OSX_TOOLBAR_ICON_SIZE);
		[button setButtonType:NSSwitchButton];
		[button setState:(_expertMode ? NSOnState : NSOffState)];
		[button setTitle:nil];
		[item setView:button];
		[item setMinSize:buttonSize];
		[item setMaxSize:buttonSize];
		[item setTarget:self];
		[item setAction:@selector(expertSelected:)];

		return item;
	}

	ConfigWidget *cw = reinterpret_cast<ConfigWidget *>([[_nameWidgetMapping valueForKey:identifier] pointerValue]);
	NSToolbarItem *item = [[[NSToolbarItem alloc] initWithItemIdentifier:identifier] autorelease];
	[item setLabel:identifier];
	NSImage *img = ConfigDialogDelegate_QIcon_to_AutoreleasedNSImage(cw->icon());
	[item setImage:img];
	[item setTarget:self];
	[item setAction:@selector(itemSelected:)];

	return item;
}

- (void) selectItem:(ConfigWidget *)cw {
	NSString *str = ConfigDialogDelegate_QString_to_AutoreleasedNSString(cw->title());
	[_toolbar setSelectedItemIdentifier:str];
}

- (void) itemSelected:(NSToolbarItem *)toolbarItem {
	NSString *identifier = [toolbarItem itemIdentifier];
	NSValue *val = [_nameWidgetMapping valueForKey:identifier];
	ConfigWidget *cw = reinterpret_cast<ConfigWidget *>([val pointerValue]);
	if (cw != NULL) {
		_configDialog->on_widgetSelected(cw);
	}
}

- (void) expertSelected:(NSButton *)button {
	_configDialog->updateExpert([button state] == NSOnState);
}

- (BOOL) expertMode {
	return _expertMode;
}

@end
