// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SearchDialogItemDelegate.h"

#include "QtWidgetUtils.h"

#include <QApplication>
#include <QHeaderView>
#include <QPainter>
#include <QScrollBar>
#include <QTextDocument>
#include <QTextOption>
#include <QTreeWidget>

SearchDialogItemDelegate::SearchDialogItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void setupDocuments(QTextDocument &resultDoc, QTextDocument &channelTreeDoc, const QStyleOptionViewItem &option,
					const uint8_t channelTreePadding) {
	QTextOption resultTextOption;
	resultTextOption.setWrapMode(QTextOption::NoWrap);
	resultTextOption.setTextDirection(option.direction);
	resultTextOption.setAlignment(Qt::AlignLeft);

	QTextOption channelTreeTextOption;
	channelTreeTextOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	channelTreeTextOption.setTextDirection(option.direction);
	channelTreeTextOption.setAlignment(Qt::AlignLeft);

	// Render the channel tree with a smaller font
	QFont channelTreeFont(option.font);
	if (channelTreeFont.pointSize() >= 0) {
		channelTreeFont.setPointSize(
			static_cast< int >(std::max(static_cast< float >(channelTreeFont.pointSize()) * 0.8f, 1.0f)));
	} else {
		// Font size is specified in pixels
		channelTreeFont.setPixelSize(
			static_cast< int >(std::max(static_cast< float >(channelTreeFont.pixelSize()) * 0.8f, 1.0f)));
	}

	resultDoc.setDefaultTextOption(resultTextOption);
	resultDoc.setDefaultFont(option.font);
	resultDoc.setDocumentMargin(1);
	resultDoc.setTextWidth(option.rect.width());

	channelTreeDoc.setDefaultTextOption(channelTreeTextOption);
	channelTreeDoc.setDefaultFont(channelTreeFont);
	channelTreeDoc.setDocumentMargin(1);
	channelTreeDoc.setTextWidth(std::max(option.rect.width() - channelTreePadding, 0));
}

void SearchDialogItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &inOption,
									 const QModelIndex &index) const {
	QStyleOptionViewItem option = inOption;
	initStyleOption(&option, index);

	QStyle *style = option.widget ? option.widget->style() : QApplication::style();

	// Painting item without text (this takes care of painting e.g. the highlighted for selected
	// or hovered over items in an ItemView)
	option.text = QString();
	style->drawControl(QStyle::CE_ItemViewItem, &option, painter, inOption.widget);


	const QString resultText      = index.data(Qt::DisplayRole).toString();
	const QString channelTreeText = index.data(CHANNEL_TREE_ROLE).toString();

	QTextDocument resultDoc;
	QTextDocument channelTreeDoc;

	setupDocuments(resultDoc, channelTreeDoc, option, CHANNEL_TREE_LEFT_INDENT);

	resultDoc.setHtml(resultText);
	resultDoc.adjustSize();

	channelTreeDoc.setHtml(channelTreeText);

	const QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &option);

	if (resultDoc.size().width() > textRect.width()) {
		Mumble::QtUtils::elideText(resultDoc, static_cast< unsigned int >(textRect.width()));
	}

	// Figure out where to render the text in order to follow the requested alignment
	QSize contentSize(static_cast< int >(
						  std::max(resultDoc.size().width(), channelTreeDoc.size().width() + CHANNEL_TREE_LEFT_INDENT)),
					  static_cast< int >(resultDoc.size().height() + channelTreeDoc.size().height() + BOTTOM_MARGIN));

	QRect contentLayoutRect =
		QStyle::alignedRect(Qt::LayoutDirectionAuto, option.displayAlignment, contentSize, textRect);

	painter->save();

	// Translate the painter to the origin of the layout rectangle in order for the text to be
	// rendered at the correct position
	QRect clipRect = textRect.translated(-textRect.topLeft());

	painter->translate(contentLayoutRect.topLeft());
	resultDoc.drawContents(painter, clipRect);
	painter->resetTransform();
	painter->translate(contentLayoutRect.topLeft());
	painter->translate(CHANNEL_TREE_LEFT_INDENT, resultDoc.size().height());
	channelTreeDoc.drawContents(painter, clipRect);

	painter->restore();
}

QSize SearchDialogItemDelegate::sizeHint(const QStyleOptionViewItem &inOption, const QModelIndex &index) const {
	QStyleOptionViewItem option = inOption;
	initStyleOption(&option, index);

	const QTreeWidget *widget = qobject_cast< const QTreeWidget * >(option.widget);

	const QSize size = widget->size();

	// In the SearchDialog, we have 2 sections. The first one for the icon and the second (last!) one for the
	// actual text. The latter is the one that we are interested in here.
	const int textWidth = widget->header()->sectionSize(widget->header()->count() - 1);

	option.rect.setWidth(textWidth);
	option.rect.setHeight(size.height());

	const QString resultText      = index.data(Qt::DisplayRole).toString();
	const QString channelTreeText = index.data(CHANNEL_TREE_ROLE).toString();

	QTextDocument resultDoc;
	QTextDocument channelTreeDoc;

	setupDocuments(resultDoc, channelTreeDoc, option, CHANNEL_TREE_LEFT_INDENT);

	resultDoc.setHtml(resultText);
	resultDoc.adjustSize();

	channelTreeDoc.setHtml(channelTreeText);

	const int hintWidth =
		std::max(static_cast< int >(
					 std::max(resultDoc.size().width(), channelTreeDoc.size().width() + CHANNEL_TREE_LEFT_INDENT)),
				 size.width());
	const int hintHeight =
		static_cast< int >(resultDoc.size().height() + channelTreeDoc.size().height() + BOTTOM_MARGIN);

	return { hintWidth, hintHeight };
}

void SearchDialogItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const {
	QStyledItemDelegate::initStyleOption(option, index);

	// Make sure icons are aligned top-left
	option->decorationAlignment = Qt::AlignLeft | Qt::AlignTop;
}
