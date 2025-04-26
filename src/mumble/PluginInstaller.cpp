// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PluginInstaller.h"
#include "PluginManager.h"
#include "PluginManifest.h"
#include "QtUtils.h"
#include "Global.h"

#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QException>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QIcon>

#include <exception>
#include <fstream>
#include <string>

#include <Poco/Exception.h>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Zip/ZipArchive.h>
#include <Poco/Zip/ZipStream.h>

PluginInstallException::PluginInstallException(const QString &msg) : m_msg(msg) {
}

QString PluginInstallException::getMessage() const {
	return m_msg;
}

const QString PluginInstaller::pluginFileExtension = QLatin1String("mumble_plugin");

bool PluginInstaller::canBePluginFile(const QFileInfo &fileInfo) noexcept {
	if (!fileInfo.isFile()) {
		// A plugin file has to be a file (obviously)
		return false;
	}

	if (fileInfo.suffix().compare(PluginInstaller::pluginFileExtension, Qt::CaseInsensitive) == 0) {
		// A plugin file has the extension given in PluginInstaller::pluginFileExtension
		return true;
	}

	// We might also accept a shared library directly
	return QLibrary::isLibrary(fileInfo.fileName());
}

PluginInstaller::PluginInstaller(const QString &filePath, QWidget *p) : PluginInstaller(QFileInfo(filePath), p) {
}

PluginInstaller::PluginInstaller(const QFileInfo &fileInfo, QWidget *p)
	: QDialog(p), m_pluginArchive(fileInfo), m_plugin(nullptr), m_pluginSource(), m_pluginDestination(),
	  m_copyPlugin(false) {
	setupUi(this);

	setWindowIcon(QIcon(QLatin1String("skin:mumble.svg")));

	QObject::connect(qpbYes, &QPushButton::clicked, this, &PluginInstaller::on_qpbYesClicked);
	QObject::connect(qpbNo, &QPushButton::clicked, this, &PluginInstaller::on_qpbNoClicked);

	init();
}

PluginInstaller::~PluginInstaller() {
	if (m_plugin) {
		delete m_plugin;
	}
}

void PluginInstaller::init() {
	if (!PluginInstaller::canBePluginFile(m_pluginArchive)) {
		throw PluginInstallException(tr("The file \"%1\" is not a valid plugin file!").arg(m_pluginArchive.fileName()));
	}

	if (QLibrary::isLibrary(m_pluginArchive.fileName())) {
		// For a library the fileInfo provided is already the actual plugin library
		m_pluginSource = m_pluginArchive;

		m_copyPlugin = true;
	} else {
		// We have been provided with a zip-file
		try {
			Poco::FileInputStream zipInput(m_pluginArchive.filePath().toStdString());
			Poco::Zip::ZipArchive archive(zipInput);
			auto manifestIt = archive.findHeader("manifest.xml");

			if (manifestIt == archive.headerEnd()) {
				throw PluginInstallException(tr("Unable to locate the plugin manifest (manifest.xml)"));
			}

			zipInput.clear();
			Poco::Zip::ZipInputStream manifestStream(zipInput, manifestIt->second);

			PluginManifest manifest;
			try {
				manifest.parse(manifestStream);
			} catch (const PluginManifestException &e) {
				throw PluginInstallException(
					tr("Error while processing manifest: %1").arg(QString::fromUtf8(e.what())));
			}

			if (!manifest.specifiesPluginPath(MUMBLE_TARGET_OS, MUMBLE_TARGET_ARCH)) {
				throw PluginInstallException(
					tr("Unable to find plugin for the current OS (\"%1\") and architecture (\"%2\")")
						.arg(QString::fromUtf8(MUMBLE_TARGET_OS))
						.arg(QString::fromUtf8(MUMBLE_TARGET_ARCH)));
			}

			std::string pluginPath = manifest.getPluginPath(MUMBLE_TARGET_OS, MUMBLE_TARGET_ARCH);

			// Unpack the plugin library into the tmp dir
			// We don't have to create the directory structure as we're only interested in the library itself
			QString tmpPluginPath = QDir::temp().filePath(QFileInfo(QString::fromStdString(pluginPath)).fileName());
			auto pluginIt         = archive.findHeader(pluginPath);

			if (pluginIt == archive.headerEnd()) {
				throw PluginInstallException(
					tr("Unable to locate plugin library specified in manifest (\"%1\") in the bundle")
						.arg(QString::fromStdString(pluginPath)));
			}

			zipInput.clear();
			Poco::Zip::ZipInputStream zipin(zipInput, pluginIt->second);
			std::ofstream out(Mumble::QtUtils::qstring_to_path(tmpPluginPath), std::ios::out | std::ios::binary);
			Poco::StreamCopier::copyStream(zipin, out);

			m_pluginSource = QFileInfo(tmpPluginPath);
		} catch (const Poco::Exception &e) {
			// Something didn't work out during the Zip processing
			throw PluginInstallException(
				QString::fromStdString(std::string("Failed to process zip archive: ") + e.message()));
		}
	}

	QString pluginFileName = m_pluginSource.fileName();

	// Try to load the plugin up to see if it is actually valid
	try {
		m_plugin = Plugin::createNew< Plugin >(m_pluginSource.absoluteFilePath());
	} catch (const PluginError &) {
		throw PluginInstallException(
			tr("Unable to load plugin \"%1\" - check the plugin interface!").arg(pluginFileName));
	}

	m_pluginDestination = QFileInfo(QString::fromLatin1("%1/%2").arg(getInstallDir()).arg(pluginFileName));


	// Now that we located the plugin, it is time to fill in its details in the UI
	qlName->setText(m_plugin->getName());

	mumble_version_t pluginVersion  = m_plugin->getVersion();
	mumble_version_t usedAPIVersion = m_plugin->getAPIVersion();
	qlVersion->setText(
		QString::fromLatin1("%1 (API %2)")
			.arg(pluginVersion == MUMBLE_VERSION_UNKNOWN ? "Unknown" : static_cast< QString >(pluginVersion))
			.arg(usedAPIVersion == MUMBLE_VERSION_UNKNOWN ? "Unknown" : static_cast< QString >(usedAPIVersion)));

	qlAuthor->setText(m_plugin->getAuthor());

	qlDescription->setText(m_plugin->getDescription());
}

