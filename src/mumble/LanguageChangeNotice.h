#ifndef MUMBLE_MUMBLE_LANGUAGE_CHANGE_NOTICE_H
#define MUMBLE_MUMBLE_LANGUAGE_CHANGE_NOTICE_H

#include <QtCore/QObject>

class LanguageChangeNotice : public QObject {
	private:
		Q_OBJECT
	
	protected:
		QLabel *languageLabel;
		int startLanguageID;
		QString originalLabelText;
		QString restartNotice;
	
	public:
		LanguageChangeNotice(QLabel *languageLabel, int startLanguageID);
	
	public slots:
		void languageChanged(int);
};



#endif
