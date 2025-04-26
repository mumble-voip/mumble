// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUIContainer.h"
#include "TalkingUIEntry.h"

#include <QGroupBox>
#include <QVBoxLayout>

#include <algorithm>


struct entry_ptr_less {
	bool operator()(const std::unique_ptr< TalkingUIEntry > &first, const std::unique_ptr< TalkingUIEntry > &second) {
		return *first < *second;
	}
};



TalkingUIContainer::TalkingUIContainer(int associatedChannelID, TalkingUI &talkingUI)
	: m_associatedChannelID(associatedChannelID), m_talkingUI(talkingUI) {
}


int TalkingUIContainer::find(unsigned int associatedUserSession, EntryType type) const {
	for (std::size_t i = 0; i < m_entries.size(); i++) {
		const std::unique_ptr< TalkingUIEntry > &currentEntry = m_entries[i];
		if (currentEntry->getType() == type && currentEntry->getAssociatedUserSession() == associatedUserSession) {
			return static_cast< int >(i);
		}
	}

	return -1;
}

int TalkingUIContainer::getAssociatedChannelID() const {
	return m_associatedChannelID;
}

void TalkingUIContainer::addEntry(std::unique_ptr< TalkingUIEntry > entry) {
	// set container
	entry->m_container = this;

	m_entries.push_back(std::move(entry));
}

std::unique_ptr< TalkingUIEntry > TalkingUIContainer::removeEntry(const TalkingUIEntry *entry) {
	return removeEntry(entry->getAssociatedUserSession(), entry->getType());
}

std::unique_ptr< TalkingUIEntry > TalkingUIContainer::removeEntry(unsigned int associatedUserSession, EntryType type) {
	int index = find(associatedUserSession, type);

	std::unique_ptr< TalkingUIEntry > entry(nullptr);

	if (index >= 0) {
		// Move the entry out of the vector
		entry = std::move(m_entries[static_cast< std::size_t >(index)]);
		m_entries.erase(m_entries.begin() + index);

		// reset container
		entry->m_container = nullptr;
	}

	return entry;
}

std::vector< std::unique_ptr< TalkingUIEntry > > &TalkingUIContainer::getEntries() {
	return m_entries;
}

const std::vector< std::unique_ptr< TalkingUIEntry > > &TalkingUIContainer::getEntries() const {
	return m_entries;
}

bool TalkingUIContainer::contains(unsigned int associatedUserSession, EntryType type) const {
	return find(associatedUserSession, type) >= 0;
}

std::size_t TalkingUIContainer::size() const {
	return m_entries.size();
}

bool TalkingUIContainer::isEmpty() const {
	return size() == 0;
}

void TalkingUIContainer::setPermanent(bool permanent) {
	m_permanent = permanent;
}

bool TalkingUIContainer::isPermanent() const {
	return m_permanent;
}

TalkingUIEntry *TalkingUIContainer::get(unsigned int associatedUserSession, EntryType type) {
	int index = find(associatedUserSession, type);

	if (index >= 0) {
		return m_entries[static_cast< std::size_t >(index)].get();
	} else {
		return nullptr;
	}
}

bool TalkingUIContainer::operator==(const TalkingUIContainer &other) const {
	return compare(other) == 0;
}

bool TalkingUIContainer::operator!=(const TalkingUIContainer &other) const {
	return compare(other) != 0;
}

bool TalkingUIContainer::operator>(const TalkingUIContainer &other) const {
	return compare(other) > 0;
}

bool TalkingUIContainer::operator>=(const TalkingUIContainer &other) const {
	return compare(other) >= 0;
}

bool TalkingUIContainer::operator<(const TalkingUIContainer &other) const {
	return compare(other) < 0;
}

bool TalkingUIContainer::operator<=(const TalkingUIContainer &other) const {
	return compare(other) <= 0;
}



const int CHANNEL_LAYOUT_TOP_MARGIN = 10;

TalkingUIChannel::TalkingUIChannel(int associatedChannelID, QString name, TalkingUI &talkingUI)
	: TalkingUIContainer(associatedChannelID, talkingUI), m_channelBox(new QGroupBox()),
	  m_channelBoxStyleWrapper(m_channelBox) {
	// Set name
	m_channelBox->setTitle(name);

	// Don't inherit the background color set on the channel box to its children.
	m_channelBoxStyleWrapper.setBackgroundColorSelector("TalkingUI > QGroupBox");

	// Set layout
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(0, CHANNEL_LAYOUT_TOP_MARGIN, 0, 0);
	m_channelBox->setLayout(layout);
}

TalkingUIChannel::~TalkingUIChannel() {
	m_channelBox->deleteLater();
}

void TalkingUIChannel::updatePriority() {
	// First reset the priority to the lowest possible value as the loop below
	// will only update it to be higher than the current priority
	m_highestUserPriority = EntryPriority::LOWEST;

	for (auto &currentEntry : m_entries) {
		if (currentEntry->getPriority() > m_highestUserPriority) {
			m_highestUserPriority = currentEntry->getPriority();
		}
	}
}

QString TalkingUIChannel::getName() const {
	return m_channelBox->title();
}

void TalkingUIChannel::setName(const QString &name) {
	m_channelBox->setTitle(name);
}

int TalkingUIChannel::compare(const TalkingUIContainer &other) const {
	if (getType() != other.getType()) {
		return static_cast< int >(other.getType()) - static_cast< int >(getType());
	}

	const TalkingUIChannel &otherChannel = static_cast< const TalkingUIChannel & >(other);

	if (m_highestUserPriority != otherChannel.m_highestUserPriority) {
		return m_highestUserPriority < otherChannel.m_highestUserPriority ? 1 : -1;
	}

	int res = getName().localeAwareCompare(otherChannel.getName());

	if (res == 0) {
		// Only consider channels to be the same if they have the same associated ID
		res = otherChannel.getAssociatedChannelID() - getAssociatedChannelID();
	}

	return res;
}

QWidget *TalkingUIChannel::getWidget() {
	return m_channelBox;
}

const QWidget *TalkingUIChannel::getWidget() const {
	return m_channelBox;
}

MultiStyleWidgetWrapper &TalkingUIChannel::getStylableWidget() {
	return m_channelBoxStyleWrapper;
}

ContainerType TalkingUIChannel::getType() const {
	return ContainerType::CHANNEL;
}

void TalkingUIChannel::addEntry(std::unique_ptr< TalkingUIEntry > entry) {
	if (entry->getPriority() > m_highestUserPriority) {
		m_highestUserPriority = entry->getPriority();
	}
	TalkingUIContainer::addEntry(std::move(entry));

	// Remove all entries from the layout
	for (const auto &currentEntry : m_entries) {
		m_channelBox->layout()->removeWidget(currentEntry->getWidget());
	}

	// Sort entries
	std::sort(m_entries.begin(), m_entries.end(), entry_ptr_less());

	// Add all entries again (including the new one) in order
	for (auto &currentEntry : m_entries) {
		m_channelBox->layout()->addWidget(currentEntry->getWidget());
	}
}

std::unique_ptr< TalkingUIEntry > TalkingUIChannel::removeEntry(unsigned int associatedUserSession, EntryType type) {
	std::unique_ptr< TalkingUIEntry > entry = TalkingUIContainer::removeEntry(associatedUserSession, type);

	if (entry) {
		m_channelBox->layout()->removeWidget(entry->getWidget());
	}

	updatePriority();

	return entry;
}
