// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GlobalShortcut.h"

#include "AudioInput.h"
#include "Channel.h"
#include "ClientUser.h"
#include "Database.h"
#include "EnvUtils.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#include "widgets/EventFilters.h"
#include "Global.h"
#include "GlobalShortcutButtons.h"

#include <QtCore/QProcess>
#include <QtCore/QSortFilterProxyModel>
#include <QtGui/QHelpEvent>
#include <QtWidgets/QItemEditorFactory>
#include <QtWidgets/QToolTip>

#ifdef Q_OS_MAC
#	include <ApplicationServices/ApplicationServices.h>
#	include <QtCore/QOperatingSystemVersion>
#endif

#include <cassert>

const QString GlobalShortcutConfig::name = QLatin1String("GlobalShortcutConfig");

/**
 * Used to save the global, unique, platform specific GlobalShortcutEngine.
 */
GlobalShortcutEngine *GlobalShortcutEngine::engine = nullptr;

static ConfigWidget *GlobalShortcutConfigDialogNew(Settings &st) {
	return new GlobalShortcutConfig(st);
}

static ConfigRegistrar registrarGlobalShortcut(1200, GlobalShortcutConfigDialogNew);

static const QString UPARROW = QString::fromUtf8("\xE2\x86\x91 ");

ShortcutActionWidget::ShortcutActionWidget(QWidget *p) : QWidget(p) {
	int idx = 0;

	m_comboBox = new MUComboBox();
	m_comboBox->insertItem(idx, tr("Unassigned"));
	m_comboBox->setItemData(idx, -1);
#ifndef Q_OS_MAC
	m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
#endif

	idx++;

	QFontMetrics fontMetrics = m_comboBox->fontMetrics();
	int maxWidth             = m_comboBox->minimumWidth();

	foreach (GlobalShortcut *gs, GlobalShortcutEngine::engine->qmShortcuts) {
		m_comboBox->insertItem(idx, gs->name);
		m_comboBox->setItemData(idx, gs->idx);
		if (!gs->qsToolTip.isEmpty()) {
			m_comboBox->setItemData(idx, gs->qsToolTip, Qt::ToolTipRole);
		}
		if (!gs->qsWhatsThis.isEmpty()) {
			m_comboBox->setItemData(idx, gs->qsWhatsThis, Qt::WhatsThisRole);
		}
		idx++;

		maxWidth = std::max(maxWidth, fontMetrics.horizontalAdvance(gs->name));
	}

	// Sort the ShortcutActionWidget items
	QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
	proxy->setSourceModel(m_comboBox->model());

	m_comboBox->model()->setParent(proxy);
	m_comboBox->setModel(proxy);

	m_comboBox->model()->sort(0);

	m_comboBox->setFocusPolicy(Qt::NoFocus);

	setMinimumWidth(maxWidth);
	m_comboBox->view()->setMinimumWidth(maxWidth);
	m_comboBox->adjustSize();

	m_comboBox->setParent(this);

	adjustSize();

	KeyEventObserver *eventFilter = new KeyEventObserver(this, QEvent::KeyPress, true, { Qt::Key_Space });
	connect(eventFilter, &KeyEventObserver::keyEventObserved, this, [this]() { m_comboBox->showPopup(); });
	installEventFilter(eventFilter);

	QTreeWidget *treeWidget = qobject_cast< QTreeWidget * >(p->parentWidget());
	if (treeWidget) {
		treeWidget->resizeColumnToContents(0);
	}
}

void ShortcutActionWidget::setIndex(unsigned int idx) {
	m_comboBox->setCurrentIndex(m_comboBox->findData(idx));
}

unsigned int ShortcutActionWidget::index() const {
	return m_comboBox->itemData(m_comboBox->currentIndex()).toUInt();
}

ShortcutToggleWidget::ShortcutToggleWidget(QWidget *p) : MUComboBox(p) {
	int idx = 0;

	insertItem(idx, tr("Off"));
	setItemData(idx, -1);
	idx++;

	insertItem(idx, tr("Toggle"));
	setItemData(idx, 0);
	idx++;

	insertItem(idx, tr("On"));
	setItemData(idx, 1);
	idx++;
}

void ShortcutToggleWidget::setIndex(int idx) {
	setCurrentIndex(findData(idx));
}

int ShortcutToggleWidget::index() const {
	return itemData(currentIndex()).toInt();
}

ChannelSelectWidget::ChannelSelectWidget(QWidget *parent) : MUComboBox(parent) {
	QReadLocker lock(&Channel::c_qrwlChannels);

	int index = 0;

	for (const Channel *currentChannel : Channel::c_qhChannels) {
		assert(currentChannel);

		insertItem(index, currentChannel->qsName);
		setItemData(index, currentChannel->iId);

		index++;
	}

	model()->sort(0);
}

void ChannelSelectWidget::setCurrentChannel(const ChannelTarget &target) {
	setCurrentIndex(findData(target.channelID));
}

ChannelTarget ChannelSelectWidget::currentChannel() const {
	return itemData(currentIndex()).toUInt();
}

