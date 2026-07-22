// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#	include "ScreenPickerDialog.h"

#	include "CaptureSourceLister.h"

#	include <QtWidgets/QDialogButtonBox>
#	include <QtWidgets/QLabel>
#	include <QtWidgets/QListWidget>
#	include <QtWidgets/QPushButton>
#	include <QtWidgets/QVBoxLayout>

ScreenPickerDialog::ScreenPickerDialog(QWidget *parent) : QDialog(parent) {
	setWindowTitle(tr("Choose what to share"));
	setMinimumSize(640, 480);

	m_list = new QListWidget(this);
	m_list->setViewMode(QListView::IconMode);
	m_list->setIconSize(QSize(160, 90));
	m_list->setResizeMode(QListView::Adjust);
	m_list->setSpacing(8);
	m_list->setWordWrap(true);
	m_list->setMovement(QListView::Static);
	m_list->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(m_list, &QListWidget::itemDoubleClicked, this, &ScreenPickerDialog::onItemDoubleClicked);

	auto *hint = new QLabel(tr("Select a screen or window to share, then click Share."), this);
	hint->setWordWrap(true);

	auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	buttons->button(QDialogButtonBox::Ok)->setText(tr("Share"));
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

	auto *layout = new QVBoxLayout(this);
	layout->addWidget(hint);
	layout->addWidget(m_list, 1);
	layout->addWidget(buttons);
	setLayout(layout);

	// Populate list.
	m_sources = listCaptureSources();
	for (const CaptureSource &src : m_sources) {
		QIcon icon = src.thumbnail.isNull() ? QIcon() : QIcon(src.thumbnail);
		auto *item = new QListWidgetItem(icon, src.displayName);
		item->setToolTip(src.displayName);
		m_list->addItem(item);
	}

	if (m_list->count() > 0)
		m_list->setCurrentRow(0);
}

CaptureSource ScreenPickerDialog::selectedSource() const {
	const int row = m_list->currentRow();
	if (row >= 0 && row < m_sources.size())
		return m_sources.at(row);
	return {};
}

void ScreenPickerDialog::onItemDoubleClicked(QListWidgetItem *) {
	accept();
}

#endif // USE_SCREEN_SHARING
