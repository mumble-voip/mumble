// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_RICHTEXTITEMDELEGATE_H_
#define MUMBLE_MUMBLE_WIDGETS_RICHTEXTITEMDELEGATE_H_

#include <QSize>
#include <QStyledItemDelegate>

class QPainter;
class QStyledItemDelegate;
class QModelIndex;

/**
 * An ItemDelegate that is capable of rendering rich text. It can be used in order to bring
 * rich text support to the *View and *Widget objects (e.g. TreeWidget).
 */
class RichTextItemDelegate : public QStyledItemDelegate {
	Q_OBJECT
	Q_DISABLE_COPY(RichTextItemDelegate)

public:
	// inherit constructors
	using QStyledItemDelegate::QStyledItemDelegate;

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // MUMBLE_MUMBLE_WIDGETS_RICHTEXTITEMDELEGATE_H_
