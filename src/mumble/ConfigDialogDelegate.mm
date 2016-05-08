// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// This file implements a NSToolbar delegate used for the Mumble
// preferences dialog on Mac OS X. 

#include "mumble_pch.hpp"

#import "ConfigDialogDelegate.h"

#define OSX_TOOLBAR_ICON_SIZE 32  // Currently, Mac OS X NSToolbars use 32x32 items when in 'normal' mode.

static NSString *ConfigDialogDelegate_QString_to_AutoreleasedNSString(const QString& string) {
	return [const_cast<NSString *>(reinterpret_cast<const NSString *>(CFStringCreateWithCharacters(kCFAllocatorDefault,
	                                reinterpret_cast<const UniChar *>(string.unicode()), string.length()))) autorelease];
}

// Private Qt symbol: gui/kernel/qt_cocoa_helpers_mac.mm
extern NSImage *qt_mac_create_nsimage(const QPixmap &pm);

static NSImage *ConfigDialogDelegate_QIcon_to_AutoreleasedNSImage(const QIcon &icon) {
	QPixmap pm = icon.pixmap(OSX_TOOLBAR_ICON_SIZE, OSX_TOOLBAR_ICON_SIZE);
	NSImage *nimg = qt_mac_create_nsimage(pm);
	return [nimg autorelease];
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
