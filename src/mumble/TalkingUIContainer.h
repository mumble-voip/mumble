// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TALKINGUICONTAINER_H_
#define MUMBLE_MUMBLE_TALKINGUICONTAINER_H_

#include "TalkingUIComponent.h"
#include "TalkingUIEntry.h"

#include "widgets/MultiStyleWidgetWrapper.h"

#include <QString>

#include <memory>

class QWidget;
class QGroupBox;
class TalkingUI;

enum class ContainerType { CHANNEL };

class TalkingUIContainer : public TalkingUIComponent {
	friend class TalkingUIUser;

protected:
	std::vector< std::unique_ptr< TalkingUIEntry > > m_entries;

	int m_associatedChannelID = -1;

	bool m_permanent = false;

	TalkingUI &m_talkingUI;

	virtual int find(unsigned int associatedUserSession, EntryType type) const;

public:
	TalkingUIContainer(int associatedChannelID, TalkingUI &talkingUI);
	virtual ~TalkingUIContainer() = default;

	virtual QString getName() const = 0;

	virtual int compare(const TalkingUIContainer &other) const = 0;

	virtual ContainerType getType() const = 0;

	virtual int getAssociatedChannelID() const;

	virtual void addEntry(std::unique_ptr< TalkingUIEntry > entry);
	virtual std::unique_ptr< TalkingUIEntry > removeEntry(const TalkingUIEntry *entry);
	virtual std::unique_ptr< TalkingUIEntry > removeEntry(unsigned int associatedUserSession, EntryType type);

	virtual std::vector< std::unique_ptr< TalkingUIEntry > > &getEntries();
	virtual const std::vector< std::unique_ptr< TalkingUIEntry > > &getEntries() const;

	virtual bool contains(unsigned int associatedUserSession, EntryType type) const;

	virtual std::size_t size() const;
	virtual bool isEmpty() const;

	virtual void setPermanent(bool permanent);
	virtual bool isPermanent() const;

	virtual TalkingUIEntry *get(unsigned int associatedUserSession, EntryType type);

	bool operator==(const TalkingUIContainer &other) const;
	bool operator!=(const TalkingUIContainer &other) const;
	bool operator>(const TalkingUIContainer &other) const;
	bool operator>=(const TalkingUIContainer &other) const;
	bool operator<(const TalkingUIContainer &other) const;
	bool operator<=(const TalkingUIContainer &other) const;
};


class TalkingUIChannel : public TalkingUIContainer {
protected:
	QGroupBox *m_channelBox;
	MultiStyleWidgetWrapper m_channelBoxStyleWrapper;

	EntryPriority m_highestUserPriority = EntryPriority::LOWEST;

	void updatePriority();

public:
	TalkingUIChannel(int associatedChannelID, QString name, TalkingUI &talkingUI);
	virtual ~TalkingUIChannel();

	virtual QString getName() const override;
	virtual void setName(const QString &name);

	virtual int compare(const TalkingUIContainer &other) const override;

	virtual QWidget *getWidget() override;
	virtual const QWidget *getWidget() const override;
	virtual MultiStyleWidgetWrapper &getStylableWidget() override;

	virtual ContainerType getType() const override;

	virtual void addEntry(std::unique_ptr< TalkingUIEntry > entry) override;
	virtual std::unique_ptr< TalkingUIEntry > removeEntry(unsigned int associatedUserSession, EntryType type) override;
};

#endif // MUMBLE_MUMBLE_TALKINGUICONTAINER_H_
