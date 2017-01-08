// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_BONJOURCLIENT_H_
#define MUMBLE_MUMBLE_BONJOURCLIENT_H_

#include <QtCore/QObject>

class BonjourServiceBrowser;
class BonjourServiceResolver;

class BonjourClient : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(BonjourClient)
	public:
		BonjourClient();
		~BonjourClient() Q_DECL_OVERRIDE;

		BonjourServiceBrowser *bsbBrowser;
		BonjourServiceResolver *bsrResolver;
};

#endif
