// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ZEROCONF_H_
#define MUMBLE_MUMBLE_ZEROCONF_H_

#include "BonjourServiceBrowser.h"
#include "BonjourServiceResolver.h"

class Zeroconf : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Zeroconf)
public:
	Zeroconf();

	QScopedPointer< BonjourServiceBrowser > bsbBrowser;
	QScopedPointer< BonjourServiceResolver > bsrResolver;
};

#endif
