// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SearchDialog.h"
#include "Accessibility.h"
#include "Channel.h"
#include "ClientUser.h"
#include "MainWindow.h"
#include "SearchDialogItemDelegate.h"
#include "ServerHandler.h"
#include "UserModel.h"
#include "Global.h"

#include <QContextMenuEvent>
#include <QFontMetrics>
#include <QHash>
#include <QHideEvent>
#include <QIcon>
#include <QKeyEvent>
#include <QReadLocker>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QShowEvent>
#include <QTreeWidgetItem>

#include <stack>

namespace Search {

QString SearchDialog::toString(UserAction action) {
	switch (action) {
		case UserAction::NONE:
			return SearchDialog::tr("None");
		case UserAction::JOIN:
			return SearchDialog::tr("Join");
	}

	throw "Function incomplete or invalid enum value passed";
}

QString SearchDialog::toString(ChannelAction action) {
	switch (action) {
		case ChannelAction::NONE:
			return SearchDialog::tr("None");
		case ChannelAction::JOIN:
			return SearchDialog::tr("Join");
	}

	throw "Function incomplete or invalid enum value passed";
}

class SearchResultItem : public QTreeWidgetItem {
	Q_DISABLE_COPY(SearchResultItem)

public:
	template< typename parent_t >
	SearchResultItem(const SearchResult &result, unsigned int id, parent_t parent,
					 QTreeWidgetItem *precedingItem = nullptr)
		: QTreeWidgetItem(parent, precedingItem), m_result(result), m_id(id) {
		constexpr int TYPE_COLUMN  = 0;
		constexpr int MATCH_COLUMN = 1;

		if (m_result.type == SearchType::User) {
			QIcon userIcon = QIcon(QLatin1String("skin:talking_off.svg"));
			setIcon(TYPE_COLUMN, userIcon);
		}

		setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);

		const qsizetype begin  = m_result.begin;
		const qsizetype length = m_result.length;
		const QString matchText =
			m_result.fullText.replace(begin, length, "<b>" + m_result.fullText.mid(begin, length) + "</b>");

		setData(MATCH_COLUMN, Qt::DisplayRole, std::move(matchText));
		setData(MATCH_COLUMN, SearchDialogItemDelegate::CHANNEL_TREE_ROLE, m_result.channelHierarchy);

		setTextAlignment(MATCH_COLUMN, Qt::AlignLeft | Qt::AlignVCenter);
	}

	unsigned int getID() const { return m_id; }

	const SearchResult &getResult() const { return m_result; }

private:
	SearchResult m_result;
	unsigned int m_id;
};

class ChannelItem : public QTreeWidgetItem {
	Q_DISABLE_COPY(ChannelItem)

public:
	template< typename parent_t >
	ChannelItem(const Channel *chan, parent_t parent = nullptr, QTreeWidgetItem *precedingItem = nullptr)
		: QTreeWidgetItem(parent, precedingItem), m_chanID(chan->iId) {
		constexpr int NAME_COLUMN = 0;

		setText(NAME_COLUMN, chan->qsName);

		setTextAlignment(NAME_COLUMN, Qt::AlignLeft | Qt::AlignVCenter);
	}

	unsigned int getChannelID() const { return m_chanID; }

private:
	unsigned int m_chanID;
};

