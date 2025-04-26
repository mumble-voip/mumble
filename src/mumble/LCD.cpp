// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LCD.h"

#include "Channel.h"
#include "ClientUser.h"
#include "ServerHandler.h"
#include "Utils.h"
#include "Global.h"

#include <QtGui/QPainter>

const QString LCDConfig::name = QLatin1String("LCDConfig");

QList< LCDEngineNew > *LCDEngineRegistrar::qlInitializers;

LCDEngineRegistrar::LCDEngineRegistrar(LCDEngineNew cons) {
	if (!qlInitializers)
		qlInitializers = new QList< LCDEngineNew >();
	n = cons;
	qlInitializers->append(n);
}

LCDEngineRegistrar::~LCDEngineRegistrar() {
	qlInitializers->removeAll(n);
	if (qlInitializers->isEmpty()) {
		delete qlInitializers;
		qlInitializers = nullptr;
	}
}

static ConfigWidget *LCDConfigDialogNew(Settings &st) {
	return new LCDConfig(st);
}

class LCDDeviceManager : public DeferInit {
protected:
	ConfigRegistrar *crLCD;

public:
	QList< LCDEngine * > qlEngines;
	QList< LCDDevice * > qlDevices;
	void initialize();
	void destroy();
};

void LCDDeviceManager::initialize() {
	if (LCDEngineRegistrar::qlInitializers) {
		foreach (LCDEngineNew engine, *LCDEngineRegistrar::qlInitializers) {
			LCDEngine *e = engine();
			qlEngines.append(e);

			foreach (LCDDevice *d, e->devices()) { qlDevices << d; }
		}
	}
	if (qlDevices.count() > 0) {
		crLCD = new ConfigRegistrar(5900, LCDConfigDialogNew);
	} else {
		crLCD = nullptr;
	}
}

void LCDDeviceManager::destroy() {
	qlDevices.clear();
	foreach (LCDEngine *e, qlEngines) { delete e; }
	delete crLCD;
}

static LCDDeviceManager devmgr;

/* --- */


LCDConfig::LCDConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	QTreeWidgetItem *qtwi;
	foreach (LCDDevice *d, devmgr.qlDevices) {
		qtwi = new QTreeWidgetItem(qtwDevices);

		qtwi->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

		qtwi->setText(0, d->name());
		qtwi->setToolTip(0, d->name().toHtmlEscaped());

		QSize lcdsize  = d->size();
		QString qsSize = QString::fromLatin1("%1x%2").arg(lcdsize.width()).arg(lcdsize.height());
		qtwi->setText(1, qsSize);
		qtwi->setToolTip(1, qsSize);

		qtwi->setCheckState(2, Qt::Unchecked);
		qtwi->setToolTip(2, tr("Enable this device"));
	}
}

QString LCDConfig::title() const {
	return tr("LCD");
}

const QString &LCDConfig::getName() const {
	return LCDConfig::name;
}

QIcon LCDConfig::icon() const {
	return QIcon(QLatin1String("skin:config_lcd.png"));
}

void LCDConfig::load(const Settings &r) {
	QList< QTreeWidgetItem * > qlItems = qtwDevices->findItems(QString(), Qt::MatchContains);
	foreach (QTreeWidgetItem *qtwi, qlItems) {
		QString qsName = qtwi->text(0);
		bool enabled   = r.qmLCDDevices.contains(qsName) ? r.qmLCDDevices.value(qsName) : true;
		qtwi->setCheckState(2, enabled ? Qt::Checked : Qt::Unchecked);
	}

	loadSlider(qsMinColWidth, r.iLCDUserViewMinColWidth);
	loadSlider(qsSplitterWidth, r.iLCDUserViewSplitterWidth);
}

void LCDConfig::save() const {
	QList< QTreeWidgetItem * > qlItems = qtwDevices->findItems(QString(), Qt::MatchContains);

	foreach (QTreeWidgetItem *qtwi, qlItems) {
		QString qsName = qtwi->text(0);
		s.qmLCDDevices.insert(qsName, qtwi->checkState(2) == Qt::Checked);
	}

	s.iLCDUserViewMinColWidth   = qsMinColWidth->value();
	s.iLCDUserViewSplitterWidth = qsSplitterWidth->value();
}

