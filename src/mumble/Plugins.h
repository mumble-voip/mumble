// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PLUGINS_H_
#define MUMBLE_MUMBLE_PLUGINS_H_

#include "ConfigDialog.h"

#include "ui_Plugins.h"

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/QUrl>

struct PluginInfo;

class PluginConfig : public ConfigWidget, public Ui::PluginConfig {
private:
	Q_OBJECT
	Q_DISABLE_COPY(PluginConfig)
protected:
	void refillPluginList();
	PluginInfo *pluginForItem(QTreeWidgetItem *) const;

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	PluginConfig(Settings &st);
	virtual QString title() const Q_DECL_OVERRIDE;
	const QString &getName() const Q_DECL_OVERRIDE;
	virtual QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
	void on_qpbConfig_clicked();
	void on_qpbAbout_clicked();
	void on_qpbReload_clicked();
	void on_qtwPlugins_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
};

struct PluginFetchMeta;

class Plugins : public QObject {
	friend class PluginConfig;

private:
	Q_OBJECT
	Q_DISABLE_COPY(Plugins)
protected:
	QReadWriteLock qrwlPlugins;
	QMutex qmPluginStrings;
	QList< PluginInfo * > qlPlugins;
	PluginInfo *locked;
	PluginInfo *prevlocked;
	void clearPlugins();
	int iPluginTry;
	QMap< QString, PluginFetchMeta > qmPluginFetchMeta;
	QString qsSystemPlugins;
	QString qsUserPlugins;
#ifdef Q_OS_WIN
	HANDLE hToken;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious;
#endif
public:
	std::string ssContext, ssContextSent;
	std::wstring swsIdentity, swsIdentitySent;
	bool bValid;
	bool bUnlink;
	float fPosition[3], fFront[3], fTop[3];
	float fCameraPosition[3], fCameraFront[3], fCameraTop[3];

	Plugins(QObject *p = nullptr);
	~Plugins() Q_DECL_OVERRIDE;
public slots:
	void on_Timer_timeout();
	void rescanPlugins();
	bool fetch();
	void checkUpdates();
	void fetchedUpdatePAPlugins(QByteArray, QUrl);
	void fetchedPAPluginDL(QByteArray, QUrl);
};

#endif
