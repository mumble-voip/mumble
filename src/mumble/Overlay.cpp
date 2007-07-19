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
#include "Message.h"
#include "ServerHandler.h"

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
	qcbEnable->setObjectName(QLatin1String("Enable"));
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
	qcbAlwaysSelf->setObjectName(QLatin1String("AlwaysSelf"));
	qcbAlwaysSelf->setToolTip(tr("Always show yourself on overlay."));
	qcbAlwaysSelf->setWhatsThis(tr("This sets whether to always show yourself or not. "
	                               "This setting is useful if you aren't showing everyone in the overlay, "
	                               "as then you would only see your own status if you were talking, which "
	                               "wouldn't let you see that you were deafened or muted."));
	qcbAlwaysSelf->setChecked(g.s.bOverlayAlwaysSelf);
	grid->addWidget(qcbAlwaysSelf, 2, 0, 1, 2);

	qcbUserTextures = new QCheckBox(tr("Show User Textures"));
	qcbUserTextures->setObjectName(QLatin1String("UserTextures"));
	qcbUserTextures->setToolTip(tr("Show User custom textures instead of text on the overlay."));
	qcbUserTextures->setWhatsThis(tr("This sets whether to download and use custom textures for registered users. "
	                                 "If disabled, the regular outline text will be used instead."));
	qcbUserTextures->setChecked(g.s.bOverlayUserTextures);
	grid->addWidget(qcbUserTextures, 3, 0, 1, 2);


	qgbOptions->setLayout(grid);

	grid=new QGridLayout();

	qcbLeft = new QCheckBox(tr("Grow Left"));
	qcbLeft->setObjectName(QLatin1String("Left"));
	qcbLeft->setToolTip(tr("Let overlay grow to the left"));
	qcbLeft->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
	                         "you have selected. This allows the overlay to grow to the left if "
	                         "needed."));
	qcbLeft->setChecked(g.s.bOverlayLeft);
	qcbRight = new QCheckBox(tr("Grow Right"));
	qcbRight->setObjectName(QLatin1String("Right"));
	qcbRight->setToolTip(tr("Let overlay grow to the Right"));
	qcbRight->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
	                          "you have selected. This allows the overlay to grow to the Right if "
	                          "needed."));
	qcbRight->setChecked(g.s.bOverlayRight);
	qcbTop = new QCheckBox(tr("Grow Up"));
	qcbTop->setObjectName(QLatin1String("Top"));
	qcbTop->setToolTip(tr("Let overlay grow upwards"));
	qcbTop->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
	                        "you have selected. This allows the overlay to grow to the top if "
	                        "needed."));
	qcbTop->setChecked(g.s.bOverlayTop);
	qcbBottom = new QCheckBox(tr("Grow Down"));
	qcbBottom->setObjectName(QLatin1String("Bottom"));
	qcbBottom->setToolTip(tr("Let overlay grow downwards"));
	qcbBottom->setWhatsThis(tr("The overlay tries to stay as small as possible and at the position "
	                           "you have selected. This allows the overlay to grow towards the bottom if "
	                           "needed."));
	qcbBottom->setChecked(g.s.bOverlayBottom);

	qsX = new QSlider(Qt::Horizontal);
	qsX->setObjectName(QLatin1String("X"));
	qsX->setToolTip(tr("X-Position of Overlay"));
	qsX->setWhatsThis(tr("This sets the relative X position of the overlay."));
	qsX->setRange(0, 100);
	qsX->setSingleStep(1);
	qsX->setPageStep(10);
	qsX->setValue(qRound64(g.s.fOverlayX * 100));

	qsY = new QSlider(Qt::Vertical);
	qsY->setObjectName(QLatin1String("Y"));
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
	qpbSetFont->setObjectName(QLatin1String("SetFont"));

	grid->addWidget(lab, 0, 0);
	grid->addWidget(qlCurrentFont, 0, 1);
	grid->addWidget(qpbSetFont, 0, 2);

	qsMaxHeight = new QSlider(Qt::Horizontal);
	qsMaxHeight->setObjectName(QLatin1String("MaxHeight"));
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
	qpbPlayer->setObjectName(QLatin1String("Player"));
	lab = new QLabel(tr("Color for players"));
	setColorLabel(qlPlayer, qcPlayer);

	grid->addWidget(lab, 2, 0);
	grid->addWidget(qlPlayer, 2, 1);
	grid->addWidget(qpbPlayer, 2, 2);

	qlTalking = new QLabel();
	qpbTalking = new QPushButton(tr("Change"));
	qpbTalking->setObjectName(QLatin1String("Talking"));
	lab = new QLabel(tr("Color for talking players"));
	setColorLabel(qlTalking, qcTalking);

	grid->addWidget(lab, 3, 0);
	grid->addWidget(qlTalking, 3, 1);
	grid->addWidget(qpbTalking, 3, 2);

	qlAltTalking = new QLabel();
	qpbAltTalking = new QPushButton(tr("Change"));
	qpbAltTalking->setObjectName(QLatin1String("AltTalking"));
	lab = new QLabel(tr("Color for alt-talking players"));
	setColorLabel(qlAltTalking, qcAltTalking);

	grid->addWidget(lab, 4, 0);
	grid->addWidget(qlAltTalking, 4, 1);
	grid->addWidget(qpbAltTalking, 4, 2);

	qlChannel = new QLabel();
	qpbChannel = new QPushButton(tr("Change"));
	qpbChannel->setObjectName(QLatin1String("Channel"));
	lab = new QLabel(tr("Color for Channels"));
	setColorLabel(qlChannel, qcChannel);

	grid->addWidget(lab, 5, 0);
	grid->addWidget(qlChannel, 5, 1);
	grid->addWidget(qpbChannel, 5, 2);

	qlChannelTalking = new QLabel();
	qpbChannelTalking = new QPushButton(tr("Change"));
	qpbChannelTalking->setObjectName(QLatin1String("ChannelTalking"));
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
	qlMaxHeight->setText(QString::fromLatin1("%1%").arg(v/10.0, 0, 'f', 1));
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