SearchDialog::SearchDialog(QWidget *parent) : QWidget(parent), m_itemDelegate(new SearchDialogItemDelegate()) {
	setupUi(this);

	// Make the search dialog be always on top and is shown as a Window
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::Window);

	// Set the correct icon for the options button
	toggleOptions->setIcon(QIcon("skin:config_basic.png"));

	// We can only init this after the UI has been set up
	m_searchFieldStyleWrapper = MultiStyleWidgetWrapper(searchField);

	// Init options
	userOption->setChecked(Global::get().s.searchForUsers);
	channelOption->setChecked(Global::get().s.searchForChannels);
	caseSensitiveOption->setChecked(Global::get().s.searchCaseSensitive);
	regexOption->setChecked(Global::get().s.searchAsRegex);

	searchOptionBox->setVisible(Global::get().s.searchOptionsShown);

	// This makes sure that our contextMenuEvent function gets called for creating the context menu
	searchResultTree->setContextMenuPolicy(Qt::DefaultContextMenu);

	// We have to use a custom ItemDelegate in order to be able to display rich text
	searchResultTree->setItemDelegate(m_itemDelegate.get());

	searchResultTree->setItemsExpandable(false);
	// Remove icons for expanding items
	searchResultTree->setRootIsDecorated(false);
	searchResultTree->header()->setMinimumSectionSize(0);
	searchResultTree->header()->hide();

	QFontMetrics metric(searchResultTree->font());
	searchResultTree->header()->resizeSection(0, static_cast< int >(metric.height() * 1.2));
	searchResultTree->setIconSize(QSize(metric.ascent(), metric.ascent()));

	if (Global::get().mw) {
		QObject::connect(Global::get().mw, &MainWindow::serverSynchronized, this,
						 &SearchDialog::on_serverConnectionSynchronized);

		// Add the action to toggle the search dialog to this dialof as well in order to make sure that
		// toggling it off again also works when the search dialog has focus.
		addAction(Global::get().mw->qaSearch);
	}
	if (Global::get().sh) {
		QObject::connect(Global::get().sh.get(), &ServerHandler::disconnected, this,
						 &SearchDialog::on_serverDisconnected);
	}
}

void SearchDialog::on_toggleOptions_clicked() {
	// Toggle the search option's visibility
	Global::get().s.searchOptionsShown = !searchOptionBox->isVisible();

	searchOptionBox->setVisible(Global::get().s.searchOptionsShown);
}

void SearchDialog::on_searchField_returnPressed() {
	// Then focus the search results (but only if there are actually
	// search results available)
	if (searchResultTree->topLevelItemCount() > 0) {
		searchResultTree->setFocus();

		if (!searchResultTree->currentItem()) {
			// Select the first result
			searchResultTree->setCurrentItem(searchResultTree->topLevelItem(0));
		}

		SearchResultItem *selectedItem = static_cast< SearchResultItem * >(searchResultTree->currentItem());

		if (selectedItem) {
			performDefaultAction(*selectedItem);
		}
	}
}

void SearchDialog::on_searchField_textChanged(const QString &text) {
	// Reset the search field's background color (might have been changed to indicate an error)
	m_searchFieldStyleWrapper.clearBackgroundColor();

	search(text);
}

void SearchDialog::on_userOption_clicked(bool checked) {
	Global::get().s.searchForUsers = checked;

	searchAgain();
}

void SearchDialog::on_channelOption_clicked(bool checked) {
	Global::get().s.searchForChannels = checked;

	searchAgain();
}

void SearchDialog::on_caseSensitiveOption_clicked(bool checked) {
	Global::get().s.searchCaseSensitive = checked;

	searchAgain();
}

void SearchDialog::on_regexOption_clicked(bool checked) {
	Global::get().s.searchAsRegex = checked;

	searchAgain();
}

void SearchDialog::on_searchResultTree_currentItemChanged(QTreeWidgetItem *c, QTreeWidgetItem *) {
	if (!c || Global::get().uiSession == 0) {
		return;
	}

	SearchResultItem &item = static_cast< SearchResultItem & >(*c);
	if (item.getResult().type == SearchType::User) {
		const ClientUser *user = ClientUser::get(item.getID());

		if (user) {
			// Only try to select the user if (s)he still exists
			Global::get().mw->pmModel->setSelectedUser(user->uiSession);
			item.setData(1, Qt::AccessibleTextRole, Mumble::Accessibility::userToText(user));
		}
	} else {
		const Channel *channel = Channel::get(item.getID());

		if (channel) {
			// Only try to select the channel if it still exists
			Global::get().mw->pmModel->setSelectedChannel(channel->iId);
			item.setData(1, Qt::AccessibleTextRole, Mumble::Accessibility::channelToText(channel));
		}
	}

	// Hack to make screen readers read search results...
	if (searchResultTree->currentColumn() == 1) {
		searchResultTree->setCurrentItem(c, 0);
	}
}

