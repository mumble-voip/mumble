/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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
#include "User.h"
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

	loadSlider(qsX, iroundf(r.fOverlayX * 100.0f));
	loadSlider(qsY, 100 - iroundf(r.fOverlayY * 100.0f));
	qfFont = r.qfOverlayFont;
	qcUser = r.qcOverlayUser;
	qcTalking= r.qcOverlayTalking;
	qcWhisper= r.qcOverlayWhisper;
	qcChannel = r.qcOverlayChannel;
	qcChannelTalking = r.qcOverlayChannelTalking;
	qlCurrentFont->setText(qfFont.family());
	loadSlider(qsMaxHeight, iroundf(r.fOverlayHeight * 1000.0f));
	setColorLabel(qlUser, qcUser);
	setColorLabel(qlTalking, qcTalking);
	setColorLabel(qlWhisper, qcWhisper);
	setColorLabel(qlChannel, qcChannel);
	setColorLabel(qlChannelTalking, qcChannelTalking);
}

bool OverlayConfig::expert(bool b) {
	qcbUserTextures->setVisible(b);

	qcbLeft->setVisible(b);
	qcbRight->setVisible(b);
	qcbTop->setVisible(b);
	qcbBottom->setVisible(b);

	qgbFont->setVisible(b);
	return true;
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

#if QT_VERSION >= 0x040500
#define UPDCOLOR(var, text) { QColor col = QColorDialog::getColor(qc##var, this, text, QColorDialog::ShowAlphaChannel); if (col.isValid()) { qc##var = col; setColorLabel(ql##var, qc##var); } }
#else
#define UPDCOLOR(var, text) { QColor col = QColorDialog::getColor(qc##var, this); if (col.isValid()) { qc##var = col; setColorLabel(ql##var, qc##var); } }
#endif

void OverlayConfig::on_qpbUser_clicked() {
	UPDCOLOR(User, tr("Color for users"));
}

void OverlayConfig::on_qpbTalking_clicked() {
	UPDCOLOR(Talking, tr("Color for talking users"));
}

void OverlayConfig::on_qpbWhisper_clicked() {
	UPDCOLOR(Whisper, tr("Color for whispering users"));
}

void OverlayConfig::on_qpbChannel_clicked() {
	UPDCOLOR(Channel, tr("Color for channels"));
}

void OverlayConfig::on_qpbChannelTalking_clicked() {
	UPDCOLOR(ChannelTalking, tr("Color for active channels"));
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
	s.fOverlayX = static_cast<float>(qsX->value()) / 100.0f;
	s.fOverlayY = 1.0f - static_cast<float>(qsY->value()) / 100.0f;
	s.bOverlayLeft = qcbLeft->isChecked();
	s.bOverlayRight = qcbRight->isChecked();
	s.bOverlayTop = qcbTop->isChecked();
	s.bOverlayBottom = qcbBottom->isChecked();
	s.qfOverlayFont = qfFont;
	s.fOverlayHeight = static_cast<float>(qsMaxHeight->value()) / 1000.0f;
	s.qcOverlayUser = qcUser;
	s.qcOverlayTalking = qcTalking;
	s.qcOverlayWhisper = qcWhisper;
	s.qcOverlayChannel = qcChannel;
	s.qcOverlayChannelTalking = qcChannelTalking;
}

void OverlayConfig::accept() const {
	g.o->forceSettings();
	g.o->setActive(s.bOverlayEnable);
}

bool Overlay::TextLine::operator <(const Overlay::TextLine &other) const {
	if (iPriority < other.iPriority)
		return true;
	else if (iPriority > other.iPriority)
		return false;

	return QString::localeAwareCompare(qsText, other.qsText) < 0;
}

Overlay::Overlay() : QObject() {
	d = NULL;
	qlOverlay = new QLibrary(this);

#ifdef Q_OS_WIN
	QString path=QString::fromLatin1("%1/mumble_ol.dll").arg(qApp->applicationDirPath());

	qlOverlay->setFileName(path);
	if (! qlOverlay->load()) {
		QMessageBox::critical(NULL, tr("Mumble"), tr("Failed to load overlay library. This means either that:\n"
		                      "- the library (mumble_ol.dll) wasn't found in the directory you ran Mumble from\n"
		                      "- you're on an OS earlier than WinXP SP2"), QMessageBox::Ok, QMessageBox::NoButton);
		qWarning("Overlay failure");
	} else {
		sm.resolve(qlOverlay);
	}
#else
	sm.resolve(qlOverlay);
#endif

	if (! sm.sm) {
		QMessageBox::warning(NULL, tr("Mumble"), tr("Failed to initialize overlay memory. This usually means that the shared memory is "
		                     "locked by the OS, and you need to reboot to release it."), QMessageBox::Ok, QMessageBox::NoButton);
	} else {
#ifndef QT_NO_DEBUG
		sm.sm->bDebug = true;
#else
		sm.sm->bDebug = false;
#endif
		sm.sm->version[0] = OVERLAY_VERSION_MAJ;
		sm.sm->version[1] = OVERLAY_VERSION_MIN;
		sm.sm->version[2] = OVERLAY_VERSION_PATCH;
		sm.sm->version[3] = OVERLAY_VERSION_SUB;
	}

	QImage img;
	img = QIcon(QLatin1String("skin:muted_self.svg")).pixmap(60,60).toImage();
	qbaMuted = QByteArray(reinterpret_cast<const char *>(img.bits()), img.numBytes());

	img = QIcon(QLatin1String("skin:deafened_self.svg")).pixmap(60,60).toImage();
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
	if (d)
		delete d;
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

	if (! sm.sm)
		return;

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

void Overlay::verifyTexture(ClientUser *cp) {
	qsForce.insert(cp->uiSession);
	if (! cp->qbaTexture.isEmpty() && (cp->qbaTexture.size() != TEXTURE_SIZE)) {
		cp->qbaTexture = qUncompress(cp->qbaTexture);
		if (cp->qbaTexture.size() != TEXTURE_SIZE)
			cp->qbaTexture = QByteArray();
	}
	if (cp->qbaTexture.isEmpty()) {
		cp->iTextureWidth = 0;
		return;
	}

	unsigned char *data = reinterpret_cast<unsigned char *>(cp->qbaTexture.data());
	int width = 0;
	for (int y=0;y<TEXT_HEIGHT;++y) {
		for (int x=0;x<TEXT_WIDTH; ++x) {
			if ((x > width) && (data[(y*TEXT_WIDTH+x)*4] != 0x00))
				width = x;
		}
	}
	cp->iTextureWidth = width;
}

typedef QPair<QString, quint32> qpChanCol;

void Overlay::updateOverlay() {
	quint32 colUser = g.s.qcOverlayUser.rgba();
	quint32 colTalking = g.s.qcOverlayTalking.rgba();
	quint32 colWhisper = g.s.qcOverlayWhisper.rgba();
	quint32 colChannel = g.s.qcOverlayChannel.rgba();
	quint32 colChannelTalking = g.s.qcOverlayChannelTalking.rgba();
	QString str;
	QList<qpChanCol> linkchans;

	if (! isActive())
		return;

	if (g.uiSession) {
		Channel *home = ClientUser::get(g.uiSession)->cChannel;
		foreach(Channel *c, home->allLinks()) {
			if (home == c)
				continue;

			bool act = false;
			foreach(User *p, c->qlUsers) {
				ClientUser *u = static_cast<ClientUser *>(p);
				bool talking = (u->tsState != ClientUser::TalkingOff);
				act = act || talking;
				if (talking)
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
					lines << TextLine(cc.first, cc.second, 0);
				}
			}
			if (linkchans.count() > 0) {
				lines << TextLine(QString(), 0, 0);
			}
		}

		foreach(User *p, ClientUser::get(g.uiSession)->cChannel->qlUsers) {
			ClientUser *u = static_cast<ClientUser *>(p);
			if ((g.s.osOverlay == Settings::All) || (u->tsState != ClientUser::TalkingOff) || ((u == ClientUser::get(g.uiSession)) && g.s.bOverlayAlwaysSelf)) {
				QString name = u->qsName;
				Decoration dec = None;
				if (u->bDeaf || u->bSelfDeaf)
					dec = Deafened;
				else if (u->bMute || u->bSelfMute || u->bLocalMute)
					dec = Muted;
				quint32 col;
				switch (u->tsState) {
					case ClientUser::TalkingOff:
						col = colUser;
						break;
					case ClientUser::Talking:
						col = colTalking;
						break;
					default:
						col = colWhisper;
						break;
				}
				lines << TextLine(name, col, 1, u->uiSession, dec);
			}
		}

		if (! g.s.bOverlayTop) {
			if (linkchans.count() > 0) {
				lines << TextLine(QString(), 0, 2);
			}
			foreach(qpChanCol cc, linkchans) {
				if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
					lines << TextLine(cc.first, cc.second, 2);
				}
			}
		}
	} else {
		clearCache();
	}
	qSort(lines);
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

	fFontBase = static_cast<float>(fabs(br.top()));

	clearCache();

	qlCurrentTexts.clear();
}

