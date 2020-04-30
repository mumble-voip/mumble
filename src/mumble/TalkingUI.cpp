// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TalkingUI.h"
#include "ClientUser.h"
#include "Channel.h"
#include "MainWindow.h"
#include "UserModel.h"

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QScreen>
#include <QMouseEvent>
#include <QPalette>
#include <QItemSelectionModel>
#include <QModelIndex>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

TalkingUI::TalkingUI(QWidget *parent)
	: QWidget(parent),
	  m_entries(),
	  m_channels(),
	  m_timers(),
	  m_currentSelection(nullptr),
	  m_talkingIcon(QIcon(QLatin1String("skin:talking_on.svg"))),
	  m_passiveIcon(QIcon(QLatin1String("skin:talking_off.svg"))),
	  m_shoutingIcon(QIcon(QLatin1String("skin:talking_alt.svg"))),
	  m_whisperingIcon(QIcon(QLatin1String("skin:talking_whisper.svg"))) {

	setupUI();
}

void TalkingUI::setupUI() {
	QVBoxLayout *layout = new QVBoxLayout;

	setLayout(layout);

	setWindowTitle(QObject::tr("Talking UI"));

	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

	connect(g.mw->qtvUsers->selectionModel(), &QItemSelectionModel::currentChanged, this, &TalkingUI::on_mainWindowSelectionChanged);
}

void TalkingUI::removeUser(unsigned int session) {
	QHash<unsigned int, Entry>::iterator iter = m_entries.find(session);
	if (iter == m_entries.end()) {
		return;
	}
	
	Entry &entry = iter.value();

	entry.background->hide();

	// In principle the user could have changed channel since it has been added to
	// the UI, so we'll iterate over all GroupBoxes and remove the labels from every
	// single one of them.
	QHashIterator<int, QGroupBox *> it(m_channels);

	while (it.hasNext()) {
		it.next();

		QGroupBox *channelBox = it.value();

		channelBox->layout()->removeWidget(entry.background);

		if (channelBox->layout()->count() == 0) {
			// There are no active speakers in this channel anymore
			channelBox->hide();
			layout()->removeWidget(channelBox);
		}
	}

	updateUI();
}

void TalkingUI::updateUI() {
	// Use timer to execute this after all other events have been processed
	QTimer::singleShot(0, [this]() { adjustSize(); });
}

void TalkingUI::setSelection(Entry *entry) {
	if (entry != m_currentSelection) {
		if (m_currentSelection) {
			m_currentSelection->background->setStyleSheet(QLatin1String(""));
		}
		if (entry) {
			QString colorHex = palette().color(QPalette::Normal, QPalette::Highlight).name(QColor::HexRgb);
			entry->background->setStyleSheet(QString::fromLatin1("background-color: %1;").arg(colorHex));

			if (g.mw && g.mw->pmModel) {
				g.mw->pmModel->setSelectedUser(entry->userSession);
			}
		}

		m_currentSelection = entry;
	}
}

void TalkingUI::mousePressEvent(QMouseEvent *event) {
	Q_UNUSED(event);

	QWidget *widget = qApp->widgetAt(event->globalPos());

	if (!widget) {
		setSelection(nullptr);
		return;
	}

	// iterate all available entries
	QMutableHashIterator<unsigned int, Entry> it(m_entries);

	Entry *entry = nullptr;

	while (it.hasNext()) {
		it.next();

		Entry *currentEntry = &it.value();
		if (currentEntry->icon == widget || currentEntry->name == widget || currentEntry->background == widget) {
			entry = currentEntry;
			break;
		}
	}

	setSelection(entry);

	updateUI();
}

void TalkingUI::setVisible(bool visible) {
	if (visible) {
		adjustSize();
	}

	QWidget::setVisible(visible);
}

QSize TalkingUI::sizeHint() const {
	// Prefer to occupy at least 10% of the screen's size
	// This aims to be a good compromise between not being in the way and not
	// being too small to being handled properly.
	int width =  QGuiApplication::screens()[0]->availableSize().width() * 0.1;

	return {width, 0};
}

QSize TalkingUI::minimumSizeHint() const {
	return {0, 0};
}