void SearchDialog::on_searchResultTree_itemActivated(QTreeWidgetItem *item, int) {
	if (item) {
		const SearchResultItem &activatedItem = static_cast< SearchResultItem & >(*item);

		performDefaultAction(activatedItem);
	}
}

void SearchDialog::searchAgain() {
	search(searchField->text());
}

void SearchDialog::clearSearchResults() {
	searchResultTree->clear();
}

SearchResult regularSearch(const QString &source, const QString &searchTerm, SearchType type, bool caseSensitive) {
	constexpr int FROM    = 0;
	const auto startIndex = source.indexOf(searchTerm, FROM, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);

	if (startIndex >= 0) {
		const auto length = searchTerm.size();

		return { startIndex, length, type, source, "" };
	} else {
		// Not found
		return {};
	}
}

SearchResult regexSearch(const QString &source, const QRegularExpression &regex, SearchType type) {
	QRegularExpressionMatch match = regex.match(source);

	if (match.hasMatch()) {
		// Found
		const auto startIndex = match.capturedStart();
		const auto length     = match.capturedEnd() - startIndex;

		return { startIndex, length, type, source, "" };
	} else {
		// Not found
		return {};
	}
}

QString getChannelHierarchy(const Channel &channel, bool includeSource) {
	std::stack< const Channel * > channels;

	if (includeSource) {
		channels.push(&channel);
	}

	const Channel *currentChannel = &channel;
	;
	while ((currentChannel = currentChannel->cParent)) {
		channels.push(currentChannel);
	}

	const QString separator = Global::get().s.qsHierarchyChannelSeparator;

	QString hierarchy;
	while (!channels.empty()) {
		hierarchy += channels.top()->qsName;
		channels.pop();

		if (!channels.empty()) {
			hierarchy += separator;
		}
	}

	return hierarchy;
}

void SearchDialog::search(const QString &searchTerm) {
	if (searchTerm.size() == 0 || Global::get().uiSession == 0) {
		// The search bar is either empty or there is no (fully synchronized) server connection
		clearSearchResults();

		return;
	}

	// Copy the values from the settings to ensure that the following code is not affected by
	// settings changing mid-execution.
	const bool searchUsers    = Global::get().s.searchForUsers;
	const bool searchChannels = Global::get().s.searchForChannels;
	const bool caseSensitive  = Global::get().s.searchCaseSensitive;
	const bool useRegEx       = Global::get().s.searchAsRegex;

	// Build and validate the RegEx if required
	QRegularExpression regex;
	if (useRegEx) {
		regex.setPattern(searchTerm);

		QRegularExpression::PatternOptions options = QRegularExpression::UseUnicodePropertiesOption;
		if (!caseSensitive) {
			options |= QRegularExpression::CaseInsensitiveOption;
		}
		regex.setPatternOptions(options);

		// Check that the provided RegEx is actually valid and usable
		if (!regex.isValid()) {
			// Indicate that there is an error by changing the search field's background color
			m_searchFieldStyleWrapper.setBackgroundColor("#fc5555");

			clearSearchResults();

			return;
		}
	}

	SearchResultMap matches;

	// Start by searching for users
	if (searchUsers) {
		QReadLocker userLock(&ClientUser::c_qrwlUsers);

		QHash< unsigned int, ClientUser * >::const_iterator it = ClientUser::c_qmUsers.constBegin();
		while (it != ClientUser::c_qmUsers.constEnd()) {
			const ClientUser *currentUser = it.value();

			SearchResult result;
			if (useRegEx) {
				result = regexSearch(currentUser->qsName, regex, SearchType::User);
			} else {
				result = regularSearch(currentUser->qsName, searchTerm, SearchType::User, caseSensitive);
			}

			if (result) {
				result.channelHierarchy = getChannelHierarchy(*currentUser->cChannel, true);
				matches.insert({ result, currentUser->uiSession });
			}

			it++;
		}
	}

	// Continue doing the same for channels
	if (searchChannels) {
		QReadLocker userLock(&Channel::c_qrwlChannels);

		QHash< unsigned int, Channel * >::const_iterator it = Channel::c_qhChannels.constBegin();
		while (it != Channel::c_qhChannels.constEnd()) {
			const Channel *currentChannel = it.value();

			SearchResult result;
			if (useRegEx) {
				result = regexSearch(currentChannel->qsName, regex, SearchType::Channel);
			} else {
				result = regularSearch(currentChannel->qsName, searchTerm, SearchType::Channel, caseSensitive);
			}

			if (result) {
				result.channelHierarchy = getChannelHierarchy(*currentChannel, false);
				// As the channel ID is never negative, we can safely cast it to an unsigned int
				matches.insert({ result, static_cast< unsigned int >(currentChannel->iId) });
			}

			it++;
		}
	}

	setSearchResults(matches);
}

