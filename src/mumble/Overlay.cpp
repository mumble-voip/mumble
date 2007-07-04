/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
	qcbEnable->setWhatsThis(tr("This sets whether the overlay is enabled or not. This setting is "
							"only checked with D3D9 applications are started, so make sure Mumble "
							"is running and this option is on before you start the application.<br />"
							"Please note that if you start the application after starting Mumble, "
							"or if you disable the overlay while running, there is no safe way "
							"to restart the overlay without also restarting the application."));
	qcbEnable->setChecked(g.s.bOverlayEnable);
	grid->addWidget(qcbEnable, 0, 0, 1, 2);

	qcbShow = new QComboBox();
	qcbShow->addItem(tr("No one"), Settings::Nothing);
	qcbShow->addItem(tr("Only talking"), Settings::Talking);
	qcbShow->addItem(tr("Everyone"), Settings::All);
	qcbShow->setCurrentIndex(g.s.osOverlay);
	lab = new QLabel(tr("Show"));
	lab->setBuddy(qcbShow);
	qcbShow->setToolTip(tr("Who to show on the overlay"));
	qcbShow->setWhatsThis(tr("<b>This sets who to show in the in-game overlay.</b><br />"
			"If many people are connected to the same channel, the overlay list might be "
			"very long. Use this to shorten it.<br />"
			"<i>No one</i> - Don't show anyone (but leave overlay running).<br />"
			"<i>Only talking</i> - Only show talking people.<br />"
			"<i>Everyone</i> - Show everyone."));
	grid->addWidget(lab, 1, 0);
	grid->addWidget(qcbShow, 1, 1, 1, 2);

	qcbAlwaysSelf = new QCheckBox(tr("Always Show Self"));
	qcbAlwaysSelf->setObjectName("AlwaysSelf");
	qcbAlwaysSelf->setToolTip(tr("Always show yourself on overlay."));
	qcbAlwaysSelf->setWhatsThis(tr("This sets whether to always show yourself or not. "
							"This setting is useful if you aren't showing everyone in the overlay, "
							"as then you would only see your own status if you were talking, which "
							"wouldn't let you see that you were deafened or muted."));
	qcbAlwaysSelf->setChecked(g.s.bOverlayAlwaysSelf);
	grid->addWidget(qcbAlwaysSelf, 2, 0, 1, 2);

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

	qsMaxHeight = new QSlider(Qt::Horizontal);
	qsMaxHeight->setObjectName("MaxHeight");
	qsMaxHeight->setToolTip(tr("Maximum height of names."));
	qsMaxHeight->setWhatsThis(tr("This sets the maximum height of names shown, relative to the screen height. If your active 3D window is 800 pixels tall and this is set to 5%, each name will be 40 pixels "
					"tall. Note that the names will not be taller than 60 pixels "
					"no matter what you set here."));
	qsMaxHeight->setRange(10, 200);
	qsMaxHeight->setSingleStep(1);
	qsMaxHeight->setPageStep(10);
	qsMaxHeight->setValue(qRound64(g.s.fOverlayHeight * 1000));

	qlMaxHeight=new QLabel();
	qlMaxHeight->setMinimumHeight(40);
	on_MaxHeight_valueChanged(qsMaxHeight->value());

	lab = new QLabel(tr("Maximum height"));
	lab->setBuddy(qsMaxHeight);

	grid->addWidget(lab, 1, 0);
	grid->addWidget(qsMaxHeight, 1, 1);
	grid->addWidget(qlMaxHeight, 1, 2);

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

