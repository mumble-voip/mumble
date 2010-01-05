/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GlobalShortcut.h"
#include "MainWindow.h"
#include "Global.h"

GlobalShortcutEngine *GlobalShortcutEngine::engine = NULL;

static ConfigWidget *GlobalShortcutConfigDialogNew(Settings &st) {
	return new GlobalShortcutConfig(st);
}

static ConfigRegistrar registrar(1200, GlobalShortcutConfigDialogNew);

ShortcutKeyWidget::ShortcutKeyWidget(QWidget *p) : QLineEdit(p) {
	setReadOnly(true);
	clearFocus();
	bModified = false;
	displayKeys();
}

QList<QVariant> ShortcutKeyWidget::getShortcut() const {
	return qlButtons;
}

void ShortcutKeyWidget::setShortcut(const QList<QVariant> &buttons) {
	qlButtons = buttons;
	displayKeys();
}

void ShortcutKeyWidget::focusInEvent(QFocusEvent *) {
	setText(tr("Press Shortcut"));

	QPalette pal=parentWidget()->palette();
	pal.setColor(QPalette::Base, pal.color(QPalette::Base).dark(120));
	setPalette(pal);

	setForegroundRole(QPalette::Button);
	GlobalShortcutEngine::engine->resetMap();
	connect(GlobalShortcutEngine::engine, SIGNAL(buttonPressed(bool)), this, SLOT(updateKeys(bool)));
	installEventFilter(this);
}

void ShortcutKeyWidget::focusOutEvent(QFocusEvent *e) {
	if ((e->reason() == Qt::TabFocusReason) || (e->reason() == Qt::BacktabFocusReason))
		return;

	setPalette(parentWidget()->palette());
	clearFocus();
	disconnect(GlobalShortcutEngine::engine, SIGNAL(buttonPressed(bool)), this, SLOT(updateKeys(bool)));
	displayKeys();
	removeEventFilter(this);
}

bool ShortcutKeyWidget::eventFilter(QObject *, QEvent *evt) {
	if ((evt->type() == QEvent::KeyPress) || (evt->type() == QEvent::MouseButtonPress))
		return true;
	return false;
}

void ShortcutKeyWidget::mouseDoubleClickEvent(QMouseEvent *) {
	bModified = true;
	qlButtons.clear();
	clearFocus();
	displayKeys();
}

void ShortcutKeyWidget::updateKeys(bool last) {
	qlButtons = GlobalShortcutEngine::engine->qlActiveButtons;
	bModified = true;

	if (qlButtons.isEmpty())
		return;

	if (last)
		clearFocus();
	else
		displayKeys();
}

void ShortcutKeyWidget::displayKeys() {
	QStringList keys;

	foreach(QVariant button, qlButtons) {
		QString id = GlobalShortcutEngine::engine->buttonName(button);
		if (! id.isEmpty())
			keys << id;
	}
	setText(keys.join(QLatin1String(" + ")));
	emit keySet(keys.count() > 0);
}

ShortcutActionWidget::ShortcutActionWidget(QWidget *p) : QComboBox(p) {
	int idx = 0;

	insertItem(idx, tr("Unassigned"));
	setItemData(idx, -1);

	idx++;

	bool expert = true;
	while (p) {
		GlobalShortcutConfig *gsc = qobject_cast<GlobalShortcutConfig *>(p);
		if (gsc) {
			expert = gsc->bExpert;
			break;
		}
		p = p->parentWidget();
	}
	foreach(GlobalShortcut *gs, GlobalShortcutEngine::engine->qmShortcuts) {
		if (expert || ! gs->bExpert) {
			insertItem(idx, gs->name);
			setItemData(idx, gs->idx);
			if (! gs->qsToolTip.isEmpty())
				setItemData(idx, gs->qsToolTip, Qt::ToolTipRole);
			if (! gs->qsWhatsThis.isEmpty())
				setItemData(idx, gs->qsWhatsThis, Qt::WhatsThisRole);
			idx++;
		}
	}
}

void ShortcutActionWidget::setIndex(int idx) {
	setCurrentIndex(findData(idx));
}

int ShortcutActionWidget::index() const {
	return itemData(currentIndex()).toInt();
}