void SearchDialog::on_serverConnectionSynchronized() {
	searchAgain();

	if (Global::get().sh) {
		// Connect signal for clearing all search results on disconnect
		QObject::connect(Global::get().sh.get(), &ServerHandler::disconnected, this,
						 &SearchDialog::on_serverDisconnected);
	}
}

void SearchDialog::on_serverDisconnected() {
	clearSearchResults();
}

void SearchDialog::on_clientDisconnected(unsigned int userSession) {
	removeSearchResult(userSession, true);
}

void SearchDialog::on_channelRemoved(unsigned int channelID) {
	removeSearchResult(channelID, false);
}

void SearchDialog::setSearchResults(const SearchResultMap &results) {
	// First clear all existing results
	clearSearchResults();

	if (results.size() > 0) {
		SearchResultMap::const_iterator it = results.cbegin();

		SearchResultItem *previousItem = nullptr;
		while (it != results.cend()) {
			// Move the SearchResult out of the map
			const SearchResult currentResult = std::move(it->first);
			const unsigned int currentID     = it->second;

			// Constructing this instance is enough to set everything up and adding it to the tree
			// We have to add a pointer to the previous item so that this item is actually appended after
			// the preceding one and thus the order of the map is preserved. Without this, the order would
			// be reversed.
			previousItem = new SearchResultItem(currentResult, currentID, searchResultTree, previousItem);

			it++;
		}
	}
}

void SearchDialog::hideEvent(QHideEvent *event) {
	QWidget::hideEvent(event);

	if (!event->spontaneous()) {
		// Clear results and search string
		clearSearchResults();
		searchField->clear();
	}
}

void SearchDialog::showEvent(QShowEvent *event) {
	QWidget::showEvent(event);

	// We have to activate the window to make sure that it currently has focus and (more importantly)
	// is able to receive keyboard events. Without this function the window will appear but keyboard
	// events will still be sent to whatever window had focus at the time this was made visible (at
	// least of that previously focused window was not Mumble).
	//
	// Note however that this DOES NOT WORK on Windows!
	// Citing from https://doc.qt.io/qt-5/qwidget.html#activateWindow:
	// "On Windows, if you are calling this when the application is not currently the active one then it will not make
	// it the active window. It will change the color of the taskbar entry to indicate that the window has changed in
	// some way. This is because Microsoft does not allow an application to interrupt what the user is currently doing
	// in another application."
	activateWindow();

	// Focus the search box so that the user can directly start typing
	searchField->setFocus();
}