bool PluginInstaller::install() {
	if (!m_plugin) {
		// This function shouldn't even be called, if the plugin object has not been created...
		throw PluginInstallException(QLatin1String("[INTERNAL ERROR]: Trying to install an invalid plugin"));
	}

	if (m_pluginSource == m_pluginDestination) {
		// Apparently the plugin is already installed
		return false;
	}

	if (m_pluginDestination.exists()) {
		// This most likely means that we already have some version of this plugin installed. Figure out which
		// one that is and ask the user for confirmation for overwriting it.
		// NOTE: Because we currently have multiple install directories, this branch may not execute if the old
		// version of the plugin was installed into a different directory than the current one.
		const_plugin_ptr_t oldPlugin;
		for (const const_plugin_ptr_t &currentPlugin : Global::get().pluginManager->getPlugins()) {
			if (currentPlugin->getFilePath() == m_pluginDestination.absoluteFilePath()) {
				// This is the one
				oldPlugin = currentPlugin;
				break;
			}
		}

		if (oldPlugin) {
			QMessageBox::StandardButton result = QMessageBox::question(
				this, tr("Overwrite plugin?"),
				tr("The new plugin \"%1\" (%2) is about to overwrite the already installed plugin "
				   "\"%3\" (%4). Do you wish to proceed?")
					.arg(m_plugin->getName())
					.arg(QString(m_plugin->getVersion()))
					.arg(oldPlugin->getName())
					.arg(QString(oldPlugin->getVersion())));

			if (result != QMessageBox::StandardButton::Yes) {
				// Abort as the user did not specify that they want to proceed
				return false;
			}

			// If we proceed we have to make sure the plugin is unloaded as otherwise Mumble
			// could still hold a handle to the underlying library file which on some OS (e.g. Windows)
			// prevents it from being deleted/overwritten.
			Global::get().pluginManager->clearPlugin(oldPlugin->getID());

			// We have to let go of our handle of the plugin here as well in order to make sure it actually
			// gets deleted
			oldPlugin.reset();
		}

		// Delete old version first
		if (!QFile(m_pluginDestination.absoluteFilePath()).remove()) {
			throw PluginInstallException(
				tr("Unable to delete old plugin at \"%1\"").arg(m_pluginDestination.absoluteFilePath()));
		}
	}

	if (m_copyPlugin) {
		if (!QFile(m_pluginSource.absoluteFilePath()).copy(m_pluginDestination.absoluteFilePath())) {
			throw PluginInstallException(tr("Unable to copy plugin library from \"%1\" to \"%2\"")
											 .arg(m_pluginSource.absoluteFilePath())
											 .arg(m_pluginDestination.absoluteFilePath()));
		}
	} else {
		// Move the plugin into the respective dir
		if (!QFile(m_pluginSource.absoluteFilePath()).rename(m_pluginDestination.absoluteFilePath())) {
			throw PluginInstallException(
				tr("Unable to move plugin library to \"%1\"").arg(m_pluginDestination.absoluteFilePath()));
		}
	}

	return true;
}

QString PluginInstaller::getInstallDir() {
	// Get the path to the plugin-dir in "user-land" (aka: the user definitely has write access to this
	// location).
	return Global::get().qdBasePath.absolutePath() + QLatin1String("/Plugins");
}

void PluginInstaller::on_qpbYesClicked() {
	if (install()) {
		accept();
	} else {
		close();
	}
}

void PluginInstaller::on_qpbNoClicked() {
	close();
}
