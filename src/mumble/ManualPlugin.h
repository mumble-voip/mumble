// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MANUALPLUGIN_H_
#define MUMBLE_MUMBLE_MANUALPLUGIN_H_

#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
# include <QtWidgets/QDialog>
# include <QtWidgets/QGraphicsItem>
# include <QtWidgets/QGraphicsScene>
#else
# include <QDialog>
# include <QGraphicsItem>
# include <QGraphicsScene>
#endif

#include "ui_ManualPlugin.h"

#include "../../plugins/mumble_plugin.h"

class Manual : public QDialog, public Ui::Manual {
		Q_OBJECT
	public:
		Manual(QWidget *parent = 0);

	public slots:
		void on_qpbUnhinge_pressed();
		void on_qpbLinked_clicked(bool);
		void on_qpbActivated_clicked(bool);
		void on_qdsbX_valueChanged(double);
		void on_qdsbY_valueChanged(double);
		void on_qdsbZ_valueChanged(double);
		void on_qsbAzimuth_valueChanged(int);
		void on_qsbElevation_valueChanged(int);
		void on_qdAzimuth_valueChanged(int);
		void on_qdElevation_valueChanged(int);
		void on_qleContext_editingFinished();
		void on_qleIdentity_editingFinished();
		void on_buttonBox_clicked(QAbstractButton *);
	protected:
		QGraphicsScene *qgsScene;
		QGraphicsItem *qgiPosition;

		bool eventFilter(QObject *, QEvent *);
		void changeEvent(QEvent *e);
		void updateTopAndFront(int orientation, int azimut);
};

MumblePlugin *ManualPlugin_getMumblePlugin();
MumblePluginQt *ManualPlugin_getMumblePluginQt();

#endif