ShortcutDelegate::ShortcutDelegate(QObject *p) : QStyledItemDelegate(p) {
	QItemEditorFactory *factory = new QItemEditorFactory;
	QItemEditorCreatorBase *shortcutCreator = new QStandardItemEditorCreator<ShortcutKeyWidget>();
	QItemEditorCreatorBase *indexCreator = new QStandardItemEditorCreator<ShortcutActionWidget>();

	factory->registerEditor(QVariant::List, shortcutCreator);
	factory->registerEditor(QVariant::Int, indexCreator);
	setItemEditorFactory(factory);
}

ShortcutDelegate::~ShortcutDelegate() {
	delete itemEditorFactory();
	setItemEditorFactory(NULL);
}

QString ShortcutDelegate::displayText(const QVariant &item, const QLocale &loc) const {
	if (item.type() == QVariant::List)
		return GlobalShortcutEngine::buttonText(item.toList());
	else if (item.type() == QVariant::Int) {
		GlobalShortcut *gs = GlobalShortcutEngine::engine->qmShortcuts.value(item.toInt());
		if (gs)
			return gs->name;
		else
			return tr("Unassigned");
	}

	return QStyledItemDelegate::displayText(item,loc);
}

GlobalShortcutConfig::GlobalShortcutConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	bool canSuppress = GlobalShortcutEngine::engine->canSuppress();

	qtwShortcuts->setColumnCount(canSuppress ? 3 : 2);
	qtwShortcuts->setItemDelegate(new ShortcutDelegate(qtwShortcuts));

	qtwShortcuts->header()->setResizeMode(QHeaderView::ResizeToContents);
}

void GlobalShortcutConfig::on_qpbAdd_clicked(bool) {
	Shortcut sc;
	sc.iIndex = -1;
	sc.bSuppress = false;
	qlShortcuts << sc;
	reload();
}

void GlobalShortcutConfig::on_qpbRemove_clicked(bool) {
	QTreeWidgetItem *qtwi = qtwShortcuts->currentItem();
	if (! qtwi)
		return;
	qlShortcuts.removeAt(qtwShortcuts->indexOfTopLevelItem(qtwi));
	delete qtwi;
}

void GlobalShortcutConfig::on_qtwShortcuts_currentItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *) {
	qpbRemove->setEnabled(item ? true : false);
}

void GlobalShortcutConfig::on_qtwShortcuts_itemChanged(QTreeWidgetItem *item, int) {
	int idx = qtwShortcuts->indexOfTopLevelItem(item);
	Shortcut &sc = qlShortcuts[idx];
	sc.iIndex = item->data(0, Qt::DisplayRole).toInt();
	sc.qlButtons = item->data(1, Qt::DisplayRole).toList();
	sc.bSuppress = item->checkState(2) == Qt::Checked;
}

QString GlobalShortcutConfig::title() const {
	return tr("Shortcuts");
}

QIcon GlobalShortcutConfig::icon() const {
	return QIcon(QLatin1String("skin:config_shortcuts.png"));
}

void GlobalShortcutConfig::load(const Settings &r) {
	qlShortcuts = r.qlShortcuts;
	bExpert = r.bExpert;
	reload();
}

void GlobalShortcutConfig::save() const {
	s.qlShortcuts = qlShortcuts;
}

QTreeWidgetItem *GlobalShortcutConfig::itemForShortcut(const Shortcut &sc) const {
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setData(0, Qt::DisplayRole, sc.iIndex);
	item->setData(1, Qt::DisplayRole, sc.qlButtons);
	item->setCheckState(2, sc.bSuppress ? Qt::Checked : Qt::Unchecked);
	item->setFlags(item->flags() | Qt::ItemIsEditable);

	::GlobalShortcut *gs = GlobalShortcutEngine::engine->qmShortcuts.value(sc.iIndex);

	if (gs) {
		if (! gs->qsToolTip.isEmpty())
			item->setData(0, Qt::ToolTipRole, gs->qsToolTip);
		if (! gs->qsWhatsThis.isEmpty())
			item->setData(0, Qt::WhatsThisRole, gs->qsWhatsThis);
	}

	item->setData(1, Qt::ToolTipRole, tr("Shortcut button combination."));
	item->setData(1, Qt::WhatsThisRole, tr("<b>This is the global shortcut key combination.</b><br />"
	                                       "Double-click this field and then the desired key/button combo "
	                                       "to rebind."));

	item->setData(2, Qt::ToolTipRole, tr("Suppress keys from other applications"));
	item->setData(2, Qt::WhatsThisRole, tr("<b>This hides the button presses from other applications.</b><br />"
	                                       "Enabling this will hide the button (or the last button of a multi-button combo) "
	                                       "from other applications. Note that not all buttons can be suppressed."));

	return item;
}

