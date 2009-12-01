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
#define __declspec(x)
typedef unsigned long HWND;
#define DLL_PUBLIC __attribute__((visibility("default")))
#else
#define DLL_PUBLIC __declspec(dllexport)
#endif

#include "../mumble_plugin.h"

QPointer<Manual> mDlg = NULL;
bool bLinkable = false;
bool bActive = true;

int iOrientation = 0;
int iAzimut = 0;

struct {
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
	std::string(), std::wstring()};


Manual::Manual(QWidget *parent) : QDialog(parent)
{
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

    qsbOrientation->setValue(iOrientation);
    qsbAzimut->setValue(iAzimut);
    updateTopAndFront(iOrientation, iAzimut);
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

void Manual::changeEvent(QEvent *e)
{
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

void Manual::on_qsbOrientation_valueChanged(int i) {
	if (i > 180)
		qdOrientation->setValue(-360 + i);
	else
		qdOrientation->setValue(i);

	updateTopAndFront(i, qsbAzimut->value());
}

void Manual::on_qsbAzimut_valueChanged(int i) {
	qdAzimut->setValue(i + 90);
	updateTopAndFront(qsbOrientation->value(), i);
}

void Manual::on_qdOrientation_valueChanged(int i) {
	if (i < 0)
		qsbOrientation->setValue(360 + i);
	else
		qsbOrientation->setValue(i);
}

void Manual::on_qdAzimut_valueChanged(int i) {
	if (i > 180)
		qdAzimut->setValue(180);
	else if (i < 0)
		qdAzimut->setValue(0);
	else
		qsbAzimut->setValue(i - 90);
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

		qsbAzimut->setValue(0);
		qsbOrientation->setValue(0);
	}
}

void Manual::updateTopAndFront(int orientation, int azimut) {
	iOrientation = orientation;
	iAzimut = azimut;

	double ori = orientation * M_PI / 180;
	double azi = azimut * M_PI / 180;

	my.avatar_front[0]	= - cos(azi) * sin(ori);
	my.avatar_front[1]	= sin(azi);
	my.avatar_front[2]	= cos(azi) * cos(ori);

	my.avatar_top[0]	= - sin(azi) * sin(ori);
	my.avatar_top[1]	= sin(azi - M_PI_2);
	my.avatar_top[2]	= sin(azi) * cos(ori);

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

static void config(WId h) {
	if (mDlg) {
		mDlg->setParent(QWidget::find(h), Qt::Dialog);
		mDlg->qpbUnhinge->setEnabled(true);
	}
	else {
		mDlg = new Manual(QWidget::find(h));
	}

	mDlg->show();
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	if (!bLinkable)
		return false;

	if (!bActive) {
		memset(avatar_pos, 0, sizeof(float)*3);
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
static std::wstring shortname(L"manual");

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
