/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include "Player.h"
#include "Channel.h"
#include "Overlay.h"
#include "Global.h"

static ConfigWidget *OverlayConfigDialogNew() {
	return new OverlayConfig();
}

static ConfigRegistrar registrar(60, OverlayConfigDialogNew);

OverlayConfig::OverlayConfig(QWidget *p) : ConfigWidget(p) {
	QGroupBox *qgbOptions = new QGroupBox(tr("Options"));
	QGroupBox *qgbPosition = new QGroupBox(tr("Position"));
	QGroupBox *qgbColors = new QGroupBox(tr("Font"));
	QLabel *lab;
	QVBoxLayout *v;
	QGridLayout *grid;

	grid=new QGridLayout();

	qcbEnable = new QCheckBox(tr("Enable Overlay"));
	qcbEnable->setObjectName("Enable");
	qcbEnable->setToolTip(tr("Enable overlay."));
	qcbEnable->setWhatsThis(tr("This sets whether the overlay is enabled or not. This settings is "
							"only checked with D3D9 applications are started, so make sure Mumble "
							"is running and this option is on before you start the application."));
	qcbEnable->setChecked(g.s.bOverlayEnable);
	grid->addWidget(qcbEnable, 0, 0);
	qgbOptions->setLayout(grid);

	grid=new QGridLayout();

	qcbLeft = new QCheckBox(tr("Grow Left"));
	qcbLeft->setObjectName("Left");
	qcbLeft->setToolTip(tr("Let overlay grow to the left"));
	qcbLeft->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
							"you have selected. This allows the overlay to grow to the left if "
							"needed."));
	qcbLeft->setChecked(g.s.bOverlayLeft);
	qcbRight = new QCheckBox(tr("Grow Right"));
	qcbRight->setObjectName("Right");
	qcbRight->setToolTip(tr("Let overlay grow to the Right"));
	qcbRight->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
							"you have selected. This allows the overlay to grow to the Right if "
							"needed."));
	qcbRight->setChecked(g.s.bOverlayRight);
	qcbTop = new QCheckBox(tr("Grow Up"));
	qcbTop->setObjectName("Top");
	qcbTop->setToolTip(tr("Let overlay grow upwards"));
	qcbTop->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
							"you have selected. This allows the overlay to grow to the top if "
							"needed."));
	qcbTop->setChecked(g.s.bOverlayTop);
	qcbBottom = new QCheckBox(tr("Grow Down"));
	qcbBottom->setObjectName("Bottom");
	qcbBottom->setToolTip(tr("Let overlay grow downwards"));
	qcbBottom->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
							"you have selected. This allows the overlay to grow towards the bottom if "
							"needed."));
	qcbBottom->setChecked(g.s.bOverlayBottom);

	qsX = new QSlider(Qt::Horizontal);
	qsX->setObjectName("X");
	qsX->setToolTip(tr("X-Position of Overlay"));
	qsX->setWhatsThis(tr("This sets the relative X position of the overlay."));
	qsX->setRange(0, 100);
	qsX->setSingleStep(1);
	qsX->setPageStep(10);
	qsX->setValue(qRound64(g.s.fOverlayX * 100));

	qsY = new QSlider(Qt::Vertical);
	qsY->setObjectName("Y");
	qsY->setToolTip(tr("Y-Position of Overlay"));
	qsY->setWhatsThis(tr("This sets the relative Y position of the overlay."));
	qsY->setRange(0, 100);
	qsY->setSingleStep(1);
	qsY->setPageStep(10);
	qsY->setValue(100 - qRound64(g.s.fOverlayY * 100));

	grid->addWidget(qcbLeft, 1, 0);
	grid->addWidget(qcbTop, 0, 1);
	grid->addWidget(qcbBottom, 2, 1);
	grid->addWidget(qcbRight, 1, 2);

	grid->addWidget(qsX, 3, 0, 1, 3);
	grid->addWidget(qsY, 0, 3, 3, 1);

	qgbPosition->setLayout(grid);

	grid = new QGridLayout();

	qfFont = g.s.qfOverlayFont;
	qcPlayer = g.s.qcOverlayPlayer;
	qcTalking= g.s.qcOverlayTalking;
	qcAltTalking= g.s.qcOverlayAltTalking;
	qcChannel = g.s.qcOverlayChannel;
	qcChannelTalking = g.s.qcOverlayChannelTalking;

	lab=new QLabel(tr("Current Font"));
	qlCurrentFont = new QLabel();
	qlCurrentFont->setText(qfFont.family());

	qpbSetFont = new QPushButton(tr("Set Font"));
	qpbSetFont->setObjectName("SetFont");

	grid->addWidget(lab, 0, 0);
	grid->addWidget(qlCurrentFont, 0, 1);
	grid->addWidget(qpbSetFont, 0, 2);

	qsMaxWidth = new QSlider(Qt::Horizontal);
	qsMaxWidth->setObjectName("MaxWidth");
	qsMaxWidth->setToolTip(tr("Maximum width of names."));
	qsMaxWidth->setWhatsThis(tr("This sets the maximum width of names shows, relative to the height. If you set this to 100%, no line "
							"of text will be wider than it is high. At 500%, no line will be more than 5 times wider than it's height, "
							"and so on."));
	qsMaxWidth->setRange(100, 5000);
	qsMaxWidth->setSingleStep(10);
	qsMaxWidth->setPageStep(50);
	qsMaxWidth->setValue(qRound64(g.s.fOverlayWidth * 100));

	qlMaxWidth=new QLabel();
	qlMaxWidth->setMinimumWidth(40);
	on_MaxWidth_valueChanged(qsMaxWidth->value());

	lab = new QLabel(tr("Maximum width"));
	lab->setBuddy(qsMaxWidth);

	grid->addWidget(lab, 1, 0);
	grid->addWidget(qsMaxWidth, 1, 1);
	grid->addWidget(qlMaxWidth, 1, 2);

	qlPlayer = new QLabel();
	qpbPlayer = new QPushButton(tr("Change"));
	qpbPlayer->setObjectName("Player");
	lab = new QLabel(tr("Color for players"));
	setColorLabel(qlPlayer, qcPlayer);

	grid->addWidget(lab, 2, 0);
	grid->addWidget(qlPlayer, 2, 1);
	grid->addWidget(qpbPlayer, 2, 2);

	qlTalking = new QLabel();
	qpbTalking = new QPushButton(tr("Change"));
	qpbTalking->setObjectName("Talking");
	lab = new QLabel(tr("Color for talking players"));
	setColorLabel(qlTalking, qcTalking);

	grid->addWidget(lab, 3, 0);
	grid->addWidget(qlTalking, 3, 1);
	grid->addWidget(qpbTalking, 3, 2);

	qlAltTalking = new QLabel();
	qpbAltTalking = new QPushButton(tr("Change"));
	qpbAltTalking->setObjectName("AltTalking");
	lab = new QLabel(tr("Color for alt-talking players"));
	setColorLabel(qlAltTalking, qcAltTalking);

	grid->addWidget(lab, 4, 0);
	grid->addWidget(qlAltTalking, 4, 1);
	grid->addWidget(qpbAltTalking, 4, 2);

	qlChannel = new QLabel();
	qpbChannel = new QPushButton(tr("Change"));
	qpbChannel->setObjectName("Channel");
	lab = new QLabel(tr("Color for Channels"));
	setColorLabel(qlChannel, qcChannel);

	grid->addWidget(lab, 5, 0);
	grid->addWidget(qlChannel, 5, 1);
	grid->addWidget(qpbChannel, 5, 2);

	qlChannelTalking = new QLabel();
	qpbChannelTalking = new QPushButton(tr("Change"));
	qpbChannelTalking->setObjectName("ChannelTalking");
	lab = new QLabel(tr("Color for active Channels"));
	setColorLabel(qlChannelTalking, qcChannelTalking);

	grid->addWidget(lab, 6, 0);
	grid->addWidget(qlChannelTalking, 6, 1);
	grid->addWidget(qpbChannelTalking, 6, 2);

	qgbColors->setLayout(grid);

    v = new QVBoxLayout;
    v->addWidget(qgbOptions);
    v->addWidget(qgbPosition);
    v->addWidget(qgbColors);
    v->addStretch(1);
    setLayout(v);

    QMetaObject::connectSlotsByName(this);
}