void LCDConfig::accept() const {
	foreach (LCDDevice *d, devmgr.qlDevices) {
		bool enabled = s.qmLCDDevices.value(d->name());
		d->setEnabled(enabled);
	}
	Global::get().lcd->updateUserView();
}

void LCDConfig::on_qsMinColWidth_valueChanged(int v) {
	qlMinColWidth->setText(QString::number(v));
}

void LCDConfig::on_qsSplitterWidth_valueChanged(int v) {
	qlSplitterWidth->setText(QString::number(v));
}

/* --- */

LCD::LCD() : QObject() {
#ifdef Q_OS_MAC
	qfNormal.setStyleStrategy(QFont::NoAntialias);
	qfNormal.setKerning(false);
	qfNormal.setPointSize(10);
	qfNormal.setFixedPitch(true);
	qfNormal.setFamily(QString::fromLatin1("Andale Mono"));
#else
	qfNormal = QFont(QString::fromLatin1("Arial"), 7);
#endif

	qfItalic = qfNormal;
	qfItalic.setItalic(true);

	qfBold = qfNormal;
	qfBold.setWeight(QFont::Black);

	qfItalicBold = qfBold;
	qfItalic.setItalic(true);

	QFontMetrics qfm(qfNormal);

	iFontHeight = 10;

	initBuffers();

	iFrameIndex = 0;

	qtTimer = new QTimer(this);
	connect(qtTimer, SIGNAL(timeout()), this, SLOT(tick()));

	foreach (LCDDevice *d, devmgr.qlDevices) {
		bool enabled =
			Global::get().s.qmLCDDevices.contains(d->name()) ? Global::get().s.qmLCDDevices.value(d->name()) : true;
		d->setEnabled(enabled);
	}
	qiLogo = QIcon(QLatin1String("skin:mumble.svg")).pixmap(48, 48).toImage().convertToFormat(QImage::Format_MonoLSB);

	qiLogo.invertPixels();

	updateUserView();
}

void LCD::tick() {
	iFrameIndex++;
	updateUserView();
}

void LCD::initBuffers() {
	foreach (LCDDevice *d, devmgr.qlDevices) {
		QSize size = d->size();
		if (!qhImageBuffers.contains(size)) {
			size_t buflen        = static_cast< std::size_t >(size.width() * size.height()) / 8;
			qhImageBuffers[size] = new unsigned char[buflen];
			qhImages[size] = new QImage(qhImageBuffers[size], size.width(), size.height(), QImage::Format_MonoLSB);
		}
	}
}

void LCD::destroyBuffers() {
	foreach (QImage *img, qhImages)
		delete img;
	qhImages.clear();

	foreach (unsigned char *buf, qhImageBuffers)
		delete[] buf;
	qhImageBuffers.clear();
}

struct ListEntry {
	QString qsString;
	bool bBold;
	bool bItalic;
	ListEntry(const QString &qs, bool bB, bool bI) : qsString(qs), bBold(bB), bItalic(bI){};
};

static bool entriesSort(const ListEntry &a, const ListEntry &b) {
	return a.qsString < b.qsString;
}

