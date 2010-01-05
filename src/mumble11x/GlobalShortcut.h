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


#ifndef _GLOBALSHORTCUT_H
#define _GLOBALSHORTCUT_H

#include "mumble_pch.hpp"
#include "ConfigDialog.h"
#include "Timer.h"
#include "ui_GlobalShortcut.h"

class GlobalShortcut : public QObject {
		friend class GlobalShortcutEngine;
		friend class GlobalShortcutConfig;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcut)
	protected:
		bool bActive;
	signals:
		void down();
		void up();
		void triggered(bool);
	public:
		QString qsToolTip;
		QString qsWhatsThis;
		QString name;
		QVariant data;
		bool bExpert;
		int idx;

		GlobalShortcut(QObject *parent, int index, QString qsName, bool expert = true);
		~GlobalShortcut();

		bool active() const {
			return bActive;
		};
};

class ShortcutKeyWidget : public QLineEdit {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutKeyWidget)
		Q_PROPERTY(QList<QVariant> shortcut READ getShortcut WRITE setShortcut USER true)
	protected:
		virtual void focusInEvent(QFocusEvent *event);
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void mouseDoubleClickEvent(QMouseEvent *e);
		virtual bool eventFilter(QObject *, QEvent *);
	public:
		QList<QVariant> qlButtons;
		bool bModified;
		ShortcutKeyWidget(QWidget *p = NULL);
		QList<QVariant> getShortcut() const;
	public slots:
		void updateKeys(bool last);
		void displayKeys();
		void setShortcut(const QList<QVariant> &shortcut);
	signals:
		void keySet(bool);
};

class ShortcutActionWidget : public QComboBox {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutActionWidget)
		Q_PROPERTY(int index READ index WRITE setIndex USER true)
	public:
		ShortcutActionWidget(QWidget *p = NULL);
		int index() const;
		void setIndex(int);
};

class ShortcutDelegate : public QStyledItemDelegate {
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutDelegate)
	public:
		ShortcutDelegate(QObject *);
		~ShortcutDelegate();
		QString displayText(const QVariant &, const QLocale &) const;
};

class GlobalShortcutConfig : public ConfigWidget, public Ui::GlobalShortcut {
		friend class ShortcutActionWidget;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutConfig)
	protected:
		QList<Shortcut> qlShortcuts;
		QTreeWidgetItem *itemForShortcut(const Shortcut &) const;
		bool bExpert;
	public:
		GlobalShortcutConfig(Settings &st);
		virtual QString title() const;
		virtual QIcon icon() const;
	public slots:
		void accept() const;
		void save() const;
		void load(const Settings &r);
		void reload();
		bool expert(bool);
		void on_qpbAdd_clicked(bool);
		void on_qpbRemove_clicked(bool);
		void on_qtwShortcuts_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
		void on_qtwShortcuts_itemChanged(QTreeWidgetItem *, int);
};

struct ShortcutKey {
	Shortcut s;
	int iNumUp;
	GlobalShortcut *gs;
};

class GlobalShortcutEngine : public QThread {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutEngine)
	public:
		bool bNeedRemap;
		Timer tReset;

		static GlobalShortcutEngine *engine;
		static GlobalShortcutEngine *platformInit();

		QHash<int, GlobalShortcut *> qmShortcuts;
		QList<QVariant> qlActiveButtons;
		QList<QVariant> qlDownButtons;
		QList<QVariant> qlSuppressed;

		QList<QVariant> qlButtonList;
		QList<QList<ShortcutKey *> > qlShortcutList;

		GlobalShortcutEngine(QObject *p = NULL);
		~GlobalShortcutEngine();
		void resetMap();
		void remap();
		virtual void needRemap();
		void run();

		bool handleButton(const QVariant &, bool);
		static void add(GlobalShortcut *);
		static void remove(GlobalShortcut *);
		static QString buttonText(const QList<QVariant> &);
		virtual QString buttonName(const QVariant &) = 0;
		virtual bool canSuppress();
	signals:
		void buttonPressed(bool last);
};


#else
class GlobalShortcut;
#endif
