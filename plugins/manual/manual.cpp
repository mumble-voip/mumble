/* Copyright (C) 2009-2010, Stefan Hacker <dD0t@users.sourceforge.net>

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

#define _USE_MATH_DEFINES

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QMessageBox>
#include <QPointer>
#include <math.h>
#include <float.h>
#include "manual.h"
#include "ui_manual.h"

#ifdef Q_OS_UNIX
#define __cdecl
typedef WId HWND;
#define DLL_PUBLIC __attribute__((visibility("default")))
#else
#define DLL_PUBLIC __declspec(dllexport)
#endif

#include "../mumble_plugin.h"

static QPointer<Manual> mDlg = NULL;
static bool bLinkable = false;
static bool bActive = true;

static int iAzimuth = 0;
static int iElevation = 0;

static struct {
	float avatar_pos[3];
	float avatar_front[3];
	float avatar_top[3];
	float camera_pos[3];
	float camera_front[3];
	float camera_top[3];
	std::string context;
	std::wstring identity;
} my = {{0,0,0}, {0,0,0}, {0,0,0},
	{0,0,0}, {0,0,0}, {0,0,0},
	std::string(), std::wstring()
};


Manual::Manual(QWidget *p) : QDialog(p) {
	setupUi(this);

	qgvPosition->viewport()->installEventFilter(this);
	qgvPosition->scale(1.0f, 1.0f);
	qgsScene = new QGraphicsScene(QRectF(-5.0f, -5.0f, 10.0f, 10.0f), this);
	qgiPosition = qgsScene->addEllipse(QRectF(-0.5f, -0.5f, 1.0f, 1.0f), QPen(Qt::black), QBrush(Qt::red));

	qgvPosition->setScene(qgsScene);
	qgvPosition->fitInView(-5.0f, -5.0f, 10.0f, 10.0f, Qt::KeepAspectRatio);

	qdsbX->setRange(-FLT_MAX, FLT_MAX);
	qdsbY->setRange(-FLT_MAX, FLT_MAX);
	qdsbZ->setRange(-FLT_MAX, FLT_MAX);

	qdsbX->setValue(my.avatar_pos[0]);
	qdsbY->setValue(my.avatar_pos[1]);
	qdsbZ->setValue(my.avatar_pos[2]);

	qpbActivated->setChecked(bActive);
	qpbLinked->setChecked(bLinkable);

	qsbAzimuth->setValue(iAzimuth);
	qsbElevation->setValue(iElevation);
	updateTopAndFront(iAzimuth, iElevation);
}

bool Manual::eventFilter(QObject *obj, QEvent *evt) {
	if ((evt->type() == QEvent::MouseButtonPress) || (evt->type() == QEvent::MouseMove)) {
		QMouseEvent *qme = dynamic_cast<QMouseEvent *>(evt);
		if (qme) {
			if (qme->buttons() & Qt::LeftButton) {
				QPointF qpf = qgvPosition->mapToScene(qme->pos());
				qdsbX->setValue(-qpf.x());
				qdsbZ->setValue(-qpf.y());
				qgiPosition->setPos(qpf);
			}
		}
	}
	return QDialog::eventFilter(obj, evt);
}

void Manual::changeEvent(QEvent *e) {
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			retranslateUi(this);
			break;
		default:
			break;
	}
}

void Manual::on_qpbUnhinge_pressed() {
	qpbUnhinge->setEnabled(false);
	mDlg->setParent(NULL);
	mDlg->show();
}

void Manual::on_qpbLinked_clicked(bool b) {
	bLinkable = b;
}

void Manual::on_qpbActivated_clicked(bool b) {
	bActive = b;
}

void Manual::on_qdsbX_valueChanged(double d) {
	my.avatar_pos[0] = my.camera_pos[0] = static_cast<float>(d);
	qgiPosition->setPos(-my.avatar_pos[0], -my.avatar_pos[2]);
}

void Manual::on_qdsbY_valueChanged(double d) {
	my.avatar_pos[1] = my.camera_pos[1] = static_cast<float>(d);
}

void Manual::on_qdsbZ_valueChanged(double d) {
	my.avatar_pos[2] = my.camera_pos[2] = static_cast<float>(d);
	qgiPosition->setPos(-my.avatar_pos[0], -my.avatar_pos[2]);
}

void Manual::on_qsbAzimuth_valueChanged(int i) {
	if (i > 180)
		qdAzimuth->setValue(-360 + i);
	else
		qdAzimuth->setValue(i);

	updateTopAndFront(i, qsbElevation->value());
}

void Manual::on_qsbElevation_valueChanged(int i) {
	qdElevation->setValue(90 - i);
	updateTopAndFront(qsbAzimuth->value(), i);
}

void Manual::on_qdAzimuth_valueChanged(int i) {
	if (i < 0)
		qsbAzimuth->setValue(360 + i);
	else
		qsbAzimuth->setValue(i);
}

void Manual::on_qdElevation_valueChanged(int i) {
	if (i < -90)
		qdElevation->setValue(180);
	else if (i < 0)
		qdElevation->setValue(0);
	else
		qsbElevation->setValue(90 - i);
}

void Manual::on_qleContext_editingFinished() {
	my.context = qleContext->text().toStdString();
}

void Manual::on_qleIdentity_editingFinished() {
	my.identity = qleIdentity->text().toStdWString();
}

void Manual::on_buttonBox_clicked(QAbstractButton *button) {
	if (buttonBox->buttonRole(button) == buttonBox->ResetRole) {
		qpbLinked->setChecked(false);
		qpbActivated->setChecked(true);

		bLinkable = false;
		bActive = true;

		qdsbX->setValue(0);
		qdsbY->setValue(0);
		qdsbZ->setValue(0);

		qleContext->clear();
		qleIdentity->clear();

		qsbElevation->setValue(0);
		qsbAzimuth->setValue(0);
	}
}

void Manual::updateTopAndFront(int azimuth, int elevation) {
	iAzimuth = azimuth;
	iElevation = elevation;

	double azim = azimuth * M_PI / 180.;
	double elev = elevation * M_PI / 180.;

	my.avatar_front[0]	= static_cast<float>(cos(elev) * sin(azim));
	my.avatar_front[1]	= static_cast<float>(sin(elev));
	my.avatar_front[2]	= static_cast<float>(cos(elev) * cos(azim));

	my.avatar_top[0]	= static_cast<float>(-sin(elev) * sin(azim));
	my.avatar_top[1]	= static_cast<float>(cos(elev));
	my.avatar_top[2]	= static_cast<float>(-sin(elev) * cos(azim));

	memcpy(my.camera_top, my.avatar_top, sizeof(float) * 3);
	memcpy(my.camera_front, my.avatar_front, sizeof(float) * 3);
}

static int trylock() {
	return bLinkable;
}

static void unlock() {
	if (mDlg) {
		mDlg->qpbLinked->setChecked(false);
	}
	bLinkable = false;
}

static void config(HWND h) {
	if (mDlg) {
		mDlg->setParent(QWidget::find(h), Qt::Dialog);
		mDlg->qpbUnhinge->setEnabled(true);
	} else {
		mDlg = new Manual(QWidget::find(h));
	}

	mDlg->show();
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	if (!bLinkable)
		return false;

	if (!bActive) {
		memset(avatar_pos, 0, sizeof(float)*3);
		memset(camera_pos, 0, sizeof(float)*3);
		return true;
	}

	memcpy(avatar_pos, my.avatar_pos, sizeof(float)*3);
	memcpy(avatar_front, my.avatar_front, sizeof(float)*3);
	memcpy(avatar_top, my.avatar_top, sizeof(float)*3);

	memcpy(camera_pos, my.camera_pos, sizeof(float)*3);
	memcpy(camera_front, my.camera_front, sizeof(float)*3);
	memcpy(camera_top, my.camera_top, sizeof(float)*3);

	context.assign(my.context);
	identity.assign(my.identity);

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"This is the manual placement plugin. It allows you to place yourself manually.");
}

static std::wstring description(L"Manual placement plugin");
static std::wstring shortname(L"Manual placement");

static void about(WId h) {
	QMessageBox::about(QWidget::find(h), QString::fromStdWString(description), QString::fromStdWString(longdesc()));
}

static MumblePlugin manual = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	shortname,
	about,
	config,
	trylock,
	unlock,
	longdesc,
	fetch
};

extern "C" DLL_PUBLIC MumblePlugin *getMumblePlugin() {
	return &manual;
}
