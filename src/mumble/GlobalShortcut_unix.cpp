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

#include "GlobalShortcut_unix.h"
#include "Global.h"
#include <QX11Info>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xevie.h>

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
                qlButtons = s->qlButtons;
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
		gsx->resetMap();
		connect(gsx, SIGNAL(buttonPressed(bool)), this, SLOT(setButton(bool)));
        }
}

void XInputKeyWidget::focusOutEvent(QFocusEvent *) {
        setPalette(parentWidget()->palette());
        clearFocus();
	disconnect(gsx, SIGNAL(buttonPressed(bool)), this, SLOT(setButton(bool)));
        displayKeys();
}

void XInputKeyWidget::mouseDoubleClickEvent (QMouseEvent *) {
  bModified = true;
  qlButtons.clear();
  clearFocus();
  displayKeys();
}

void XInputKeyWidget::setButton(bool last) {
  qlButtons = gsx->getCurrentButtons();
  bModified = true;
  if (last)
    clearFocus();
  else
    displayKeys();
}

void XInputKeyWidget::displayKeys() {
  QStringList sl;
  foreach(int key, qlButtons) {
    KeySym ks=XKeycodeToKeysym(QX11Info::display(), key, 0);
    if (ks == NoSymbol) {
      sl << QLatin1String("0x")+QString::number(key,16);
    } else {
      const char *str = XKeysymToString(ks);
      if (strlen(str) == 0) {
	sl << QLatin1String("KS0x")+QString::number(ks,16);
      } else {
	sl << QLatin1String(str);
      }
    }
  }
  setText(sl.join(QLatin1String(" ")));
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
                                                        "to rebind. Double-click to clear.").arg(gs->name));
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
        return QIcon(QLatin1String("skin:config_shortcuts.png"));
}

void GlobalShortcutXConfig::accept() {
  foreach(GlobalShortcut *gs, gsx->qmShortcuts) {
    XInputKeyWidget *dikw = qhKeys[gs];
    if (dikw->bModified) {
      QString base=QString::fromLatin1("GS%1_").arg(gs->idx);
      g.qs->setValue(base + QLatin1String("num"), dikw->qlButtons.count());
      int i=0;
      foreach(int bt, dikw->qlButtons) {
	g.qs->setValue(base + QString::fromLatin1("%1_Key").arg(i), bt);
	i++;
      }
    }
  }
  gsx->bNeedRemap = true;
}

GlobalShortcutX::GlobalShortcutX() {
  gsx = this;
  ref = 0;
  bGrabbing = false;
  bNeedRemap = true;
  int min, maj;
  bRunning=false;
  display = XOpenDisplay(NULL);

  if (! display) {
    qWarning("GlobalShortcutX: Unable to open dedicated display connection.");
    return;
  }

  maj = 1;
  min = 1;

  if (! XevieQueryVersion(display, &maj, &min)) {
    qWarning("GlobalShortcutX: XEVIE extension not found. Enable it in xorg.conf");
    return;
  }

  qWarning("GlobalShortcutX: XEVIE %d.%d", maj, min);

  // Here's the thing. If we're debugging, it doesn't do to have the xevie application hang.

#ifdef QT_NO_DEBUG
  if (! XevieStart(display)) {
    qWarning("GlobalShortcutX: Another client is already using XEVIE");
    return;
  }

  XevieSelectInput(display, KeyPressMask | KeyReleaseMask);
#endif

  bRunning=true;
  start(QThread::TimeCriticalPriority);
}

void GlobalShortcutX::add(GlobalShortcut *gs) {
  qmShortcuts[gs->idx] = gs;
  bNeedRemap=true;
}

void GlobalShortcutX::remove(GlobalShortcut *gs) {
  qmShortcuts.remove(gs->idx);
  bNeedRemap=true;
}

void GlobalShortcutX::grab() {
  bGrabbing = true;
}

void GlobalShortcutX::release() {
  bGrabbing = false;
}

GlobalShortcutX::~GlobalShortcutX() {
  bRunning = false;
  wait();
}

void GlobalShortcutX::remap() {
  bNeedRemap = false;

  foreach(Shortcut *s, qhGlobalToX) {
    delete s;
  }
  qhGlobalToX.clear();
  qmhKeyToShortcut.clear();

  foreach(GlobalShortcut *gs, qmShortcuts) {

    QString base=QString::fromLatin1("GS%1_").arg(gs->idx);
    QList<unsigned int> buttons;
    int nbuttons = g.qs->value(base + QLatin1String("num"), 0).toInt();
    for(int i=0;i<nbuttons;i++) {
      unsigned int key = g.qs->value(base + QString::fromLatin1("%1_Key").arg(i), 0xffffffff).toUInt();
      if (key != 0xffffffff) {
	buttons << key;
      }
    }
    if (buttons.count() > 0) {
      Shortcut *s = new Shortcut();
      s->gs = gs;
      s->bActive = false;
      s->iNumDown = 0;
      for(int i=0;i<buttons.count();i++) {
	s->qlButtons << buttons[i];
	qmhKeyToShortcut.insert(buttons[i], s);
      }
      qhGlobalToX[gs] = s;
    }
  }
}

void GlobalShortcutX::resetMap() {
  for(int i=0;i<256;i++)
    touchMap[i]=false;
}

QList<int> GlobalShortcutX::getCurrentButtons() {
  QList<int> keys;
  for(int i=0;i<256;i++)
    if (touchMap[i])
      keys << i;
  return keys;
}

void GlobalShortcutX::run() {
  fd_set in_fds;
  XEvent evt;
  struct timeval tv;
  while(bRunning) {
    if (bNeedRemap)
      remap();
    FD_ZERO(&in_fds);
    FD_SET(ConnectionNumber(display), &in_fds);
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    if(select(ConnectionNumber(display)+1, &in_fds, NULL, NULL, &tv)) {
      while (XPending(display)) {
	XNextEvent(display, &evt);
	XevieSendEvent(display, &evt, XEVIE_UNMODIFIED);
	switch(evt.type) {
	case KeyPress:
	case KeyRelease:
	  {
	    bool down = (evt.type == KeyPress);

	    if (down == activeMap[evt.xkey.keycode])
	      break;
       	    activeMap[evt.xkey.keycode] = down;
	    if (down)
	      touchMap[evt.xkey.keycode] = true;
	    emit buttonPressed(!down);
	    foreach(Shortcut *s, qmhKeyToShortcut.values(evt.xkey.keycode)) {
	      if (down) {
		s->iNumDown++;
		if (s->iNumDown == s->qlButtons.count()) {
		  s->bActive = true;
		  emit s->gs->triggered(down);
		  emit s->gs->down();
		}
	      } else {
		s->iNumDown--;
		if (s->bActive) {
		  s->bActive = false;
		  emit s->gs->triggered(down);
		  emit s->gs->up();
		}
	      }
	    }
	  }

	  break;
	default:
	  qWarning("GlobalShortcutX: EVT %x", evt.type);
	}
      }
    }
  }
  XevieEnd(display);
  XCloseDisplay(display);
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