void LCD::updateUserView() {
	if (qhImages.count() == 0)
		return;

	QStringList qslTalking;
	User *me      = Global::get().uiSession ? ClientUser::get(Global::get().uiSession) : nullptr;
	Channel *home = me ? me->cChannel : nullptr;
	bool alert    = false;

	foreach (const QSize &size, qhImages.keys()) {
		QImage *img = qhImages.value(size);
		QPainter painter(img);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, false);

		painter.setPen(Qt::color1);

		painter.setFont(qfNormal);

		img->fill(Qt::color0);

		if (!me) {
			qmNew.clear();
			qmOld.clear();
			qmSpeaking.clear();
			qmNameCache.clear();
			painter.drawImage(0, 0, qiLogo);
			painter.drawText(60, 20, tr("Not connected"));
			continue;
		}

		foreach (User *p, me->cChannel->qlUsers) {
			if (!qmNew.contains(p->uiSession)) {
				qmNew.insert(p->uiSession, Timer());
				qmNameCache.insert(p->uiSession, p->qsName);
				qmOld.remove(p->uiSession);
			}
		}

		foreach (unsigned int session, qmNew.keys()) {
			User *p = ClientUser::get(session);
			if (!p || (p->cChannel != me->cChannel)) {
				qmNew.remove(session);
				qmOld.insert(session, Timer());
			}
		}

		QMap< unsigned int, Timer > old;

		foreach (unsigned int session, qmOld.keys()) {
			Timer t = qmOld.value(session);
			if (t.elapsed() > 3000000) {
				qmNameCache.remove(session);
			} else {
				old.insert(session, qmOld.value(session));
			}
		}
		qmOld = old;

		QList< struct ListEntry > entries;
		entries << ListEntry(
			QString::fromLatin1("[%1:%2]").arg(me->cChannel->qsName).arg(me->cChannel->qlUsers.count()), false, false);

		bool hasnew = false;

		QMap< unsigned int, Timer > speaking;

		foreach (Channel *c, home->allLinks()) {
			foreach (User *p, c->qlUsers) {
				ClientUser *u = static_cast< ClientUser * >(p);
				bool bTalk    = (u->tsState != Settings::Passive);
				if (bTalk) {
					speaking.insert(p->uiSession, Timer());
				} else if (qmSpeaking.contains(p->uiSession)) {
					Timer t = qmSpeaking.value(p->uiSession);
					if (t.elapsed() > 1000000)
						qmSpeaking.remove(p->uiSession);
					else {
						speaking.insert(p->uiSession, t);
						bTalk = true;
					}
				}
				if (bTalk) {
					alert = true;
					entries << ListEntry(p->qsName, true, (p->cChannel != me->cChannel));
				} else if (c == me->cChannel) {
					if (qmNew.value(p->uiSession).elapsed() < 3000000) {
						entries << ListEntry(QLatin1String("+") + p->qsName, false, false);
						hasnew = true;
					}
				}
			}
		}
		qmSpeaking = speaking;

		foreach (unsigned int session, qmOld.keys()) {
			entries << ListEntry(QLatin1String("-") + qmNameCache.value(session), false, false);
		}

		if (!qmOld.isEmpty() || hasnew || !qmSpeaking.isEmpty()) {
			qtTimer->start(500);
		} else {
			qtTimer->stop();
		}

		std::sort(++entries.begin(), entries.end(), entriesSort);

		const int iWidth          = size.width();
		const int iHeight         = size.height();
		const int iUsersPerColumn = iHeight / iFontHeight;
		const int iSplitterWidth  = Global::get().s.iLCDUserViewSplitterWidth;
		const int iUserColumns    = static_cast< int >((entries.count() + iUsersPerColumn - 1) / iUsersPerColumn);

		int iColumns     = iUserColumns;
		int iColumnWidth = 1;

		while (iColumns >= 1) {
			iColumnWidth = (iWidth - (iColumns - 1) * iSplitterWidth) / iColumns;
			if (iColumnWidth >= Global::get().s.iLCDUserViewMinColWidth)
				break;
			--iColumns;
		}

		int row = 0, col = 0;


		foreach (const ListEntry &le, entries) {
			if (row >= iUsersPerColumn) {
				row = 0;
				++col;
			}
			if (col > iColumns)
				break;

			if (!le.qsString.isEmpty()) {
				if (le.bBold && le.bItalic)
					painter.setFont(qfItalicBold);
				else if (le.bBold)
					painter.setFont(qfBold);
				else if (le.bItalic)
					painter.setFont(qfItalic);
				else
					painter.setFont(qfNormal);
				painter.drawText(
					QRect(col * (iColumnWidth + iSplitterWidth), row * iFontHeight, iColumnWidth, iFontHeight + 2),
					Qt::AlignLeft, le.qsString);
			}
			++row;
		}
	}

	foreach (LCDDevice *d, devmgr.qlDevices) {
		QImage *img = qhImages[d->size()];
		if (!img)
			continue;
		d->blitImage(img, alert);
	}
}

LCD::~LCD() {
	destroyBuffers();
}

bool LCD::hasDevices() {
	return (!devmgr.qlDevices.isEmpty());
}

/* --- */

LCDEngine::LCDEngine() : QObject() {
}

LCDEngine::~LCDEngine() {
	foreach (LCDDevice *lcd, qlDevices)
		delete lcd;
}

LCDDevice::LCDDevice() {
}

LCDDevice::~LCDDevice() {
}

/* --- */

uint qHash(const QSize &size) {
	return static_cast< uint >((size.width() & 0xffff) << 16) | (size.height() & 0xffff);
}
