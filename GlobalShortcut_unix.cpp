/* Copyright (C) 2005-2006, Thorvald Natvig <thorvald@natvig.com>

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

#include "GlobalShortcut_unix.h"
#include "Global.h"
#include <QX11Info>
#include <X11/X.h>
#include <X11/Xlib.h>

static GlobalShortcutX *gsx = NULL;

static ConfigWidget *GlobalShortcutXConfigDialogNew() {
        return new GlobalShortcutXConfig();
}

static ConfigRegistrar registrar(55, GlobalShortcutXConfigDialogNew);

XInputKeyWidget::XInputKeyWidget(QWidget *p) : QLineEdit(p) {
        setReadOnly(true);
        clearFocus();
        bModified = false;
        displayKeys();
}

void XInputKeyWidget::setShortcut(GlobalShortcut *gs) {
        if (gsx->qhGlobalToX.contains(gs)) {
                Shortcut *s = gsx->qhGlobalToX[gs];
                button = s->button;
                displayKeys();
        }
}

void XInputKeyWidget::focusInEvent(QFocusEvent *e) {
        if (e->reason() == Qt::MouseFocusReason) {
                setText(tr("Press Shortcut"));

                QPalette pal=parentWidget()->palette();
                pal.setColor(QPalette::Base, pal.color(QPalette::Base).dark(120));
                setPalette(pal);

                setForegroundRole(QPalette::Button);
		connect(gsx, SIGNAL(keyPress(qpButton)), this, SLOT(setKey(qpButton)));
		connect(gsx, SIGNAL(keyRelease(qpButton)), this, SLOT(doneKey(qpButton)));
       		grabKeyboard();
		gsx->grab();
        }
}

void XInputKeyWidget::focusOutEvent(QFocusEvent *) {
        setPalette(parentWidget()->palette());
        clearFocus();
	disconnect(gsx, SIGNAL(keyPress(qpButton)), this, SLOT(setKey(qpButton)));
	disconnect(gsx, SIGNAL(keyRelease(qpButton)), this, SLOT(doneKey(qpButton)));
	releaseKeyboard();
	gsx->release();
        displayKeys();
}

void XInputKeyWidget::mouseDoubleClickEvent (QMouseEvent *) {
        bModified = true;
	button=qpButton(0,0);
        clearFocus();
        displayKeys();
}

void XInputKeyWidget::setKey(qpButton b) {
  button=b;
  bModified = true;
  displayKeys();
}

void XInputKeyWidget::doneKey(qpButton) {
  clearFocus();
}

void XInputKeyWidget::displayKeys() {
  unsigned int mods = 0;
  if (button.second & ShiftMask)
    mods |= Qt::SHIFT;
  if (button.second & ControlMask)
    mods |= Qt::CTRL;
  if (button.second & Mod1Mask)
    mods |= Qt::ALT;
  if (button.second & Mod5Mask)
    mods |= Qt::META;
  QKeySequence keys(button.first | mods);
  setText(keys.toString());
}

GlobalShortcutXConfig::GlobalShortcutXConfig(QWidget *p) : ConfigWidget(p) {
        QGroupBox *qgbShortcuts = new QGroupBox(tr("Shortcuts"));
        QLabel *lab;

        QGridLayout *l=new QGridLayout();

        lab=new QLabel(tr("Function"));
        l->addWidget(lab, 0, 0);
        lab=new QLabel(tr("Shortcut"));
        l->addWidget(lab, 0, 1);

        int i=0;

        foreach(GlobalShortcut *gs, gsx->qmShortcuts) {
                XInputKeyWidget *dikw=new XInputKeyWidget();
                dikw->setShortcut(gs);

                lab=new QLabel(gs->name);
                l->addWidget(lab, i+1, 0);
                l->addWidget(dikw, i+1, 1);

                dikw->setToolTip(tr("Shortcut bound to %1.").arg(gs->name));
                dikw->setWhatsThis(tr("<b>This is the global shortcut bound to %1</b><br />"
                                                        "Click this field and then the desired key combo "
                                                        "to rebind. Doubleclick to clear.").arg(gs->name));
                qhKeys[gs]=dikw;
                i++;
        }

        qgbShortcuts->setLayout(l);

    QVBoxLayout *v = new QVBoxLayout;
    v->addWidget(qgbShortcuts);
    v->addStretch(1);
    setLayout(v);

    QMetaObject::connectSlotsByName(this);
}

QString GlobalShortcutXConfig::title() const {
        return tr("Shortcuts");
}

QIcon GlobalShortcutXConfig::icon() const {
        return QIcon(":/icons/config_shortcuts.png");
}

void GlobalShortcutXConfig::accept() {
        foreach(GlobalShortcut *gs, gsx->qmShortcuts) {
                XInputKeyWidget *dikw = qhKeys[gs];
                if (dikw->bModified) {
                        QString base=QString("GS%1_").arg(gs->idx);
			g.qs->setValue(base + QString("sym"), dikw->button.first);
                        g.qs->setValue(base + QString("state"), dikw->button.second);
                }
        }

        gsx->remap();
}

GlobalShortcutX::GlobalShortcutX() {
  gsx = this;
  ref = 0;
  bGrabbing = false;
  
  remap();
}

void GlobalShortcutX::add(GlobalShortcut *gs) {
  qmShortcuts[gs->idx] = gs;
  remap();
}

void GlobalShortcutX::remove(GlobalShortcut *gs) {
  qmShortcuts.remove(gs->idx);
  remap();
}

void GlobalShortcutX::grab() {
  bGrabbing = true;
}

void GlobalShortcutX::release() {
  bGrabbing = false;
}

GlobalShortcutX::~GlobalShortcutX() {
}

void GlobalShortcutX::remap() {
  qWarning("remapping");
}

bool GlobalShortcutX::globalEvent(XEvent *evt) {
  if ((evt->type != KeyPress) && (evt->type != KeyRelease))
    return false;

  if (!QWidget::keyboardGrabber() && !QApplication::activePopupWidget()) {
    XUngrabKeyboard(QX11Info::display(),evt->xkey.time);
    XFlush(QX11Info::display());
  }

  KeySym keysym=XLookupKeysym(reinterpret_cast<XKeyEvent *>(evt), 0);
  qpButton button(keysym, evt->xkey.state);

  if (bGrabbing) {
    if (evt->type == KeyPress)
      emit keyPress(button);
    else
      emit keyRelease(button);
    return true;
  }

  qWarning("%x %x", keysym, evt->xkey.state);

  foreach(Shortcut *s, qhGlobalToX) {
    if (s->button == button) {
      bool act = (evt->type == KeyPress);
      emit s->gs->triggered(act);
      if (act)
	emit s->gs->down();
      else
	emit s->gs->up();
      return true;
    }
  }

  if (QApplication::focusWidget())
    return false;
  return true;
}

bool globalshortcutevent(XEvent *evt) {
  if (gsx)
    return gsx->globalEvent(evt);
  return false;
}

GlobalShortcut::GlobalShortcut(QObject *p, int index, QString qsName) : QObject(p) {
  if (! gsx)
    gsx = new GlobalShortcutX();
  gsx->ref++;
  idx = index;
  name=qsName;
  act = false;
  gsx->add(this);
}

GlobalShortcut::~GlobalShortcut()
{
  gsx->remove(this);
  gsx->ref--;
  if (gsx->ref == 0) {
    delete gsx;
    gsx = NULL;
  }
}
