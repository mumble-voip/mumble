// ArgumentParser.h
#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <QDir>
#include <QStringList>

struct Arguments {
	bool help                        = false;
	bool version                     = false;
	bool allowMultiple               = false;
	bool suppressIdentity            = false;
	bool license                     = false;
	bool authors                     = false;
	bool thirdPartyLicenses          = false;
	bool dumpInputStreams            = false;
	bool printEchocancelQueue        = false;
	bool hidden                      = false;
	bool printTranslationDirectories = false;
	std::optional< QString > windowTitlePostFix;
	std::optional< QString > jackName;
	std::optional< QString > rpc;
	std::optional< QString > locale;
	std::optional< QString > translationDirectory;
	std::optional< QDir > defaultCertificateDirectory;
};

class ArgumentParser {
public:
	explicit ArgumentParser(const QStringList &arguments);
	Arguments parse();

private:
	std::optional< QString > getNextArg(int currentIndex) const;
	bool isHelpArgument(const QString &arg) const;
	bool isVersionArgument(const QString &arg) const;
	bool isAllowMultiple(const QString &arg) const;
	bool isSuppressIdentity(const QString &arg) const;
	bool isLicense(const QString &arg) const;
	bool isAuthors(const QString &arg) const;
	bool isThirdPartyLicenses(const QString &arg) const;
	bool isDumpInputStreams(const QString &arg) const;
	bool isPrintEchocancelQueue(const QString &arg) const;
	bool isHidden(const QString &arg) const;
	bool isPrintTranslationDirectories(const QString &arg) const;
	std::optional< QString > isJackName(const QString &arg, const std::optional< QString > &value) const;
	std::optional< QString > isWindowTitlePostFix(const QString &arg, const std::optional< QString > &value) const;
	std::optional< QString > isRpc(const QString &arg, const std::optional< QString > &value) const;
	std::optional< QString > isLocale(const QString &arg, const std::optional< QString > &value) const;
	std::optional< QString > isTranslationDirectory(const QString &arg, const std::optional< QString > &value) const;
	std::optional< QDir > isDefaultCertificateDirectory(const QString &arg,
														const std::optional< QString > &value) const;
	QStringList getArguments() const;
	QStringList args;
};

#endif // ARGUMENTPARSER_H