void GlobalShortcutConfig::reload() {
	qStableSort(qlShortcuts);
	qtwShortcuts->clear();
	foreach(const Shortcut &sc, qlShortcuts) {
		QTreeWidgetItem *item = itemForShortcut(sc);
		::GlobalShortcut *gs = GlobalShortcutEngine::engine->qmShortcuts.value(sc.iIndex);
		qtwShortcuts->addTopLevelItem(item);
		if (gs && gs->bExpert && ! bExpert)
			item->setHidden(true);
	}
}

void GlobalShortcutConfig::accept() const {
	GlobalShortcutEngine::engine->bNeedRemap = true;
	GlobalShortcutEngine::engine->needRemap();
}

bool GlobalShortcutConfig::expert(bool exp) {
	bExpert = exp;
	reload();
	return true;
}


GlobalShortcutEngine::GlobalShortcutEngine(QObject *p) : QThread(p) {
	bNeedRemap = true;
	needRemap();
}

GlobalShortcutEngine::~GlobalShortcutEngine() {
	QSet<ShortcutKey *> qs;
	foreach(const QList<ShortcutKey*> &ql, qlShortcutList)
		qs += ql.toSet();

	foreach(ShortcutKey *sk, qs)
		delete sk;
}

void GlobalShortcutEngine::remap() {
	bNeedRemap = false;

	QSet<ShortcutKey *> qs;
	foreach(const QList<ShortcutKey*> &ql, qlShortcutList)
		qs += ql.toSet();

	foreach(ShortcutKey *sk, qs)
		delete sk;

	qlButtonList.clear();
	qlShortcutList.clear();
	qlDownButtons.clear();

	foreach(const Shortcut &sc, g.s.qlShortcuts) {
		GlobalShortcut *gs = qmShortcuts.value(sc.iIndex);
		if (gs && ! sc.qlButtons.isEmpty()) {
			ShortcutKey *sk = new ShortcutKey;
			sk->s = sc;
			sk->iNumUp = sc.qlButtons.count();
			sk->gs = gs;

			foreach(const QVariant &button, sc.qlButtons) {
				int idx = qlButtonList.indexOf(button);
				if (idx == -1) {
					qlButtonList << button;
					qlShortcutList << QList<ShortcutKey *>();
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

void GlobalShortcutEngine::resetMap() {
	tReset.restart();
	qlActiveButtons.clear();
}

void GlobalShortcutEngine::needRemap() {
}

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
		emit buttonPressed(! down);
	}

	int idx = qlButtonList.indexOf(button);
	if (idx == -1)
		return false;

	bool suppress = false;

	foreach(ShortcutKey *sk, qlShortcutList.at(idx)) {
		if (down) {
			sk->iNumUp--;
			if (sk->iNumUp == 0) {
				GlobalShortcut *gs = sk->gs;
				if (sk->s.bSuppress) {
					suppress = true;
					qlSuppressed << button;
				}
				if (! gs->bActive) {
					gs->bActive = true;
					emit gs->triggered(sk->gs->bActive);
					emit gs->down();
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
				if (gs->bActive) {
					gs->bActive = false;
					emit gs->triggered(sk->gs->bActive);
					emit gs->up();
				}
			} else if (sk->iNumUp > sk->s.qlButtons.count()) {
				sk->iNumUp = sk->s.qlButtons.count();
			}
		}
	}
	return suppress;
}

void GlobalShortcutEngine::add(GlobalShortcut *gs) {
	if (! GlobalShortcutEngine::engine)
		GlobalShortcutEngine::engine = GlobalShortcutEngine::platformInit();

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
		GlobalShortcutEngine::engine = NULL;
	}
}

QString GlobalShortcutEngine::buttonText(const QList<QVariant> &list) {
	QStringList keys;

	foreach(QVariant button, list) {
		QString id = GlobalShortcutEngine::engine->buttonName(button);
		if (! id.isEmpty())
			keys << id;
	}
	return keys.join(QLatin1String(" + "));
}

GlobalShortcut::GlobalShortcut(QObject *p, int index, QString qsName, bool expert) : QObject(p) {
	idx = index;
	name=qsName;
	bExpert = expert;
	bActive = false;
	GlobalShortcutEngine::add(this);
}

GlobalShortcut::~GlobalShortcut() {
	GlobalShortcutEngine::remove(this);
}
