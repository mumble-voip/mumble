#include "Channel.h"

QHash<int, Channel *> Channel::c_qhChannels;
QReadWriteLock Channel::c_qrwlChannels;

Channel::Channel(int id, const QString &name, QObject *p) : QObject(p) {
	iId = id;
	iPosition = 0;
	qsName = name;
	bTemporary = false;
	cParent = qobject_cast<Channel *>(p);
	if (cParent)
		cParent->addChannel(this);
}

Channel::~Channel() {
	if (cParent)
		cParent->removeChannel(this);

	foreach(Channel *c, qlChannels)
		delete c;

	Q_ASSERT(qlChannels.count() == 0);
	Q_ASSERT(children().count() == 0);
}

Channel *Channel::get(int id) {
	QReadLocker lock(&c_qrwlChannels);
	return c_qhChannels.value(id);
}
Channel *Channel::add(int id, const QString &name) {
	QWriteLocker lock(&c_qrwlChannels);

	if (c_qhChannels.contains(id))
		return NULL;

	Channel *c = new Channel(id, name, NULL);
	c_qhChannels.insert(id, c);
	return c;
}

void Channel::remove(Channel *c) {
	QWriteLocker lock(&c_qrwlChannels);
	c_qhChannels.remove(c->iId);
}

size_t Channel::getLevel() const {
	size_t i = 0;

	const Channel *c = this;
	while (c->cParent) {
		c = c->cParent;
		++i;
	}

	return i;
}

size_t Channel::getDepth() const {
	if(qlChannels.empty()) {
		return 0;
	}

	size_t result = 0;
	foreach(Channel *child, qlChannels) {
		result = qMax(result, child->getDepth() + 1);
	}

	return result;
}

QString Channel::getPath() const {
	QString out;

	const Channel *tmp = this;
	while (tmp->cParent) {
		// Skip the root channel.
		if (tmp->iId == 0) {
			break;
		}

		out.prepend(QString::fromLatin1("/"));
		out.prepend(tmp->qsName);

		tmp = tmp->cParent;
	}

	return out;
}

void Channel::addChannel(Channel *c) {
	c->cParent = this;
	c->setParent(this);
	qlChannels << c;
}

void Channel::removeChannel(Channel *c) {
	c->cParent = NULL;
	c->setParent(NULL);
	qlChannels.removeAll(c);
}

Channel::operator QString() const {
	return QString::fromLatin1("%1[%2:%3%4]").arg(qsName,
	        QString::number(iId),
	        QString::number(cParent ? cParent->iId : -1),
	        bTemporary ? QLatin1String("*") : QLatin1String(""));
}