void Overlay::clearCache() {
	foreach(unsigned char *ptr, qhTextures)
		delete [] ptr;

	qhTextures.clear();
	qhWidths.clear();
	qsForce.clear();
	qsQueried.clear();
}

void Overlay::setTexts(const QList<TextLine> &lines) {
	QSet<unsigned int> query;

	foreach(const TextLine &e, lines) {
		ClientUser *cp = ClientUser::get(e.uiSession);
		if (g.s.bOverlayUserTextures && cp && ! cp->qbaTextureHash.isEmpty()) {
			if (cp->qbaTexture.isEmpty() && ! qsQueried.contains(cp->uiSession))
				query.insert(cp->uiSession);
			else if (! cp->qbaTexture.isEmpty() && qsQueried.contains(cp->uiSession))
				qsQueried.remove(cp->uiSession);
		}
		if ((! e.qsText.isEmpty()) && (! qhTextures.contains(e.qsText)) && !(g.s.bOverlayUserTextures && cp && cp->iTextureWidth)) {
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
			qhWidths[e.qsText] = qMin(static_cast<short>(qp.boundingRect().width()+6), static_cast<short>(TEXT_WIDTH));
		}
	}

	if (! query.isEmpty()) {
		MumbleProto::RequestBlob mprb;
		foreach(unsigned int session, query) {
			qsQueried.insert(session);
			mprb.add_session_texture(session);
		}
		g.sh->sendMessage(mprb);
	}

	if (! sm.sm || ! sm.tryLock())
		return;

	int i;

	for (i=0;i<lines.count();i++) {
		if (i >= NUM_TEXTS)
			break;

		const TextLine &tl = lines.at(i);
		TextEntry *te = & sm.sm->texts[i];

		tl.qsText.left(127).toWCharArray(te->text);
		te->color = lines[i].uiColor;

		if ((i >= qlCurrentTexts.count()) || (qlCurrentTexts[i].dDecor != tl.dDecor) || (qlCurrentTexts[i].qsText != tl.qsText) || qsForce.contains(tl.uiSession)) {
			if (tl.qsText.isNull()) {
				te->width = 0;
			} else {
				int width = 0;
				const unsigned char *src = NULL;
				ClientUser *cp = ClientUser::get(tl.uiSession);

				if (g.s.bOverlayUserTextures && cp && cp->iTextureWidth) {
					width = cp->iTextureWidth;
					src = reinterpret_cast<const unsigned char *>(cp->qbaTexture.constData());
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

				te->width = static_cast<short>(width);
				te->uiCounter++;
			}
			qsForce.remove(tl.uiSession);
		}
	}

	for (;i<NUM_TEXTS;i++) {
		sm.sm->texts[i].width = -1;
	}

	qlCurrentTexts = lines;
	sm.unlock();
}