void TalkingUI::on_talkingStateChanged() {
	ClientUser *user = qobject_cast<ClientUser *>(sender());

	if (!user) {
		return;
	}

	if (!user->cChannel) {
		// If the user doesn't have an associated channel, something's either wrong
		// or that user has just disconnected. In either way, we want to make sure
		// that this user won't stick around in the UI.
		removeUser(user->uiSession);
		return;
	}

	bool isSelf = g.uiSession == user->uiSession;

	if (!m_channels.contains(user->cChannel->iId)) {
		// Create a QGroupBox for this user
		QGroupBox *box = new QGroupBox(user->cChannel->qsName, this);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->setContentsMargins(0, 0, 0, 0);
		box->setLayout(layout);

		m_channels.insert(user->cChannel->iId, box);
	}

	if (!m_entries.contains(user->uiSession)) {
		// Create an Entry for this user (alongside the respective labels)
		// We initially set the labels to not be visible, so that we'll
		// enter the code-block further down.

		QWidget *background = new QWidget(this);
		QLayout *backgroundLayout = new QHBoxLayout();
		backgroundLayout->setContentsMargins(2, 3, 2, 3);
		background->setLayout(backgroundLayout);
		background->setAutoFillBackground(true);
		background->hide();

		background->setProperty("userName", user->qsName);

		QLabel *name = new QLabel(background);
		if (isSelf) {
			// Indicate self by bold name
			name->setText(QString::fromLatin1("<b>%1</b>").arg(user->qsName));
		} else {
			name->setText(user->qsName);
		}

		QLabel *icon = new QLabel(background);
		icon->setAlignment(Qt::AlignCenter);
		icon->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

		m_entries.insert(user->uiSession, {icon, name, background, user->uiSession});

		backgroundLayout->addWidget(icon);
		backgroundLayout->addWidget(name);

		// Also create a timer for this specific user
		QTimer *timer = new QTimer(this);
		timer->setSingleShot(true);
		const unsigned int session = user->uiSession;
		QObject::connect(timer, &QTimer::timeout, [this, session](){
			removeUser(session);
		});

		m_timers.insert(user->uiSession, timer);

		// If this user is currently selected, mark him/her as such
		if (g.mw && g.mw->pmModel && g.mw->pmModel->getSelectedUser() == user) {
			setSelection(&m_entries[user->uiSession]);
		}
	}


	// * 1000 as the setting is in seconds whereas the timer expects milliseconds
	// We set the interval every time the talking state of a user has changed, in case the settings for
	// it have changed during runtime.
	m_timers[user->uiSession]->setInterval(g.s.iTalkingUI_SilentUserLifeTime * 1000);

	// Get the Entry for this user
	Entry entry = m_entries[user->uiSession];


	// Set the icon for this user according to the TalkingState
	QIcon *icon = nullptr;
	switch (user->tsState) {
		case Settings::Talking:
			icon = &m_talkingIcon;
			break;
		case Settings::Whispering:
			icon = &m_whisperingIcon;
			break;
		case Settings::Shouting:
			icon = &m_shoutingIcon;
			break;
		default:
			icon = &m_passiveIcon;
			break;

	}
	const int height = fontMetrics().height(); // Make icon as high as the text
	entry.icon->setPixmap(icon->pixmap(QSize(height, height), QIcon::Normal, QIcon::On));


	if (user->tsState == Settings::Passive) {
		// User stopped talking
		// Start the timer that will remove this user
		m_timers[user->uiSession]->start();
	} else {
		// User started talking
		// Stop the timer for this user in case it has been started before but the user
		// started speaking again before having been removed
		m_timers[user->uiSession]->stop();

		// Get the box we'll want to have the user in (representing the current
		// channel of the user)
		QGroupBox *channelBox = m_channels[user->cChannel->iId];

		if (!channelBox) {
			qCritical("TalkingUI: Can't find channel for speaker");
			return;
		}

		// Check if the user has changed channel and handle this separately in case
		// the user is currently still displayed as being in that channel
		bool changedChannel = false;
		if (channelBox->layout()->indexOf(entry.name) < 0) {
			changedChannel = true;

			removeUser(user->uiSession);
		}

		bool adjust = false;
		if (!channelBox->isVisible()) {
			// Make sure the channel-box is visible
			channelBox->show();

			// Sort the channels alphabetically
			bool inserted = false;
			for (int i = 0; i < layout()->count(); i++) {
				QGroupBox *currentBox = static_cast<QGroupBox *>(layout()->itemAt(i)->widget());

				if (currentBox->title() > channelBox->title()) {
					static_cast<QVBoxLayout *>(layout())->insertWidget(i, channelBox);
					inserted = true;
					break;
				}
			}

			if (!inserted) {
				layout()->addWidget(channelBox);
			}

			adjust = true;
		}

		if (!entry.background->isVisible() || changedChannel) {
			// Make sure the Entry for this user is visible
			entry.background->show();

			// Sort the users alphabetically
			QString currentName = entry.background->property("userName").toString();
			bool inserted = false;
			for (int i = 0; i < channelBox->layout()->count(); i++) {
				QWidget *currentBackground = static_cast<QWidget *>(channelBox->layout()->itemAt(i)->widget());

				QString userName = currentBackground->property("userName").toString();

				if (userName > currentName) {
					static_cast<QHBoxLayout *>(channelBox->layout())->insertWidget(i, entry.background);
					inserted = true;
					break;
				}
			}

			if (!inserted) {
				channelBox->layout()->addWidget(entry.background);
			}


			adjust = true;
		}

		if (adjust) {
			updateUI();
		}
	}
}

void TalkingUI::on_mainWindowSelectionChanged(const QModelIndex &current, const QModelIndex &previous) {
	Q_UNUSED(previous);

	// Sync the selection in the MainWindow to the TalkingUI
	if (g.mw && g.mw->pmModel) {
		ClientUser *user = g.mw->pmModel->getUser(current);

		if (user && m_entries.contains(user->uiSession)) {
			setSelection(&m_entries[user->uiSession]);
		} else {
			setSelection(nullptr);
		}
	}
}
