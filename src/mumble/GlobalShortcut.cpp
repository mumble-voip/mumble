/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

static ConfigRegistrar registrar(12, GlobalShortcutConfigDialogNew);

ShortcutKeyWidget::ShortcutKeyWidget(QWidget *p) : QLineEdit(p) {
	setReadOnly(true);
	clearFocus();
	bModified = false;
	displayKeys();
}

void ShortcutKeyWidget::setShortcut(const QList<QVariant> &buttons) {
	qlButtons = buttons;
	displayKeys();
}

void ShortcutKeyWidget::focusInEvent(QFocusEvent *e) {
	if (e->reason() == Qt::MouseFocusReason) {
		setText(tr("Press Shortcut"));

		QPalette pal=parentWidget()->palette();
		pal.setColor(QPalette::Base, pal.color(QPalette::Base).dark(120));
		setPalette(pal);

		setForegroundRole(QPalette::Button);
		GlobalShortcutEngine::engine->resetMap();
		connect(GlobalShortcutEngine::engine, SIGNAL(buttonPressed(bool)), this, SLOT(updateKeys(bool)));
	}
}

void ShortcutKeyWidget::focusOutEvent(QFocusEvent *) {
	setPalette(parentWidget()->palette());
	clearFocus();
	disconnect(GlobalShortcutEngine::engine, SIGNAL(buttonPressed(bool)), this, SLOT(updateKeys(bool)));
	displayKeys();
}

void ShortcutKeyWidget::mouseDoubleClickEvent(QMouseEvent *) {
	bModified = true;
	qlButtons.clear();
	clearFocus();
	displayKeys();
}

void ShortcutKeyWidget::updateKeys(bool last) {
	qlButtons = GlobalShortcutEngine::engine->getCurrentButtons();
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
}

GlobalShortcutConfig::GlobalShortcutConfig(Settings &st) : ConfigWidget(st) {
	QGroupBox *qgbShortcuts = new QGroupBox(tr("Shortcuts"));
	QLabel *lab;

	QGridLayout *l=new QGridLayout();

	lab=new QLabel(tr("Function"));
	l->addWidget(lab, 0, 0);
	lab=new QLabel(tr("Shortcut"));
	l->addWidget(lab, 0, 1);

	int i=0;

	foreach(GlobalShortcut *gs, GlobalShortcutEngine::engine->qmShortcuts) {
		ShortcutKeyWidget *skw=new ShortcutKeyWidget();

		lab=new QLabel(gs->name);
		l->addWidget(lab, i+1, 0);
		l->addWidget(skw, i+1, 1);

		skw->setToolTip(tr("Shortcut bound to %1.").arg(gs->name));
		skw->setWhatsThis(tr("<b>This is the global shortcut bound to %1</b><br />"
		                      "Click this field and then the desired key/button combo "
		                      "to rebind. Double-click to clear.").arg(gs->name));
		qhKeys[gs]=skw;
		i++;
	}

	lab = new QLabel(tr("Double-click an entry to clear the shortcut."));
	l->addWidget(lab,i+1,0,1,2);

	qgbShortcuts->setLayout(l);

	QVBoxLayout *v = new QVBoxLayout;
	v->addWidget(qgbShortcuts);
	v->addStretch(1);
	setLayout(v);

	QMetaObject::connectSlotsByName(this);
}

QString GlobalShortcutConfig::title() const {
	return tr("Shortcuts");
}

QIcon GlobalShortcutConfig::icon() const {
	return QIcon(QLatin1String("skin:config_shortcuts.png"));
}

void GlobalShortcutConfig::load(const Settings &r) {
	foreach(GlobalShortcut *gs, GlobalShortcutEngine::engine->qmShortcuts) {
		ShortcutKeyWidget *dikw = qhKeys.value(gs);
		dikw->setShortcut(r.qmShortcuts.value(gs->idx));
	}
}

void GlobalShortcutConfig::save() const {
	Settings::ShortcutMap m;

	foreach(GlobalShortcut *gs, GlobalShortcutEngine::engine->qmShortcuts) {
		ShortcutKeyWidget *dikw = qhKeys[gs];
		m.insert(gs->idx, dikw->qlButtons);
	}
	s.qmShortcuts = m;
}

void GlobalShortcutConfig::accept() const {
	GlobalShortcutEngine::engine->remap();
}

bool GlobalShortcutConfig::expert(bool) {
	return true;
}


GlobalShortcutEngine::GlobalShortcutEngine(QObject *p) : QThread(p) {
	bNeedRemap = true;
}

void GlobalShortcutEngine::remap() {
	bNeedRemap = true;
}

void GlobalShortcutEngine::run() {
}
