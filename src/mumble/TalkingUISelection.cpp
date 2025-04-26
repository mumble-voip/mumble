// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUISelection.h"
#include "MainWindow.h"
#include "UserModel.h"
#include "Global.h"

#include <QVariant>
#include <QWidget>

TalkingUISelection::TalkingUISelection(QWidget *widget) : m_widget(widget) {
}


void TalkingUISelection::setActive(bool active) {
	if (m_widget) {
		m_widget->setProperty("selected", active);
		// Unpolish the widget's style so that the new property can take effect
		m_widget->style()->unpolish(m_widget);
	}
}

void TalkingUISelection::apply() {
	setActive(true);
}

void TalkingUISelection::discard() {
	setActive(false);
}

bool TalkingUISelection::operator==(const TalkingUISelection &other) const {
	return m_widget == other.m_widget;
}

bool TalkingUISelection::operator!=(const TalkingUISelection &other) const {
	return m_widget != other.m_widget;
}

bool TalkingUISelection::operator==(const QWidget *widget) const {
	return m_widget == widget;
}

bool TalkingUISelection::operator!=(const QWidget *widget) const {
	return m_widget != widget;
}


UserSelection::UserSelection(QWidget *widget, unsigned int userSession)
	: TalkingUISelection(widget), m_userSession(userSession) {
}

void UserSelection::syncToMainWindow() const {
	if (Global::get().mw && Global::get().mw->pmModel) {
		Global::get().mw->pmModel->setSelectedUser(m_userSession);
	}
}

std::unique_ptr< TalkingUISelection > UserSelection::cloneToHeap() const {
	return std::make_unique< UserSelection >(*this);
}



ChannelSelection::ChannelSelection(QWidget *widget, unsigned int channelID)
	: TalkingUISelection(widget), m_channelID(channelID) {
}

void ChannelSelection::syncToMainWindow() const {
	if (Global::get().mw && Global::get().mw->pmModel) {
		Global::get().mw->pmModel->setSelectedChannel(m_channelID);
	}
}

std::unique_ptr< TalkingUISelection > ChannelSelection::cloneToHeap() const {
	return std::make_unique< ChannelSelection >(*this);
}



ListenerSelection::ListenerSelection(QWidget *widget, unsigned int userSession, unsigned int channelID)
	: TalkingUISelection(widget), m_userSession(userSession), m_channelID(channelID) {
}

void ListenerSelection::syncToMainWindow() const {
	if (Global::get().mw && Global::get().mw->pmModel) {
		Global::get().mw->pmModel->setSelectedChannelListener(m_userSession, m_channelID);
	}
}

std::unique_ptr< TalkingUISelection > ListenerSelection::cloneToHeap() const {
	return std::make_unique< ListenerSelection >(*this);
}



void EmptySelection::syncToMainWindow() const {
	// Do nothing
}

std::unique_ptr< TalkingUISelection > EmptySelection::cloneToHeap() const {
	return std::make_unique< EmptySelection >(*this);
}
