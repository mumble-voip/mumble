// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBALSHORTCUT_H_
#define MUMBLE_MUMBLE_GLOBALSHORTCUT_H_

#include <QtCore/QThread>
#include <QtCore/QtGlobal>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QToolButton>

#include "Channel.h"
#include "ConfigDialog.h"
#include "MUComboBox.h"
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
	QList< QVariant > qlActive;
signals:
	void down(QVariant);
	void triggered(bool, QVariant);

public:
	QString qsToolTip;
	QString qsWhatsThis;
	QString name;
	QVariant qvDefault;
	int idx;

	GlobalShortcut(QObject *parent, int index, QString qsName, QVariant def = QVariant());
	~GlobalShortcut() Q_DECL_OVERRIDE;

	bool active() const { return !qlActive.isEmpty(); }
};

/**
 * Combo box widget used to define the kind of action a shortcut triggers. Then
 * entries get auto-generated from the GlobalShortcutEngine::qmShortcuts store.
 *
 * @see GlobalShortcutEngine
 */
class ShortcutActionWidget : public QWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ShortcutActionWidget)
	Q_PROPERTY(unsigned int index READ index WRITE setIndex USER true)

	MUComboBox *m_comboBox;

public:
	ShortcutActionWidget(QWidget *p = nullptr);
	unsigned int index() const;
	void setIndex(unsigned int);
};

class ShortcutToggleWidget : public MUComboBox {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ShortcutToggleWidget)
	Q_PROPERTY(int index READ index WRITE setIndex USER true)
public:
	ShortcutToggleWidget(QWidget *p = nullptr);
	int index() const;
	void setIndex(int);
};

class ChannelSelectWidget : public MUComboBox {
	Q_OBJECT
	Q_DISABLE_COPY(ChannelSelectWidget)
	Q_PROPERTY(ChannelTarget currentChannel READ currentChannel WRITE setCurrentChannel USER true)
public:
	ChannelSelectWidget(QWidget *parent = nullptr);

	ChannelTarget currentChannel() const;
	void setCurrentChannel(const ChannelTarget &);
};

class TextEditWidget : public QWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(TextEditWidget)
	Q_PROPERTY(QString currentString READ currentString WRITE setCurrentString USER true)

	QLineEdit *m_lineEdit;

public:
	TextEditWidget(QWidget *p = nullptr);
	QString currentString() const;
	void setCurrentString(const QString &);
};

/**
 * Dialog which is used to select the targets of a targeted shortcut like Whisper.
 */
class ShortcutTargetDialog : public QDialog, public Ui::GlobalShortcutTarget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ShortcutTargetDialog)

	enum Target { SELECTION = 0, USERLIST = 1, CHANNEL = 2 };

protected:
	QMap< QString, QString > qmHashNames;
	ShortcutTarget stTarget;

public:
	ShortcutTargetDialog(const ShortcutTarget &, QWidget *p = nullptr);
	ShortcutTarget target() const;
public slots:
	void accept() Q_DECL_OVERRIDE;
	void on_qcbTarget_currentIndexChanged(int index);
	void on_qpbAdd_clicked();
	void on_qpbRemove_clicked();
};

enum ShortcutTargetTypes {
	SHORTCUT_TARGET_ROOT              = -1,
	SHORTCUT_TARGET_PARENT            = -2,
	SHORTCUT_TARGET_CURRENT           = -3,
	SHORTCUT_TARGET_SUBCHANNEL        = -4,
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
	ShortcutTargetWidget(QWidget *p = nullptr);
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
 * @see ChannelSelectWidget
 */
class ShortcutDelegate : public QStyledItemDelegate {
	Q_OBJECT
	Q_DISABLE_COPY(ShortcutDelegate)
public:
	ShortcutDelegate(QObject *);
	~ShortcutDelegate() Q_DECL_OVERRIDE;
	QString displayText(const QVariant &, const QLocale &) const Q_DECL_OVERRIDE;
	bool helpEvent(QHelpEvent *, QAbstractItemView *, const QStyleOptionViewItem &,
				   const QModelIndex &) Q_DECL_OVERRIDE;
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
	QList< Shortcut > qlShortcuts;
	QTreeWidgetItem *itemForShortcut(const Shortcut &) const;
	bool showWarning() const;
	bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

public:
	/// The unique name of this ConfigWidget
	static const QString name;
	GlobalShortcutConfig(Settings &st);
	virtual QString title() const Q_DECL_OVERRIDE;
	virtual const QString &getName() const Q_DECL_OVERRIDE;
	virtual QIcon icon() const Q_DECL_OVERRIDE;
public slots:
	void accept() const Q_DECL_OVERRIDE;
	void save() const Q_DECL_OVERRIDE;
	void load(const Settings &r) Q_DECL_OVERRIDE;
	void reload();
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
	qsizetype iNumUp;
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
	struct ButtonInfo {
		QString device;
		QString devicePrefix;
		QString name;

		ButtonInfo() : device(tr("Unknown")), name(tr("Unknown")) {}
	};

	bool bNeedRemap;
	Timer tReset;

	static GlobalShortcutEngine *engine;
	static GlobalShortcutEngine *platformInit();

	QHash< int, GlobalShortcut * > qmShortcuts;
	QList< QVariant > qlActiveButtons;
	QList< QVariant > qlDownButtons;
	QList< QVariant > qlSuppressed;

	QList< QVariant > qlButtonList;
	QList< QList< ShortcutKey * > > qlShortcutList;

	GlobalShortcutEngine(QObject *p = nullptr);
	~GlobalShortcutEngine() Q_DECL_OVERRIDE;
	void resetMap();
	void remap();
	virtual void needRemap();
	void run() Q_DECL_OVERRIDE;

	bool handleButton(const QVariant &, bool);
	static void add(GlobalShortcut *);
	static void remove(GlobalShortcut *);

	virtual bool canDisable();
	virtual bool canSuppress();
	virtual bool enabled();
	virtual void setEnabled(bool b);

	virtual ButtonInfo buttonInfo(const QVariant &) = 0;
signals:
	void buttonPressed(bool last);
};

#endif