void OverlayConfig::on_MaxHeight_valueChanged(int v) {
	qlMaxHeight->setText(QString("%1%").arg(v/10.0, 0, 'f', 1));
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

QIcon OverlayConfig::icon() const {
	return QIcon(":/config_osd.png");
}

void OverlayConfig::accept() {
	g.s.bOverlayEnable = qcbEnable->isChecked();
	g.s.osOverlay = static_cast<Settings::OverlayShow>(qcbShow->currentIndex());
	g.s.bOverlayAlwaysSelf = qcbAlwaysSelf->isChecked();
	g.s.fOverlayX = qsX->value() / 100.0;
	g.s.fOverlayY = 1.0 - qsY->value() / 100.0;
	g.s.bOverlayLeft = qcbLeft->isChecked();
	g.s.bOverlayRight = qcbRight->isChecked();
	g.s.bOverlayTop = qcbTop->isChecked();
	g.s.bOverlayBottom = qcbBottom->isChecked();
	g.s.qfOverlayFont = qfFont;
	g.s.fOverlayHeight = qsMaxHeight->value() / 1000.0;
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
#ifdef Q_OS_WIN
	QString path="../overlay/mumble_ol.dll";
#else
	QString path="../gloverlay/mumble_ol.so";
#endif
#else
#ifdef Q_OS_WIN
	QString path=QString("%1/mumble_ol.dll").arg(qApp->applicationDirPath());
#else
	QString path=QString("%1/mumble_ol.so").arg(qApp->applicationDirPath());
#endif
#endif

	qlOverlay->setFileName(path);
	if (! qlOverlay->load()) {
#ifdef Q_OS_WIN
		QMessageBox::critical(NULL, tr("Mumble"), tr("Failed to load overlay library. This means either that:\n"
				"- the library (mumble_ol.dll) wasn't found in the directory you ran Mumble from\n"
				"- you're on an OS earlier than WinXP SP2\n"
				"- you do not have the June 2007 updated version of DX9.0c"), QMessageBox::Ok, QMessageBox::NoButton);
#endif
		qWarning("Overlay failure");
	}

	sm.resolve(qlOverlay);

#ifndef QT_NO_DEBUG
	if (sm.sm)
		sm.sm->bDebug = true;
#endif

	qtTimer=new QTimer(this);
	qtTimer->setObjectName("Timer");
	qtTimer->start(1000);

	platformInit();
	forceSettings();

	QMetaObject::connectSlotsByName(this);
}

Overlay::~Overlay() {
	setActive(false);
	qlOverlay->unload();
}

bool Overlay::isActive() const {
	if (! sm.sm)
		return false;

	return sm.sm->bHooked;
}

void Overlay::toggleShow() {
	Settings::OverlayShow ns;

	switch(g.s.osOverlay) {
		case Settings::Nothing:
			ns = Settings::All;
			break;
		case Settings::All:
			ns = Settings::Talking;
			break;
		default:
			ns = Settings::All;
			break;
	}
	g.s.osOverlay = ns;

	if (sm.tryLock()) {
		sm.sm->bShow = (g.s.osOverlay != Settings::Nothing);
		sm.unlock();
	}
	updateOverlay();
}

void Overlay::forceSettings() {
	QString str;

	if (! sm.sm)
		return;

	fixFont();

	if (sm.tryLock()) {
		sm.sm->fX = g.s.fOverlayX;
		sm.sm->fY = g.s.fOverlayY;
		sm.sm->bTop = g.s.bOverlayTop;
		sm.sm->bBottom = g.s.bOverlayBottom;
		sm.sm->bLeft = g.s.bOverlayLeft;
		sm.sm->bRight = g.s.bOverlayRight;
		sm.sm->bReset = true;
		sm.sm->bShow = (g.s.osOverlay != Settings::Nothing);
		sm.sm->fFontSize = g.s.fOverlayHeight;
		sm.unlock();
	}
	updateOverlay();
}

#define SAFE_INC_IDX(x) x=(x < NUM_TEXTS) ? (x+1) : (NUM_TEXTS-1)

typedef QPair<QString, quint32> qpChanCol;

void Overlay::updateOverlay() {
	quint32 colPlayer = g.s.qcOverlayPlayer.rgba();
	quint32 colTalking = g.s.qcOverlayTalking.rgba();
	quint32 colAltTalking = g.s.qcOverlayAltTalking.rgba();
	quint32 colChannel = g.s.qcOverlayChannel.rgba();
	quint32 colChannelTalking = g.s.qcOverlayChannelTalking.rgba();
	QString str;
	QList<qpChanCol> linkchans;

	if (! isActive())
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

	QList<TextLine> lines;

	if (g.sId) {

		if (g.s.bOverlayTop) {
			foreach(qpChanCol cc, linkchans) {
				if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
					lines << TextLine(cc.first, cc.second);
				}
			}
			if (linkchans.count() > 0) {
				lines << TextLine(QString(), 0);
			}
		}

		foreach(Player *p, Player::get(g.sId)->cChannel->qlPlayers) {
			if ((g.s.osOverlay == Settings::All) || p->bTalking || ((p == Player::get(g.sId)) && g.s.bOverlayAlwaysSelf)) {
				QString name = p->qsName;
				if (p->bDeaf || p->bSelfDeaf)
					name = name + QString("(D)");
				else if (p->bMute || p->bSelfMute || p->bLocalMute)
					name = name + QString("(M)");
				lines << TextLine(name, p->bTalking ? (p->bAltSpeak ? colAltTalking : colTalking) : colPlayer);
			}
		}

		if (! g.s.bOverlayTop) {
			if (linkchans.count() > 0) {
				lines << TextLine(QString(), 0);
			}
			foreach(qpChanCol cc, linkchans) {
				if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
					lines << TextLine(cc.first, cc.second);
				}
			}
		}
	}
	setTexts(lines);
}

