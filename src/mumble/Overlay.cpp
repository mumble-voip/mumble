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
#include "Database.h"
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

OverlayClient::OverlayClient(QLocalSocket *socket, QObject *p) : QObject(p) {
	qlsSocket = socket;
	qlsSocket->setParent(this);
	connect(qlsSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

	omMsg.omh.iLength = -1;
	smMem = NULL;
	uiWidth = uiHeight = 0;
}

void OverlayClient::readyRead() {
	while (1) {
		int ready = qlsSocket->bytesAvailable();

		if (omMsg.omh.iLength == -1) {
			if (ready < sizeof(OverlayMsgHeader))
				break;
			else {
				qlsSocket->read(omMsg.headerbuffer, sizeof(OverlayMsgHeader));
				if ((omMsg.omh.uiMagic != OVERLAY_MAGIC_NUMBER) || (omMsg.omh.iLength < 0) || (omMsg.omh.iLength > sizeof(OverlayMsgShmem))) {
					disconnect();
					return;
				}
				ready -= sizeof(OverlayMsgHeader);
			}
		}

		if (ready >= omMsg.omh.iLength) {
			int length = qlsSocket->read(omMsg.msgbuffer, omMsg.omh.iLength);

			if (length != omMsg.omh.iLength) {
				disconnect();
				return;
			}

			switch (omMsg.omh.uiType) {
				case OVERLAY_MSGTYPE_INIT: {
						if (length != sizeof(OverlayMsgInit))
							break;

						OverlayMsgInit *omi = & omMsg.omi;

						uiWidth = omi->uiWidth;
						uiHeight = omi->uiHeight;
						qrLast = QRect();

						if (smMem)
							delete smMem;

						smMem = new SharedMemory2(this, uiWidth * uiHeight * 4);
						if (! smMem->data()) {
							qWarning() << "OverlayClient: Failed to create shared memory";
							delete smMem;
							smMem = NULL;
							break;
						}
						QByteArray key = smMem->name().toUtf8();
						key.append(static_cast<char>(0));

						OverlayMsg om;
						om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
						om.omh.uiType = OVERLAY_MSGTYPE_SHMEM;
						om.omh.iLength = key.length();
						memcpy(om.oms.a_cName, key.constData(), key.length());
						qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + om.omh.iLength);

						setupRender();

						Overlay *o = static_cast<Overlay *>(parent());
						QTimer::singleShot(0, o, SLOT(updateOverlay()));
					}
					break;
				case OVERLAY_MSGTYPE_SHMEM: {
						if (smMem)
							smMem->systemRelease();
					}
					break;
				default:
					break;
			}
			omMsg.omh.iLength = -1;
		} else {
			break;
		}
	}
}

void OverlayClient::reset() {
	if (! uiWidth || ! uiHeight || ! smMem)
		return;
	setupRender();
}

void OverlayClient::setupRender() {
	Overlay *o = static_cast<Overlay *>(parent());

	fItemHeight = uiHeight * g.s.fOverlayHeight;
	iItemHeight = iroundf(fItemHeight + 0.5f);
	fEdge = fItemHeight * 0.05f;

	qiMuted = QImage(iItemHeight / 2, iItemHeight / 2, QImage::Format_ARGB32);
	qiDeafened = QImage(iItemHeight / 2, iItemHeight / 2, QImage::Format_ARGB32);
	{
		QPainter p(&qiMuted);
		p.setRenderHint(QPainter::Antialiasing);
		p.setRenderHint(QPainter::TextAntialiasing);
		p.setCompositionMode(QPainter::CompositionMode_Clear);
		p.setBackground(QColor(0,0,0,0));
		p.eraseRect(0, 0, iItemHeight, iItemHeight);
		p.setCompositionMode(QPainter::CompositionMode_Source);
		o->qsrMuted.render(&p);
	}

	{
		QPainter p(&qiDeafened);
		p.setRenderHint(QPainter::Antialiasing);
		p.setRenderHint(QPainter::TextAntialiasing);
		p.setCompositionMode(QPainter::CompositionMode_Clear);
		p.setBackground(QColor(0,0,0,0));
		p.eraseRect(0, 0, iItemHeight, iItemHeight);
		p.setCompositionMode(QPainter::CompositionMode_Source);
		o->qsrDeafened.render(&p);
	}

	fFont = g.s.qfOverlayFont;

	QPainterPath qp;
	qp.addText(0.0f, 0.0f, fFont, QLatin1String("Üy"));
	QRectF r = qp.controlPointRect();

	float fs = fFont.pointSizeF();
	float ds = fs * ((fItemHeight - 2 * fEdge) / r.height());

	fFont.setPointSizeF(ds);

	qp = QPainterPath();
	qp.addText(0.0f, 0.0f, fFont, QLatin1String("Hello"));
	fBase = qp.controlPointRect().height();

	qcTexts.clear();
	qlLines.clear();

	smMem->erase();

	OverlayMsg om;
	om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
	om.omh.uiType = OVERLAY_MSGTYPE_BLIT;
	om.omh.iLength = sizeof(OverlayMsgBlit);
	om.omb.x = 0;
	om.omb.y = 0;
	om.omb.w = uiWidth;
	om.omb.h = uiHeight;
	qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgBlit));
}

