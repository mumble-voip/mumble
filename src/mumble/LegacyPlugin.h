// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_LEGACY_PLUGIN_H_
#define MUMBLE_MUMBLE_LEGACY_PLUGIN_H_

#include "Plugin.h"

#include <QtCore/QString>

#include <memory>
#include <string>

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

class LegacyPlugin;

/// Typedef for a LegacyPlugin pointer
typedef std::shared_ptr< LegacyPlugin > legacy_plugin_ptr_t;
/// Typedef for a const LegacyPlugin pointer
typedef std::shared_ptr< const LegacyPlugin > const_legacy_plugin_ptr_t;


/// This class is meant for compatibility for old Mumble "plugins" that stem from before the plugin framework has been
/// introduced. Thus the "plugins" represented by this class are for positional data gathering only.
class LegacyPlugin : public Plugin {
	friend class Plugin; // needed in order for Plugin::createNew to access LegacyPlugin::doInitialize()
private:
	Q_OBJECT
	Q_DISABLE_COPY(LegacyPlugin)

protected:
	/// The name of the "plugin"
	QString m_name;
	/// The description of the "plugin"
	QString m_description;
	/// The Version of the "plugin"
	mumble_version_t m_version;
	/// A pointer to the PluginStruct in its initial version. After initialization this
	/// field is effectively const and therefore it is not needed to protect read-access by a lock.
	MumblePlugin *m_mumPlug;
	/// A pointer to the PluginStruct in its second, enhanced version. After initialization this
	/// field is effectively const and therefore it is not needed to protect read-access by a lock.
	MumblePlugin2 *m_mumPlug2;
	/// A pointer to the PluginStruct that encorporates Qt functionality. After initialization this
	/// field is effectively const and therefore it is not needed to protect read-access by a lock.
	MumblePluginQt *m_mumPlugQt;

	virtual void resolveFunctionPointers() override;
	virtual bool doInitialize() override;

	LegacyPlugin(QString path, bool isBuiltIn = false, QObject *p = 0);

	virtual bool showAboutDialog(QWidget *parent) const override;
	virtual bool showConfigDialog(QWidget *parent) const override;
	virtual uint8_t initPositionalData(const char *const *programNames, const uint64_t *programPIDs,
									   size_t programCount) override;
	virtual bool fetchPositionalData(Position3D &avatarPos, Vector3D &avatarDir, Vector3D &avatarAxis,
									 Position3D &cameraPos, Vector3D &cameraDir, Vector3D &cameraAxis, QString &context,
									 QString &identity) const override;
	virtual void shutdownPositionalData() override;

public:
	virtual ~LegacyPlugin() override;

	virtual mumble_error_t init() override;

	// functions for direct plugin-interaction
	virtual QString getName() const override;

	virtual QString getDescription() const override;
	virtual uint32_t getFeatures() const override;
	virtual mumble_version_t getAPIVersion() const override;

	virtual mumble_version_t getVersion() const override;

	// functions for checking which underlying plugin functions are implemented
	virtual bool providesAboutDialog() const override;
	virtual bool providesConfigDialog() const override;
};

#endif