/*
 * Here's the thing. The painterpath, and what you get if you just use painter. They're different.
 * The font metrics? They must be for martian charaters; human ones fit inside a much smaller box.
 *
 * So. If at first you don't succeed, try and try again.
 */

void Overlay::fixFont() {
    g.s.qfOverlayFont.setStyleStrategy(QFont::ForceOutline);

    g.s.qfOverlayFont.setPixelSize(TEXT_HEIGHT);

    QPainterPath qp;
    qp.addText(0, 0, g.s.qfOverlayFont, QLatin1String("Üy"));
    QRectF br=qp.boundingRect();
    qWarning("Attempt for size %d gave %f %f", TEXT_HEIGHT, br.height(),br.top());

    fFontBase = fabs(br.top());

    foreach(unsigned char *ptr, qhTextures)
    	delete [] ptr;

    qhTextures.clear();
    qhWidths.clear();

    qlCurrentTexts.clear();
}

void Overlay::setTexts(const QList<TextLine> &lines) {
	foreach(TextLine e, lines) {
	    if ((! e.first.isEmpty()) && (! qhTextures.contains(e.first))) {
		unsigned char *td = new unsigned char[TEXTURE_SIZE];
		memset(td, 0, TEXTURE_SIZE);

		QImage qi(td, TEXT_WIDTH, TEXT_HEIGHT, QImage::Format_ARGB32);

		QPainterPath qp;
		qp.addText(0, fFontBase, g.s.qfOverlayFont, e.first);

		QPainter p(&qi);
		p.setRenderHint(QPainter::Antialiasing);
		p.setRenderHint(QPainter::TextAntialiasing);
		p.setBrush(Qt::white);
		p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                p.drawPath(qp);
//		p.setFont(g.s.qfOverlayFont);
//		p.drawText(0, fm.ascent(), e.first);

		qhTextures[e.first] = td;
		qhWidths[e.first] = qMin(static_cast<int>(qp.boundingRect().width())+4, TEXT_WIDTH);
	    }
	}

	if (! sm.tryLock())
		return;

	int i;

	for(i=0;i<lines.count();i++) {
	    	if (i >= NUM_TEXTS)
	    		break;

	    	const TextLine &tl = lines.at(i);
	    	TextEntry *te = & sm.sm->texts[i];

		wcscpy(te->text, reinterpret_cast<const wchar_t *>(tl.first.left(127).utf16()));
	    	te->color = lines[i].second;

		if ((i >= qlCurrentTexts.count()) || (qlCurrentTexts[i].first != tl.first)) {
		    if (tl.first.isNull()) {
		    	te->width = 0;
		    } else {
			memcpy(sm.sm->texts[i].texture, qhTextures[tl.first], TEXTURE_SIZE);
			te->width = qhWidths[tl.first];
			te->bUpdated = true;
		    }
		}
	}

	for(;i<NUM_TEXTS;i++) {
	    sm.sm->texts[i].width = -1;
	}

	qlCurrentTexts = lines;
	sm.unlock();
}
