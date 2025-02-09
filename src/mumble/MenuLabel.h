// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MENULABEL_H_
#define MUMBLE_MUMBLE_MENULABEL_H_

#include <QString>
#include <QWidgetAction>

/// This class can be used as a QAction for a QMenu to display static,
/// non-interactive text. It is useful for labeling inline QWidgets.
class MenuLabel : public QWidgetAction {
	Q_OBJECT

public:
	MenuLabel(const QString &text, QObject *parent = nullptr);

protected:
	QWidget *createWidget(QWidget *parent) override;

private:
	const QString m_text;
};

#endif