void SearchDialog::keyPressEvent(QKeyEvent *event) {
	auto it = m_relayedKeyEvents.find(event);
	if (it != m_relayedKeyEvents.cend()) {
		// This is an KeyEvent that we relayed, but that did not get accepted by the TreeWidget and thus it is
		// proagating back to us. Thus we will accept it here in order to keep it from propagating further up
		// but we don't act on it since this here is not the originally intended target anyways.
		event->accept();

		m_relayedKeyEvents.erase(it);
		return;
	}

	if (event->matches(QKeySequence::Cancel)) {
		event->accept();
		// Mimic behavior of dialogs (close on Esc)
		close();
	}

	if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_PageUp
		|| event->key() == Qt::Key_PageDown) {
		QKeyEvent *copy = new QKeyEvent(event->type(), event->key(), event->modifiers(), event->nativeScanCode(),
										event->nativeVirtualKey(), event->nativeScanCode(), event->text(),
										event->isAutoRepeat(), static_cast< ushort >(event->count()));

		m_relayedKeyEvents.insert(copy);

		qApp->postEvent(searchResultTree, copy);
	}
}

void SearchDialog::contextMenuEvent(QContextMenuEvent *event) {
	if (Global::get().mw && searchResultTree->currentIndex().isValid() && Global::get().uiSession > 0) {
		// We pretend as if the user had clicked on the client in the MainWindow. For this to work we map the global
		// mouse position to the local coordinate system of the UserView in the MainWindow. The function will use
		// some internal logic to determine the user to invoke the context menu on but if that fails (which in this
		// case it will), it'll fall back to the currently selected item.
		// As we synchronize the selection from the search dialog to the MainWindow, this will already be what we want
		// it to be.
		QMetaObject::invokeMethod(Global::get().mw, "on_qtvUsers_customContextMenuRequested", Qt::QueuedConnection,
								  Q_ARG(QPoint, Global::get().mw->qtvUsers->mapFromGlobal(event->globalPos())),
								  Q_ARG(bool, false));

		event->accept();
	}
}

void SearchDialog::performDefaultAction(const SearchResultItem &item) {
	MainWindow *mainWindow = Global::get().mw;
	if (!mainWindow) {
		return;
	}

	if (item.getResult().type == SearchType::User) {
		ClientUser *selectedUser = mainWindow->pmModel->getSelectedUser();

		if (selectedUser) {
			mainWindow->cuContextUser = selectedUser;

			switch (Global::get().s.searchUserAction) {
				case UserAction::NONE:
					break;
				case UserAction::JOIN:
					mainWindow->on_qaUserJoin_triggered();
					break;
			}

			// Reset to avoid side-effects
			mainWindow->cuContextUser = nullptr;
		}
	} else {
		Channel *selectedChannel = mainWindow->pmModel->getSelectedChannel();

		if (selectedChannel) {
			mainWindow->cContextChannel = selectedChannel;

			switch (Global::get().s.searchChannelAction) {
				case ChannelAction::NONE:
					break;
				case ChannelAction::JOIN:
					mainWindow->on_qaChannelJoin_triggered();
					break;
			}

			// Reset to avoid side-effects
			mainWindow->cContextChannel = nullptr;
		}
	}

	// Close dialog after the default action
	close();
}

bool SearchDialog::removeSearchResult(unsigned int id, bool isUser) {
	for (int i = 0; i < searchResultTree->topLevelItemCount(); i++) {
		SearchResultItem *item = static_cast< SearchResultItem * >(searchResultTree->topLevelItem(i));

		if (item) {
			bool typeMatches = (isUser && item->getResult().type == SearchType::User)
							   || (!isUser && item->getResult().type == SearchType::Channel);

			if (typeMatches && item->getID() == id) {
				// Remove this item
				QTreeWidgetItem *removedItem = searchResultTree->takeTopLevelItem(i);

				delete removedItem;

				return true;
			}
		}
	}

	return false;
}

} // namespace Search
