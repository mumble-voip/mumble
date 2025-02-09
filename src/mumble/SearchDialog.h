// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SEARCHDIALOG_H_
#define MUMBLE_MUMBLE_SEARCHDIALOG_H_

#include "MultiStyleWidgetWrapper.h"

#include <QString>

#include <map>
#include <memory>
#include <unordered_set>

#include "ui_SearchDialog.h"

class QWidget;
class QTreeWidgetItem;
class QKeyEvent;
class QContextMenuEvent;
class QHideEvent;
class QShowEvent;

class SearchDialogItemDelegate;

namespace Search {

/**
 * The type of a search result
 */
enum class SearchType { User, Channel };

/**
 * This struct represents a search result and contains some metainformation
 * on it.
 */
struct SearchResult {
	int64_t begin  = -1;
	int64_t length = -1;
	SearchType type;
	QString fullText;
	QString channelHierarchy;

	operator bool() {
		// A search result is only valid if it has start and end
		return begin >= 0 && length > 0;
	}
};

class SearchResultItem;
class ChannelItem;

/**
 * This comparator is used to sort search results
 */
struct SearchResultSortComparator {
	bool operator()(const SearchResult &lhs, const SearchResult &rhs) const {
		// Prioritize longer matches (only relevant if match is obtained via RegEx)
		if (lhs.length != rhs.length) {
			return lhs.length > rhs.length;
		}

		if (lhs.begin != rhs.begin) {
			// The closer to the start a match is found, the higher its priority
			return lhs.begin < rhs.begin;
		}

		if (lhs.fullText != rhs.fullText) {
			return lhs.fullText.compare(rhs.fullText) < 0;
		}

		if (lhs.channelHierarchy != rhs.channelHierarchy) {
			return lhs.channelHierarchy.compare(rhs.channelHierarchy) < 0;
		}

		// Order users before channels
		return lhs.type == SearchType::User;
	}
};

using SearchResultMap = std::map< SearchResult, unsigned int, SearchResultSortComparator >;

/**
 * The search result class is the one that pops up when triggering the search functionality
 */
class SearchDialog : public QWidget, private Ui::SearchDialog {
	Q_OBJECT
	Q_DISABLE_COPY(SearchDialog)

public:
	SearchDialog(QWidget *parent = nullptr);
	~SearchDialog() = default;

	/**
	 * Possible actions that can be performed on a selected search
	 * result, if it is a user.
	 */
	enum class UserAction { NONE, JOIN };
	/**
	 * Possible actions that can be performed on a selected search
	 * result, if it is a channel.
	 */
	enum class ChannelAction { NONE, JOIN };

	static QString toString(UserAction action);
	static QString toString(ChannelAction action);

public slots:
	void on_toggleOptions_clicked();
	void on_searchField_returnPressed();
	void on_searchField_textChanged(const QString &text);
	void on_userOption_clicked(bool checked);
	void on_channelOption_clicked(bool checked);
	void on_caseSensitiveOption_clicked(bool checked);
	void on_regexOption_clicked(bool checked);
	void on_searchResultTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_searchResultTree_itemActivated(QTreeWidgetItem *item, int column);
	void searchAgain();
	void clearSearchResults();
	void search(const QString &searchTerm);
	void on_serverConnectionSynchronized();
	void on_serverDisconnected();
	void on_clientDisconnected(unsigned int userSession);
	void on_channelRemoved(unsigned int channelID);

private:
	MultiStyleWidgetWrapper m_searchFieldStyleWrapper;
	std::unordered_set< void * > m_relayedKeyEvents;
	std::unique_ptr< SearchDialogItemDelegate > m_itemDelegate;

	void setSearchResults(const SearchResultMap &results);
	void hideEvent(QHideEvent *event) override;
	void showEvent(QShowEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void contextMenuEvent(QContextMenuEvent *event) override;
	void performDefaultAction(const SearchResultItem &item);
	bool removeSearchResult(unsigned int id, bool isUser);
};

} // namespace Search

Q_DECLARE_METATYPE(Search::SearchDialog::UserAction)
Q_DECLARE_METATYPE(Search::SearchDialog::ChannelAction)

#endif // MUMBLE_MUMBLE_SEARCHDIALOG_H_