void iterateChannelChildren(QTreeWidgetItem *root, Channel *chan, QMap< int, QTreeWidgetItem * > &map) {
	foreach (Channel *c, chan->qlChannels) {
		QTreeWidgetItem *sub = new QTreeWidgetItem(root, QStringList(c->qsName));
		sub->setData(0, Qt::UserRole, c->iId);
		map.insert(static_cast< int >(c->iId), sub);
		iterateChannelChildren(sub, c, map);
	}
}

ShortcutTargetDialog::ShortcutTargetDialog(const ShortcutTarget &st, QWidget *pw) : QDialog(pw) {
	stTarget = st;
	setupUi(this);


	// Load current shortcut configuration
	qcbForceCenter->setChecked(st.bForceCenter);
	qgbModifiers->setVisible(true);

	if (st.bCurrentSelection) {
		qcbTarget->setCurrentIndex(Target::SELECTION);
		qswStack->setCurrentWidget(qwSelectionPage);
	} else if (st.bUsers) {
		qcbTarget->setCurrentIndex(Target::USERLIST);
		qswStack->setCurrentWidget(qwUserPage);
	} else {
		qcbTarget->setCurrentIndex(Target::CHANNEL);
		qswStack->setCurrentWidget(qwChannelPage);
	}

	qcbLinks->setChecked(st.bLinks);
	qcbShoutLinks->setChecked(st.bLinks);
	qcbChildren->setChecked(st.bChildren);
	qcbShoutSubchans->setChecked(st.bChildren);

	// Insert all known friends into the possible targets list
	const QMap< QString, QString > &friends = Global::get().db->getFriends();
	if (!friends.isEmpty()) {
		QMap< QString, QString >::const_iterator i;
		for (i = friends.constBegin(); i != friends.constEnd(); ++i) {
			qcbUser->addItem(i.key(), i.value());
			qmHashNames.insert(i.value(), i.key());
		}
		qcbUser->insertSeparator(qcbUser->count());
	}

	// If we are connected to a server also add all connected players with certificates to the list
	if (Global::get().uiSession) {
		QMap< QString, QString > others;
		QMap< QString, QString >::const_iterator i;

		QReadLocker lock(&ClientUser::c_qrwlUsers);
		foreach (ClientUser *p, ClientUser::c_qmUsers) {
			if ((p->uiSession != Global::get().uiSession) && p->qsFriendName.isEmpty() && !p->qsHash.isEmpty()) {
				others.insert(p->qsName, p->qsHash);
				qmHashNames.insert(p->qsHash, p->qsName);
			}
		}

		for (i = others.constBegin(); i != others.constEnd(); ++i) {
			qcbUser->addItem(i.key(), i.value());
		}
	}

	QMap< QString, QString > users;

	foreach (const QString &hash, st.qlUsers) {
		if (qmHashNames.contains(hash))
			users.insert(qmHashNames.value(hash), hash);
		else
			users.insert(QString::fromLatin1("#%1").arg(hash), hash);
	}

	{
		QMap< QString, QString >::const_iterator i;
		for (i = users.constBegin(); i != users.constEnd(); ++i) {
			QListWidgetItem *itm = new QListWidgetItem(i.key());
			itm->setData(Qt::UserRole, i.value());
			qlwUsers->addItem(itm);
		}
	}

	// Now generate the tree of possible channel targets, first add the default ones
	QMap< int, QTreeWidgetItem * > qmTree;

	QTreeWidgetItem *root = new QTreeWidgetItem(qtwChannels, QStringList(tr("Root")));
	root->setData(0, Qt::UserRole, SHORTCUT_TARGET_ROOT);
	root->setExpanded(true);
	qmTree.insert(-1, root);

	QTreeWidgetItem *parent_item = new QTreeWidgetItem(root, QStringList(tr("Parent")));
	parent_item->setData(0, Qt::UserRole, SHORTCUT_TARGET_PARENT);
	parent_item->setExpanded(true);
	qmTree.insert(-2, parent_item);

	QTreeWidgetItem *current = new QTreeWidgetItem(parent_item, QStringList(tr("Current")));
	current->setData(0, Qt::UserRole, SHORTCUT_TARGET_CURRENT);
	qmTree.insert(-3, current);

	for (int i = 0; i < 8; ++i) {
		QTreeWidgetItem *sub = new QTreeWidgetItem(current, QStringList(tr("Subchannel #%1").arg(i + 1)));
		sub->setData(0, Qt::UserRole, SHORTCUT_TARGET_SUBCHANNEL - i);
		qmTree.insert(SHORTCUT_TARGET_SUBCHANNEL - i, sub);
	}

	for (int i = 0; i < 8; ++i) {
		QTreeWidgetItem *psub =
			new QTreeWidgetItem(parent_item, QStringList(UPARROW + tr("Subchannel #%1").arg(i + 1)));
		psub->setData(0, Qt::UserRole, SHORTCUT_TARGET_PARENT_SUBCHANNEL - i);
		qmTree.insert(SHORTCUT_TARGET_PARENT_SUBCHANNEL - i, psub);
	}

	// And if we are connected add the channels on the current server
	if (Global::get().uiSession) {
		Channel *c             = Channel::get(Channel::ROOT_ID);
		QTreeWidgetItem *sroot = new QTreeWidgetItem(qtwChannels, QStringList(c->qsName));
		qmTree.insert(0, sroot);
		iterateChannelChildren(sroot, c, qmTree);
	}

	qtwChannels->sortByColumn(0, Qt::AscendingOrder);

	QTreeWidgetItem *qtwi;
	if (Global::get().uiSession) {
		qtwi = qmTree.value(static_cast< int >(ClientUser::get(Global::get().uiSession)->cChannel->iId));
		if (qtwi)
			qtwChannels->scrollToItem(qtwi);
	}

	qtwi = qmTree.value(st.iChannel);
	if (qtwi) {
		qtwChannels->scrollToItem(qtwi);
		qtwChannels->setCurrentItem(qtwi);
	}

	qleGroup->setText(stTarget.qsGroup);
}

