// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TALKINGUICOMPONENT_H_
#define MUMBLE_MUMBLE_TALKINGUICOMPONENT_H_

class QWidget;
class MultiStyleWidgetWrapper;

class TalkingUIComponent {
public:
	virtual QWidget *getWidget()                         = 0;
	virtual const QWidget *getWidget() const             = 0;
	virtual MultiStyleWidgetWrapper &getStylableWidget() = 0;
};

#endif // MUMBLE_MUMBLE_TALKINGUICOMPONENT_H_