void OverlayConfig::setColorLabel(QLabel *label, QColor col) {
	label->setText(col.name());

    QPalette palette;
    palette.setColor(label->foregroundRole(), col);
    label->setPalette(palette);
}

void OverlayConfig::on_SetFont_clicked() {
	bool ok;
	QFont qf = QFontDialog::getFont(&ok, qfFont, this);
	if (ok) {
		qfFont = qf;
		qlCurrentFont->setText(qfFont.family());
	}
}

void OverlayConfig::on_Player_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcPlayer.rgba(), &ok, this);
	if (ok) {
		qcPlayer = QColor::fromRgba(rgb);
		setColorLabel(qlPlayer, qcPlayer);
	}
}

void OverlayConfig::on_Talking_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcTalking.rgba(), &ok, this);
	if (ok) {
		qcTalking = QColor::fromRgba(rgb);
		setColorLabel(qlTalking, qcTalking);
	}
}

void OverlayConfig::on_AltTalking_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcAltTalking.rgba(), &ok, this);
	if (ok) {
		qcAltTalking = QColor::fromRgba(rgb);
		setColorLabel(qlAltTalking, qcAltTalking);
	}
}

void OverlayConfig::on_Channel_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcChannel.rgba(), &ok, this);
	if (ok) {
		qcChannel = QColor::fromRgba(rgb);
		setColorLabel(qlChannel, qcChannel);
	}
}

