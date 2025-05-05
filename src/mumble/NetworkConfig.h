// Copyright The Mumble Developers. All rights reserved.
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
	/// The unique name of this ConfigWidget
	static const QString name;
	NetworkConfig(Settings &st);
	virtual QString title() const Q_DECL_OVERRIDE;
	virtual const QString &getName() const Q_DECL_OVERRIDE;
	virtual QIcon icon() const Q_DECL_OVERRIDE;
	static void SetupProxy();
	static bool TcpModeEnabled();
public slots:
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;

	void on_qcbType_currentIndexChanged(int v);
#ifdef NO_UPDATE_CHECK
	void on_qcbAutoUpdate_stateChanged(int state);
#endif
};

namespace Network {
void prepareRequest(QNetworkRequest &);
QNetworkReply *get(const QUrl &);
} // namespace Network

#endif
