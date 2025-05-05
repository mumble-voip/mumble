// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GlobalShortcutButtons.h"
#include "ui_GlobalShortcutButtons.h"

#include "GlobalShortcut.h"

#include "MainWindow.h"
#include "Global.h"

#include <QRect>
#include <QTimer>

GlobalShortcutButtons::GlobalShortcutButtons(QWidget *parent) : QDialog(parent), m_ui(new Ui::GlobalShortcutButtons) {
	m_ui->setupUi(this);
	setModal(true);

	connect(m_ui->addButton, &QPushButton::toggled, this, &GlobalShortcutButtons::toggleCapture);
	connect(m_ui->removeButton, &QPushButton::clicked, this, &GlobalShortcutButtons::removeItem);

	// Due to what appears to be a Qt bug, this dialog will not take
	// the parent geometry into account when spawning.
	// Therefore, we search for a different parent or even fallback to the
	// MainWindow and move this dialog to its center.
	QWidget *searchWidget = parent;
	while (searchWidget && !qobject_cast< QDialog * >(searchWidget)) {
		searchWidget = searchWidget->parentWidget();
	}
	QWidget *effectiveParent = qobject_cast< QDialog * >(searchWidget);
	if (!effectiveParent && Global::get().mw && Global::get().mw->isVisible()) {
		effectiveParent = Global::get().mw;
	}

	int32_t posx = geometry().x();
	int32_t posy = geometry().y();

	if (effectiveParent) {
		QRect geometry = effectiveParent->geometry();
		posx           = geometry.center().x() - (width() / 2);
		posy           = geometry.center().y() - (height() / 2);
	}

	QTimer::singleShot(0, [this, posx, posy]() { move(posx, posy); });
}

GlobalShortcutButtons::~GlobalShortcutButtons() {
}

bool GlobalShortcutButtons::eventFilter(QObject *, QEvent *event) {
	return event->type() == QEvent::MouseButtonPress || event->type() == QEvent::KeyPress;
}

QList< QVariant > GlobalShortcutButtons::buttons() {
	if (result() != Accepted) {
		return m_previousButtons;
	}

	QList< QVariant > buttons;

	const QTreeWidgetItem *rootItem = m_ui->buttonTree->invisibleRootItem();
	for (int i = 0; i < rootItem->childCount(); ++i) {
		const QTreeWidgetItem *buttonItem = rootItem->child(i);
		for (int k = 0; k < buttonItem->childCount(); ++k) {
			buttons.append(buttonItem->child(k)->data(0, Qt::UserRole));
		}
	}

	return buttons;
}

void GlobalShortcutButtons::setButtons(const QList< QVariant > &buttons) {
	m_previousButtons = buttons;

	for (const auto &button : buttons) {
		addItem(button);
	}

	adjustSize();

	// Without this the new dialog window will not be focused and the
	// shortcut edit dialog is closed when TAB is pressed...
	// Due to Qt action processing weirdness the timer is needed, otherwise
	// the call has no effect.
	if (buttons.isEmpty()) {
		QTimer::singleShot(0, [&]() { m_ui->addButton->setFocus(Qt::TabFocusReason); });
	} else {
		QTimer::singleShot(0, [&]() { m_ui->buttonTree->setFocus(Qt::TabFocusReason); });
	}
}

void GlobalShortcutButtons::addItem(const QVariant &button) {
	const auto info = GlobalShortcutEngine::engine->buttonInfo(button);
	auto items      = m_ui->buttonTree->findItems(info.device, Qt::MatchExactly);

	QTreeWidgetItem *deviceItem;
	if (!items.isEmpty()) {
		deviceItem = items[0];
	} else {
		deviceItem = new QTreeWidgetItem(m_ui->buttonTree);
		deviceItem->setText(0, info.device);
	}

	for (auto i = 0; i < deviceItem->childCount(); ++i) {
		if (deviceItem->child(i)->text(0) == info.name) {
			return;
		}
	}

	auto buttonItem = new QTreeWidgetItem(deviceItem);
	buttonItem->setData(0, Qt::UserRole, button);
	buttonItem->setText(0, info.name);
}

void GlobalShortcutButtons::removeItem() {
	const auto buttonTree = m_ui->buttonTree;
	auto selectedItems    = buttonTree->selectedItems();
	// If we delete a top item, its children are deleted as well.
	// We cannot delete all selected items in one shot because
	// we risk deleting a child that doesn't exist anymore.
	while (selectedItems.count() > 0) {
		delete selectedItems[0];
		selectedItems = buttonTree->selectedItems();
	}
}

void GlobalShortcutButtons::toggleCapture(const bool enabled) {
	if (enabled) {
		installEventFilter(this);
		GlobalShortcutEngine::engine->resetMap();
		connect(GlobalShortcutEngine::engine, &GlobalShortcutEngine::buttonPressed, this,
				&GlobalShortcutButtons::updateButtons);
		m_ui->addButton->setText(QObject::tr("Listening for input"));
	} else {
		disconnect(GlobalShortcutEngine::engine, &GlobalShortcutEngine::buttonPressed, this,
				   &GlobalShortcutButtons::updateButtons);
		removeEventFilter(this);
		m_ui->addButton->setText(QObject::tr("Add"));
	}
}

void GlobalShortcutButtons::updateButtons(const bool last) {
	for (const auto &button : GlobalShortcutEngine::engine->qlActiveButtons) {
		addItem(button);
	}

	if (last) {
		m_ui->addButton->setChecked(false);
	}
}