bool OverlayClient::setTexts(const QList<OverlayTextLine> &lines) {
	if (! uiWidth || ! uiHeight || ! smMem)
		return true;

	QImage qi(reinterpret_cast<unsigned char *>(smMem->data()), uiWidth, uiHeight, QImage::Format_ARGB32);
	QRect dirty, active;

	QPainter p(&qi);
	p.setRenderHint(QPainter::Antialiasing);
	p.setRenderHint(QPainter::TextAntialiasing);
	p.setBackground(QColor(0,0,0,0));
	p.setCompositionMode(QPainter::CompositionMode_SourceOver);

	QList<TextImageKey> imagekeys;
	QList<TextImageKey> avatarkeys;

	if (qcTexts.maxCost() < 4 * lines.count())
		qcTexts.setMaxCost(4 * lines.count());

	int height = 0;

	foreach(const OverlayTextLine &e, lines) {
		TextImageKey key, avatar;

		if (! e.qsText.isEmpty() || e.uiSession != 0) {
			key = TextImageKey(sha1(e.qsText), e.uiColor);
			if (! qcTexts.contains(key)) {
				QPainterPath qp;
				qp.addText(0.0f, fBase+fEdge, fFont, e.qsText);

				QRectF qr = qp.controlPointRect();
#if QT_VERSION >= 0x040600
				qp.translate(- qr.x() + fEdge, - qr.y() + fEdge);
#else
				qp = QPainterPath();
				qp.addText(- qr.x() + fEdge, - qr.y() + 2 * fEdge + fBase, fFont, str);
#endif

				int w = iroundf(qr.width() + 2 * fEdge + 0.5f);
				int h = iroundf(qr.height() + 2 * fEdge + 0.5f);

				QImage img(w, h, QImage::Format_ARGB32);

				QPainter imgp(&img);
				imgp.setRenderHint(QPainter::Antialiasing);
				imgp.setRenderHint(QPainter::TextAntialiasing);

				imgp.setBackground(QColor(0,0,0,0));
				imgp.setCompositionMode(QPainter::CompositionMode_Clear);
				imgp.eraseRect(0, 0, w, h);

				QColor qc(e.uiColor);
				qc.setAlpha(255);

				imgp.setCompositionMode(QPainter::CompositionMode_Source);
				imgp.setBrush(qc);
				imgp.setPen(QPen(Qt::black, fEdge, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
				imgp.drawPath(qp);

				imgp.setPen(Qt::NoPen);
				imgp.drawPath(qp);

				qcTexts.insert(key, new TextImage(img, qr.top()));
			}
			height += iItemHeight;

			if (e.uiSession != 0) {
				ClientUser *cu = ClientUser::get(e.uiSession);

				if (cu) {
					QByteArray hash = cu->qbaTextureHash;
					if (cu->qbaTexture.isEmpty())
						hash = QByteArray();
					switch (e.dDecor) {
						case OverlayTextLine::Muted:
							hash.append('m');
							break;
						case OverlayTextLine::Deafened:
							hash.append('d');
							break;
						default:
							break;
					}

					if (! hash.isEmpty()) {
						avatar = TextImageKey(hash, e.uiColor);

						if (! qcTexts.contains(avatar)) {
							QImage img;

							if (! cu->qbaTexture.isEmpty()) {
								if (qFromBigEndian<unsigned int>(reinterpret_cast<const unsigned char *>(cu->qbaTexture.constData())) == 600 * 60 * 4) {
									QByteArray qba = qUncompress(cu->qbaTexture);
									if (qba.length() == 600 * 60 * 4) {
										int width = 0;
										int height = 0;
										const unsigned int *ptr = reinterpret_cast<const unsigned int *>(qba.constData());

										// If we have an alpha only part on the right side of the image ignore it
										for (int y=0;y<60;++y) {
											for (int x=0;x<600; ++x) {
												if (ptr[y*600+x] & 0xff000000) {
													if (x > width)
														width = x;
													if (y > height)
														height = y;
												}
											}
										}

										img = QImage(width+1, height+1, QImage::Format_ARGB32);
										{
											QImage srcimg(reinterpret_cast<const uchar *>(cu->qbaTexture.constData()), 600, 60, QImage::Format_ARGB32);

											QPainter imgp(&img);
											imgp.setRenderHint(QPainter::Antialiasing);
											imgp.setRenderHint(QPainter::TextAntialiasing);
											imgp.setCompositionMode(QPainter::CompositionMode_Clear);
											imgp.setBackground(QColor(0,0,0,0));
											imgp.eraseRect(0, 0, img.height(), img.width());
											imgp.setCompositionMode(QPainter::CompositionMode_Source);
											imgp.drawImage(0, 0, srcimg);
										}
										img = img.scaled(iItemHeight, iItemHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
									}
								} else {
									QBuffer qb(& cu->qbaTexture);
									qb.open(QIODevice::ReadOnly);

									QImageReader qir(&qb);
									if (qir.canRead() && (qir.size().width() <= 1024) && (qir.size().height() <= 1024)) {
										QSize sz = qir.size();
										sz.scale(iItemHeight, iItemHeight, Qt::KeepAspectRatio);
										qir.setScaledSize(sz);
										img = qir.read();
									}
								}
								if (img.isNull()) {
									cu->qbaTexture = QByteArray();
									cu->qbaTextureHash = QByteArray();
								}
							}

							QImage decal(iItemHeight, iItemHeight, QImage::Format_ARGB32);

							QPainter imgp(&decal);
							imgp.setRenderHint(QPainter::Antialiasing);
							imgp.setRenderHint(QPainter::TextAntialiasing);
							imgp.setCompositionMode(QPainter::CompositionMode_Clear);
							imgp.setBackground(QColor(0,0,0,0));
							imgp.eraseRect(0, 0, decal.height(), decal.width());
							imgp.setCompositionMode(QPainter::CompositionMode_SourceOver);
							if (! img.isNull()) {
								imgp.drawImage((iItemHeight - img.width()) / 2, (iItemHeight - img.height()) / 2, img);
								imgp.setOpacity(0.8f);
							}
							switch (e.dDecor) {
								case OverlayTextLine::Muted:
									imgp.drawImage(iItemHeight / 2, iItemHeight / 2, qiMuted);
									break;
								case OverlayTextLine::Deafened:
									imgp.drawImage(iItemHeight / 2, iItemHeight / 2, qiDeafened);
									break;
								default:
									break;
							}

							qcTexts.insert(avatar, new TextImage(decal, 0));
						}
					}
				}
			}
		} else {
			height += iItemHeight / 2;
		}
		imagekeys << key;
		avatarkeys << avatar;
	}

	if (height > static_cast<int>(uiHeight))
		height = uiHeight;

	int y = iroundf(uiHeight * g.s.fOverlayY);
	if (g.s.bOverlayTop) {
		y -= height;
	} else if (! g.s.bOverlayBottom) {
		y -= height / 2;
	}

	if (y < 0)
		y = 0;
	if ((y + height) >= static_cast<int>(uiHeight))
		y = uiHeight - height;

	int i;
	for (i=0;i<lines.count();++i) {
		if (i >= qlLines.count())
			qlLines << OverlayTextLine();

		const OverlayTextLine &src = lines.at(i);
		OverlayTextLine &dst = qlLines[i];
		const TextImageKey &key = imagekeys.at(i);
		const TextImageKey &avatar = avatarkeys.at(i);
		TextImage *ti = qcTexts.object(key);
		TextImage *ai = qcTexts.object(avatar);

		int x = iroundf(uiWidth * g.s.fOverlayX);
		int width = iItemHeight;

		if (ti)
			width += ti->qiImage.width();

		if (width > static_cast<int>(uiWidth))
			width = uiWidth;

		if (g.s.bOverlayLeft) {
			x -= width;
		} else if (! g.s.bOverlayRight) {
			x -= width / 2;
		}

		QRect nr;

		QColor qc;
		qc.setRgba(src.uiColor);

		if (x < 0)
			x = 0;
		if ((x + width) >= static_cast<int>(uiWidth))
			x = uiWidth - width;

		if (ti)
			nr = QRect(g.s.bOverlayLeft ? x : x + iItemHeight, y + ti->iOffset, ti->qiImage.width(), ti->qiImage.height());

		if ((src.uiColor != dst.uiColor) || (src.qsText != dst.qsText)) {
			dst.dDecor = OverlayTextLine::None;
			dst.qbaHash = QByteArray();
			dst.uiColor = src.uiColor;
			dst.qsText = src.qsText;

			if (dst.qrRect.isValid()) {
				dirty |= dst.qrRect;
				p.setOpacity(1.0f);
				p.setCompositionMode(QPainter::CompositionMode_Clear);
				p.eraseRect(dst.qrRect);
				p.setCompositionMode(QPainter::CompositionMode_SourceOver);
			}

			if (ti) {
				dirty |= nr;
				p.setOpacity(qc.alphaF());
				p.drawImage(nr.x(), nr.y(), ti->qiImage);
			}
		}

		int decorx = x + (g.s.bOverlayLeft ? (width - iItemHeight) : 0);
		QRect decr = QRect(decorx, y, iItemHeight, iItemHeight);
		nr = nr.united(decr);

		if ((src.uiColor != dst.uiColor) || (src.dDecor != dst.dDecor) || (avatar.first != dst.qbaHash)) {
			dst.dDecor = src.dDecor;
			dst.qbaHash = avatar.first;

			p.setOpacity(1.0f);
			p.setCompositionMode(QPainter::CompositionMode_Clear);
			p.eraseRect(decr);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);

			p.setOpacity(qc.alphaF());

			if (ai)
				p.drawImage(decorx, y + ai->iOffset, ai->qiImage);

			dirty |= decr;
		}

		dst.qrRect = nr;
		active |= nr;

		y += iItemHeight;
	}

	while (qlLines.count() > lines.count()) {
		OverlayTextLine &dst = qlLines.last();

		if (dst.qrRect.isValid()) {
			p.setOpacity(1.0f);
			p.setCompositionMode(QPainter::CompositionMode_Clear);
			p.eraseRect(dst.qrRect);
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);

			dirty |= dst.qrRect;
		}

		qlLines.removeLast();
	}

	if (dirty.isValid()) {
		dirty = dirty.intersected(QRect(0,0,uiWidth, uiHeight));
		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_BLIT;
		om.omh.iLength = sizeof(OverlayMsgBlit);
		om.omb.x = dirty.x();
		om.omb.y = dirty.y();
		om.omb.w = dirty.width();
		om.omb.h = dirty.height();
		qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgBlit));
	}

	if (active.isEmpty())
		active = QRect(0,0,0,0);

	active = active.intersected(QRect(0,0,uiWidth,uiHeight));

	if (active != qrLast) {
		qrLast = active;

		OverlayMsg om;
		om.omh.uiMagic = OVERLAY_MAGIC_NUMBER;
		om.omh.uiType = OVERLAY_MSGTYPE_ACTIVE;
		om.omh.iLength = sizeof(OverlayMsgActive);
		om.oma.x = qrLast.x();
		om.oma.y = qrLast.y();
		om.oma.w = qrLast.width();
		om.oma.h = qrLast.height();
		qlsSocket->write(om.headerbuffer, sizeof(OverlayMsgHeader) + sizeof(OverlayMsgActive));
	}

	bool late = false;

	if (! qlsSocket->flush() && (qlsSocket->bytesToWrite() > 1024)) {
		return (t.elapsed() <= 5000000ULL);
	} else {
		t.restart();
		return true;
	}
}

