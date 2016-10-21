#ifndef MUMBLE_TESTS_CHANNEL_H_
#define MUMBLE_TESTS_CHANNEL_H_

#include <QObject>
#include <QHash>
#include <QReadWriteLock>

class Channel : public QObject {
	Q_OBJECT
	private:
		Q_DISABLE_COPY(Channel)
	public:
		int iId;
		int iPosition;
		bool bTemporary;
		Channel *cParent;
		QString qsName;
		QList<Channel *> qlChannels;
		Channel(int id, const QString &name, QObject *p = NULL);
		~Channel();

		unsigned int uiPermissions;
		bool bFiltered;

		static QHash<int, Channel *> c_qhChannels;
		static QReadWriteLock c_qrwlChannels;

		static Channel *get(int);
		static Channel *add(int, const QString &);
		static void remove(Channel *);

		size_t getLevel() const;
		size_t getDepth() const;
		QString getPath() const;

		void addChannel(Channel *c);
		void removeChannel(Channel *c);

		operator QString() const;
};

#endif
