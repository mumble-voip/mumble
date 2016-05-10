// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CONFIGDIALOG_DELEGATE_H_
#define MUMBLE_MUMBLE_CONFIGDIALOG_DELEGATE_H_

#include "ConfigDialog_macx.h"
#include "ConfigWidget.h"
#import <Cocoa/Cocoa.h>

@class NSToolbarItem;

@interface ConfigDialogDelegate : NSObject
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1060
<NSToolbarDelegate>
#endif
- (id) initWithConfigDialog:(ConfigDialogMac *)dialog andToolbar:(NSToolbar *)toolbar andWidgetMap:(QMap<unsigned int, ConfigWidget *> *)map inExpertMode:(BOOL)isExpertMode;
- (void) selectItem:(ConfigWidget *)cw;
- (BOOL) expertMode;
@end

#endif
