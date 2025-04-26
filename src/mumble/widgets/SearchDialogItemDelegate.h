// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_SEARCHDIALOGITEMDELEGATE_H_
#define MUMBLE_MUMBLE_WIDGETS_SEARCHDIALOGITEMDELEGATE_H_

#include <QSize>
#include <QStyledItemDelegate>

class QPainter;
class QStyleOptionViewItem;
class QModelIndex;
class QObject;

class SearchDialogItemDelegate : public QStyledItemDelegate {
	Q_OBJECT
	Q_DISABLE_COPY(SearchDialogItemDelegate)

public:
	constexpr static int CHANNEL_TREE_ROLE            = Qt::UserRole + 1;
	constexpr static uint8_t BOTTOM_MARGIN            = 0;
	constexpr static uint8_t CHANNEL_TREE_LEFT_INDENT = 5;

	SearchDialogItemDelegate(QObject *parent = nullptr);

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
	void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};


#endif // MUMBLE_MUMBLE_WIDGETS_SEARCHDIALOGITEMDELEGATE_H_