ShortcutTarget ShortcutTargetDialog::target() const {
	return stTarget;
}

void ShortcutTargetDialog::accept() {
	if (qswStack->currentWidget() == qwSelectionPage) {
		stTarget.bLinks    = qcbShoutLinks->isChecked();
		stTarget.bChildren = qcbShoutSubchans->isChecked();
	} else {
		stTarget.bLinks    = qcbLinks->isChecked();
		stTarget.bChildren = qcbChildren->isChecked();
	}

	stTarget.bForceCenter = qcbForceCenter->isChecked();

	stTarget.qlUsers.clear();
	stTarget.qsGroup.clear();
	stTarget.iChannel = -3; // Reset it to the value it has been assigned in the constructor of ShotcutTarget

	if (!stTarget.bCurrentSelection) {
		QList< QListWidgetItem * > ql = qlwUsers->findItems(QString(), Qt::MatchStartsWith);
		foreach (QListWidgetItem *itm, ql) { stTarget.qlUsers << itm->data(Qt::UserRole).toString(); }

		QTreeWidgetItem *qtwi = qtwChannels->currentItem();
		if (qtwi) {
			stTarget.iChannel = qtwi->data(0, Qt::UserRole).toInt();
			stTarget.qsGroup  = qleGroup->text().trimmed();
		}
	}

	QDialog::accept();
}

void ShortcutTargetDialog::on_qcbTarget_currentIndexChanged(int index) {
	switch (index) {
		default:
			qWarning("GlobalShortcutDialog: Encountered impossible target index! => bug");
			// Fallthrough
		case Target::SELECTION:
			stTarget.bUsers            = false;
			stTarget.bCurrentSelection = true;

			qswStack->setCurrentWidget(qwSelectionPage);
			break;
		case Target::USERLIST:
			stTarget.bUsers            = true;
			stTarget.bCurrentSelection = false;

			qswStack->setCurrentWidget(qwUserPage);
			break;
		case Target::CHANNEL:
			stTarget.bUsers            = false;
			stTarget.bCurrentSelection = false;

			qswStack->setCurrentWidget(qwChannelPage);
			break;
	}
}

void ShortcutTargetDialog::on_qpbAdd_clicked() {
	if (qcbUser->currentIndex() < 0)
		return;

	QListWidgetItem *itm = new QListWidgetItem(qcbUser->currentText());
	itm->setData(Qt::UserRole, qcbUser->itemData(qcbUser->currentIndex()));
	qlwUsers->addItem(itm);
}

void ShortcutTargetDialog::on_qpbRemove_clicked() {
	QListWidgetItem *itm = qlwUsers->currentItem();
	delete itm;
}

ShortcutTargetWidget::ShortcutTargetWidget(QWidget *p) : QFrame(p) {
	qleTarget = new QLineEdit();
	qleTarget->setReadOnly(true);
	qleTarget->setFocusPolicy(Qt::NoFocus);

	qtbEdit = new QToolButton();
	qtbEdit->setText(tr("..."));
	qtbEdit->setFocusPolicy(Qt::ClickFocus);
	qtbEdit->setObjectName(QLatin1String("qtbEdit"));

	QHBoxLayout *l = new QHBoxLayout(this);
	l->setContentsMargins(0, 0, 0, 0);
	l->addWidget(qleTarget, 1);
	l->addWidget(qtbEdit);

	KeyEventObserver *eventFilter = new KeyEventObserver(this, QEvent::KeyPress, true, { Qt::Key_Space });
	connect(eventFilter, &KeyEventObserver::keyEventObserved, this, [this]() { qtbEdit->click(); });
	installEventFilter(eventFilter);

	QMetaObject::connectSlotsByName(this);
}

TextEditWidget::TextEditWidget(QWidget *p) : QWidget(p) {
	m_lineEdit = new QLineEdit();
	m_lineEdit->setFocusPolicy(Qt::ClickFocus);

	QHBoxLayout *l = new QHBoxLayout(this);
	l->setContentsMargins(0, 0, 0, 0);
	l->setSpacing(0);
	l->addWidget(m_lineEdit);

	KeyEventObserver *eventFilter = new KeyEventObserver(this, QEvent::KeyPress, true, { Qt::Key_Space });
	connect(eventFilter, &KeyEventObserver::keyEventObserved, this,
			[this]() { m_lineEdit->setFocus(Qt::MouseFocusReason); });
	installEventFilter(eventFilter);

	QMetaObject::connectSlotsByName(this);
}

