/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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


#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_H_

#include <QtCore/QtGlobal>
#include <QtCore/QThread>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
# include <QtWidgets/QToolButton>
# include <QtWidgets/QStyledItemDelegate>
#else
# include <QtGui/QToolButton>
# include <QtGui/QStyledItemDelegate>
#endif

#include "ConfigDialog.h"
#include "Timer.h"

#include "ui_GlobalShortcut.h"
#include "ui_GlobalShortcutTarget.h"

class GlobalShortcut : public QObject {
		friend class GlobalShortcutEngine;
		friend class GlobalShortcutConfig;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcut)
	protected:
		QList<QVariant> qlActive;
	signals:
		void down(QVariant);
		void triggered(bool, QVariant);
	public:
		QString qsToolTip;
		QString qsWhatsThis;
		QString name;
		QVariant qvDefault;
		bool bExpert;
		int idx;

		GlobalShortcut(QObject *parent, int index, QString qsName, bool expert = true, QVariant def = QVariant());
		~GlobalShortcut();

		bool active() const {
			return ! qlActive.isEmpty();
		}
};

/**
 * Widget used to define and key combination for a shortcut. Once it gains
 * focus it will listen for a button combination until it looses focus.
 */
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
		void displayKeys(bool last = true);
	public slots:
		void updateKeys(bool last);
		void setShortcut(const QList<QVariant> &shortcut);
	signals:
		void keySet(bool, bool);
};

/**
 * Combo box widget used to define the kind of action a shortcut triggers. Then
 * entries get auto-generated from the GlobalShortcutEngine::qmShortcuts store.
 *
 * @see GlobalShortcutEngine
 */
class ShortcutActionWidget : public QComboBox {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutActionWidget)
		Q_PROPERTY(unsigned int index READ index WRITE setIndex USER true)
	public:
		ShortcutActionWidget(QWidget *p = NULL);
		unsigned int index() const;
		void setIndex(int);
};

class ShortcutToggleWidget : public QComboBox {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutToggleWidget)
		Q_PROPERTY(int index READ index WRITE setIndex USER true)
	public:
		ShortcutToggleWidget(QWidget *p = NULL);
		int index() const;
		void setIndex(int);
};

/**
 * Dialog which is used to select the targets of a targeted shortcut like Whisper.
 */
class ShortcutTargetDialog : public QDialog, public Ui::GlobalShortcutTarget {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutTargetDialog)
	protected:
		QMap<QString, QString> qmHashNames;
		ShortcutTarget stTarget;
	public:
		ShortcutTargetDialog(const ShortcutTarget &, QWidget *p = NULL);
		ShortcutTarget target() const;
	public slots:
		void accept();
		void on_qrbUsers_clicked();
		void on_qrbChannel_clicked();
		void on_qpbAdd_clicked();
		void on_qpbRemove_clicked();
};

enum ShortcutTargetTypes {
	SHORTCUT_TARGET_ROOT = -1,
	SHORTCUT_TARGET_PARENT = -2,
	SHORTCUT_TARGET_CURRENT = -3,
	SHORTCUT_TARGET_SUBCHANNEL = -4,
	SHORTCUT_TARGET_PARENT_SUBCHANNEL = -12
};

/**
 * Widget used to display and change a ShortcutTarget. The widget displays a textual representation
 * of a ShortcutTarget and enable its editing with a ShortCutTargetDialog.
 */
class ShortcutTargetWidget : public QFrame {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutTargetWidget)
		Q_PROPERTY(ShortcutTarget target READ target WRITE setTarget USER true)
	protected:
		ShortcutTarget stTarget;
		QLineEdit *qleTarget;
		QToolButton *qtbEdit;
	public:
		ShortcutTargetWidget(QWidget *p = NULL);
		ShortcutTarget target() const;
		void setTarget(const ShortcutTarget &);
		static QString targetString(const ShortcutTarget &);
	public slots:
		void on_qtbEdit_clicked();
};

/**
 * Used to get custom display and edit behaviour for the model used in GlobalShortcutConfig::qtwShortcuts.
 * It registers custom handlers which link specific types to custom ShortcutXWidget editors and also
 * provides a basic textual representation for them when they are not edited.
 *
 * @see GlobalShortcutConfig
 * @see ShortcutKeyWidget
 * @see ShortcutActionWidget
 * @see ShortcutTargetWidget
 */
class ShortcutDelegate : public QStyledItemDelegate {
		Q_OBJECT
		Q_DISABLE_COPY(ShortcutDelegate)
	public:
		ShortcutDelegate(QObject *);
		~ShortcutDelegate();
		QString displayText(const QVariant &, const QLocale &) const;
};

/**
 * Contains the Shortcut tab from the settings. This ConfigWidget provides
 * the user with the interface to add/edit/delete global shortcuts in Mumble.
 */
class GlobalShortcutConfig : public ConfigWidget, public Ui::GlobalShortcut {
		friend class ShortcutActionWidget;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(GlobalShortcutConfig)
	protected:
		QList<Shortcut> qlShortcuts;
		QTreeWidgetItem *itemForShortcut(const Shortcut &) const;
		bool bExpert;
		bool showWarning() const;
		bool eventFilter(QObject *, QEvent *);
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
		void commit();
		void on_qcbEnableGlobalShortcuts_stateChanged(int);
		void on_qpbAdd_clicked(bool);
		void on_qpbRemove_clicked(bool);
		void on_qtwShortcuts_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
		void on_qtwShortcuts_itemChanged(QTreeWidgetItem *, int);
		void on_qpbOpenAccessibilityPrefs_clicked();
		void on_qpbSkipWarning_clicked();
};

struct ShortcutKey {
	Shortcut s;
	int iNumUp;
	GlobalShortcut *gs;
};

/**
 * Creates a background thread which handles global shortcut behaviour. This class inherits
 * a system unspecific interface and basic functionality to the actually used native backend
 * classes (GlobalShortcutPlatform).
 *
 * @see GlobalShortcutX
 * @see GlobalShortcutMac
 * @see GlobalShortcutWin
 */
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

		virtual void setEnabled(bool b);
		virtual bool enabled();
		virtual bool canDisable();

		virtual void prepareInput();
	signals:
		void buttonPressed(bool last);
};

#endif
