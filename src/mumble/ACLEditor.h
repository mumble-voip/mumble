// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ACLEDITOR_H_
#define MUMBLE_MUMBLE_ACLEDITOR_H_

#include "ACL.h"
#include "Group.h"
#include "Mumble.pb.h"

#include "ui_ACLEditor.h"

class ACLGroup : public Group {
private:
	Q_DISABLE_COPY(ACLGroup)
public:
	bool bInherited;
	ACLGroup(const QString &name);
};

class ACLEditor : public QDialog, public Ui::ACLEditor {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ACLEditor)
protected:
	typedef QPair< QString, int > idname;
	MumbleProto::ACL msg;
	enum WaitID { GroupAdd, GroupRemove, GroupInherit, ACLList };
	QHash< int, QString > qhNameCache;
	QHash< QString, int > qhIDCache;
	QHash< QString, int > qhNameWait;

	int iUnknown;

	void refill(WaitID what);

	ACLGroup *currentGroup();
	ChanACL *currentACL();

	int iId;
	bool bInheritACL;
	QList< ChanACL * > qlACLs;
	QList< ACLGroup * > qlGroups;
	ChanACL *pcaPassword;

	int numInheritACL;
	unsigned int iChannel;
	bool bAddChannelMode;

	const QString userName(int id);
	int id(const QString &uname);

	QList< QCheckBox * > qlACLAllow;
	QList< QCheckBox * > qlACLDeny;
	QList< ChanACL::Perm > qlPerms;

	void updatePasswordACL(void);
	void updatePasswordField(void);
	void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
	void fillWidgetFromSet(QListWidget *, const QSet< int > &);

public:
	ACLEditor(unsigned int parentchannelid, QWidget *p = nullptr);
	ACLEditor(unsigned int channelid, const MumbleProto::ACL &mea, QWidget *p = nullptr);
	~ACLEditor();
	void returnQuery(const MumbleProto::QueryUsers &mqu);
public slots:
	void accept() Q_DECL_OVERRIDE;
public slots:
	void refillACL();
	void refillGroupNames();
	void refillGroupAdd();
	void refillGroupRemove();
	void refillGroupInherit();
	void refillComboBoxes();
	void groupEnableCheck();
	void ACLEnableCheck();

	void on_qtwTab_currentChanged(int index);
	void on_qlwACLs_currentRowChanged();
	void on_qpbACLAdd_clicked();
	void on_qpbACLRemove_clicked();
	void on_qpbACLUp_clicked();
	void on_qpbACLDown_clicked();
	void on_qcbACLInherit_clicked(bool checked);
	void on_qcbACLApplyHere_clicked(bool checked);
	void on_qcbACLApplySubs_clicked(bool checked);
	void on_qcbACLGroup_activated(const QString &text);
	void on_qcbACLUser_activated(const QString &text);
	void ACLPermissions_clicked();
	void qcbACLGroup_focusLost();
	void qcbACLUser_focusLost();
	void qcbACLGroup_spacePressed();
	void qcbACLUser_spacePressed();

	void on_qcbGroupList_activated(const QString &text);
	void on_qcbGroupList_editTextChanged(const QString &text);
	void on_qpbGroupAdd_clicked();
	void on_qpbGroupRemove_clicked();
	void on_qcbGroupInherit_clicked(bool checked);
	void on_qcbGroupInheritable_clicked(bool checked);
	void on_qpbGroupAddAdd_clicked();
	void on_qpbGroupAddRemove_clicked();
	void on_qpbGroupRemoveAdd_clicked();
	void on_qpbGroupRemoveRemove_clicked();
	void on_qpbGroupInheritRemove_clicked();
};

#endif
