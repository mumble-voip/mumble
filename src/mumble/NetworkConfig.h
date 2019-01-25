// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_NETWORKCONFIG_H_
#define MUMBLE_MUMBLE_NETWORKCONFIG_H_

class QNetworkReply;
class QNetworkRequest;
class QUrl;

#include "ConfigDialog.h"
#include "ui_NetworkConfig.h"

class NetworkConfig : public ConfigWidget, Ui::NetworkConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(NetworkConfig)
	public:
		NetworkConfig(Settings &st);
		virtual QString title() const Q_DECL_OVERRIDE;
		virtual QIcon icon() const Q_DECL_OVERRIDE;
		static void SetupProxy();
		static bool TcpModeEnabled();
	public slots:
		void accept() const Q_DECL_OVERRIDE;
		void save() const Q_DECL_OVERRIDE;
		void load(const Settings &r) Q_DECL_OVERRIDE;

		void on_qcbType_currentIndexChanged(int v);
};

namespace Network {
	void prepareRequest(QNetworkRequest &);
	QNetworkReply *get(const QUrl &);
}

#endif
