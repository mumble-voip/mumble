// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "RichTextItemDelegate.h"

#include "QtWidgetUtils.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QModelIndex>
#include <QPainter>
#include <QSizeF>
#include <QStyleOptionViewItem>
#include <QTextDocument>
#include <QVariant>

#include <cmath>

void RichTextItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &inOption,
								 const QModelIndex &index) const {
	QStyleOptionViewItem option = inOption;
	initStyleOption(&option, index);

	if (option.text.isEmpty()) {
		// This is nothing this function is supposed to handle
		QStyledItemDelegate::paint(painter, inOption, index);

		return;
	}

	QStyle *style = option.widget ? option.widget->style() : QApplication::style();

	QTextOption textOption;
	textOption.setWrapMode(option.features & QStyleOptionViewItem::WrapText ? QTextOption::WordWrap
																			: QTextOption::ManualWrap);
	textOption.setTextDirection(option.direction);

	QTextDocument doc;
	doc.setDefaultTextOption(textOption);
	doc.setHtml(option.text);
	doc.setDefaultFont(option.font);
	doc.setDocumentMargin(1);
	doc.setTextWidth(option.rect.width());
	doc.adjustSize();

	if (doc.size().width() > option.rect.width()) {
		Mumble::QtUtils::elideText(doc, static_cast< std::uint32_t >(option.rect.width()));
	}

	// Painting item without text (this takes care of painting e.g. the highlighted for selected
	// or hovered over items in an ItemView)
	option.text = QString();
	style->drawControl(QStyle::CE_ItemViewItem, &option, painter, inOption.widget);

	// Figure out where to render the text in order to follow the requested alignment
	QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &option);
	QSize documentSize(static_cast< int >(doc.size().width()),
					   static_cast< int >(doc.size().height())); // Convert QSizeF to QSize
	QRect layoutRect = QStyle::alignedRect(Qt::LayoutDirectionAuto, option.displayAlignment, documentSize, textRect);

	painter->save();

	// Translate the painter to the origin of the layout rectangle in order for the text to be
	// rendered at the correct position
	painter->translate(layoutRect.topLeft());
	doc.drawContents(painter, textRect.translated(-textRect.topLeft()));

	painter->restore();
}

QSize RichTextItemDelegate::sizeHint(const QStyleOptionViewItem &inOption, const QModelIndex &index) const {
	QStyleOptionViewItem option = inOption;
	initStyleOption(&option, index);

	if (option.text.isEmpty()) {
		// This is nothing this function is supposed to handle
		return QStyledItemDelegate::sizeHint(inOption, index);
	}

	QTextDocument doc;
	doc.setHtml(option.text);
	doc.setTextWidth(option.rect.width());
	doc.setDefaultFont(option.font);
	doc.setDocumentMargin(1);

	return QSize(static_cast< int >(doc.idealWidth()), static_cast< int >(doc.size().height()));
}
