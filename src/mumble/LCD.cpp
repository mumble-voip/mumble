/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008-2009, Mikkel Krautz <mikkel@krautz.dk>

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

#include "LCD.h"
#include "Player.h"
#include "Channel.h"
#include "Global.h"
#include "Message.h"
#include "ServerHandler.h"

QList<LCDEngineNew> *LCDEngineRegistrar::qlInitializers;

LCDEngineRegistrar::LCDEngineRegistrar(LCDEngineNew n) {
	if (! qlInitializers)
		qlInitializers = new QList<LCDEngineNew>();
	this->n = n;
	qlInitializers->append(n);
}

LCDEngineRegistrar::~LCDEngineRegistrar() {
	qlInitializers->removeAll(n);
}

static ConfigWidget *LCDConfigDialogNew(Settings &st) {
	return new LCDConfig(st);
}

class LCDDeviceManager : public DeferInit {
	protected:
		ConfigRegistrar *crLCD;
	public:
		QList<LCDEngine *> qlEngines;
		QList<LCDDevice *> qlDevices;
		void initialize();
		void destroy();
};

void LCDDeviceManager::initialize() {
	if (LCDEngineRegistrar::qlInitializers) {
		foreach (LCDEngineNew engine, *LCDEngineRegistrar::qlInitializers) {
			LCDEngine *e = engine();
			qlEngines.append(e);

			foreach(LCDDevice *d, e->devices()) {
				qlDevices << d;
			}
		}
	}
	if (qlDevices.count() > 0) {
		crLCD = new ConfigRegistrar(5900, LCDConfigDialogNew);
	} else {
		crLCD = NULL;
	}
}

void LCDDeviceManager::destroy() {
	qlDevices.clear();
	foreach (LCDEngine *e, qlEngines) {
		delete e;
	}
	if (crLCD)
		delete crLCD;
}

static LCDDeviceManager devmgr;

/* --- */


LCDConfig::LCDConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	QTreeWidgetItem *qtwi;
	foreach (LCDDevice *d, devmgr.qlDevices) {
		qtwi = new QTreeWidgetItem(qtwDevices);

		qtwi->setFlags(Qt::ItemIsEnabled |Qt::ItemIsUserCheckable);

		qtwi->setText(0, d->name());
		qtwi->setToolTip(0, d->name());

		LCDDevice::Type type = d->type();
		QString qsType;
		if (type == LCDDevice::GraphicLCD)
			qsType = tr("Graphic");
		else if (type == LCDDevice::CharacterLCD)
			qsType = tr("Character");

		qtwi->setText(1, qsType);
		qtwi->setToolTip(1, qsType);

		QSize size = d->size();
		QString qsSize = QString::fromLatin1("%1x%2").arg(size.width()).arg(size.height());
		qtwi->setText(2, qsSize);
		qtwi->setToolTip(2, qsSize);

		qtwi->setCheckState(3, Qt::Unchecked);
		qtwi->setToolTip(3, tr("Enable this device"));
	}
}

bool LCDConfig::expert(bool) {
	return false;
}

QString LCDConfig::title() const {
	return tr("LCD");
}

QIcon LCDConfig::icon() const {
	return QIcon(QLatin1String("skin:config_lcd.png"));
}

void LCDConfig::load(const Settings &r) {
	QList<QTreeWidgetItem *> qlItems = qtwDevices->findItems(QString(), Qt::MatchContains);
	foreach(QTreeWidgetItem *qtwi, qlItems) {
		QString qsName = qtwi->text(0);
		bool enabled = r.qmLCDDevices.contains(qsName) ? r.qmLCDDevices.value(qsName) : true;
		qtwi->setCheckState(3, enabled ? Qt::Checked : Qt::Unchecked);
	}

	loadSlider(qsMinColWidth, r.iLCDPlayerViewMinColWidth);
	loadSlider(qsSplitterWidth, r.iLCDPlayerViewSplitterWidth);
}

