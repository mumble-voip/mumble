/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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


#ifndef _GLOBALSHORTCUT_H
#define _GLOBALSHORTCUT_H

#include "mumble_pch.hpp"
#include "ConfigDialog.h"
#include "Timer.h"

class GlobalShortcut : public QObject {
		friend class GlobalShortcutEngine;
		friend class GlobalShortcutConfig;
		Q_OBJECT
		Q_PROPERTY(QVariant data READ data WRITE setData)
		Q_PROPERTY(bool active READ active)
	protected:
		QString name;
		QVariant dv;
		int idx;

		bool bActive;
		int iNumUp;
		QList<QVariant> qlButtons;
	signals:
		void down();
		void up();
		void triggered(bool);
	public:
		GlobalShortcut(QObject *parent, int index, QString qsName);
		~GlobalShortcut();

		QVariant data() const {
			return dv;
		};
		void setData(QVariant d) {
			dv = d;
		};
		bool active() const {
			return bActive;
		};
	private:
		Q_DISABLE_COPY(GlobalShortcut)
};

class ShortcutKeyWidget : public QLineEdit {
		Q_OBJECT
	protected:
		virtual void focusInEvent(QFocusEvent *event);
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void mouseDoubleClickEvent(QMouseEvent *e);
	public:
		QList<QVariant> qlButtons;
		bool bModified;
		ShortcutKeyWidget(QWidget *p = NULL);
		void setShortcut(const QList<QVariant> &buttons);
	public slots:
		void updateKeys(bool last);
		void displayKeys();
};

class GlobalShortcutConfig : public ConfigWidget {
		Q_OBJECT
	protected:
		QHash<GlobalShortcut *, ShortcutKeyWidget *> qhKeys;
	public:
		GlobalShortcutConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void accept() const;
		void save() const;
		void load(const Settings &r);
		bool expert(bool);
};

class GlobalShortcutEngine : public QThread {
		Q_OBJECT
	public:
		bool bNeedRemap;
		Timer tReset;

		static GlobalShortcutEngine *engine;
		static GlobalShortcutEngine *platformInit();

		QHash<int, GlobalShortcut *> qmShortcuts;
		QList<QVariant> qlActiveButtons;

		QList<QVariant> qlButtonList;
		QList<QList<GlobalShortcut *> > qlShortcutList;

		GlobalShortcutEngine(QObject *p = NULL);
		void resetMap();
		void remap();
		virtual void needRemap();
		void run();

		void handleButton(const QVariant &, bool);
		static void add(GlobalShortcut *);
		static void remove(GlobalShortcut *);
		virtual QString buttonName(const QVariant &) = 0;
	signals:
		void buttonPressed(bool last);
};


#else
class GlobalShortcut;
#endif