void OverlayConfig::on_ChannelTalking_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcChannelTalking.rgba(), &ok, this);
	if (ok) {
		qcChannelTalking = QColor::fromRgba(rgb);
		setColorLabel(qlChannelTalking, qcChannelTalking);
	}
}

void OverlayConfig::on_MaxWidth_valueChanged(int v) {
	qlMaxWidth->setText(QString("%1%").arg(v));
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

QIcon OverlayConfig::icon() const {
	return QIcon(":/icons/config_osd.png");
}

void OverlayConfig::accept() {
	g.s.bOverlayEnable = qcbEnable->isChecked();
	g.s.fOverlayX = qsX->value() / 100.0;
	g.s.fOverlayY = 1.0 - qsY->value() / 100.0;
	g.s.bOverlayLeft = qcbLeft->isChecked();
	g.s.bOverlayRight = qcbRight->isChecked();
	g.s.bOverlayTop = qcbTop->isChecked();
	g.s.bOverlayBottom = qcbBottom->isChecked();
	g.s.qfOverlayFont = qfFont;
	g.s.fOverlayWidth = qsMaxWidth->value() / 100.0;
	g.s.qcOverlayPlayer = qcPlayer;
	g.s.qcOverlayTalking = qcTalking;
	g.s.qcOverlayAltTalking = qcAltTalking;
	g.s.qcOverlayChannel = qcChannel;
	g.s.qcOverlayChannelTalking = qcChannelTalking;

	g.o->forceSettings();
	g.o->setActive(g.s.bOverlayEnable);
}

Overlay::Overlay() : QObject() {
	qlOverlay = new QLibrary(this);

#ifndef QT_NO_DEBUG
	QString path="../overlay/mumble_ol.dll";
#else
	QString path=QString("%1/mumble_ol.dll").arg(qApp->applicationDirPath());
#endif

	qlOverlay->setFileName(path);
	if (! qlOverlay->load()) {
		QMessageBox::critical(NULL, tr("Mumble"), tr("Failed to load overlay library. This means either that the library (mumble_ol.dll) wasn't found in the directory you ran mumble from, or that you're on an OS earlier than WinXP SP2."), QMessageBox::Ok, QMessageBox::NoButton);
		qWarning("Overlay failure");
	}

	sm=reinterpret_cast<SharedMem *>(qlOverlay->resolve("sm"));
#ifndef QT_NO_DEBUG
	if (sm)
		sm->bDebug = true;
#endif

	hpInstall = (HooksProc)qlOverlay->resolve("InstallHooks");
	hpRemove = (HooksProc)qlOverlay->resolve("RemoveHooks");

	qtTimer=new QTimer(this);
	qtTimer->setObjectName("Timer");
	qtTimer->start(1000);

	hMutex = CreateMutex(NULL, false, L"MumbleSharedMutex");

	bShowAll = true;

	forceSettings();

	QMetaObject::connectSlotsByName(this);
}

Overlay::~Overlay() {
	setActive(false);
	qlOverlay->unload();
}

bool Overlay::isActive() const {
	if (! sm)
		return false;

	return sm->bHooked;
}

void Overlay::setActive(bool act) {
	if (! sm)
		return;

	if (act)
		hpInstall();
	else
		hpRemove();
}

void Overlay::on_Timer_timeout() {
	if (sm)
		sm->lastAppAlive = GetTickCount();
}

void Overlay::toggleShow() {
	if (! sm)
		return;

	DWORD dwWaitResult = WaitForSingleObject(hMutex, 500L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		if (sm->bShow && bShowAll) {
			bShowAll = false;
		} else if (sm->bShow) {
			sm->bShow = false;
		} else {
			sm->bShow = true;
			bShowAll = true;
		}
		ReleaseMutex(hMutex);
	}
	updateOverlay();
}

void Overlay::forceSettings() {
	QString str;
	const wchar_t *wstr;

	if (! sm)
		return;

	DWORD dwWaitResult = WaitForSingleObject(hMutex, 500L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		str = g.s.qfOverlayFont.family();
		wstr = reinterpret_cast<const wchar_t *>(str.utf16());
		wcscpy(sm->strFontname, wstr);
		sm->iFontSize = g.s.qfOverlayFont.pointSize();
		sm->bFontBold = g.s.qfOverlayFont.bold();
		sm->bFontItalic = g.s.qfOverlayFont.italic();
		sm->fWidthFactor = g.s.fOverlayWidth;
		sm->fX = g.s.fOverlayX;
		sm->fY = g.s.fOverlayY;
		sm->bTop = g.s.bOverlayTop;
		sm->bBottom = g.s.bOverlayBottom;
		sm->bLeft = g.s.bOverlayLeft;
		sm->bRight = g.s.bOverlayRight;
		sm->bReset = true;
		ReleaseMutex(hMutex);
	}
	updateOverlay();
}

#define SAFE_INC_IDX(x) x=(x < NUM_TEXTS) ? (x+1) : (NUM_TEXTS-1)

typedef QPair<QString, DWORD> qpChanCol;

void Overlay::updateOverlay() {
	DWORD colPlayer = g.s.qcOverlayPlayer.rgba();
	DWORD colTalking = g.s.qcOverlayTalking.rgba();
	DWORD colAltTalking = g.s.qcOverlayAltTalking.rgba();
	DWORD colChannel = g.s.qcOverlayChannel.rgba();
	DWORD colChannelTalking = g.s.qcOverlayChannelTalking.rgba();
	QString str;
	QList<qpChanCol> linkchans;
	const wchar_t *wstr;

	if (! sm)
		return;

	if (g.sId) {
		Channel *home = Player::get(g.sId)->cChannel;
		foreach(Channel *c, home->allLinks()) {
			if (home == c)
				continue;

			bool act = false;
			foreach(Player *p, c->qlPlayers) {
				act = act || p->bTalking;
				if (p->bTalking)
					linkchans << qpChanCol(p->qsName + QString("[") + c->qsName + QString("]"), colChannelTalking);
			}
			if (! act)
				linkchans << qpChanCol(c->qsName, colChannel);
		}
		qSort(linkchans);
	}

	DWORD dwWaitResult = WaitForSingleObject(hMutex, 500L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		int idx = 0;

		if (g.s.bOverlayTop) {
			foreach(qpChanCol cc, linkchans) {
				if (bShowAll || (cc.second == colChannelTalking)) {
					sm->texts[idx].color = cc.second;
					str = cc.first.left(127);
					wstr = reinterpret_cast<const wchar_t *>(str.utf16());
					wcscpy(sm->texts[idx].text, wstr);
					SAFE_INC_IDX(idx);
				}
			}
			if (linkchans.count() > 0) {
				sm->texts[idx].text[0] = L' ';
				sm->texts[idx].text[1] = 0;
				SAFE_INC_IDX(idx);
			}
		}

		foreach(Player *p, Player::get(g.sId)->cChannel->qlPlayers) {
			if (bShowAll || p->bTalking) {
				QString name = p->qsName;
				if (p->bDeaf || p->bSelfDeaf)
					name = name + QString("(D)");
				else if (p->bMute || p->bSelfMute || p->bLocalMute)
					name = name + QString("(M)");
				sm->texts[idx].color = p->bTalking ? (p->bAltSpeak ? colAltTalking : colTalking) : colPlayer;
				str = name.left(127);
				wstr = reinterpret_cast<const wchar_t *>(str.utf16());
				wcscpy(sm->texts[idx].text, wstr);
				SAFE_INC_IDX(idx);
			}
		}

		if (! g.s.bOverlayTop) {
			if (linkchans.count() > 0) {
				sm->texts[idx].text[0] = L' ';
				sm->texts[idx].text[1] = 0;
				SAFE_INC_IDX(idx);
			}
			foreach(qpChanCol cc, linkchans) {
				if (bShowAll || (cc.second == colChannelTalking)) {
					sm->texts[idx].color = cc.second;
					str = cc.first.left(127);
					wstr = reinterpret_cast<const wchar_t *>(str.utf16());
					wcscpy(sm->texts[idx].text, wstr);
					SAFE_INC_IDX(idx);
				}
			}
		}

		for(; idx<NUM_TEXTS;idx++)
			sm->texts[idx].text[0] = 0;
		ReleaseMutex(hMutex);
	}
}