void LCDConfig::save() const {
	QList<QTreeWidgetItem *> qlItems = qtwDevices->findItems(QString(), Qt::MatchContains);

	foreach(QTreeWidgetItem *qtwi, qlItems) {
		QString qsName = qtwi->text(0);
		s.qmLCDDevices.insert(qsName, qtwi->checkState(3) == Qt::Checked);
	}

	s.iLCDPlayerViewMinColWidth = qsMinColWidth->value();
	s.iLCDPlayerViewSplitterWidth = qsSplitterWidth->value();
}

void LCDConfig::accept() const {
	foreach (LCDDevice *d, devmgr.qlDevices) {
		bool enabled = s.qmLCDDevices.value(d->name());
		d->setEnabled(enabled);
	}
	g.lcd->updatePlayerView();
}

void LCDConfig::on_qsMinColWidth_valueChanged(int v) {
	qlMinColWidth->setText(QString::number(v));
}

void LCDConfig::on_qsSplitterWidth_valueChanged(int v) {
	qlSplitterWidth->setText(QString::number(v));
}

/* --- */

LCD::LCD() : QObject() {
	qfNormal = QFont(QString::fromLatin1("Arial"), 7);
	qfBold = qfNormal;
	qfBold.setBold(true);
	qfItalic = qfNormal;
	qfItalic.setItalic(true);
	qfItalicBold = qfBold;
	qfItalic.setItalic(true);

	QFontMetrics qfm(qfNormal);

	// Font metrics are broken, and unfortunately means we loose a lot of font space.
//	iFontHeight = qfm.height();
	iFontHeight = 10;

	initBuffers();

	iFrameIndex = 0;

	qtTimer = new QTimer(this);
	connect(qtTimer, SIGNAL(timeout()), this, SLOT(tick()));

	foreach (LCDDevice *d, devmgr.qlDevices) {
		bool enabled = g.s.qmLCDDevices.contains(d->name()) ? g.s.qmLCDDevices.value(d->name()) : true;
		d->setEnabled(enabled);
	}
	qiLogo = QImage(QLatin1String("skin:mumble.48x48.png")).convertToFormat(QImage::Format_MonoLSB);
	updatePlayerView();
}

void LCD::tick() {
	iFrameIndex ++;
	updatePlayerView();
}

void LCD::initBuffers() {
	foreach (LCDDevice *d, devmgr.qlDevices) {
		QSize size = d->size();
		if (! qhImageBuffers.contains(size)) {
			size_t buflen = (size.width() * size.height()) / 8;
			qhImageBuffers[size] = new unsigned char[buflen];
			qhImages[size] = new QImage(qhImageBuffers[size], size.width(), size.height(), QImage::Format_MonoLSB);
		 }
	}
}

void LCD::destroyBuffers() {
	foreach (QImage *img, qhImages)
		delete img;
	qhImages.clear();

	foreach(unsigned char *buf, qhImageBuffers)
		delete buf;
	qhImageBuffers.clear();
}

static bool playerSort(const Player *a, const Player *b) {
	return a->qsName < b->qsName;
}

static bool channelSort(const Channel *home, const Channel *a, const Channel *b) {
	if ((a == home) && (a != b))
		return true;
	else if ((b == home) && (a != b))
		return false;
	return a->qsName < b->qsName;
}

struct ListEntry {
	QString qsString;
	bool bBold;
	bool bItalic;
	ListEntry(const QString &qs, bool bB, bool bI) : qsString(qs), bBold(bB), bItalic(bI) {};
};

