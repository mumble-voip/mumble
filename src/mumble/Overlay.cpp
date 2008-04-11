/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#include "Overlay.h"
#include "Player.h"
#include "Channel.h"
#include "Global.h"
#include "Message.h"
#include "ServerHandler.h"

static ConfigWidget *OverlayConfigDialogNew(Settings &st) {
	return new OverlayConfig(st);
}

static ConfigRegistrar registrar(6000, OverlayConfigDialogNew);

OverlayConfig::OverlayConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);


	qcbShow->addItem(tr("Show no one"), Settings::Nothing);
	qcbShow->addItem(tr("Show only talking"), Settings::Talking);
	qcbShow->addItem(tr("Show everyone"), Settings::All);

//	on_qsMaxHeight_valueChanged(qsMaxHeight->value());
}

void OverlayConfig::load(const Settings &r) {
	loadCheckBox(qcbEnable, r.bOverlayEnable);
	loadCheckBox(qcbAlwaysSelf, r.bOverlayAlwaysSelf);
	loadCheckBox(qcbUserTextures, r.bOverlayUserTextures);
	loadComboBox(qcbShow, r.osOverlay);

	loadCheckBox(qcbLeft, r.bOverlayLeft);
	loadCheckBox(qcbRight, r.bOverlayRight);
	loadCheckBox(qcbTop, r.bOverlayTop);
	loadCheckBox(qcbBottom, r.bOverlayBottom);

	loadSlider(qsX, qRound64(r.fOverlayX * 100));
	loadSlider(qsY, 100 - qRound64(r.fOverlayY * 100));
	qfFont = r.qfOverlayFont;
	qcPlayer = r.qcOverlayPlayer;
	qcTalking= r.qcOverlayTalking;
	qcAltTalking= r.qcOverlayAltTalking;
	qcChannel = r.qcOverlayChannel;
	qcChannelTalking = r.qcOverlayChannelTalking;
	qlCurrentFont->setText(qfFont.family());
	loadSlider(qsMaxHeight, qRound64(r.fOverlayHeight * 1000));
	setColorLabel(qlPlayer, qcPlayer);
	setColorLabel(qlTalking, qcTalking);
	setColorLabel(qlAltTalking, qcAltTalking);
	setColorLabel(qlChannel, qcChannel);
	setColorLabel(qlChannelTalking, qcChannelTalking);
}

bool OverlayConfig::expert(bool) {
	return false;
}

void OverlayConfig::setColorLabel(QLabel *label, QColor col) {
	label->setText(col.name());

	QPalette palette;
	palette.setColor(label->foregroundRole(), col);
	label->setPalette(palette);
}

void OverlayConfig::on_qpbSetFont_clicked() {
	bool ok;
	QFont qf = QFontDialog::getFont(&ok, qfFont, this);
	if (ok) {
		qfFont = qf;
		qlCurrentFont->setText(qfFont.family());
		qsMaxHeight->setValue(qfFont.pointSize());
	}
}

void OverlayConfig::on_qsMaxHeight_valueChanged(int v) {
	qlMaxHeight->setText(QString::fromLatin1("%1%").arg(v/10.0, 0, 'f', 1));
	qfFont.setPointSize(v);
}


void OverlayConfig::on_qpbPlayer_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcPlayer.rgba(), &ok, this);
	if (ok) {
		qcPlayer = QColor::fromRgba(rgb);
		setColorLabel(qlPlayer, qcPlayer);
	}
}

void OverlayConfig::on_qpbTalking_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcTalking.rgba(), &ok, this);
	if (ok) {
		qcTalking = QColor::fromRgba(rgb);
		setColorLabel(qlTalking, qcTalking);
	}
}

void OverlayConfig::on_qpbAltTalking_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcAltTalking.rgba(), &ok, this);
	if (ok) {
		qcAltTalking = QColor::fromRgba(rgb);
		setColorLabel(qlAltTalking, qcAltTalking);
	}
}

void OverlayConfig::on_qpbChannel_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcChannel.rgba(), &ok, this);
	if (ok) {
		qcChannel = QColor::fromRgba(rgb);
		setColorLabel(qlChannel, qcChannel);
	}
}

void OverlayConfig::on_qpbChannelTalking_clicked() {
	bool ok;
	QRgb rgb=QColorDialog::getRgba(qcChannelTalking.rgba(), &ok, this);
	if (ok) {
		qcChannelTalking = QColor::fromRgba(rgb);
		setColorLabel(qlChannelTalking, qcChannelTalking);
	}
}

QString OverlayConfig::title() const {
	return tr("Overlay");
}

QIcon OverlayConfig::icon() const {
	return QIcon(QLatin1String("skin:config_osd.png"));
}

void OverlayConfig::save() const {
	s.bOverlayEnable = qcbEnable->isChecked();
	s.osOverlay = static_cast<Settings::OverlayShow>(qcbShow->currentIndex());
	s.bOverlayAlwaysSelf = qcbAlwaysSelf->isChecked();
	s.bOverlayUserTextures = qcbUserTextures->isChecked();
	s.fOverlayX = qsX->value() / 100.0;
	s.fOverlayY = 1.0 - qsY->value() / 100.0;
	s.bOverlayLeft = qcbLeft->isChecked();
	s.bOverlayRight = qcbRight->isChecked();
	s.bOverlayTop = qcbTop->isChecked();
	s.bOverlayBottom = qcbBottom->isChecked();
	s.qfOverlayFont = qfFont;
	s.fOverlayHeight = qsMaxHeight->value() / 1000.0;
	s.qcOverlayPlayer = qcPlayer;
	s.qcOverlayTalking = qcTalking;
	s.qcOverlayAltTalking = qcAltTalking;
	s.qcOverlayChannel = qcChannel;
	s.qcOverlayChannelTalking = qcChannelTalking;
}

void OverlayConfig::accept() const {
	g.o->forceSettings();
	g.o->setActive(s.bOverlayEnable);
}

Overlay::Overlay() : QObject() {
	qlOverlay = new QLibrary(this);

#ifdef Q_OS_WIN
	QString path=QString::fromLatin1("%1/mumble_ol.dll").arg(qApp->applicationDirPath());

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

	if (sm.sm) {
		sm.sm->version[0] = OVERLAY_VERSION_MAJ;
		sm.sm->version[1] = OVERLAY_VERSION_MIN;
		sm.sm->version[2] = OVERLAY_VERSION_PATCH;
		sm.sm->version[3] = OVERLAY_VERSION_SUB;
	}

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
		Channel *home = ClientPlayer::get(g.uiSession)->cChannel;
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

		foreach(Player *p, ClientPlayer::get(g.uiSession)->cChannel->qlPlayers) {
			if ((g.s.osOverlay == Settings::All) || p->bTalking || ((p == ClientPlayer::get(g.uiSession)) && g.s.bOverlayAlwaysSelf)) {
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
	qfFont = g.s.qfOverlayFont;

	qfFont.setStyleStrategy(QFont::ForceOutline);

	int psize = TEXT_HEIGHT;

	QRectF br;

	do {
		qfFont.setPixelSize(psize--);
		QPainterPath qp;
		qp.addText(0, 0, qfFont, QLatin1String("Üy"));
		br=qp.boundingRect();
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
			qp.addText(2, fFontBase, qfFont, e.qsText);

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

		tl.qsText.left(127).toWCharArray(te->text);
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
