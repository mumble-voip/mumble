/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2008, Mikkel Krautz <mikkel@krautz.dk>

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

class LCDDeviceManager : public DeferInit {
	public:
		QList<LCDEngine *> qlEngines;
		QList<LCDDevice *> qlDevices;
		void initialize();
		void destroy();
};

#ifdef NO_LCD
void LCDDeviceManager::initialize() {}
void LCDDeviceManager::destroy() {}
#else
void LCDDeviceManager::initialize() {
	foreach (LCDEngineNew engine, *LCDEngineRegistrar::qlInitializers) {
		LCDEngine *e = engine();
		qlEngines.append(e);

		foreach(LCDDevice *d, e->devices()) {
			qlDevices << d;
		}
	}
}

void LCDDeviceManager::destroy() {
	qlDevices.clear();
	foreach (LCDEngine *e, qlEngines) {
		delete e;
	}
}
#endif

static LCDDeviceManager devmgr;

/* --- */

static ConfigWidget *LCDConfigDialogNew(Settings &st) {
	return new LCDConfig(st);
}

static ConfigRegistrar registrar(5900, LCDConfigDialogNew);

LCDConfig::LCDConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	qtwDevices->header()->setResizeMode(0, QHeaderView::Stretch);
	qtwDevices->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	qtwDevices->header()->setResizeMode(2, QHeaderView::ResizeToContents);
	qtwDevices->header()->setResizeMode(3, QHeaderView::ResizeToContents);

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

	qcbCurrentView->addItem(tr("Player View"), Settings::PlayerView);
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
	loadCheckBox(qcbEnable, r.bLCDEnable);

	QList<QTreeWidgetItem *> qlItems = qtwDevices->findItems(QString(), Qt::MatchContains);
	foreach(QTreeWidgetItem *qtwi, qlItems) {
		QString qsName = qtwi->text(0);
		bool enabled = r.qslLCDEnabledDevices.contains(qsName);
		qtwi->setCheckState(3, enabled ? Qt::Checked : Qt::Unchecked);
	}

	loadComboBox(qcbCurrentView, r.lvView);
	loadCheckBox(qcbAlwaysSelf, r.bLCDPlayerViewSelf);
	loadSlider(qsMinColWidth, r.iLCDPlayerViewMinColWidth);
	loadSlider(qsSplitterPadding, r.iLCDPlayerViewSplitterPadding);
	loadSlider(qsSplitterWidth, r.iLCDPlayerViewSplitterWidth);
}

void LCDConfig::save() const {
	s.bLCDEnable = qcbEnable->isChecked();

	s.qslLCDEnabledDevices.clear();
	QList<QTreeWidgetItem *> qlItems = qtwDevices->findItems(QString(), Qt::MatchContains);

	foreach(QTreeWidgetItem *qtwi, qlItems) {
		QString qsName = qtwi->text(0);
		if (qtwi->checkState(3) == Qt::Checked)
			s.qslLCDEnabledDevices << qsName;
	}

	s.lvView = static_cast<Settings::LCDView>(qcbCurrentView->currentIndex());
	s.bLCDPlayerViewSelf = qcbAlwaysSelf->isChecked();
	s.iLCDPlayerViewMinColWidth = qsMinColWidth->value();
	s.iLCDPlayerViewSplitterPadding = qsSplitterPadding->value();
	s.iLCDPlayerViewSplitterWidth = qsSplitterWidth->value();
}

void LCDConfig::accept() const {
	g.lcd->setEnabled(s.bLCDEnable);

	foreach (LCDDevice *d, devmgr.qlDevices) {
		bool enabled = s.qslLCDEnabledDevices.contains(d->name());
		d->setEnabled(enabled);
	}
}

void LCDConfig::on_qsMinColWidth_valueChanged(int v) {
	qlMinColWidth->setText(QString::number(v));
}

void LCDConfig::on_qsSplitterPadding_valueChanged(int v) {
	qlSplitterPadding->setText(QString::number(v));
}

void LCDConfig::on_qsSplitterWidth_valueChanged(int v) {
	qlSplitterWidth->setText(QString::number(v));
}

void LCDConfig::on_qcbCurrentView_currentIndexChanged(int v) {
	switch (v) {
		case Settings::PlayerView:
			qswView->setCurrentWidget(qwPlayerView);
			break;
	}
}

/* --- */

LCD::LCD() : QObject() {
	qfNormal = QFont(QString::fromLatin1("Arial"), 7);
	QFontMetrics qfm(qfNormal);
	iFontHeight = qfm.height();

	setEnabled(g.s.bLCDEnable);
	foreach (LCDDevice *d, devmgr.qlDevices) {
		bool enabled = g.s.qslLCDEnabledDevices.contains(d->name());
		d->setEnabled(enabled);
	}
}

void LCD::setEnabled(bool e) {
	if (e)
		initBuffers();
	else
		destroyBuffers();
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

void LCD::updatePlayerView() {

	if (!g.s.bLCDEnable || g.uiSession == 0)
		return;

	QStringList qslTalking;
	Player *me = ClientPlayer::get(g.uiSession);
	Channel *home = me->cChannel;
	foreach (Channel *c, home->allLinks()) {
		foreach (Player *p, c->qlPlayers) {
			if (p == me && !g.s.bLCDPlayerViewSelf)
				continue;
			if (p->bTalking)
				qslTalking << p->qsName;
		}
	}
	qSort(qslTalking);

	foreach (QSize size, qhImages.keys()) {
		QImage *img = qhImages[size];
		if (img == NULL)
			continue;

		QPainter painter(img);

		img->fill(Qt::color0);

		if (! qslTalking.size())
			continue;

		painter.setRenderHints(0, true);
		painter.setPen(Qt::color1);
		painter.setFont(qfNormal);

		const int iWidth = size.width();
		const int iHeight = size.height();
		const int iNumTalking = qslTalking.size();
		const int iPlayersPerColumn = iHeight / iFontHeight;
		const int iSplitterPadding = g.s.iLCDPlayerViewSplitterPadding;
		const int iSplitterWidth = g.s.iLCDPlayerViewSplitterWidth;

		int iColumns = iNumTalking / iPlayersPerColumn + !!(iNumTalking % iPlayersPerColumn);
		int iColumnWidth;

		while (iColumns >= 1) {
			iColumnWidth = (iWidth - (iColumns-1)*((2*iSplitterPadding) + iSplitterWidth)) / iColumns;
			if (iColumnWidth >= g.s.iLCDPlayerViewMinColWidth)
				break;
			--iColumns;
		}

		QRect bound;
		int row = 0, col = 0;
		foreach (QString qsName, qslTalking) {
			if (row >= iPlayersPerColumn) {
				row = 0;
				++col;
			}
			if (col >= iColumns)
				break;
			else if (col > 0) {
				int x = col * (iColumnWidth + (iSplitterPadding * 2 + iSplitterWidth))  - iSplitterPadding - iSplitterWidth;
				painter.fillRect(QRect(x, 0, iSplitterWidth, iHeight), Qt::color1);
			}

			painter.drawText(QRect(col * (iColumnWidth  + iSplitterPadding * 2 + iSplitterWidth),
			                       row * iFontHeight, iColumnWidth, iFontHeight), Qt::AlignLeft, qsName);
			++row;
		}
	}

	foreach (LCDDevice *d, devmgr.qlDevices) {
		LCDDevice::Type type = d->type();
		if (type == LCDDevice::GraphicLCD) {
			QImage *img = qhImages[d->size()];
			if (! img)
				continue;
			d->blitImage(img);
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