void LCD::updatePlayerView() {
	if (qhImages.count() == 0)
		return;

	QStringList qslTalking;
	Player *me = g.uiSession ? ClientPlayer::get(g.uiSession) : NULL;
	Channel *home = me ? me->cChannel : NULL;
	bool alert = false;

	foreach (const QSize &size, qhImages.keys()) {
		QImage *img = qhImages.value(size);
		QPainter painter(img);
		painter.setRenderHints(0, true);
		painter.setPen(Qt::color1);
		painter.setFont(qfNormal);

		img->fill(Qt::color0);

		if (! me) {
			painter.drawImage(0,0,qiLogo);
			painter.drawText(60,20, tr("Not connected"));
			continue;
		}

		QList<Channel *> qlLinks = home->allLinks().toList();

		qSort(qlLinks.begin(), qlLinks.end(), boost::bind(channelSort, home, _1, _2));

		const int iWidth = size.width();
		const int iHeight = size.height();
		const int iPlayersPerColumn = iHeight / iFontHeight;
		const int iSplitterWidth = g.s.iLCDPlayerViewSplitterWidth;

		QList<struct ListEntry> entries;
		QList<struct ListEntry> talking;

		int iCount = 0;

		foreach(Channel *c, qlLinks) {
			QList<Player *> players = c->qlPlayers;
			qSort(players.begin(), players.end(), playerSort);
			if ((players.count() > 0) && (((iCount+1) % iPlayersPerColumn) == 0)) {
				entries << ListEntry(QString(), false, false);
				iCount++;
			}
			entries << ListEntry(QString::fromLatin1("[%1]").arg(c->qsName), false, false);
			iCount++;

			foreach(const Player *p, players) {
				entries << ListEntry(p->qsName, p->bTalking, false);
				iCount++;
				if (p->bTalking) {
					alert = true;
					talking << ListEntry(p->qsName, true, (c != home));
				}
			}
		}

		const int iPlayerColumns = (iCount + iPlayersPerColumn - 1) / iPlayersPerColumn;

		int iColumns = iPlayerColumns;
		int iColumnWidth;

		while (iColumns >= 1) {
			iColumnWidth = (iWidth - (iColumns-1)*iSplitterWidth) / iColumns;
			if (iColumnWidth >= g.s.iLCDPlayerViewMinColWidth)
				break;
			--iColumns;
		}

		if (iColumns == iPlayerColumns) {
			talking = entries;
			if (qtTimer->isActive())
				qtTimer->stop();
		} else {
			if (! qtTimer->isActive())
				qtTimer->start(2000);
			while (talking.count() % iPlayersPerColumn) {
				talking << ListEntry(QString(), false, false);
			}
			int iSkipped = (iPlayerColumns - iColumns + talking.count() / iPlayersPerColumn);
			for(int i=(iFrameIndex % (iSkipped+1))*iPlayersPerColumn;i<entries.count();i++)
				talking << entries.at(i);
		}

		QRect bound;
		int row = 0, col = 0;


		foreach(const ListEntry &le, talking) {
				if (row >= iPlayersPerColumn) {
					row = 0;
					++col;
				}
				if (col > iColumns)
					break;

				if (! le.qsString.isEmpty()) {
					if (le.bBold && le.bItalic)
						painter.setFont(qfItalicBold);
					else if (le.bBold)
						painter.setFont(qfBold);
					else if (le.bItalic)
						painter.setFont(qfItalic);
					else
						painter.setFont(qfNormal);
					painter.drawText(QRect(col * (iColumnWidth  + iSplitterWidth),
										   row * iFontHeight, iColumnWidth, iFontHeight+2), Qt::AlignLeft, le.qsString);
				}
				++row;
		}
	}

	foreach (LCDDevice *d, devmgr.qlDevices) {
		LCDDevice::Type type = d->type();
		if (type == LCDDevice::GraphicLCD) {
			QImage *img = qhImages[d->size()];
			if (! img)
				continue;
			d->blitImage(img, alert);
		} else if (type == LCDDevice::CharacterLCD) {
			/* TODO. */
			qWarning("LCD: Unable to draw to Character LCD.");
		}
	}
}

LCD::~LCD() {
	destroyBuffers();
}

/* --- */

LCDEngine::LCDEngine() : QObject() {
}

LCDEngine::~LCDEngine() {
}

LCDDevice::LCDDevice() {
}

LCDDevice::~LCDDevice() {
}

/* --- */

uint qHash(const QSize &size) {
	return ((size.width() & 0xffff) << 16) | (size.height() & 0xffff);
}