QString TextEditWidget::currentString() const {
	return m_lineEdit->text();
}

void TextEditWidget::setCurrentString(const QString &str) {
	m_lineEdit->setText(str);
}

/**
 * This function returns a textual representation of the given shortcut target st.
 */
QString ShortcutTargetWidget::targetString(const ShortcutTarget &st) {
	if (st.bCurrentSelection) {
		return tr("Current selection");
	} else if (st.bUsers) {
		if (!st.qlUsers.isEmpty()) {
			QMap< QString, QString > hashes;

			QReadLocker lock(&ClientUser::c_qrwlUsers);
			foreach (ClientUser *p, ClientUser::c_qmUsers) {
				if (!p->qsHash.isEmpty()) {
					hashes.insert(p->qsHash, p->qsName);
				}
			}

			QStringList users;
			foreach (const QString &hash, st.qlUsers) {
				QString name;
				if (hashes.contains(hash)) {
					name = hashes.value(hash);
				} else {
					name = Global::get().db->getFriend(hash);
					if (name.isEmpty())
						name = QString::fromLatin1("#%1").arg(hash);
				}
				users << name;
			}

			users.sort();
			return users.join(tr(", "));
		}
	} else {
		if (st.iChannel < 0) {
			switch (st.iChannel) {
				case SHORTCUT_TARGET_ROOT:
					return tr("Root");
				case SHORTCUT_TARGET_PARENT:
					return tr("Parent");
				case SHORTCUT_TARGET_CURRENT:
					return tr("Current");
				default:
					if (st.iChannel <= SHORTCUT_TARGET_PARENT_SUBCHANNEL)
						return (UPARROW
								+ tr("Subchannel #%1").arg(SHORTCUT_TARGET_PARENT_SUBCHANNEL + 1 - st.iChannel));
					else
						return tr("Subchannel #%1").arg(SHORTCUT_TARGET_CURRENT - st.iChannel);
			}
		} else {
			Channel *c = Channel::get(static_cast< unsigned int >(st.iChannel));
			if (c)
				return c->qsName;
			else
				return tr("Invalid");
		}
	}
	return tr("Empty");
}

ShortcutTarget ShortcutTargetWidget::target() const {
	return stTarget;
}

void ShortcutTargetWidget::setTarget(const ShortcutTarget &st) {
	stTarget = st;
	qleTarget->setText(ShortcutTargetWidget::targetString(st));
}

void ShortcutTargetWidget::on_qtbEdit_clicked() {
	ShortcutTargetDialog *std = new ShortcutTargetDialog(stTarget, this);
	if (std->exec() == QDialog::Accepted) {
		stTarget = std->target();
		qleTarget->setText(ShortcutTargetWidget::targetString(stTarget));

		// Qt bug? Who knows, but since there won't be focusOut events for this widget anymore,
		// we need to force the commit.
		QWidget *p = parentWidget();
		while (p) {
			if (QAbstractItemView *qaiv = qobject_cast< QAbstractItemView * >(p)) {
				QStyledItemDelegate *qsid = qobject_cast< QStyledItemDelegate * >(qaiv->itemDelegate());
				if (qsid) {
					QMetaObject::invokeMethod(qsid, "_q_commitDataAndCloseEditor", Qt::QueuedConnection,
											  Q_ARG(QWidget *, this));
				}
				break;
			}
			p = p->parentWidget();
		}
	}
	delete std;
}

ShortcutDelegate::ShortcutDelegate(QObject *p) : QStyledItemDelegate(p) {
	QItemEditorFactory *factory = new QItemEditorFactory;

	factory->registerEditor(QMetaType::QVariantList, new QStandardItemEditorCreator< GlobalShortcutButtons >());
	factory->registerEditor(QMetaType::UInt, new QStandardItemEditorCreator< ShortcutActionWidget >());
	factory->registerEditor(QMetaType::Int, new QStandardItemEditorCreator< ShortcutToggleWidget >());
	factory->registerEditor(static_cast< int >(QVariant::fromValue(ShortcutTarget()).userType()),
							new QStandardItemEditorCreator< ShortcutTargetWidget >());
	factory->registerEditor(static_cast< int >(QVariant::fromValue(ChannelTarget()).userType()),
							new QStandardItemEditorCreator< ChannelSelectWidget >());
	factory->registerEditor(QMetaType::QString, new QStandardItemEditorCreator< TextEditWidget >());
	factory->registerEditor(QMetaType::UnknownType, new QStandardItemEditorCreator< QWidget >());
	setItemEditorFactory(factory);
}

ShortcutDelegate::~ShortcutDelegate() {
	delete itemEditorFactory();
	setItemEditorFactory(nullptr);
}

/**
 * Provides textual representations for the mappings done for the edit behaviour.
 */