bool OverlayTextLine::operator <(const OverlayTextLine &other) const {
	if (iPriority < other.iPriority)
		return true;
	else if (iPriority > other.iPriority)
		return false;

	return QString::localeAwareCompare(qsText, other.qsText) < 0;
}

Overlay::Overlay() : QObject() {
	d = NULL;

	qsrMuted.load(QLatin1String("skin:muted_self.svg"));
	qsrDeafened.load(QLatin1String("skin:deafened_self.svg"));

	platformInit();
	forceSettings();

	qlsServer = new QLocalServer(this);
	QString pipepath;
#ifdef Q_OS_WIN
	pipepath = QLatin1String("MumbleOverlayPipe");
#else
	pipepath = QDir::home().absoluteFilePath(QLatin1String(".MumbleOverlayPipe"));
	{
		QFile f(pipepath);
		if (f.exists()) {
			qWarning() << "Overlay: Removing old socket on" << pipepath;
			f.remove();
		}
	}
#endif

	if (! qlsServer->listen(pipepath)) {
		QMessageBox::warning(NULL, QLatin1String("Mumble"), tr("Failed to create communication with overlay at %2: %1. No overlay will be available.").arg(qlsServer->errorString(),pipepath), QMessageBox::Ok, QMessageBox::NoButton);
	} else {
		qWarning() << "Overlay: Listening on" << qlsServer->fullServerName();
		connect(qlsServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}

	QMetaObject::connectSlotsByName(this);
}

Overlay::~Overlay() {
	setActive(false);
	if (d)
		delete d;
}

void Overlay::newConnection() {
	while (1) {
		QLocalSocket *qls = qlsServer->nextPendingConnection();
		if (! qls)
			break;
		OverlayClient *oc = new OverlayClient(qls, this);
		qlClients << oc;

		connect(qls, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(qls, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(error(QLocalSocket::LocalSocketError)));
	}
}

void Overlay::disconnected() {
	QLocalSocket *qls = qobject_cast<QLocalSocket *>(sender());
	foreach(OverlayClient *oc, qlClients) {
		if (oc->qlsSocket == qls) {
			qlClients.removeAll(oc);
			oc->deleteLater();
			return;
		}
	}
}

void Overlay::error(QLocalSocket::LocalSocketError) {
	disconnected();
}

bool Overlay::isActive() const {
	return ! qlClients.isEmpty();
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

	updateOverlay();
}

void Overlay::forceSettings() {
	foreach(OverlayClient *oc, qlClients) {
		oc->reset();
	}

	updateOverlay();
}

void Overlay::verifyTexture(ClientUser *cp, bool allowupdate) {
	ClientUser *self = ClientUser::get(g.uiSession);
	if (allowupdate && self && self->cChannel->isLinked(cp->cChannel))
		setTexts(qlCurrentTexts);

	qsQueried.remove(cp->uiSession);
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
	QList<OverlayTextLine> lines;

	if (! g.uiSession) {
		qsQueried.clear();
		setTexts(lines);
		return;
	}

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

	if (g.s.bOverlayTop) {
		foreach(qpChanCol cc, linkchans) {
			if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
				lines << OverlayTextLine(cc.first, cc.second, 0);
			}
		}
		if (linkchans.count() > 0) {
			lines << OverlayTextLine(QString(), 0, 0);
		}
	}

	foreach(User *p, ClientUser::get(g.uiSession)->cChannel->qlUsers) {
		ClientUser *u = static_cast<ClientUser *>(p);
		if ((g.s.osOverlay == Settings::All) || (u->tsState != ClientUser::TalkingOff) || ((u == ClientUser::get(g.uiSession)) && g.s.bOverlayAlwaysSelf)) {
			QString name = u->qsName;
			OverlayTextLine::Decoration dec = OverlayTextLine::None;
			if (u->bDeaf || u->bSelfDeaf)
				dec = OverlayTextLine::Deafened;
			else if (u->bMute || u->bSelfMute || u->bLocalMute)
				dec = OverlayTextLine::Muted;
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
			lines << OverlayTextLine(name, col, 1, u->uiSession, dec);
		}
	}

	if (! g.s.bOverlayTop) {
		if (linkchans.count() > 0) {
			lines << OverlayTextLine(QString(), 0, 2);
		}
		foreach(qpChanCol cc, linkchans) {
			if ((g.s.osOverlay == Settings::All) || (cc.second == colChannelTalking)) {
				lines << OverlayTextLine(cc.first, cc.second, 2);
			}
		}
	}

	qSort(lines);
	setTexts(lines);
}

void Overlay::setTexts(const QList<OverlayTextLine> &lines) {
	qlCurrentTexts = lines;

	if (qlClients.isEmpty())
		return;

	QSet<unsigned int> query;

	foreach(const OverlayTextLine &e, lines) {
		ClientUser *cp = ClientUser::get(e.uiSession);
		if (g.s.bOverlayUserTextures && cp && ! cp->qbaTextureHash.isEmpty()) {
			if (cp->qbaTexture.isEmpty() && ! qsQueried.contains(cp->uiSession)) {
				cp->qbaTexture=Database::blob(cp->qbaTextureHash);
				if (cp->qbaTexture.isEmpty())
					query.insert(cp->uiSession);
				else
					verifyTexture(cp, false);
			}
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

	foreach(OverlayClient *oc, qlClients) {
		if (! oc->setTexts(lines)) {
			qWarning() << "Overlay: Dead client detected";
			qlClients.removeAll(oc);
			delete oc;
			break;
		}
	}
}
