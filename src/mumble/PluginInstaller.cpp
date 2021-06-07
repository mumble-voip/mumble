// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PluginInstaller.h"
#include "Global.h"

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

	if (fileInfo.suffix().compare(PluginInstaller::pluginFileExtension, Qt::CaseInsensitive) == 0
		|| fileInfo.suffix().compare(QLatin1String("zip"), Qt::CaseInsensitive) == 0) {
		// A plugin file has either the extension given in PluginInstaller::pluginFileExtension or zip
		return true;
	}

	// We might also accept a shared library directly
	return QLibrary::isLibrary(fileInfo.fileName());
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

			// Iterate over all files in the archive to see which ones could be the correct plugin library
			QString pluginName;
			auto it = archive.fileInfoBegin();
			while (it != archive.fileInfoEnd()) {
				QString currentFileName = QString::fromStdString(it->first);

				if (QLibrary::isLibrary(currentFileName)) {
					if (!pluginName.isEmpty()) {
						// There seem to be multiple plugins in here. That's not allowed
						throw PluginInstallException(
							tr("Found more than one plugin library for the current OS in \"%1\" (\"%2\" and \"%3\")!")
								.arg(m_pluginArchive.fileName())
								.arg(pluginName)
								.arg(currentFileName));
					}

					pluginName = currentFileName;
				}

				it++;
			}

			if (pluginName.isEmpty()) {
				throw PluginInstallException(
					tr("Unable to find a plugin for the current OS in \"%1\"").arg(m_pluginArchive.fileName()));
			}

			// Unpack the plugin library into the tmp dir
			// We don't have to create the directory structure as we're only interested in the library itself
			QString tmpPluginPath = QDir::temp().filePath(QFileInfo(pluginName).fileName());
			auto pluginIt         = archive.findHeader(pluginName.toStdString());
			zipInput.clear();
			Poco::Zip::ZipInputStream zipin(zipInput, pluginIt->second);
			std::ofstream out(tmpPluginPath.toStdString(), std::ios::out | std::ios::binary);
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
			.arg(pluginVersion == VERSION_UNKNOWN ? "Unknown" : static_cast< QString >(pluginVersion))
			.arg(usedAPIVersion == VERSION_UNKNOWN ? "Unknown" : static_cast< QString >(usedAPIVersion)));

	qlAuthor->setText(m_plugin->getAuthor());

	qlDescription->setText(m_plugin->getDescription());
}

void PluginInstaller::install() const {
	if (!m_plugin) {
		// This function shouldn't even be called, if the plugin object has not been created...
		throw PluginInstallException(QLatin1String("[INTERNAL ERROR]: Trying to install an invalid plugin"));
	}

	if (m_pluginSource == m_pluginDestination) {
		// Apparently the plugin is already installed
		return;
	}

	if (m_pluginDestination.exists()) {
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
}

QString PluginInstaller::getInstallDir() {
	// Get the path to the plugin-dir in "user-land" (aka: the user definitely has write access to this
	// location).
	return Global::get().qdBasePath.absolutePath() + QLatin1String("/Plugins");
}

void PluginInstaller::on_qpbYesClicked() {
	install();

	accept();
}

void PluginInstaller::on_qpbNoClicked() {
	close();
}