QString ShortcutDelegate::displayText(const QVariant &item, const QLocale &loc) const {
	if (item.userType() == QVariant::fromValue(ShortcutTarget()).userType()) {
		return ShortcutTargetWidget::targetString(item.value< ShortcutTarget >());
	} else if (item.userType() == QVariant::fromValue(ChannelTarget()).userType()) {
		ChannelTarget target = item.value< ChannelTarget >();

		const Channel *c = Channel::get(target.channelID);
		if (c) {
			return c->qsName;
		} else {
			return tr("< Unknown Channel >");
		}
	}
	switch (item.typeId()) {
		case QMetaType::Int: {
			const auto v = item.toInt();
			if (v > 0) {
				return tr("On");
			} else if (v < 0) {
				return tr("Off");
			}

			return tr("Toggle");
		}
		case QMetaType::UInt: {
			const auto shortcut = GlobalShortcutEngine::engine->qmShortcuts.value(item.toInt());
			return shortcut ? shortcut->name : tr("Unassigned");
		}
		case QMetaType::QVariantList: {
			const auto buttons = item.value< QList< QVariant > >();
			if (buttons.count() > 0) {
				QString text;
				for (const auto &button : buttons) {
					if (!text.isEmpty()) {
						text += ' ';
					}

					const auto info = GlobalShortcutEngine::engine->buttonInfo(button);
					text += QString("'%1%2'").arg(info.devicePrefix, info.name);
				}

				return text;
			} else {
				return tr("No buttons assigned");
			}
		}
		default:
			qWarning("ShortcutDelegate::displayText(): Unknown type %d", item.typeId());
	}

	return QStyledItemDelegate::displayText(item, loc);
}

bool ShortcutDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *, const QStyleOptionViewItem &,
								 const QModelIndex &index) {
	if (event->type() != QEvent::ToolTip || !index.isValid()) {
		return false;
	}

	QToolTip::showText(event->globalPos(), tr("Press to show button combination"));
	return true;
}

GlobalShortcutConfig::GlobalShortcutConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);
	installEventFilter(this);

	bool canSuppress = GlobalShortcutEngine::engine->canSuppress();
	bool canDisable  = GlobalShortcutEngine::engine->canDisable();

	qwWarningContainer->setVisible(false);

#ifdef Q_OS_WIN
	qgbWindowsShortcutEngines->setVisible(true);
#else
	qgbWindowsShortcutEngines->setVisible(false);
#endif

	qtwShortcuts->setColumnCount(canSuppress ? 4 : 3);
	qtwShortcuts->setItemDelegate(new ShortcutDelegate(qtwShortcuts));

	qtwShortcuts->headerItem()->setData(0, Qt::AccessibleTextRole, tr("Shortcut action"));
	qtwShortcuts->headerItem()->setData(1, Qt::AccessibleTextRole, tr("Shortcut data"));
	qtwShortcuts->headerItem()->setData(2, Qt::AccessibleTextRole, tr("Shortcut input combinations"));

	qtwShortcuts->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	qtwShortcuts->header()->resizeSection(0, 150);
	qtwShortcuts->header()->setSectionResizeMode(2, QHeaderView::Stretch);
	if (canSuppress) {
		qtwShortcuts->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
	}

	qcbEnableGlobalShortcuts->setVisible(canDisable);

	qlWaylandNote->setVisible(false);
#ifdef Q_OS_LINUX
	if (EnvUtils::waylandIsUsed()) {
		// Our global shortcut system doesn't work properly with Wayland
		qlWaylandNote->setVisible(true);
	}
#endif

#ifdef Q_OS_MAC
	// Help Mac users enable accessibility access for Mumble...
	const QOperatingSystemVersion current = QOperatingSystemVersion::current();
	if (current >= QOperatingSystemVersion::OSXMavericks) {
		qpbOpenAccessibilityPrefs->setHidden(true);
		label->setText(tr("<html><head/><body>"
						  "<p>"
						  "Mumble can currently only use mouse buttons and keyboard modifier keys (Alt, Ctrl, Cmd, "
						  "etc.) for global shortcuts."
						  "</p>"
						  "<p>"
						  "If you want more flexibility, you can add Mumble as a trusted accessibility program in the "
						  "Security & Privacy section "
						  "of your Mac's System Preferences."
						  "</p>"
						  "<p>"
						  "In the Security & Privacy preference pane, change to the Privacy tab. Then choose "
						  "Accessibility (near the bottom) in "
						  "the list to the left. Finally, add Mumble to the list of trusted accessibility programs."
						  "</body></html>"));
	}
#endif
}

bool GlobalShortcutConfig::eventFilter(QObject * /*object*/, QEvent *e) {
#ifdef Q_OS_MAC
	if (e->type() == QEvent::WindowActivate) {
		if (!Global::get().s.bSuppressMacEventTapWarning) {
			qwWarningContainer->setVisible(showWarning());
		}
	}
#else
	Q_UNUSED(e)
#endif
	return false;
}

bool GlobalShortcutConfig::showWarning() const {
#ifdef Q_OS_MAC
#	if MAC_OS_X_VERSION_MAX_ALLOWED >= 1090
	const QOperatingSystemVersion current = QOperatingSystemVersion::current();
	if (current >= QOperatingSystemVersion::OSXMavericks) {
		return !AXIsProcessTrustedWithOptions(nullptr);
	} else
#	endif
	{
		return !QFile::exists(QLatin1String("/private/var/db/.AccessibilityAPIEnabled"));
	}
#endif
	return false;
}