QIcon OverlayConfig::icon() const {
	return QIcon(QLatin1String("skin:config_osd.png"));
}

void OverlayConfig::accept() {
	g.s.bOverlayEnable = qcbEnable->isChecked();
	g.s.osOverlay = static_cast<Settings::OverlayShow>(qcbShow->currentIndex());
	g.s.bOverlayAlwaysSelf = qcbAlwaysSelf->isChecked();
	g.s.bOverlayUserTextures = qcbUserTextures->isChecked();
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

#ifdef Q_OS_WIN
#ifndef QT_NO_DEBUG
	QString path="../overlay/mumble_ol.dll";
#else
	QString path=QString("%1/mumble_ol.dll").arg(qApp->applicationDirPath());
#endif

	qlOverlay->setFileName(path);
	if (! qlOverlay->load()) {
		QMessageBox::critical(NULL, tr("Mumble"), tr("Failed to load overlay library. This means either that:\n"
		                      "- the library (mumble_ol.dll) wasn't found in the directory you ran Mumble from\n"
		                      "- you're on an OS earlier than WinXP SP2\n"
		                      "- you do not have the June 2007 updated version of DX9.0c"), QMessageBox::Ok, QMessageBox::NoButton);
		qWarning("Overlay failure");
	} else {
		sm.resolve(qlOverlay);
	}
#else
	sm.resolve(qlOverlay);
#endif

#ifndef QT_NO_DEBUG
	if (sm.sm)
		sm.sm->bDebug = true;
#else
	if (sm.sm)
		sm.sm->bDebug = false;
#endif

	QImage img;
	img.load(QLatin1String("skin:muted_overlay.png"));
	qbaMuted = QByteArray(reinterpret_cast<const char *>(img.bits()), img.numBytes());

	img.load(QLatin1String("skin:deafened_overlay.png"));
	qbaDeafened = QByteArray(reinterpret_cast<const char *>(img.bits()), img.numBytes());

	qtTimer=new QTimer(this);
	qtTimer->setObjectName(QLatin1String("Timer"));
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

	switch (g.s.osOverlay) {
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

void Overlay::textureResponse(int id, const QByteArray &texture) {
	QString s = qhQueried.value(id);
	if (s.isEmpty())
		return;

	QByteArray t = qUncompress(texture);

	if (t.size() != TEXTURE_SIZE)
		return;

	const unsigned char *data = reinterpret_cast<const unsigned char *>(t.constData());

	int width = 0;
	for (int y=0;y<TEXT_HEIGHT;y++) {
		for (int x=0;x<TEXT_WIDTH; x++) {
			if ((x > width) && (data[(y*TEXT_WIDTH+x)*4] != 0x00))
				width = x;
		}
	}
	qhUserTextures[id] = UserTexture(width, t);
	qsForce.insert(id);
	setTexts(qlCurrentTexts);
}

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

	if (g.uiSession) {
		Channel *home = Player::get(g.uiSession)->cChannel;
		foreach(Channel *c, home->allLinks()) {
			if (home == c)
				continue;

			bool act = false;
			foreach(Player *p, c->qlPlayers) {
				act = act || p->bTalking;
				if (p->bTalking)
					linkchans << qpChanCol(p->qsName + QString::fromLatin1("[") + c->qsName + QString::fromLatin1("]"), colChannelTalking);
			}
			if (! act)
				linkchans << qpChanCol(c->qsName, colChannel);
		}
		qSort(linkchans);
	}

	QList<TextLine> lines;

	if (g.uiSession) {

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

		foreach(Player *p, Player::get(g.uiSession)->cChannel->qlPlayers) {
			if ((g.s.osOverlay == Settings::All) || p->bTalking || ((p == Player::get(g.uiSession)) && g.s.bOverlayAlwaysSelf)) {
				if (g.s.bOverlayUserTextures && (p->iId >= 0) && (! qhQueried.contains(p->iId))) {
					qhQueried.insert(p->iId, p->qsName);
					MessageTexture mt;
					mt.iPlayerId = p->iId;
					g.sh->sendMessage(&mt);
				}
				QString name = p->qsName;
				Decoration dec = None;
				if (p->bDeaf || p->bSelfDeaf)
					dec = Deafened;
				else if (p->bMute || p->bSelfMute || p->bLocalMute)
					dec = Muted;
				lines << TextLine(name, p->bTalking ? (p->bAltSpeak ? colAltTalking : colTalking) : colPlayer, p->iId, dec);
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
	} else {
		qhUserTextures.clear();
		clearCache();
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

	int psize = TEXT_HEIGHT;

	QRectF br;

	do {
		g.s.qfOverlayFont.setPixelSize(psize--);
		QPainterPath qp;
		qp.addText(0, 0, g.s.qfOverlayFont, QLatin1String("Üy"));
		br=qp.boundingRect();
		qWarning("Overlay: Attempt for pixelsize %d gave actual sizes %f %f", psize+1, br.height(),br.top());
	} while ((br.height()+2) > TEXT_HEIGHT);

	fFontBase = fabs(br.top());

	clearCache();

	qlCurrentTexts.clear();
}

void Overlay::clearCache() {
	foreach(unsigned char *ptr, qhTextures)
	delete [] ptr;

	qhTextures.clear();
	qhWidths.clear();
	qhQueried.clear();
}

void Overlay::setTexts(const QList<TextLine> &lines) {
	foreach(const TextLine &e, lines) {
		if ((! e.qsText.isEmpty()) && (! qhTextures.contains(e.qsText)) && (! qhUserTextures.contains(e.iPlayer))) {
			unsigned char *td = new unsigned char[TEXTURE_SIZE];
			memset(td, 0, TEXTURE_SIZE);

			QImage qi(td, TEXT_WIDTH, TEXT_HEIGHT, QImage::Format_ARGB32);

			QPainterPath qp;
			qp.addText(2, fFontBase, g.s.qfOverlayFont, e.qsText);

			QPainter p(&qi);
			p.setRenderHint(QPainter::Antialiasing);
			p.setRenderHint(QPainter::TextAntialiasing);
			p.setBrush(Qt::white);

			// Draw with big border, this will be the "outline"
			p.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			p.drawPath(qp);

			// And again, all white with no border. This avoids thin fonts being just black outline.
			p.setPen(Qt::NoPen);
			p.drawPath(qp);


			qhTextures[e.qsText] = td;
			qhWidths[e.qsText] = qMin(static_cast<int>(qp.boundingRect().width())+6, TEXT_WIDTH);
		}
	}

	if (! sm.tryLock())
		return;

	int i;

	for (i=0;i<lines.count();i++) {
		if (i >= NUM_TEXTS)
			break;

		const TextLine &tl = lines.at(i);
		TextEntry *te = & sm.sm->texts[i];

		wcscpy(te->text, reinterpret_cast<const wchar_t *>(tl.qsText.left(127).utf16()));
		te->color = lines[i].uiColor;

		if ((i >= qlCurrentTexts.count()) || (qlCurrentTexts[i].dDecor != tl.dDecor) || (qlCurrentTexts[i].qsText != tl.qsText) || qsForce.contains(tl.iPlayer)) {
			if (tl.qsText.isNull()) {
				te->width = 0;
			} else {
				int width = 0;
				const unsigned char *src = NULL;

				if (qhUserTextures.contains(tl.iPlayer)) {
					const UserTexture &ut=qhUserTextures.value(tl.iPlayer);
					width = ut.first;
					src = reinterpret_cast<const unsigned char *>(ut.second.constData());
				} else {
					width = qhWidths[tl.qsText];
					src = qhTextures[tl.qsText];
				}

				unsigned char * dst = NULL;

				if (tl.dDecor != None) {
					unsigned char * decdst;
					const unsigned char * decsrc = reinterpret_cast<const unsigned char *>((tl.dDecor == Muted) ? qbaMuted.constData() : qbaDeafened.constData());

					width = qMin(TEXT_WIDTH - TEXT_HEIGHT, width);
					if (g.s.bOverlayLeft) {
						dst = sm.sm->texts[i].texture + TEXT_HEIGHT * 4;
						decdst = sm.sm->texts[i].texture;
					} else {
						dst = sm.sm->texts[i].texture;
						decdst = sm.sm->texts[i].texture + width * 4;
					}
					for (int j=0;j<TEXT_HEIGHT;j++)
						memcpy(decdst + j * TEXT_WIDTH * 4, decsrc + j * TEXT_HEIGHT * 4, TEXT_HEIGHT * 4);

				} else {
					width = qMin(TEXT_WIDTH, width);
					dst = sm.sm->texts[i].texture;
				}

				for (int j=0;j<TEXT_HEIGHT;j++)
					memcpy(dst + j * TEXT_WIDTH * 4, src + j * TEXT_WIDTH * 4, width * 4);

				if (tl.dDecor != None)
					width += TEXT_HEIGHT;

				te->width = width;
				te->bUpdated = true;
			}
			qsForce.remove(tl.iPlayer);
		}
	}

	for (;i<NUM_TEXTS;i++) {
		sm.sm->texts[i].width = -1;
	}

	qlCurrentTexts = lines;
	sm.unlock();
}
