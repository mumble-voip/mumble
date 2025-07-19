#include "ArgumentParser.h"

#include "ThemeInfo.h"
#include <QDir>
#include <QString>
#include <QStringList>


ArgumentParser::ArgumentParser(const QStringList &arguments) : args(arguments) {
	parse();
}

std::optional< QString > ArgumentParser::getNextArg(int currentIndex) const {
	if (currentIndex + 1 < args.count()) {
		return args.at(currentIndex + 1);
	}
	return std::nullopt;
}

Arguments ArgumentParser::parse() {
	Arguments result;
	for (int i = 1; i < args.count(); ++i) {
		const QString &arg = args.at(i);

		result.help                        = result.help || isHelpArgument(arg);
		result.version                     = result.version || isVersionArgument(arg);
		result.allowMultiple               = result.allowMultiple || isAllowMultiple(arg);
		result.suppressIdentity            = result.suppressIdentity || isSuppressIdentity(arg);
		result.license                     = result.license || isLicense(arg);
		result.authors                     = result.authors || isAuthors(arg);
		result.thirdPartyLicenses          = result.thirdPartyLicenses || isThirdPartyLicenses(arg);
		result.printEchocancelQueue        = result.printEchocancelQueue || isPrintEchocancelQueue(arg);
		result.hidden                      = result.hidden || isHidden(arg);
		result.printTranslationDirectories = result.printTranslationDirectories || isPrintTranslationDirectories(arg);

		if (auto value = isJackName(arg, getNextArg(i))) {
			result.jackName = *value;
			++i;
		}

		if (auto value = isRpc(arg, getNextArg(i))) {
			result.rpc = *value;
			++i;
		}

		if (auto value = isWindowTitlePostFix(arg, getNextArg(i))) {
			result.windowTitlePostFix = *value;
			++i;
		}

		if (auto value = isLocale(arg, getNextArg(i))) {
			result.locale = *value;
			++i;
		}
		if (auto value = isTranslationDirectory(arg, getNextArg(i))) {
			result.translationDirectory = *value;
			++i;
		}
		if (auto value = isDefaultCertificateDirectory(arg, getNextArg(i))) {
			result.defaultCertificateDirectory = *value;
			++i;
		}
	}
	return result;
}


bool ArgumentParser::isPrintTranslationDirectories(const QString &arg) const {
	return arg == QLatin1String("--print-translation-dirs");
}

bool ArgumentParser::isHelpArgument(const QString &arg) const {
	return (arg == QLatin1String("-h") || arg == QLatin1String("--help")
#if defined(Q_OS_WIN)
			|| arg == QLatin1String("/?")
#endif
	);
}

bool ArgumentParser::isVersionArgument(const QString &arg) const {
	return arg == QLatin1String("--version");
}

bool ArgumentParser::isAllowMultiple(const QString &arg) const {
	return arg == QLatin1String("-m") || arg == QLatin1String("--multiple");
}

bool ArgumentParser::isSuppressIdentity(const QString &arg) const {
	return arg == QLatin1String("-n") || arg == QLatin1String("--noidentity");
}

bool ArgumentParser::isLicense(const QString &arg) const {
	return arg == QLatin1String("-license") || arg == QLatin1String("--license");
}

bool ArgumentParser::isAuthors(const QString &arg) const {
	return arg == QLatin1String("-authors") || arg == QLatin1String("--authors");
}

bool ArgumentParser::isThirdPartyLicenses(const QString &arg) const {
	return arg == QLatin1String("-third-party-licenses") || arg == QLatin1String("--third-party-licenses");
}

bool ArgumentParser::isPrintEchocancelQueue(const QString &arg) const {
	return arg == QLatin1String("--dump-input-streams");
}

bool ArgumentParser::isHidden(const QString &arg) const {
	return arg == QLatin1String("--hidden");
}

std::optional< QString > ArgumentParser::isJackName(const QString &arg, const std::optional< QString > &value) const {
	if (arg == QLatin1String("-jn") || arg == QLatin1String("--jackname")) {
		if (value) {
			return *value;
		} else {
			qCritical("Missing argument for --jackname!");
		}
	};
	return std::nullopt;
}

std::optional< QString > ArgumentParser::isWindowTitlePostFix(const QString &arg,
															  const std::optional< QString > &value) const {
	if (arg == QLatin1String("--window-title-ext")) {
		if (value) {
			return *value;
		} else {
			qCritical("Missing argument for --window-title-ext!");
		}
	};
	return std::nullopt;
}


std::optional< QString > ArgumentParser::isRpc(const QString &arg, const std::optional< QString > &value) const {
	if (arg == QLatin1String("rpc")) {
		if (value) {
			return *value;
		} else {
			qCritical("Error: No RPC command specified");
		}
	};
	return std::nullopt;
}


std::optional< QString > ArgumentParser::isLocale(const QString &arg, const std::optional< QString > &value) const {
	if (arg == QLatin1String("--locale")) {
		if (value) {
			return *value;
		} else {
			qCritical("Missing argument for --locale!");
		}
	};
	return std::nullopt;
}

std::optional< QString > ArgumentParser::isTranslationDirectory(const QString &arg,
																const std::optional< QString > &value) const {
	if (arg == QLatin1String("--translation-dir")) {
		if (value) {
			return *value;
		} else {
			qCritical("Missing argument for --translation-dir!");
		}
	};
	return std::nullopt;
}
std::optional< QDir > ArgumentParser::isDefaultCertificateDirectory(const QString &arg,
																	const std::optional< QString > &value) const {
	if (arg == QLatin1String("--default-certificate-dir")) {
		if (value) {
			QDir qdCert = QDir(*value);
			// I suppose we should really be checking whether the directory is writable here too,
			// but there are some subtleties with doing that:
			// (doc.qt.io/qt-5/qfile.html#platform-specific-issues)
			// so we can just let things fail down below when this directory is used.
			if (!qdCert.exists()) {
				qCritical("Directory %s does not exist", qUtf8Printable(*value));
			}
		} else {
			qCritical("Missing argument for --default-certificate-dir!");
		}
	};
	return std::nullopt;
}

QStringList ArgumentParser::getArguments() const {
	return args;
}