void GlobalShortcutConfig::on_qpbOpenAccessibilityPrefs_clicked() {
	QStringList args;
	args << QLatin1String("/Applications/System Preferences.app");
	args << QLatin1String("/System/Library/PreferencePanes/UniversalAccessPref.prefPane");
	(void) QProcess::startDetached(QLatin1String("/usr/bin/open"), args);
}

void GlobalShortcutConfig::on_qpbSkipWarning_clicked() {
	// Store to both global and local settings.  The 'Skip' is live, as in
	// we don't expect the user to click Apply for their choice to work.
	Global::get().s.bSuppressMacEventTapWarning = s.bSuppressMacEventTapWarning = true;
	qwWarningContainer->setVisible(false);
}

void GlobalShortcutConfig::commit() {
	qtwShortcuts->closePersistentEditor(qtwShortcuts->currentItem(), qtwShortcuts->currentColumn());
}

void GlobalShortcutConfig::on_qcbEnableGlobalShortcuts_stateChanged(int state) {
	bool b = state == Qt::Checked;
	qpbAdd->setEnabled(b);
	if (!b)
		qpbRemove->setEnabled(false);
	else
		qpbRemove->setEnabled(qtwShortcuts->currentItem() ? true : false);
	qtwShortcuts->setEnabled(b);

	// We have to enable this here. Otherwise, adding new shortcuts wouldn't work.
	GlobalShortcutEngine::engine->setEnabled(b);
}

void GlobalShortcutConfig::on_qpbAdd_clicked(bool) {
	commit();
	Shortcut sc;
	sc.iIndex    = -1;
	sc.bSuppress = false;
	qlShortcuts << sc;
	reload();
	qtwShortcuts->setFocus(Qt::TabFocusReason);
}

void GlobalShortcutConfig::on_qpbRemove_clicked(bool) {
	commit();
	QTreeWidgetItem *qtwi = qtwShortcuts->currentItem();
	if (!qtwi)
		return;
	int idx = qtwShortcuts->indexOfTopLevelItem(qtwi);
	delete qtwi;
	qlShortcuts.removeAt(idx);

	// Clear the selected item. If we don't do this, the next shortcut in the list will be
	// automatically selected. This allows for the user to accidentally hit remove a second
	// time (as the mouse is over the remove button at this point already) leading to
	// another shortcut being deleted accidentally.
	qtwShortcuts->setCurrentItem(nullptr);
}

void GlobalShortcutConfig::on_qtwShortcuts_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *) {
	qpbRemove->setEnabled(item ? true : false);
}

void GlobalShortcutConfig::on_qtwShortcuts_itemChanged(QTreeWidgetItem *item, int) {
	int idx = qtwShortcuts->indexOfTopLevelItem(item);

	Shortcut &sc = qlShortcuts[idx];
	sc.iIndex    = item->data(0, Qt::DisplayRole).toInt();
	sc.qvData    = item->data(1, Qt::DisplayRole);
	sc.qlButtons = item->data(2, Qt::DisplayRole).toList();
	sc.bSuppress = item->checkState(3) == Qt::Checked;

	const ::GlobalShortcut *gs = GlobalShortcutEngine::engine->qmShortcuts.value(sc.iIndex);
	if (gs && sc.qvData.userType() != gs->qvDefault.userType()) {
		item->setData(1, Qt::DisplayRole, gs->qvDefault);
	}

	if (gs) {
		item->setData(0, Qt::AccessibleTextRole, gs->name);
	} else {
		item->setData(0, Qt::AccessibleTextRole, tr("Unassigned"));
	}
	item->setData(1, Qt::AccessibleTextRole, sc.qvData);
	item->setData(3, Qt::AccessibleDescriptionRole,
				  item->checkState(3) == Qt::Checked ? tr("checked") : tr("unchecked"));

	qtwShortcuts->resizeColumnToContents(0);
}

QString GlobalShortcutConfig::title() const {
	return tr("Shortcuts");
}

const QString &GlobalShortcutConfig::getName() const {
	return GlobalShortcutConfig::name;
}

QIcon GlobalShortcutConfig::icon() const {
	return QIcon(QLatin1String("skin:config_shortcuts.png"));
}

void GlobalShortcutConfig::load(const Settings &r) {
	qlShortcuts = r.qlShortcuts;

	// The 'Skip' button is supposed to be live, meaning users do not need to click Apply for
	// their choice of skipping to apply.
	//
	// To make this work well, we set the setting on load. This is to make 'Reset' and 'Restore Defaults'
	// work as expected.
	Global::get().s.bSuppressMacEventTapWarning = s.bSuppressMacEventTapWarning = r.bSuppressMacEventTapWarning;
	if (!Global::get().s.bSuppressMacEventTapWarning) {
		qwWarningContainer->setVisible(showWarning());
	}

	qcbEnableUIAccess->setChecked(r.bEnableUIAccess);
	qcbEnableGKey->setChecked(r.bEnableGKey);
	qcbEnableXboxInput->setChecked(r.bEnableXboxInput);

	qcbEnableGlobalShortcuts->setCheckState(r.bShortcutEnable ? Qt::Checked : Qt::Unchecked);
	on_qcbEnableGlobalShortcuts_stateChanged(qcbEnableGlobalShortcuts->checkState());
	reload();
}

