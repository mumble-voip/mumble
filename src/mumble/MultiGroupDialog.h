// Copyright 2026 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.
//
// 版本号: 1.0.1
// 修订日期: 2026-07-17
// 作者/生成者: Auto (Composer)
// 最后修订人: Auto (Composer)
// 变更说明:
//   - 1.0.0 (2026-07-17): 初版。多群组（Channel Listener）管理对话框。
//   - 1.0.1 (2026-07-17): 合入 yqwer/mumble 最新 master（约 1.7.x）时同步保留。

#ifndef MUMBLE_MUMBLE_MULTIGROUPDIALOG_H_
#define MUMBLE_MUMBLE_MULTIGROUPDIALOG_H_

#include "ui_MultiGroupDialog.h"

#include <QtWidgets/QDialog>

class Channel;

/// Dialog for managing "multi-group" membership via Channel Listeners.
/// Speak/PTT still targets only the user's current channel (cChannel).
class MultiGroupDialog : public QDialog, public Ui::MultiGroupDialog {
private:
	Q_OBJECT
	Q_DISABLE_COPY(MultiGroupDialog)

	void collectChannels(const Channel *channel, QList< const Channel * > &out) const;
	void refreshLists();

public:
	explicit MultiGroupDialog(QWidget *parent = nullptr);

public slots:
	void on_qpbJoin_clicked();
	void on_qpbLeave_clicked();
	void on_qpbRefresh_clicked();
	void showEvent(QShowEvent *event) override;
};

#endif