void GlobalShortcutConfig::save() const {
	s.qlShortcuts     = qlShortcuts;
	s.bShortcutEnable = qcbEnableGlobalShortcuts->checkState() == Qt::Checked;

	bool oldUIAccess  = s.bEnableUIAccess;
	s.bEnableUIAccess = qcbEnableUIAccess->checkState() == Qt::Checked;

	bool oldGKey  = s.bEnableGKey;
	s.bEnableGKey = qcbEnableGKey->checkState() == Qt::Checked;

	bool oldXboxInput  = s.bEnableXboxInput;
	s.bEnableXboxInput = qcbEnableXboxInput->checkState() == Qt::Checked;

	if (s.bEnableUIAccess != oldUIAccess || s.bEnableGKey != oldGKey || s.bEnableXboxInput != oldXboxInput) {
		s.requireRestartToApply = true;
	}

	if (Global::get().sh && Global::get().sh->hasSynchronized()) {
		// If we are connected to a server (that has finished synchronizing) there is the change, the user has
		// configured server-specific shortcuts, which we save in the DB instead of in the regular settings. Thus we
		// have to explicitly save them here.
		// Note that the server needs to have finished synchronizing, since only then did we load any previously
		// configured shortcuts from the DB in the first place. Otherwise, "saving" the shortcuts (which are not
		// loaded yet) would effectively delete them.
		Global::get().db->setShortcuts(Global::get().sh->qbaDigest, s.qlShortcuts);
	}
}

QTreeWidgetItem *GlobalShortcutConfig::itemForShortcut(const Shortcut &sc) const {
	QTreeWidgetItem *item = new QTreeWidgetItem();
	::GlobalShortcut *gs  = GlobalShortcutEngine::engine->qmShortcuts.value(sc.iIndex);

	item->setData(0, Qt::DisplayRole, static_cast< unsigned int >(sc.iIndex));
	if (sc.qvData.isValid() && gs && (sc.qvData.metaType() == gs->qvDefault.metaType()))
		item->setData(1, Qt::DisplayRole, sc.qvData);
	else if (gs)
		item->setData(1, Qt::DisplayRole, gs->qvDefault);
	item->setData(2, Qt::DisplayRole, sc.qlButtons);
	item->setCheckState(3, sc.bSuppress ? Qt::Checked : Qt::Unchecked);
	item->setFlags(item->flags() | Qt::ItemIsEditable);


	if (gs) {
		if (!gs->qsToolTip.isEmpty())
			item->setData(0, Qt::ToolTipRole, gs->qsToolTip);
		if (!gs->qsWhatsThis.isEmpty())
			item->setData(0, Qt::WhatsThisRole, gs->qsWhatsThis);
	}

	item->setData(2, Qt::WhatsThisRole,
				  tr("<b>This is the global shortcut key combination.</b><br />"
					 "Click this field and then press the desired key/button combo "
					 "to rebind. Double-click to clear."));

	item->setData(3, Qt::ToolTipRole, tr("Suppress keys from other applications"));
	item->setData(3, Qt::WhatsThisRole,
				  tr("<b>This hides the button presses from other applications.</b><br />"
					 "Enabling this will hide the button (or the last button of a multi-button combo) "
					 "from other applications. Note that not all buttons can be suppressed."));

	return item;
}

void GlobalShortcutConfig::reload() {
	std::stable_sort(qlShortcuts.begin(), qlShortcuts.end());
	qtwShortcuts->clear();

	foreach (const Shortcut &sc, qlShortcuts) {
		QTreeWidgetItem *item = itemForShortcut(sc);
		qtwShortcuts->addTopLevelItem(item);
		on_qtwShortcuts_itemChanged(item, 0);
	}
#ifdef Q_OS_MAC
	if (!Global::get().s.bSuppressMacEventTapWarning) {
		qwWarningContainer->setVisible(showWarning());
	} else {
		qwWarningContainer->setVisible(false);
	}
#endif
}

void GlobalShortcutConfig::accept() const {
	GlobalShortcutEngine::engine->bNeedRemap = true;
	GlobalShortcutEngine::engine->needRemap();
	GlobalShortcutEngine::engine->setEnabled(Global::get().s.bShortcutEnable);
}


GlobalShortcutEngine::GlobalShortcutEngine(QObject *p) : QThread(p) {
	bNeedRemap = true;
	needRemap();
}

GlobalShortcutEngine::~GlobalShortcutEngine() {
	QSet< ShortcutKey * > qs;
	foreach (const QList< ShortcutKey * > &ql, qlShortcutList) { qs += QSet< ShortcutKey * >(ql.begin(), ql.end()); }

	foreach (ShortcutKey *sk, qs)
		delete sk;
}

void GlobalShortcutEngine::remap() {
	bNeedRemap = false;

	QSet< ShortcutKey * > qs;
	foreach (const QList< ShortcutKey * > &ql, qlShortcutList) { qs += QSet< ShortcutKey * >(ql.begin(), ql.end()); }

	foreach (ShortcutKey *sk, qs)
		delete sk;

	qlButtonList.clear();
	qlShortcutList.clear();
	qlDownButtons.clear();

	foreach (const Shortcut &sc, Global::get().s.qlShortcuts) {
		GlobalShortcut *gs = qmShortcuts.value(sc.iIndex);
		if (gs && !sc.qlButtons.isEmpty()) {
			ShortcutKey *sk = new ShortcutKey;
			sk->s           = sc;
			sk->iNumUp      = sc.qlButtons.count();
			sk->gs          = gs;

			foreach (const QVariant &button, sc.qlButtons) {
				auto idx = qlButtonList.indexOf(button);
				if (idx == -1) {
					qlButtonList << button;
					qlShortcutList << QList< ShortcutKey * >();
					idx = qlButtonList.count() - 1;
				}
				qlShortcutList[idx] << sk;
			}
		}
	}
}

void GlobalShortcutEngine::run() {
}

bool GlobalShortcutEngine::canSuppress() {
	return false;
}

void GlobalShortcutEngine::setEnabled(bool) {
}

bool GlobalShortcutEngine::enabled() {
	return true;
}

bool GlobalShortcutEngine::canDisable() {
	return false;
}

void GlobalShortcutEngine::resetMap() {
	tReset.restart();
	qlActiveButtons.clear();
}

void GlobalShortcutEngine::needRemap() {
}

/**
 * This function gets called internally to update the state
 * of a button.
 *
 * @return True if button is suppressed, otherwise false
 */
bool GlobalShortcutEngine::handleButton(const QVariant &button, bool down) {
	bool already = qlDownButtons.contains(button);
	if (already == down)
		return qlSuppressed.contains(button);
	if (down)
		qlDownButtons << button;
	else
		qlDownButtons.removeAll(button);

	if (tReset.elapsed() > 100000) {
		if (down) {
			qlActiveButtons.removeAll(button);
			qlActiveButtons << button;
		}
		emit buttonPressed(!down);
	}

	if (down) {
		AudioInputPtr ai = Global::get().ai;
		if (ai.get()) {
			// XXX: This is a data race: we write to ai->activityState
			// (accessed by the AudioInput thread) from the main thread.
			if (ai->activityState == AudioInput::ActivityStateIdle) {
				ai->activityState = AudioInput::ActivityStateReturnedFromIdle;
			}
			ai->tIdle.restart();
		}
	}

	const auto idx = qlButtonList.indexOf(button);
	if (idx == -1)
		return false;

	bool suppress = false;

	foreach (ShortcutKey *sk, qlShortcutList.at(idx)) {
		if (down) {
			sk->iNumUp--;
			if (sk->iNumUp == 0) {
				GlobalShortcut *gs = sk->gs;
				if (sk->s.bSuppress) {
					suppress = true;
					qlSuppressed << button;
				}
				if (!gs->qlActive.contains(sk->s.qvData)) {
					gs->qlActive << sk->s.qvData;
					emit gs->triggered(true, sk->s.qvData);
					emit gs->down(sk->s.qvData);
				}
			} else if (sk->iNumUp < 0) {
				sk->iNumUp = 0;
			}
		} else {
			if (qlSuppressed.contains(button)) {
				suppress = true;
				qlSuppressed.removeAll(button);
			}
			sk->iNumUp++;
			if (sk->iNumUp == 1) {
				GlobalShortcut *gs = sk->gs;
				if (gs->qlActive.contains(sk->s.qvData)) {
					gs->qlActive.removeAll(sk->s.qvData);
					emit gs->triggered(false, sk->s.qvData);
				}
			} else if (sk->iNumUp > sk->s.qlButtons.count()) {
				sk->iNumUp = sk->s.qlButtons.count();
			}
		}
	}
	return suppress;
}

void GlobalShortcutEngine::add(GlobalShortcut *gs) {
	if (!GlobalShortcutEngine::engine) {
		GlobalShortcutEngine::engine = GlobalShortcutEngine::platformInit();
		GlobalShortcutEngine::engine->setEnabled(Global::get().s.bShortcutEnable);
	}

	GlobalShortcutEngine::engine->qmShortcuts.insert(gs->idx, gs);
	GlobalShortcutEngine::engine->bNeedRemap = true;
	GlobalShortcutEngine::engine->needRemap();
}

void GlobalShortcutEngine::remove(GlobalShortcut *gs) {
	engine->qmShortcuts.remove(gs->idx);
	engine->bNeedRemap = true;
	engine->needRemap();
	if (engine->qmShortcuts.isEmpty()) {
		delete engine;
		GlobalShortcutEngine::engine = nullptr;
	}
}

GlobalShortcut::GlobalShortcut(QObject *p, int index, QString qsName, QVariant def) : QObject(p) {
	idx       = index;
	name      = qsName;
	qvDefault = def;
	GlobalShortcutEngine::add(this);
}

GlobalShortcut::~GlobalShortcut() {
	GlobalShortcutEngine::remove(this);
}
