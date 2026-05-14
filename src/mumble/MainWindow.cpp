// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MainWindow.h"

#include "ACL.h"
#include "ACLEditor.h"
#include "About.h"
#include "AudioInput.h"
#include "AudioStats.h"
#include "AudioWizard.h"
#include "BanEditor.h"
#include "Cert.h"
#include "Channel.h"
#include "ConnectDialog.h"
#include "Connection.h"
#include "Database.h"
#include "DeveloperConsole.h"
#include "Log.h"
#include "MumbleConstants.h"
#include "Net.h"
#include "NetworkConfig.h"
#include "GlobalShortcut.h"
#include "GlobalShortcutTypes.h"
#ifdef USE_OVERLAY
#	include "OverlayClient.h"
#endif
#include "../SignalCurry.h"
#include "ChannelListenerManager.h"
#include "ChatPerfTrace.h"
#include "FailedConnectionDialog.h"
#include "ListenerVolumeSlider.h"
#include "Markdown.h"
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
#	include "ModernShellBridge.h"
#	include "ModernShellHost.h"
#	include <QtWebEngineCore/QWebEngineProfile>
#	include <QtWebEngineCore/QWebEngineSettings>
#	include <QtWebEngineCore/QWebEngineUrlRequestInfo>
#	include <QtWebEngineCore/QWebEngineUrlRequestInterceptor>
#	include <QtWebEngineWidgets/QWebEngineView>
#endif
#include "MenuLabel.h"
#include "PTTButtonWidget.h"
#include "PersistentChatController.h"
#include "PersistentChatGateway.h"
#include "PersistentChatHistoryDelegate.h"
#include "PersistentChatHistoryModel.h"
#include "PersistentChatRender.h"
#include "PluginManager.h"
#include "PositionalAudioViewer.h"
#include "QtWidgetUtils.h"
#include "RichTextEditor.h"
#include "Screen.h"
#include "ScreenShare.h"
#include "ScreenShareManager.h"
#include "SearchDialog.h"
#include "ServerHandler.h"
#include "ServerInformation.h"
#include "Settings.h"
#include "SvgIcon.h"
#include "TalkingUI.h"
#include "TextMessage.h"
#include "Themes.h"
#include "Tokens.h"
#include "UiTheme.h"
#include "User.h"
#include "UserEdit.h"
#include "UserInformation.h"
#include "UserLocalNicknameDialog.h"
#include "UserLocalVolumeSlider.h"
#include "UserModel.h"
#include "Utils.h"
#include "VersionCheck.h"
#include "ViewCert.h"
#include "VoiceRecorderDialog.h"
#include "VolumeAdjustmentController.h"
#include "Global.h"

#ifdef Q_OS_WIN
#	include "win.h"
#	include "TaskList.h"
#endif

#ifdef Q_OS_MAC
#	include "AppNap.h"
#endif

#include <QAccessible>
#include <QtCore/QBuffer>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QLocale>
#include <QtCore/QMimeData>
#include <QtCore/QPointer>
#include <QtCore/QRegularExpression>
#include <QtCore/QSet>
#include <QtCore/QSignalBlocker>
#include <QtCore/QStandardPaths>
#include <QtCore/QTimer>
#include <QtCore/QUrlQuery>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>
#include <QtGui/QImageReader>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPixmap>
#include <QtGui/QScreen>
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtGui/QTextDocumentFragment>
#include <QtGui/QTextFrame>
#include <QtGui/QWheelEvent>
#include <QtGui/QWindow>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWhatsThis>

#include "widgets/BanDialog.h"
#include "widgets/PersistentChatListWidget.h"
#include "widgets/PersistentChatMessageGroupWidget.h"
#include "widgets/ResponsiveImageDialog.h"
#include "widgets/SemanticSlider.h"

#ifdef Q_OS_WIN
#	include <dbt.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <optional>

#include "widgets/EventFilters.h"
namespace {
constexpr int PersistentChatScopeRole               = Qt::UserRole;
constexpr int PersistentChatScopeIDRole             = Qt::UserRole + 1;
constexpr int PersistentChatThreadRole              = Qt::UserRole + 2;
constexpr int PersistentChatMessageIDRole           = Qt::UserRole + 3;
constexpr int PersistentChatLabelRole               = Qt::UserRole + 4;
constexpr int PersistentChatUnreadRole              = Qt::UserRole + 5;
constexpr int LocalServerLogScope                   = -1;
constexpr int LocalDirectMessageScope               = -2;
constexpr int PersistentChatBottomInsetHeight       = 18;
constexpr int ModernShellSnapshotActiveCoalesceMs   = 100;
constexpr int ModernShellSnapshotInactiveCoalesceMs = 350;
constexpr int NativeWindowMoveResizeWatchdogMs      = 4000;

bool modernShellMinimalSnapshotEnabled() {
	static const bool enabled = qEnvironmentVariableIntValue("MUMBLE_MODERN_SHELL_MINIMAL_SNAPSHOT") != 0;
	return enabled;
}

bool modernShellStaticModeEnabled() {
	static const bool enabled = qEnvironmentVariableIntValue("MUMBLE_MODERN_SHELL_STATIC_MODE") != 0;
	return enabled;
}

void appendModernShellConnectTrace(const QString &message) {
	if (qEnvironmentVariableIntValue("MUMBLE_CONNECT_TRACE") == 0) {
		return;
	}

	QFile traceFile(Global::get().qdBasePath.filePath(QLatin1String("shared-modern-connect-trace.log")));
	if (!traceFile.open(QIODevice::Append | QIODevice::Text)) {
		return;
	}

	const QByteArray line =
		QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs).toUtf8() + " UI " + message.toUtf8() + '\n';
	traceFile.write(line);
	traceFile.flush();
}

void removeChannelSubtreeWithoutModel(Channel *channel) {
	if (!channel) {
		return;
	}

	while (!channel->qlUsers.isEmpty()) {
		User *baseUser   = channel->qlUsers.constFirst();
		ClientUser *user = baseUser ? ClientUser::get(baseUser->uiSession) : nullptr;
		if (!user) {
			break;
		}
		ClientUser::remove(user);
		delete user;
	}

	while (!channel->qlChannels.isEmpty()) {
		Channel *child = channel->qlChannels.constFirst();
		removeChannelSubtreeWithoutModel(child);
	}

	Channel::remove(channel);
	delete channel;
}

void clearConnectedStateWithoutUserModel() {
	Channel *root = Channel::get(Mumble::ROOT_CHANNEL_ID);
	if (!root) {
		return;
	}

	while (!root->qlUsers.isEmpty()) {
		User *baseUser   = root->qlUsers.constFirst();
		ClientUser *user = baseUser ? ClientUser::get(baseUser->uiSession) : nullptr;
		if (!user) {
			break;
		}
		ClientUser::remove(user);
		delete user;
	}

	while (!root->qlChannels.isEmpty()) {
		Channel *child = root->qlChannels.constFirst();
		removeChannelSubtreeWithoutModel(child);
	}
}

QString modernShellScopeToken(const int scopeValue, const unsigned int scopeID) {
	return QStringLiteral("%1:%2").arg(scopeValue).arg(scopeID);
}

QString modernShellParticipantSubtitle(const ClientUser *user, const Channel *contextChannel, const ClientUser *self,
									   const ClientUser *directMessageUser) {
	if (!user) {
		return QObject::tr("Unavailable");
	}
	if (user == self) {
		return QObject::tr("You");
	}
	if (directMessageUser && user == directMessageUser) {
		return QObject::tr("Direct conversation");
	}
	if (!user->cChannel) {
		return QObject::tr("No active channel");
	}
	if (contextChannel && user->cChannel == contextChannel) {
		return QObject::tr("In this room");
	}

	return QObject::tr("In %1").arg(user->cChannel->qsName);
}

QString modernShellTalkStateKey(const ClientUser *user) {
	if (!user) {
		return QStringLiteral("passive");
	}

	switch (user->tsState) {
		case Settings::Talking:
			return QStringLiteral("talking");
		case Settings::Whispering:
			return QStringLiteral("whispering");
		case Settings::Shouting:
			return QStringLiteral("shouting");
		case Settings::MutedTalking:
			return QStringLiteral("mutedTalking");
		case Settings::Passive:
		default:
			return QStringLiteral("passive");
	}
}

QString modernShellTalkStateLabel(const ClientUser *user) {
	if (!user) {
		return QString();
	}

	switch (user->tsState) {
		case Settings::Talking:
			return QObject::tr("Talking");
		case Settings::Whispering:
			return QObject::tr("Whispering");
		case Settings::Shouting:
			return QObject::tr("Shouting");
		case Settings::MutedTalking:
			return QObject::tr("Talking while muted locally");
		case Settings::Passive:
		default:
			return QString();
	}
}

QString modernShellTalkStateTone(const ClientUser *user) {
	if (!user) {
		return QString();
	}

	switch (user->tsState) {
		case Settings::Whispering:
			return QStringLiteral("whisper");
		case Settings::Shouting:
			return QStringLiteral("warning");
		case Settings::MutedTalking:
			return QStringLiteral("danger");
		case Settings::Talking:
			return QStringLiteral("speaking");
		case Settings::Passive:
		default:
			return QString();
	}
}

QVariantList modernShellParticipantBadges(const ClientUser *user, const ClientUser *self) {
	QVariantList badges;
	if (!user) {
		return badges;
	}

	if (user == self) {
		badges.push_back(QObject::tr("You"));
	}

	switch (user->tsState) {
		case Settings::Talking:
			badges.push_back(QObject::tr("Talking"));
			break;
		case Settings::Whispering:
			badges.push_back(QObject::tr("Whispering"));
			break;
		case Settings::Shouting:
			badges.push_back(QObject::tr("Shouting"));
			break;
		case Settings::MutedTalking:
			badges.push_back(QObject::tr("Muted mic"));
			break;
		case Settings::Passive:
		default:
			break;
	}

	if (user->bPrioritySpeaker) {
		badges.push_back(QObject::tr("Priority"));
	}
	if (user->bRecording) {
		badges.push_back(QObject::tr("Recording"));
	}
	if (user->bSuppress) {
		badges.push_back(QObject::tr("Suppressed"));
	}
	if (user->bDeaf || user->bSelfDeaf) {
		badges.push_back(QObject::tr("Deafened"));
	} else if (user->bMute || user->bSelfMute) {
		badges.push_back(QObject::tr("Muted"));
	}

	return badges;
}

QVariantMap modernShellParticipantStatus(const QString &kind, const QString &label, const QString &tone) {
	QVariantMap status;
	status.insert(QStringLiteral("kind"), kind);
	status.insert(QStringLiteral("label"), label);
	if (!tone.isEmpty()) {
		status.insert(QStringLiteral("tone"), tone);
	}
	return status;
}

QVariantList modernShellParticipantStatuses(const ClientUser *user) {
	QVariantList statuses;
	if (!user) {
		return statuses;
	}

	const QString talkStateKey = modernShellTalkStateKey(user);
	if (talkStateKey != QLatin1String("passive")) {
		statuses.push_back(modernShellParticipantStatus(talkStateKey, modernShellTalkStateLabel(user),
														modernShellTalkStateTone(user)));
	}

	if (!user->qsFriendName.isEmpty()) {
		const bool customFriendLabel = user->qsFriendName.compare(user->qsName, Qt::CaseInsensitive) != 0;
		statuses.push_back(modernShellParticipantStatus(
			QStringLiteral("friend"),
			customFriendLabel ? QObject::tr("Friend: %1").arg(user->qsFriendName) : QObject::tr("Friend"),
			QStringLiteral("favorite")));
	}

	if (user->iId >= 0) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("authenticated"),
														QObject::tr("Authenticated user"), QStringLiteral("success")));
	}

	if (user->bPrioritySpeaker) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("priority"), QObject::tr("Priority speaker"),
														QStringLiteral("warning")));
	}

	if (user->bRecording) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("recording"), QObject::tr("Recording"),
														QStringLiteral("danger")));
	}

	if (user->bDeaf) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("serverDeafened"),
														QObject::tr("Server deafened"), QStringLiteral("danger")));
	} else if (user->bSelfDeaf) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("selfDeafened"), QObject::tr("Deafened"),
														QStringLiteral("danger")));
	} else if (user->bMute) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("serverMuted"), QObject::tr("Server muted"),
														QStringLiteral("warning")));
	} else if (user->bSelfMute) {
		statuses.push_back(
			modernShellParticipantStatus(QStringLiteral("selfMuted"), QObject::tr("Muted"), QStringLiteral("warning")));
	}

	if (user->bLocalMute) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("localMuted"), QObject::tr("Locally muted"),
														QStringLiteral("muted")));
	}

	if (user->bSuppress) {
		statuses.push_back(modernShellParticipantStatus(QStringLiteral("suppressed"), QObject::tr("Suppressed"),
														QStringLiteral("warning")));
	}

	return statuses;
}

bool parseModernShellScopeToken(const QString &token, int &scopeValue, unsigned int &scopeID) {
	const QStringList parts = token.trimmed().split(QLatin1Char(':'));
	if (parts.size() != 2) {
		return false;
	}

	bool scopeOk = false;
	bool idOk    = false;
	scopeValue   = parts.at(0).toInt(&scopeOk);
	scopeID      = parts.at(1).toUInt(&idOk);
	return scopeOk && idOk;
}

enum class PersistentChatComposerGlyph { Attach, Send };

QColor alphaColor(const QColor &color, qreal alpha) {
	QColor adjusted = color;
	adjusted.setAlphaF(qBound< qreal >(0.0, alpha, 1.0));
	return adjusted;
}

QPixmap tintedIconPixmap(const QIcon &icon, const QSize &size, const QColor &color) {
	const QSize effectiveSize = size.isValid() ? size : QSize(16, 16);
	QPixmap pixmap            = icon.pixmap(effectiveSize);
	if (pixmap.isNull()) {
		return pixmap;
	}

	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.fillRect(pixmap.rect(), color);
	return pixmap;
}

QPointF composerGlyphPoint(const QRectF &rect, qreal x, qreal y) {
	return QPointF(rect.left() + (rect.width() * x / 18.0), rect.top() + (rect.height() * y / 18.0));
}

QPainterPath persistentChatComposerGlyphPath(PersistentChatComposerGlyph glyph, const QRectF &rect) {
	QPainterPath path;
	switch (glyph) {
		case PersistentChatComposerGlyph::Attach:
			path.moveTo(composerGlyphPoint(rect, 11.9, 4.2));
			path.cubicTo(composerGlyphPoint(rect, 11.1, 3.6), composerGlyphPoint(rect, 10.2, 3.3),
						 composerGlyphPoint(rect, 9.2, 3.3));
			path.cubicTo(composerGlyphPoint(rect, 7.2, 3.3), composerGlyphPoint(rect, 5.6, 4.9),
						 composerGlyphPoint(rect, 5.6, 6.9));
			path.lineTo(composerGlyphPoint(rect, 5.6, 11.8));
			path.cubicTo(composerGlyphPoint(rect, 5.6, 14.1), composerGlyphPoint(rect, 7.5, 16.0),
						 composerGlyphPoint(rect, 9.8, 16.0));
			path.cubicTo(composerGlyphPoint(rect, 12.1, 16.0), composerGlyphPoint(rect, 14.0, 14.1),
						 composerGlyphPoint(rect, 14.0, 11.8));
			path.lineTo(composerGlyphPoint(rect, 14.0, 7.3));
			path.cubicTo(composerGlyphPoint(rect, 14.0, 5.7), composerGlyphPoint(rect, 12.7, 4.5),
						 composerGlyphPoint(rect, 11.1, 4.5));
			path.cubicTo(composerGlyphPoint(rect, 9.6, 4.5), composerGlyphPoint(rect, 8.3, 5.7),
						 composerGlyphPoint(rect, 8.3, 7.3));
			path.lineTo(composerGlyphPoint(rect, 8.3, 11.3));
			path.cubicTo(composerGlyphPoint(rect, 8.3, 12.2), composerGlyphPoint(rect, 9.0, 12.9),
						 composerGlyphPoint(rect, 9.9, 12.9));
			path.cubicTo(composerGlyphPoint(rect, 10.8, 12.9), composerGlyphPoint(rect, 11.5, 12.2),
						 composerGlyphPoint(rect, 11.5, 11.3));
			break;
		case PersistentChatComposerGlyph::Send:
			path.moveTo(composerGlyphPoint(rect, 2.4, 8.9));
			path.lineTo(composerGlyphPoint(rect, 15.5, 2.4));
			path.lineTo(composerGlyphPoint(rect, 11.0, 15.6));
			path.lineTo(composerGlyphPoint(rect, 8.6, 10.3));
			path.lineTo(composerGlyphPoint(rect, 2.4, 8.9));
			break;
	}

	return path;
}

QPixmap persistentChatComposerGlyphPixmap(PersistentChatComposerGlyph glyph, const QSize &size, const QColor &color) {
	const QSize effectiveSize = size.isValid() ? size : QSize(18, 18);
	QPixmap pixmap(effectiveSize);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen(color, glyph == PersistentChatComposerGlyph::Attach ? 1.55 : 1.45, Qt::SolidLine, Qt::RoundCap,
			 Qt::RoundJoin);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);

	const QRectF glyphRect(1.0, 1.0, effectiveSize.width() - 2.0, effectiveSize.height() - 2.0);
	painter.drawPath(persistentChatComposerGlyphPath(glyph, glyphRect));
	if (glyph == PersistentChatComposerGlyph::Send) {
		painter.drawLine(composerGlyphPoint(glyphRect, 2.4, 8.9), composerGlyphPoint(glyphRect, 8.6, 10.3));
	}

	return pixmap;
}

QIcon persistentChatComposerGlyphIcon(PersistentChatComposerGlyph glyph, const QSize &size, const QColor &normalColor,
									  const QColor &activeColor, const QColor &disabledColor) {
	QIcon icon;
	icon.addPixmap(persistentChatComposerGlyphPixmap(glyph, size, normalColor), QIcon::Normal);
	icon.addPixmap(persistentChatComposerGlyphPixmap(glyph, size, activeColor), QIcon::Active);
	icon.addPixmap(persistentChatComposerGlyphPixmap(glyph, size, activeColor), QIcon::Selected);
	icon.addPixmap(persistentChatComposerGlyphPixmap(glyph, size, disabledColor), QIcon::Disabled);
	return icon;
}

QPixmap serverNavigatorSelfAvatarPixmap(const QString &initial, const QSize &size, const QColor &backgroundColor,
										const QColor &textColor, const QColor &presenceColor,
										const QColor &presenceRingColor) {
	const QSize effectiveSize = size.isValid() ? size : QSize(24, 24);
	QPixmap pixmap(effectiveSize);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	const QRectF avatarRect(0.5, 0.5, effectiveSize.width() - 1.0, effectiveSize.height() - 1.0);
	painter.setPen(Qt::NoPen);
	painter.setBrush(backgroundColor);
	painter.drawEllipse(avatarRect);

	QFont avatarFont;
	avatarFont.setBold(true);
	avatarFont.setPixelSize(std::max(11, effectiveSize.height() / 2));
	painter.setFont(avatarFont);
	painter.setPen(textColor);
	painter.drawText(avatarRect.adjusted(0.0, -0.5, 0.0, 0.0), Qt::AlignCenter, initial.left(1).toUpper());

	const qreal ringDiameter = std::max< qreal >(9.0, effectiveSize.width() * 0.42);
	const qreal dotDiameter  = ringDiameter - 3.0;
	const QRectF ringRect(avatarRect.right() - ringDiameter + 0.5, avatarRect.bottom() - ringDiameter + 0.5,
						  ringDiameter, ringDiameter);
	const QRectF dotRect(ringRect.center().x() - dotDiameter / 2.0, ringRect.center().y() - dotDiameter / 2.0,
						 dotDiameter, dotDiameter);
	painter.setBrush(presenceRingColor);
	painter.drawEllipse(ringRect);
	painter.setBrush(presenceColor);
	painter.drawEllipse(dotRect);

	return pixmap;
}

enum class ServerNavigatorPresenceState { Online, Away, Muted, Deafened };

QString serverNavigatorPresenceStateLabel(ServerNavigatorPresenceState state) {
	switch (state) {
		case ServerNavigatorPresenceState::Away:
			return QObject::tr("Away");
		case ServerNavigatorPresenceState::Muted:
			return QObject::tr("Muted");
		case ServerNavigatorPresenceState::Deafened:
			return QObject::tr("Deafened");
		case ServerNavigatorPresenceState::Online:
		default:
			return QObject::tr("Online");
	}
}

struct ServerNavigatorPresenceDisplay {
	QString name;
	QString label;
	ServerNavigatorPresenceState currentState = ServerNavigatorPresenceState::Online;
	QColor presenceColor;
	QColor onlineColor;
	QColor awayColor;
	QColor mutedStateColor;
	QColor deafenedColor;
	QColor mutedTextColor;
	QColor textColor;
	QColor avatarBackgroundColor;
	QColor avatarTextColor;
	QColor presenceRingColor;
	QColor panelColor;
	QColor borderColor;
	QColor hoverColor;
	QColor selectedColor;
	QColor dividerColor;
	QColor dangerColor;
};

QColor serverNavigatorPresenceStateColor(const ServerNavigatorPresenceDisplay &display,
										 ServerNavigatorPresenceState state) {
	switch (state) {
		case ServerNavigatorPresenceState::Away:
			return display.awayColor;
		case ServerNavigatorPresenceState::Muted:
			return display.mutedStateColor;
		case ServerNavigatorPresenceState::Deafened:
			return display.deafenedColor;
		case ServerNavigatorPresenceState::Online:
		default:
			return display.onlineColor;
	}
}

QPixmap serverNavigatorStatusDotPixmap(const QSize &size, const QColor &fillColor, const QColor &ringColor = QColor()) {
	const QSize effectiveSize = size.isValid() ? size : QSize(10, 10);
	QPixmap pixmap(effectiveSize);
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	const QRectF dotRect(1.0, 1.0, effectiveSize.width() - 2.0, effectiveSize.height() - 2.0);
	if (ringColor.isValid() && ringColor.alpha() > 0) {
		painter.setPen(Qt::NoPen);
		painter.setBrush(ringColor);
		painter.drawEllipse(dotRect);
		painter.setBrush(fillColor);
		painter.drawEllipse(dotRect.adjusted(1.5, 1.5, -1.5, -1.5));
	} else {
		painter.setPen(Qt::NoPen);
		painter.setBrush(fillColor);
		painter.drawEllipse(dotRect);
	}

	return pixmap;
}

QIcon serverNavigatorSkinIcon(const QString &skinPath) {
	QIcon icon;
	SvgIcon::addSvgPixmapsToIcon(icon, skinPath);
	return icon;
}

class ServerNavigatorUserMenuPopup : public QFrame {
public:
	explicit ServerNavigatorUserMenuPopup(QWidget *parent = nullptr) : QFrame(parent) {
		setObjectName(QLatin1String("qwServerNavigatorUserMenuPopup"));
		setAttribute(Qt::WA_StyledBackground, false);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setFocusPolicy(Qt::StrongFocus);
		setFixedWidth(214);

		QVBoxLayout *layout = new QVBoxLayout(this);
		layout->setContentsMargins(8, 8, 8, 18);
		layout->setSpacing(0);

		QFrame *headerFrame = new QFrame(this);
		headerFrame->setObjectName(QLatin1String("qfServerNavigatorUserMenuHeader"));
		headerFrame->setAttribute(Qt::WA_StyledBackground, true);
		QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
		headerLayout->setContentsMargins(14, 10, 14, 12);
		headerLayout->setSpacing(10);

		m_avatarLabel = new QLabel(headerFrame);
		m_avatarLabel->setObjectName(QLatin1String("qlServerNavigatorUserMenuAvatar"));
		m_avatarLabel->setAlignment(Qt::AlignCenter);
		m_avatarLabel->setFixedSize(38, 38);

		QWidget *headerTextWidget     = new QWidget(headerFrame);
		QVBoxLayout *headerTextLayout = new QVBoxLayout(headerTextWidget);
		headerTextLayout->setContentsMargins(0, 0, 0, 0);
		headerTextLayout->setSpacing(1);

		m_nameLabel = new QLabel(QObject::tr("You"), headerTextWidget);
		m_nameLabel->setObjectName(QLatin1String("qlServerNavigatorUserMenuName"));
		m_nameLabel->setTextFormat(Qt::PlainText);
		QFont nameFont = m_nameLabel->font();
		nameFont.setBold(true);
		nameFont.setPointSizeF(std::max(nameFont.pointSizeF() - 0.4, 10.2));
		m_nameLabel->setFont(nameFont);

		m_statusLabel = new QLabel(QObject::tr("Online"), headerTextWidget);
		m_statusLabel->setObjectName(QLatin1String("qlServerNavigatorUserMenuStatus"));
		m_statusLabel->setTextFormat(Qt::PlainText);
		QFont statusFont = m_statusLabel->font();
		statusFont.setPointSizeF(std::max(statusFont.pointSizeF() - 1.0, 8.8));
		m_statusLabel->setFont(statusFont);

		headerTextLayout->addWidget(m_nameLabel);
		headerTextLayout->addWidget(m_statusLabel);
		headerLayout->addWidget(m_avatarLabel, 0, Qt::AlignTop);
		headerLayout->addWidget(headerTextWidget, 1, Qt::AlignVCenter);

		m_stateList                  = new QWidget(this);
		QVBoxLayout *stateListLayout = new QVBoxLayout(m_stateList);
		stateListLayout->setContentsMargins(0, 0, 0, 0);
		stateListLayout->setSpacing(1);

		addPresenceRow(stateListLayout, ServerNavigatorPresenceState::Online);
		addPresenceRow(stateListLayout, ServerNavigatorPresenceState::Away);
		addPresenceRow(stateListLayout, ServerNavigatorPresenceState::Muted);
		addPresenceRow(stateListLayout, ServerNavigatorPresenceState::Deafened);

		m_muteButton       = createActionButton(QObject::tr("Self mute"), true, QLatin1String("neutral"));
		m_deafButton       = createActionButton(QObject::tr("Self deafen"), true, QLatin1String("neutral"));
		m_settingsButton   = createActionButton(QObject::tr("Settings"), false, QLatin1String("neutral"));
		m_disconnectButton = createActionButton(QObject::tr("Disconnect"), false, QLatin1String("danger"));

		layout->addWidget(headerFrame);
		layout->addSpacing(4);
		layout->addWidget(m_stateList);
		layout->addSpacing(5);
		layout->addWidget(createSeparator());
		layout->addSpacing(5);
		layout->addWidget(m_muteButton);
		layout->addWidget(m_deafButton);
		layout->addSpacing(5);
		layout->addWidget(createSeparator());
		layout->addSpacing(5);
		layout->addWidget(m_settingsButton);
		layout->addSpacing(5);
		layout->addWidget(createSeparator());
		layout->addSpacing(5);
		layout->addWidget(m_disconnectButton);
	}

	void sync(const ServerNavigatorPresenceDisplay &display, bool muteChecked, bool deafChecked, bool settingsEnabled,
			  bool disconnectEnabled) {
		m_panelColor   = display.panelColor;
		m_borderColor  = display.borderColor;
		m_arrowFill    = display.panelColor;
		m_arrowOutline = display.borderColor;

		m_avatarLabel->setPixmap(serverNavigatorSelfAvatarPixmap(display.name.left(1), m_avatarLabel->size(),
																 display.avatarBackgroundColor, display.avatarTextColor,
																 display.presenceColor, display.presenceRingColor));
		m_nameLabel->setText(display.name);
		m_statusLabel->setText(display.label);
		m_statusLabel->setStyleSheet(QString::fromLatin1("color:%1;").arg(display.presenceColor.name(QColor::HexArgb)));

		m_preferredFocusButton = nullptr;
		for (const PresenceRowWidgets &row : m_presenceRows) {
			const QColor rowColor        = serverNavigatorPresenceStateColor(display, row.state);
			const bool selected          = row.state == display.currentState;
			const bool interactive       = row.state != ServerNavigatorPresenceState::Away;
			const QColor rowBackground   = selected ? alphaColor(rowColor, 0.14) : QColor(Qt::transparent);
			const QColor rowTextColor    = selected ? display.textColor : display.mutedTextColor;
			const QColor hoverBackground = interactive ? display.hoverColor : rowBackground;
			const QColor hoverTextColor  = interactive ? display.textColor : rowTextColor;
			row.button->setIcon(QIcon(serverNavigatorStatusDotPixmap(row.button->iconSize(), rowColor)));
			row.button->setToolTip(interactive ? QString()
											   : QObject::tr("Away is shown automatically when you are idle."));
			row.button->setStyleSheet(QString::fromLatin1("QPushButton {"
														  " border: none;"
														  " border-radius: 5px;"
														  " background: %1;"
														  " color: %2;"
														  " font-size: 12px;"
														  " padding: 6px 10px;"
														  " text-align: left;"
														  "}"
														  "QPushButton:hover {"
														  " background: %3;"
														  " color: %4;"
														  "}")
										  .arg(rowBackground.name(QColor::HexArgb), rowTextColor.name(QColor::HexArgb),
											   hoverBackground.name(QColor::HexArgb),
											   hoverTextColor.name(QColor::HexArgb)));
			if (selected && interactive) {
				m_preferredFocusButton = row.button;
			}
		}

		m_muteButton->setChecked(muteChecked);
		m_deafButton->setChecked(deafChecked);
		m_settingsButton->setEnabled(settingsEnabled);
		m_disconnectButton->setEnabled(disconnectEnabled);
		if (!m_preferredFocusButton) {
			m_preferredFocusButton = m_muteButton;
		}

		QColor neutralActionColor = display.textColor;
		neutralActionColor.setAlphaF(0.72f);
		const QColor neutralIconColor = neutralActionColor;
		const QColor dangerIconColor  = display.dangerColor;
		m_muteButton->setIcon(QIcon(tintedIconPixmap(serverNavigatorSkinIcon(QLatin1String("skin:muted_self.svg")),
													 QSize(14, 14), neutralIconColor)));
		m_deafButton->setIcon(QIcon(tintedIconPixmap(serverNavigatorSkinIcon(QLatin1String("skin:deafened_self.svg")),
													 QSize(14, 14), neutralIconColor)));
		m_settingsButton->setIcon(QIcon(tintedIconPixmap(serverNavigatorSkinIcon(QLatin1String("skin:config_ui.svg")),
														 QSize(14, 14), neutralIconColor)));
		m_disconnectButton->setIcon(QIcon(tintedIconPixmap(
			serverNavigatorSkinIcon(QLatin1String("skin:disconnect.svg")), QSize(14, 14), dangerIconColor)));

		const QString colorText     = display.textColor.name(QColor::HexArgb);
		const QString colorMuted    = neutralActionColor.name(QColor::HexArgb);
		const QString colorHover    = display.hoverColor.name(QColor::HexArgb);
		const QString colorSelected = display.selectedColor.name(QColor::HexArgb);
		const QString colorDivider  = display.dividerColor.name(QColor::HexArgb);
		const QString colorDanger   = display.dangerColor.name(QColor::HexArgb);
		setStyleSheet(
			QString::fromLatin1("QFrame#qfServerNavigatorUserMenuHeader {"
								" border-bottom: 1px solid %1;"
								"}"
								"QLabel#qlServerNavigatorUserMenuName {"
								" color: %3;"
								"}"
								"QFrame#qfServerNavigatorUserMenuSeparator {"
								" background: %1;"
								"}"
								"QPushButton[serverNavigatorMenuRow='true'] {"
								" border: none;"
								" border-radius: 5px;"
								" background: transparent;"
								" color: %4;"
								" font-size: 12px;"
								" padding: 6px 10px;"
								" text-align: left;"
								"}"
								"QPushButton[serverNavigatorMenuRow='true']:hover {"
								" background: %5;"
								" color: %3;"
								"}"
								"QPushButton[serverNavigatorMenuRow='true']:checked {"
								" background: %6;"
								" color: %3;"
								"}"
								"QPushButton[serverNavigatorMenuRole='danger'] {"
								" color: %7;"
								"}"
								"QPushButton[serverNavigatorMenuRole='danger']:hover {"
								" background: %5;"
								" color: %7;"
								"}"
								"QPushButton[serverNavigatorMenuRow='true']:disabled {"
								" color: %4;"
								"}")
				.arg(colorDivider, colorSelected, colorText, colorMuted, colorHover, colorSelected, colorDanger));
		adjustSize();
		update();
	}

	void setArrowCenterX(int x) {
		m_arrowCenterX = x;
		update();
	}

	QPushButton *presenceButton(ServerNavigatorPresenceState state) const {
		for (const PresenceRowWidgets &row : m_presenceRows) {
			if (row.state == state) {
				return row.button;
			}
		}

		return nullptr;
	}

	QPushButton *preferredFocusButton() const { return m_preferredFocusButton ? m_preferredFocusButton : m_muteButton; }

	QPushButton *muteButton() const { return m_muteButton; }

	QPushButton *deafButton() const { return m_deafButton; }

	QPushButton *settingsButton() const { return m_settingsButton; }

	QPushButton *disconnectButton() const { return m_disconnectButton; }

protected:
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
		Q_UNUSED(event);

		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing, true);

		constexpr qreal arrowHeight    = 10.0;
		constexpr qreal arrowHalfWidth = 8.0;
		const QRectF bubbleRect        = rect().adjusted(0.5, 0.5, -0.5, -arrowHeight - 0.5);
		const qreal arrowCenter =
			qBound(bubbleRect.left() + 20.0, static_cast< qreal >(m_arrowCenterX), bubbleRect.right() - 20.0);

		QPainterPath bubblePath;
		bubblePath.addRoundedRect(bubbleRect, 10.0, 10.0);

		QPainterPath arrowPath;
		arrowPath.moveTo(arrowCenter - arrowHalfWidth, bubbleRect.bottom());
		arrowPath.lineTo(arrowCenter, bubbleRect.bottom() + arrowHeight);
		arrowPath.lineTo(arrowCenter + arrowHalfWidth, bubbleRect.bottom());
		arrowPath.closeSubpath();

		const QPainterPath popupPath = bubblePath.united(arrowPath);
		painter.setPen(QPen(m_borderColor, 1.0));
		painter.setBrush(m_panelColor);
		painter.drawPath(popupPath);
	}

private:
	struct PresenceRowWidgets {
		ServerNavigatorPresenceState state = ServerNavigatorPresenceState::Online;
		QPushButton *button                = nullptr;
	};

	void addPresenceRow(QVBoxLayout *layout, ServerNavigatorPresenceState state) {
		QPushButton *button = new QPushButton(serverNavigatorPresenceStateLabel(state), m_stateList);
		button->setProperty("serverNavigatorPresenceRow", true);
		button->setCursor(state == ServerNavigatorPresenceState::Away ? Qt::ArrowCursor : Qt::PointingHandCursor);
		button->setFlat(true);
		button->setIconSize(QSize(9, 9));
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		button->setFocusPolicy(state == ServerNavigatorPresenceState::Away ? Qt::NoFocus : Qt::StrongFocus);
		layout->addWidget(button);
		m_presenceRows << PresenceRowWidgets{ state, button };
	}

	QPushButton *createActionButton(const QString &label, bool checkable, const QString &role) {
		QPushButton *button = new QPushButton(label, this);
		button->setProperty("serverNavigatorMenuRow", true);
		button->setProperty("serverNavigatorMenuRole", role);
		button->setCheckable(checkable);
		button->setCursor(Qt::PointingHandCursor);
		button->setFlat(true);
		button->setIconSize(QSize(14, 14));
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		return button;
	}

	QWidget *createSeparator() {
		QFrame *separator = new QFrame(this);
		separator->setObjectName(QLatin1String("qfServerNavigatorUserMenuSeparator"));
		separator->setAttribute(Qt::WA_StyledBackground, true);
		separator->setFixedHeight(1);
		return separator;
	}

	QLabel *m_avatarLabel = nullptr;
	QLabel *m_nameLabel   = nullptr;
	QLabel *m_statusLabel = nullptr;
	QWidget *m_stateList  = nullptr;
	QList< PresenceRowWidgets > m_presenceRows;
	QPushButton *m_preferredFocusButton = nullptr;
	QPushButton *m_muteButton           = nullptr;
	QPushButton *m_deafButton           = nullptr;
	QPushButton *m_settingsButton       = nullptr;
	QPushButton *m_disconnectButton     = nullptr;
	int m_arrowCenterX                  = 170;
	QColor m_panelColor                 = QColor(46, 51, 60);
	QColor m_borderColor                = QColor(58, 63, 74);
	QColor m_arrowFill                  = QColor(46, 51, 60);
	QColor m_arrowOutline               = QColor(58, 63, 74);
};

bool chatMessageLessThan(const MumbleProto::ChatMessage &lhs, const MumbleProto::ChatMessage &rhs) {
	const quint64 lhsCreatedAt = lhs.has_created_at() ? lhs.created_at() : 0;
	const quint64 rhsCreatedAt = rhs.has_created_at() ? rhs.created_at() : 0;
	if (lhsCreatedAt != rhsCreatedAt) {
		return lhsCreatedAt < rhsCreatedAt;
	}
	if (lhs.thread_id() != rhs.thread_id()) {
		return lhs.thread_id() < rhs.thread_id();
	}

	return lhs.message_id() < rhs.message_id();
}

[[maybe_unused]] QString persistentChatScopeCacheKey(MumbleProto::ChatScope scope, unsigned int scopeID) {
	return QString::fromLatin1("%1:%2").arg(static_cast< int >(scope)).arg(scopeID);
}

[[maybe_unused]] QString persistentChatScopeJumpUrl(MumbleProto::ChatScope scope, unsigned int scopeID) {
	switch (scope) {
		case MumbleProto::TextChannel:
			return QString::fromLatin1("mumble-chat://scope/text/%1").arg(scopeID);
		case MumbleProto::Channel:
		case MumbleProto::ServerGlobal:
		case MumbleProto::Aggregate:
		default:
			return QString();
	}
}

[[maybe_unused]] std::size_t unreadMessagesAfter(const std::vector< MumbleProto::ChatMessage > &messages,
												 unsigned int lastReadMessageID) {
	std::size_t unreadCount = 0;
	for (const MumbleProto::ChatMessage &message : messages) {
		if (message.message_id() > lastReadMessageID) {
			++unreadCount;
		}
	}

	return unreadCount;
}

QString persistentChatDateLabel(const QDate &date) {
	if (!date.isValid()) {
		return QObject::tr("Unknown date");
	}

	const QDate today = QDate::currentDate();
	if (date == today) {
		return QObject::tr("Today");
	}
	if (date == today.addDays(-1)) {
		return QObject::tr("Yesterday");
	}

	return QLocale().toString(date, QLocale::LongFormat);
}

QString persistentChatTimestampLabel(const QDateTime &dateTime) {
	return dateTime.isValid() ? dateTime.time().toString(QLatin1String("HH:mm")) : QObject::tr("Unknown time");
}

QString persistentChatTimestampToolTip(const QDateTime &dateTime) {
	if (!dateTime.isValid()) {
		return QObject::tr("Unknown time");
	}

	return QLocale().toString(dateTime.date(), QLatin1String("d MMM yyyy")) + QString::fromLatin1(", ")
		   + dateTime.time().toString(QLatin1String("HH:mm:ss"));
}

QString persistentChatInitials(const QString &name) {
	const QString simplified = name.simplified();
	if (simplified.isEmpty()) {
		return QStringLiteral("?");
	}

	QString initials;
	const QStringList parts = simplified.split(QLatin1Char(' '), Qt::SkipEmptyParts);
	for (const QString &part : parts) {
		initials.append(part.left(1).toUpper());
		if (initials.size() >= 2) {
			break;
		}
	}

	if (initials.isEmpty()) {
		initials = simplified.left(1).toUpper();
	}

	return initials.left(2);
}

QColor persistentChatActorAccentColor(const QString &actorSeed) {
	const QString seed =
		actorSeed.trimmed().toCaseFolded().isEmpty() ? QStringLiteral("?") : actorSeed.trimmed().toCaseFolded();
	const auto pickFromPalette = [&seed](const QStringList &palette) {
		return QColor(palette.at(static_cast< int >(qHash(seed) % static_cast< uint >(palette.size()))));
	};

	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		switch (tokens->preset) {
			case UiThemePreset::CatppuccinMocha:
				return pickFromPalette(QStringList{ QStringLiteral("#f38ba8"), QStringLiteral("#fab387"),
													QStringLiteral("#f9e2af"), QStringLiteral("#a6e3a1"),
													QStringLiteral("#94e2d5"), QStringLiteral("#89b4fa"),
													QStringLiteral("#cba6f7"), QStringLiteral("#f5c2e7") });
			case UiThemePreset::MumbleLight:
				return pickFromPalette(QStringList{ QStringLiteral("#cf222e"), QStringLiteral("#bc4c00"),
													QStringLiteral("#9a6700"), QStringLiteral("#116329"),
													QStringLiteral("#0969da"), QStringLiteral("#5e4db2"),
													QStringLiteral("#8250df"), QStringLiteral("#bf3989") });
			case UiThemePreset::MumbleDark:
			default:
				return pickFromPalette(QStringList{ QStringLiteral("#f47067"), QStringLiteral("#e0823d"),
													QStringLiteral("#c69026"), QStringLiteral("#57ab5a"),
													QStringLiteral("#39c5cf"), QStringLiteral("#6cb6ff"),
													QStringLiteral("#b083f0"), QStringLiteral("#f0a8c4") });
		}
	}

	const QPalette fallbackPalette = qApp ? qApp->palette() : QPalette();
	const bool darkTheme =
		fallbackPalette.color(QPalette::WindowText).lightness() > fallbackPalette.color(QPalette::Window).lightness();
	return darkTheme ? QColor(QStringLiteral("#6cb6ff")) : QColor(QStringLiteral("#0969da"));
}

QColor persistentChatActorAccentBackground(const QColor &accentColor) {
	QColor backgroundColor = accentColor;
	backgroundColor.setAlphaF(0.15f);
	return backgroundColor;
}

QString roomPopulationLabel(int count) {
	return count == 1 ? QObject::tr("1 person here") : QObject::tr("%1 people here").arg(count);
}

[[maybe_unused]] QSize persistentChatMeasuredItemHint(QWidget *widget, int itemWidth) {
	if (!widget) {
		return QSize(std::max(0, itemWidth), 1);
	}

	const int measuredWidth = std::max(0, itemWidth);
	widget->ensurePolished();
	widget->setFixedWidth(measuredWidth);
	if (QLayout *layout = widget->layout()) {
		layout->activate();
	}
	widget->updateGeometry();
	widget->adjustSize();

	const QVariant explicitHeight = widget->property("persistentChatItemHeight");
	int measuredHeight            = explicitHeight.isValid() ? explicitHeight.toInt() : widget->sizeHint().height();
	if (QLayout *layout = widget->layout()) {
		measuredHeight = std::max(measuredHeight, layout->sizeHint().height());
	}

	measuredHeight = std::max(measuredHeight, widget->minimumSizeHint().height());
	return QSize(measuredWidth, std::max(1, measuredHeight));
}

QWidget *createPersistentChatStateWidget(const QString &eyebrow, const QString &title, const QString &body,
										 const QStringList &hints, QWidget *parent, int minimumHeight) {
	QWidget *widget = new QWidget(parent);
	widget->setAttribute(Qt::WA_StyledBackground, true);
	widget->setProperty("persistentChatItemHeight", std::max(180, minimumHeight));

	QVBoxLayout *layout = new QVBoxLayout(widget);
	layout->setContentsMargins(20, 12, 20, 12);
	layout->setSpacing(0);
	layout->addStretch(1);

	QFrame *card = new QFrame(widget);
	card->setObjectName(QLatin1String("qfPersistentChatBanner"));
	card->setAttribute(Qt::WA_StyledBackground, true);
	card->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	card->setMaximumWidth(440);

	QVBoxLayout *cardLayout = new QVBoxLayout(card);
	cardLayout->setContentsMargins(18, 18, 18, 18);
	cardLayout->setSpacing(8);

	QLabel *eyebrowLabel = new QLabel(eyebrow, card);
	eyebrowLabel->setObjectName(QLatin1String("qlPersistentChatBannerEyebrow"));
	eyebrowLabel->setTextFormat(Qt::PlainText);
	QFont eyebrowFont = eyebrowLabel->font();
	eyebrowFont.setCapitalization(QFont::AllUppercase);
	eyebrowFont.setBold(true);
	eyebrowFont.setPointSizeF(std::max(eyebrowFont.pointSizeF() - 1.0, 8.0));
	eyebrowLabel->setFont(eyebrowFont);

	QLabel *titleLabel = new QLabel(title, card);
	titleLabel->setObjectName(QLatin1String("qlPersistentChatBannerTitle"));
	titleLabel->setTextFormat(Qt::PlainText);
	titleLabel->setWordWrap(true);
	QFont titleFont = titleLabel->font();
	titleFont.setBold(true);
	titleFont.setPointSizeF(titleFont.pointSizeF() + 2.0);
	titleLabel->setFont(titleFont);

	QLabel *bodyLabel = new QLabel(body, card);
	bodyLabel->setObjectName(QLatin1String("qlPersistentChatBannerBody"));
	bodyLabel->setTextFormat(Qt::PlainText);
	bodyLabel->setWordWrap(true);
	bodyLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	cardLayout->addWidget(eyebrowLabel);
	cardLayout->addWidget(titleLabel);
	cardLayout->addWidget(bodyLabel);

	if (!hints.isEmpty()) {
		QWidget *hintRow = new QWidget(card);
		hintRow->setObjectName(QLatin1String("qwPersistentChatBannerHints"));
		QHBoxLayout *hintLayout = new QHBoxLayout(hintRow);
		hintLayout->setContentsMargins(0, 4, 0, 0);
		hintLayout->setSpacing(6);
		hintLayout->addStretch(1);
		for (const QString &hint : hints) {
			if (hint.trimmed().isEmpty()) {
				continue;
			}

			QLabel *hintLabel = new QLabel(hint, hintRow);
			hintLabel->setObjectName(QLatin1String("qlPersistentChatBannerHint"));
			hintLabel->setAttribute(Qt::WA_StyledBackground, true);
			hintLabel->setTextFormat(Qt::PlainText);
			hintLayout->addWidget(hintLabel);
		}
		hintLayout->addStretch(1);
		cardLayout->addWidget(hintRow);
	}

	layout->addWidget(card, 0, Qt::AlignHCenter);
	layout->addStretch(1);
	return widget;
}

QImage persistentChatAvatarTexture(const ClientUser *user, int avatarSize) {
	if (!user) {
		return QImage();
	}

	if (user->qbaTexture.isEmpty() && !user->qbaTextureHash.isEmpty() && Global::get().db) {
		const_cast< ClientUser * >(user)->qbaTexture = Global::get().db->blob(user->qbaTextureHash);
	}

	if (user->qbaTexture.isEmpty()) {
		return QImage();
	}

	QBuffer buffer(const_cast< QByteArray * >(&user->qbaTexture));
	buffer.open(QIODevice::ReadOnly);
	QImageReader reader(&buffer, user->qbaTextureFormat);
	QSize scaledSize = reader.size();
	scaledSize.scale(avatarSize, avatarSize, Qt::KeepAspectRatio);
	reader.setScaledSize(scaledSize);
	return reader.read();
}

[[maybe_unused]] QString modernShellAvatarDataUrl(const ClientUser *user, int avatarSize) {
	const QImage image = persistentChatAvatarTexture(user, avatarSize);
	if (image.isNull()) {
		return QString();
	}

	QByteArray encodedBytes;
	QBuffer buffer(&encodedBytes);
	if (!buffer.open(QIODevice::WriteOnly) || !image.save(&buffer, "PNG") || encodedBytes.isEmpty()) {
		return QString();
	}

	return QString::fromLatin1("data:image/png;base64,%1").arg(QString::fromLatin1(encodedBytes.toBase64()));
}

bool isDarkPalette(const QPalette &palette) {
	return palette.color(QPalette::WindowText).lightness() > palette.color(QPalette::Window).lightness();
}

QString qssColor(const QColor &color) {
	return uiThemeQssColor(color);
}

QString applyQssArgs(QString style, const QStringList &args) {
	for (int i = args.size(); i >= 1; --i) {
		const QString placeholder = QStringLiteral("%") + QString::number(i);
		style.replace(placeholder, args.at(i - 1));
	}
	return style;
}

QColor mixColors(const QColor &baseColor, const QColor &overlayColor, qreal overlayRatio) {
	const qreal clampedRatio = qBound< qreal >(0.0, overlayRatio, 1.0);
	const qreal baseRatio    = 1.0 - clampedRatio;
	return QColor::fromRgbF(baseColor.redF() * baseRatio + overlayColor.redF() * clampedRatio,
							baseColor.greenF() * baseRatio + overlayColor.greenF() * clampedRatio,
							baseColor.blueF() * baseRatio + overlayColor.blueF() * clampedRatio, 1.0);
}

struct ChromePaletteColors {
	bool darkTheme = false;
	QColor railColor;
	QColor cardColor;
	QColor elevatedCardColor;
	QColor panelColor;
	QColor inputColor;
	QColor accentColor;
	QColor textColor;
	QColor mutedTextColor;
	QColor eyebrowColor;
	QColor borderColor;
	QColor dividerColor;
	QColor hoverColor;
	QColor selectedColor;
	QColor selectedTextColor;
	QColor scrollbarHandleColor;
	QColor scrollbarHandleHoverColor;
};

ChromePaletteColors buildChromePalette(const QPalette &palette) {
	ChromePaletteColors colors;
	colors.darkTheme = isDarkPalette(palette);

	const QColor windowColor     = palette.color(QPalette::Window);
	const QColor baseColor       = palette.color(QPalette::Base);
	const QColor alternateColor  = palette.color(QPalette::AlternateBase);
	const QColor highlightColor  = palette.color(QPalette::Highlight);
	const QColor highlightedText = palette.color(QPalette::HighlightedText);
	const QColor textColor       = palette.color(QPalette::WindowText);

	colors.railColor         = colors.darkTheme ? mixColors(windowColor, textColor, 0.02) : windowColor;
	colors.textColor         = textColor;
	colors.selectedTextColor = highlightedText;
	colors.accentColor =
		colors.darkTheme ? mixColors(textColor, highlightColor, 0.24) : mixColors(textColor, highlightColor, 0.24);

	if (colors.darkTheme) {
		colors.cardColor         = mixColors(windowColor, alternateColor, 0.26);
		colors.elevatedCardColor = mixColors(baseColor, alternateColor, 0.70);
		colors.panelColor        = mixColors(baseColor, alternateColor, 0.48);
		colors.inputColor        = mixColors(colors.panelColor, alternateColor, 0.20);
		colors.borderColor       = mixColors(alternateColor, textColor, 0.14);
		colors.dividerColor      = mixColors(colors.panelColor, textColor, 0.08);
	} else {
		colors.cardColor         = mixColors(windowColor, alternateColor, 0.40);
		colors.elevatedCardColor = mixColors(colors.cardColor, baseColor, 0.18);
		colors.panelColor        = mixColors(colors.elevatedCardColor, baseColor, 0.08);
		colors.inputColor        = mixColors(colors.panelColor, windowColor, 0.32);
		colors.borderColor       = mixColors(colors.cardColor, textColor, 0.16);
		colors.dividerColor      = mixColors(colors.cardColor, colors.borderColor, 0.66);
	}

	colors.mutedTextColor       = mixColors(textColor, colors.railColor, colors.darkTheme ? 0.44 : 0.34);
	colors.eyebrowColor         = mixColors(textColor, colors.accentColor, colors.darkTheme ? 0.30 : 0.42);
	colors.hoverColor           = mixColors(colors.elevatedCardColor, textColor, colors.darkTheme ? 0.05 : 0.05);
	colors.selectedColor        = mixColors(colors.elevatedCardColor, highlightColor, colors.darkTheme ? 0.28 : 0.16);
	colors.scrollbarHandleColor = colors.darkTheme ? mixColors(colors.panelColor, textColor, 0.20)
												   : mixColors(colors.panelColor, textColor, 0.10);
	colors.scrollbarHandleHoverColor = colors.darkTheme ? mixColors(colors.panelColor, textColor, 0.30)
														: mixColors(colors.panelColor, textColor, 0.18);

	return colors;
}

[[maybe_unused]] QString peopleHereLabel(int count) {
	const QString countLabel = QLocale().toString(std::max(0, count));
	return count == 1 ? QObject::tr("%1 person here").arg(countLabel) : QObject::tr("%1 people here").arg(countLabel);
}

QModelIndex resolveTreeViewportIndex(const QTreeView *view, const QPoint &viewportPos) {
	if (!view || !view->viewport() || !view->model() || viewportPos.y() < 0
		|| viewportPos.y() >= view->viewport()->height()) {
		return QModelIndex();
	}

	QModelIndex idx = view->indexAt(viewportPos);
	if (idx.isValid()) {
		return idx;
	}

	const int viewportWidth = view->viewport()->width();
	if (viewportWidth <= 0) {
		return QModelIndex();
	}

	const int probeXs[] = { qBound(0, viewportPos.x(), viewportWidth - 1),
							qBound(0, viewportWidth / 2, viewportWidth - 1),
							qBound(0, viewportWidth - 12, viewportWidth - 1), qBound(0, 12, viewportWidth - 1) };
	for (const int probeX : probeXs) {
		idx = view->indexAt(QPoint(probeX, viewportPos.y()));
		if (idx.isValid()) {
			return idx;
		}
	}

	for (int probeX = 4; probeX < viewportWidth; probeX += 24) {
		idx = view->indexAt(QPoint(probeX, viewportPos.y()));
		if (idx.isValid()) {
			return idx;
		}
	}

	return QModelIndex();
}

class PersistentChatScopeListDelegate : public QStyledItemDelegate {
public:
	explicit PersistentChatScopeListDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE {
		QSize hint = QStyledItemDelegate::sizeHint(option, index);
		if (!index.isValid()) {
			return hint;
		}

		return QSize(hint.width(), std::max(hint.height(), QFontMetrics(option.font).height() + 10));
	}

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE {
		if (!index.isValid()) {
			return;
		}

		QStyleOptionViewItem opt(option);
		initStyleOption(&opt, index);

		const QPalette effectivePalette = opt.widget ? opt.widget->palette() : opt.palette;
		const bool selected             = opt.state.testFlag(QStyle::State_Selected);
		const bool hovered              = opt.state.testFlag(QStyle::State_MouseOver);
		const QString label             = index.data(PersistentChatLabelRole).toString().isEmpty()
								  ? opt.text
								  : index.data(PersistentChatLabelRole).toString();
		const qulonglong unreadCount = index.data(PersistentChatUnreadRole).toULongLong();
		const int scopeValue         = index.data(PersistentChatScopeRole).toInt();
		const bool utilityRow =
			scopeValue == LocalServerLogScope || scopeValue == static_cast< int >(MumbleProto::ServerGlobal);
		const bool textChannelRow   = scopeValue == static_cast< int >(MumbleProto::TextChannel);
		const bool directMessageRow = scopeValue == LocalDirectMessageScope;
		const bool voiceChannelRow  = scopeValue == static_cast< int >(MumbleProto::Channel);
		static const QIcon s_voiceRoomIcon(QLatin1String("skin:priority_speaker.svg"));

		QString chipText = QStringLiteral("#");
		switch (scopeValue) {
			case LocalServerLogScope:
				chipText = QObject::tr("LOG");
				break;
			case LocalDirectMessageScope:
				chipText = QObject::tr("DM");
				break;
			case static_cast< int >(MumbleProto::ServerGlobal):
				chipText = QObject::tr("ALL");
				break;
			case static_cast< int >(MumbleProto::Channel):
				chipText = QObject::tr("VC");
				break;
			case static_cast< int >(MumbleProto::TextChannel):
				chipText = QObject::tr("TXT");
				break;
			default:
				break;
		}

		if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
			QColor backgroundColor(Qt::transparent);
			if (selected) {
				backgroundColor = alphaColor(tokens->surface1, 0.42);
			} else if (hovered) {
				backgroundColor = alphaColor(tokens->surface1, 0.24);
			}
			const QColor textColor =
				selected ? tokens->textPrimary
						 : ((hovered || unreadCount > 0 || utilityRow || voiceChannelRow || directMessageRow)
								? tokens->textPrimary
								: tokens->textSecondary);
			const QColor secondaryTextColor = selected ? textColor : tokens->textMuted;
			const QColor unreadFillColor    = tokens->accentSubtle;
			const QColor unreadTextColor    = tokens->accent;
			const QColor chipFillColor      = scopeValue == LocalServerLogScope
											 ? alphaColor(tokens->yellow, 0.82)
											 : (scopeValue == static_cast< int >(MumbleProto::ServerGlobal)
													? alphaColor(tokens->accent, 0.18)
													: (textChannelRow ? (selected ? alphaColor(tokens->accent, 0.18)
																				  : alphaColor(tokens->surface1, 0.62))
																	  : QColor(Qt::transparent)));
			const QColor chipTextColor =
				scopeValue == LocalServerLogScope || scopeValue == static_cast< int >(MumbleProto::ServerGlobal)
					? tokens->crust
					: (textChannelRow ? (selected ? tokens->accent : tokens->textMuted) : secondaryTextColor);
			const QColor voiceIconColor = selected ? tokens->accent : secondaryTextColor;

			QRect rowRect = option.rect.adjusted(4, 1, -4, -1);
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing, true);
			if (rowRect.isValid() && backgroundColor.alpha() > 0) {
				painter->setPen(Qt::NoPen);
				painter->setBrush(backgroundColor);
				painter->drawRoundedRect(rowRect, 8.0f, 8.0f);
			}
			if (selected && rowRect.isValid()) {
				const QRect accentRect(rowRect.left(), rowRect.top() + 3, 3, rowRect.height() - 6);
				painter->setPen(Qt::NoPen);
				painter->setBrush(tokens->accent);
				painter->drawRoundedRect(accentRect, 1.5f, 1.5f);
			}

			QFont chipFont(opt.font);
			chipFont.setBold(true);
			chipFont.setPointSizeF(std::max(chipFont.pointSizeF() - 1.0, 8.0));
			const QFontMetrics chipMetrics(chipFont);

			int x = rowRect.left() + 8;
			if (voiceChannelRow) {
				const QRect iconRect(x, rowRect.center().y() - 6, 12, 12);
				painter->drawPixmap(iconRect.topLeft(),
									tintedIconPixmap(s_voiceRoomIcon, iconRect.size(), voiceIconColor));
				x = iconRect.right() + 8;
			} else {
				const int chipWidth = std::max(20, chipMetrics.horizontalAdvance(chipText) + 10);
				const QRect chipRect(x, rowRect.center().y() - 9, chipWidth, 18);
				if (chipFillColor.alpha() > 0) {
					painter->setPen(Qt::NoPen);
					painter->setBrush(chipFillColor);
					painter->drawRoundedRect(chipRect, 4.0f, 4.0f);
				}
				painter->setPen(chipTextColor);
				painter->setFont(chipFont);
				painter->drawText(chipRect, Qt::AlignCenter, chipText);
				x = chipRect.right() + 8;
			}

			int textRight = rowRect.right() - 8;
			if (unreadCount > 0) {
				const QString unreadText = QString::number(unreadCount);
				const int unreadWidth    = chipMetrics.horizontalAdvance(unreadText) + 12;
				const QRect unreadRect(textRight - unreadWidth, rowRect.center().y() - 9, unreadWidth, 18);
				painter->setPen(Qt::NoPen);
				painter->setBrush(unreadFillColor);
				painter->drawRoundedRect(unreadRect, 10.0f, 10.0f);
				painter->setPen(unreadTextColor);
				painter->drawText(unreadRect, Qt::AlignCenter, unreadText);
				textRight = unreadRect.left() - 6;
			}

			const QRect textRect(x, rowRect.top(), std::max(18, textRight - x), rowRect.height());
			QFont titleFont(opt.font);
			titleFont.setBold(selected || utilityRow || unreadCount > 0);
			painter->setFont(titleFont);
			painter->setPen(textChannelRow && !selected && !hovered && unreadCount == 0 ? tokens->textSecondary
																						: textColor);
			painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
							  QFontMetrics(titleFont).elidedText(label.simplified(), Qt::ElideRight, textRect.width()));
			painter->restore();
			return;
		}

		const ChromePaletteColors chrome = buildChromePalette(effectivePalette);

		const QColor utilityRowColor =
			mixColors(effectivePalette.color(QPalette::Window), chrome.textColor, chrome.darkTheme ? 0.10 : 0.04);
		const QColor channelRowColor =
			mixColors(effectivePalette.color(QPalette::Window), chrome.textColor, chrome.darkTheme ? 0.04 : 0.02);
		const QColor channelHoverColor = mixColors(channelRowColor, chrome.textColor, chrome.darkTheme ? 0.03 : 0.02);
		const QColor utilityOutlineColor =
			mixColors(chrome.borderColor, chrome.textColor, chrome.darkTheme ? 0.12 : 0.05);
		const QColor rowFillColor =
			selected
				? (utilityRow ? mixColors(utilityRowColor, chrome.selectedColor, chrome.darkTheme ? 0.22 : 0.14)
							  : mixColors(channelRowColor, chrome.accentColor, chrome.darkTheme ? 0.10 : 0.06))
				: (hovered ? (utilityRow ? mixColors(utilityRowColor, chrome.textColor, chrome.darkTheme ? 0.04 : 0.03)
										 : channelHoverColor)
						   : (utilityRow ? utilityRowColor : QColor(Qt::transparent)));
		const QColor rowOutlineColor =
			selected ? mixColors(chrome.borderColor, chrome.accentColor, chrome.darkTheme ? 0.30 : 0.14)
					 : (utilityRow ? utilityOutlineColor : QColor(Qt::transparent));
		const QColor textColor =
			selected ? chrome.textColor
					 : ((textChannelRow || voiceChannelRow) && unreadCount == 0
							? mixColors(chrome.textColor, chrome.panelColor, chrome.darkTheme ? 0.10 : 0.05)
							: chrome.textColor);
		const QColor mutedTextColor =
			selected ? chrome.textColor
					 : ((textChannelRow || voiceChannelRow)
							? mixColors(chrome.textColor, chrome.panelColor, chrome.darkTheme ? 0.18 : 0.10)
							: chrome.mutedTextColor);
		const QColor chipFillColor =
			selected
				? (textChannelRow ? mixColors(chrome.selectedColor, chrome.panelColor, chrome.darkTheme ? 0.22 : 0.14)
								  : mixColors(chrome.selectedColor, chrome.textColor, chrome.darkTheme ? 0.08 : 0.05))
				: (utilityRow
					   ? mixColors(utilityRowColor, chrome.textColor, chrome.darkTheme ? 0.12 : 0.06)
					   : (textChannelRow ? mixColors(channelRowColor, chrome.textColor, chrome.darkTheme ? 0.16 : 0.08)
										 : QColor(Qt::transparent)));
		const QColor chipTextColor = textChannelRow ? (selected ? chrome.selectedTextColor : mutedTextColor)
													: (selected ? chrome.textColor : mutedTextColor);
		const QColor unreadFillColor =
			selected ? mixColors(chrome.selectedColor, chrome.selectedTextColor, chrome.darkTheme ? 0.16 : 0.10)
					 : mixColors(chrome.selectedColor, utilityRow ? utilityRowColor : channelRowColor,
								 chrome.darkTheme ? 0.44 : 0.28);
		const QColor unreadTextColor = selected ? chrome.selectedTextColor : chrome.textColor;
		const QColor voiceIconColor  = selected ? chrome.accentColor : mutedTextColor;

		QRect rowRect = option.rect.adjusted(4, 1, -4, -1);
		painter->save();
		painter->setRenderHint(QPainter::Antialiasing, true);
		if (rowRect.isValid() && (selected || hovered || utilityRow)) {
			painter->setPen(rowOutlineColor.alpha() == 0 ? Qt::NoPen : QPen(rowOutlineColor, 1.0));
			painter->setBrush(rowFillColor);
			painter->drawRoundedRect(rowRect, 10.0f, 10.0f);
		}
		if (selected && rowRect.isValid()) {
			const QRect accentRect(rowRect.left(), rowRect.top() + 3, 3, rowRect.height() - 6);
			painter->setPen(Qt::NoPen);
			painter->setBrush(chrome.accentColor);
			painter->drawRoundedRect(accentRect, 1.5f, 1.5f);
		}

		QFont chipFont(opt.font);
		chipFont.setBold(true);
		chipFont.setPointSizeF(std::max(chipFont.pointSizeF() - 1.0, 8.0));
		const QFontMetrics chipMetrics(chipFont);

		int x = rowRect.left() + 8;
		if (voiceChannelRow) {
			const QRect iconRect(x, rowRect.center().y() - 6, 12, 12);
			painter->drawPixmap(iconRect.topLeft(), tintedIconPixmap(s_voiceRoomIcon, iconRect.size(), voiceIconColor));
			x = iconRect.right() + 8;
		} else {
			const int chipWidth = std::max(20, chipMetrics.horizontalAdvance(chipText) + 10);
			const QRect chipRect(x, rowRect.center().y() - 9, chipWidth, 18);
			if (chipFillColor.alpha() > 0) {
				painter->setPen(Qt::NoPen);
				painter->setBrush(chipFillColor);
				painter->drawRoundedRect(chipRect, 4.0f, 4.0f);
			}
			painter->setPen(chipTextColor);
			painter->setFont(chipFont);
			painter->drawText(chipRect, Qt::AlignCenter, chipText);
			x = chipRect.right() + 8;
		}

		int textRight = rowRect.right() - 8;
		if (unreadCount > 0) {
			const QString unreadText = QString::number(unreadCount);
			const int unreadWidth    = chipMetrics.horizontalAdvance(unreadText) + 12;
			const QRect unreadRect(textRight - unreadWidth, rowRect.center().y() - 9, unreadWidth, 18);
			painter->setPen(Qt::NoPen);
			painter->setBrush(unreadFillColor);
			painter->drawRoundedRect(unreadRect, 9.0f, 9.0f);
			painter->setPen(unreadTextColor);
			painter->drawText(unreadRect, Qt::AlignCenter, unreadText);
			textRight = unreadRect.left() - 6;
		}

		const QRect textRect(x, rowRect.top(), std::max(18, textRight - x), rowRect.height());
		QFont titleFont(opt.font);
		titleFont.setBold(selected || utilityRow || unreadCount > 0);
		painter->setFont(titleFont);
		painter->setPen(textColor);
		painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
						  QFontMetrics(titleFont).elidedText(label.simplified(), Qt::ElideRight, textRect.width()));
		painter->restore();
	}
};

#ifdef Q_OS_WIN
using DwmSetWindowAttributeFn = HRESULT(WINAPI *)(HWND, DWORD, LPCVOID, DWORD);

constexpr DWORD DwmUseImmersiveDarkModeAttribute       = 20;
constexpr DWORD DwmUseImmersiveDarkModeLegacyAttribute = 19;
constexpr DWORD DwmBorderColorAttribute                = 34;
constexpr DWORD DwmCaptionColorAttribute               = 35;
constexpr DWORD DwmTextColorAttribute                  = 36;

COLORREF colorRefFromQColor(const QColor &color) {
	return RGB(color.red(), color.green(), color.blue());
}

void applyNativeTitleBarTheme(QWidget *widget) {
	if (!widget) {
		return;
	}

	const HWND hwnd = reinterpret_cast< HWND >(widget->winId());
	if (!hwnd) {
		return;
	}

	static const HMODULE dwmapiModule = GetModuleHandleW(L"dwmapi.dll");
	if (!dwmapiModule) {
		return;
	}

	static const DwmSetWindowAttributeFn setWindowAttribute =
		reinterpret_cast< DwmSetWindowAttributeFn >(GetProcAddress(dwmapiModule, "DwmSetWindowAttribute"));
	if (!setWindowAttribute) {
		return;
	}

	const QPalette palette = widget->palette();
	const bool darkTheme   = isDarkPalette(palette);
	QColor captionColor;
	QColor titleTextColor;
	QColor borderColor;

	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		captionColor   = tokens->crust;
		titleTextColor = tokens->text;
		borderColor    = tokens->surface1;
	} else {
		const QColor windowColor = palette.color(QPalette::Window);
		const QColor baseColor   = palette.color(QPalette::Base);
		const QColor accentColor = palette.color(QPalette::Highlight);
		titleTextColor           = palette.color(QPalette::WindowText);
		captionColor = darkTheme ? mixColors(windowColor, baseColor, 0.22) : mixColors(windowColor, accentColor, 0.08);
		borderColor =
			darkTheme ? mixColors(captionColor, accentColor, 0.18) : mixColors(captionColor, accentColor, 0.26);
	}

	const BOOL immersiveDarkMode = darkTheme ? TRUE : FALSE;
	HRESULT result =
		setWindowAttribute(hwnd, DwmUseImmersiveDarkModeAttribute, &immersiveDarkMode, sizeof(immersiveDarkMode));
	if (FAILED(result)) {
		setWindowAttribute(hwnd, DwmUseImmersiveDarkModeLegacyAttribute, &immersiveDarkMode, sizeof(immersiveDarkMode));
	}

	const COLORREF captionColorRef = colorRefFromQColor(captionColor);
	const COLORREF textColorRef    = colorRefFromQColor(titleTextColor);
	const COLORREF borderColorRef  = colorRefFromQColor(borderColor);
	setWindowAttribute(hwnd, DwmCaptionColorAttribute, &captionColorRef, sizeof(captionColorRef));
	setWindowAttribute(hwnd, DwmTextColorAttribute, &textColorRef, sizeof(textColorRef));
	setWindowAttribute(hwnd, DwmBorderColorAttribute, &borderColorRef, sizeof(borderColorRef));
}
#endif

bool samePersistentChatActor(const MumbleProto::ChatMessage &lhs, const MumbleProto::ChatMessage &rhs) {
	if (lhs.has_actor() || rhs.has_actor()) {
		return lhs.has_actor() && rhs.has_actor() && lhs.actor() == rhs.actor();
	}

	if (lhs.has_actor_user_id() || rhs.has_actor_user_id()) {
		return lhs.has_actor_user_id() && rhs.has_actor_user_id() && lhs.actor_user_id() == rhs.actor_user_id();
	}

	if (lhs.has_actor_name() || rhs.has_actor_name()) {
		return lhs.has_actor_name() && rhs.has_actor_name() && u8(lhs.actor_name()) == u8(rhs.actor_name());
	}

	return true;
}

bool samePersistentChatScope(const MumbleProto::ChatMessage &lhs, const MumbleProto::ChatMessage &rhs) {
	const MumbleProto::ChatScope lhsScope = lhs.has_scope() ? lhs.scope() : MumbleProto::Channel;
	const MumbleProto::ChatScope rhsScope = rhs.has_scope() ? rhs.scope() : MumbleProto::Channel;
	const unsigned int lhsScopeID         = lhs.has_scope_id() ? lhs.scope_id() : 0;
	const unsigned int rhsScopeID         = rhs.has_scope_id() ? rhs.scope_id() : 0;
	return lhsScope == rhsScope && lhsScopeID == rhsScopeID;
}

[[maybe_unused]] bool startsPersistentChatGroup(const std::optional< MumbleProto::ChatMessage > &previousMessage,
												const QDateTime &previousCreatedAt,
												const MumbleProto::ChatMessage &message, const QDateTime &createdAt) {
	if (!previousMessage.has_value()) {
		return true;
	}

	if (previousCreatedAt.date() != createdAt.date()) {
		return true;
	}

	if (!samePersistentChatActor(previousMessage.value(), message)
		|| !samePersistentChatScope(previousMessage.value(), message)) {
		return true;
	}

	if (previousCreatedAt.isValid() && createdAt.isValid() && previousCreatedAt.secsTo(createdAt) > (5 * 60)) {
		return true;
	}

	return false;
}

QString persistentChatActorLabel(const MumbleProto::ChatMessage &msg) {
	if (msg.has_actor_name()) {
		const QString actorName = u8(msg.actor_name()).trimmed();
		if (!actorName.isEmpty()) {
			return actorName;
		}
	}

	if (msg.has_actor()) {
		ClientUser *user = ClientUser::get(msg.actor());
		if (user) {
			return user->qsName;
		}
	}

	if (msg.has_actor_user_id()) {
		return QObject::tr("User %1").arg(msg.actor_user_id());
	}

	return QObject::tr("Unknown user");
}

QString normalizedPersistentChatText(QString text) {
	return text.replace(QLatin1String("\r\n"), QLatin1String("\n")).replace(QLatin1Char('\r'), QLatin1Char('\n'));
}

constexpr int PERSISTENT_CHAT_INLINE_IMAGE_MAX_WIDTH                    = 480;
constexpr int PERSISTENT_CHAT_INLINE_IMAGE_MAX_HEIGHT                   = 320;
constexpr qint64 PERSISTENT_CHAT_INLINE_DATA_IMAGE_RAW_INLINE_MAX_BYTES = 64 * 1024;
constexpr qint64 PERSISTENT_CHAT_INLINE_DATA_IMAGE_THUMBNAIL_MAX_BYTES  = 50 * 1024 * 1024;

QSize persistentChatInlineImageDisplaySize(const QSize &sourceSize) {
	if (!sourceSize.isValid() || sourceSize.isEmpty()) {
		return QSize();
	}

	return sourceSize.scaled(PERSISTENT_CHAT_INLINE_IMAGE_MAX_WIDTH, PERSISTENT_CHAT_INLINE_IMAGE_MAX_HEIGHT,
							 Qt::KeepAspectRatio);
}

QString sanitizePersistentChatInlineStyle(QString style) {
	if (style.trimmed().isEmpty()) {
		return QString();
	}

	static const QRegularExpression s_blockedDeclarationPattern(
		QLatin1String("(?:^|;)\\s*(?:overflow(?:-x|-y)?|text-overflow|max-height|-webkit-line-clamp)\\s*:[^;]*;?"),
		QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression s_duplicateSemicolonPattern(QLatin1String(";{2,}"));
	static const QRegularExpression s_leadingTrailingSemicolonPattern(QLatin1String("^;+|;+$"));

	style.replace(s_blockedDeclarationPattern, QStringLiteral(";"));
	style.replace(s_duplicateSemicolonPattern, QStringLiteral(";"));
	style.remove(s_leadingTrailingSemicolonPattern);

	QStringList declarations;
	for (const QString &segment : style.split(QLatin1Char(';'), Qt::SkipEmptyParts)) {
		const QString declaration = segment.trimmed();
		if (!declaration.isEmpty()) {
			declarations << declaration;
		}
	}

	if (declarations.isEmpty()) {
		return QString();
	}

	return declarations.join(QStringLiteral("; ")) + QLatin1Char(';');
}

QString normalizePersistentChatInlineStyles(QString html) {
	if (!html.contains(QLatin1String("style="), Qt::CaseInsensitive)) {
		return html;
	}

	static const QRegularExpression s_styleAttributePattern(QLatin1String("\\bstyle\\s*=\\s*(['\"])(.*?)\\1"),
															QRegularExpression::CaseInsensitiveOption
																| QRegularExpression::DotMatchesEverythingOption);

	QString normalizedHtml;
	normalizedHtml.reserve(html.size());
	int lastOffset = 0;

	QRegularExpressionMatchIterator it = s_styleAttributePattern.globalMatch(html);
	while (it.hasNext()) {
		const QRegularExpressionMatch match = it.next();
		normalizedHtml += html.mid(lastOffset, match.capturedStart() - lastOffset);

		const QString sanitizedStyle = sanitizePersistentChatInlineStyle(match.captured(2));
		if (!sanitizedStyle.isEmpty()) {
			const QString quote = match.captured(1);
			normalizedHtml += QString::fromLatin1("style=%1%2%1").arg(quote, sanitizedStyle);
		}

		lastOffset = match.capturedEnd();
	}

	normalizedHtml += html.mid(lastOffset);
	return normalizedHtml;
}

QString persistentChatInlineImageStyle(const QString &existingStyle = QString()) {
	QString style = sanitizePersistentChatInlineStyle(existingStyle);
	if (!style.isEmpty() && !style.endsWith(QLatin1Char(';'))) {
		style.append(QLatin1Char(';'));
	}

	style += QString::fromLatin1(
				 "max-width:100%%; max-height:%1px; width:auto; height:auto; border:none; outline:none; display:block; "
				 "margin:0; overflow:visible;")
				 .arg(PERSISTENT_CHAT_INLINE_IMAGE_MAX_HEIGHT);
	return style;
}

struct PersistentChatInlineDataImageInfo {
	bool valid  = false;
	bool base64 = false;
	QString mimeType;
	QString formatLabel;
	qint64 estimatedBytes = -1;
};

using PersistentChatInlineDataImageReplacementBuilder =
	std::function< QString(const QString &, const QString &, const PersistentChatInlineDataImageInfo &) >;

QString persistentChatInlineDataImageFormatLabel(const QString &mimeType) {
	const QString subtype = mimeType.section(QLatin1Char('/'), 1, 1).trimmed().toUpper();
	return subtype.isEmpty() ? QObject::tr("image") : subtype;
}

QString persistentChatInlineDataImageSizeLabel(qint64 bytes) {
	if (bytes < 0) {
		return QString();
	}
	if (bytes >= 1024 * 1024) {
		return QString::fromLatin1("%1 MB").arg(static_cast< double >(bytes) / (1024.0 * 1024.0), 0, 'f', 1);
	}
	if (bytes >= 1024) {
		return QString::fromLatin1("%1 KB").arg(static_cast< double >(bytes) / 1024.0, 0, 'f', 1);
	}

	return QString::fromLatin1("%1 B").arg(bytes);
}

QString persistentChatInlineDataImageToken(const QString &source) {
	return QString::fromLatin1(QCryptographicHash::hash(source.toUtf8(), QCryptographicHash::Sha256).toHex().left(24));
}

bool persistentChatInlineDataLooksLikeImageBytes(const QByteArray &bytes) {
	if (bytes.size() >= 3 && static_cast< unsigned char >(bytes[0]) == 0xFF
		&& static_cast< unsigned char >(bytes[1]) == 0xD8 && static_cast< unsigned char >(bytes[2]) == 0xFF) {
		return true;
	}
	if (bytes.startsWith("\x89PNG\r\n\x1A\n")) {
		return true;
	}
	if (bytes.startsWith("GIF87a") || bytes.startsWith("GIF89a")) {
		return true;
	}
	if (bytes.startsWith("BM")) {
		return true;
	}
	return bytes.size() >= 12 && bytes.startsWith("RIFF") && bytes.mid(8, 4) == "WEBP";
}

QByteArray persistentChatInlineDataNormalizedBase64(QByteArray payload) {
	QByteArray normalized;
	normalized.reserve(payload.size());
	for (char ch : payload) {
		switch (ch) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				break;
			default:
				normalized.push_back(ch);
				break;
		}
	}
	return normalized;
}

PersistentChatInlineDataImageInfo persistentChatInlineDataImageInfo(const QString &source) {
	PersistentChatInlineDataImageInfo info;
	if (!source.startsWith(QLatin1String("data:image/"), Qt::CaseInsensitive)) {
		return info;
	}

	const int commaIndex = source.indexOf(QLatin1Char(','));
	if (commaIndex <= 5) {
		return info;
	}

	const QString metadata  = source.mid(5, commaIndex - 5);
	const QStringList parts = metadata.split(QLatin1Char(';'), Qt::SkipEmptyParts);
	if (parts.isEmpty()) {
		return info;
	}

	info.valid       = true;
	info.mimeType    = parts.front().trimmed().toLower();
	info.formatLabel = persistentChatInlineDataImageFormatLabel(info.mimeType);
	for (int i = 1; i < parts.size(); ++i) {
		if (parts[i].trimmed().compare(QLatin1String("base64"), Qt::CaseInsensitive) == 0) {
			info.base64 = true;
			break;
		}
	}

	if (info.base64) {
		const QString payload = source.mid(commaIndex + 1).trimmed();
		int padding           = 0;
		if (payload.endsWith(QLatin1String("=="))) {
			padding = 2;
		} else if (payload.endsWith(QLatin1Char('='))) {
			padding = 1;
		}
		info.estimatedBytes = std::max< qint64 >(0, (static_cast< qint64 >(payload.size()) * 3) / 4 - padding);
	}

	return info;
}

QByteArray persistentChatInlineDataImageBytes(const QString &source, const PersistentChatInlineDataImageInfo &info) {
	const int commaIndex = source.indexOf(QLatin1Char(','));
	if (!info.valid || commaIndex < 0) {
		return QByteArray();
	}

	const QByteArray payload = source.mid(commaIndex + 1).trimmed().toLatin1();
	if (payload.isEmpty()) {
		return QByteArray();
	}

	if (!info.base64) {
		const QByteArray bytes = QByteArray::fromPercentEncoding(payload);
		if (mumble::chatperf::enabled()) {
			mumble::chatperf::recordNote("chat.inline_data_image.decode",
										 QString::fromLatin1("mode=percent bytes=%1").arg(bytes.size()));
		}
		return bytes;
	}

	QByteArray normalizedPayload = payload;
	if (normalizedPayload.contains('%')) {
		normalizedPayload = QByteArray::fromPercentEncoding(normalizedPayload);
	}
	if (persistentChatInlineDataLooksLikeImageBytes(normalizedPayload)) {
		if (mumble::chatperf::enabled()) {
			mumble::chatperf::recordNote(
				"chat.inline_data_image.decode",
				QString::fromLatin1("mode=percent_raw bytes=%1").arg(normalizedPayload.size()));
		}
		return normalizedPayload;
	}

	QByteArray decodedBytes = QByteArray::fromBase64(persistentChatInlineDataNormalizedBase64(normalizedPayload));
	if (!decodedBytes.isEmpty()) {
		if (mumble::chatperf::enabled()) {
			mumble::chatperf::recordNote("chat.inline_data_image.decode",
										 QString::fromLatin1("mode=base64_normalized bytes=%1 percent=%2")
											 .arg(decodedBytes.size())
											 .arg(payload.contains('%') ? 1 : 0));
		}
		return decodedBytes;
	}

	decodedBytes = QByteArray::fromBase64(persistentChatInlineDataNormalizedBase64(payload));
	if (!decodedBytes.isEmpty()) {
		if (mumble::chatperf::enabled()) {
			mumble::chatperf::recordNote("chat.inline_data_image.decode",
										 QString::fromLatin1("mode=base64_raw bytes=%1").arg(decodedBytes.size()));
		}
		return decodedBytes;
	}

	if (mumble::chatperf::enabled()) {
		mumble::chatperf::recordNote(
			"chat.inline_data_image.decode",
			QString::fromLatin1("mode=failed prefix=\"%1\"").arg(QString::fromLatin1(payload.left(32))));
	}
	return QByteArray();
}

QImage persistentChatInlineDataImagePreviewImage(const QString &source, const PersistentChatInlineDataImageInfo &info) {
	if (info.estimatedBytes > PERSISTENT_CHAT_INLINE_DATA_IMAGE_THUMBNAIL_MAX_BYTES) {
		if (mumble::chatperf::enabled()) {
			mumble::chatperf::recordNote("chat.inline_data_image.thumbnail_skipped",
										 QString::fromLatin1("estimated=%1 max=%2")
											 .arg(info.estimatedBytes)
											 .arg(PERSISTENT_CHAT_INLINE_DATA_IMAGE_THUMBNAIL_MAX_BYTES));
		}
		return QImage();
	}

	const QByteArray bytes = persistentChatInlineDataImageBytes(source, info);
	if (bytes.isEmpty()) {
		return QImage();
	}

	QBuffer buffer;
	buffer.setData(bytes);
	if (!buffer.open(QIODevice::ReadOnly)) {
		return QImage();
	}

	QImageReader reader(&buffer);
	reader.setAutoTransform(true);
	QImage image = reader.read();
	if (image.isNull()) {
		return QImage();
	}

	const QSize boundedSize = persistentChatInlineImageDisplaySize(image.size());
	if (boundedSize.isValid() && image.size() != boundedSize) {
		image = image.scaled(boundedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	return image;
}

[[maybe_unused]] QString persistentChatInlineDataImageThumbnailSource(const QImage &image) {
	if (image.isNull()) {
		return QString();
	}

	QByteArray encodedBytes;
	QBuffer buffer(&encodedBytes);
	if (!buffer.open(QIODevice::WriteOnly)) {
		return QString();
	}

	const bool hasAlpha = image.hasAlphaChannel();
	const char *format  = hasAlpha ? "PNG" : "JPEG";
	const int quality   = hasAlpha ? -1 : 82;
	if (!image.save(&buffer, format, quality) || encodedBytes.isEmpty()) {
		return QString();
	}

	const QString mimeType = hasAlpha ? QStringLiteral("image/png") : QStringLiteral("image/jpeg");
	if (mumble::chatperf::enabled()) {
		mumble::chatperf::recordNote("chat.inline_data_image.thumbnail",
									 QString::fromLatin1("mime=%1 bytes=%2 width=%3 height=%4")
										 .arg(mimeType)
										 .arg(encodedBytes.size())
										 .arg(image.width())
										 .arg(image.height()));
	}
	return QString::fromLatin1("data:%1;base64,%2").arg(mimeType, QString::fromLatin1(encodedBytes.toBase64()));
}

QString persistentChatInlineDataImageThumbnailHtml(const QString &imageSrc, const QString &openHref,
												   const QString &altText, const QSize &imageSize,
												   qint64 estimatedBytes) {
	const QString title = !altText.trimmed().isEmpty() ? altText.trimmed().toHtmlEscaped()
													   : QObject::tr("Embedded image").toHtmlEscaped();
	QString caption = title;
	if (const QString sizeLabel = persistentChatInlineDataImageSizeLabel(estimatedBytes); !sizeLabel.isEmpty()) {
		caption += QString::fromLatin1(" (%1)").arg(sizeLabel.toHtmlEscaped());
	}
	caption += QString::fromLatin1(" - ") + QObject::tr("Click to open or save.").toHtmlEscaped();

	const int boundedWidth  = std::max(1, imageSize.width());
	const int boundedHeight = std::max(1, std::min(imageSize.height(), PERSISTENT_CHAT_INLINE_IMAGE_MAX_HEIGHT));
	QString imageHtml =
		QString::fromLatin1("<img src=\"%1\" alt=\"%2\" "
							"width=\"%3\" height=\"%4\" "
							"style=\"display:block; width:%3px; height:%4px; max-width:%3px; max-height:%4px; "
							"border:none; outline:none; margin:0; background:transparent;\" />")
			.arg(imageSrc.toHtmlEscaped())
			.arg(altText.toHtmlEscaped())
			.arg(boundedWidth)
			.arg(boundedHeight);
	if (!openHref.isEmpty()) {
		imageHtml =
			QString::fromLatin1(
				"<a href=\"%1\" style=\"display:block; text-decoration:none; color:inherit; line-height:0;\">%2</a>")
				.arg(openHref.toHtmlEscaped(), imageHtml);
	}
	imageHtml = QString::fromLatin1("<table cellspacing='0' cellpadding='0' width='100%%' "
									"style='border-collapse:collapse; border:none; background:transparent;'>"
									"<tr><td align='left' style='padding:0; line-height:0;'>%1</td></tr></table>")
					.arg(imageHtml);

	return QString::fromLatin1("<div style='margin:6px 0 2px 0;'>%1</div>"
							   "<div style='margin-top:4px; opacity:0.78; font-size:0.92em;'>%2</div>")
		.arg(imageHtml, caption);
}

QString persistentChatInlineDataImagePlaceholderHtml(const PersistentChatInlineDataImageInfo &info,
													 const QString &altText, const QString &openHref) {
	const QString title = !altText.trimmed().isEmpty()
							  ? altText.trimmed().toHtmlEscaped()
							  : QObject::tr("Embedded %1 image").arg(info.formatLabel).toHtmlEscaped();

	QString detail = info.estimatedBytes > PERSISTENT_CHAT_INLINE_DATA_IMAGE_THUMBNAIL_MAX_BYTES
						 ? QObject::tr("Large image attachment")
						 : QObject::tr("Image attachment");
	if (const QString sizeLabel = persistentChatInlineDataImageSizeLabel(info.estimatedBytes); !sizeLabel.isEmpty()) {
		detail += QString::fromLatin1(" (%1)").arg(sizeLabel);
	}
	detail += QString::fromLatin1(". ") + QObject::tr("Open image to view or save.");

	QString actionRow;
	if (!openHref.isEmpty()) {
		actionRow = QString::fromLatin1("<div style='margin-top:6px;'><a href=\"%1\" "
										"style='font-weight:600; text-decoration:none;'>%2</a></div>")
						.arg(openHref.toHtmlEscaped(), QObject::tr("Open image").toHtmlEscaped());
	}

	return QString::fromLatin1("<div style='margin:6px 0; padding:8px 10px; border-left:3px solid #8b93a6; "
							   "background:rgba(139,147,166,0.10); border-radius:4px;'>"
							   "<div style='font-weight:600;'>%1</div>"
							   "<div style='margin-top:3px; opacity:0.78;'>%2</div>"
							   "%3"
							   "</div>")
		.arg(title, detail.toHtmlEscaped(), actionRow);
}

QString normalizePersistentChatInlineImages(
	QString html, const PersistentChatInlineDataImageReplacementBuilder &buildInlineDataImageReplacement = {}) {
	if (!html.contains(QLatin1String("<img"), Qt::CaseInsensitive)) {
		return html;
	}

	static const QRegularExpression s_imgTagPattern(QLatin1String("<img\\b[^>]*>"),
													QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression s_srcPattern(QLatin1String("\\bsrc\\s*=\\s*(['\"])(.*?)\\1"),
												 QRegularExpression::CaseInsensitiveOption
													 | QRegularExpression::DotMatchesEverythingOption);
	static const QRegularExpression s_altPattern(QLatin1String("\\balt\\s*=\\s*(['\"])(.*?)\\1"),
												 QRegularExpression::CaseInsensitiveOption
													 | QRegularExpression::DotMatchesEverythingOption);
	static const QRegularExpression s_widthPattern(QLatin1String("\\bwidth\\s*=\\s*(['\"]?)(\\d+)\\1"),
												   QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression s_heightPattern(QLatin1String("\\bheight\\s*=\\s*(['\"]?)(\\d+)\\1"),
													QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression s_stylePattern(QLatin1String("\\bstyle\\s*=\\s*(['\"])(.*?)\\1"),
												   QRegularExpression::CaseInsensitiveOption
													   | QRegularExpression::DotMatchesEverythingOption);
	QString normalizedHtml;
	normalizedHtml.reserve(html.size() + 128);
	int lastOffset = 0;

	QRegularExpressionMatchIterator it = s_imgTagPattern.globalMatch(html);
	while (it.hasNext()) {
		const QRegularExpressionMatch match = it.next();
		normalizedHtml += html.mid(lastOffset, match.capturedStart() - lastOffset);

		QString tag                            = match.captured(0);
		const QRegularExpressionMatch srcMatch = s_srcPattern.match(tag);
		if (srcMatch.hasMatch()) {
			const PersistentChatInlineDataImageInfo dataImageInfo =
				persistentChatInlineDataImageInfo(srcMatch.captured(2));
			const bool shouldUseReplacement =
				buildInlineDataImageReplacement || dataImageInfo.estimatedBytes < 0
				|| dataImageInfo.estimatedBytes > PERSISTENT_CHAT_INLINE_DATA_IMAGE_RAW_INLINE_MAX_BYTES;
			if (dataImageInfo.valid && shouldUseReplacement) {
				const QRegularExpressionMatch altMatch = s_altPattern.match(tag);
				const QString altText                  = altMatch.hasMatch() ? altMatch.captured(2) : QString();
				const QString replacementHtml =
					buildInlineDataImageReplacement
						? buildInlineDataImageReplacement(srcMatch.captured(2), altText, dataImageInfo)
						: QString();
				if (!replacementHtml.isEmpty()) {
					normalizedHtml += replacementHtml;
				} else {
					normalizedHtml += persistentChatInlineDataImagePlaceholderHtml(dataImageInfo, altText, QString());
				}
				mumble::chatperf::recordValue("chat.inline_data_image.replaced", 1);
				lastOffset = match.capturedEnd();
				continue;
			}
		}

		const QRegularExpressionMatch widthMatch  = s_widthPattern.match(tag);
		const QRegularExpressionMatch heightMatch = s_heightPattern.match(tag);
		if (widthMatch.hasMatch() && heightMatch.hasMatch()) {
			const QSize boundedSize = persistentChatInlineImageDisplaySize(
				QSize(widthMatch.captured(2).toInt(), heightMatch.captured(2).toInt()));
			if (boundedSize.isValid()) {
				tag.replace(s_widthPattern, QString::fromLatin1("width=\"%1\"").arg(boundedSize.width()));
				tag.replace(s_heightPattern, QString::fromLatin1("height=\"%1\"").arg(boundedSize.height()));
			}
		}

		const QRegularExpressionMatch styleMatch = s_stylePattern.match(tag);
		if (styleMatch.hasMatch()) {
			const QString quote = styleMatch.captured(1);
			tag.replace(
				styleMatch.capturedStart(), styleMatch.capturedLength(),
				QString::fromLatin1("style=%1%2%1").arg(quote, persistentChatInlineImageStyle(styleMatch.captured(2))));
		} else {
			tag.insert(tag.size() - 1, QString::fromLatin1(" style=\"%1\"").arg(persistentChatInlineImageStyle()));
		}

		normalizedHtml += tag;
		lastOffset = match.capturedEnd();
	}

	normalizedHtml += html.mid(lastOffset);
	return normalizedHtml;
}

QString persistentChatLinkedPlainTextHtml(const QString &content) {
	static const QRegularExpression s_urlRegex(QLatin1String("(https?://[^\\s<>\"']+)"),
											   QRegularExpression::CaseInsensitiveOption);
	QString html;
	int lastOffset            = 0;
	auto appendEscapedSegment = [&html](const QString &segment) {
		html += segment.toHtmlEscaped().replace(QLatin1Char('\n'), QLatin1String("<br/>"));
	};

	for (QRegularExpressionMatchIterator it = s_urlRegex.globalMatch(content); it.hasNext();) {
		const QRegularExpressionMatch match = it.next();
		appendEscapedSegment(content.mid(lastOffset, match.capturedStart() - lastOffset));
		const QString urlText = match.captured(1);
		html += QString::fromLatin1("<a href=\"%1\">%2</a>").arg(urlText.toHtmlEscaped(), urlText.toHtmlEscaped());
		lastOffset = match.capturedEnd();
	}

	appendEscapedSegment(content.mid(lastOffset));
	return html;
}

QString persistentChatContentHtml(
	const QString &content,
	const PersistentChatInlineDataImageReplacementBuilder &buildInlineDataImageReplacement = {}) {
	const QString normalizedContent = normalizedPersistentChatText(content);
	if (!Qt::mightBeRichText(normalizedContent)) {
		return persistentChatLinkedPlainTextHtml(normalizedContent);
	}

	const QString sanitizedHtml = normalizePersistentChatInlineStyles(Log::validHtml(normalizedContent));
	static const QRegularExpression bodyPattern(QLatin1String("<body[^>]*>(.*)</body>"),
												QRegularExpression::DotMatchesEverythingOption
													| QRegularExpression::CaseInsensitiveOption);
	const QRegularExpressionMatch bodyMatch = bodyPattern.match(sanitizedHtml);
	if (bodyMatch.hasMatch()) {
		return normalizePersistentChatInlineImages(bodyMatch.captured(1), buildInlineDataImageReplacement);
	}

	return normalizePersistentChatInlineImages(sanitizedHtml, buildInlineDataImageReplacement);
}

QString persistentChatPlainTextSummary(const QString &content, int maxLength = 160) {
	const QString plainText =
		QTextDocumentFragment::fromHtml(persistentChatContentHtml(content)).toPlainText().simplified();
	if (plainText.size() <= maxLength) {
		return plainText;
	}

	return plainText.left(std::max(0, maxLength - 3)).trimmed() + QLatin1String("...");
}

bool persistentChatBodyHtmlContainsInlineImageThumbnail(const QString &bodyHtml) {
	return bodyHtml.contains(QLatin1String("mumble-chat://inline-data-image"), Qt::CaseInsensitive);
}

QString persistentChatCondensedBodyHtml(const QString &bodyHtml, const QString &sourceText) {
	constexpr int kMaxRenderedBodyHtmlChars = 16384;
	constexpr int kPreviewPlainTextChars    = 1200;
	if (bodyHtml.size() <= kMaxRenderedBodyHtmlChars || persistentChatBodyHtmlContainsInlineImageThumbnail(bodyHtml)) {
		return bodyHtml;
	}

	const QString previewSource = sourceText.isEmpty() ? bodyHtml : sourceText;
	const QString previewText   = persistentChatPlainTextSummary(previewSource, kPreviewPlainTextChars);
	return QString::fromLatin1("<div>%1</div>"
							   "<div style='margin-top:6px; opacity:0.78;'><em>%2</em></div>")
		.arg(previewText.toHtmlEscaped().replace(QLatin1Char('\n'), QLatin1String("<br/>")),
			 QObject::tr("[Long message preview. Use Copy message for the full text.]").toHtmlEscaped());
}

bool persistentChatBodyTextContainsLegacyInlineImageHtml(const QString &bodyText) {
	return bodyText.contains(QLatin1String("<img"), Qt::CaseInsensitive)
		   && bodyText.contains(QLatin1String("data:image"), Qt::CaseInsensitive);
}

QString persistentChatMessageSourceText(const MumbleProto::ChatMessage &message) {
	if (message.has_body_text()) {
		return normalizedPersistentChatText(u8(message.body_text()));
	}

	return QTextDocumentFragment::fromHtml(u8(message.message())).toPlainText();
}

std::optional< QString > persistentChatSystemMessageText(const MumbleProto::ChatMessage &message) {
	const QString sourceText = persistentChatMessageSourceText(message).trimmed();
	static const QRegularExpression s_systemPrefixPattern(QLatin1String("^\\[(stack|scope|system)\\]\\s*"),
														  QRegularExpression::CaseInsensitiveOption);
	if (!s_systemPrefixPattern.match(sourceText).hasMatch()) {
		return std::nullopt;
	}

	QString systemText = sourceText;
	systemText.remove(s_systemPrefixPattern);
	return systemText.isEmpty() ? std::optional< QString >(QObject::tr("System event"))
								: std::optional< QString >(systemText);
}

QString persistentChatMessageRawBody(const MumbleProto::ChatMessage &message) {
	if (message.has_body_text()) {
		return normalizedPersistentChatText(u8(message.body_text()));
	}

	return u8(message.message());
}

QString persistentChatMessageBodyHtml(
	const MumbleProto::ChatMessage &message,
	const PersistentChatInlineDataImageReplacementBuilder &buildInlineDataImageReplacement = {}) {
	if (message.has_body_text()) {
		const QString bodyText = normalizedPersistentChatText(u8(message.body_text()));
		if (persistentChatBodyTextContainsLegacyInlineImageHtml(bodyText)) {
			return persistentChatContentHtml(bodyText, buildInlineDataImageReplacement);
		}
		if (message.has_body_format() && message.body_format() == MumbleProto::ChatBodyFormatMarkdownLite) {
			return persistentChatContentHtml(Markdown::markdownToHTML(bodyText), buildInlineDataImageReplacement);
		}

		return persistentChatContentHtml(bodyText, buildInlineDataImageReplacement);
	}

	return persistentChatContentHtml(u8(message.message()), buildInlineDataImageReplacement);
}

[[maybe_unused]] QString mirroredServerLogHtml(const QString &html) {
	const QString fragmentHtml = persistentChatContentHtml(html);
	return QString::fromLatin1("<div style='margin:0; padding:0; border:none; background:transparent;'>%1</div>")
		.arg(fragmentHtml);
}

void insertPersistentChatContent(QTextCursor &cursor, const QString &content) {
	const QString fragmentHtml = persistentChatContentHtml(content);
	if (!fragmentHtml.isEmpty()) {
		cursor.insertHtml(fragmentHtml);
	}
}

QString persistentChatDocumentStylesheet(const QString &baseStylesheet) {
	return baseStylesheet
		   + QString::fromLatin1(
				 "html, body { margin: 0; padding: 0; border: 0; background: transparent; }"
				 "body, table, tr, td, div, span, p, a { font-size: 0.875em; line-height: 1.35; }"
				 "p { margin-top: 0; margin-bottom: 4px; }"
				 "table, tr, td { margin: 0; padding: 0; border: none; background: transparent; }"
				 "img { border: none; outline: none; display: block; margin: 0; max-width: %1px; max-height: %2px;"
				 " width: auto; height: auto; background: transparent; }")
				 .arg(PERSISTENT_CHAT_INLINE_IMAGE_MAX_WIDTH)
				 .arg(PERSISTENT_CHAT_INLINE_IMAGE_MAX_HEIGHT);
}

void configurePersistentChatDocument(QTextDocument *document, const QString &baseStylesheet) {
	if (!document) {
		return;
	}

	document->setDocumentMargin(0);
	document->setDefaultStyleSheet(persistentChatDocumentStylesheet(baseStylesheet));
	if (QTextFrame *rootFrame = document->rootFrame()) {
		QTextFrameFormat rootFrameFormat = rootFrame->frameFormat();
		rootFrameFormat.setBorder(0);
		rootFrameFormat.setMargin(0);
		rootFrameFormat.setPadding(0);
		rootFrame->setFrameFormat(rootFrameFormat);
	}
}

QString persistentTextAclChannelLabel(const Channel *channel) {
	if (!channel) {
		return QObject::tr("Unknown channel");
	}

	QStringList segments;
	for (const Channel *current = channel; current; current = current->cParent) {
		segments.prepend(current->qsName);
	}

	return segments.join(QString::fromLatin1(" / "));
}

QString trimTrailingUrlPunctuation(QString url) {
	while (!url.isEmpty()) {
		const QChar last = url.back();
		if (QString::fromLatin1(".,!?;:)]}>").contains(last)) {
			url.chop(1);
		} else {
			break;
		}
	}

	return url;
}

int countVisibleUserTreeRows(const QTreeView *tree, const QModelIndex &parent = QModelIndex()) {
	if (!tree || !tree->model()) {
		return 0;
	}

	int visibleRows                 = 0;
	const QAbstractItemModel *model = tree->model();
	const int rowCount              = model->rowCount(parent);
	for (int row = 0; row < rowCount; ++row) {
		if (tree->isRowHidden(row, parent)) {
			continue;
		}

		const QModelIndex index = model->index(row, 0, parent);
		if (!index.isValid()) {
			continue;
		}

		++visibleRows;
		if (tree->isExpanded(index)) {
			visibleRows += countVisibleUserTreeRows(tree, index);
		}
	}

	return visibleRows;
}

void setDockSplitterHandleWidth(QWidget *root, int width) {
	if (!root) {
		return;
	}

	for (QSplitter *splitter : root->findChildren< QSplitter * >()) {
		splitter->setHandleWidth(width);
	}
}

std::optional< QString > extractYouTubeVideoId(const QUrl &url) {
	if (!url.isValid()) {
		return std::nullopt;
	}

	QString host = url.host().toLower();
	if (host.startsWith(QLatin1String("www."))) {
		host.remove(0, 4);
	}
	if (host.startsWith(QLatin1String("m."))) {
		host.remove(0, 2);
	}

	QString videoId;
	const QStringList pathSegments = url.path().split(QLatin1Char('/'), Qt::SkipEmptyParts);
	if (host == QLatin1String("youtu.be")) {
		if (!pathSegments.isEmpty()) {
			videoId = pathSegments.front();
		}
	} else if (host == QLatin1String("youtube.com") || host == QLatin1String("youtube-nocookie.com")) {
		const QString path = url.path();
		QUrlQuery query(url);
		if (path == QLatin1String("/watch")) {
			videoId = query.queryItemValue(QLatin1String("v"));
		} else if (!pathSegments.isEmpty()) {
			if (pathSegments.front() == QLatin1String("shorts") || pathSegments.front() == QLatin1String("embed")
				|| pathSegments.front() == QLatin1String("live")) {
				if (pathSegments.size() > 1) {
					videoId = pathSegments.at(1);
				}
			}
		}
	}

	static const QRegularExpression s_videoIdPattern(
		QRegularExpression::anchoredPattern(QLatin1String("[A-Za-z0-9_-]{11}")));
	if (!s_videoIdPattern.match(videoId).hasMatch()) {
		return std::nullopt;
	}

	return videoId;
}

bool isYouTubeHost(QString host) {
	host = host.toLower();
	if (host.startsWith(QLatin1String("www."))) {
		host.remove(0, 4);
	}
	if (host.startsWith(QLatin1String("m."))) {
		host.remove(0, 2);
	}
	if (host.startsWith(QLatin1String("mobile."))) {
		host.remove(0, 7);
	}

	return host == QLatin1String("youtube.com") || host == QLatin1String("youtube-nocookie.com")
		   || host == QLatin1String("youtu.be");
}

struct PersistentChatPreviewProviderInfo {
	QString siteLabel;
	QString openLabel;
	QString fallbackTitle;
};

bool isDirectImageUrl(const QUrl &url) {
	if (!url.isValid()) {
		return false;
	}

	const QString path = url.path().toLower();
	return path.endsWith(QLatin1String(".png")) || path.endsWith(QLatin1String(".jpg"))
		   || path.endsWith(QLatin1String(".jpeg")) || path.endsWith(QLatin1String(".gif"))
		   || path.endsWith(QLatin1String(".webp")) || path.endsWith(QLatin1String(".bmp"));
}

QString normalizedPreviewUrl(const QUrl &url) {
	QUrl normalized = url.adjusted(QUrl::NormalizePathSegments | QUrl::RemoveFragment);
	return normalized.toString(QUrl::FullyEncoded);
}

QString previewDisplayHost(const QUrl &url) {
	QString host = url.host().toLower();
	if (host.startsWith(QLatin1String("www."))) {
		host.remove(0, 4);
	}
	return host;
}

std::optional< PersistentChatPreviewProviderInfo > previewProviderInfo(const QUrl &url) {
	const QString host = previewDisplayHost(url);
	if (host == QLatin1String("open.spotify.com") || host == QLatin1String("spotify.link")) {
		return PersistentChatPreviewProviderInfo{ QObject::tr("Spotify"), QObject::tr("Open on Spotify"),
												  QObject::tr("Spotify link") };
	}
	if (host == QLatin1String("patreon.com")) {
		return PersistentChatPreviewProviderInfo{ QObject::tr("Patreon"), QObject::tr("Open on Patreon"),
												  QObject::tr("Patreon post") };
	}
	if (host == QLatin1String("reddit.com") || host == QLatin1String("old.reddit.com")
		|| host == QLatin1String("redd.it")) {
		return PersistentChatPreviewProviderInfo{ QObject::tr("Reddit"), QObject::tr("Open on Reddit"),
												  QObject::tr("Reddit post") };
	}
	if (host == QLatin1String("twitter.com") || host == QLatin1String("x.com")
		|| host == QLatin1String("mobile.twitter.com")) {
		return PersistentChatPreviewProviderInfo{ QObject::tr("Twitter/X"), QObject::tr("Open on X"),
												  QObject::tr("Post on X") };
	}
	if (host == QLatin1String("imgur.com") || host == QLatin1String("i.imgur.com")) {
		return PersistentChatPreviewProviderInfo{ QObject::tr("Imgur"), QObject::tr("Open on Imgur"),
												  QObject::tr("Imgur link") };
	}
	if (isYouTubeHost(url.host())) {
		return PersistentChatPreviewProviderInfo{ QObject::tr("YouTube"), QObject::tr("Open on YouTube"),
												  QObject::tr("YouTube video") };
	}

	return std::nullopt;
}

bool isPrivateOrLocalAddress(const QHostAddress &address) {
	if (address.isNull() || address.isLoopback() || address.isMulticast()) {
		return true;
	}

	bool isIPv4               = false;
	const quint32 ipv4Address = address.toIPv4Address(&isIPv4);
	if (isIPv4) {
		const quint8 firstOctet  = static_cast< quint8 >((ipv4Address >> 24) & 0xff);
		const quint8 secondOctet = static_cast< quint8 >((ipv4Address >> 16) & 0xff);
		if (firstOctet == 0 || firstOctet == 10 || firstOctet == 127) {
			return true;
		}
		if (firstOctet == 169 && secondOctet == 254) {
			return true;
		}
		if (firstOctet == 172 && secondOctet >= 16 && secondOctet <= 31) {
			return true;
		}
		if (firstOctet == 192 && secondOctet == 168) {
			return true;
		}
		if (firstOctet == 100 && secondOctet >= 64 && secondOctet <= 127) {
			return true;
		}

		return false;
	}

	const Q_IPV6ADDR ipv6Address = address.toIPv6Address();
	if ((ipv6Address.c[0] & 0xfe) == 0xfc) {
		return true;
	}
	if (ipv6Address.c[0] == 0xfe && (ipv6Address.c[1] & 0xc0) == 0x80) {
		return true;
	}
	if (ipv6Address.c[0] == 0xfe && (ipv6Address.c[1] & 0xc0) == 0xc0) {
		return true;
	}

	return false;
}

bool isSafePreviewTarget(const QUrl &url) {
	if (!url.isValid()) {
		return false;
	}

	const QString scheme = url.scheme().toLower();
	if (scheme != QLatin1String("http") && scheme != QLatin1String("https")) {
		return false;
	}
	if (!url.userName().isEmpty() || !url.password().isEmpty()) {
		return false;
	}

	const QString host = previewDisplayHost(url);
	if (host.isEmpty()) {
		return false;
	}
	if (host == QLatin1String("localhost") || host.endsWith(QLatin1String(".localhost"))
		|| host.endsWith(QLatin1String(".local")) || host.endsWith(QLatin1String(".lan"))
		|| host.endsWith(QLatin1String(".internal")) || host.endsWith(QLatin1String(".home.arpa"))) {
		return false;
	}

	QHostAddress literalAddress;
	if (literalAddress.setAddress(host)) {
		return !isPrivateOrLocalAddress(literalAddress);
	}

	if (!host.contains(QLatin1Char('.'))) {
		return false;
	}

	return true;
}

constexpr int PREVIEW_REQUEST_TIMEOUT_MSEC             = 8000;
constexpr qint64 PREVIEW_MAX_PAGE_BYTES                = 512 * 1024;
constexpr qint64 PREVIEW_MAX_IMAGE_BYTES               = 4 * 1024 * 1024;
constexpr int PERSISTENT_CHAT_RESIZE_RENDER_DELAY_MSEC = 120;
constexpr int PERSISTENT_CHAT_PREVIEW_SOURCE_WIDTH     = 640;
constexpr int PERSISTENT_CHAT_PREVIEW_SOURCE_HEIGHT    = 480;
constexpr int PERSISTENT_CHAT_PREVIEW_DISPLAY_WIDTH    = 360;
constexpr int PERSISTENT_CHAT_PREVIEW_DISPLAY_HEIGHT   = 300;
constexpr int PERSISTENT_CHAT_PREVIEW_CARD_MAX_WIDTH   = 480;
constexpr int PERSISTENT_CHAT_PREVIEW_WIDTH_STEP       = 16;
static const QByteArray s_previewBrowserUserAgent =
	QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
					  "(KHTML, like Gecko) Chrome/135.0.0.0 Safari/537.36");
static const QByteArray s_previewAcceptHeader =
	QByteArrayLiteral("text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/*,*/*;q=0.8");
static const QByteArray s_previewAcceptLanguageHeader = QByteArrayLiteral("en-US,en;q=0.9");

void preparePreviewRequest(QNetworkRequest &request) {
	Network::prepareRequest(request);
	request.setRawHeader(QByteArrayLiteral("User-Agent"), s_previewBrowserUserAgent);
	request.setRawHeader(QByteArrayLiteral("Accept"), s_previewAcceptHeader);
	request.setRawHeader(QByteArrayLiteral("Accept-Language"), s_previewAcceptLanguageHeader);
}

void setPreviewAbortReason(QNetworkReply *reply, const QString &reason) {
	if (reply) {
		reply->setProperty("previewAbortReason", reason);
	}
}

QString previewAbortReason(const QNetworkReply *reply) {
	return reply ? reply->property("previewAbortReason").toString() : QString();
}

QString previewFailureText(const QNetworkReply *reply) {
	const QString abortReason = previewAbortReason(reply);
	if (abortReason == QLatin1String("timeout")) {
		return QObject::tr("Preview request timed out");
	}
	if (abortReason == QLatin1String("too_large")) {
		return QObject::tr("Preview exceeded size limit");
	}

	return QObject::tr("Preview unavailable");
}

bool previewContentTypeLooksHtml(const QString &contentType) {
	return contentType.isEmpty() || contentType.contains(QLatin1String("html"))
		   || contentType.contains(QLatin1String("xml"));
}

QString previewImageMetaTag(const QHash< QString, QString > &metaTags) {
	static const QStringList preferredKeys = {
		QStringLiteral("og:image"),      QStringLiteral("og:image:url"),      QStringLiteral("og:image:secure_url"),
		QStringLiteral("twitter:image"), QStringLiteral("twitter:image:src"),
	};

	for (const QString &key : preferredKeys) {
		const QString value = metaTags.value(key).trimmed();
		if (!value.isEmpty()) {
			return value;
		}
	}

	return QString();
}

void applyPreviewReplyGuards(QNetworkReply *reply, qint64 maxBytes, bool abortOnContentLength = true) {
	if (!reply) {
		return;
	}

	QTimer *timeoutTimer = new QTimer(reply);
	timeoutTimer->setSingleShot(true);
	timeoutTimer->setInterval(PREVIEW_REQUEST_TIMEOUT_MSEC);
	QObject::connect(timeoutTimer, &QTimer::timeout, reply, [reply]() {
		if (!reply->isFinished()) {
			setPreviewAbortReason(reply, QLatin1String("timeout"));
			reply->abort();
		}
	});
	QObject::connect(reply, &QNetworkReply::finished, timeoutTimer, &QTimer::stop);
	timeoutTimer->start();

	if (maxBytes > 0) {
		if (abortOnContentLength) {
			QObject::connect(reply, &QNetworkReply::metaDataChanged, reply, [reply, maxBytes]() {
				const qint64 contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
				if (contentLength > maxBytes && !reply->isFinished()) {
					setPreviewAbortReason(reply, QLatin1String("too_large"));
					reply->abort();
				}
			});
		}
		QObject::connect(reply, &QNetworkReply::downloadProgress, reply, [reply, maxBytes](qint64 received, qint64) {
			if (received > maxBytes && !reply->isFinished()) {
				setPreviewAbortReason(reply, QLatin1String("too_large"));
				reply->abort();
			}
		});
	}
}

#if defined(MUMBLE_HAS_MODERN_LAYOUT)
class PreviewSnapshotUrlInterceptor final : public QWebEngineUrlRequestInterceptor {
public:
	explicit PreviewSnapshotUrlInterceptor(QObject *parent = nullptr) : QWebEngineUrlRequestInterceptor(parent) {}

	void interceptRequest(QWebEngineUrlRequestInfo &info) override {
		const QUrl url       = info.requestUrl();
		const QString scheme = url.scheme().toLower();
		if (scheme == QLatin1String("about") || scheme == QLatin1String("data") || scheme == QLatin1String("blob")) {
			return;
		}

		if (!isSafePreviewTarget(url)) {
			info.block(true);
			return;
		}

		info.setHttpHeader(QByteArrayLiteral("User-Agent"), s_previewBrowserUserAgent);
		info.setHttpHeader(QByteArrayLiteral("Accept-Language"), s_previewAcceptLanguageHeader);
		if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame
			|| info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeSubFrame) {
			info.setHttpHeader(QByteArrayLiteral("Accept"), s_previewAcceptHeader);
		}
	}
};

class PersistentChatPreviewSnapshotRenderer final : public QObject {
public:
	using ResultCallback = std::function< void(const QString &, const QImage &, bool) >;

	explicit PersistentChatPreviewSnapshotRenderer(QObject *parent = nullptr) : QObject(parent) {
		m_timeoutTimer = new QTimer(this);
		m_timeoutTimer->setSingleShot(true);
		m_timeoutTimer->setInterval(15000);
		QObject::connect(m_timeoutTimer, &QTimer::timeout, this, [this]() { finishCurrent(false, QImage()); });

		m_settleTimer = new QTimer(this);
		m_settleTimer->setSingleShot(true);
		m_settleTimer->setInterval(1200);
		QObject::connect(m_settleTimer, &QTimer::timeout, this, [this]() { captureCurrentView(); });
	}

	~PersistentChatPreviewSnapshotRenderer() override {
		if (m_view) {
			m_view->hide();
			m_view->close();
			delete m_view;
			m_view = nullptr;
			m_page = nullptr;
		}
		if (m_profile) {
			delete m_profile;
			m_profile     = nullptr;
			m_interceptor = nullptr;
		}
	}

	void setResultCallback(ResultCallback callback) { m_resultCallback = std::move(callback); }

	void requestSnapshot(const QString &previewKey, const QUrl &url) {
		if (previewKey.trimmed().isEmpty() || !isSafePreviewTarget(url)) {
			return;
		}

		if ((m_busy && m_current.previewKey == previewKey) || m_queuedPreviewKeys.contains(previewKey)) {
			return;
		}

		Request request;
		request.previewKey = previewKey;
		request.url        = url;
		m_queue.push_back(request);
		m_queuedPreviewKeys.insert(previewKey);
		startNextIfIdle();
	}

private:
	struct Request {
		QString previewKey;
		QUrl url;
	};

	static constexpr int kViewportWidth         = 960;
	static constexpr int kViewportHeight        = 720;
	static constexpr int kCaptureRetryDelayMsec = 250;
	static constexpr int kMaxCaptureAttempts    = 2;

	void ensureView() {
		if (m_view) {
			return;
		}

		m_profile = new QWebEngineProfile(this);
		m_profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
		m_profile->setPersistentPermissionsPolicy(QWebEngineProfile::PersistentPermissionsPolicy::StoreInMemory);
		m_profile->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);
		m_profile->setHttpUserAgent(QString::fromLatin1(s_previewBrowserUserAgent));
		m_profile->setHttpAcceptLanguage(QString::fromLatin1(s_previewAcceptLanguageHeader));

		m_interceptor = new PreviewSnapshotUrlInterceptor(m_profile);
		m_profile->setUrlRequestInterceptor(m_interceptor);

		m_view = new QWebEngineView();
		m_view->setAttribute(Qt::WA_ShowWithoutActivating, true);
		m_view->setFocusPolicy(Qt::NoFocus);
		m_view->setContextMenuPolicy(Qt::NoContextMenu);
		m_view->setWindowFlag(Qt::Tool, true);
		m_view->setWindowFlag(Qt::FramelessWindowHint, true);
		m_view->setWindowFlag(Qt::WindowDoesNotAcceptFocus, true);
		m_view->resize(kViewportWidth, kViewportHeight);
		m_view->move(-32000, -32000);

		m_page = new QWebEnginePage(m_profile, m_view);
		m_page->setAudioMuted(true);
		m_page->setBackgroundColor(Qt::white);
		m_view->setPage(m_page);

		m_view->settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, true);
		m_view->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
		m_view->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, false);

		QObject::connect(m_view, &QWebEngineView::loadFinished, this, [this](bool ok) {
			if (!m_busy) {
				return;
			}
			if (!ok) {
				finishCurrent(false, QImage());
				return;
			}

			m_captureAttempts = 0;
			m_page->runJavaScript(QStringLiteral("try {"
												 "window.scrollTo(0, 0);"
												 "document.documentElement.style.scrollBehavior = 'auto';"
												 "if (document.body) { document.body.style.scrollBehavior = 'auto'; }"
												 "} catch (e) {}"),
								  [this](const QVariant &) {
									  if (m_busy) {
										  m_settleTimer->start();
									  }
								  });
		});
		QObject::connect(m_view, &QWebEngineView::renderProcessTerminated, this,
						 [this](QWebEnginePage::RenderProcessTerminationStatus, int) {
							 recreateView();
							 finishCurrent(false, QImage());
						 });
		QObject::connect(m_page, &QWebEnginePage::contentsSizeChanged, this, [this](const QSizeF &) {
			if (m_busy && !m_page->isLoading()) {
				m_settleTimer->start();
			}
		});
	}

	void recreateView() {
		if (!m_view) {
			return;
		}

		m_view->hide();
		m_view->close();
		delete m_view;
		m_view = nullptr;
		m_page = nullptr;
		if (m_profile) {
			delete m_profile;
			m_profile     = nullptr;
			m_interceptor = nullptr;
		}
		ensureView();
	}

	void startNextIfIdle() {
		if (m_busy || m_queue.isEmpty()) {
			return;
		}

		ensureView();

		m_busy    = true;
		m_current = m_queue.takeFirst();
		m_queuedPreviewKeys.remove(m_current.previewKey);
		m_captureAttempts = 0;

		m_view->resize(kViewportWidth, kViewportHeight);
		m_view->move(-32000, -32000);
		m_view->show();
		m_view->raise();
		m_page->setVisible(true);
		m_page->setUrl(m_current.url);
		m_timeoutTimer->start();
	}

	void captureCurrentView() {
		if (!m_busy || !m_view) {
			return;
		}

		m_page->runJavaScript(QStringLiteral("try {"
											 "window.scrollTo(0, 0);"
											 "} catch (e) {}"),
							  [this](const QVariant &) {
								  if (!m_busy || !m_view) {
									  return;
								  }

								  const QPixmap pixmap = m_view->grab();
								  const QImage image   = pixmap.toImage();
								  if (!image.isNull() && image.width() > 1 && image.height() > 1) {
									  finishCurrent(true, image);
									  return;
								  }

								  ++m_captureAttempts;
								  if (m_captureAttempts < kMaxCaptureAttempts) {
									  m_settleTimer->start(kCaptureRetryDelayMsec);
									  return;
								  }

								  finishCurrent(false, QImage());
							  });
	}

	void finishCurrent(bool success, const QImage &image) {
		if (!m_busy) {
			return;
		}

		const QString previewKey = m_current.previewKey;
		m_busy                   = false;
		m_current                = Request();
		m_timeoutTimer->stop();
		m_settleTimer->stop();

		if (m_page) {
			m_page->setVisible(false);
			m_page->triggerAction(QWebEnginePage::Stop);
		}
		if (m_view) {
			m_view->hide();
		}

		if (m_resultCallback) {
			m_resultCallback(previewKey, image, success);
		}

		QTimer::singleShot(0, this, [this]() { startNextIfIdle(); });
	}

	ResultCallback m_resultCallback;
	QList< Request > m_queue;
	QSet< QString > m_queuedPreviewKeys;
	Request m_current;
	QTimer *m_timeoutTimer                       = nullptr;
	QTimer *m_settleTimer                        = nullptr;
	QWebEngineProfile *m_profile                 = nullptr;
	PreviewSnapshotUrlInterceptor *m_interceptor = nullptr;
	QWebEngineView *m_view                       = nullptr;
	QWebEnginePage *m_page                       = nullptr;
	bool m_busy                                  = false;
	int m_captureAttempts                        = 0;
};
#endif

QImage persistentChatThumbnailImage(const QImage &image) {
	if (image.isNull()) {
		return QImage();
	}

	if (image.width() <= PERSISTENT_CHAT_PREVIEW_SOURCE_WIDTH
		&& image.height() <= PERSISTENT_CHAT_PREVIEW_SOURCE_HEIGHT) {
		return image;
	}

	return image.scaled(PERSISTENT_CHAT_PREVIEW_SOURCE_WIDTH, PERSISTENT_CHAT_PREVIEW_SOURCE_HEIGHT,
						Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QImage decodePersistentChatThumbnailImage(const QByteArray &data) {
	if (data.isEmpty()) {
		return QImage();
	}

	QBuffer buffer;
	buffer.setData(data);
	if (!buffer.open(QIODevice::ReadOnly)) {
		return QImage();
	}

	QImageReader reader(&buffer);
	reader.setAutoTransform(true);
	const QSize sourceSize = reader.size();
	if (sourceSize.isValid()
		&& (sourceSize.width() > PERSISTENT_CHAT_PREVIEW_SOURCE_WIDTH
			|| sourceSize.height() > PERSISTENT_CHAT_PREVIEW_SOURCE_HEIGHT)) {
		reader.setScaledSize(sourceSize.scaled(PERSISTENT_CHAT_PREVIEW_SOURCE_WIDTH,
											   PERSISTENT_CHAT_PREVIEW_SOURCE_HEIGHT, Qt::KeepAspectRatio));
	}

	return reader.read();
}

QUrl persistentChatThumbnailResourceUrl(const QString &previewKey) {
	QByteArray resourceUrl("mumble-preview:");
	resourceUrl.append(QUrl::toPercentEncoding(previewKey));
	return QUrl::fromEncoded(resourceUrl);
}

QUrl persistentChatPreviewImageUrl(const QString &previewKey) {
	QByteArray encodedUrl("mumble-chat://preview-image/");
	encodedUrl.append(QUrl::toPercentEncoding(previewKey));
	return QUrl::fromEncoded(encodedUrl);
}

QString persistentChatThumbnailHtml(const QString &previewKey, const QImage &image, int maxWidth, int maxHeight,
									const QString &inlineStyle = QString(), const QString &anchorHref = QString()) {
	if (previewKey.isEmpty() || image.isNull() || maxWidth <= 0 || maxHeight <= 0) {
		return QString();
	}

	const QSize displaySize = image.size().scaled(maxWidth, maxHeight, Qt::KeepAspectRatio);
	if (!displaySize.isValid() || displaySize.isEmpty()) {
		return QString();
	}

	const QString enforcedStyle = QString::fromLatin1("border:none; outline:none; display:block; margin:0;"
													  " width:%1px; max-width:%1px; height:%2px; max-height:%2px;"
													  " object-fit:contain; background:transparent; %3")
									  .arg(displaySize.width())
									  .arg(displaySize.height())
									  .arg(inlineStyle);

	QString imageHtml =
		QString::fromLatin1("<img src=\"%1\" width=\"%2\" height=\"%3\" "
							"style=\"%4\" />")
			.arg(persistentChatThumbnailResourceUrl(previewKey).toString(QUrl::FullyEncoded).toHtmlEscaped())
			.arg(displaySize.width())
			.arg(displaySize.height())
			.arg(enforcedStyle.toHtmlEscaped());
	if (!anchorHref.isEmpty()) {
		return QString::fromLatin1(
				   "<a href=\"%1\" style=\"text-decoration:none; color:inherit; display:block; line-height:0;\">%2</a>")
			.arg(anchorHref.toHtmlEscaped(), imageHtml);
	}

	return imageHtml;
}

QString decodedPreviewText(const QString &text) {
	return QTextDocumentFragment::fromHtml(text).toPlainText().simplified();
}

QString trimmedPreviewText(QString text, int maxLength) {
	text = decodedPreviewText(text);
	if (text.size() <= maxLength) {
		return text;
	}

	return text.left(std::max(0, maxLength - 1)).trimmed() + QChar(0x2026);
}

QString extractHtmlTitle(const QString &html) {
	static const QRegularExpression s_titleRegex(QLatin1String("<title[^>]*>(.*?)</title>"),
												 QRegularExpression::CaseInsensitiveOption
													 | QRegularExpression::DotMatchesEverythingOption);
	const QRegularExpressionMatch match = s_titleRegex.match(html);
	return match.hasMatch() ? decodedPreviewText(match.captured(1)) : QString();
}

QHash< QString, QString > extractMetaTags(const QString &html) {
	QHash< QString, QString > tags;

	static const QRegularExpression s_metaTagRegex(QLatin1String("<meta\\s+([^>]+)>"),
												   QRegularExpression::CaseInsensitiveOption);
	static const QRegularExpression s_attrRegex(QLatin1String("([A-Za-z_:][-A-Za-z0-9_:.]*)\\s*=\\s*([\"'])(.*?)\\2"),
												QRegularExpression::CaseInsensitiveOption
													| QRegularExpression::DotMatchesEverythingOption);

	QRegularExpressionMatchIterator metaTags = s_metaTagRegex.globalMatch(html);
	while (metaTags.hasNext()) {
		const QString attrsString = metaTags.next().captured(1);
		QHash< QString, QString > attrs;
		QRegularExpressionMatchIterator attrsIt = s_attrRegex.globalMatch(attrsString);
		while (attrsIt.hasNext()) {
			const QRegularExpressionMatch attrMatch = attrsIt.next();
			attrs.insert(attrMatch.captured(1).toLower(), attrMatch.captured(3));
		}

		const QString key     = attrs.value(QLatin1String("property"), attrs.value(QLatin1String("name"))).toLower();
		const QString content = attrs.value(QLatin1String("content"));
		if (!key.isEmpty() && !content.isEmpty() && !tags.contains(key)) {
			tags.insert(key, decodedPreviewText(content));
		}
	}

	return tags;
}

QList< QUrl > extractPreviewableUrls(const QString &messageHtml) {
	QList< QUrl > urls;
	QSet< QString > seen;

	auto addUrlCandidate = [&](QString candidate) {
		candidate = trimTrailingUrlPunctuation(candidate.trimmed());
		if (candidate.isEmpty()) {
			return;
		}

		const QUrl url = QUrl::fromUserInput(candidate);
		if (!url.isValid()) {
			return;
		}
		const QString scheme = url.scheme().toLower();
		if (scheme != QLatin1String("http") && scheme != QLatin1String("https")) {
			return;
		}

		const QString normalized = url.toString(QUrl::FullyEncoded);
		if (seen.contains(normalized)) {
			return;
		}

		seen.insert(normalized);
		urls << url;
	};

	static const QRegularExpression s_hrefRegex(QLatin1String("<a\\s[^>]*href\\s*=\\s*[\"']([^\"']+)[\"']"),
												QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatchIterator hrefMatches = s_hrefRegex.globalMatch(messageHtml);
	while (hrefMatches.hasNext()) {
		addUrlCandidate(hrefMatches.next().captured(1));
	}

	const QString plainText = QTextDocumentFragment::fromHtml(messageHtml).toPlainText();
	static const QRegularExpression s_urlRegex(QLatin1String("(https?://[^\\s<>\"]+)"),
											   QRegularExpression::CaseInsensitiveOption);
	QRegularExpressionMatchIterator urlMatches = s_urlRegex.globalMatch(plainText);
	while (urlMatches.hasNext()) {
		addUrlCandidate(urlMatches.next().captured(1));
	}

	return urls;
}

bool persistentChatSourceTextIsSinglePreviewableUrl(const QString &sourceText) {
	static const QRegularExpression s_singleUrlPattern(QLatin1String("^\\s*https?://[^\\s<>\"']+\\s*$"),
													   QRegularExpression::CaseInsensitiveOption);
	return s_singleUrlPattern.match(normalizedPersistentChatText(sourceText)).hasMatch();
}

QString persistentChatPreviewUrlDisplayHtml(const QString &urlText) {
	QString displayHtml;
	displayHtml.reserve(urlText.size() * 2);

	int consecutiveTokenChars         = 0;
	const auto appendBreakOpportunity = [&displayHtml, &consecutiveTokenChars]() {
		displayHtml += QLatin1String("<wbr/>");
		consecutiveTokenChars = 0;
	};

	for (const QChar character : urlText) {
		displayHtml += QString(character).toHtmlEscaped();
		switch (character.unicode()) {
			case '/':
			case '?':
			case '&':
			case '=':
			case '#':
			case '%':
			case '-':
			case '_':
			case '.':
				appendBreakOpportunity();
				break;
			default:
				++consecutiveTokenChars;
				if (consecutiveTokenChars >= 14) {
					appendBreakOpportunity();
				}
				break;
		}
	}

	return displayHtml;
}

QString persistentChatPreviewSourceUrlHtml(const QString &sourceText) {
	const QString normalizedUrl = normalizedPersistentChatText(sourceText).trimmed();
	if (normalizedUrl.isEmpty()) {
		return QString();
	}

	QColor mutedUrlColor = qApp->palette().color(QPalette::Link);
	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		mutedUrlColor = mixColors(tokens->subtext0, tokens->accent, 0.22);
	}
	mutedUrlColor.setAlphaF(std::clamp(mutedUrlColor.alphaF() * 0.88f, 0.0f, 1.0f));

	return QString::fromLatin1("<!--mumble-preview-url-->"
							   "<div style='margin:0; padding:0 0 6px 0; line-height:1.3;'>"
							   "<a class='mumble-preview-url' href=\"%1\" style=\"font-size:0.82em; color:%3; "
							   "text-decoration:none;\">%2</a>"
							   "</div>")
		.arg(normalizedUrl.toHtmlEscaped(), persistentChatPreviewUrlDisplayHtml(normalizedUrl),
			 mutedUrlColor.name(QColor::HexArgb));
}

struct PersistentChatReplyReference {
	unsigned int messageID = 0;
	QString actor;
	QString snippet;
};

QString persistentChatMessageTextSnippet(const QString &messageHtml, int maxLength = 140) {
	QString snippet = Qt::mightBeRichText(messageHtml)
						  ? QTextDocumentFragment::fromHtml(messageHtml).toPlainText().simplified()
						  : normalizedPersistentChatText(messageHtml).simplified();
	if (snippet.size() > maxLength) {
		snippet = snippet.left(std::max(0, maxLength - 1)).trimmed() + QChar(0x2026);
	}
	return snippet;
}

QString buildPersistentChatReplyHtml(const MumbleProto::ChatMessage &replyTarget, const QString &bodyHtml) {
	QJsonObject metadata;
	metadata.insert(QStringLiteral("message_id"), static_cast< int >(replyTarget.message_id()));
	metadata.insert(QStringLiteral("actor"), persistentChatActorLabel(replyTarget));
	metadata.insert(QStringLiteral("snippet"),
					persistentChatMessageTextSnippet(persistentChatMessageSourceText(replyTarget)));

	const QString actor = persistentChatActorLabel(replyTarget).toHtmlEscaped();
	const QString snippet =
		persistentChatMessageTextSnippet(persistentChatMessageSourceText(replyTarget)).toHtmlEscaped();
	const QString metadataJson = QString::fromUtf8(QJsonDocument(metadata).toJson(QJsonDocument::Compact));
	return QString::fromLatin1("<!--mumble-reply:%1--><blockquote "
							   "data-mumble-reply-quote=\"1\"><strong>%2</strong><br/>%3</blockquote>%4")
		.arg(metadataJson, actor, snippet, bodyHtml);
}

std::optional< PersistentChatReplyReference > extractPersistentChatReplyReference(const QString &messageHtml,
																				  QString *bodyHtml) {
	static const QRegularExpression s_replyRegex(QLatin1String("^\\s*<!--mumble-reply:([^>]*)-->\\s*(?:<blockquote\\s+"
															   "data-mumble-reply-quote=\"1\"[^>]*>.*?</blockquote>)?"),
												 QRegularExpression::CaseInsensitiveOption
													 | QRegularExpression::DotMatchesEverythingOption);
	const QRegularExpressionMatch match = s_replyRegex.match(messageHtml);
	if (!match.hasMatch()) {
		if (bodyHtml) {
			*bodyHtml = messageHtml;
		}
		return std::nullopt;
	}

	PersistentChatReplyReference reference;
	QJsonParseError error;
	const QByteArray metadataJson = match.captured(1).toUtf8();
	const QJsonDocument metadata  = QJsonDocument::fromJson(metadataJson, &error);
	if (error.error == QJsonParseError::NoError && metadata.isObject()) {
		const QJsonObject object = metadata.object();
		reference.messageID      = static_cast< unsigned int >(object.value(QStringLiteral("message_id")).toInt());
		reference.actor          = object.value(QStringLiteral("actor")).toString().trimmed();
		reference.snippet        = object.value(QStringLiteral("snippet")).toString().trimmed();
	}

	if (bodyHtml) {
		*bodyHtml = messageHtml.mid(match.capturedLength(0)).trimmed();
	}

	if (reference.actor.isEmpty() && reference.snippet.isEmpty() && reference.messageID == 0) {
		return std::nullopt;
	}

	return reference;
}

const MumbleProto::ChatMessage *findPersistentChatMessageByID(const std::vector< MumbleProto::ChatMessage > &messages,
															  unsigned int messageID) {
	for (const MumbleProto::ChatMessage &message : messages) {
		if (message.message_id() == messageID) {
			return &message;
		}
	}

	return nullptr;
}

const MumbleProto::ChatMessage *findPersistentChatMessage(const std::vector< MumbleProto::ChatMessage > &messages,
														  unsigned int threadID, unsigned int messageID) {
	for (const MumbleProto::ChatMessage &message : messages) {
		if (message.thread_id() == threadID && message.message_id() == messageID) {
			return &message;
		}
	}

	return nullptr;
}

MumbleProto::ChatMessage *findPersistentChatMessage(std::vector< MumbleProto::ChatMessage > &messages,
													unsigned int threadID, unsigned int messageID) {
	for (MumbleProto::ChatMessage &message : messages) {
		if (message.thread_id() == threadID && message.message_id() == messageID) {
			return &message;
		}
	}

	return nullptr;
}

std::optional< PersistentChatReplyReference >
	resolvedPersistentChatReplyReference(const std::vector< MumbleProto::ChatMessage > &messages,
										 const MumbleProto::ChatMessage &message) {
	if (!message.has_reply_to_message_id()) {
		return std::nullopt;
	}

	if (const MumbleProto::ChatMessage *replyTarget =
			findPersistentChatMessageByID(messages, message.reply_to_message_id())) {
		if (!(replyTarget->has_deleted_at() && replyTarget->deleted_at() > 0)) {
			PersistentChatReplyReference reference;
			reference.messageID = replyTarget->message_id();
			reference.actor     = persistentChatActorLabel(*replyTarget);
			reference.snippet   = persistentChatMessageTextSnippet(persistentChatMessageSourceText(*replyTarget));
			return reference;
		}
	}

	if (message.has_reply_actor_name() || message.has_reply_snippet()) {
		PersistentChatReplyReference reference;
		reference.messageID = message.reply_to_message_id();
		reference.actor     = message.has_reply_actor_name() ? u8(message.reply_actor_name()) : QObject::tr("Reply");
		reference.snippet   = message.has_reply_snippet() ? u8(message.reply_snippet()) : QString();
		return reference;
	}

	PersistentChatReplyReference unavailableReference;
	unavailableReference.messageID = message.reply_to_message_id();
	unavailableReference.actor     = QObject::tr("Reply");
	unavailableReference.snippet   = QObject::tr("Original message unavailable");
	return unavailableReference;
}
} // namespace

MessageBoxEvent::MessageBoxEvent(QString m) : QEvent(static_cast< QEvent::Type >(MB_QEVENT)) {
	msg = m;
}

OpenURLEvent::OpenURLEvent(QUrl u) : QEvent(static_cast< QEvent::Type >(OU_QEVENT)) {
	url = u;
}

MainWindow::MainWindow(QWidget *p)
	: QMainWindow(p), m_localVolumeLabel(make_qt_unique< MenuLabel >(tr("Local Volume Adjustment:"), this)),
	  m_userLocalVolumeSlider(make_qt_unique< UserLocalVolumeSlider >(this)),
	  m_listenerVolumeController(make_qt_unique< ListenerVolumeController >(this)),
	  m_listenerVolumeSlider(make_qt_unique< ListenerVolumeSlider >(*m_listenerVolumeController, this)) {
	SvgIcon::addSvgPixmapsToIcon(qiIconMuteSelf, QLatin1String("skin:muted_self.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiIconMuteServer, QLatin1String("skin:muted_server.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiIconMuteSuppressed, QLatin1String("skin:muted_suppressed.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiIconMutePushToMute, QLatin1String("skin:muted_pushtomute.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiIconDeafSelf, QLatin1String("skin:deafened_self.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiIconDeafServer, QLatin1String("skin:deafened_server.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiTalkingOff, QLatin1String("skin:talking_off.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiTalkingOn, QLatin1String("skin:talking_on.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiTalkingShout, QLatin1String("skin:talking_alt.svg"));
	SvgIcon::addSvgPixmapsToIcon(qiTalkingWhisper, QLatin1String("skin:talking_whisper.svg"));
	SvgIcon::addSvgPixmapsToIcon(m_iconInformation, QLatin1String("skin:Information_icon.svg"));

#ifdef Q_OS_MAC
	if (QFile::exists(QLatin1String("skin:mumble.icns")))
		qiIcon.addFile(QLatin1String("skin:mumble.icns"));
	else
		SvgIcon::addSvgPixmapsToIcon(qiIcon, QLatin1String("skin:mumble.svg"));
#else
	{ SvgIcon::addSvgPixmapsToIcon(qiIcon, QLatin1String("skin:mumble.svg")); }

	// Set application icon except on MacOSX, where the window-icon
	// shown in the title-bar usually serves as a draggable version of the
	// current open document (i.e. you can copy the open document anywhere
	// simply by dragging this icon).
	qApp->setWindowIcon(qiIcon);

	// Set the icon on the MainWindow directly. This fixes the icon not
	// being set on the MainWindow in certain environments (Ex: GTK+).
	setWindowIcon(qiIcon);
#endif

#ifdef Q_OS_WIN
	uiNewHardware = 1;
#endif
	forceQuit     = false;
	restartOnQuit = false;

	Channel::add(Mumble::ROOT_CHANNEL_ID, tr("Root"));

	aclEdit   = nullptr;
	banEdit   = nullptr;
	userEdit  = nullptr;
	tokenEdit = nullptr;

	voiceRecorderDialog = nullptr;

	qwPTTButtonWidget = nullptr;

	qtReconnect = new QTimer(this);
	qtReconnect->setInterval(10000);
	qtReconnect->setSingleShot(true);
	qtReconnect->setObjectName(QLatin1String("Reconnect"));

	qmUser     = new QMenu(tr("&User"), this);
	qmChannel  = new QMenu(tr("&Channel"), this);
	qmListener = new QMenu(tr("&Listener"), this);

	qmDeveloper = new QMenu(tr("&Developer"), this);

	qaEmpty = new QAction(tr("No action available..."), this);
	qaEmpty->setEnabled(false);

	createActions();
	setupUi(this);
	qaServerSettings = new QAction(tr("Server Settings..."), this);
	qaServerSettings->setToolTip(tr("Change server settings for connected clients"));
	qaServerSettings->setWhatsThis(tr("This opens server settings that are applied live and saved on the server."));
	connect(qaServerSettings, &QAction::triggered, this, &MainWindow::on_qaServerSettings_triggered);
	qaCreateTextRoom = new QAction(tr("Create Room..."), this);
	qaCreateTextRoom->setToolTip(tr("Create a voice room or persistent text room on this server"));
	qaCreateTextRoom->setWhatsThis(tr("This creates a voice room or named text room and saves it on the server."));
	connect(qaCreateTextRoom, &QAction::triggered, this, &MainWindow::on_qaCreateTextRoom_triggered);
	qaUserRemoteSpeechCleanup = new QAction(tr("Remote Speech Cleanup"), this);
	qaUserRemoteSpeechCleanup->setCheckable(true);
	qaUserRemoteSpeechCleanup->setToolTip(tr("Clean up this user's incoming speech locally"));
	qaUserRemoteSpeechCleanup->setWhatsThis(
		tr("Enable or disable receive-side speech cleanup for this user on this client only."));
	connect(qaUserRemoteSpeechCleanup, &QAction::triggered, this, &MainWindow::triggerUserRemoteSpeechCleanup);
	qaChannelScreenShareStart        = new QAction(tr("Start Screen Share"), this);
	qaChannelScreenShareStop         = new QAction(tr("Stop Screen Share"), this);
	qaChannelScreenShareWatch        = new QAction(tr("Watch Screen Share"), this);
	qaChannelScreenShareStopWatching = new QAction(tr("Stop Watching Screen Share"), this);
	qaChannelScreenShareOpenWindow   = new QAction(tr("Open Screen Share Window"), this);
	connect(qaChannelScreenShareStart, &QAction::triggered, this, &MainWindow::startChannelScreenShare);
	connect(qaChannelScreenShareStop, &QAction::triggered, this, &MainWindow::stopChannelScreenShare);
	connect(qaChannelScreenShareWatch, &QAction::triggered, this, &MainWindow::watchChannelScreenShare);
	connect(qaChannelScreenShareStopWatching, &QAction::triggered, this, &MainWindow::stopWatchingChannelScreenShare);
	connect(qaChannelScreenShareOpenWindow, &QAction::triggered, this, &MainWindow::openChannelScreenShareWindow);
	setupGui();
	connect(qmUser, SIGNAL(aboutToShow()), this, SLOT(qmUser_aboutToShow()));
	connect(qmChannel, SIGNAL(aboutToShow()), this, SLOT(qmChannel_aboutToShow()));
	connect(qmListener, SIGNAL(aboutToShow()), this, SLOT(qmListener_aboutToShow()));
	connect(qteChat, SIGNAL(entered(QString)), this, SLOT(sendChatbarText(QString)));
	connect(qteChat, &ChatbarTextEdit::ctrlEnterPressed, [this](const QString &msg) { sendChatbarText(msg, true); });
	connect(qteChat, SIGNAL(pastedImage(QString)), this, SLOT(sendChatbarMessage(QString)));
#ifdef Q_OS_MACOS
	// Use default preferences icon in the macOS menu bar
	qaConfigDialog->setIconVisibleInMenu(false);
#endif

	QObject::connect(qaServerAddToFavorites, &QAction::triggered, this, &MainWindow::addServerAsFavorite);

	QObject::connect(this, &MainWindow::transmissionModeChanged, this, &MainWindow::updateTransmitModeComboBox);

	// Explicitly add actions to mainwindow so their shortcuts are available
	// if only the main window is visible (e.g. Global::get(). minimal mode)
	addActions(findChildren< QAction * >());

	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

	qmDeveloper->addAction(qaDeveloperConsole);
	qmDeveloper->addAction(qaPositionalAudioViewer);

	setOnTop(Global::get().s.aotbAlwaysOnTop == Settings::OnTopAlways
			 || (Global::get().s.bMinimalView && Global::get().s.aotbAlwaysOnTop == Settings::OnTopInMinimal)
			 || (!Global::get().s.bMinimalView && Global::get().s.aotbAlwaysOnTop == Settings::OnTopInNormal));

	m_screenShareManager = std::make_unique< ScreenShareManager >(this);
	QObject::connect(this, &MainWindow::serverSynchronized, Global::get().pluginManager,
					 &PluginManager::on_serverSynchronized);
	QObject::connect(this, &MainWindow::disconnectedFromServer, m_screenShareManager.get(),
					 &ScreenShareManager::resetState);
	QObject::connect(m_screenShareManager.get(), &ScreenShareManager::sessionUpdated, this,
					 &MainWindow::queueModernShellSnapshotSync);
	QObject::connect(m_screenShareManager.get(), &ScreenShareManager::sessionStopped, this,
					 &MainWindow::queueModernShellSnapshotSync);
	QObject::connect(&m_screenShareManager->helperClient(), &ScreenShareHelperClient::capabilitiesChanged, this,
					 &MainWindow::queueModernShellSnapshotSync);

	// Set up initial client side talking state without the need for the user to do anything.
	// This will, for example, make sure the correct status tray icon is used on connect.
	QObject::connect(this, &MainWindow::serverSynchronized, this, &MainWindow::userStateChanged);

	QObject::connect(this, &MainWindow::channelStateChanged, this, &MainWindow::on_channelStateChanged);

	QAccessible::installFactory(AccessibleSlider::semanticSliderFactory);
}

// Loading a state that was stored by a different version of Qt can lead to a crash.
// This function calculates the state version based on Qt's version, MainWindow.ui's
// hash (provided through CMake), and an explicit runtime shell revision.
// The latter is needed because modern shell structure now changes in MainWindow.cpp
// too, especially the toolbar/dock topology that restoreState() serializes.
constexpr int MainWindow::stateVersion(const bool modernShell) {
	constexpr int kRuntimeLegacyShellStateVersion = 0x20260407;
	constexpr int kRuntimeModernShellStateVersion = 0x20260418;
	return MUMBLE_MAINWINDOW_UI_HASH ^ QT_VERSION
		   ^ (modernShell ? kRuntimeModernShellStateVersion : kRuntimeLegacyShellStateVersion);
}

Settings::WindowLayout MainWindow::effectiveWindowLayout() const {
	const Settings::WindowLayout storedLayout = Global::get().s.wlWindowLayout;
	const Settings::WindowLayout legacyLayout =
		storedLayout == Settings::LayoutModern ? Settings::LayoutHybrid : storedLayout;
#if !defined(MUMBLE_HAS_MODERN_LAYOUT)
	return legacyLayout;
#else
	if (m_modernShellRuntimeDisabled) {
		if (legacyLayout == Settings::LayoutHybrid && Global::get().s.bAutoSwitchModernOnCompatibleServers
			&& m_modernLayoutCompatibleServer) {
			return Settings::LayoutHybrid;
		}
		return legacyLayout;
	}

	if (Global::get().s.modernLayoutPolicy == Settings::ModernLayoutForced) {
		return Settings::LayoutModern;
	}

	if (storedLayout == Settings::LayoutModern) {
		return Settings::LayoutModern;
	}

	if (legacyLayout == Settings::LayoutHybrid && Global::get().s.bAutoSwitchModernOnCompatibleServers
		&& m_modernLayoutCompatibleServer) {
		return Settings::LayoutModern;
	}

	return legacyLayout;
#endif
}

bool MainWindow::usesModernShell() const {
	return effectiveWindowLayout() == Settings::LayoutModern;
}

void MainWindow::refreshShellLayout() {
	appendModernShellConnectTrace(
		QStringLiteral("refreshShellLayout enter shellInitialized=%1 active=%2 effective=%3 minimal=%4")
			.arg(m_shellLayoutInitialized ? 1 : 0)
			.arg(static_cast< int >(m_activeShellLayout))
			.arg(static_cast< int >(effectiveWindowLayout()))
			.arg((modernShellMinimalSnapshotEnabled() && usesModernShell()) ? 1 : 0));
	if (m_shellLayoutInitialized) {
		const bool currentModernShell = m_activeShellLayout == Settings::LayoutModern;
		if (Global::get().s.bMinimalView) {
			if (currentModernShell) {
				Global::get().s.qbaModernMinimalViewGeometry = saveGeometry();
				Global::get().s.qbaModernMinimalViewState    = saveState(stateVersion(true));
			} else {
				Global::get().s.qbaMinimalViewGeometry = saveGeometry();
				Global::get().s.qbaMinimalViewState    = saveState(stateVersion(false));
			}
		} else {
			if (currentModernShell) {
				Global::get().s.qbaModernMainWindowGeometry = saveGeometry();
				Global::get().s.qbaModernMainWindowState    = saveState(stateVersion(true));
			} else {
				Global::get().s.qbaMainWindowGeometry = saveGeometry();
				Global::get().s.qbaMainWindowState    = saveState(stateVersion(false));
			}
		}
	}

	applyShellLayout();
	appendModernShellConnectTrace(QStringLiteral("refreshShellLayout post-apply active=%1 centralWidget=%2")
									  .arg(static_cast< int >(m_activeShellLayout))
									  .arg(centralWidget()
											   ? QString::fromLatin1(centralWidget()->metaObject()->className())
											   : QStringLiteral("null")));
	if (m_userPresenceRefreshTimer) {
		const bool hiddenLegacyUserModelSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();
		if (hiddenLegacyUserModelSafeMode) {
			m_userPresenceRefreshTimer->stop();
		} else if (!m_userPresenceRefreshTimer->isActive()) {
			m_userPresenceRefreshTimer->start();
		}
		appendModernShellConnectTrace(QStringLiteral("refreshShellLayout presence-timer active=%1")
										  .arg(m_userPresenceRefreshTimer->isActive() ? 1 : 0));
	}
	setupView(false);
	appendModernShellConnectTrace(QStringLiteral("refreshShellLayout post-setupView"));
	loadState(Global::get().s.bMinimalView);
	appendModernShellConnectTrace(QStringLiteral("refreshShellLayout post-loadState"));
	if (effectiveWindowLayout() == Settings::LayoutModern) {
		removeDockWidget(qdwLog);
		removeDockWidget(qdwChat);
		qdwLog->hide();
		qdwChat->hide();
		appendModernShellConnectTrace(QStringLiteral("refreshShellLayout modern-docks-hidden"));
	}
	setShowDockTitleBars((effectiveWindowLayout() == Settings::LayoutCustom) && !Global::get().s.bLockLayout);
	appendModernShellConnectTrace(QStringLiteral("refreshShellLayout exit"));
}

void MainWindow::applyShellLayout() {
	const Settings::WindowLayout targetLayout = effectiveWindowLayout();
	if (m_shellLayoutInitialized && m_activeShellLayout == targetLayout) {
		return;
	}

	if (targetLayout == Settings::LayoutModern) {
		activateModernShell();
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
		if (!m_modernShellHost || centralWidget() != m_modernShellHost) {
			return;
		}
#endif
	} else {
		activateLegacyShell();
	}

	m_activeShellLayout      = targetLayout;
	m_shellLayoutInitialized = true;
}

void MainWindow::createActions() {
	gsPushTalk = new GlobalShortcut(this, GlobalShortcutType::PushToTalk, tr("Push-to-Talk", "Global Shortcut"));
	gsPushTalk->setObjectName(QLatin1String("PushToTalk"));
	gsPushTalk->qsToolTip   = tr("Push and hold this button to send voice.", "Global Shortcut");
	gsPushTalk->qsWhatsThis = tr(
		"This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.",
		"Global Shortcut");


	gsResetAudio =
		new GlobalShortcut(this, GlobalShortcutType::ResetAudio, tr("Reset Audio Processor", "Global Shortcut"));
	gsResetAudio->setObjectName(QLatin1String("ResetAudio"));

	gsMuteSelf = new GlobalShortcut(this, GlobalShortcutType::MuteSelf, tr("Mute Self", "Global Shortcut"), 0);
	gsMuteSelf->setObjectName(QLatin1String("gsMuteSelf"));
	gsMuteSelf->qsToolTip = tr("Set self-mute status.", "Global Shortcut");
	gsMuteSelf->qsWhatsThis =
		tr("This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.",
		   "Global Shortcut");

	gsDeafSelf = new GlobalShortcut(this, GlobalShortcutType::DeafenSelf, tr("Deafen Self", "Global Shortcut"), 0);
	gsDeafSelf->setObjectName(QLatin1String("gsDeafSelf"));
	gsDeafSelf->qsToolTip = tr("Set self-deafen status.", "Global Shortcut");
	gsDeafSelf->qsWhatsThis =
		tr("This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.",
		   "Global Shortcut");

	gsUnlink = new GlobalShortcut(this, GlobalShortcutType::UnlinkPlugin, tr("Unlink Plugin", "Global Shortcut"));
	gsUnlink->setObjectName(QLatin1String("UnlinkPlugin"));

	gsPushMute = new GlobalShortcut(this, GlobalShortcutType::PushToMute, tr("Push-to-Mute", "Global Shortcut"));
	gsPushMute->setObjectName(QLatin1String("PushToMute"));

	gsJoinChannel = new GlobalShortcut(this, GlobalShortcutType::JoinChannel, tr("Join Channel", "Global Shortcut"));
	gsJoinChannel->setObjectName(QLatin1String("MetaChannel"));
	gsJoinChannel->qsToolTip = tr("Use in conjunction with Whisper to.", "Global Shortcut");

	gsListenChannel =
		new GlobalShortcut(this, GlobalShortcutType::ListenToChannel, tr("Listen to Channel", "Global Shortcut"),
						   QVariant::fromValue(ChannelTarget()));
	gsListenChannel->setObjectName(QLatin1String("gsListenChannel"));
	gsListenChannel->qsToolTip = tr("Toggles listening to the given channel.", "Global Shortcut");

#ifdef USE_OVERLAY
	gsToggleOverlay =
		new GlobalShortcut(this, GlobalShortcutType::ToggleOverlay, tr("Toggle Overlay", "Global Shortcut"));
	gsToggleOverlay->setObjectName(QLatin1String("ToggleOverlay"));
	gsToggleOverlay->qsToolTip   = tr("Toggle state of in-game overlay.", "Global Shortcut");
	gsToggleOverlay->qsWhatsThis = tr("This will switch the states of the in-game overlay.", "Global Shortcut");

	connect(gsToggleOverlay, SIGNAL(down(QVariant)), Global::get().o, SLOT(toggleShow()));
#endif

	gsMinimal =
		new GlobalShortcut(this, GlobalShortcutType::ToggleMinimalView, tr("Toggle Minimal", "Global Shortcut"));
	gsMinimal->setObjectName(QLatin1String("ToggleMinimal"));

	gsVolumeUp = new GlobalShortcut(this, GlobalShortcutType::VolumeUp, tr("Volume Up (+10%)", "Global Shortcut"));
	gsVolumeUp->setObjectName(QLatin1String("VolumeUp"));

	gsVolumeDown =
		new GlobalShortcut(this, GlobalShortcutType::VolumeDown, tr("Volume Down (-10%)", "Global Shortcut"));
	gsVolumeDown->setObjectName(QLatin1String("VolumeDown"));

	gsWhisper = new GlobalShortcut(this, GlobalShortcutType::Whisper_Shout, tr("Whisper/Shout"),
								   QVariant::fromValue(ShortcutTarget()));
	gsWhisper->setObjectName(QLatin1String("gsWhisper"));

	gsLinkChannel = new GlobalShortcut(this, GlobalShortcutType::LinkChannel, tr("Link Channel", "Global Shortcut"));
	gsLinkChannel->setObjectName(QLatin1String("MetaLink"));
	gsLinkChannel->qsToolTip = tr("Use in conjunction with Whisper to.", "Global Shortcut");

	gsCycleTransmitMode =
		new GlobalShortcut(this, GlobalShortcutType::CycleTransmitMode, tr("Cycle Transmit Mode", "Global Shortcut"));
	gsCycleTransmitMode->setObjectName(QLatin1String("gsCycleTransmitMode"));

	gsToggleMainWindowVisibility = new GlobalShortcut(this, GlobalShortcutType::ToggleMainWindowVisibility,
													  tr("Hide/show main window", "Global Shortcut"));
	gsToggleMainWindowVisibility->setObjectName(QLatin1String("gsToggleMainWindowVisibility"));

	gsTransmitModePushToTalk = new GlobalShortcut(this, GlobalShortcutType::UsePushToTalk,
												  tr("Set Transmit Mode to Push-To-Talk", "Global Shortcut"));
	gsTransmitModePushToTalk->setObjectName(QLatin1String("gsTransmitModePushToTalk"));

	gsTransmitModeContinuous = new GlobalShortcut(this, GlobalShortcutType::UseContinous,
												  tr("Set Transmit Mode to Continuous", "Global Shortcut"));
	gsTransmitModeContinuous->setObjectName(QLatin1String("gsTransmitModeContinuous"));

	gsTransmitModeVAD =
		new GlobalShortcut(this, GlobalShortcutType::UseVAD, tr("Set Transmit Mode to VAD", "Global Shortcut"));
	gsTransmitModeVAD->setObjectName(QLatin1String("gsTransmitModeVAD"));

	gsSendTextMessage = new GlobalShortcut(this, GlobalShortcutType::SendTextMessage,
										   tr("Send Text Message", "Global Shortcut"), QVariant(QString()));
	gsSendTextMessage->setObjectName(QLatin1String("gsSendTextMessage"));

	gsSendClipboardTextMessage = new GlobalShortcut(this, GlobalShortcutType::SendTextMessageClipboard,
													tr("Send Clipboard Text Message", "Global Shortcut"));
	gsSendClipboardTextMessage->setObjectName(QLatin1String("gsSendClipboardTextMessage"));
	gsSendClipboardTextMessage->qsWhatsThis =
		tr("This will send your Clipboard content to the channel you are currently in.", "Global Shortcut");

	gsToggleTalkingUI =
		new GlobalShortcut(this, GlobalShortcutType::ToggleTalkingUI, tr("Toggle TalkingUI", "Global shortcut"));
	gsToggleTalkingUI->setObjectName(QLatin1String("gsToggleTalkingUI"));
	gsToggleTalkingUI->qsWhatsThis = tr("Toggles the visibility of the TalkingUI.", "Global Shortcut");

	gsToggleSearch =
		new GlobalShortcut(this, GlobalShortcutType::ToggleSearch, tr("Toggle search dialog", "Global Shortcut"));
	gsToggleSearch->setObjectName(QLatin1String("gsToggleSearch"));
	gsToggleSearch->qsWhatsThis =
		tr("This will open or close the search dialog depending on whether it is currently opened already");

	gsServerConnect =
		new GlobalShortcut(this, GlobalShortcutType::ServerConnect, tr("Connect to a server", "Global Shortcut"));
	gsServerConnect->setObjectName(QLatin1String("gsServerConnect"));
	gsServerConnect->qsWhatsThis = tr("This will open the server connection dialog", "Global Shortcut");

	gsServerDisconnect =
		new GlobalShortcut(this, GlobalShortcutType::ServerDisconnect, tr("Disconnect from server", "Global Shortcut"));
	gsServerDisconnect->setObjectName(QLatin1String("gsServerDisconnect"));
	gsServerDisconnect->qsWhatsThis = tr("This will disconnect you from the server", "Global Shortcut");

	gsServerInformation = new GlobalShortcut(this, GlobalShortcutType::ServerInformation,
											 tr("Open server information", "Global Shortcut"));
	gsServerInformation->setObjectName(QLatin1String("gsServerInformation"));
	gsServerInformation->qsWhatsThis = tr("This will show information about the server connection", "Global Shortcut");

	gsServerTokens =
		new GlobalShortcut(this, GlobalShortcutType::ServerTokens, tr("Open server tokens", "Global Shortcut"));
	gsServerTokens->setObjectName(QLatin1String("gsServerTokens"));
	gsServerTokens->qsWhatsThis = tr("This will open the server tokens dialog", "Global Shortcut");

	gsServerUserList =
		new GlobalShortcut(this, GlobalShortcutType::ServerUserList, tr("Open server user list", "Global Shortcut"));
	gsServerUserList->setObjectName(QLatin1String("gsServerUserList"));
	gsServerUserList->qsWhatsThis = tr("This will open the server user list dialog", "Global Shortcut");

	gsServerBanList =
		new GlobalShortcut(this, GlobalShortcutType::ServerBanList, tr("Open server ban list", "Global Shortcut"));
	gsServerBanList->setObjectName(QLatin1String("gsServerBanList"));
	gsServerBanList->qsWhatsThis = tr("This will open the server ban list dialog", "Global Shortcut");

	gsSelfPrioritySpeaker = new GlobalShortcut(this, GlobalShortcutType::SelfPrioritySpeaker,
											   tr("Toggle priority speaker", "Global Shortcut"));
	gsSelfPrioritySpeaker->setObjectName(QLatin1String("gsSelfPrioritySpeaker"));
	gsSelfPrioritySpeaker->qsWhatsThis = tr("This will enable/disable the priority speaker", "Global Shortcut");

	gsRecording =
		new GlobalShortcut(this, GlobalShortcutType::Recording, tr("Open recording dialog", "Global Shortcut"));
	gsRecording->setObjectName(QLatin1String("gsRecording"));
	gsRecording->qsWhatsThis = tr("This will open the recording dialog");

	gsSelfComment = new GlobalShortcut(this, GlobalShortcutType::SelfComment, tr("Change comment", "Global Shortcut"));
	gsSelfComment->setObjectName(QLatin1String("gsSelfComment"));
	gsSelfComment->qsWhatsThis = tr("This will open the change comment dialog");

	gsServerTexture =
		new GlobalShortcut(this, GlobalShortcutType::ServerTexture, tr("Change avatar", "Global Shortcut"));
	gsServerTexture->setObjectName(QLatin1String("gsServerTexture"));
	gsServerTexture->qsWhatsThis = tr("This will open your file explorer to change your avatar image on this server");

	gsServerTextureRemove =
		new GlobalShortcut(this, GlobalShortcutType::ServerTextureRemove, tr("Remove avatar", "Global Shortcut"));
	gsServerTextureRemove->setObjectName(QLatin1String("gsServerTextureRemove"));
	gsServerTextureRemove->qsWhatsThis = tr("This will reset your avatar on the server");

	gsSelfRegister =
		new GlobalShortcut(this, GlobalShortcutType::SelfRegister, tr("Register on the server", "Global Shortcut"));
	gsSelfRegister->setObjectName(QLatin1String("gsSelfRegister"));
	gsSelfRegister->qsWhatsThis = tr("This will register you on the server");

	gsAudioStats = new GlobalShortcut(this, GlobalShortcutType::AudioStats, tr("Audio statistics", "Global Shortcut"));
	gsAudioStats->setObjectName(QLatin1String("gsAudioStats"));
	gsAudioStats->qsWhatsThis = tr("This will open the audio statistics dialog");

	gsConfigDialog = new GlobalShortcut(this, GlobalShortcutType::ConfigDialog, tr("Open settings", "Global Shortcut"));
	gsConfigDialog->setObjectName(QLatin1String("gsConfigDialog"));
	gsConfigDialog->qsWhatsThis = tr("This will open the settings dialog");

	gsAudioWizard =
		new GlobalShortcut(this, GlobalShortcutType::AudioWizard, tr("Start audio wizard", "Global Shortcut"));
	gsAudioWizard->setObjectName(QLatin1String("gsAudioWizard"));
	gsAudioWizard->qsWhatsThis = tr("This will open the audio wizard dialog");

	gsConfigCert =
		new GlobalShortcut(this, GlobalShortcutType::ConfigCert, tr("Start certificate wizard", "Global Shortcut"));
	gsConfigCert->setObjectName(QLatin1String("gsConfigCert"));
	gsConfigCert->qsWhatsThis = tr("This will open the certificate wizard dialog");

	gsAudioTTS = new GlobalShortcut(this, GlobalShortcutType::AudioTTS, tr("Toggle text to speech", "Global Shortcut"));
	gsAudioTTS->setObjectName(QLatin1String("gsAudioTTS"));
	gsAudioTTS->qsWhatsThis = tr("This will enable/disable the text to speech");

	gsHelpAbout = new GlobalShortcut(this, GlobalShortcutType::HelpAbout, tr("Open about dialog", "Global Shortcut"));
	gsHelpAbout->setObjectName(QLatin1String("gsHelpAbout"));
	gsHelpAbout->qsWhatsThis = tr("This will open the about dialog");

	gsHelpAboutQt =
		new GlobalShortcut(this, GlobalShortcutType::HelpAboutQt, tr("Open about Qt dialog", "Global Shortcut"));
	gsHelpAboutQt->setObjectName(QLatin1String("gsHelpAboutQt"));
	gsHelpAboutQt->qsWhatsThis = tr("This will open the about Qt dialog");

	gsHelpVersionCheck =
		new GlobalShortcut(this, GlobalShortcutType::HelpVersionCheck, tr("Check for update", "Global Shortcut"));
	gsHelpVersionCheck->setObjectName(QLatin1String("gsHelpVersionCheck"));
	gsHelpVersionCheck->qsWhatsThis = tr("This will check if mumble is up to date");

	gsTogglePositionalAudio = new GlobalShortcut(this, GlobalShortcutType::TogglePositionalAudio,
												 tr("Toggle positional audio", "Global Shortcut"));
	gsTogglePositionalAudio->setObjectName("gsTogglePositionalAudio");
	gsTogglePositionalAudio->qsWhatsThis = tr("This will toggle positional audio on/off");

	gsMoveBack = new GlobalShortcut(this, GlobalShortcutType::MoveBack, tr("Move back", "Global shortcut"));
	gsMoveBack->setObjectName("gsMoveBack");
	gsMoveBack->qsWhatsThis = tr("This will move you back into your previous channel");

	gsCycleListenerAttenuationMode = new GlobalShortcut(this, GlobalShortcutType::CycleListenerAttenuationMode,
														tr("Cycle listener attenuation mode", "Global shortcut"));
	gsCycleListenerAttenuationMode->setObjectName("gsCycleListenerAttenuationMode");
	gsCycleListenerAttenuationMode->qsWhatsThis =
		tr("This will cycle through the different attenuation modes for channel listeners");

	gsListenerAttenuationUp = new GlobalShortcut(this, GlobalShortcutType::ListenerAttenuationUp,
												 tr("Listener attenuation up (+10%)", "Global shortcut"));
	gsListenerAttenuationUp->setObjectName("gsListenerAttenuationUp");
	gsListenerAttenuationUp->qsWhatsThis =
		tr("This increases the attenuation of channel listeners by 10 percents points");

	gsListenerAttenuationDown = new GlobalShortcut(this, GlobalShortcutType::ListenerAttenuationDown,
												   tr("Listener attenuation down (-10%)", "Global shortcut"));
	gsListenerAttenuationDown->setObjectName("gsListenerAttenuationDown");
	gsListenerAttenuationDown->qsWhatsThis =
		tr("This decreases the attenuation of channel listeners by 10 percents points");

	gsAdaptivePush = new GlobalShortcut(this, GlobalShortcutType::AdaptivePush, tr("Adaptive Push", "Global Shortcut"));
	gsAdaptivePush->setObjectName("gsAdaptivePush");
	gsAdaptivePush->qsToolTip = tr("When using the push-to-talk transmission mode, this will act as the push-to-talk "
								   "action. Otherwise, it will act as a push-to-mute action.",
								   "Global Shortcut");
}

void MainWindow::setupGui() {
	updateWindowTitle();
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	setupServerNavigator();
	setCentralWidget(m_serverNavigatorContainer);
#else
	setCentralWidget(qtvUsers);
#endif
	setAcceptDrops(true);

#ifdef Q_OS_MAC
	QMenu *qmWindow = new QMenu(tr("&Window"), this);
	menubar->insertMenu(qmHelp->menuAction(), qmWindow);
#	if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
	qmWindow->addAction(tr("Minimize"), QKeySequence(tr("Ctrl+M")), this, &MainWindow::showMinimized);
#	else
	qmWindow->addAction(tr("Minimize"), this, SLOT(showMinimized()), QKeySequence(tr("Ctrl+M")));
#	endif

	qtvUsers->setAttribute(Qt::WA_MacShowFocusRect, false);
	qteChat->setAttribute(Qt::WA_MacShowFocusRect, false);
	qteChat->setFrameShape(QFrame::NoFrame);
	qteLog->setFrameStyle(QFrame::NoFrame);
#endif

	qteLog->setFrameShape(QFrame::NoFrame);
	qteLog->setFrameStyle(QFrame::NoFrame);
	qteLog->setLineWidth(0);
	qteLog->setMidLineWidth(0);
	qteLog->setAttribute(Qt::WA_StyledBackground, true);
	qteLog->resetViewportChrome();
	if (QWidget *logViewport = qteLog->viewport()) {
		logViewport->setAttribute(Qt::WA_StyledBackground, true);
	}
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	qteLog->setParent(nullptr);
	m_logSurface = new QWidget(qdwLog);
	m_logSurface->setObjectName(QLatin1String("qwLogSurface"));
	m_logSurface->setAttribute(Qt::WA_StyledBackground, true);
	QVBoxLayout *logLayout = new QVBoxLayout(m_logSurface);
	logLayout->setContentsMargins(8, 8, 0, 4);
	logLayout->setSpacing(0);
	logLayout->addWidget(qteLog);
	qdwLog->setWidget(m_logSurface);
	if (QLayout *dockLayout = qdwLog->layout()) {
		dockLayout->setContentsMargins(0, 0, 0, 0);
		dockLayout->setSpacing(0);
	}
#endif
	LogDocument *ld = new LogDocument(this);
	qteLog->setDocument(ld);
	qteLog->document()->setDocumentMargin(0);
	if (QTextFrame *rootFrame = qteLog->document()->rootFrame()) {
		QTextFrameFormat rootFrameFormat = rootFrame->frameFormat();
		rootFrameFormat.setBorder(0);
		rootFrameFormat.setMargin(0);
		rootFrameFormat.setPadding(0);
		rootFrame->setFrameFormat(rootFrameFormat);
	}
	connect(qteLog, &LogTextBrowser::imageActivated, this,
			[this](const QTextCursor &cursor) { openImageDialog(qteLog, cursor); });

	qteLog->document()->setMaximumBlockCount(Global::get().s.iMaxLogBlocks);
	connect(qteLog->document(), &QTextDocument::contentsChanged, this, [this]() {
		if (modernShellMinimalSnapshotEnabled() && usesModernShell()) {
			appendModernShellConnectTrace(QStringLiteral("UI qteLog contentsChanged minimal-skip"));
			return;
		}
		if (const PersistentChatTarget target = currentPersistentChatTarget(); target.serverLog) {
			renderServerLogView(true);
		}
	});

	pmModel = new UserModel(qtvUsers);
	qtvUsers->setModel(pmModel);
	qtvUsers->setRowHidden(0, QModelIndex(), true);
	qtvUsers->ensurePolished();
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	updateServerNavigatorChrome();
#endif

	QObject::connect(this, &MainWindow::userAddedChannelListener, pmModel, &UserModel::addChannelListener);
	QObject::connect(
		this, &MainWindow::userRemovedChannelListener, pmModel,
		static_cast< void (UserModel::*)(const ClientUser *, const Channel *) >(&UserModel::removeChannelListener));
	QObject::connect(Global::get().channelListenerManager.get(), &ChannelListenerManager::localVolumeAdjustmentsChanged,
					 pmModel, &UserModel::on_channelListenerLocalVolumeAdjustmentChanged);
	QObject::connect(pmModel, &UserModel::userMoved, this, &MainWindow::handleUserMoved);

	// connect slots to PluginManager
	QObject::connect(pmModel, &UserModel::userAdded, Global::get().pluginManager, &PluginManager::on_userAdded);
	QObject::connect(pmModel, &UserModel::userRemoved, Global::get().pluginManager, &PluginManager::on_userRemoved);
	QObject::connect(pmModel, &UserModel::channelAdded, Global::get().pluginManager, &PluginManager::on_channelAdded);
	QObject::connect(pmModel, &UserModel::channelRemoved, Global::get().pluginManager,
					 &PluginManager::on_channelRemoved);
	QObject::connect(pmModel, &UserModel::channelRenamed, Global::get().pluginManager,
					 &PluginManager::on_channelRenamed);
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	QObject::connect(pmModel, &QAbstractItemModel::dataChanged, this, [this]() { queueModernShellSnapshotSync(); });
	QObject::connect(pmModel, &QAbstractItemModel::rowsInserted, this, [this]() { queueModernShellSnapshotSync(); });
	QObject::connect(pmModel, &QAbstractItemModel::rowsRemoved, this, [this]() { queueModernShellSnapshotSync(); });
	QObject::connect(pmModel, &QAbstractItemModel::rowsMoved, this, [this]() { queueModernShellSnapshotSync(); });
	QObject::connect(pmModel, &QAbstractItemModel::modelReset, this, [this]() { queueModernShellSnapshotSync(); });
	QObject::connect(pmModel, &QAbstractItemModel::layoutChanged, this, [this]() { queueModernShellSnapshotSync(); });
#endif

	qaAudioMute->setChecked(Global::get().s.bMute);
	qaAudioDeaf->setChecked(Global::get().s.bDeaf);

	updateAudioToolTips();

#ifdef USE_NO_TTS
	qaAudioTTS->setChecked(false);
	qaAudioTTS->setDisabled(true);
#else
	qaAudioTTS->setChecked(Global::get().s.bTTS);
#endif
	qaFilterToggle->setChecked(Global::get().s.bFilterActive);
	on_qaFilterToggle_triggered();

	qaHelpWhatsThis->setShortcuts(QKeySequence::WhatsThis);

	qaConfigMinimal->setChecked(Global::get().s.bMinimalView);
	qaConfigHideFrame->setChecked(Global::get().s.bHideFrame);

	connect(gsResetAudio, SIGNAL(down(QVariant)), qaAudioReset, SLOT(trigger()));
	connect(gsUnlink, SIGNAL(down(QVariant)), qaAudioUnlink, SLOT(trigger()));
	connect(gsMinimal, SIGNAL(down(QVariant)), qaConfigMinimal, SLOT(trigger()));

#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	dtbLogDockTitle = new DockTitleBar();
	qdwLog->setTitleBarWidget(dtbLogDockTitle);
	dtbLogDockTitle->setMinimumHeight(0);
	dtbLogDockTitle->setMaximumHeight(0);

	for (QWidget *w : qdwLog->findChildren< QWidget * >()) {
		w->installEventFilter(dtbLogDockTitle);
		w->setMouseTracking(true);
	}

	dtbChatDockTitle = new DockTitleBar();
	qdwChat->setTitleBarWidget(dtbChatDockTitle);
	dtbChatDockTitle->setMinimumHeight(0);
	dtbChatDockTitle->setMaximumHeight(0);
	qdwChat->installEventFilter(dtbChatDockTitle);
	setupPersistentChatDock();
#endif
	refreshTextDocumentStylesheets();
	qteChat->setDefaultText(tr("<center>Not connected</center>"), true);
	qteChat->setEnabled(false);

	QWidget *dummyTitlebar = new QWidget(qdwMinimalViewNote);
	qdwMinimalViewNote->setTitleBarWidget(dummyTitlebar);

	m_modernShellSyncTimer = new QTimer(this);
	m_modernShellSyncTimer->setSingleShot(true);
	m_modernShellSyncTimer->setTimerType(Qt::PreciseTimer);
	connect(m_modernShellSyncTimer, &QTimer::timeout, this, &MainWindow::syncModernShellSnapshot);

	m_nativeWindowMoveResizeRecoveryTimer = new QTimer(this);
	m_nativeWindowMoveResizeRecoveryTimer->setSingleShot(true);
	m_nativeWindowMoveResizeRecoveryTimer->setTimerType(Qt::CoarseTimer);
	connect(m_nativeWindowMoveResizeRecoveryTimer, &QTimer::timeout, this, [this]() {
		if (!m_nativeWindowMoveResizeActive) {
			return;
		}

		appendModernShellConnectTrace(QStringLiteral("nativeWindowMoveResize watchdog-end"));
		endNativeWindowMoveOrResize();
	});

	applyShellLayout();
	setShowDockTitleBars((effectiveWindowLayout() == Settings::LayoutCustom) && !Global::get().s.bLockLayout);

#ifdef Q_OS_MAC
	// Workaround for QTBUG-3116 -- using a unified toolbar on Mac OS X
	// and using restoreGeometry before the window has updated its frameStrut
	// causes the MainWindow to jump around on screen on launch.  Workaround
	// is to call show() to update the frameStrut and set the windowOpacity to
	// 0 to hide any graphical glitches that occur when we add stuff to the
	// window.
	setWindowOpacity(0.0f);
	show();
#endif

	connect(qtvUsers->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
			SLOT(qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &)));

	// QtCreator and uic.exe do not allow adding arbitrary widgets
	// such as a MUComboBox to a QToolbar, even though they are supported.
	qcbTransmitMode = new MUComboBox(qtIconToolbar);
	qcbTransmitMode->setObjectName(QLatin1String("qcbTransmitMode"));
	qcbTransmitMode->addItem(tr("Continuous"));
	qcbTransmitMode->addItem(tr("Voice Activity"));
	qcbTransmitMode->addItem(tr("Push-to-Talk"));

	qaTransmitModeSeparator = qtIconToolbar->insertSeparator(qaConfigDialog);
	qaTransmitMode          = qtIconToolbar->insertWidget(qaTransmitModeSeparator, qcbTransmitMode);

	connect(qcbTransmitMode, SIGNAL(activated(int)), this, SLOT(qcbTransmitMode_activated(int)));

	updateTransmitModeComboBox(Global::get().s.atTransmit);

#ifdef Q_OS_WIN
	setupView(false);
#endif

	loadState(Global::get().s.bMinimalView);

	setupView(false);
	queueModernShellSnapshotSync();

#ifdef Q_OS_MAC
	setWindowOpacity(1.0f);
#endif
}

void MainWindow::setupServerNavigator() {
	m_serverNavigatorContainer = new QWidget(this);
	m_serverNavigatorContainer->setObjectName(QLatin1String("qwServerNavigator"));
	m_serverNavigatorContainer->setAttribute(Qt::WA_StyledBackground, true);
	m_serverNavigatorContainer->setFixedWidth(182);
	m_serverNavigatorContainer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	QVBoxLayout *layout = new QVBoxLayout(m_serverNavigatorContainer);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	m_serverNavigatorContentFrame = new QFrame(m_serverNavigatorContainer);
	m_serverNavigatorContentFrame->setObjectName(QLatin1String("qfServerNavigatorContent"));
	m_serverNavigatorContentFrame->setAttribute(Qt::WA_StyledBackground, true);
	QVBoxLayout *contentLayout = new QVBoxLayout(m_serverNavigatorContentFrame);
	contentLayout->setContentsMargins(5, 4, 5, 6);
	contentLayout->setSpacing(2);

	m_serverNavigatorHeaderFrame = new QFrame(m_serverNavigatorContentFrame);
	m_serverNavigatorHeaderFrame->setObjectName(QLatin1String("qfServerNavigatorHeader"));

	QVBoxLayout *headerLayout = new QVBoxLayout(m_serverNavigatorHeaderFrame);
	headerLayout->setContentsMargins(0, 0, 0, 0);
	headerLayout->setSpacing(0);

	m_serverNavigatorEyebrow = new QLabel(tr("Server"), m_serverNavigatorHeaderFrame);
	m_serverNavigatorEyebrow->setObjectName(QLatin1String("qlServerNavigatorEyebrow"));
	QFont eyebrowFont = m_serverNavigatorEyebrow->font();
	eyebrowFont.setCapitalization(QFont::AllUppercase);
	eyebrowFont.setBold(true);
	eyebrowFont.setPointSizeF(std::max(eyebrowFont.pointSizeF() - 1.0, 8.0));
	m_serverNavigatorEyebrow->setFont(eyebrowFont);

	m_serverNavigatorTitle = new QLabel(tr("Not connected"), m_serverNavigatorHeaderFrame);
	m_serverNavigatorTitle->setObjectName(QLatin1String("qlServerNavigatorTitle"));
	QFont titleFont = m_serverNavigatorTitle->font();
	titleFont.setBold(true);
	titleFont.setPointSizeF(titleFont.pointSizeF());
	m_serverNavigatorTitle->setFont(titleFont);
	m_serverNavigatorTitle->setTextFormat(Qt::PlainText);
	m_serverNavigatorTitle->setWordWrap(true);

	m_serverNavigatorSubtitle = new QLabel(tr("Browse channels and people here."), m_serverNavigatorHeaderFrame);
	m_serverNavigatorSubtitle->setObjectName(QLatin1String("qlServerNavigatorSubtitle"));
	m_serverNavigatorSubtitle->setWordWrap(true);
	m_serverNavigatorSubtitle->setTextFormat(Qt::PlainText);
	m_serverNavigatorSubtitle->setTextInteractionFlags(Qt::NoTextInteraction);

	headerLayout->addWidget(m_serverNavigatorEyebrow);
	headerLayout->addWidget(m_serverNavigatorTitle);
	headerLayout->addWidget(m_serverNavigatorSubtitle);
	m_serverNavigatorHeaderFrame->hide();

	m_serverNavigatorTextChannelsMotdFrame = new QFrame(m_serverNavigatorContentFrame);
	m_serverNavigatorTextChannelsMotdFrame->setObjectName(QLatin1String("qfServerNavigatorTextChannelsMotd"));
	m_serverNavigatorTextChannelsMotdFrame->setAttribute(Qt::WA_StyledBackground, true);
	m_serverNavigatorTextChannelsMotdFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	QVBoxLayout *textChannelsMotdLayout = new QVBoxLayout(m_serverNavigatorTextChannelsMotdFrame);
	textChannelsMotdLayout->setContentsMargins(5, 5, 5, 5);
	textChannelsMotdLayout->setSpacing(2);

	QHBoxLayout *textChannelsMotdHeaderLayout = new QHBoxLayout();
	textChannelsMotdHeaderLayout->setContentsMargins(0, 0, 0, 0);
	textChannelsMotdHeaderLayout->setSpacing(4);

	m_serverNavigatorTextChannelsMotdTitle = new QLabel(tr("MOTD"), m_serverNavigatorTextChannelsMotdFrame);
	m_serverNavigatorTextChannelsMotdTitle->setObjectName(QLatin1String("qlServerNavigatorTextChannelsMotdTitle"));
	m_serverNavigatorTextChannelsMotdTitle->setTextFormat(Qt::PlainText);

	m_serverNavigatorTextChannelsMotdToggleButton = new QToolButton(m_serverNavigatorTextChannelsMotdFrame);
	m_serverNavigatorTextChannelsMotdToggleButton->setObjectName(
		QLatin1String("qtbServerNavigatorTextChannelsMotdToggle"));
	m_serverNavigatorTextChannelsMotdToggleButton->setAutoRaise(true);
	m_serverNavigatorTextChannelsMotdToggleButton->hide();

	textChannelsMotdHeaderLayout->addWidget(m_serverNavigatorTextChannelsMotdTitle);
	textChannelsMotdHeaderLayout->addStretch(1);
	textChannelsMotdHeaderLayout->addWidget(m_serverNavigatorTextChannelsMotdToggleButton);
	textChannelsMotdLayout->addLayout(textChannelsMotdHeaderLayout);

	m_serverNavigatorTextChannelsMotdBody = new QLabel(m_serverNavigatorTextChannelsMotdFrame);
	m_serverNavigatorTextChannelsMotdBody->setObjectName(QLatin1String("qlServerNavigatorTextChannelsMotdBody"));
	m_serverNavigatorTextChannelsMotdBody->setAttribute(Qt::WA_StyledBackground, true);
	m_serverNavigatorTextChannelsMotdBody->setTextFormat(Qt::RichText);
	m_serverNavigatorTextChannelsMotdBody->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_serverNavigatorTextChannelsMotdBody->setWordWrap(true);
	m_serverNavigatorTextChannelsMotdBody->setTextInteractionFlags(Qt::TextBrowserInteraction);
	m_serverNavigatorTextChannelsMotdBody->setOpenExternalLinks(false);
	m_serverNavigatorTextChannelsMotdBody->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	textChannelsMotdLayout->addWidget(m_serverNavigatorTextChannelsMotdBody);
	m_serverNavigatorTextChannelsMotdFrame->hide();
	contentLayout->addWidget(m_serverNavigatorTextChannelsMotdFrame);
	contentLayout->addWidget(m_serverNavigatorHeaderFrame);

	m_serverNavigatorVoiceSectionEyebrow = new QLabel(tr("Rooms"), m_serverNavigatorContentFrame);
	m_serverNavigatorVoiceSectionEyebrow->setObjectName(QLatin1String("qlServerNavigatorVoiceSectionEyebrow"));
	QFont voiceEyebrowFont = m_serverNavigatorVoiceSectionEyebrow->font();
	voiceEyebrowFont.setCapitalization(QFont::AllUppercase);
	voiceEyebrowFont.setBold(true);
	voiceEyebrowFont.setPointSizeF(std::max(voiceEyebrowFont.pointSizeF() - 1.0, 8.0));
	m_serverNavigatorVoiceSectionEyebrow->setFont(voiceEyebrowFont);
	contentLayout->addWidget(m_serverNavigatorVoiceSectionEyebrow);

	m_serverNavigatorVoiceSectionSubtitle = new QLabel(tr("Rooms and people"), m_serverNavigatorContentFrame);
	m_serverNavigatorVoiceSectionSubtitle->setObjectName(QLatin1String("qlServerNavigatorVoiceSectionSubtitle"));
	m_serverNavigatorVoiceSectionSubtitle->setWordWrap(true);
	m_serverNavigatorVoiceSectionSubtitle->setTextFormat(Qt::PlainText);
	m_serverNavigatorVoiceSectionSubtitle->setTextInteractionFlags(Qt::NoTextInteraction);
	m_serverNavigatorVoiceSectionSubtitle->hide();
	contentLayout->addWidget(m_serverNavigatorVoiceSectionSubtitle);

	qtvUsers->setParent(m_serverNavigatorContentFrame);
	qtvUsers->setObjectName(QLatin1String("qtvUsers"));
	qtvUsers->setFrameShape(QFrame::NoFrame);
	qtvUsers->setUniformRowHeights(true);
	qtvUsers->setAnimated(true);
	qtvUsers->setIndentation(12);
	qtvUsers->setRootIsDecorated(false);
	qtvUsers->setExpandsOnDoubleClick(false);
	qtvUsers->setAllColumnsShowFocus(false);
	qtvUsers->setMouseTracking(true);
	qtvUsers->setMinimumWidth(0);
	qtvUsers->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	qtvUsers->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	qtvUsers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	qtvUsers->setContextMenuPolicy(Qt::CustomContextMenu);
	qtvUsers->setAttribute(Qt::WA_StyledBackground, true);
	qtvUsers->viewport()->setObjectName(QLatin1String("qtvUsersViewport"));
	qtvUsers->viewport()->setAttribute(Qt::WA_StyledBackground, true);
	qtvUsers->viewport()->setMouseTracking(true);
	qtvUsers->setStyleSheet(QString());
	qtvUsers->viewport()->setStyleSheet(QString());
	contentLayout->addWidget(qtvUsers, 3);

	m_serverNavigatorTextChannelsDivider = new QFrame(m_serverNavigatorContentFrame);
	m_serverNavigatorTextChannelsDivider->setObjectName(QLatin1String("qfServerNavigatorTextChannelsDivider"));
	m_serverNavigatorTextChannelsDivider->setFrameShape(QFrame::HLine);
	m_serverNavigatorTextChannelsDivider->setFrameShadow(QFrame::Plain);
	m_serverNavigatorTextChannelsDivider->hide();
	contentLayout->addWidget(m_serverNavigatorTextChannelsDivider);

	m_serverNavigatorTextChannelsFrame = new QFrame(m_serverNavigatorContentFrame);
	m_serverNavigatorTextChannelsFrame->setObjectName(QLatin1String("qfServerNavigatorTextChannels"));
	m_serverNavigatorTextChannelsFrame->setAttribute(Qt::WA_StyledBackground, true);
	m_serverNavigatorTextChannelsFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	QVBoxLayout *textChannelsLayout = new QVBoxLayout(m_serverNavigatorTextChannelsFrame);
	textChannelsLayout->setContentsMargins(0, 0, 0, 0);
	textChannelsLayout->setSpacing(0);

	m_serverNavigatorTextChannelsEyebrow = new QLabel(tr("Text"), m_serverNavigatorTextChannelsFrame);
	m_serverNavigatorTextChannelsEyebrow->setObjectName(QLatin1String("qlServerNavigatorTextChannelsEyebrow"));
	QFont textChannelsEyebrowFont = m_serverNavigatorTextChannelsEyebrow->font();
	textChannelsEyebrowFont.setCapitalization(QFont::AllUppercase);
	textChannelsEyebrowFont.setBold(true);
	textChannelsEyebrowFont.setPointSizeF(std::max(textChannelsEyebrowFont.pointSizeF() - 1.0, 8.0));
	m_serverNavigatorTextChannelsEyebrow->setFont(textChannelsEyebrowFont);

	m_serverNavigatorTextChannelsTitle = new QLabel(tr("Conversations"), m_serverNavigatorTextChannelsFrame);
	m_serverNavigatorTextChannelsTitle->setObjectName(QLatin1String("qlServerNavigatorTextChannelsTitle"));
	QFont textChannelsTitleFont = m_serverNavigatorTextChannelsTitle->font();
	textChannelsTitleFont.setBold(true);
	textChannelsTitleFont.setPointSizeF(textChannelsTitleFont.pointSizeF() + 0.5);
	m_serverNavigatorTextChannelsTitle->setFont(textChannelsTitleFont);
	m_serverNavigatorTextChannelsTitle->setTextFormat(Qt::PlainText);
	m_serverNavigatorTextChannelsTitle->setWordWrap(true);

	m_serverNavigatorTextChannelsSubtitle =
		new QLabel(tr("Named text rooms and voice room chat."), m_serverNavigatorTextChannelsFrame);
	m_serverNavigatorTextChannelsSubtitle->setObjectName(QLatin1String("qlServerNavigatorTextChannelsSubtitle"));
	m_serverNavigatorTextChannelsSubtitle->setWordWrap(true);
	m_serverNavigatorTextChannelsSubtitle->setTextFormat(Qt::PlainText);
	m_serverNavigatorTextChannelsSubtitle->setTextInteractionFlags(Qt::NoTextInteraction);
	m_serverNavigatorTextChannelsSubtitle->hide();

	const QSize textChannelAdminIconSize(16, 16);
	QIcon serverSettingsIcon;
	SvgIcon::addSvgPixmapsToIcon(serverSettingsIcon, QLatin1String("skin:exported/config_basic.svg"));
	m_serverNavigatorServerSettingsButton = new QToolButton(m_serverNavigatorTextChannelsFrame);
	m_serverNavigatorServerSettingsButton->setObjectName(QLatin1String("qtbServerNavigatorServerSettings"));
	m_serverNavigatorServerSettingsButton->setAutoRaise(true);
	m_serverNavigatorServerSettingsButton->setToolTip(tr("Server settings"));
	m_serverNavigatorServerSettingsButton->setAccessibleName(tr("Server settings"));
	m_serverNavigatorServerSettingsButton->setIcon(serverSettingsIcon);
	m_serverNavigatorServerSettingsButton->setIconSize(textChannelAdminIconSize);
	m_serverNavigatorServerSettingsButton->setFixedSize(22, 22);
	m_serverNavigatorServerSettingsButton->setCursor(Qt::PointingHandCursor);
	m_serverNavigatorServerSettingsButton->setFocusPolicy(Qt::NoFocus);
	m_serverNavigatorServerSettingsButton->hide();

	QIcon createTextRoomIcon;
	SvgIcon::addSvgPixmapsToIcon(createTextRoomIcon, QLatin1String("skin:document-new.svg"));
	m_serverNavigatorCreateTextChannelButton = new QToolButton(m_serverNavigatorTextChannelsFrame);
	m_serverNavigatorCreateTextChannelButton->setObjectName(QLatin1String("qtbServerNavigatorCreateTextRoom"));
	m_serverNavigatorCreateTextChannelButton->setAutoRaise(true);
	m_serverNavigatorCreateTextChannelButton->setToolTip(tr("Create room"));
	m_serverNavigatorCreateTextChannelButton->setAccessibleName(tr("Create room"));
	m_serverNavigatorCreateTextChannelButton->setIcon(createTextRoomIcon);
	m_serverNavigatorCreateTextChannelButton->setIconSize(textChannelAdminIconSize);
	m_serverNavigatorCreateTextChannelButton->setFixedSize(22, 22);
	m_serverNavigatorCreateTextChannelButton->setCursor(Qt::PointingHandCursor);
	m_serverNavigatorCreateTextChannelButton->setFocusPolicy(Qt::NoFocus);
	m_serverNavigatorCreateTextChannelButton->hide();

	QHBoxLayout *textChannelsTitleLayout = new QHBoxLayout();
	textChannelsTitleLayout->setContentsMargins(0, 0, 0, 0);
	textChannelsTitleLayout->setSpacing(4);
	textChannelsTitleLayout->addWidget(m_serverNavigatorTextChannelsTitle, 1);
	textChannelsTitleLayout->addWidget(m_serverNavigatorServerSettingsButton, 0, Qt::AlignTop);
	textChannelsTitleLayout->addWidget(m_serverNavigatorCreateTextChannelButton, 0, Qt::AlignTop);

	m_persistentChatChannelList = new QListWidget(m_serverNavigatorTextChannelsFrame);
	m_persistentChatChannelList->setObjectName(QLatin1String("qlwPersistentTextChannels"));
	m_persistentChatChannelList->setAccessibleName(tr("Conversations"));
	m_persistentChatChannelList->setFrameShape(QFrame::NoFrame);
	m_persistentChatChannelList->setAlternatingRowColors(false);
	m_persistentChatChannelList->setUniformItemSizes(true);
	m_persistentChatChannelList->setSpacing(0);
	m_persistentChatChannelList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_persistentChatChannelList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_persistentChatChannelList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_persistentChatChannelList->setSelectionMode(QAbstractItemView::SingleSelection);
	m_persistentChatChannelList->setContextMenuPolicy(Qt::CustomContextMenu);
	m_persistentChatChannelList->setMouseTracking(true);
	m_persistentChatChannelList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_persistentChatChannelList->setMinimumHeight(0);
	m_persistentChatChannelList->setMaximumHeight(QWIDGETSIZE_MAX);
	m_persistentChatChannelList->setItemDelegate(new PersistentChatScopeListDelegate(m_persistentChatChannelList));

	textChannelsLayout->addWidget(m_serverNavigatorTextChannelsEyebrow);
	textChannelsLayout->addLayout(textChannelsTitleLayout);
	textChannelsLayout->addWidget(m_serverNavigatorTextChannelsSubtitle);
	textChannelsLayout->addWidget(m_persistentChatChannelList, 1);
	m_serverNavigatorTextChannelsFrame->hide();
	contentLayout->addWidget(m_serverNavigatorTextChannelsFrame, 2);

	m_serverNavigatorFooterFrame = new QFrame(m_serverNavigatorContentFrame);
	m_serverNavigatorFooterFrame->setObjectName(QLatin1String("qfServerNavigatorFooterFrame"));
	m_serverNavigatorFooterFrame->setAttribute(Qt::WA_StyledBackground, true);
	m_serverNavigatorFooterFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	QVBoxLayout *footerLayout = new QVBoxLayout(m_serverNavigatorFooterFrame);
	footerLayout->setContentsMargins(0, 7, 0, 0);
	footerLayout->setSpacing(0);

	m_serverNavigatorFooterPresenceButton = new QPushButton(m_serverNavigatorFooterFrame);
	m_serverNavigatorFooterPresenceButton->setObjectName(QLatin1String("qpbServerNavigatorFooterPresence"));
	m_serverNavigatorFooterPresenceButton->setAccessibleName(tr("Open self menu"));
	m_serverNavigatorFooterPresenceButton->setAttribute(Qt::WA_StyledBackground, true);
	m_serverNavigatorFooterPresenceButton->setCursor(Qt::PointingHandCursor);
	m_serverNavigatorFooterPresenceButton->setFlat(true);
	m_serverNavigatorFooterPresenceButton->setCheckable(false);
	m_serverNavigatorFooterPresenceButton->setAutoDefault(false);
	m_serverNavigatorFooterPresenceButton->setDefault(false);
	m_serverNavigatorFooterPresenceButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	QHBoxLayout *footerPresenceLayout = new QHBoxLayout(m_serverNavigatorFooterPresenceButton);
	footerPresenceLayout->setContentsMargins(0, 0, 0, 0);
	footerPresenceLayout->setSpacing(7);

	m_serverNavigatorFooterAvatar = new QLabel(m_serverNavigatorFooterPresenceButton);
	m_serverNavigatorFooterAvatar->setObjectName(QLatin1String("qlServerNavigatorFooterAvatar"));
	m_serverNavigatorFooterAvatar->setAlignment(Qt::AlignCenter);
	m_serverNavigatorFooterAvatar->setFixedSize(22, 22);

	QWidget *footerInfo           = new QWidget(m_serverNavigatorFooterPresenceButton);
	QVBoxLayout *footerInfoLayout = new QVBoxLayout(footerInfo);
	footerInfoLayout->setContentsMargins(0, 0, 0, 0);
	footerInfoLayout->setSpacing(0);

	m_serverNavigatorFooterName = new QLabel(tr("You"), footerInfo);
	m_serverNavigatorFooterName->setObjectName(QLatin1String("qlServerNavigatorFooterName"));
	QFont footerNameFont = m_serverNavigatorFooterName->font();
	footerNameFont.setBold(true);
	footerNameFont.setPointSizeF(std::max(footerNameFont.pointSizeF() - 0.4, 9.6));
	m_serverNavigatorFooterName->setFont(footerNameFont);
	m_serverNavigatorFooterName->setTextFormat(Qt::PlainText);
	m_serverNavigatorFooterName->setWordWrap(false);

	m_serverNavigatorFooterPresence = new QLabel(tr("Online"), footerInfo);
	m_serverNavigatorFooterPresence->setObjectName(QLatin1String("qlServerNavigatorFooterPresence"));
	QFont footerPresenceFont = m_serverNavigatorFooterPresence->font();
	footerPresenceFont.setPointSizeF(std::max(footerPresenceFont.pointSizeF() - 1.1, 8.6));
	m_serverNavigatorFooterPresence->setFont(footerPresenceFont);
	m_serverNavigatorFooterPresence->setTextFormat(Qt::PlainText);
	m_serverNavigatorFooterPresence->setWordWrap(false);

	footerInfoLayout->addWidget(m_serverNavigatorFooterName);
	footerInfoLayout->addWidget(m_serverNavigatorFooterPresence);

	footerPresenceLayout->addWidget(m_serverNavigatorFooterAvatar, 0, Qt::AlignTop);
	footerPresenceLayout->addWidget(footerInfo, 0, Qt::AlignVCenter);
	footerPresenceLayout->addStretch(1);

	m_serverNavigatorFooter = new QLabel(tr("Ctrl+F searches the server tree."), m_serverNavigatorFooterFrame);
	m_serverNavigatorFooter->setObjectName(QLatin1String("qlServerNavigatorFooter"));
	m_serverNavigatorFooter->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_serverNavigatorFooter->setWordWrap(true);
	m_serverNavigatorFooter->setTextFormat(Qt::PlainText);
	m_serverNavigatorFooter->setTextInteractionFlags(Qt::NoTextInteraction);
	footerLayout->addWidget(m_serverNavigatorFooterPresenceButton);
	footerLayout->addWidget(m_serverNavigatorFooter);
	m_serverNavigatorFooterPresenceButton->hide();
	m_serverNavigatorFooterFrame->hide();
	contentLayout->addWidget(m_serverNavigatorFooterFrame);

	connect(m_serverNavigatorFooterPresenceButton, &QPushButton::clicked, this,
			[this]() { toggleServerNavigatorUserMenu(); });
	connect(m_serverNavigatorTextChannelsMotdToggleButton, &QToolButton::clicked, this, [this]() {
		m_persistentChatMotdExpanded = !m_persistentChatMotdExpanded;
		refreshServerNavigatorSectionHeights();
	});
	connect(m_serverNavigatorTextChannelsMotdBody, &QLabel::linkActivated, this,
			[this](const QString &link) { on_qteLog_anchorClicked(QUrl(link)); });
	connect(m_serverNavigatorServerSettingsButton, &QToolButton::clicked, this, &MainWindow::openServerSettingsDialog);
	connect(m_serverNavigatorCreateTextChannelButton, &QToolButton::clicked, this,
			&MainWindow::createPersistentTextChannel);
	const auto scheduleVoiceTreeHeightRefresh = [this]() {
		QPointer< MainWindow > guardedThis(this);
		QMetaObject::invokeMethod(
			this,
			[guardedThis]() {
				if (!guardedThis) {
					return;
				}

				guardedThis->updateServerNavigatorVoiceTreeHeight();
			},
			Qt::QueuedConnection);
	};
	if (QAbstractItemModel *userModel = qtvUsers->model()) {
		connect(userModel, &QAbstractItemModel::modelReset, this, scheduleVoiceTreeHeightRefresh);
		connect(userModel, &QAbstractItemModel::layoutChanged, this, scheduleVoiceTreeHeightRefresh);
		connect(userModel, &QAbstractItemModel::rowsInserted, this,
				[this, scheduleVoiceTreeHeightRefresh](const QModelIndex &, int, int) {
					scheduleVoiceTreeHeightRefresh();
				});
		connect(userModel, &QAbstractItemModel::rowsRemoved, this,
				[this, scheduleVoiceTreeHeightRefresh](const QModelIndex &, int, int) {
					scheduleVoiceTreeHeightRefresh();
				});
	}
	connect(qtvUsers, &QTreeView::expanded, this,
			[scheduleVoiceTreeHeightRefresh](const QModelIndex &) { scheduleVoiceTreeHeightRefresh(); });
	connect(qtvUsers, &QTreeView::collapsed, this,
			[scheduleVoiceTreeHeightRefresh](const QModelIndex &) { scheduleVoiceTreeHeightRefresh(); });
	connect(qtvUsers, &QTreeView::collapsed, this, [this](const QModelIndex &index) {
		if (!pmModel) {
			return;
		}

		if (Channel *channel = pmModel->getChannel(index); channel && channel->iId == Mumble::ROOT_CHANNEL_ID) {
			qtvUsers->setExpanded(index, true);
		}
	});
	connect(qtvUsers, &QTreeView::clicked, this, [this](const QModelIndex &index) {
		if (!index.isValid()) {
			return;
		}

		updateServerNavigatorChrome();
	});
	connect(qtvUsers, &QTreeView::activated, this, [this](const QModelIndex &index) {
		if (!index.isValid()) {
			return;
		}

		updateServerNavigatorChrome();
	});
	connect(qtvUsers, &QWidget::customContextMenuRequested, this,
			[this](const QPoint &position) { on_qtvUsers_customContextMenuRequested(position); });
	m_serverNavigatorContentFrame->installEventFilter(this);
	m_serverNavigatorTextChannelsMotdFrame->installEventFilter(this);
	m_serverNavigatorTextChannelsMotdBody->installEventFilter(this);

	layout->addWidget(m_serverNavigatorContentFrame, 1);

	refreshServerNavigatorStyles();
	refreshServerNavigatorSectionHeights();
}

void MainWindow::setupPersistentChatDock() {
	qteChat->setParent(nullptr);

	m_persistentChatContainer = new QWidget(qdwChat);
	m_persistentChatContainer->setObjectName(QLatin1String("qwPersistentChat"));
	m_persistentChatContainer->setAttribute(Qt::WA_StyledBackground, true);
	QVBoxLayout *layout = new QVBoxLayout(m_persistentChatContainer);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(2);

	qdwChat->setWindowTitle(tr("Conversation"));
	qdwChat->setMinimumWidth(360);
	qdwLog->setWindowTitle(tr("Server log"));
	qdwLog->setMinimumWidth(180);

	m_persistentChatHeaderFrame = new QFrame(m_persistentChatContainer);
	m_persistentChatHeaderFrame->setObjectName(QLatin1String("qfPersistentChatHeader"));

	QVBoxLayout *headerLayout = new QVBoxLayout(m_persistentChatHeaderFrame);
	headerLayout->setContentsMargins(7, 1, 7, 1);
	headerLayout->setSpacing(0);

	m_persistentChatHeaderEyebrow = new QLabel(tr("Text"), m_persistentChatHeaderFrame);
	m_persistentChatHeaderEyebrow->setObjectName(QLatin1String("qlPersistentChatHeaderEyebrow"));
	QFont headerEyebrowFont = m_persistentChatHeaderEyebrow->font();
	headerEyebrowFont.setCapitalization(QFont::AllUppercase);
	headerEyebrowFont.setBold(true);
	headerEyebrowFont.setPointSizeF(std::max(headerEyebrowFont.pointSizeF() - 1.0, 8.0));
	m_persistentChatHeaderEyebrow->setFont(headerEyebrowFont);

	m_persistentChatHeaderTitle = new QLabel(tr("Conversation"), m_persistentChatHeaderFrame);
	m_persistentChatHeaderTitle->setObjectName(QLatin1String("qlPersistentChatHeaderTitle"));
	m_persistentChatHeaderTitle->setTextFormat(Qt::PlainText);
	QFont headerTitleFont = m_persistentChatHeaderTitle->font();
	headerTitleFont.setBold(true);
	headerTitleFont.setPointSizeF(headerTitleFont.pointSizeF() + 0.5);
	m_persistentChatHeaderTitle->setFont(headerTitleFont);

	m_persistentChatHeaderContext = new QLabel(m_persistentChatHeaderFrame);
	m_persistentChatHeaderContext->setObjectName(QLatin1String("qlPersistentChatHeaderContext"));
	m_persistentChatHeaderContext->setAttribute(Qt::WA_StyledBackground, true);
	m_persistentChatHeaderContext->setTextFormat(Qt::PlainText);
	m_persistentChatHeaderContext->setTextInteractionFlags(Qt::NoTextInteraction);
	m_persistentChatHeaderContext->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
	m_persistentChatHeaderContext->setAlignment(Qt::AlignCenter);
	m_persistentChatHeaderContext->hide();

	m_persistentChatHeaderSubtitle = new QLabel(tr("Catch up, then reply."), m_persistentChatHeaderFrame);
	m_persistentChatHeaderSubtitle->setObjectName(QLatin1String("qlPersistentChatHeaderSubtitle"));
	m_persistentChatHeaderSubtitle->setTextFormat(Qt::PlainText);
	m_persistentChatHeaderSubtitle->setWordWrap(true);
	m_persistentChatHeaderSubtitle->setTextInteractionFlags(Qt::NoTextInteraction);

	headerLayout->addWidget(m_persistentChatHeaderEyebrow);
	headerLayout->addWidget(m_persistentChatHeaderTitle);
	headerLayout->addWidget(m_persistentChatHeaderContext);
	headerLayout->addWidget(m_persistentChatHeaderSubtitle);
	m_persistentChatHeaderFrame->hide();

	layout->addWidget(m_persistentChatHeaderFrame);

	m_persistentChatHistory = new PersistentChatListWidget(m_persistentChatContainer);
	m_persistentChatHistory->setObjectName(QLatin1String("qtePersistentChatHistory"));
	m_persistentChatHistory->setAccessibleName(tr("Persistent chat history"));
	m_persistentChatHistory->setFrameShape(QFrame::NoFrame);
	m_persistentChatHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_persistentChatHistory->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_persistentChatHistory->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	m_persistentChatHistory->setSelectionMode(QAbstractItemView::NoSelection);
	m_persistentChatHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_persistentChatHistory->setFocusPolicy(Qt::NoFocus);
	m_persistentChatHistory->setUniformItemSizes(false);
	m_persistentChatHistory->setWrapping(false);
	m_persistentChatHistory->setWordWrap(false);
	m_persistentChatHistory->setResizeMode(QListView::Adjust);
	m_persistentChatHistory->setSpacing(0);
	m_persistentChatHistory->setContextMenuPolicy(Qt::CustomContextMenu);
	m_persistentChatGateway    = new PersistentChatGateway(this);
	m_persistentChatController = new PersistentChatController(this);
	m_persistentChatController->setGateway(m_persistentChatGateway);
	m_persistentChatHistoryModel    = new PersistentChatHistoryModel(this);
	m_persistentChatHistoryDelegate = new PersistentChatHistoryDelegate(this);
	m_persistentChatHistory->setModel(m_persistentChatHistoryModel);
	m_persistentChatHistory->setItemDelegate(m_persistentChatHistoryDelegate);
	connect(m_persistentChatHistoryModel, &QAbstractItemModel::modelReset, m_persistentChatHistoryDelegate,
			&PersistentChatHistoryDelegate::clearCache);
	connect(m_persistentChatHistoryModel, &QAbstractItemModel::rowsRemoved, m_persistentChatHistoryDelegate,
			&PersistentChatHistoryDelegate::clearCache);
	m_persistentChatResizeRenderTimer = new QTimer(this);
	m_persistentChatResizeRenderTimer->setSingleShot(true);
	m_persistentChatResizeRenderTimer->setInterval(PERSISTENT_CHAT_RESIZE_RENDER_DELAY_MSEC);
	m_persistentChatScrollIdleTimer = new QTimer(this);
	m_persistentChatScrollIdleTimer->setSingleShot(true);
	m_persistentChatScrollIdleTimer->setInterval(180);
	m_userPresenceRefreshTimer = new QTimer(this);
	m_userPresenceRefreshTimer->setInterval(20000);
	connect(m_userPresenceRefreshTimer, &QTimer::timeout, this, &MainWindow::refreshUserPresenceStats);
	if (!(modernShellMinimalSnapshotEnabled() && usesModernShell())) {
		m_userPresenceRefreshTimer->start();
	}
	m_persistentChatReplyFrame = new QFrame(m_persistentChatContainer);
	m_persistentChatReplyFrame->setObjectName(QLatin1String("qfPersistentChatReply"));
	m_persistentChatReplyFrame->setAttribute(Qt::WA_StyledBackground, true);
	QHBoxLayout *replyLayout = new QHBoxLayout(m_persistentChatReplyFrame);
	replyLayout->setContentsMargins(8, 5, 8, 5);
	replyLayout->setSpacing(5);
	QVBoxLayout *replyTextLayout = new QVBoxLayout();
	replyTextLayout->setContentsMargins(0, 0, 0, 0);
	replyTextLayout->setSpacing(1);
	m_persistentChatReplyLabel = new QLabel(m_persistentChatReplyFrame);
	m_persistentChatReplyLabel->setObjectName(QLatin1String("qlPersistentChatReplyLabel"));
	m_persistentChatReplySnippet = new QLabel(m_persistentChatReplyFrame);
	m_persistentChatReplySnippet->setObjectName(QLatin1String("qlPersistentChatReplySnippet"));
	m_persistentChatReplySnippet->setWordWrap(true);
	replyTextLayout->addWidget(m_persistentChatReplyLabel);
	replyTextLayout->addWidget(m_persistentChatReplySnippet);
	replyLayout->addLayout(replyTextLayout, 1);
	m_persistentChatReplyCancelButton = new QToolButton(m_persistentChatReplyFrame);
	m_persistentChatReplyCancelButton->setText(tr("Cancel"));
	replyLayout->addWidget(m_persistentChatReplyCancelButton, 0, Qt::AlignTop);
	m_persistentChatReplyFrame->hide();
	qteChat->setAttribute(Qt::WA_StyledBackground, true);
	if (QWidget *chatViewport = qteChat->viewport()) {
		chatViewport->setAttribute(Qt::WA_StyledBackground, true);
	}
	qteChat->setFrameShape(QFrame::NoFrame);
	qteChat->setFrameStyle(QFrame::NoFrame);
	qteChat->setLineWidth(0);
	qteChat->setMidLineWidth(0);
	qteChat->resetViewportChrome();
	if (qteChat->document()) {
		qteChat->document()->setDocumentMargin(0);
	}
	qteChat->setMinimumHeight(0);
	qteChat->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

	m_persistentChatComposerFrame = new QFrame(m_persistentChatContainer);
	m_persistentChatComposerFrame->setObjectName(QLatin1String("qfPersistentChatComposer"));
	m_persistentChatComposerFrame->setAttribute(Qt::WA_StyledBackground, true);
	m_persistentChatComposerFrame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	QVBoxLayout *composerLayout = new QVBoxLayout(m_persistentChatComposerFrame);
	composerLayout->setContentsMargins(7, 0, 7, 1);
	composerLayout->setSpacing(0);
	composerLayout->addWidget(m_persistentChatReplyFrame);

	m_persistentChatComposerInputRow = new QWidget(m_persistentChatComposerFrame);
	m_persistentChatComposerInputRow->setObjectName(QLatin1String("qwPersistentChatComposerInputRow"));
	m_persistentChatComposerInputRow->setAttribute(Qt::WA_StyledBackground, true);
	m_persistentChatComposerInputRow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	m_persistentChatComposerInputRow->setMinimumHeight(28);
	QHBoxLayout *composerInputLayout = new QHBoxLayout(m_persistentChatComposerInputRow);
	composerInputLayout->setContentsMargins(6, 0, 4, 0);
	composerInputLayout->setSpacing(3);
	composerInputLayout->addWidget(qteChat, 1);

	const QSize composerIconSize(16, 16);
	const QColor composerIconColor(0x5a, 0x5f, 0x6b);
	const QColor composerIconHoverColor(0x79, 0x7f, 0x8d);
	const QColor composerIconDisabledColor = alphaColor(composerIconColor, 0.55);

	m_persistentChatAttachButton = new QToolButton(m_persistentChatComposerInputRow);
	m_persistentChatAttachButton->setObjectName(QLatin1String("qtbPersistentChatAttach"));
	m_persistentChatAttachButton->setAutoRaise(true);
	m_persistentChatAttachButton->setToolTip(tr("Attach image"));
	m_persistentChatAttachButton->setIcon(
		persistentChatComposerGlyphIcon(PersistentChatComposerGlyph::Attach, composerIconSize, composerIconColor,
										composerIconHoverColor, composerIconDisabledColor));
	m_persistentChatAttachButton->setIconSize(composerIconSize);
	m_persistentChatAttachButton->setFixedSize(22, 22);
	m_persistentChatAttachButton->setCursor(Qt::PointingHandCursor);
	m_persistentChatAttachButton->setFocusPolicy(Qt::NoFocus);
	composerInputLayout->addWidget(m_persistentChatAttachButton, 0, Qt::AlignVCenter);

	m_persistentChatSendButton = new QToolButton(m_persistentChatComposerInputRow);
	m_persistentChatSendButton->setObjectName(QLatin1String("qtbPersistentChatSend"));
	m_persistentChatSendButton->setAutoRaise(true);
	m_persistentChatSendButton->setToolTip(tr("Send message"));
	m_persistentChatSendButton->setIcon(
		persistentChatComposerGlyphIcon(PersistentChatComposerGlyph::Send, composerIconSize, composerIconColor,
										composerIconHoverColor, composerIconDisabledColor));
	m_persistentChatSendButton->setIconSize(composerIconSize);
	m_persistentChatSendButton->setFixedSize(22, 22);
	m_persistentChatSendButton->setCursor(Qt::PointingHandCursor);
	m_persistentChatSendButton->setFocusPolicy(Qt::NoFocus);
	composerInputLayout->addWidget(m_persistentChatSendButton, 0, Qt::AlignVCenter);
	composerLayout->addWidget(m_persistentChatComposerInputRow);

	m_persistentChatConversationPanel = new QWidget(m_persistentChatContainer);
	m_persistentChatConversationPanel->setObjectName(QLatin1String("qwPersistentChatPanel"));
	m_persistentChatConversationPanel->setAttribute(Qt::WA_StyledBackground, true);
	QVBoxLayout *chatLayout = new QVBoxLayout();
	chatLayout->setContentsMargins(0, 0, 0, 0);
	chatLayout->setSpacing(0);
	chatLayout->addWidget(m_persistentChatHistory, 1);
	chatLayout->addWidget(m_persistentChatComposerFrame);

	m_persistentChatLogPanel = new QWidget(m_persistentChatContainer);
	m_persistentChatLogPanel->setObjectName(QLatin1String("qwPersistentChatLogPanel"));
	m_persistentChatLogPanel->setAttribute(Qt::WA_StyledBackground, true);
	QVBoxLayout *logPanelLayout = new QVBoxLayout(m_persistentChatLogPanel);
	logPanelLayout->setContentsMargins(0, 0, 0, 0);
	logPanelLayout->setSpacing(0);

	m_persistentChatLogView = new LogTextBrowser(m_persistentChatLogPanel);
	m_persistentChatLogView->setObjectName(QLatin1String("qtePersistentChatLog"));
	m_persistentChatLogView->setAccessibleName(tr("Server log"));
	m_persistentChatLogView->setFrameShape(QFrame::NoFrame);
	m_persistentChatLogView->setFrameStyle(QFrame::NoFrame);
	m_persistentChatLogView->setLineWidth(0);
	m_persistentChatLogView->setMidLineWidth(0);
	m_persistentChatLogView->setAttribute(Qt::WA_StyledBackground, true);
	m_persistentChatLogView->setOpenLinks(false);
	m_persistentChatLogView->setContextMenuPolicy(Qt::CustomContextMenu);
	m_persistentChatLogView->setMouseTracking(true);
	m_persistentChatLogView->resetViewportChrome();
	if (QWidget *logViewport = m_persistentChatLogView->viewport()) {
		logViewport->setAttribute(Qt::WA_StyledBackground, true);
	}
	if (qteLog && qteLog->document()) {
		m_persistentChatLogView->setDocument(qteLog->document());
	}
	logPanelLayout->addWidget(m_persistentChatLogView, 1);
	m_persistentChatLogPanel->hide();
	chatLayout->insertWidget(1, m_persistentChatLogPanel, 1);
	m_persistentChatConversationPanel->setLayout(chatLayout);

	QWidget *contentSurface = new QWidget(m_persistentChatContainer);
	contentSurface->setObjectName(QLatin1String("qwPersistentChatSurface"));
	contentSurface->setAttribute(Qt::WA_StyledBackground, true);
	QVBoxLayout *contentSurfaceLayout = new QVBoxLayout(contentSurface);
	contentSurfaceLayout->setContentsMargins(0, 0, 0, 0);
	contentSurfaceLayout->setSpacing(0);
	contentSurfaceLayout->addWidget(m_persistentChatConversationPanel, 1);

	layout->addWidget(contentSurface, 1);

	qdwChat->setWidget(m_persistentChatContainer);
	if (QLayout *dockLayout = qdwChat->layout()) {
		dockLayout->setContentsMargins(0, 0, 0, 0);
		dockLayout->setSpacing(0);
	}

	refreshPersistentChatStyles();

	auto schedulePersistentChatScopeRefresh = [this](bool forceReload) {
		QPointer< MainWindow > guardedThis(this);
		QMetaObject::invokeMethod(
			this,
			[guardedThis, forceReload]() {
				if (!guardedThis) {
					return;
				}

				guardedThis->updatePersistentTextChannelControls();
				guardedThis->updateChatBar(forceReload);
			},
			Qt::QueuedConnection);
	};

	auto activatePersistentChatChannelItem = [this, schedulePersistentChatScopeRefresh](QListWidgetItem *item,
																						bool forceReload) {
		if (!item || !m_persistentChatChannelList) {
			return;
		}

		cachePersistentChatChannelSelection(item);
		if (persistentChatChannelListCurrentItem() != item) {
			m_persistentChatChannelList->setCurrentItem(item);
		}

		schedulePersistentChatScopeRefresh(forceReload);
	};

	connect(m_persistentChatChannelList, &QListWidget::currentItemChanged, this,
			[this](QListWidgetItem *current, QListWidgetItem *) { cachePersistentChatChannelSelection(current); });
	connect(m_persistentChatChannelList, SIGNAL(currentRowChanged(int)), this,
			SLOT(on_persistentChatScopeChanged(int)));
	connect(m_persistentChatChannelList, &QListWidget::itemSelectionChanged, this, [this]() {
		if (!m_persistentChatChannelList) {
			return;
		}

		const QList< QListWidgetItem * > selectedItems = persistentChatChannelListSelectedItems();
		if (!selectedItems.isEmpty()) {
			setPersistentChatTargetUsesVoiceTree(false);
		}
		if (selectedItems.size() == 1 && persistentChatChannelListCurrentItem() != selectedItems.front()) {
			cachePersistentChatChannelSelection(selectedItems.front());
			m_persistentChatChannelList->setCurrentItem(selectedItems.front());
		}
	});
	connect(m_persistentChatChannelList, &QListWidget::itemClicked, this,
			[this, activatePersistentChatChannelItem](QListWidgetItem *item) {
				if (!item) {
					return;
				}

				setPersistentChatTargetUsesVoiceTree(false);
				const int clickedScopeValue       = item->data(PersistentChatScopeRole).toInt();
				const unsigned int clickedScopeID = item->data(PersistentChatScopeIDRole).toUInt();
				const bool clickedVisibleScope =
					m_visiblePersistentChatScope && clickedScopeValue != LocalServerLogScope
					&& static_cast< int >(*m_visiblePersistentChatScope) == clickedScopeValue
					&& m_visiblePersistentChatScopeID == clickedScopeID;
				activatePersistentChatChannelItem(item, clickedVisibleScope);
			});
	connect(m_persistentChatChannelList, &QListWidget::customContextMenuRequested, this,
			&MainWindow::showPersistentTextChannelContextMenu);
	connect(m_persistentChatLogView, &LogTextBrowser::customContextMenuRequested, this,
			&MainWindow::on_qteLog_customContextMenuRequested);
	connect(m_persistentChatLogView, &LogTextBrowser::imageActivated, this,
			[this](const QTextCursor &cursor) { openImageDialog(m_persistentChatLogView, cursor); });
	connect(m_persistentChatLogView, &LogTextBrowser::anchorClicked, this, &MainWindow::on_qteLog_anchorClicked);
	connect(m_persistentChatLogView, &LogTextBrowser::highlighted, this,
			QOverload< const QUrl & >::of(&MainWindow::on_qteLog_highlighted));
	connect(m_persistentChatLogView->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int) {
		if (m_persistentChatLogView) {
			m_persistentChatLogStickToBottom = m_persistentChatLogView->isScrolledToBottom();
		}
	});
	connect(m_persistentChatLogView->verticalScrollBar(), &QScrollBar::rangeChanged, this, [this](int, int maximum) {
		if (m_persistentChatLogStickToBottom && m_persistentChatLogPanel && m_persistentChatLogPanel->isVisible()
			&& m_persistentChatLogView) {
			m_persistentChatLogView->verticalScrollBar()->setValue(maximum);
		}
	});
	connect(m_persistentChatReplyCancelButton, &QToolButton::clicked, this,
			[this]() { setPersistentChatReplyTarget(std::nullopt); });
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::loadOlderRequested, this,
			&MainWindow::requestOlderPersistentChatHistory);
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::previewRequested, this,
			[this](const QString &previewKey) { ensurePersistentChatPreview(previewKey); });
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::replyRequested, this,
			[this](unsigned int messageID) {
				if (const MumbleProto::ChatMessage *message =
						findPersistentChatMessageByID(m_persistentChatMessages, messageID)) {
					setPersistentChatReplyTarget(*message);
				}
			});
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::deleteRequested, this,
			[this](unsigned int messageID) { deletePersistentChatMessage(messageID); });
	connect(
		m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::scopeJumpRequested, this,
		[this](MumbleProto::ChatScope scope, unsigned int scopeID) { navigateToPersistentChatScope(scope, scopeID); });
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::logContextMenuRequested, this,
			[this](LogTextBrowser *browser, const QPoint &position) { showLogContextMenu(browser, position); });
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::logImageActivated, this,
			[this](LogTextBrowser *browser, const QTextCursor &cursor) { openImageDialog(browser, cursor); });
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::anchorClicked, this,
			&MainWindow::on_qteLog_anchorClicked);
	connect(m_persistentChatHistoryDelegate, &PersistentChatHistoryDelegate::highlighted, this,
			QOverload< const QUrl & >::of(&MainWindow::on_qteLog_highlighted));
	connect(m_persistentChatController, &PersistentChatController::activeSnapshotChanged, this, [this]() {
		const std::optional< MumbleProto::ChatScope > previousScope = m_visiblePersistentChatScope;
		const unsigned int previousScopeID                          = m_visiblePersistentChatScopeID;
		const PersistentChatScopeStateSnapshot snapshot             = m_persistentChatController
															  ? m_persistentChatController->activeSnapshot()
															  : PersistentChatScopeStateSnapshot();
		const bool switchingScope =
			previousScope.has_value() != snapshot.key.valid
			|| (previousScope.has_value()
				&& (!snapshot.key.valid || !snapshot.key.matches(*previousScope, previousScopeID)));
		const bool wasAtBottom = m_persistentChatHistory ? m_persistentChatHistory->isScrolledToBottom() : true;
		m_persistentChatMessages.assign(snapshot.messages.cbegin(), snapshot.messages.cend());
		if (m_pendingPersistentChatReply) {
			const MumbleProto::ChatMessage *replyTarget =
				findPersistentChatMessageByID(m_persistentChatMessages, m_pendingPersistentChatReply->message_id());
			if (replyTarget && replyTarget->has_deleted_at() && replyTarget->deleted_at() > 0) {
				clearPersistentChatReplyTarget(false);
			}
		}
		if (snapshot.key.valid) {
			m_visiblePersistentChatScope   = snapshot.key.scope;
			m_visiblePersistentChatScopeID = snapshot.key.scopeID;
		} else {
			m_visiblePersistentChatScope.reset();
			m_visiblePersistentChatScopeID = 0;
		}
		m_visiblePersistentChatLastReadMessageID = snapshot.lastReadMessageId;
		m_visiblePersistentChatOldestMessageID   = snapshot.oldestLoadedMessageId;
		m_visiblePersistentChatHasMore           = snapshot.hasOlder;
		m_persistentChatLoadingOlder             = snapshot.loadingState == PersistentChatLoadingState::Older;
		const bool preserveScrollPosition = m_persistentChatRestoreAnchorPending || (!switchingScope && !wasAtBottom);
		const bool scrollToBottom =
			switchingScope || wasAtBottom || snapshot.loadingState == PersistentChatLoadingState::Initial;
		renderPersistentChatView(QString(), scrollToBottom, preserveScrollPosition);
		if (qdwChat && qdwChat->isVisible()) {
			markPersistentChatRead(false);
		}
	});
	connect(m_persistentChatController, &PersistentChatController::unreadStateChanged, this,
			[this]() { updatePersistentChatScopeSelectorLabels(); });
	connect(m_persistentChatController, &PersistentChatController::activeReadStateChanged, this,
			[this](unsigned int lastReadMessageID, int unreadCount) {
				m_visiblePersistentChatLastReadMessageID = lastReadMessageID;
				if (!m_persistentChatHistoryModel || !m_persistentChatHistory || unreadCount > 0) {
					return;
				}

				if (m_persistentChatHistoryModel->removeUnreadDivider()) {
					m_persistentChatHistory->stabilizeVisibleContent();
				}
			});
	connect(qteChat, &QTextEdit::textChanged, this, [this]() {
		const bool keepBottomPinned = m_persistentChatHistory && m_persistentChatHistory->isScrolledToBottom();
		updatePersistentChatSendButton();
		if (!keepBottomPinned || !m_persistentChatHistory) {
			return;
		}

		QTimer::singleShot(0, this, [this]() {
			if (!m_persistentChatHistory) {
				return;
			}

			if (QScrollBar *scrollBar = m_persistentChatHistory->verticalScrollBar()) {
				scrollBar->setValue(scrollBar->maximum());
			}
		});
	});
	connect(m_persistentChatAttachButton, &QToolButton::clicked, this, [this]() {
		if (!m_persistentChatAttachButton || !m_persistentChatAttachButton->isEnabled()) {
			return;
		}

		openPersistentChatImagePicker();
	});
	connect(m_persistentChatSendButton, &QToolButton::clicked, this, [this]() {
		if (!qteChat || !qteChat->isEnabled() || qteChat->isShowingDefaultText()) {
			return;
		}

		const QString message = qteChat->toPlainText().trimmed();
		if (message.isEmpty()) {
			return;
		}

		sendChatbarText(qteChat->toPlainText());
	});
	connect(m_persistentChatResizeRenderTimer, &QTimer::timeout, this, [this]() {
		if (m_pendingPersistentChatViewportWidth <= 0 || !m_persistentChatHistory || m_persistentChatMessages.empty()) {
			return;
		}

		if (m_lastPersistentChatViewportWidth >= 0
			&& std::abs(m_pendingPersistentChatViewportWidth - m_lastPersistentChatViewportWidth) < 8) {
			return;
		}

		m_lastPersistentChatViewportWidth = m_pendingPersistentChatViewportWidth;
		renderPersistentChatView(QString(), false, true);
	});
	connect(m_persistentChatHistory, &PersistentChatListWidget::contentWidthChanged, this, [this](int width) {
		if (width <= 0 || !m_persistentChatHistory || m_persistentChatMessages.empty()) {
			return;
		}

		if (m_lastPersistentChatViewportWidth >= 0 && std::abs(width - m_lastPersistentChatViewportWidth) < 8) {
			return;
		}

		m_pendingPersistentChatViewportWidth = width;
		m_persistentChatResizeRenderTimer->start();
	});
	connect(m_persistentChatHistory->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int) {
		if (m_persistentChatScrollIdleTimer) {
			m_persistentChatScrollIdleTimer->start();
		}
	});
	connect(m_persistentChatScrollIdleTimer, &QTimer::timeout, this, [this]() {
		if (!m_persistentChatPreviewRefreshPending || !m_persistentChatHistory) {
			markPersistentChatRead(false);
			return;
		}

		m_persistentChatPreviewRefreshPending = false;
		const bool wasAtBottom                = m_persistentChatHistory->isScrolledToBottom();
		renderPersistentChatView(QString(), wasAtBottom, !wasAtBottom);
		markPersistentChatRead(false);
	});
	connect(qdwChat, &QDockWidget::visibilityChanged, this, [this](bool visible) {
		if (visible) {
			markPersistentChatRead(false);
		}
	});
	connect(this, &MainWindow::windowActivated, this, [this]() { markPersistentChatRead(false); });
	syncPersistentChatGatewayHandler();
	rebuildPersistentChatChannelList();
	updatePersistentChatScopeSelectorLabels();
	updatePersistentTextChannelControls();
	clearPersistentChatView(tr("Connect to a server to load conversations and history."), tr("Start a conversation"),
							{ tr("Open Server to connect"), tr("Room chat and history appear here") });
}

void MainWindow::activateModernShell() {
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	const int modernShellHostPresent = m_modernShellHost ? 1 : 0;
#else
	const int modernShellHostPresent = 0;
#endif
	appendModernShellConnectTrace(QStringLiteral("activateModernShell enter host=%1 container=%2 chatContainer=%3")
									  .arg(modernShellHostPresent)
									  .arg(m_serverNavigatorContainer ? 1 : 0)
									  .arg(m_persistentChatContainer ? 1 : 0));
	if (!m_serverNavigatorContainer) {
		setupServerNavigator();
		appendModernShellConnectTrace(QStringLiteral("activateModernShell setupServerNavigator"));
	}
	if (!m_persistentChatContainer) {
		setupPersistentChatDock();
		appendModernShellConnectTrace(QStringLiteral("activateModernShell setupPersistentChatDock"));
	}

#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (!m_modernShellHost) {
		m_modernShellHost = new ModernShellHost(this);
		appendModernShellConnectTrace(QStringLiteral("activateModernShell created-host"));
		connect(m_modernShellHost, &ModernShellHost::bootFailed, this, &MainWindow::handleModernShellBootFailure);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::scopeSelectionRequested, this,
				&MainWindow::handleModernShellScopeSelection);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::voiceJoinRequested, this,
				&MainWindow::handleModernShellVoiceJoin);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::scopeActionRequested, this,
				&MainWindow::handleModernShellScopeAction);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::scopeActionValueChangedRequested, this,
				&MainWindow::handleModernShellScopeActionValueChanged);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::messageSendRequested, this,
				[this](const QString &message) { sendChatbarText(message); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::replyStartRequested, this,
				&MainWindow::handleModernShellReplyStart);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::replyCancelRequested, this,
				&MainWindow::handleModernShellReplyCancel);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::reactionToggleRequested, this,
				&MainWindow::handleModernShellReactionToggle);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::messageDeleteRequested, this,
				&MainWindow::handleModernShellMessageDelete);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::participantMessageRequested, this,
				&MainWindow::handleModernShellParticipantMessage);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::participantJoinRequested, this,
				&MainWindow::handleModernShellParticipantJoin);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::participantMoveRequested, this,
				&MainWindow::handleModernShellParticipantMove);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::participantActionRequested, this,
				&MainWindow::handleModernShellParticipantAction);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::participantActionValueChangedRequested, this,
				&MainWindow::handleModernShellParticipantActionValueChanged);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::channelMoveRequested, this,
				&MainWindow::handleModernShellChannelMove);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::olderHistoryRequested, this,
				&MainWindow::requestOlderPersistentChatHistory);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::markReadRequested, this,
				[this]() { markPersistentChatRead(false); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::selfMuteToggleRequested, this,
				[this]() { setAudioMute(!Global::get().s.bMute); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::selfDeafToggleRequested, this,
				[this]() { setAudioDeaf(!Global::get().s.bDeaf); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::connectDialogRequested, this,
				[this]() { on_qaServerConnect_triggered(false); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::disconnectRequested, this,
				&MainWindow::on_qaServerDisconnect_triggered);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::settingsRequested, this,
				&MainWindow::openConfigDialog);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::clipboardImageAttachmentRequested, this,
				[this]() { attachPersistentChatClipboardImage(); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::imagePickerRequested, this,
				&MainWindow::openPersistentChatImagePicker);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::imageDataAttachmentRequested, this,
				[this](const QString &dataUrl) { attachPersistentChatImageData(dataUrl); });
		connect(m_modernShellHost->bridge(), &ModernShellBridge::linkActivationRequested, this,
				[this](const QString &href) {
					QUrl url(href);
					if (!url.isValid()) {
						url = QUrl::fromUserInput(href);
					}

					if (!url.isEmpty()) {
						on_qteLog_anchorClicked(url);
					}
				});
		connect(m_modernShellHost->bridge(), &ModernShellBridge::appActionRequested, this,
				&MainWindow::handleModernShellAppAction);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::layoutToggleRequested, this,
				&MainWindow::togglePreferredModernShellLayout);
		connect(m_modernShellHost->bridge(), &ModernShellBridge::bootReady, this,
				&MainWindow::queueModernShellSnapshotSync);
		connect(m_modernShellHost, &ModernShellHost::imageDropped, this, &MainWindow::attachPersistentChatImage);
		connect(m_modernShellHost, &ModernShellHost::imageUrlsDropped, this, &MainWindow::attachPersistentChatImages);
		QString modernShellError;
		if (!m_modernShellHost->start(&modernShellError)) {
			appendModernShellConnectTrace(
				QStringLiteral("activateModernShell start-failed error=%1").arg(modernShellError));
			if (!modernShellError.trimmed().isEmpty()) {
				msgBox(modernShellError);
			}
			m_modernShellHost->deleteLater();
			m_modernShellHost = nullptr;
			activateLegacyShell();
			return;
		}
		appendModernShellConnectTrace(QStringLiteral("activateModernShell start-ok"));
	}

	if (centralWidget() != m_modernShellHost) {
		QPointer< QWidget > previousCentral = centralWidget();
		QPointer< QWidget > navigatorGuard(m_serverNavigatorContainer);
		QPointer< QTreeView > treeGuard(qtvUsers);
		if (previousCentral && previousCentral != m_modernShellHost) {
			if (takeCentralWidget() == previousCentral) {
				previousCentral->hide();
				previousCentral->setParent(this);
			}
		}
		setCentralWidget(m_modernShellHost);
		appendModernShellConnectTrace(QStringLiteral("activateModernShell set-central-widget prev=%1 nav=%2 tree=%3")
										  .arg(previousCentral ? 1 : 0)
										  .arg(navigatorGuard ? 1 : 0)
										  .arg(treeGuard ? 1 : 0));
	}
	if (m_modernShellHost) {
		m_modernShellHost->show();
		appendModernShellConnectTrace(QStringLiteral("activateModernShell host-show"));
	}
#else
	if (centralWidget() != m_serverNavigatorContainer) {
		setCentralWidget(m_serverNavigatorContainer);
	}
#endif
	if (m_serverNavigatorContainer) {
		m_serverNavigatorContainer->hide();
	}

	if (m_logSurface && qdwLog->widget() != m_logSurface) {
		if (QWidget *currentLogWidget = qdwLog->widget()) {
			currentLogWidget->hide();
			currentLogWidget->setParent(nullptr);
		}
		if (QLayout *logLayout = m_logSurface->layout()) {
			if (logLayout->indexOf(qteLog) < 0) {
				logLayout->addWidget(qteLog);
			}
		}
		qdwLog->setWidget(m_logSurface);
	}
	if (m_logSurface) {
		m_logSurface->show();
	}

	if (m_persistentChatContainer && qdwChat->widget() != m_persistentChatContainer) {
		if (QWidget *currentChatWidget = qdwChat->widget()) {
			currentChatWidget->hide();
			currentChatWidget->setParent(nullptr);
		}
		if (m_persistentChatComposerInputRow) {
			if (QHBoxLayout *composerInputLayout =
					qobject_cast< QHBoxLayout * >(m_persistentChatComposerInputRow->layout())) {
				if (composerInputLayout->indexOf(qteChat) < 0) {
					composerInputLayout->addWidget(qteChat, 1);
				}
			}
		}
		qdwChat->setWidget(m_persistentChatContainer);
	}
	if (m_persistentChatContainer) {
		m_persistentChatContainer->show();
	}

	if (!dtbLogDockTitle) {
		dtbLogDockTitle = new DockTitleBar();
		qdwLog->setTitleBarWidget(dtbLogDockTitle);
		dtbLogDockTitle->setMinimumHeight(0);
		dtbLogDockTitle->setMaximumHeight(0);
		for (QWidget *w : qdwLog->findChildren< QWidget * >()) {
			w->installEventFilter(dtbLogDockTitle);
			w->setMouseTracking(true);
		}
	}
	if (!dtbChatDockTitle) {
		dtbChatDockTitle = new DockTitleBar();
		qdwChat->setTitleBarWidget(dtbChatDockTitle);
		dtbChatDockTitle->setMinimumHeight(0);
		dtbChatDockTitle->setMaximumHeight(0);
		qdwChat->installEventFilter(dtbChatDockTitle);
	}

	qdwLog->setWindowTitle(tr("Server log"));
	qdwChat->setWindowTitle(tr("Conversation"));
	rebuildPersistentChatChannelList();
	appendModernShellConnectTrace(QStringLiteral("activateModernShell rebuilt-channel-list"));
	if (!modernShellMinimalSnapshotEnabled()) {
		QPointer< MainWindow > guardedThis(this);
		QMetaObject::invokeMethod(
			this,
			[guardedThis]() {
				if (!guardedThis || !guardedThis->usesModernShell()) {
					return;
				}

				guardedThis->updateChatBar();
			},
			Qt::QueuedConnection);
	}
	queueModernShellSnapshotSync();
	appendModernShellConnectTrace(QStringLiteral("activateModernShell exit"));
}

void MainWindow::handleModernShellBootFailure(const QString &reason) {
	appendModernShellConnectTrace(QStringLiteral("handleModernShellBootFailure reason=%1").arg(reason));
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (m_modernShellHost) {
		m_modernShellHost->hide();
		m_modernShellHost->deleteLater();
		m_modernShellHost = nullptr;
	}
#endif

	m_modernShellRuntimeDisabled = true;
	if (m_activeShellLayout == Settings::LayoutModern || usesModernShell()) {
		refreshShellLayout();
	}

	if (!reason.trimmed().isEmpty()) {
		msgBox(reason);
	}
}

void MainWindow::activateLegacyShell() {
	appendModernShellConnectTrace(QStringLiteral("activateLegacyShell enter"));
	if (centralWidget() != qtvUsers) {
		QPointer< QWidget > previousCentral = centralWidget();
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
		QPointer< QWidget > modernHostGuard(m_modernShellHost);
#else
		QPointer< QWidget > modernHostGuard;
#endif
		QPointer< QTreeView > treeGuard(qtvUsers);
		if (previousCentral && previousCentral != qtvUsers) {
			if (takeCentralWidget() == previousCentral) {
				previousCentral->hide();
				previousCentral->setParent(this);
			}
		}
		setCentralWidget(qtvUsers);
		appendModernShellConnectTrace(QStringLiteral("activateLegacyShell set-central-widget prev=%1 host=%2 tree=%3")
										  .arg(previousCentral ? 1 : 0)
										  .arg(modernHostGuard ? 1 : 0)
										  .arg(treeGuard ? 1 : 0));
	}
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (m_modernShellHost) {
		m_modernShellHost->hide();
	}
#endif

	if (qdwLog->widget() != qteLog) {
		if (QWidget *currentLogWidget = qdwLog->widget()) {
			currentLogWidget->hide();
			currentLogWidget->setParent(nullptr);
		}
		if (m_logSurface) {
			m_logSurface->hide();
		}
		qteLog->setParent(nullptr);
		qdwLog->setWidget(qteLog);
	}

	if (qdwChat->widget() != qteChat) {
		if (QWidget *currentChatWidget = qdwChat->widget()) {
			currentChatWidget->hide();
			currentChatWidget->setParent(nullptr);
		}
		if (m_persistentChatContainer) {
			m_persistentChatContainer->hide();
		}
		qteChat->setParent(nullptr);
		qdwChat->setWidget(qteChat);
	}

	if (m_serverNavigatorContainer) {
		m_serverNavigatorContainer->hide();
	}

	if (dtbLogDockTitle) {
		qdwLog->setTitleBarWidget(nullptr);
		dtbLogDockTitle = nullptr;
	}
	if (dtbChatDockTitle) {
		qdwChat->setTitleBarWidget(nullptr);
		dtbChatDockTitle = nullptr;
	}

	qdwLog->setWindowTitle(tr("Server log"));
	qdwChat->setWindowTitle(tr("Chatbar"));
	queueModernShellSnapshotSync();
	appendModernShellConnectTrace(QStringLiteral("activateLegacyShell exit"));
}

void MainWindow::queueModernShellSnapshotSync() {
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (!m_modernShellSyncTimer || !m_modernShellHost) {
		appendModernShellConnectTrace(QStringLiteral("queueModernShellSnapshotSync skipped timer=%1 host=%2")
										  .arg(m_modernShellSyncTimer ? 1 : 0)
										  .arg(m_modernShellHost ? 1 : 0));
		return;
	}
	if (modernShellStaticModeEnabled()) {
		appendModernShellConnectTrace(QStringLiteral("queueModernShellSnapshotSync skipped-static"));
		return;
	}

	if (m_nativeWindowMoveResizeActive) {
		m_modernShellSnapshotPendingAfterNativeMoveResize = true;
		if (m_modernShellSyncTimer->isActive()) {
			m_modernShellSyncTimer->stop();
		}
		appendModernShellConnectTrace(QStringLiteral("queueModernShellSnapshotSync deferred-window-move"));
		return;
	}

	if (m_modernShellSyncTimer->isActive()) {
		appendModernShellConnectTrace(QStringLiteral("queueModernShellSnapshotSync coalesced remaining=%1")
										  .arg(m_modernShellSyncTimer->remainingTime()));
		return;
	}

	const int baseDelayMs = (isActiveWindow() && !isMinimized()) ? ModernShellSnapshotActiveCoalesceMs
																 : ModernShellSnapshotInactiveCoalesceMs;
	int delayMs = baseDelayMs;
	if (m_modernShellLastSnapshotSyncMs > 0) {
		const qint64 elapsedMs = QDateTime::currentMSecsSinceEpoch() - m_modernShellLastSnapshotSyncMs;
		if (elapsedMs >= 0 && elapsedMs < baseDelayMs) {
			delayMs = std::max(1, static_cast< int >(baseDelayMs - elapsedMs));
		}
	}

	appendModernShellConnectTrace(QStringLiteral("queueModernShellSnapshotSync queued delay=%1").arg(delayMs));
	m_modernShellSyncTimer->start(delayMs);
#endif
}

void MainWindow::syncModernShellSnapshot() {
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (!m_modernShellHost || !m_modernShellHost->bridge()) {
		appendModernShellConnectTrace(QStringLiteral("syncModernShellSnapshot skipped host=%1 bridge=%2")
										  .arg(m_modernShellHost ? 1 : 0)
										  .arg((m_modernShellHost && m_modernShellHost->bridge()) ? 1 : 0));
		return;
	}

	if (m_nativeWindowMoveResizeActive) {
		m_modernShellSnapshotPendingAfterNativeMoveResize = true;
		appendModernShellConnectTrace(QStringLiteral("syncModernShellSnapshot deferred-window-move"));
		return;
	}

	appendModernShellConnectTrace(QStringLiteral("syncModernShellSnapshot enter"));
	const QVariantMap snapshot    = buildModernShellSnapshot();
	const QVariantList textRooms  = snapshot.value(QStringLiteral("textRooms")).toList();
	const QVariantList voiceRooms = snapshot.value(QStringLiteral("voiceRooms")).toList();
	const QVariantList messages   = snapshot.value(QStringLiteral("messages")).toList();
	appendModernShellConnectTrace(QStringLiteral("syncModernShellSnapshot built text=%1 voice=%2 messages=%3")
									  .arg(textRooms.size())
									  .arg(voiceRooms.size())
									  .arg(messages.size()));
	appendModernShellConnectTrace(QStringLiteral("syncModernShellSnapshot before-setSnapshot"));
	m_modernShellHost->bridge()->setSnapshot(snapshot);
	m_modernShellLastSnapshotSyncMs = QDateTime::currentMSecsSinceEpoch();
	appendModernShellConnectTrace(QStringLiteral("syncModernShellSnapshot after-setSnapshot"));
#endif
}

void MainWindow::beginNativeWindowMoveOrResize() {
	if (m_nativeWindowMoveResizeRecoveryTimer) {
		m_nativeWindowMoveResizeRecoveryTimer->start(NativeWindowMoveResizeWatchdogMs);
	}

	if (m_nativeWindowMoveResizeActive) {
		return;
	}

	m_nativeWindowMoveResizeActive = true;
	appendModernShellConnectTrace(QStringLiteral("nativeWindowMoveResize begin"));
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (m_modernShellSyncTimer && m_modernShellSyncTimer->isActive()) {
		m_modernShellSyncTimer->stop();
		m_modernShellSnapshotPendingAfterNativeMoveResize = true;
	}
#endif
}

void MainWindow::endNativeWindowMoveOrResize() {
	if (!m_nativeWindowMoveResizeActive) {
		return;
	}

	if (m_nativeWindowMoveResizeRecoveryTimer && m_nativeWindowMoveResizeRecoveryTimer->isActive()) {
		m_nativeWindowMoveResizeRecoveryTimer->stop();
	}

	m_nativeWindowMoveResizeActive = false;
	appendModernShellConnectTrace(QStringLiteral("nativeWindowMoveResize end"));
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	if (m_modernShellSnapshotPendingAfterNativeMoveResize) {
		m_modernShellSnapshotPendingAfterNativeMoveResize = false;
		queueModernShellSnapshotSync();
	}
#endif
}

#if defined(MUMBLE_HAS_MODERN_LAYOUT)
QVariantList MainWindow::serializeModernShellMenu(QMenu *menu, const ModernShellMenuContext context,
												  ModernShellMenuSerializer::ActionRegistry *registry) const {
	return ModernShellMenuSerializer::serializeMenu(
		menu,
		[this, context](const QAction *action) {
			return modernShellActionDefinition(context, const_cast< QAction * >(action));
		},
		registry);
}

ModernShellMenuSerializer::ActionDefinition
	MainWindow::modernShellActionDefinition(const ModernShellMenuContext context, QAction *action) const {
	ModernShellMenuSerializer::ActionDefinition definition;
	if (!action || action->menu()) {
		return definition;
	}

	const auto assignDynamicContextAction = [&definition, action](const QString &scope) {
		definition.id                = ModernShellMenuSerializer::contextActionId(scope, action->data());
		definition.contextActionData = action->data().toString().trimmed();
	};
	const auto assignTone = [&definition](const QString &tone) { definition.tone = tone; };

	switch (context) {
		case ModernShellMenuContext::AppServer:
			if (action == qaServerConnect) {
				definition.id = QStringLiteral("server.connect");
			} else if (action == qaServerDisconnect) {
				definition.id = QStringLiteral("server.disconnect");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaCreateTextRoom) {
				definition.id = QStringLiteral("server.createRoom");
			} else if (action == qaServerInformation) {
				definition.id = QStringLiteral("server.information");
			} else if (action == qaServerAddToFavorites) {
				definition.id = QStringLiteral("server.favorite");
			} else if (action == qaSearch) {
				definition.id = QStringLiteral("server.search");
			} else if (action == qaServerTokens) {
				definition.id = QStringLiteral("server.tokens");
			} else if (action == qaServerUserList) {
				definition.id = QStringLiteral("server.userList");
			} else if (action == qaServerBanList) {
				definition.id = QStringLiteral("server.banList");
			} else if (action == qaQuit) {
				definition.id = QStringLiteral("server.quit");
				assignTone(QStringLiteral("danger"));
			} else if (qlServerActions.contains(action)) {
				assignDynamicContextAction(QStringLiteral("server"));
			}
			break;
		case ModernShellMenuContext::AppSelf:
			if (action == qaAudioMute) {
				definition.id = QStringLiteral("self.toggleMute");
			} else if (action == qaAudioDeaf) {
				definition.id = QStringLiteral("self.toggleDeaf");
			} else if (action == qaSelfPrioritySpeaker) {
				definition.id = QStringLiteral("self.prioritySpeaker");
			} else if (action == qaRecording) {
				definition.id = QStringLiteral("self.recording");
			} else if (action == qaSelfComment) {
				definition.id = QStringLiteral("self.comment");
			} else if (action == qaServerTexture) {
				definition.id = QStringLiteral("self.avatarChange");
			} else if (action == qaServerTextureRemove) {
				definition.id = QStringLiteral("self.avatarRemove");
			} else if (action == qaSelfRegister) {
				definition.id = QStringLiteral("self.register");
			} else if (action == qaAudioStats) {
				definition.id = QStringLiteral("self.audioStats");
			}
			break;
		case ModernShellMenuContext::AppConfigure:
			if (action == qaConfigDialog) {
				definition.id = QStringLiteral("configure.settings");
			} else if (action == qaAudioWizard) {
				definition.id = QStringLiteral("configure.audioWizard");
			} else if (action == qaConfigCert) {
				definition.id = QStringLiteral("configure.certificate");
			} else if (action == qaAudioTTS) {
				definition.id = QStringLiteral("configure.tts");
			} else if (action == qaConfigMinimal) {
				definition.id = QStringLiteral("configure.minimal");
			} else if (action == qaFilterToggle) {
				definition.id = QStringLiteral("configure.filterToggle");
			} else if (action == qaTalkingUIToggle) {
				definition.id = QStringLiteral("configure.talkingUi");
			} else if (action == qaConfigHideFrame) {
				definition.id = QStringLiteral("configure.hideFrame");
			}
			break;
		case ModernShellMenuContext::AppHelp:
			if (action == qaHelpWhatsThis) {
				definition.id = QStringLiteral("help.whatsThis");
			} else if (action == qaHelpVersionCheck) {
				definition.id = QStringLiteral("help.versionCheck");
			} else if (action == qaHelpAbout) {
				definition.id = QStringLiteral("help.about");
			} else if (action == qaHelpAboutQt) {
				definition.id = QStringLiteral("help.aboutQt");
			}
			break;
		case ModernShellMenuContext::Participant:
			if (action == qaUserJoin) {
				definition.id = QStringLiteral("join");
			} else if (action == qaUserMove) {
				definition.id = QStringLiteral("move");
			} else if (action == qaUserKick) {
				definition.id = QStringLiteral("kick");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaUserBan) {
				definition.id = QStringLiteral("ban");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaUserMute) {
				definition.id = QStringLiteral("mute");
			} else if (action == qaUserDeaf) {
				definition.id = QStringLiteral("deaf");
			} else if (action == qaUserPrioritySpeaker || action == qaSelfPrioritySpeaker) {
				definition.id = QStringLiteral("prioritySpeaker");
			} else if (action == qaUserLocalMute) {
				definition.id = QStringLiteral("localMute");
			} else if (action == qaUserRemoteSpeechCleanup) {
				definition.id = QStringLiteral("remoteSpeechCleanup");
			} else if (action == qaUserLocalIgnore) {
				definition.id = QStringLiteral("ignoreMessages");
			} else if (action == qaUserLocalIgnoreTTS) {
				definition.id = QStringLiteral("ignoreTts");
			} else if (action == m_userLocalVolumeSlider.get()) {
				definition.id = QStringLiteral("localVolume");
			} else if (action == qaUserLocalNickname) {
				definition.id = QStringLiteral("localNickname");
			} else if (action == qaSelfComment) {
				definition.id = QStringLiteral("selfComment");
			} else if (action == qaUserCommentView) {
				definition.id = QStringLiteral("commentView");
			} else if (action == qaUserCommentReset) {
				definition.id = QStringLiteral("commentReset");
			} else if (action == qaUserTextureReset) {
				definition.id = QStringLiteral("textureReset");
			} else if (action == qaUserTextMessage) {
				definition.id = QStringLiteral("textMessage");
			} else if (action == qaUserInformation) {
				definition.id = QStringLiteral("userInfo");
			} else if (action == qaChannelScreenShareStart) {
				definition.id = QStringLiteral("screenShareStart");
			} else if (action == qaChannelScreenShareStop) {
				definition.id = QStringLiteral("screenShareStop");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaChannelScreenShareWatch) {
				definition.id = QStringLiteral("screenShareWatch");
			} else if (action == qaChannelScreenShareStopWatching) {
				definition.id = QStringLiteral("screenShareStopWatching");
			} else if (action == qaChannelScreenShareOpenWindow) {
				definition.id = QStringLiteral("screenShareOpenWindow");
			} else if (action == qaUserRegister || action == qaSelfRegister) {
				definition.id = QStringLiteral("register");
			} else if (action == qaUserFriendAdd) {
				definition.id = QStringLiteral("friendAdd");
			} else if (action == qaUserFriendUpdate) {
				definition.id = QStringLiteral("friendUpdate");
			} else if (action == qaUserFriendRemove) {
				definition.id = QStringLiteral("friendRemove");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaAudioMute) {
				definition.id = QStringLiteral("self.toggleMute");
			} else if (action == qaAudioDeaf) {
				definition.id = QStringLiteral("self.toggleDeaf");
			} else if (action == qaRecording) {
				definition.id = QStringLiteral("recording");
			} else if (action == qaServerTexture) {
				definition.id = QStringLiteral("avatarChange");
			} else if (action == qaServerTextureRemove) {
				definition.id = QStringLiteral("avatarRemove");
			} else if (action == qaAudioStats) {
				definition.id = QStringLiteral("audioStats");
			} else if (qlUserActions.contains(action)) {
				assignDynamicContextAction(QStringLiteral("user"));
			}
			break;
		case ModernShellMenuContext::Scope:
			if (action == qaChannelJoin) {
				definition.id = QStringLiteral("join");
			} else if (action == qaChannelListen) {
				definition.id = QStringLiteral("listen");
			} else if (action == qaChannelScreenShareStart) {
				definition.id = QStringLiteral("screenShareStart");
			} else if (action == qaChannelScreenShareStop) {
				definition.id = QStringLiteral("screenShareStop");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaChannelScreenShareWatch) {
				definition.id = QStringLiteral("screenShareWatch");
			} else if (action == qaChannelScreenShareStopWatching) {
				definition.id = QStringLiteral("screenShareStopWatching");
			} else if (action == qaChannelScreenShareOpenWindow) {
				definition.id = QStringLiteral("screenShareOpenWindow");
			} else if (action == qaChannelAdd) {
				definition.id    = QStringLiteral("add");
				definition.label = tr("Create room...");
			} else if (action == qaChannelACL) {
				definition.id    = QStringLiteral("acl");
				definition.label = tr("Edit room ACL...");
			} else if (action == qaChannelRemove) {
				definition.id    = QStringLiteral("remove");
				definition.label = tr("Remove room...");
				assignTone(QStringLiteral("danger"));
			} else if (action == qaChannelLink) {
				definition.id    = QStringLiteral("link");
				definition.label = tr("Link room");
			} else if (action == qaChannelUnlink) {
				definition.id    = QStringLiteral("unlink");
				definition.label = tr("Unlink room");
			} else if (action == qaChannelUnlinkAll) {
				definition.id    = QStringLiteral("unlinkAll");
				definition.label = tr("Unlink all rooms");
			} else if (action == qaChannelCopyURL) {
				definition.id    = QStringLiteral("copyUrl");
				definition.label = tr("Copy room URL");
			} else if (action == qaChannelSendMessage) {
				definition.id    = QStringLiteral("sendMessage");
				definition.label = tr("Send room message...");
			} else if (action == qaChannelHide) {
				definition.id = QStringLiteral("hide");
			} else if (action == qaChannelPin) {
				definition.id = QStringLiteral("pin");
			} else if (qlChannelActions.contains(action)) {
				assignDynamicContextAction(QStringLiteral("channel"));
			}
			break;
		case ModernShellMenuContext::Listener:
			if (action == m_listenerVolumeSlider.get()) {
				definition.id = QStringLiteral("listenerVolume");
			} else if (action == qaChannelListen) {
				definition.id = QStringLiteral("listen");
			}
			break;
	}

	return definition;
}

bool MainWindow::triggerModernShellSerializedAction(const ModernShellMenuSerializer::ActionRegistry &registry,
													const QString &actionId, ClientUser *contextUser,
													Channel *contextChannel) {
	const QString normalizedActionID = actionId.trimmed();
	if (normalizedActionID.isEmpty()) {
		return false;
	}

	const auto entryIt = registry.constFind(normalizedActionID);
	if (entryIt == registry.cend()) {
		return false;
	}

	const ModernShellMenuSerializer::RegistryEntry &entry = entryIt.value();
	if (!entry.action || !entry.action->isEnabled()) {
		return false;
	}

	if (!entry.contextActionData.isEmpty()) {
		triggerContextAction(entry.contextActionData, contextUser, contextChannel);
		return true;
	}

	entry.action->trigger();
	return true;
}

QVariantMap MainWindow::buildModernShellSnapshot() {
	appendModernShellConnectTrace(QStringLiteral("buildModernShellSnapshot enter"));
	QVariantMap snapshot;
	QVariantMap appState;
	QVariantMap activeScope;
	QVariantList textRooms;
	QVariantList voiceRooms;
	QVariantList messages;

	const Settings::WindowLayout storedLayout    = Global::get().s.wlWindowLayout;
	const Settings::WindowLayout effectiveLayout = effectiveWindowLayout();
	const bool forcedModernLayout                = Global::get().s.modernLayoutPolicy == Settings::ModernLayoutForced;
	const bool connected = Global::get().uiSession != 0 && Global::get().sh && Global::get().sh->isRunning();
	const bool autoSwitchedModern =
		!forcedModernLayout && effectiveLayout == Settings::LayoutModern && storedLayout != Settings::LayoutModern
		&& Global::get().s.bAutoSwitchModernOnCompatibleServers && m_modernLayoutCompatibleServer;

	QString host;
	QString username;
	QString password;
	unsigned short port = 0;
	if (Global::get().sh) {
		Global::get().sh->getConnectionInfo(host, port, username, password);
	}

	const QString serverLabel =
		host.trimmed().isEmpty() ? tr("Mumble")
								 : (port == 0 || port == DEFAULT_MUMBLE_PORT ? host : tr("%1:%2").arg(host).arg(port));
	const Channel *joinedVoiceChannel   = currentVoiceChannel();
	const Channel *selfVoiceChannel     = currentVoiceChannel();
	const ClientUser *selfUser          = ClientUser::get(Global::get().uiSession);
	const bool canUsePersistedReactions = selfUser && selfUser->iId >= 0;
	appState.insert(QStringLiteral("serverEyebrow"), connected ? tr("Connected server") : tr("Mumble"));
	appState.insert(QStringLiteral("serverTitle"), connected ? serverLabel : tr("Modern Layout"));
	appState.insert(QStringLiteral("serverSubtitle"),
					connected ? (username.trimmed().isEmpty()
									 ? tr("Room-first shell with persistent chat history")
									 : tr("Room-first shell with persistent chat history as %1").arg(username))
							  : tr("Connect to a server to populate rooms, activity, and history."));
	appState.insert(QStringLiteral("motdHtml"), persistentChatContentHtml(m_persistentChatWelcomeText));
	appState.insert(QStringLiteral("motdSummary"), persistentChatPlainTextSummary(m_persistentChatWelcomeText));
	appState.insert(QStringLiteral("layoutLabel"), autoSwitchedModern ? tr("Modern (auto)") : tr("Modern"));
	appState.insert(QStringLiteral("layoutTone"), QStringLiteral("accent"));
	appState.insert(QStringLiteral("layoutSwitchLabel"), forcedModernLayout
															 ? tr("Modern layout is required by this build")
															 : tr("Switch to classic layout"));
	appState.insert(QStringLiteral("layoutSwitchTargetLabel"), tr("Classic"));
	appState.insert(QStringLiteral("canToggleLayout"), !forcedModernLayout);
	appState.insert(QStringLiteral("connectionLabel"), connected ? tr("Connected") : tr("Disconnected"));
	appState.insert(QStringLiteral("connectionTone"), connected ? QStringLiteral("success") : QStringLiteral("danger"));
	appState.insert(QStringLiteral("compatibilityLabel"),
					!connected
						? tr("Waiting for server config")
						: (m_modernLayoutCompatibleServer ? tr("Fork features detected") : tr("Standard server")));
	appState.insert(QStringLiteral("compatibilityTone"),
					!connected
						? QStringLiteral("warning")
						: (m_modernLayoutCompatibleServer ? QStringLiteral("accent") : QStringLiteral("warning")));
	appState.insert(QStringLiteral("canConnect"), !connected);
	appState.insert(QStringLiteral("canDisconnect"), connected);
	appState.insert(QStringLiteral("selfMuted"), Global::get().s.bMute);
	appState.insert(QStringLiteral("selfDeafened"), Global::get().s.bDeaf);
	appState.insert(QStringLiteral("selfName"),
					selfUser ? selfUser->qsName : (!username.trimmed().isEmpty() ? username : tr("You")));
	appState.insert(
		QStringLiteral("selfStatusLabel"),
		!connected ? tr("Offline")
				   : (Global::get().s.bDeaf ? tr("Deafened") : (Global::get().s.bMute ? tr("Muted") : tr("Online"))));
	appState.insert(QStringLiteral("selfStatusTone"),
					!connected ? QStringLiteral("muted")
							   : (Global::get().s.bDeaf ? QStringLiteral("danger")
														: (Global::get().s.bMute ? QStringLiteral("warning")
																				 : QStringLiteral("success"))));
	appState.insert(QStringLiteral("selfVoiceLabel"), selfVoiceChannel ? selfVoiceChannel->qsName : QString());
	appState.insert(QStringLiteral("selfAvatarUrl"), modernShellAvatarDataUrl(selfUser, 56));
	if (const Channel *rootVoiceChannel = Channel::get(Mumble::ROOT_CHANNEL_ID)) {
		appState.insert(QStringLiteral("voiceRootLabel"), rootVoiceChannel->qsName);
		appState.insert(QStringLiteral("voiceRootScopeToken"),
						modernShellScopeToken(static_cast< int >(MumbleProto::Channel), rootVoiceChannel->iId));
	}
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	on_qmConfig_aboutToShow();
	qmHelp->ensurePolished();
	QVariantList appMenus;
	const auto addAppMenu = [&appMenus](const QString &id, const QString &menuLabel, const QVariantList &items) {
		QVariantMap menu;
		menu.insert(QStringLiteral("id"), id);
		menu.insert(QStringLiteral("label"), menuLabel);
		menu.insert(QStringLiteral("items"), items);
		appMenus.push_back(menu);
	};
	addAppMenu(QStringLiteral("server"), ModernShellMenuSerializer::normalizedActionLabel(qmServer->title()),
			   serializeModernShellMenu(qmServer, ModernShellMenuContext::AppServer));
	addAppMenu(QStringLiteral("self"), ModernShellMenuSerializer::normalizedActionLabel(qmSelf->title()),
			   serializeModernShellMenu(qmSelf, ModernShellMenuContext::AppSelf));
	addAppMenu(QStringLiteral("configure"), ModernShellMenuSerializer::normalizedActionLabel(qmConfig->title()),
			   serializeModernShellMenu(qmConfig, ModernShellMenuContext::AppConfigure));
	addAppMenu(QStringLiteral("help"), ModernShellMenuSerializer::normalizedActionLabel(qmHelp->title()),
			   serializeModernShellMenu(qmHelp, ModernShellMenuContext::AppHelp));
	for (QVariant &menuEntryVariant : appMenus) {
		QVariantMap menuEntry = menuEntryVariant.toMap();
		if (menuEntry.value(QStringLiteral("id")).toString() != QLatin1String("configure")) {
			continue;
		}

		QVariantList menuItems = menuEntry.value(QStringLiteral("items")).toList();
		menuItems.push_back(ModernShellMenuSerializer::separatorItem());
		menuItems.push_back(ModernShellMenuSerializer::actionItem(QStringLiteral("configure.screenShare"),
																  tr("Screen sharing settings"), true, false));
		menuEntry.insert(QStringLiteral("items"), ModernShellMenuSerializer::normalize(menuItems));
		menuEntryVariant = menuEntry;
	}
	appState.insert(QStringLiteral("menus"), appMenus);

	const bool selfIdle = connected && selfUser && isUserIdle(selfUser->uiSession);
	const ServerNavigatorPresenceState selfPresenceState =
		!connected
			? ServerNavigatorPresenceState::Online
			: (Global::get().s.bDeaf ? ServerNavigatorPresenceState::Deafened
									 : (Global::get().s.bMute ? ServerNavigatorPresenceState::Muted
															  : (selfIdle ? ServerNavigatorPresenceState::Away
																		  : ServerNavigatorPresenceState::Online)));
	QVariantMap selfMenu;
	selfMenu.insert(QStringLiteral("name"), appState.value(QStringLiteral("selfName")));
	selfMenu.insert(QStringLiteral("statusLabel"), appState.value(QStringLiteral("selfStatusLabel")));
	selfMenu.insert(QStringLiteral("statusTone"), appState.value(QStringLiteral("selfStatusTone")));
	QVariantList selfPresenceItems;
	const auto appendSelfPresenceItem = [&selfPresenceItems, connected, selfPresenceState](
											const QString &id, ServerNavigatorPresenceState state, const QString &tone,
											const QString &hint = QString(), bool enabled = true) {
		QVariantMap item;
		item.insert(QStringLiteral("kind"), QStringLiteral("action"));
		item.insert(QStringLiteral("id"), id);
		item.insert(QStringLiteral("label"), serverNavigatorPresenceStateLabel(state));
		item.insert(QStringLiteral("enabled"), connected && enabled);
		item.insert(QStringLiteral("checked"), connected && selfPresenceState == state);
		if (!tone.isEmpty()) {
			item.insert(QStringLiteral("tone"), tone);
		}
		if (!hint.isEmpty()) {
			item.insert(QStringLiteral("hint"), hint);
		}
		selfPresenceItems.push_back(item);
	};
	appendSelfPresenceItem(QStringLiteral("self.presence.online"), ServerNavigatorPresenceState::Online,
						   QStringLiteral("success"));
	appendSelfPresenceItem(QStringLiteral("self.presence.away"), ServerNavigatorPresenceState::Away,
						   QStringLiteral("warning"), tr("Away is shown automatically when you are idle."), false);
	appendSelfPresenceItem(QStringLiteral("self.presence.muted"), ServerNavigatorPresenceState::Muted,
						   QStringLiteral("warning"));
	appendSelfPresenceItem(QStringLiteral("self.presence.deafened"), ServerNavigatorPresenceState::Deafened,
						   QStringLiteral("danger"));
	selfMenu.insert(QStringLiteral("presence"), selfPresenceItems);

	QVariantList selfMenuActions;
	selfMenuActions.push_back(ModernShellMenuSerializer::actionItem(
		QStringLiteral("self.toggleMute"), tr("Self mute"), qaAudioMute->isEnabled(),
		qaAudioMute->isCheckable() && qaAudioMute->isChecked()));
	selfMenuActions.push_back(ModernShellMenuSerializer::actionItem(
		QStringLiteral("self.toggleDeaf"), tr("Self deafen"), qaAudioDeaf->isEnabled(),
		qaAudioDeaf->isCheckable() && qaAudioDeaf->isChecked()));
	if (const ClientUser *user = ClientUser::get(Global::get().uiSession); qaSelfRegister && user) {
		selfMenuActions.push_back(ModernShellMenuSerializer::separatorItem());
		if (user->iId < 0) {
			QString registrationHint;
			if (user->qsHash.isEmpty()) {
				registrationHint = tr("Connect with a certificate before registering on this server.");
			} else if (!(Global::get().pPermissions & (ChanACL::SelfRegister | ChanACL::Write))) {
				registrationHint = tr("The server is not allowing self-registration for this account.");
			}
			selfMenuActions.push_back(
				ModernShellMenuSerializer::actionItem(QStringLiteral("self.register"), tr("Register on server"),
													  qaSelfRegister->isEnabled(), false, QString(), registrationHint));
		} else {
			selfMenuActions.push_back(ModernShellMenuSerializer::actionItem(
				QStringLiteral("self.registrationStatus"), tr("Registered on server"), false, false, QString(),
				tr("This certificate is already registered on this server.")));
		}
	}
	selfMenuActions.push_back(ModernShellMenuSerializer::separatorItem());
	selfMenuActions.push_back(ModernShellMenuSerializer::actionItem(
		QStringLiteral("configure.settings"), tr("Settings"), qaConfigDialog->isEnabled(), false));
	selfMenuActions.push_back(ModernShellMenuSerializer::actionItem(
		QStringLiteral("configure.screenShare"), tr("Screen sharing settings"), qaConfigDialog->isEnabled(), false));
	selfMenuActions.push_back(ModernShellMenuSerializer::separatorItem());
	selfMenuActions.push_back(ModernShellMenuSerializer::actionItem(QStringLiteral("server.disconnect"),
																	tr("Disconnect"), qaServerDisconnect->isEnabled(),
																	false, QStringLiteral("danger")));
	selfMenu.insert(QStringLiteral("actions"), selfMenuActions);
	appState.insert(QStringLiteral("selfMenu"), selfMenu);
	snapshot.insert(QStringLiteral("app"), appState);

	if (modernShellMinimalSnapshotEnabled()) {
		appendModernShellConnectTrace(
			QStringLiteral("buildModernShellSnapshot minimal connected=%1").arg(connected ? 1 : 0));
		activeScope.insert(QStringLiteral("kindLabel"), connected ? tr("Conversation") : tr("Disconnected"));
		activeScope.insert(QStringLiteral("label"), connected ? serverLabel : tr("Modern Layout"));
		activeScope.insert(QStringLiteral("description"),
						   connected ? tr("Connected in minimal shared-shell mode.")
									 : tr("Connect to a server to populate the shared shell."));
		activeScope.insert(QStringLiteral("banner"),
						   connected
							   ? tr("Detailed room and history sync is temporarily disabled for this shared-shell run.")
							   : QString());
		activeScope.insert(QStringLiteral("scopeToken"),
						   connected ? QStringLiteral("minimal:connected") : QStringLiteral("minimal:disconnected"));
		activeScope.insert(QStringLiteral("meta"), QVariantList());
		activeScope.insert(QStringLiteral("canSend"), false);
		activeScope.insert(QStringLiteral("canLoadOlder"), false);
		activeScope.insert(QStringLiteral("canMarkRead"), false);
		activeScope.insert(QStringLiteral("composerPlaceholder"),
						   connected ? tr("Shared shell safe mode") : tr("Connect to chat"));
		activeScope.insert(QStringLiteral("composerHint"),
						   tr("The shared WebEngine bootstrap is running with a reduced snapshot payload."));
		activeScope.insert(QStringLiteral("emptyCopy"),
						   tr("Room lists and message history are temporarily disabled in this shared-shell mode."));
		activeScope.insert(QStringLiteral("scrollToBottom"), true);
		activeScope.insert(QStringLiteral("autoMarkRead"), false);
		snapshot.insert(QStringLiteral("textRooms"), QVariantList());
		snapshot.insert(QStringLiteral("voiceRooms"), QVariantList());
		snapshot.insert(QStringLiteral("activeScope"), activeScope);
		snapshot.insert(QStringLiteral("messages"), QVariantList());
		appendModernShellConnectTrace(QStringLiteral("buildModernShellSnapshot minimal-return"));
		return snapshot;
	}

	const PersistentChatTarget target       = currentPersistentChatTarget();
	const auto restoreModernShellInputState = [this]() {
		const PersistentChatTarget restoredTarget = currentPersistentChatTarget();
		syncPersistentChatInputState(restoredTarget.valid && !restoredTarget.readOnly && !restoredTarget.serverLog);
	};
	const auto buildParticipantActions = [this, restoreModernShellInputState](ClientUser *user) {
		QVariantList actions;
		if (!user) {
			return actions;
		}

		const QPointer< ClientUser > previousUser = cuContextUser;
		const QPointer< Channel > previousChannel = cContextChannel;
		const QPoint previousContextPosition      = qpContextPosition;

		cuContextUser     = user;
		cContextChannel   = user->cChannel;
		qpContextPosition = QPoint();
		qmUser_aboutToShow();
		actions = serializeModernShellMenu(qmUser, ModernShellMenuContext::Participant);

		cuContextUser     = previousUser;
		cContextChannel   = previousChannel;
		qpContextPosition = previousContextPosition;
		restoreModernShellInputState();
		return actions;
	};
	const auto buildChannelActions = [this, restoreModernShellInputState](Channel *channel, bool voiceRoomContext) {
		QVariantList actions;
		if (!channel) {
			return actions;
		}

		const QPointer< ClientUser > previousUser = cuContextUser;
		const QPointer< Channel > previousChannel = cContextChannel;
		const QPoint previousContextPosition      = qpContextPosition;

		cuContextUser     = nullptr;
		cContextChannel   = channel;
		qpContextPosition = QPoint();
		qmChannel_aboutToShow();
		actions = serializeModernShellMenu(qmChannel, ModernShellMenuContext::Scope);
		if (!voiceRoomContext) {
			QVariantList filteredActions;
			for (const QVariant &actionVariant : actions) {
				const QVariantMap action = actionVariant.toMap();
				const QString actionID   = action.value(QStringLiteral("id")).toString();
				if (actionID == QLatin1String("join") || actionID == QLatin1String("listen")
					|| actionID == QLatin1String("screenShareStart") || actionID == QLatin1String("screenShareStop")
					|| actionID == QLatin1String("screenShareWatch")
					|| actionID == QLatin1String("screenShareStopWatching")
					|| actionID == QLatin1String("screenShareOpenWindow")) {
					continue;
				}

				filteredActions.push_back(actionVariant);
			}
			actions = ModernShellMenuSerializer::normalize(filteredActions);
		}

		cuContextUser     = previousUser;
		cContextChannel   = previousChannel;
		qpContextPosition = previousContextPosition;
		restoreModernShellInputState();
		return actions;
	};
	const bool exposeChannelMenuInAppChrome =
		target.channel && (target.scope == MumbleProto::TextChannel || target.scope == MumbleProto::Channel);
	if (exposeChannelMenuInAppChrome) {
		QVariantMap channelMenu;
		channelMenu.insert(QStringLiteral("id"), QStringLiteral("channel"));
		channelMenu.insert(QStringLiteral("label"),
						   ModernShellMenuSerializer::normalizedActionLabel(qmChannel->title()));
		channelMenu.insert(QStringLiteral("items"),
						   buildChannelActions(target.channel, target.scope == MumbleProto::Channel));
		appMenus.insert(1, channelMenu);
		appState.insert(QStringLiteral("menus"), appMenus);
		snapshot.insert(QStringLiteral("app"), appState);
	}
	const auto buildListenerActions = [this, restoreModernShellInputState](ClientUser *owner, Channel *channel) {
		QVariantList actions;
		if (!owner || !channel) {
			return actions;
		}

		const QPointer< ClientUser > previousUser = cuContextUser;
		const QPointer< Channel > previousChannel = cContextChannel;
		const QPoint previousContextPosition      = qpContextPosition;

		cuContextUser     = owner;
		cContextChannel   = channel;
		qpContextPosition = QPoint();
		qmListener_aboutToShow();
		actions = serializeModernShellMenu(qmListener, ModernShellMenuContext::Listener);

		cuContextUser     = previousUser;
		cContextChannel   = previousChannel;
		qpContextPosition = previousContextPosition;
		restoreModernShellInputState();
		return actions;
	};
	const auto sortParticipantUsers = [selfUser](QList< const ClientUser * > &users) {
		std::sort(users.begin(), users.end(), [selfUser](const ClientUser *lhs, const ClientUser *rhs) {
			if (lhs == selfUser || rhs == selfUser) {
				return lhs == selfUser;
			}
			if (!lhs || !rhs) {
				return lhs != nullptr;
			}
			const bool lhsActive = lhs->tsState != Settings::Passive;
			const bool rhsActive = rhs->tsState != Settings::Passive;
			if (lhsActive != rhsActive) {
				return lhsActive;
			}

			const QString lhsName = lhs->qsName.toCaseFolded();
			const QString rhsName = rhs->qsName.toCaseFolded();
			if (lhsName != rhsName) {
				return lhsName < rhsName;
			}

			return lhs->uiSession < rhs->uiSession;
		});
	};
	const auto buildParticipantState = [this, connected, selfUser, selfVoiceChannel,
										&buildParticipantActions](const ClientUser *user, const Channel *contextChannel,
																  const ClientUser *directMessagePeer, int avatarSize) {
		QVariantMap participant;
		if (!user) {
			return participant;
		}

		PersistentChatTarget participantTarget;
		participantTarget.valid         = true;
		participantTarget.directMessage = true;
		participantTarget.user          = const_cast< ClientUser * >(user);
		participantTarget.channel       = user->cChannel;

		participant.insert(QStringLiteral("entryKind"), QStringLiteral("user"));
		participant.insert(QStringLiteral("participantKey"),
						   QStringLiteral("user:%1").arg(static_cast< qulonglong >(user->uiSession)));
		participant.insert(QStringLiteral("session"), static_cast< qulonglong >(user->uiSession));
		participant.insert(QStringLiteral("label"), user->qsName);
		participant.insert(QStringLiteral("subtitle"),
						   modernShellParticipantSubtitle(user, contextChannel, selfUser, directMessagePeer));
		participant.insert(QStringLiteral("isSelf"), user == selfUser);
		participant.insert(QStringLiteral("avatarUrl"), modernShellAvatarDataUrl(user, avatarSize));
		participant.insert(QStringLiteral("talkState"), modernShellTalkStateKey(user));
		participant.insert(QStringLiteral("talkLabel"), modernShellTalkStateLabel(user));
		participant.insert(QStringLiteral("talkTone"), modernShellTalkStateTone(user));
		participant.insert(QStringLiteral("talking"), user->tsState != Settings::Passive);
		participant.insert(QStringLiteral("badges"), modernShellParticipantBadges(user, selfUser));
		participant.insert(QStringLiteral("statuses"), modernShellParticipantStatuses(user));
		participant.insert(QStringLiteral("canMessage"),
						   connected && user != selfUser && canSendToPersistentChatTarget(participantTarget, false));
		participant.insert(QStringLiteral("canJoin"), connected && user != selfUser && selfVoiceChannel
														  && user->cChannel
														  && selfVoiceChannel->iId != user->cChannel->iId);
		participant.insert(QStringLiteral("actions"),
						   connected ? buildParticipantActions(const_cast< ClientUser * >(user)) : QVariantList());
		return participant;
	};
	const auto buildListenerState = [this, connected, selfUser, &buildListenerActions](
										const ClientUser *user, const Channel *channel, int avatarSize) {
		QVariantMap participant;
		if (!user || !channel) {
			return participant;
		}

		QVariantList badges;
		badges.push_back(tr("Listener"));

		participant.insert(QStringLiteral("entryKind"), QStringLiteral("listener"));
		participant.insert(QStringLiteral("participantKey"), QStringLiteral("listener:%1:%2")
																 .arg(static_cast< qulonglong >(channel->iId))
																 .arg(static_cast< qulonglong >(user->uiSession)));
		participant.insert(QStringLiteral("session"), static_cast< qulonglong >(user->uiSession));
		participant.insert(QStringLiteral("ownerSession"), static_cast< qulonglong >(user->uiSession));
		participant.insert(QStringLiteral("channelId"), static_cast< qulonglong >(channel->iId));
		participant.insert(QStringLiteral("scopeToken"),
						   modernShellScopeToken(static_cast< int >(MumbleProto::Channel), channel->iId));
		participant.insert(QStringLiteral("label"), user->qsName);
		participant.insert(QStringLiteral("subtitle"),
						   user == selfUser ? tr("You are listening in this room") : tr("Listening in this room"));
		participant.insert(QStringLiteral("isSelf"), user == selfUser);
		participant.insert(QStringLiteral("avatarUrl"), modernShellAvatarDataUrl(user, avatarSize));
		participant.insert(QStringLiteral("talkState"), modernShellTalkStateKey(user));
		participant.insert(QStringLiteral("talkLabel"), modernShellTalkStateLabel(user));
		participant.insert(QStringLiteral("talkTone"), modernShellTalkStateTone(user));
		participant.insert(QStringLiteral("talking"), user->tsState != Settings::Passive);
		participant.insert(QStringLiteral("badges"), badges);

		QVariantList statuses;
		statuses.push_back(
			modernShellParticipantStatus(QStringLiteral("listener"), tr("Listener"), QStringLiteral("accent")));
		for (const QVariant &status : modernShellParticipantStatuses(user)) {
			statuses.push_back(status);
		}
		participant.insert(QStringLiteral("statuses"), statuses);
		participant.insert(QStringLiteral("canMessage"), false);
		participant.insert(QStringLiteral("canJoin"), false);
		participant.insert(QStringLiteral("actions"), connected ? buildListenerActions(const_cast< ClientUser * >(user),
																					   const_cast< Channel * >(channel))
																: QVariantList());
		return participant;
	};
	const auto buildChannelParticipants = [this, &sortParticipantUsers, &buildParticipantState,
										   &buildListenerState](const Channel *channel, int avatarSize) {
		QVariantList roomParticipants;
		if (!channel) {
			return roomParticipants;
		}

		QList< const ClientUser * > listenerUsers;
		if (Global::get().channelListenerManager) {
			for (const unsigned int session :
				 Global::get().channelListenerManager->getListenersForChannel(channel->iId)) {
				const ClientUser *user = ClientUser::get(session);
				if (user) {
					listenerUsers.push_back(user);
				}
			}
		}

		QList< const ClientUser * > roomUsers;
		for (const User *baseUser : channel->qlUsers) {
			const ClientUser *user = baseUser ? ClientUser::get(baseUser->uiSession) : nullptr;
			if (user) {
				roomUsers.push_back(user);
			}
		}

		sortParticipantUsers(listenerUsers);
		sortParticipantUsers(roomUsers);
		for (const ClientUser *user : listenerUsers) {
			roomParticipants.push_back(buildListenerState(user, channel, avatarSize));
		}
		for (const ClientUser *user : roomUsers) {
			roomParticipants.push_back(buildParticipantState(user, channel, nullptr, avatarSize));
		}
		return roomParticipants;
	};
	const auto buildVoiceRoomScreenShareState = [this, selfUser, joinedVoiceChannel](const Channel *channel) {
		QVariantMap shareState;
		shareState.insert(QStringLiteral("visible"), channel != nullptr && m_screenShareManager != nullptr);
		shareState.insert(QStringLiteral("available"), channel != nullptr && m_screenShareManager != nullptr);
		shareState.insert(QStringLiteral("mode"), QStringLiteral("idle"));
		shareState.insert(QStringLiteral("ownerLabel"), QString());
		shareState.insert(QStringLiteral("ownerSession"), 0);
		shareState.insert(QStringLiteral("streamId"), QString());
		shareState.insert(QStringLiteral("detachedWindowOpen"), false);
		shareState.insert(QStringLiteral("usingFallback"), false);
		shareState.insert(QStringLiteral("statusLabel"), QString());
		shareState.insert(QStringLiteral("statusTone"), QStringLiteral("muted"));
		shareState.insert(QStringLiteral("fallbackLabel"), QString());
		shareState.insert(QStringLiteral("primaryActionId"), QString());
		shareState.insert(QStringLiteral("primaryLabel"), QString());
		shareState.insert(QStringLiteral("primaryEnabled"), false);
		shareState.insert(QStringLiteral("primaryHint"), QString());
		shareState.insert(QStringLiteral("primaryTone"), QString());
		shareState.insert(QStringLiteral("overflowActions"), QVariantList());
		shareState.insert(QStringLiteral("badgeLabel"), QString());
		shareState.insert(QStringLiteral("badgeTone"), QString());
		shareState.insert(QStringLiteral("scopeToken"),
						  channel ? modernShellScopeToken(static_cast< int >(MumbleProto::Channel), channel->iId)
								  : QString());

		if (!channel || !m_screenShareManager) {
			return shareState;
		}

		const bool joinedRoom  = joinedVoiceChannel && joinedVoiceChannel->iId == channel->iId;
		const QString streamID = screenShareStreamForChannel(channel);
		const bool hasShare    = !streamID.isEmpty();
		QVariantList overflowActions;

		const auto setPrimaryAction = [&shareState](const QString &id, const QString &actionLabel, const bool enabled,
													const QString &hint, const QString &tone = QString()) {
			shareState.insert(QStringLiteral("primaryActionId"), id);
			shareState.insert(QStringLiteral("primaryLabel"), actionLabel);
			shareState.insert(QStringLiteral("primaryEnabled"), enabled);
			shareState.insert(QStringLiteral("primaryHint"), hint);
			shareState.insert(QStringLiteral("primaryTone"), tone);
		};

		if (!hasShare) {
			const QString unavailableReason = joinedRoom ? m_screenShareManager->localShareUnavailableReason()
														 : tr("Join this voice room before starting a share.");
			shareState.insert(QStringLiteral("statusLabel"),
							  joinedRoom ? tr("Ready to share") : tr("Join this room to share"));
			shareState.insert(QStringLiteral("statusTone"),
							  unavailableReason.isEmpty() ? QStringLiteral("success") : QStringLiteral("warning"));
			setPrimaryAction(QStringLiteral("screenShareStart"), tr("Share screen"), unavailableReason.isEmpty(),
							 unavailableReason,
							 unavailableReason.isEmpty() ? QStringLiteral("success") : QStringLiteral("warning"));
			return shareState;
		}

		const ScreenShareSession session = m_screenShareManager->sessions().value(streamID);
		const ClientUser *owner          = ClientUser::get(session.ownerSession);
		const QString ownerLabel = owner ? owner->qsName : tr("session %1").arg(QString::number(session.ownerSession));
		const bool selfOwned     = selfUser && session.ownerSession == selfUser->uiSession;
		const bool publishing    = selfOwned && m_screenShareManager->isPublishingSession(streamID);
		const bool viewing       = !selfOwned && m_screenShareManager->isViewingSession(streamID);
		const bool detachedWindowOpen = m_screenShareManager->hasDetachedWindow(streamID);
		const bool usingFallback      = m_screenShareManager->isUsingFallbackRuntime(streamID);

		shareState.insert(QStringLiteral("streamId"), streamID);
		shareState.insert(QStringLiteral("ownerLabel"), ownerLabel);
		shareState.insert(QStringLiteral("ownerSession"), static_cast< qulonglong >(session.ownerSession));
		shareState.insert(QStringLiteral("detachedWindowOpen"), detachedWindowOpen);
		shareState.insert(QStringLiteral("usingFallback"), usingFallback);
		shareState.insert(QStringLiteral("fallbackLabel"),
						  usingFallback ? tr("Using helper/browser fallback.") : QString());
		shareState.insert(QStringLiteral("badgeLabel"), tr("Live"));
		shareState.insert(QStringLiteral("badgeTone"),
						  selfOwned ? QStringLiteral("success") : QStringLiteral("accent"));

		if (selfOwned) {
			shareState.insert(QStringLiteral("mode"),
							  usingFallback ? QStringLiteral("fallback")
											: (publishing ? QStringLiteral("publishing") : QStringLiteral("error")));
			shareState.insert(QStringLiteral("statusLabel"), tr("You are sharing in this room"));
			shareState.insert(QStringLiteral("statusTone"),
							  usingFallback ? QStringLiteral("warning") : QStringLiteral("success"));
			setPrimaryAction(QStringLiteral("screenShareOpenWindow"), tr("Open share window"), detachedWindowOpen,
							 detachedWindowOpen ? QString() : tr("The share window is not available right now."),
							 usingFallback ? QStringLiteral("warning") : QStringLiteral("success"));
			overflowActions.push_back(ModernShellMenuSerializer::actionItem(
				QStringLiteral("screenShareStop"), tr("Stop sharing"), true, false, QStringLiteral("danger")));
		} else if (viewing) {
			shareState.insert(QStringLiteral("mode"),
							  usingFallback ? QStringLiteral("fallback") : QStringLiteral("viewing"));
			shareState.insert(QStringLiteral("statusLabel"), tr("Watching %1's share").arg(ownerLabel));
			shareState.insert(QStringLiteral("statusTone"),
							  usingFallback ? QStringLiteral("warning") : QStringLiteral("accent"));
			setPrimaryAction(QStringLiteral("screenShareOpenWindow"), tr("Open share window"), detachedWindowOpen,
							 detachedWindowOpen ? QString() : tr("The share window is not available right now."),
							 usingFallback ? QStringLiteral("warning") : QStringLiteral("accent"));
			overflowActions.push_back(ModernShellMenuSerializer::actionItem(QStringLiteral("screenShareStopWatching"),
																			tr("Stop watching"), true, false));
		} else {
			const bool canWatch = m_screenShareManager->canViewSession(streamID);
			const QString unavailableReason =
				canWatch ? QString()
						 : (joinedRoom ? tr("This share is not viewable on this client right now.")
									   : tr("Join this voice room before watching."));
			shareState.insert(QStringLiteral("mode"),
							  canWatch || !joinedRoom ? QStringLiteral("available") : QStringLiteral("error"));
			shareState.insert(QStringLiteral("statusLabel"), tr("%1 is sharing in this room").arg(ownerLabel));
			shareState.insert(QStringLiteral("statusTone"),
							  canWatch ? QStringLiteral("accent") : QStringLiteral("warning"));
			setPrimaryAction(QStringLiteral("screenShareWatch"), tr("Watch share"), canWatch, unavailableReason,
							 canWatch ? QStringLiteral("accent") : QStringLiteral("warning"));
		}

		shareState.insert(QStringLiteral("overflowActions"), ModernShellMenuSerializer::normalize(overflowActions));
		return shareState;
	};

	if (m_persistentChatChannelList) {
		for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
			const QListWidgetItem *item = m_persistentChatChannelList->item(i);
			if (!item) {
				continue;
			}

			const int scopeValue       = item->data(PersistentChatScopeRole).toInt();
			const unsigned int scopeID = item->data(PersistentChatScopeIDRole).toUInt();
			const QString roomLabel    = item->data(PersistentChatLabelRole).toString();
			const qulonglong unreadRaw = item->data(PersistentChatUnreadRole).toULongLong();
			const QString itemToolTip  = item->toolTip();
			const bool selectedScope =
				(target.serverLog && scopeValue == LocalServerLogScope)
				|| (target.directMessage && target.user && scopeValue == LocalDirectMessageScope
					&& target.user->uiSession == scopeID)
				|| (!target.serverLog && !target.directMessage && target.valid && target.scopeID == scopeID
					&& scopeValue == static_cast< int >(target.scope));

			const bool voiceRoomChat = scopeValue == static_cast< int >(MumbleProto::Channel);
			if (scopeValue == LocalServerLogScope || scopeValue == static_cast< int >(MumbleProto::TextChannel)
				|| voiceRoomChat || scopeValue == static_cast< int >(MumbleProto::ServerGlobal)
				|| scopeValue == LocalDirectMessageScope || scopeValue == static_cast< int >(MumbleProto::Aggregate)) {
				QVariantMap room;
				room.insert(QStringLiteral("token"), modernShellScopeToken(scopeValue, scopeID));
				room.insert(QStringLiteral("label"), roomLabel.isEmpty() ? tr("Room") : roomLabel);
				room.insert(QStringLiteral("description"),
							itemToolTip.isEmpty()
								? (voiceRoomChat ? tr("Voice room chat") : tr("Conversation"))
								: itemToolTip);
				room.insert(QStringLiteral("depth"), 0);
				room.insert(QStringLiteral("selected"), selectedScope);
				room.insert(QStringLiteral("joined"),
							voiceRoomChat && joinedVoiceChannel && joinedVoiceChannel->iId == scopeID);
				room.insert(QStringLiteral("unreadCount"), static_cast< qulonglong >(unreadRaw));
				room.insert(QStringLiteral("kindLabel"),
							scopeValue == LocalServerLogScope
								? tr("Activity")
								: (scopeValue == LocalDirectMessageScope
									   ? tr("Direct message")
									   : (voiceRoomChat
											  ? tr("Voice room")
											  : (scopeValue == static_cast< int >(MumbleProto::TextChannel)
													 ? tr("Text room")
													 : tr("Legacy")))));
				Channel *roomChannel = nullptr;
				if (scopeValue == static_cast< int >(MumbleProto::TextChannel)) {
					const auto textChannelIt = m_persistentTextChannels.constFind(scopeID);
					if (textChannelIt != m_persistentTextChannels.cend()) {
						roomChannel = Channel::get(textChannelIt->aclChannelID);
					}
				} else if (voiceRoomChat) {
					roomChannel = Channel::get(scopeID);
				}
				if (roomChannel) {
					room.insert(QStringLiteral("actions"), buildChannelActions(roomChannel, false));
				}
				if (scopeValue == LocalDirectMessageScope) {
					if (ClientUser *roomUser = ClientUser::get(scopeID)) {
						room.insert(QStringLiteral("participantSession"),
									static_cast< qulonglong >(roomUser->uiSession));
						room.insert(QStringLiteral("participantActions"), buildParticipantActions(roomUser));
					}
				}
				textRooms.push_back(room);
			}
		}
	}

	std::function< void(const Channel *, int) > appendVoiceRooms = [&](const Channel *parent, int depth) {
		if (!parent) {
			return;
		}

		for (const Channel *channel : parent->qlChannels) {
			if (!channel) {
				continue;
			}

			QVariantMap room;
			room.insert(QStringLiteral("token"),
						modernShellScopeToken(static_cast< int >(MumbleProto::Channel), channel->iId));
			room.insert(QStringLiteral("label"), channel->qsName);
			room.insert(
				QStringLiteral("description"),
				tr("%1 people in %2").arg(channel->qlUsers.count()).arg(persistentTextAclChannelLabel(channel)));
			room.insert(QStringLiteral("pathLabel"), persistentTextAclChannelLabel(channel));
			room.insert(QStringLiteral("depth"), depth);
			room.insert(QStringLiteral("memberCount"), channel->qlUsers.count());
			room.insert(QStringLiteral("participants"), buildChannelParticipants(channel, 32));
			room.insert(QStringLiteral("selected"),
						target.valid && target.scope == MumbleProto::Channel && target.scopeID == channel->iId);
			room.insert(QStringLiteral("joined"), joinedVoiceChannel && joinedVoiceChannel->iId == channel->iId);
			room.insert(QStringLiteral("unreadCount"),
						static_cast< qulonglong >(cachedPersistentChatUnreadCount(MumbleProto::Channel, channel->iId)));
			room.insert(QStringLiteral("kindLabel"), tr("Voice room"));
			room.insert(QStringLiteral("actions"), buildChannelActions(const_cast< Channel * >(channel), true));
			room.insert(QStringLiteral("screenShare"), buildVoiceRoomScreenShareState(channel));
			voiceRooms.push_back(room);

			appendVoiceRooms(channel, depth + 1);
		}
	};
	appendVoiceRooms(Channel::get(Mumble::ROOT_CHANNEL_ID), 0);

	QString kindLabel = tr("Conversation");
	if (target.serverLog) {
		kindLabel = tr("Activity");
	} else if (target.directMessage) {
		kindLabel = tr("Direct message");
	} else if (target.scope == MumbleProto::TextChannel) {
		kindLabel = tr("Text room");
	} else if (target.scope == MumbleProto::Channel) {
		kindLabel = tr("Voice room");
	} else if (target.scope == MumbleProto::Aggregate || target.scope == MumbleProto::ServerGlobal) {
		kindLabel = tr("Legacy");
	}

	QString scopeDescription = target.description.trimmed();
	if (scopeDescription.isEmpty()) {
		if (target.serverLog) {
			scopeDescription = tr("Server log, connection status, notices, and diagnostics.");
		} else if (!connected) {
			scopeDescription = tr("Connect to a server to load rooms and history.");
		} else if (target.directMessage) {
			scopeDescription = tr("Direct messages still use the classic text-message path.");
		} else if (target.scope == MumbleProto::TextChannel) {
			scopeDescription = tr("Persistent history for this text room.");
		} else if (target.scope == MumbleProto::Channel) {
			scopeDescription = tr("Persistent history for this voice room.");
		}
	}

	QString composerPlaceholder = tr("Write a message");
	if (!connected || !target.valid) {
		composerPlaceholder = tr("Connect to chat");
	} else if (target.serverLog) {
		composerPlaceholder = tr("Read-only activity");
	} else if (target.directMessage && target.user) {
		composerPlaceholder = tr("Write to %1...").arg(target.user->qsName);
	} else if (target.scope == MumbleProto::TextChannel || target.scope == MumbleProto::Channel) {
		composerPlaceholder = tr("Write in %1...").arg(target.label);
	}

	QString composerHint = tr("Persistent room history stays with the selected room.");
	if (m_pendingPersistentChatReply) {
		composerHint = tr("Replying to %1").arg(persistentChatActorLabel(*m_pendingPersistentChatReply));
	} else if (target.directMessage) {
		composerHint = tr("Direct messages still use the classic text-message transport.");
	} else if (target.readOnly) {
		composerHint = tr("Choose another room to reply.");
	}

	QVariantList scopeMeta;
	if (connected && selfVoiceChannel) {
		scopeMeta.push_back(tr("In %1").arg(selfVoiceChannel->qsName));
	}
	if (target.valid && !target.serverLog && !m_persistentChatMessages.empty()) {
		scopeMeta.push_back(tr("%1 loaded messages").arg(static_cast< qulonglong >(m_persistentChatMessages.size())));
	}
	const std::size_t unreadCount = persistentChatUnreadCount();
	if (unreadCount > 0 && target.valid && !target.serverLog) {
		scopeMeta.push_back(tr("%1 unread").arg(static_cast< qulonglong >(unreadCount)));
	}
	if (target.readOnly) {
		scopeMeta.push_back(tr("Read-only"));
	}

	const int activeScopeValue =
		target.serverLog ? LocalServerLogScope
						 : (target.directMessage ? LocalDirectMessageScope : static_cast< int >(target.scope));
	const unsigned int activeScopeID = target.directMessage && target.user ? target.user->uiSession : target.scopeID;
	activeScope.insert(QStringLiteral("kindLabel"), kindLabel);
	activeScope.insert(QStringLiteral("scopeToken"), modernShellScopeToken(activeScopeValue, activeScopeID));
	activeScope.insert(QStringLiteral("label"), target.label.isEmpty() ? tr("No room selected") : target.label);
	activeScope.insert(QStringLiteral("description"), scopeDescription);
	activeScope.insert(QStringLiteral("banner"), target.statusMessage);
	activeScope.insert(QStringLiteral("meta"), scopeMeta);
	activeScope.insert(QStringLiteral("canSend"), connected && target.valid && !target.readOnly && !target.serverLog
													  && (!qteChat || qteChat->isEnabled()));
	activeScope.insert(QStringLiteral("canLoadOlder"), connected && target.valid && !target.serverLog
														   && !target.directMessage && !target.legacyTextPath
														   && m_visiblePersistentChatHasMore
														   && !m_persistentChatLoadingOlder && m_persistentChatGateway);
	activeScope.insert(QStringLiteral("canMarkRead"), canMarkPersistentChatRead(false));
	activeScope.insert(QStringLiteral("canReply"), connected && target.valid && !target.readOnly && !target.serverLog
													   && !target.directMessage && !target.legacyTextPath
													   && (!qteChat || qteChat->isEnabled()));
	activeScope.insert(QStringLiteral("canReact"), connected && target.valid && !target.readOnly && !target.serverLog
													   && !target.directMessage && !target.legacyTextPath
													   && canUsePersistedReactions);
	activeScope.insert(QStringLiteral("canDeleteMessages"), canDeletePersistentChatMessages(target, true));
	activeScope.insert(QStringLiteral("composerPlaceholder"), composerPlaceholder);
	activeScope.insert(QStringLiteral("composerHint"), composerHint);
	activeScope.insert(QStringLiteral("canAttachImages"), connected && target.valid && !target.readOnly
															  && !target.serverLog && Global::get().bAllowHTML
															  && (!qteChat || qteChat->isEnabled()));
	activeScope.insert(QStringLiteral("emptyCopy"),
					   target.readOnly ? tr("This conversation is read-only.")
									   : tr("Messages will appear here once the selected room has activity."));
	activeScope.insert(QStringLiteral("scrollToBottom"),
					   !m_persistentChatHistory || m_persistentChatHistory->isScrolledToBottom());
	activeScope.insert(QStringLiteral("autoMarkRead"), false);
	activeScope.insert(QStringLiteral("hasPendingReply"), m_pendingPersistentChatReply.has_value());
	if (m_pendingPersistentChatReply) {
		activeScope.insert(QStringLiteral("replyMessageId"),
						   static_cast< qulonglong >(m_pendingPersistentChatReply->message_id()));
		activeScope.insert(QStringLiteral("replyActor"), persistentChatActorLabel(*m_pendingPersistentChatReply));
		activeScope.insert(
			QStringLiteral("replySnippet"),
			persistentChatMessageTextSnippet(persistentChatMessageSourceText(*m_pendingPersistentChatReply)));
	}
	if ((target.serverLog || target.legacyTextPath) && qteLog && qteLog->document()) {
		activeScope.insert(QStringLiteral("serverLogHtml"), qteLog->document()->toHtml());
	}
	if (target.scope == MumbleProto::Channel && target.channel) {
		activeScope.insert(QStringLiteral("screenShare"), buildVoiceRoomScreenShareState(target.channel));
	} else {
		QVariantMap hiddenScreenShare;
		hiddenScreenShare.insert(QStringLiteral("visible"), false);
		hiddenScreenShare.insert(QStringLiteral("available"), false);
		hiddenScreenShare.insert(QStringLiteral("mode"), QStringLiteral("idle"));
		hiddenScreenShare.insert(QStringLiteral("overflowActions"), QVariantList());
		activeScope.insert(QStringLiteral("screenShare"), hiddenScreenShare);
	}
	snapshot.insert(QStringLiteral("textRooms"), textRooms);
	snapshot.insert(QStringLiteral("voiceRooms"), voiceRooms);
	snapshot.insert(QStringLiteral("activeScope"), activeScope);

	QVariantList participants;
	const Channel *participantChannel = nullptr;
	if (target.directMessage && target.user) {
		if (selfUser) {
			participants.push_back(buildParticipantState(selfUser, nullptr, target.user, 40));
		}
		if (target.user != selfUser) {
			participants.push_back(buildParticipantState(target.user, nullptr, target.user, 40));
		}
	} else if (target.channel) {
		participantChannel = target.channel;
	} else if (selfVoiceChannel) {
		participantChannel = selfVoiceChannel;
	}

	if (participantChannel) {
		participants = buildChannelParticipants(participantChannel, 40);
	}

	snapshot.insert(QStringLiteral("participants"), participants);

	QVariantList voicePresence;
	if (selfVoiceChannel) {
		voicePresence = buildChannelParticipants(selfVoiceChannel, 36);
	}
	snapshot.insert(QStringLiteral("voicePresenceChannelId"),
					selfVoiceChannel ? static_cast< qulonglong >(selfVoiceChannel->iId) : 0);
	snapshot.insert(QStringLiteral("voicePresence"), voicePresence);

	if (!target.serverLog && !target.directMessage) {
		const ClientUser *self         = ClientUser::get(Global::get().uiSession);
		const std::size_t messageCount = m_persistentChatMessages.size();
		const std::size_t beginIndex   = messageCount > 200 ? messageCount - 200 : 0;
		for (std::size_t i = beginIndex; i < messageCount; ++i) {
			const MumbleProto::ChatMessage &message   = m_persistentChatMessages[i];
			const std::optional< QString > systemText = persistentChatSystemMessageText(message);
			const bool systemMessage                  = systemText.has_value();
			const bool deletedMessage                 = message.has_deleted_at() && message.deleted_at() > 0;
			const bool ownMessage =
				self
				&& ((message.has_actor() && message.actor() == Global::get().uiSession)
					|| (message.has_actor_user_id() && self->iId >= 0
						&& message.actor_user_id() == static_cast< decltype(message.actor_user_id()) >(self->iId)));
			const ClientUser *messageUser =
				ownMessage ? self : (message.has_actor() ? ClientUser::get(message.actor()) : nullptr);

			QString bodyText =
				systemMessage ? *systemText : (deletedMessage ? QString() : persistentChatMessageSourceText(message));
			QString bodyHtml;
			if (systemMessage) {
				bodyHtml = systemText->toHtmlEscaped();
			} else if (deletedMessage) {
				bodyHtml = QString::fromLatin1("<em>%1</em>").arg(tr("[message deleted]").toHtmlEscaped());
			} else {
				const auto buildModernInlineDataImageReplacement = [this](
																	   const QString &source, const QString &altText,
																	   const PersistentChatInlineDataImageInfo &info) {
					const QString token    = registerPersistentChatInlineDataImageSource(source);
					const QString openHref = persistentChatInlineDataImageOpenUrl(token).toString(QUrl::FullyEncoded);
					const QImage previewImage = persistentChatInlineDataImagePreviewImage(source, info);
					if (previewImage.isNull()) {
						mumble::chatperf::recordValue("chat.inline_data_image.modern_preview_failed", 1);
						return persistentChatInlineDataImagePlaceholderHtml(info, altText, openHref);
					}

					const QString thumbnailSource = persistentChatInlineDataImageThumbnailSource(previewImage);
					if (thumbnailSource.isEmpty()) {
						mumble::chatperf::recordValue("chat.inline_data_image.modern_thumbnail_failed", 1);
						return persistentChatInlineDataImagePlaceholderHtml(info, altText, openHref);
					}

					mumble::chatperf::recordValue("chat.inline_data_image.modern_preview_ready", 1);
					return persistentChatInlineDataImageThumbnailHtml(thumbnailSource, openHref, altText,
																	  previewImage.size(), info.estimatedBytes);
				};
				bodyHtml = message.has_body_text()
							   ? persistentChatMessageBodyHtml(message, buildModernInlineDataImageReplacement)
							   : persistentChatContentHtml(persistentChatMessageRawBody(message),
														   buildModernInlineDataImageReplacement);
				bodyHtml = persistentChatCondensedBodyHtml(bodyHtml, bodyText);
			}
			if (!deletedMessage && message.has_edited_at() && message.edited_at() > 0) {
				bodyHtml += QString::fromLatin1(" <em>%1</em>").arg(tr("(edited)").toHtmlEscaped());
			}
			const std::optional< PersistentChatReplyReference > replyReference =
				(systemMessage || deletedMessage)
					? std::nullopt
					: (message.has_body_text() ? resolvedPersistentChatReplyReference(m_persistentChatMessages, message)
											   : extractPersistentChatReplyReference(bodyHtml, &bodyHtml));

			QVariantMap messageState;
			messageState.insert(QStringLiteral("messageId"), static_cast< qulonglong >(message.message_id()));
			messageState.insert(QStringLiteral("threadId"), static_cast< qulonglong >(message.thread_id()));
			messageState.insert(QStringLiteral("createdAtMs"), static_cast< qulonglong >(message.created_at()));
			messageState.insert(QStringLiteral("actor"),
								systemMessage ? tr("System") : persistentChatActorLabel(message));
			messageState.insert(QStringLiteral("avatarUrl"), modernShellAvatarDataUrl(messageUser, 40));
			messageState.insert(
				QStringLiteral("timeLabel"),
				persistentChatTimestampLabel(
					QDateTime::fromMSecsSinceEpoch(static_cast< qint64 >(message.created_at())).toLocalTime()));
			messageState.insert(QStringLiteral("scopeLabel"),
								target.scope == MumbleProto::Aggregate ? persistentChatScopeLabel(
									message.has_scope() ? message.scope() : MumbleProto::Channel,
									message.has_scope_id() ? message.scope_id() : 0)
																	   : QString());
			messageState.insert(QStringLiteral("bodyText"), bodyText);
			messageState.insert(QStringLiteral("bodyHtml"), bodyHtml);
			messageState.insert(QStringLiteral("own"), ownMessage);
			messageState.insert(QStringLiteral("system"), systemMessage);
			messageState.insert(QStringLiteral("deleted"), deletedMessage);
			messageState.insert(QStringLiteral("canReply"), activeScope.value(QStringLiteral("canReply")).toBool()
																&& !systemMessage && !deletedMessage);
			messageState.insert(QStringLiteral("canReact"), activeScope.value(QStringLiteral("canReact")).toBool()
																&& !systemMessage && !deletedMessage);
			messageState.insert(QStringLiteral("canDelete"),
								activeScope.value(QStringLiteral("canDeleteMessages")).toBool() && !systemMessage
									&& !deletedMessage);
			if (replyReference) {
				messageState.insert(QStringLiteral("replyMessageId"),
									static_cast< qulonglong >(replyReference->messageID));
				messageState.insert(QStringLiteral("replyActor"), replyReference->actor);
				messageState.insert(QStringLiteral("replySnippet"), replyReference->snippet);
			}
			QVariantList reactions;
			if (!deletedMessage) {
				for (int reactionIndex = 0; reactionIndex < message.reactions_size(); ++reactionIndex) {
					const MumbleProto::ChatReactionAggregate &reaction = message.reactions(reactionIndex);
					QVariantMap reactionState;
					reactionState.insert(QStringLiteral("emoji"), u8(reaction.emoji()));
					reactionState.insert(QStringLiteral("count"), static_cast< qulonglong >(reaction.count()));
					reactionState.insert(QStringLiteral("selfReacted"), reaction.self_reacted());
					reactions.push_back(reactionState);
				}
			}
			messageState.insert(QStringLiteral("reactions"), reactions);
			if (!deletedMessage) {
				if (const std::optional< QString > previewKey = persistentChatPreviewKey(message); previewKey) {
					ensurePersistentChatPreview(*previewKey);
					if (const auto it = m_persistentChatPreviews.constFind(*previewKey);
						it != m_persistentChatPreviews.cend()) {
						const PersistentChatPreview &preview = it.value();
						QVariantMap previewState;
						previewState.insert(QStringLiteral("kind"), previewKey->startsWith(QLatin1String("image:"))
																		? QStringLiteral("image")
																		: QStringLiteral("link"));
						previewState.insert(QStringLiteral("url"), preview.canonicalUrl);
						previewState.insert(QStringLiteral("title"), preview.title);
						previewState.insert(QStringLiteral("subtitle"), preview.subtitle);
						previewState.insert(QStringLiteral("description"), preview.description);
						previewState.insert(QStringLiteral("openLabel"), preview.openLabel);
						previewState.insert(QStringLiteral("loading"),
											!preview.metadataFinished || !preview.thumbnailFinished);
						previewState.insert(QStringLiteral("failed"), preview.failed);
						previewState.insert(QStringLiteral("thumbnailUrl"),
											persistentChatInlineDataImageThumbnailSource(preview.thumbnailImage));
						messageState.insert(QStringLiteral("preview"), previewState);
					}
				}
			}
			messages.push_back(messageState);
		}
	}

	snapshot.insert(QStringLiteral("messages"), messages);
	return snapshot;
}

bool MainWindow::handleModernShellScopeSelection(const QString &scopeToken) {
	int scopeValue       = 0;
	unsigned int scopeID = 0;
	if (!parseModernShellScopeToken(scopeToken, scopeValue, scopeID)) {
		return false;
	}

	if (scopeValue == LocalServerLogScope && m_persistentChatChannelList) {
		for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
			QListWidgetItem *item = m_persistentChatChannelList->item(i);
			if (!item || item->data(PersistentChatScopeRole).toInt() != LocalServerLogScope) {
				continue;
			}

			setPersistentChatTargetUsesVoiceTree(false);
			cachePersistentChatChannelSelection(item);
			m_persistentChatChannelList->setCurrentItem(item);
			return true;
		}

		return false;
	}

	if (scopeValue == LocalDirectMessageScope && m_persistentChatChannelList) {
		if (QListWidgetItem *item = findPersistentChatChannelItem(LocalDirectMessageScope, scopeID)) {
			setPersistentChatTargetUsesVoiceTree(false);
			cachePersistentChatChannelSelection(item);
			m_persistentChatChannelList->setCurrentItem(item);
			return true;
		}

		return false;
	}

	if (scopeValue == static_cast< int >(MumbleProto::Channel)) {
		Channel *channel = Channel::get(scopeID);
		if (!channel) {
			return false;
		}
		focusPersistentChatVoiceChannel(channel);
		return true;
	}

	return navigateToPersistentChatScope(static_cast< MumbleProto::ChatScope >(scopeValue), scopeID);
}

bool MainWindow::handleModernShellVoiceJoin(const QString &scopeToken) {
	int scopeValue       = 0;
	unsigned int scopeID = 0;
	if (!parseModernShellScopeToken(scopeToken, scopeValue, scopeID)
		|| scopeValue != static_cast< int >(MumbleProto::Channel)) {
		return false;
	}

	Channel *channel = Channel::get(scopeID);
	if (!channel) {
		return false;
	}

	if (qtvUsers && pmModel) {
		const QModelIndex channelIndex = pmModel->index(channel);
		if (channelIndex.isValid()) {
			qtvUsers->setCurrentIndex(channelIndex);
			qtvUsers->scrollTo(channelIndex);
		}
	}

	if (Global::get().sh && Global::get().uiSession != 0) {
		Global::get().sh->joinChannel(Global::get().uiSession, channel->iId);
	}

	focusPersistentChatVoiceChannel(channel);
	return true;
}

bool MainWindow::handleModernShellScopeAction(const QString &scopeToken, const QString &actionId) {
	int scopeValue       = 0;
	unsigned int scopeID = 0;
	if (!parseModernShellScopeToken(scopeToken, scopeValue, scopeID)) {
		return false;
	}

	Channel *channel = nullptr;
	if (scopeValue == static_cast< int >(MumbleProto::Channel)) {
		channel = Channel::get(scopeID);
	} else if (scopeValue == static_cast< int >(MumbleProto::TextChannel)) {
		const auto it = m_persistentTextChannels.constFind(scopeID);
		if (it != m_persistentTextChannels.cend()) {
			channel = Channel::get(it->aclChannelID);
		}
	}

	if (!channel) {
		return false;
	}

	const QString normalizedActionID = actionId.trimmed();
	if (normalizedActionID.isEmpty()) {
		return false;
	}
	if (scopeValue != static_cast< int >(MumbleProto::Channel)
		&& (normalizedActionID == QLatin1String("join") || normalizedActionID == QLatin1String("listen"))) {
		return false;
	}
	if (normalizedActionID == QLatin1String("screenShareStart")) {
		if (!m_screenShareManager) {
			return false;
		}
		m_screenShareManager->requestStartChannelShare(channel->iId);
		queueModernShellSnapshotSync();
		return true;
	}
	if (normalizedActionID == QLatin1String("screenShareStop")
		|| normalizedActionID == QLatin1String("screenShareWatch")
		|| normalizedActionID == QLatin1String("screenShareStopWatching")
		|| normalizedActionID == QLatin1String("screenShareOpenWindow")) {
		const QString streamID = screenShareStreamForChannel(channel);
		bool handled           = false;
		if (!streamID.isEmpty() && m_screenShareManager) {
			if (normalizedActionID == QLatin1String("screenShareStop")) {
				m_screenShareManager->requestStopShare(streamID);
				handled = true;
			} else if (normalizedActionID == QLatin1String("screenShareWatch")) {
				m_screenShareManager->requestStartViewing(streamID);
				handled = true;
			} else if (normalizedActionID == QLatin1String("screenShareStopWatching")) {
				m_screenShareManager->requestStopViewing(streamID);
				handled = true;
			} else {
				handled = m_screenShareManager->focusOrReopenDetachedWindow(streamID);
			}
		}
		if (handled) {
			queueModernShellSnapshotSync();
		}
		return handled;
	}

	const QPointer< ClientUser > previousUser = cuContextUser;
	const QPointer< Channel > previousChannel = cContextChannel;
	const QPoint previousContextPosition      = qpContextPosition;

	cuContextUser     = nullptr;
	cContextChannel   = channel;
	qpContextPosition = QPoint();
	qmChannel_aboutToShow();
	ModernShellMenuSerializer::ActionRegistry registry;
	serializeModernShellMenu(qmChannel, ModernShellMenuContext::Scope, &registry);
	const bool handled = triggerModernShellSerializedAction(registry, normalizedActionID, nullptr, channel);

	cuContextUser     = previousUser;
	cContextChannel   = previousChannel;
	qpContextPosition = previousContextPosition;

	const PersistentChatTarget restoredTarget = currentPersistentChatTarget();
	syncPersistentChatInputState(restoredTarget.valid && !restoredTarget.readOnly && !restoredTarget.serverLog);
	if (handled) {
		queueModernShellSnapshotSync();
	}

	return handled;
}

bool MainWindow::handleModernShellScopeActionValueChanged(const QString &scopeToken, const QString &actionId,
														  const int value, const bool final) {
	if (actionId.trimmed() != QLatin1String("listenerVolume")) {
		return false;
	}

	int scopeValue       = 0;
	unsigned int scopeID = 0;
	if (!parseModernShellScopeToken(scopeToken, scopeValue, scopeID)) {
		return false;
	}

	Channel *channel = nullptr;
	if (scopeValue == static_cast< int >(MumbleProto::Channel)) {
		channel = Channel::get(scopeID);
	} else if (scopeValue == static_cast< int >(MumbleProto::TextChannel)) {
		const auto it = m_persistentTextChannels.constFind(scopeID);
		if (it != m_persistentTextChannels.cend()) {
			channel = Channel::get(it->aclChannelID);
		}
	}

	if (!channel || !m_listenerVolumeController) {
		return false;
	}

	m_listenerVolumeController->setListenedChannel(channel);
	m_listenerVolumeController->applyDbAdjustment(value, final);
	if (final) {
		queueModernShellSnapshotSync();
	}
	return true;
}

bool MainWindow::handleModernShellReplyStart(const qulonglong messageID) {
	if (messageID == 0) {
		return false;
	}

	if (const MumbleProto::ChatMessage *message =
			findPersistentChatMessageByID(m_persistentChatMessages, static_cast< unsigned int >(messageID))) {
		setPersistentChatReplyTarget(*message);
		return true;
	}

	return false;
}

void MainWindow::handleModernShellReplyCancel() {
	setPersistentChatReplyTarget(std::nullopt);
}

bool MainWindow::handleModernShellReactionToggle(const qulonglong messageID, const QString &emoji, const bool active) {
	if (!m_persistentChatGateway || messageID == 0 || emoji.trimmed().isEmpty()) {
		return false;
	}

	const PersistentChatTarget target = currentPersistentChatTarget();
	if (!target.valid || target.readOnly || target.serverLog || target.directMessage || target.legacyTextPath) {
		return false;
	}

	if (const MumbleProto::ChatMessage *message =
			findPersistentChatMessageByID(m_persistentChatMessages, static_cast< unsigned int >(messageID))) {
		syncPersistentChatGatewayHandler();
		m_persistentChatGateway->toggleReaction(target.scope, target.scopeID, message->thread_id(),
												static_cast< unsigned int >(messageID), emoji, active);
		return true;
	}

	return false;
}

bool MainWindow::handleModernShellMessageDelete(const qulonglong messageID) {
	if (messageID == 0 || messageID > std::numeric_limits< unsigned int >::max()) {
		return false;
	}

	return deletePersistentChatMessage(static_cast< unsigned int >(messageID));
}

bool MainWindow::handleModernShellParticipantMessage(const qulonglong session) {
	if (!m_persistentChatChannelList) {
		return false;
	}

	if (session == 0 || session > std::numeric_limits< unsigned int >::max() || session == Global::get().uiSession) {
		return false;
	}

	const unsigned int targetSession = static_cast< unsigned int >(session);
	if (!ClientUser::get(targetSession)) {
		return false;
	}

	if (QListWidgetItem *item = findPersistentChatChannelItem(LocalDirectMessageScope, targetSession)) {
		setPersistentChatTargetUsesVoiceTree(false);
		cachePersistentChatChannelSelection(item);
		m_persistentChatChannelList->setCurrentItem(item);
		return true;
	}

	m_persistentChatSelectedScopeValue = LocalDirectMessageScope;
	m_persistentChatSelectedScopeID    = targetSession;
	rebuildPersistentChatChannelList();
	if (QListWidgetItem *item = findPersistentChatChannelItem(LocalDirectMessageScope, targetSession)) {
		setPersistentChatTargetUsesVoiceTree(false);
		cachePersistentChatChannelSelection(item);
		m_persistentChatChannelList->setCurrentItem(item);
		return true;
	}

	return false;
}

bool MainWindow::handleModernShellParticipantJoin(const qulonglong session) {
	ClientUser *participant = ClientUser::get(static_cast< unsigned int >(session));
	ClientUser *self        = ClientUser::get(Global::get().uiSession);
	if (!participant || !participant->cChannel || !self || !self->cChannel || !Global::get().sh) {
		return false;
	}
	if (participant == self || participant->cChannel->iId == self->cChannel->iId) {
		return false;
	}

	focusPersistentChatVoiceChannel(participant->cChannel);
	Global::get().sh->joinChannel(Global::get().uiSession, participant->cChannel->iId);
	return true;
}

bool MainWindow::handleModernShellParticipantMove(const qulonglong session, const QString &targetScopeToken) {
	int scopeValue       = 0;
	unsigned int scopeID = 0;
	if (!parseModernShellScopeToken(targetScopeToken, scopeValue, scopeID)
		|| scopeValue != static_cast< int >(MumbleProto::Channel)) {
		return false;
	}

	Channel *targetChannel         = Channel::get(scopeID);
	ClientUser *participant        = ClientUser::get(static_cast< unsigned int >(session));
	ClientUser *self               = ClientUser::get(Global::get().uiSession);
	ServerHandlerPtr serverHandler = Global::get().sh;
	if (!targetChannel || !participant || !serverHandler) {
		return false;
	}

	if (participant->cChannel && participant->cChannel->iId == targetChannel->iId) {
		return true;
	}

	// Respect the same drag preference the original tree view uses.
	switch (Global::get().s.ceUserDrag) {
		case Settings::Ask:
			if (QMessageBox::question(this, QLatin1String("Mumble"), tr("Are you sure you want to drag this user?"),
									  QMessageBox::Yes, QMessageBox::No)
				== QMessageBox::No) {
				return false;
			}
			break;
		case Settings::DoNothing:
			Global::get().l->log(Log::Information,
								 tr("You have User Dragging set to \"Do Nothing\" so the user wasn't moved."));
			return false;
		case Settings::Move:
			break;
	}

	if (participant == self && Global::get().uiSession != 0) {
		focusPersistentChatVoiceChannel(targetChannel);
		serverHandler->joinChannel(Global::get().uiSession, targetChannel->iId);
		return true;
	}

	MumbleProto::UserState userState;
	userState.set_session(static_cast< unsigned int >(session));
	userState.set_channel_id(targetChannel->iId);
	serverHandler->sendMessage(userState);
	queueModernShellSnapshotSync();
	return true;
}

bool MainWindow::handleModernShellChannelMove(const QString &sourceScopeToken, const QString &targetScopeToken,
											  const QString &placement) {
	int sourceScopeValue       = 0;
	unsigned int sourceScopeID = 0;
	int targetScopeValue       = 0;
	unsigned int targetScopeID = 0;
	if (!parseModernShellScopeToken(sourceScopeToken, sourceScopeValue, sourceScopeID)
		|| !parseModernShellScopeToken(targetScopeToken, targetScopeValue, targetScopeID)
		|| sourceScopeValue != static_cast< int >(MumbleProto::Channel)
		|| targetScopeValue != static_cast< int >(MumbleProto::Channel)) {
		return false;
	}

	Channel *sourceChannel         = Channel::get(sourceScopeID);
	Channel *targetChannel         = Channel::get(targetScopeID);
	ServerHandlerPtr serverHandler = Global::get().sh;
	if (!sourceChannel || !targetChannel || !serverHandler || sourceChannel == targetChannel) {
		return false;
	}
	if (sourceChannel->iId == Mumble::ROOT_CHANNEL_ID) {
		return false;
	}

	QString normalizedPlacement = placement.trimmed().toLower();
	if (normalizedPlacement != QLatin1String("before") && normalizedPlacement != QLatin1String("after")
		&& normalizedPlacement != QLatin1String("inside")) {
		normalizedPlacement = QStringLiteral("inside");
	}
	if (targetChannel->iId == Mumble::ROOT_CHANNEL_ID) {
		normalizedPlacement = QStringLiteral("inside");
	}

	Channel *targetParent = nullptr;
	if (normalizedPlacement == QLatin1String("inside")) {
		targetParent = targetChannel;
	} else {
		targetParent = targetChannel->cParent ? targetChannel->cParent : Channel::get(Mumble::ROOT_CHANNEL_ID);
	}
	if (!targetParent || targetParent == sourceChannel) {
		return false;
	}

	for (Channel *ancestor = targetParent; ancestor; ancestor = ancestor->cParent) {
		if (ancestor == sourceChannel) {
			return false;
		}
	}

	switch (Global::get().s.ceChannelDrag) {
		case Settings::Ask:
			if (QMessageBox::question(this, QLatin1String("Mumble"), tr("Are you sure you want to drag this channel?"),
									  QMessageBox::Yes, QMessageBox::No)
				== QMessageBox::No) {
				return false;
			}
			break;
		case Settings::DoNothing:
			Global::get().l->log(Log::Information,
								 tr("You have Channel Dragging set to \"Do Nothing\" so the channel wasn't moved."));
			return false;
		case Settings::Move:
			break;
	}

	const auto showPositionError = [this]() {
		QMessageBox::critical(this, QLatin1String("Mumble"),
							  tr("Cannot perform this movement automatically, please reset the numeric sorting "
								 "indicators or adjust it manually."));
	};
	const auto sendPositionAdjustment = [serverHandler](Channel *channel, const int position) {
		if (!channel) {
			return;
		}

		MumbleProto::ChannelState adjustedState;
		adjustedState.set_channel_id(channel->iId);
		adjustedState.set_position(position);
		serverHandler->sendMessage(adjustedState);
	};
	const auto sortedChildren = [sourceChannel](Channel *parent) {
		QList< Channel * > children;
		if (!parent) {
			return children;
		}

		for (Channel *child : parent->qlChannels) {
			if (child && child != sourceChannel) {
				children.push_back(child);
			}
		}
		std::sort(children.begin(), children.end(), Channel::lessThan);
		return children;
	};

	long long newPosition = 0;
	if (normalizedPlacement == QLatin1String("inside")) {
		bool hasSiblings = false;
		for (Channel *child : sortedChildren(targetParent)) {
			if (!child) {
				continue;
			}

			hasSiblings = true;
			newPosition = std::max(newPosition, static_cast< long long >(child->iPosition) + 20);
		}
		if (!hasSiblings) {
			newPosition = 0;
		}
	} else {
		QList< Channel * > siblings = sortedChildren(targetParent);
		const auto targetIt         = std::find(siblings.begin(), siblings.end(), targetChannel);
		if (targetIt == siblings.end()) {
			return false;
		}

		const int targetIndex = static_cast< int >(std::distance(siblings.begin(), targetIt));
		const int insertIndex =
			normalizedPlacement == QLatin1String("before") ? targetIndex : targetIndex + 1;
		Channel *previous = insertIndex > 0 ? siblings.at(insertIndex - 1) : nullptr;
		Channel *next     = insertIndex < siblings.size() ? siblings.at(insertIndex) : nullptr;

		if (!previous && !next) {
			newPosition = 0;
		} else if (!previous) {
			if (static_cast< long long >(next->iPosition) - 20 >= INT_MIN) {
				newPosition = static_cast< long long >(next->iPosition) - 20;
			} else {
				if (siblings.isEmpty()
					|| static_cast< long long >(siblings.last()->iPosition) + 40 > INT_MAX) {
					showPositionError();
					return false;
				}
				for (Channel *child : siblings) {
					sendPositionAdjustment(child, child->iPosition + 40);
				}
				newPosition = static_cast< long long >(next->iPosition) + 20;
			}
		} else if (!next) {
			if (static_cast< long long >(previous->iPosition) + 20 <= INT_MAX) {
				newPosition = static_cast< long long >(previous->iPosition) + 20;
			} else {
				if (siblings.isEmpty()
					|| static_cast< long long >(siblings.first()->iPosition) - 40 < INT_MIN) {
					showPositionError();
					return false;
				}
				for (Channel *child : siblings) {
					sendPositionAdjustment(child, child->iPosition - 40);
				}
				newPosition = static_cast< long long >(previous->iPosition) - 20;
			}
		} else {
			const long long gap = static_cast< long long >(next->iPosition) - previous->iPosition;
			if (gap > 1) {
				newPosition = static_cast< long long >(previous->iPosition) + (gap / 2);
			} else {
				if (siblings.isEmpty()
					|| static_cast< long long >(siblings.last()->iPosition) + 40 > INT_MAX) {
					showPositionError();
					return false;
				}
				for (int i = insertIndex; i < siblings.size(); ++i) {
					Channel *child = siblings.at(i);
					sendPositionAdjustment(child, child->iPosition + 40);
				}
				newPosition = static_cast< long long >(previous->iPosition) + 20;
			}
		}
	}

	if (newPosition > INT_MAX || newPosition < INT_MIN) {
		showPositionError();
		return false;
	}

	MumbleProto::ChannelState channelState;
	channelState.set_channel_id(sourceChannel->iId);
	if (sourceChannel->cParent != targetParent) {
		channelState.set_parent(targetParent->iId);
	}
	channelState.set_position(static_cast< int >(newPosition));
	serverHandler->sendMessage(channelState);
	queueModernShellSnapshotSync();
	return true;
}

bool MainWindow::handleModernShellParticipantAction(const qulonglong session, const QString &actionId) {
	ClientUser *participant = ClientUser::get(static_cast< unsigned int >(session));
	if (!participant) {
		return false;
	}

	const QString normalizedActionID = actionId.trimmed();
	if (normalizedActionID.isEmpty()) {
		return false;
	}
	if (normalizedActionID == QLatin1String("screenShareStop")
		|| normalizedActionID == QLatin1String("screenShareWatch")
		|| normalizedActionID == QLatin1String("screenShareStopWatching")
		|| normalizedActionID == QLatin1String("screenShareOpenWindow")) {
		const QString streamID = screenShareStreamForChannel(participant->cChannel);
		bool handled           = false;
		if (!streamID.isEmpty() && m_screenShareManager) {
			if (normalizedActionID == QLatin1String("screenShareStop")) {
				m_screenShareManager->requestStopShare(streamID);
				handled = true;
			} else if (normalizedActionID == QLatin1String("screenShareWatch")) {
				m_screenShareManager->requestStartViewing(streamID);
				handled = true;
			} else if (normalizedActionID == QLatin1String("screenShareStopWatching")) {
				m_screenShareManager->requestStopViewing(streamID);
				handled = true;
			} else {
				handled = m_screenShareManager->focusOrReopenDetachedWindow(streamID);
			}
		}
		if (handled) {
			queueModernShellSnapshotSync();
		}
		return handled;
	}

	const QPointer< ClientUser > previousUser = cuContextUser;
	const QPointer< Channel > previousChannel = cContextChannel;
	const QPoint previousContextPosition      = qpContextPosition;

	cuContextUser     = participant;
	cContextChannel   = participant->cChannel;
	qpContextPosition = QPoint();
	qmUser_aboutToShow();
	ModernShellMenuSerializer::ActionRegistry registry;
	serializeModernShellMenu(qmUser, ModernShellMenuContext::Participant, &registry);
	const bool handled = triggerModernShellSerializedAction(registry, normalizedActionID, participant, nullptr);

	cuContextUser     = previousUser;
	cContextChannel   = previousChannel;
	qpContextPosition = previousContextPosition;

	const PersistentChatTarget restoredTarget = currentPersistentChatTarget();
	syncPersistentChatInputState(restoredTarget.valid && !restoredTarget.readOnly && !restoredTarget.serverLog);
	if (handled) {
		queueModernShellSnapshotSync();
	}

	return handled;
}

bool MainWindow::handleModernShellParticipantActionValueChanged(const qulonglong session, const QString &actionId,
																const int value, const bool final) {
	if (actionId.trimmed() != QLatin1String("localVolume")) {
		return false;
	}

	UserLocalVolumeController::applyDbAdjustment(static_cast< unsigned int >(session), value, final);
	if (final) {
		queueModernShellSnapshotSync();
	}
	return true;
}

bool MainWindow::handleModernShellAppAction(const QString &actionId) {
	bool handled = false;

	if (actionId == QLatin1String("self.toggleMute")) {
		setAudioMute(!Global::get().s.bMute);
		handled = true;
	} else if (actionId == QLatin1String("self.toggleDeaf")) {
		setAudioDeaf(!Global::get().s.bDeaf);
		handled = true;
	} else if (actionId == QLatin1String("self.presence.online")) {
		AudioInputPtr ai = Global::get().ai;
		if (ai) {
			if (ai->activityState == AudioInput::ActivityStateIdle) {
				ai->activityState = AudioInput::ActivityStateReturnedFromIdle;
			}
			ai->tIdle.restart();
		}
		if (Global::get().s.bDeaf) {
			setAudioDeaf(false);
		}
		if (Global::get().s.bMute) {
			setAudioMute(false);
		}
		handled = true;
	} else if (actionId == QLatin1String("self.presence.muted")) {
		if (Global::get().s.bDeaf) {
			setAudioDeaf(false);
		}
		if (!Global::get().s.bMute) {
			setAudioMute(true);
		}
		handled = true;
	} else if (actionId == QLatin1String("self.presence.deafened")) {
		if (!Global::get().s.bDeaf) {
			setAudioDeaf(true);
		}
		handled = true;
	} else if (actionId == QLatin1String("self.presence.away")) {
		return false;
	} else if (actionId == QLatin1String("configure.screenShare")) {
		openConfigDialogPage(QStringLiteral("ScreenShareConfig"));
		queueModernShellSnapshotSync();
		return true;
	}

	if (handled) {
		queueModernShellSnapshotSync();
		return true;
	}

	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	on_qmConfig_aboutToShow();

	ModernShellMenuSerializer::ActionRegistry registry;
	serializeModernShellMenu(qmServer, ModernShellMenuContext::AppServer, &registry);
	if (!handled) {
		handled = triggerModernShellSerializedAction(registry, actionId);
	}
	if (!handled) {
		registry.clear();
		serializeModernShellMenu(qmSelf, ModernShellMenuContext::AppSelf, &registry);
		handled = triggerModernShellSerializedAction(registry, actionId);
	}
	if (!handled) {
		registry.clear();
		serializeModernShellMenu(qmConfig, ModernShellMenuContext::AppConfigure, &registry);
		handled = triggerModernShellSerializedAction(registry, actionId);
	}
	if (!handled) {
		registry.clear();
		serializeModernShellMenu(qmHelp, ModernShellMenuContext::AppHelp, &registry);
		handled = triggerModernShellSerializedAction(registry, actionId);
	}
	if (handled) {
		queueModernShellSnapshotSync();
	}
	return handled;
}
#endif

void MainWindow::triggerContextAction(const QString &actionData, ClientUser *user, Channel *channel) {
	if (!Global::get().sh || actionData.trimmed().isEmpty()) {
		return;
	}

	MumbleProto::ContextAction action;
	action.set_action(u8(actionData));
	if (user && user->uiSession) {
		action.set_session(user->uiSession);
	}
	if (channel) {
		action.set_channel_id(channel->iId);
	}

	Global::get().sh->sendMessage(action);
}

void MainWindow::syncPersistentChatGatewayHandler() {
	appendModernShellConnectTrace(QStringLiteral("syncPersistentChatGatewayHandler gateway=%1 handler=%2")
									  .arg(m_persistentChatGateway ? 1 : 0)
									  .arg(Global::get().sh ? 1 : 0));
	if (!m_persistentChatGateway) {
		return;
	}

	m_persistentChatGateway->setServerHandler(Global::get().sh.get());
}

void MainWindow::refreshCustomChromeStyles() {
	refreshServerNavigatorStyles();
	refreshPersistentChatStyles();
#ifdef Q_OS_WIN
	applyNativeTitleBarTheme(this);
#endif
}

void MainWindow::refreshServerNavigatorStyles() {
	if (!m_serverNavigatorContainer || !qtvUsers) {
		return;
	}

	const QPalette navPalette = m_serverNavigatorContainer->palette();
	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		const QColor railSeamColor = alphaColor(tokens->surface1, 0.56);
		if (m_persistentChatChannelList) {
			QPalette listPalette = m_persistentChatChannelList->palette();
			listPalette.setColor(QPalette::Window, tokens->mantle);
			listPalette.setColor(QPalette::Base, tokens->mantle);
			listPalette.setColor(QPalette::AlternateBase, tokens->accentSubtle);
			listPalette.setColor(QPalette::Text, tokens->subtext0);
			listPalette.setColor(QPalette::WindowText, tokens->text);
			listPalette.setColor(QPalette::Highlight, tokens->accent);
			listPalette.setColor(QPalette::HighlightedText, tokens->text);
			m_persistentChatChannelList->setAutoFillBackground(true);
			m_persistentChatChannelList->setPalette(listPalette);
			m_persistentChatChannelList->viewport()->setAutoFillBackground(true);
			m_persistentChatChannelList->viewport()->setPalette(listPalette);
			m_persistentChatChannelList->viewport()->setStyleSheet(
				QString::fromLatin1("background-color: %1;").arg(qssColor(tokens->mantle)));
		}

		QPalette treePalette = qtvUsers->palette();
		treePalette.setColor(QPalette::Window, tokens->mantle);
		treePalette.setColor(QPalette::Base, tokens->mantle);
		treePalette.setColor(QPalette::AlternateBase, tokens->accentSubtle);
		treePalette.setColor(QPalette::Text, tokens->text);
		treePalette.setColor(QPalette::WindowText, tokens->text);
		treePalette.setColor(QPalette::Highlight, tokens->accent);
		treePalette.setColor(QPalette::HighlightedText, tokens->text);
		qtvUsers->setAutoFillBackground(true);
		qtvUsers->setPalette(treePalette);
		qtvUsers->viewport()->setAutoFillBackground(true);
		qtvUsers->viewport()->setPalette(treePalette);
		qtvUsers->viewport()->setStyleSheet(QString::fromLatin1("background-color: %1;").arg(qssColor(tokens->mantle)));

		m_serverNavigatorContainer->setAutoFillBackground(true);
		QPalette navContainerPalette = m_serverNavigatorContainer->palette();
		navContainerPalette.setColor(QPalette::Window, tokens->mantle);
		m_serverNavigatorContainer->setPalette(navContainerPalette);

		const QString serverNavigatorStyle =
			applyQssArgs(QString::fromLatin1("QWidget#qwServerNavigator {"
											 " background-color: %1;"
											 " border-left: 1px solid %9;"
											 "}"
											 "QFrame#qfServerNavigatorHeader {"
											 " background-color: transparent;"
											 " border: none;"
											 " margin: 0px;"
											 "}"
											 "QFrame#qfServerNavigatorContent {"
											 " background-color: %1;"
											 " border: none;"
											 " border-radius: 0px;"
											 " margin: 0px;"
											 "}"
											 "QLabel#qlServerNavigatorEyebrow,"
											 "QLabel#qlServerNavigatorVoiceSectionEyebrow,"
											 "QLabel#qlServerNavigatorTextChannelsEyebrow {"
											 " color: %3;"
											 " font-size: 9.5px;"
											 " font-weight: 600;"
											 " letter-spacing: 0.14em;"
											 "}"
											 "QLabel#qlServerNavigatorTitle,"
											 "QLabel#qlServerNavigatorTextChannelsTitle {"
											 " color: %4;"
											 " font-size: 11px;"
											 " font-weight: 600;"
											 "}"
											 "QLabel#qlServerNavigatorSubtitle,"
											 "QLabel#qlServerNavigatorFooter,"
											 "QLabel#qlServerNavigatorVoiceSectionSubtitle,"
											 "QLabel#qlServerNavigatorTextChannelsSubtitle {"
											 " color: %5;"
											 " font-size: 9.5px;"
											 " line-height: 1.18em;"
											 "}"
											 "QFrame#qfServerNavigatorFooterFrame {"
											 " background-color: transparent;"
											 " border-top: 1px solid %2;"
											 " margin-top: 4px;"
											 "}"
											 "QPushButton#qpbServerNavigatorFooterPresence {"
											 " background-color: transparent;"
											 " border: none;"
											 " border-radius: 6px;"
											 " padding: 0px;"
											 " text-align: left;"
											 "}"
											 "QPushButton#qpbServerNavigatorFooterPresence:hover {"
											 " background-color: %6;"
											 "}"
											 "QLabel#qlServerNavigatorFooter {"
											 " color: %5;"
											 " font-size: 9.5px;"
											 " line-height: 1.18em;"
											 " padding: 0px;"
											 "}"
											 "QLabel#qlServerNavigatorFooterName {"
											 " color: %4;"
											 " font-size: 10.8px;"
											 " font-weight: 600;"
											 "}"
											 "QLabel#qlServerNavigatorFooterPresence {"
											 " color: %5;"
											 " font-size: 9.2px;"
											 "}"
											 "QLabel#qlServerNavigatorFooterAvatar {"
											 " padding: 0px;"
											 "}"
											 "QFrame#qfServerNavigatorTextChannels {"
											 " background-color: transparent;"
											 " border: none;"
											 "}"
											 "QFrame#qfServerNavigatorTextChannelsDivider {"
											 " background-color: %2;"
											 " min-height: 0px;"
											 " max-height: 1px;"
											 " margin: 4px 0px 3px 0px;"
											 " border: none;"
											 "}"
											 "QFrame#qfServerNavigatorTextChannelsMotd {"
											 " background-color: %2;"
											 " border: none;"
											 " border-radius: 8px;"
											 "}"
											 "QLabel#qlServerNavigatorTextChannelsMotdTitle {"
											 " color: %5;"
											 " font-size: 10px;"
											 " font-weight: 600;"
											 "}"
											 "QLabel#qlServerNavigatorTextChannelsMotdBody {"
											 " color: %4;"
											 " background: transparent;"
											 " font-size: 9.8px;"
											 " line-height: 1.24em;"
											 "}"
											 "QToolButton#qtbServerNavigatorTextChannelsMotdToggle {"
											 " border: none;"
											 " background: transparent;"
											 " color: %8;"
											 " padding: 0px;"
											 " font-size: 10px;"
											 " font-weight: 600;"
											 "}"
											 "QToolButton#qtbServerNavigatorTextChannelsMotdToggle:hover {"
											 " background: transparent;"
											 " color: %8;"
											 "}"
											 "QToolButton#qtbServerNavigatorServerSettings,"
											 "QToolButton#qtbServerNavigatorCreateTextRoom {"
											 " border: none;"
											 " border-radius: 4px;"
											 " background: transparent;"
											 " padding: 2px;"
											 "}"
											 "QToolButton#qtbServerNavigatorServerSettings:hover,"
											 "QToolButton#qtbServerNavigatorCreateTextRoom:hover {"
											 " background-color: %2;"
											 "}"
											 "QTreeView#qtvUsers,"
											 "QWidget#qtvUsersViewport,"
											 "QListWidget#qlwPersistentTextChannels {"
											 " border: none;"
											 " border-radius: 0px;"
											 " background-color: transparent;"
											 " alternate-background-color: transparent;"
											 " color: %4;"
											 " padding: 0px;"
											 " outline: none;"
											 "}"
											 "QTreeView#qtvUsers::branch {"
											 " background: transparent;"
											 " image: none;"
											 " border-image: none;"
											 " min-width: 15px;"
											 " min-height: 18px;"
											 "}"
											 "QTreeView#qtvUsers::item,"
											 "QListWidget#qlwPersistentTextChannels::item {"
											 " min-height: 20px;"
											 " padding: 0px;"
											 " border: none;"
											 " background: transparent;"
											 "}"
											 "QTreeView#qtvUsers::item:hover,"
											 "QTreeView#qtvUsers::item:selected,"
											 "QTreeView#qtvUsers::item:selected:active,"
											 "QTreeView#qtvUsers::item:selected:!active,"
											 "QListWidget#qlwPersistentTextChannels::item:hover,"
											 "QListWidget#qlwPersistentTextChannels::item:selected,"
											 "QListWidget#qlwPersistentTextChannels::item:selected:active,"
											 "QListWidget#qlwPersistentTextChannels::item:selected:!active {"
											 " background: transparent;"
											 " color: %4;"
											 "}"),
						 QStringList{
							 qssColor(tokens->mantle),
							 qssColor(tokens->surface1),
							 qssColor(tokens->overlay0),
							 qssColor(tokens->text),
							 qssColor(tokens->subtext0),
							 qssColor(tokens->surface0),
							 qssColor(tokens->accent),
							 qssColor(tokens->accentHover),
							 qssColor(railSeamColor),
						 });
		m_serverNavigatorContainer->setStyleSheet(serverNavigatorStyle);
		refreshServerNavigatorSectionHeights();
		return;
	}

	const ChromePaletteColors chrome = buildChromePalette(navPalette);

	if (m_persistentChatChannelList) {
		QPalette listPalette = m_persistentChatChannelList->palette();
		listPalette.setColor(QPalette::Base, chrome.panelColor);
		listPalette.setColor(QPalette::AlternateBase, chrome.elevatedCardColor);
		listPalette.setColor(QPalette::Window, chrome.cardColor);
		listPalette.setColor(QPalette::Text, chrome.textColor);
		listPalette.setColor(QPalette::Highlight, navPalette.color(QPalette::Highlight));
		listPalette.setColor(QPalette::HighlightedText, navPalette.color(QPalette::HighlightedText));
		m_persistentChatChannelList->setAutoFillBackground(true);
		m_persistentChatChannelList->setPalette(listPalette);
		m_persistentChatChannelList->viewport()->setAutoFillBackground(true);
		m_persistentChatChannelList->viewport()->setPalette(listPalette);
		m_persistentChatChannelList->viewport()->setStyleSheet(
			QString::fromLatin1("background-color: %1;").arg(chrome.panelColor.name()));
	}

	QPalette treePalette = qtvUsers->palette();
	treePalette.setColor(QPalette::Base, chrome.panelColor);
	treePalette.setColor(QPalette::AlternateBase, chrome.elevatedCardColor);
	treePalette.setColor(QPalette::Window, chrome.cardColor);
	treePalette.setColor(QPalette::Text, chrome.textColor);
	treePalette.setColor(QPalette::Highlight, navPalette.color(QPalette::Highlight));
	treePalette.setColor(QPalette::HighlightedText, navPalette.color(QPalette::HighlightedText));
	qtvUsers->setAutoFillBackground(true);
	qtvUsers->setPalette(treePalette);
	qtvUsers->setProperty("rowHoverColor", chrome.hoverColor);
	qtvUsers->viewport()->setAutoFillBackground(true);
	qtvUsers->viewport()->setPalette(treePalette);
	qtvUsers->viewport()->setStyleSheet(QString::fromLatin1("background-color: %1;").arg(chrome.panelColor.name()));

	m_serverNavigatorContainer->setAutoFillBackground(true);
	QPalette navContainerPalette = m_serverNavigatorContainer->palette();
	navContainerPalette.setColor(QPalette::Window, chrome.cardColor);
	m_serverNavigatorContainer->setPalette(navContainerPalette);
	const QColor railSeamColor   = alphaColor(chrome.borderColor, 0.60);
	QString serverNavigatorStyle = QString::fromLatin1(
		"QWidget#qwServerNavigator {"
		" background-color: %1;"
		" border-left: 1px solid %10;"
		"}"
		"QFrame#qfServerNavigatorHeader {"
		" background-color: transparent;"
		" border: none;"
		" border-radius: 0px;"
		" margin: 0px;"
		"}"
		"QFrame#qfServerNavigatorContent {"
		" background-color: %2;"
		" border: none;"
		" border-radius: 0px;"
		" margin: 0px;"
		"}"
		"QLabel#qlServerNavigatorEyebrow {"
		" color: %3;"
		" font-size: 9.5px;"
		" font-weight: 600;"
		" letter-spacing: 0.14em;"
		"}"
		"QLabel#qlServerNavigatorTitle {"
		" color: %4;"
		" font-size: 11px;"
		" font-weight: 600;"
		"}"
		"QLabel#qlServerNavigatorSubtitle {"
		" color: %5;"
		" font-size: 9.5px;"
		" line-height: 1.18em;"
		" padding-bottom: 0px;"
		"}"
		"QFrame#qfServerNavigatorFooterFrame {"
		" background-color: transparent;"
		" border-top: 1px solid %6;"
		" margin-top: 4px;"
		"}"
		"QPushButton#qpbServerNavigatorFooterPresence {"
		" background-color: transparent;"
		" border: none;"
		" border-radius: 6px;"
		" padding: 0px;"
		" text-align: left;"
		"}"
		"QPushButton#qpbServerNavigatorFooterPresence:hover {"
		" background-color: %2;"
		"}"
		"QLabel#qlServerNavigatorFooter {"
		" color: %5;"
		" font-size: 9.5px;"
		" line-height: 1.2em;"
		" padding: 0px;"
		"}"
		"QLabel#qlServerNavigatorFooterName {"
		" color: %4;"
		" font-size: 10.8px;"
		" font-weight: 600;"
		"}"
		"QLabel#qlServerNavigatorFooterPresence {"
		" color: %5;"
		" font-size: 9.2px;"
		"}"
		"QLabel#qlServerNavigatorFooterAvatar {"
		" padding: 0px;"
		"}"
		"QLabel#qlServerNavigatorVoiceSectionEyebrow, QLabel#qlServerNavigatorTextChannelsEyebrow {"
		" color: %3;"
		" font-size: 9.5px;"
		" font-weight: 600;"
		" letter-spacing: 0.14em;"
		"}"
		"QLabel#qlServerNavigatorVoiceSectionSubtitle, QLabel#qlServerNavigatorTextChannelsSubtitle {"
		" color: %5;"
		" font-size: 9.5px;"
		" line-height: 1.18em;"
		"}"
		"QFrame#qfServerNavigatorTextChannels {"
		" background-color: transparent;"
		" border: none;"
		"}"
		"QFrame#qfServerNavigatorTextChannelsDivider {"
		" background-color: %6;"
		" min-height: 0px;"
		" max-height: 1px;"
		" margin: 4px 0px 3px 0px;"
		" border: none;"
		"}"
		"QLabel#qlServerNavigatorTextChannelsTitle {"
		" color: %4;"
		" font-size: 11px;"
		" font-weight: 600;"
		"}"
		"QFrame#qfServerNavigatorTextChannelsMotd {"
		" background-color: %8;"
		" border: none;"
		" border-radius: 8px;"
		"}"
		"QLabel#qlServerNavigatorTextChannelsMotdTitle {"
		" color: %5;"
		" font-size: 10px;"
		" font-weight: 600;"
		"}"
		"QLabel#qlServerNavigatorTextChannelsMotdBody {"
		" color: %5;"
		" background: transparent;"
		" font-size: 9.5px;"
		" line-height: 1.24em;"
		"}"
		"QToolButton#qtbServerNavigatorTextChannelsMotdToggle {"
		" border: none;"
		" border-radius: 0px;"
		" background: transparent;"
		" color: %5;"
		" padding: 0px;"
		" font-size: 10px;"
		" font-weight: 600;"
		"}"
		"QToolButton#qtbServerNavigatorTextChannelsMotdToggle:hover {"
		" border-color: transparent;"
		" background-color: transparent;"
		" color: %4;"
		"}"
		"QToolButton#qtbServerNavigatorServerSettings,"
		"QToolButton#qtbServerNavigatorCreateTextRoom {"
		" border: none;"
		" border-radius: 4px;"
		" background: transparent;"
		" padding: 2px;"
		"}"
		"QToolButton#qtbServerNavigatorServerSettings:hover,"
		"QToolButton#qtbServerNavigatorCreateTextRoom:hover {"
		" background-color: %8;"
		"}"
		"QTreeView#qtvUsers {"
		" border: none;"
		" border-radius: 0px;"
		" background-color: transparent;"
		" alternate-background-color: transparent;"
		" color: %4;"
		" padding: 0px;"
		" outline: none;"
		" show-decoration-selected: 0;"
		"}"
		"QWidget#qtvUsersViewport {"
		" background-color: transparent;"
		"}"
		"QTreeView#qtvUsers::branch {"
		" background: transparent;"
		" image: none;"
		" border-image: none;"
		" min-width: 15px;"
		" min-height: 18px;"
		"}"
		"QTreeView#qtvUsers::branch:hover, QTreeView#qtvUsers::branch:selected {"
		" background: transparent;"
		" image: none;"
		" border-image: none;"
		"}"
		"QTreeView#qtvUsers::branch:has-siblings:!adjoins-item, "
		"QTreeView#qtvUsers::branch:has-siblings:adjoins-item, "
		"QTreeView#qtvUsers::branch:!has-children:has-siblings, "
		"QTreeView#qtvUsers::branch:!has-children:!has-siblings:adjoins-item, "
		"QTreeView#qtvUsers::branch:!has-children:!has-siblings, "
		"QTreeView#qtvUsers::branch:closed:has-children:has-siblings, "
		"QTreeView#qtvUsers::branch:open:has-children:has-siblings, "
		"QTreeView#qtvUsers::branch:closed:has-children:!has-siblings, "
		"QTreeView#qtvUsers::branch:open:has-children:!has-siblings {"
		" margin: 0px;"
		" image: none;"
		" border-image: none;"
		" }"
		"QTreeView#qtvUsers::item {"
		" min-height: 20px;"
		" padding: 0px 4px;"
		" border: none;"
		" border-radius: 0px;"
		" margin: 0px;"
		"}"
		"QTreeView#qtvUsers::item:hover {"
		" background-color: transparent;"
		"}"
		"QTreeView#qtvUsers::item:selected {"
		" border-radius: 0px;"
		" background-color: transparent;"
		" color: %9;"
		"}"
		"QTreeView#qtvUsers::item:selected:active {"
		" background-color: transparent;"
		" color: %9;"
		"}"
		"QTreeView#qtvUsers::item:selected:!active {"
		" background-color: transparent;"
		" color: %9;"
		"}"
		"QListWidget#qlwPersistentTextChannels {"
		" border: none;"
		" border-radius: 0px;"
		" background-color: transparent;"
		" alternate-background-color: transparent;"
		" color: %4;"
		" padding: 0px;"
		" outline: none;"
		"}"
		"QListWidget#qlwPersistentTextChannels::item {"
		" min-height: 20px;"
		" padding: 0px;"
		" border: none;"
		" border-radius: 0px;"
		" margin: 0px;"
		"}"
		"QListWidget#qlwPersistentTextChannels::item:hover {"
		" background-color: transparent;"
		"}"
		"QListWidget#qlwPersistentTextChannels::item:selected {"
		" background-color: transparent;"
		" color: %4;"
		"}"
		"QListWidget#qlwPersistentTextChannels::item:selected:active {"
		" background-color: transparent;"
		" color: %4;"
		"}"
		"QListWidget#qlwPersistentTextChannels::item:selected:!active {"
		" background-color: transparent;"
		" color: %4;"
		"}");
	serverNavigatorStyle.replace(QStringLiteral("%10"), railSeamColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%9"), chrome.selectedTextColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%8"), chrome.hoverColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%6"), chrome.dividerColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%5"), chrome.mutedTextColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%4"), chrome.textColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%3"), chrome.eyebrowColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%2"), chrome.elevatedCardColor.name());
	serverNavigatorStyle.replace(QStringLiteral("%1"), chrome.cardColor.name());
	m_serverNavigatorContainer->setStyleSheet(serverNavigatorStyle);
	syncServerNavigatorUserMenu();
	refreshServerNavigatorSectionHeights();
}

void MainWindow::refreshServerNavigatorSectionHeights() {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}

	refreshServerNavigatorMotdHeight();
	updateServerNavigatorVoiceTreeHeight();
	updatePersistentChatChannelListHeight();
}

void MainWindow::refreshServerNavigatorMotdHeight() {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}
	if (!m_serverNavigatorContainer || !m_serverNavigatorContainer->isVisible()) {
		return;
	}

	if (!m_serverNavigatorTextChannelsMotdFrame || !m_serverNavigatorTextChannelsMotdBody) {
		return;
	}

	static const int minimumExpandedMotdHeight = 48;

	if (m_persistentChatWelcomeText.trimmed().isEmpty() || !m_serverNavigatorTextChannelsMotdFrame->isVisible()
		|| !m_serverNavigatorTextChannelsMotdBody->isVisible()) {
		m_serverNavigatorTextChannelsMotdBody->setMinimumHeight(0);
		m_serverNavigatorTextChannelsMotdBody->setMaximumHeight(QWIDGETSIZE_MAX);
		m_serverNavigatorTextChannelsMotdBody->setToolTip(QString());
		m_serverNavigatorTextChannelsMotdToggleButton->hide();
		return;
	}

	int availableWidth = m_serverNavigatorTextChannelsMotdBody->width();
	if (availableWidth <= 0 && m_serverNavigatorTextChannelsMotdFrame->layout()) {
		const QMargins margins = m_serverNavigatorTextChannelsMotdFrame->layout()->contentsMargins();
		availableWidth =
			m_serverNavigatorTextChannelsMotdFrame->contentsRect().width() - margins.left() - margins.right();
	}
	if (availableWidth <= 0 && m_serverNavigatorContentFrame) {
		availableWidth = m_serverNavigatorContentFrame->contentsRect().width() - 48;
	}
	if (availableWidth <= 0) {
		return;
	}

	const bool compactNavigator = isServerNavigatorCompactHeight();
	const QString fullHtml      = persistentChatContentHtml(m_persistentChatWelcomeText);

	QTextDocument fullDocument;
	fullDocument.setDocumentMargin(0);
	fullDocument.setDefaultFont(m_serverNavigatorTextChannelsMotdBody->font());
	fullDocument.setHtml(fullHtml);
	fullDocument.setTextWidth(availableWidth);

	const int desiredHeight =
		std::max(static_cast< int >(std::ceil(fullDocument.size().height())), minimumExpandedMotdHeight);
	int condensedHeightBudget = 0;
	if (m_serverNavigatorContentFrame) {
		const int navigatorHeight = m_serverNavigatorContentFrame->contentsRect().height();
		if (navigatorHeight > 0) {
			const double budgetRatio = compactNavigator ? 0.12 : 0.16;
			const int minBudget      = compactNavigator ? 36 : 44;
			const int maxBudget      = compactNavigator ? 52 : 68;
			condensedHeightBudget =
				std::clamp(static_cast< int >(std::round(navigatorHeight * budgetRatio)), minBudget, maxBudget);
		}
	}

	const bool motdOverflows = condensedHeightBudget > 0 && desiredHeight > condensedHeightBudget;
	if (!motdOverflows) {
		m_persistentChatMotdExpanded = false;
	}

	const bool showCondensedMotd = motdOverflows && !m_persistentChatMotdExpanded;
	if (showCondensedMotd) {
		const QString teaserText =
			persistentChatPlainTextSummary(m_persistentChatWelcomeText, compactNavigator ? 96 : 148);
		const QString teaserHtml = QString::fromLatin1("<span>%1</span>").arg(teaserText.toHtmlEscaped());

		QTextDocument teaserDocument;
		teaserDocument.setDocumentMargin(0);
		teaserDocument.setDefaultFont(m_serverNavigatorTextChannelsMotdBody->font());
		teaserDocument.setHtml(teaserHtml);
		teaserDocument.setTextWidth(availableWidth);

		const int teaserHeight =
			std::max(static_cast< int >(std::ceil(teaserDocument.size().height())), compactNavigator ? 26 : 30);
		m_serverNavigatorTextChannelsMotdBody->setText(teaserHtml);
		m_serverNavigatorTextChannelsMotdBody->setWordWrap(true);
		m_serverNavigatorTextChannelsMotdBody->setTextInteractionFlags(Qt::NoTextInteraction);
		m_serverNavigatorTextChannelsMotdBody->setToolTip(
			QTextDocumentFragment::fromHtml(fullHtml).toPlainText().simplified());
		m_serverNavigatorTextChannelsMotdBody->setMinimumHeight(teaserHeight);
		m_serverNavigatorTextChannelsMotdBody->setMaximumHeight(std::max(teaserHeight, compactNavigator ? 40 : 48));
		m_serverNavigatorTextChannelsMotdToggleButton->setText(tr("More"));
		m_serverNavigatorTextChannelsMotdToggleButton->setToolTip(tr("Show the full message of the day"));
		m_serverNavigatorTextChannelsMotdToggleButton->show();
	} else {
		m_serverNavigatorTextChannelsMotdBody->setText(fullHtml);
		m_serverNavigatorTextChannelsMotdBody->setWordWrap(true);
		m_serverNavigatorTextChannelsMotdBody->setTextInteractionFlags(Qt::TextBrowserInteraction);
		m_serverNavigatorTextChannelsMotdBody->setToolTip(QString());
		m_serverNavigatorTextChannelsMotdBody->setMinimumHeight(desiredHeight);
		m_serverNavigatorTextChannelsMotdBody->setMaximumHeight(QWIDGETSIZE_MAX);
		m_serverNavigatorTextChannelsMotdToggleButton->setText(tr("Less"));
		m_serverNavigatorTextChannelsMotdToggleButton->setToolTip(tr("Show a shorter message of the day"));
		m_serverNavigatorTextChannelsMotdToggleButton->setVisible(motdOverflows);
	}

	m_serverNavigatorTextChannelsMotdBody->updateGeometry();
}

void MainWindow::refreshPersistentChatStyles() {
	if (!m_persistentChatContainer) {
		return;
	}

	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		const QColor historyColor            = tokens->base;
		const QColor railColor               = tokens->mantle;
		const QColor inputColor              = mixColors(historyColor, tokens->surface0, 0.54);
		const QColor seamColor               = tokens->surface1;
		const QColor peerBubbleColor         = mixColors(tokens->surface0, tokens->surface2, 0.62);
		const QColor selfBubbleColor         = mixColors(tokens->surface0, tokens->accent, 0.36);
		const QColor bubbleBorderColor       = mixColors(tokens->surface1, tokens->text, 0.16);
		const QColor bubbleActiveBorderColor = mixColors(tokens->surface2, tokens->accent, 0.72);
		QColor selfTintColor                 = tokens->accent;
		selfTintColor.setAlphaF(0.08f);
		const QColor pillColor = mixColors(tokens->surface1, tokens->accent, 0.18);

		m_persistentChatContainer->setAutoFillBackground(true);
		QPalette containerPalette = m_persistentChatContainer->palette();
		containerPalette.setColor(QPalette::Window, historyColor);
		m_persistentChatContainer->setPalette(containerPalette);

		qdwChat->setAutoFillBackground(true);
		QPalette chatDockPalette = qdwChat->palette();
		chatDockPalette.setColor(QPalette::Window, historyColor);
		qdwChat->setPalette(chatDockPalette);
		qdwChat->setStyleSheet(QString::fromLatin1("QDockWidget#qdwChat { background-color: %1; border: none; }")
								   .arg(qssColor(historyColor)));

		qdwLog->setAutoFillBackground(true);
		QPalette logDockPalette = qdwLog->palette();
		logDockPalette.setColor(QPalette::Window, railColor);
		qdwLog->setPalette(logDockPalette);
		qdwLog->setStyleSheet(QString::fromLatin1("QDockWidget#qdwLog { background-color: %1; border: none; }"
												  "QWidget#qwLogSurface { background-color: %1; border: none; }")
								  .arg(qssColor(railColor)));

		const QString dockTitleBarStyle =
			QString::fromLatin1("background-color: %1; color: %1; border: none; margin: 0px; padding: 0px;")
				.arg(qssColor(railColor));
		if (dtbLogDockTitle) {
			dtbLogDockTitle->setAttribute(Qt::WA_StyledBackground, true);
			dtbLogDockTitle->setContentsMargins(0, 0, 0, 0);
			dtbLogDockTitle->setStyleSheet(dockTitleBarStyle);
		}
		if (dtbChatDockTitle) {
			dtbChatDockTitle->setAttribute(Qt::WA_StyledBackground, true);
			dtbChatDockTitle->setContentsMargins(0, 0, 0, 0);
			dtbChatDockTitle->setStyleSheet(dockTitleBarStyle);
		}
		if (menubar) {
			menubar->setStyleSheet(QString::fromLatin1("QMenuBar {"
													   " background-color: %1;"
													   " border: none;"
													   " padding: 0px 3px 0px 0px;"
													   " color: %2;"
													   "}"
													   "QMenuBar::item {"
													   " background: transparent;"
													   " border-radius: 3px;"
													   " font-size: 10px;"
													   " margin: 0px;"
													   " padding: 1px 4px;"
													   " color: %2;"
													   "}"
													   "QMenuBar::item:selected {"
													   " background: %3;"
													   " color: %2;"
													   "}"
													   "QMenuBar::item:pressed {"
													   " background: %4;"
													   " color: %5;"
													   "}"
													   "QMenu {"
													   " background-color: %6;"
													   " border: 1px solid %7;"
													   " color: %2;"
													   "}"
													   "QMenu::item:selected {"
													   " background: %4;"
													   " color: %5;"
													   "}")
									   .arg(qssColor(tokens->crust), qssColor(tokens->text), qssColor(tokens->surface1),
											qssColor(tokens->accentSubtle), qssColor(tokens->accent),
											qssColor(tokens->surface0), qssColor(tokens->surface1)));
		}
		if (qtIconToolbar) {
			qtIconToolbar->setIconSize(QSize(13, 13));
			qtIconToolbar->setStyleSheet(
				QString::fromLatin1("QToolBar {"
									" background-color: %1;"
									" border: none;"
									" spacing: 0px;"
									" padding: 0px;"
									"}"
									"QToolBar::separator { background: %2; width: 1px; margin: 2px 3px; }"
									"QToolBar QToolButton {"
									" border: none;"
									" border-radius: 2px;"
									" background: transparent;"
									" padding: 0px;"
									" margin: 0px;"
									"}"
									"QToolBar QToolButton:hover { background: %3; }"
									"QToolBar QToolButton:pressed { background: %4; }"
									"QToolBar QToolButton:checked { background: transparent; }"
									"QToolBar QComboBox {"
									" background: %3;"
									" border: 1px solid %2;"
									" border-radius: 5px;"
									" color: %5;"
									" min-height: 16px;"
									" padding: 0px 5px;"
									" margin-left: 2px;"
									"}"
									"QToolBar QComboBox:hover { border-color: %6; }"
									"QToolBar QComboBox::drop-down { border: none; width: 18px; }")
					.arg(qssColor(tokens->crust), qssColor(tokens->surface1), qssColor(tokens->surface0),
						 qssColor(tokens->accentSubtle), qssColor(tokens->text), qssColor(tokens->accent)));
		}
		setStyleSheet(
			QString::fromLatin1(
				"QMainWindow::separator:vertical {"
				" background: transparent;"
				" width: 1px;"
				" margin: 8px 0px 4px 0px;"
				"}"
				"QMainWindow::separator:vertical:hover {"
				" background: transparent;"
				" margin: 8px 0px 4px 0px;"
				"}"
				"QMainWindow::separator:horizontal {"
				" background: transparent;"
				" height: 8px;"
				" margin: 0px 8px 4px 8px;"
				"}"
				"QMainWindow::separator:horizontal:hover {"
				" background: %1;"
				" border-radius: 4px;"
				" margin: 2px 12px 2px 12px;"
				"}"
				"QTextBrowser#qteLog QScrollBar:vertical,"
				"QListWidget#qtePersistentChatHistory QScrollBar:vertical,"
				"ChatbarTextEdit#qteChat QScrollBar:vertical,"
				"QListWidget#qlwPersistentTextChannels QScrollBar:vertical,"
				"QTreeView#qtvUsers QScrollBar:vertical {"
				" background: transparent;"
				" width: 5px;"
				" margin: 2px 0px 2px 0px;"
				" border: none;"
				"}"
				"QTextBrowser#qteLog QScrollBar::handle:vertical,"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical,"
				"ChatbarTextEdit#qteChat QScrollBar::handle:vertical,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::handle:vertical,"
				"QTreeView#qtvUsers QScrollBar::handle:vertical {"
				" background: %2;"
				" min-height: 24px;"
				" border-radius: 3px;"
				"}"
				"QTextBrowser#qteLog QScrollBar:horizontal,"
				"QListWidget#qtePersistentChatHistory QScrollBar:horizontal,"
				"ChatbarTextEdit#qteChat QScrollBar:horizontal {"
				" background: transparent;"
				" height: 5px;"
				" margin: 0px;"
				" border: none;"
				"}"
				"QTextBrowser#qteLog QScrollBar::handle:vertical:hover,"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical:hover,"
				"ChatbarTextEdit#qteChat QScrollBar::handle:vertical:hover,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::handle:vertical:hover,"
				"QTreeView#qtvUsers QScrollBar::handle:vertical:hover,"
				"QTextBrowser#qteLog QScrollBar::handle:vertical:pressed,"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical:pressed,"
				"ChatbarTextEdit#qteChat QScrollBar::handle:vertical:pressed,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::handle:vertical:pressed,"
				"QTreeView#qtvUsers QScrollBar::handle:vertical:pressed {"
				" background: %3;"
				"}"
				"QTextBrowser#qteLog QScrollBar::handle:horizontal,"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:horizontal,"
				"ChatbarTextEdit#qteChat QScrollBar::handle:horizontal {"
				" background: %2;"
				" min-width: 24px;"
				" border-radius: 3px;"
				"}"
				"QTextBrowser#qteLog QScrollBar::handle:horizontal:hover,"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:horizontal:hover,"
				"ChatbarTextEdit#qteChat QScrollBar::handle:horizontal:hover,"
				"QTextBrowser#qteLog QScrollBar::handle:horizontal:pressed,"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:horizontal:pressed,"
				"ChatbarTextEdit#qteChat QScrollBar::handle:horizontal:pressed {"
				" background: %3;"
				"}"
				"QTextBrowser#qteLog QScrollBar::add-line:vertical,"
				"QTextBrowser#qteLog QScrollBar::sub-line:vertical,"
				"QTextBrowser#qteLog QScrollBar::add-page:vertical,"
				"QTextBrowser#qteLog QScrollBar::sub-page:vertical,"
				"QTextBrowser#qteLog QScrollBar::add-line:horizontal,"
				"QTextBrowser#qteLog QScrollBar::sub-line:horizontal,"
				"QTextBrowser#qteLog QScrollBar::add-page:horizontal,"
				"QTextBrowser#qteLog QScrollBar::sub-page:horizontal,"
				"QListWidget#qtePersistentChatHistory QScrollBar::add-line:vertical,"
				"QListWidget#qtePersistentChatHistory QScrollBar::sub-line:vertical,"
				"QListWidget#qtePersistentChatHistory QScrollBar::add-page:vertical,"
				"QListWidget#qtePersistentChatHistory QScrollBar::sub-page:vertical,"
				"QListWidget#qtePersistentChatHistory QScrollBar::add-line:horizontal,"
				"QListWidget#qtePersistentChatHistory QScrollBar::sub-line:horizontal,"
				"QListWidget#qtePersistentChatHistory QScrollBar::add-page:horizontal,"
				"QListWidget#qtePersistentChatHistory QScrollBar::sub-page:horizontal,"
				"ChatbarTextEdit#qteChat QScrollBar::add-line:vertical,"
				"ChatbarTextEdit#qteChat QScrollBar::sub-line:vertical,"
				"ChatbarTextEdit#qteChat QScrollBar::add-page:vertical,"
				"ChatbarTextEdit#qteChat QScrollBar::sub-page:vertical,"
				"ChatbarTextEdit#qteChat QScrollBar::add-line:horizontal,"
				"ChatbarTextEdit#qteChat QScrollBar::sub-line:horizontal,"
				"ChatbarTextEdit#qteChat QScrollBar::add-page:horizontal,"
				"ChatbarTextEdit#qteChat QScrollBar::sub-page:horizontal,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::add-line:vertical,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::sub-line:vertical,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::add-page:vertical,"
				"QListWidget#qlwPersistentTextChannels QScrollBar::sub-page:vertical,"
				"QTreeView#qtvUsers QScrollBar::add-line:vertical,"
				"QTreeView#qtvUsers QScrollBar::sub-line:vertical,"
				"QTreeView#qtvUsers QScrollBar::add-page:vertical,"
				"QTreeView#qtvUsers QScrollBar::sub-page:vertical {"
				" background: transparent;"
				" border: none;"
				" height: 0px;"
				"}"
				"QListWidget#qtePersistentChatHistory QScrollBar:vertical {"
				" width: 0px;"
				" margin: 0px;"
				"}"
				"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar:vertical {"
				" width: 5px;"
				"}"
				"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical {"
				" background: transparent;"
				" border-radius: 3px;"
				"}"
				"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar::handle:vertical {"
				" background: %2;"
				"}"
				"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar::handle:vertical:hover,"
				"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar::handle:vertical:pressed {"
				" background: %3;"
				"}")
				.arg(qssColor(seamColor), qssColor(alphaColor(tokens->text, 0.10)),
					 qssColor(alphaColor(tokens->text, 0.20))));

		if (m_persistentChatHistory) {
			QPalette historyPalette = m_persistentChatHistory->palette();
			historyPalette.setColor(QPalette::Base, historyColor);
			historyPalette.setColor(QPalette::AlternateBase, historyColor);
			historyPalette.setColor(QPalette::Window, historyColor);
			historyPalette.setColor(QPalette::Text, tokens->text);
			historyPalette.setColor(QPalette::Highlight, tokens->accent);
			historyPalette.setColor(QPalette::HighlightedText, tokens->crust);
			m_persistentChatHistory->setAutoFillBackground(true);
			m_persistentChatHistory->setPalette(historyPalette);
			m_persistentChatHistory->viewport()->setAutoFillBackground(true);
			m_persistentChatHistory->viewport()->setPalette(historyPalette);
			m_persistentChatHistory->viewport()->setStyleSheet(
				QString::fromLatin1("background-color: %1; border: none; outline: none;").arg(qssColor(historyColor)));
		}

		if (qteLog) {
			QPalette logPalette = qteLog->palette();
			logPalette.setColor(QPalette::Base, historyColor);
			logPalette.setColor(QPalette::AlternateBase, historyColor);
			logPalette.setColor(QPalette::Window, historyColor);
			logPalette.setColor(QPalette::Text, tokens->text);
			logPalette.setColor(QPalette::Highlight, tokens->accent);
			logPalette.setColor(QPalette::HighlightedText, tokens->crust);
			qteLog->setAutoFillBackground(true);
			qteLog->setPalette(logPalette);
			qteLog->viewport()->setAutoFillBackground(true);
			qteLog->viewport()->setPalette(logPalette);
			qteLog->setStyleSheet(QString::fromLatin1("border-style: solid;"
													  "border-color: %1;"
													  "border-width: 1px 0px 1px 1px;"
													  "border-radius: 0px;"
													  "background-color: %2; padding: 0px; outline: none;")
									  .arg(qssColor(tokens->surface1), qssColor(historyColor)));
			qteLog->viewport()->setStyleSheet(
				QString::fromLatin1("background-color: %1; border: none; outline: none;").arg(qssColor(historyColor)));
		}
		if (m_persistentChatLogView) {
			QPalette logPalette = m_persistentChatLogView->palette();
			logPalette.setColor(QPalette::Base, historyColor);
			logPalette.setColor(QPalette::AlternateBase, historyColor);
			logPalette.setColor(QPalette::Window, historyColor);
			logPalette.setColor(QPalette::Text, tokens->text);
			logPalette.setColor(QPalette::Highlight, tokens->accent);
			logPalette.setColor(QPalette::HighlightedText, tokens->crust);
			m_persistentChatLogView->setAutoFillBackground(true);
			m_persistentChatLogView->setPalette(logPalette);
			m_persistentChatLogView->viewport()->setAutoFillBackground(true);
			m_persistentChatLogView->viewport()->setPalette(logPalette);
			m_persistentChatLogView->setStyleSheet(
				QString::fromLatin1("border: none; background-color: %1; padding: 0px; outline: none;")
					.arg(qssColor(historyColor)));
			m_persistentChatLogView->viewport()->setStyleSheet(
				QString::fromLatin1("background-color: %1; border: none; outline: none;").arg(qssColor(historyColor)));
		}

		if (QWidget *logSurface = qdwLog->widget()) {
			QPalette logSurfacePalette = logSurface->palette();
			logSurfacePalette.setColor(QPalette::Window, railColor);
			logSurface->setAutoFillBackground(true);
			logSurface->setPalette(logSurfacePalette);
			logSurface->setStyleSheet(
				QString::fromLatin1("QWidget#qwLogSurface { background-color: %1; border: none; }")
					.arg(qssColor(railColor)));
		}

		if (qteChat) {
			QPalette inputPalette = qteChat->palette();
			inputPalette.setColor(QPalette::Base, Qt::transparent);
			inputPalette.setColor(QPalette::AlternateBase, Qt::transparent);
			inputPalette.setColor(QPalette::Window, Qt::transparent);
			inputPalette.setColor(QPalette::Text, tokens->text);
			inputPalette.setColor(QPalette::Highlight, tokens->accent);
			inputPalette.setColor(QPalette::HighlightedText, tokens->crust);
			inputPalette.setColor(QPalette::PlaceholderText, tokens->overlay0);
			qteChat->setAutoFillBackground(false);
			qteChat->setPalette(inputPalette);
			qteChat->viewport()->setAutoFillBackground(false);
			qteChat->viewport()->setPalette(inputPalette);
			qteChat->setStyleSheet(QString());
			qteChat->viewport()->setStyleSheet(QString());
		}

		const QString persistentChatStyle =
			applyQssArgs(QString::fromLatin1(
							 "QWidget#qwPersistentChat {"
							 " background-color: %1;"
							 "}"
							 "QFrame#qfPersistentChatHeader {"
							 " border: none;"
							 " border-bottom: 1px solid %2;"
							 " border-radius: 0px;"
							 " background-color: %1;"
							 "}"
							 "QWidget#qwPersistentChatSurface,"
							 "QWidget#qwPersistentChatPanel {"
							 " border: none;"
							 " border-radius: 0px;"
							 " background-color: transparent;"
							 "}"
							 "QLabel#qlPersistentChatHeaderEyebrow {"
							 " color: %3;"
							 " font-size: 9px;"
							 " font-weight: 600;"
							 " letter-spacing: 0.12em;"
							 "}"
							 "QLabel#qlPersistentChatHeaderSubtitle {"
							 " color: %4;"
							 " font-size: 10px;"
							 " line-height: 1.2em;"
							 "}"
							 "QLabel#qlPersistentChatHeaderContext {"
							 " color: %4;"
							 " background-color: %12;"
							 " border-radius: 999px;"
							 " padding: 1px 7px;"
							 " font-size: 9px;"
							 "}"
							 "QListWidget#qtePersistentChatHistory {"
							 " border: none;"
							 " border-radius: 0px;"
							 " background-color: %1;"
							 " padding: 0px;"
							 " outline: none;"
							 "}"
							 "QListWidget#qtePersistentChatHistory::item {"
							 " border: none;"
							 " margin: 0px;"
							 " padding: 0px;"
							 "}"
							 "QFrame#qfPersistentChatComposer {"
							 " border: none;"
							 " border-top: none;"
							 " background-color: %1;"
							 "}"
							 "QWidget#qwPersistentChatComposerInputRow {"
							 " background: %6;"
							 " border: 1px solid %2;"
							 " border-radius: 8px;"
							 " padding: 0px;"
							 "}"
							 "QFrame#qfPersistentChatReply {"
							 " border: none;"
							 " border-radius: 8px;"
							 " background-color: %6;"
							 "}"
							 "QLabel#qlPersistentChatReplyLabel {"
							 " color: %7;"
							 " font-weight: 600;"
							 "}"
							 "QLabel#qlPersistentChatReplySnippet {"
							 " color: %4;"
							 "}"
							 "QToolButton#qtbPersistentChatAttach {"
							 " border: none;"
							 " border-radius: 6px;"
							 " background-color: transparent;"
							 " min-width: 22px;"
							 " min-height: 22px;"
							 " max-width: 22px;"
							 " max-height: 22px;"
							 " padding: 0px;"
							 "}"
							 "QToolButton#qtbPersistentChatAttach:hover {"
							 " background-color: %12;"
							 "}"
							 "QToolButton#qtbPersistentChatAttach:disabled {"
							 " background-color: transparent;"
							 "}"
							 "QToolButton#qtbPersistentChatSend {"
							 " border: none;"
							 " border-radius: 6px;"
							 " background-color: transparent;"
							 " min-width: 22px;"
							 " min-height: 22px;"
							 " max-width: 22px;"
							 " max-height: 22px;"
							 " padding: 0px;"
							 "}"
							 "QToolButton#qtbPersistentChatSend:hover {"
							 " background-color: %12;"
							 "}"
							 "QToolButton#qtbPersistentChatSend:disabled {"
							 " background-color: transparent;"
							 "}"
							 "QFrame#qfPersistentChatBanner {"
							 " border: 1px solid %2;"
							 " border-radius: 12px;"
							 " background-color: %6;"
							 "}"
							 "QLabel#qlPersistentChatBannerEyebrow {"
							 " color: %3;"
							 " letter-spacing: 0.12em;"
							 "}"
							 "QLabel#qlPersistentChatBannerTitle {"
							 " color: %7;"
							 " font-weight: 700;"
							 "}"
							 "QLabel#qlPersistentChatBannerBody {"
							 " color: %4;"
							 " line-height: 1.45em;"
							 "}"
							 "QWidget#qwPersistentChatBannerHints {"
							 " background: transparent;"
							 " border: none;"
							 "}"
							 "QLabel#qlPersistentChatBannerHint {"
							 " color: %11;"
							 " background-color: %12;"
							 " border-radius: 999px;"
							 " padding: 4px 10px;"
							 "}"
							 "QLabel#qlPersistentChatStatusPill,"
							 "QLabel#qlPersistentChatLoadingPill,"
							 "QLabel#qlPersistentChatInfoPill,"
							 "QLabel#qlPersistentChatUnreadPill,"
							 "QLabel#qlPersistentChatAggregateNotice,"
							 "QLabel#qlPersistentChatEmptyPill,"
							 "QPushButton#qpbPersistentChatLoadOlder {"
							 " border: none;"
							 " border-radius: 999px;"
							 " background: %12;"
							 " color: %11;"
							 " padding: 5px 12px;"
							 "}"
							 "QPushButton#qpbPersistentChatLoadOlder:hover {"
							 " color: %7;"
							 "}"
							 "QWidget#qwPersistentChatMessageGroup,"
							 "QWidget#qwPersistentChatMessageColumn,"
							 "QWidget#qwPersistentChatGroupHeader,"
							 "QWidget#qwPersistentChatDateDivider,"
							 "QWidget#qwPersistentChatLeadingSpacer {"
							 " background: transparent;"
							 " border: none;"
							 "}"
							 "QWidget#qwPersistentChatMessageGroup[selfAuthored=\"true\"] {"
							 " background-color: transparent;"
							 " border-radius: 0px;"
							 "}"
							 "QWidget#qwPersistentChatMessageGroup[rowActive=\"true\"] {"
							 " background-color: %14;"
							 " border-radius: 10px;"
							 "}"
							 "QWidget#qwPersistentChatMessageGroup[selfAuthored=\"true\"][rowActive=\"true\"] {"
							 " background-color: %14;"
							 " border-radius: 10px;"
							 "}"
							 "QWidget#qwPersistentChatMessageGroup[persistentChatSystem=\"true\"],"
							 "QWidget#qwPersistentChatMessageGroup[persistentChatSystem=\"true\"][rowActive=\"true\"] {"
							 " background: transparent;"
							 "}"
							 "QWidget#qwPersistentChatMessageGroup[compactTranscript=\"true\"],"
							 "QWidget#qwPersistentChatMessageGroup[compactTranscript=\"true\"][rowActive=\"true\"] {"
							 " background: transparent;"
							 " border-radius: 0px;"
							 "}"
							 "QFrame#qfPersistentChatDateDividerLine {"
							 " background-color: %2;"
							 " border: none;"
							 "}"
							 "QFrame#qfPersistentChatDateDividerLine[compactTranscript=\"true\"] {"
							 " background-color: %10;"
							 "}"
							 "QLabel#qlPersistentChatDateDividerText {"
							 " color: %3;"
							 " font-size: 0.72em;"
							 " font-weight: 600;"
							 " letter-spacing: 0.08em;"
							 "}"
							 "QLabel#qlPersistentChatDateDividerText[compactTranscript=\"true\"] {"
							 " color: %11;"
							 " font-size: 0.86em;"
							 " font-weight: 700;"
							 " letter-spacing: 0.14em;"
							 "}"
							 "QFrame#qfPersistentChatAvatarFrame {"
							 " border: 1px solid %10;"
							 " border-radius: 11px;"
							 " background-color: %12;"
							 "}"
							 "QLabel#qlPersistentChatAvatarFallback {"
							 " border: none;"
							 " border-radius: 11px;"
							 " background-color: transparent;"
							 " color: %11;"
							 " font-weight: 700;"
							 "}"
							 "QLabel#qlPersistentChatAvatar {"
							 " background: transparent;"
							 "}"
							 "QLabel#qlPersistentChatGroupActor {"
							 " color: %11;"
							 " font-weight: 600;"
							 " font-size: 0.80em;"
							 "}"
							 "QLabel#qlPersistentChatGroupTime {"
							 " color: %3;"
							 " font-size: 0.66em;"
							 "}"
							 "QToolButton#qtbPersistentChatGroupScope {"
							 " border: none;"
							 " background: transparent;"
							 " color: %4;"
							 " padding: 1px 5px;"
							 "}"
							 "QToolButton#qtbPersistentChatGroupScope:hover {"
							 " color: %11;"
							 "}"
							 "QWidget#qwPersistentChatBubbleActions {"
							 " background: %6;"
							 " border: 1px solid %2;"
							 " border-radius: 3px;"
							 "}"
							 "QToolButton#qtbPersistentChatBubbleAction {"
							 " border: none;"
							 " background: transparent;"
							 " color: %4;"
							 " min-width: 22px;"
							 " min-height: 22px;"
							 " max-width: 22px;"
							 " max-height: 22px;"
							 " padding: 0px;"
							 "}"
							 "QToolButton#qtbPersistentChatBubbleAction:hover {"
							 " background: %2;"
							 " color: %7;"
							 "}"
							 "QFrame#qfPersistentChatBubble {"
							 " border: 1px solid %10;"
							 " border-top-left-radius: 10px;"
							 " border-top-right-radius: 10px;"
							 " border-bottom-left-radius: 10px;"
							 " border-bottom-right-radius: 10px;"
							 " background-color: %8;"
							 "}"
							 "QFrame#qfPersistentChatBubble[selfAuthored=\"true\"] {"
							 " border-color: %11;"
							 " border-top-left-radius: 10px;"
							 " border-top-right-radius: 10px;"
							 " border-bottom-left-radius: 10px;"
							 " border-bottom-right-radius: 10px;"
							 " background-color: %9;"
							 "}"
							 "QFrame#qfPersistentChatBubble[bubbleActive=\"true\"] {"
							 " border-color: %13;"
							 "}"
							 "QFrame#qfPersistentChatBubble[selfAuthored=\"true\"][bubbleActive=\"true\"] {"
							 " border-color: %11;"
							 "}"
							 "QFrame#qfPersistentChatBubble[persistentChatSystem=\"true\"] {"
							 " border: none;"
							 " background: transparent;"
							 "}"
							 "QFrame#qfPersistentChatBubble[compactTranscript=\"true\"],"
							 "QFrame#qfPersistentChatBubble[compactTranscript=\"true\"][selfAuthored=\"true\"],"
							 "QFrame#qfPersistentChatBubble[compactTranscript=\"true\"][bubbleActive=\"true\"],"
							 "QFrame#qfPersistentChatBubble[compactTranscript=\"true\"][selfAuthored=\"true\"]["
							 "bubbleActive=\"true\"] {"
							 " border: none;"
							 " border-radius: 0px;"
							 " background: transparent;"
							 "}"
							 "QFrame#qfPersistentChatBubbleQuote {"
							 " border: 1px solid %10;"
							 " border-left: 2px solid %11;"
							 " border-radius: 9px;"
							 " background-color: %6;"
							 "}"
							 "QFrame#qfPersistentChatBubbleQuote[compactTranscript=\"true\"] {"
							 " border: none;"
							 " border-left: 2px solid %11;"
							 " border-radius: 0px;"
							 " background: transparent;"
							 "}"
							 "QLabel#qlPersistentChatBubbleQuoteActor {"
							 " color: %11;"
							 "}"
							 "QLabel#qlPersistentChatBubbleQuoteSnippet {"
							 " color: %4;"
							 "}"
							 "ChatbarTextEdit#qteChat {"
							 " border: none;"
							 " border-radius: 0px;"
							 " background-color: transparent;"
							 " color: %7;"
							 " min-height: 26px;"
							 " max-height: 72px;"
							 " padding: 3px 0px;"
							 "}"
							 "ChatbarTextEdit#qteChat:focus {"
							 " border: none;"
							 "}"
							 "ChatbarTextEdit#qteChat > QWidget {"
							 " border: none;"
							 " border-radius: 0px;"
							 " background-color: transparent;"
							 " color: %7;"
							 "}"
							 "QLabel#qlPersistentChatHeaderTitle {"
							 " color: %7;"
							 " font-size: 12px;"
							 " font-weight: 600;"
							 "}"),
						 QStringList{ qssColor(historyColor), qssColor(seamColor), qssColor(tokens->overlay0),
									  qssColor(tokens->subtext0), qssColor(railColor), qssColor(inputColor),
									  qssColor(tokens->text), qssColor(peerBubbleColor), qssColor(selfBubbleColor),
									  qssColor(bubbleBorderColor), qssColor(tokens->accent), qssColor(pillColor),
									  qssColor(bubbleActiveBorderColor), qssColor(selfTintColor) });
		m_persistentChatContainer->setStyleSheet(persistentChatStyle);
		if (m_persistentChatHistory) {
			m_persistentChatHistory->setStyleSheet(persistentChatStyle);
			if (QWidget *viewport = m_persistentChatHistory->viewport()) {
				viewport->setStyleSheet(persistentChatStyle);
			}
			if (m_persistentChatHistoryDelegate) {
				m_persistentChatHistoryDelegate->clearCache();
			}
			m_persistentChatHistory->stabilizeVisibleContent();
		}
		return;
	}

	const QPalette chatPalette       = m_persistentChatContainer->palette();
	const ChromePaletteColors chrome = buildChromePalette(chatPalette);
	const QColor historyColor        = chrome.cardColor;
	const QColor headerSurfaceColor = mixColors(historyColor, chrome.elevatedCardColor, chrome.darkTheme ? 0.78 : 0.20);
	const QColor inputColor         = mixColors(historyColor, chrome.inputColor, chrome.darkTheme ? 0.56 : 0.72);
	const QColor seamColor          = chrome.dividerColor;
	const QColor peerBubbleColor =
		mixColors(chrome.cardColor, chrome.elevatedCardColor, chrome.darkTheme ? 0.88 : 0.22);
	const QColor selfBubbleColor =
		mixColors(chrome.elevatedCardColor, chrome.selectedColor, chrome.darkTheme ? 0.38 : 0.24);
	const QColor bubbleBorderColor = mixColors(chrome.borderColor, chrome.textColor, chrome.darkTheme ? 0.16 : 0.07);
	const QColor bubbleActiveBorderColor =
		mixColors(bubbleBorderColor, chrome.selectedColor, chrome.darkTheme ? 0.62 : 0.34);
	QColor selfTintColor = chrome.selectedColor;
	selfTintColor.setAlphaF(0.08f);
	const QColor quoteColor = mixColors(peerBubbleColor, chrome.panelColor, chrome.darkTheme ? 0.42 : 0.20);
	const QColor pillColor  = mixColors(chrome.elevatedCardColor, chrome.selectedColor, chrome.darkTheme ? 0.16 : 0.08);
	const QColor avatarBadgeColor =
		mixColors(chrome.elevatedCardColor, chrome.textColor, chrome.darkTheme ? 0.20 : 0.08);

	m_persistentChatContainer->setAutoFillBackground(true);
	QPalette containerPalette = m_persistentChatContainer->palette();
	containerPalette.setColor(QPalette::Window, historyColor);
	m_persistentChatContainer->setPalette(containerPalette);

	qdwChat->setAutoFillBackground(true);
	QPalette chatDockPalette = qdwChat->palette();
	chatDockPalette.setColor(QPalette::Window, historyColor);
	qdwChat->setPalette(chatDockPalette);
	qdwChat->setStyleSheet(
		QString::fromLatin1("QDockWidget#qdwChat { background-color: %1; border: none; }").arg(historyColor.name()));
	qdwLog->setAutoFillBackground(true);
	QPalette logDockPalette = qdwLog->palette();
	logDockPalette.setColor(QPalette::Window, chrome.railColor);
	qdwLog->setPalette(logDockPalette);
	qdwLog->setStyleSheet(QString::fromLatin1("QDockWidget#qdwLog { background-color: %1; border: none; }"
											  "QWidget#qwLogSurface { background-color: %2; border: none; }")
							  .arg(chrome.railColor.name(), chrome.railColor.name()));
	const QString dockTitleBarStyle =
		QString::fromLatin1("background-color: %1; color: %1; border: none; margin: 0px; padding: 0px;")
			.arg(chrome.railColor.name());
	if (dtbLogDockTitle) {
		dtbLogDockTitle->setAttribute(Qt::WA_StyledBackground, true);
		dtbLogDockTitle->setContentsMargins(0, 0, 0, 0);
		dtbLogDockTitle->setStyleSheet(dockTitleBarStyle);
	}
	if (dtbChatDockTitle) {
		dtbChatDockTitle->setAttribute(Qt::WA_StyledBackground, true);
		dtbChatDockTitle->setContentsMargins(0, 0, 0, 0);
		dtbChatDockTitle->setStyleSheet(dockTitleBarStyle);
	}
	if (menubar) {
		menubar->setStyleSheet(QString::fromLatin1("QMenuBar {"
												   " background-color: %1;"
												   " border: none;"
												   " padding: 0px 3px 0px 0px;"
												   "}"
												   "QMenuBar::item {"
												   " background: transparent;"
												   " border-radius: 3px;"
												   " font-size: 10px;"
												   " margin: 0px;"
												   " padding: 1px 4px;"
												   "}"
												   "QMenuBar::item:selected {"
												   " background: %2;"
												   " color: %4;"
												   "}"
												   "QMenuBar::item:pressed {"
												   " background: %3;"
												   " color: %4;"
												   "}")
								   .arg(chrome.railColor.name(), chrome.hoverColor.name(), chrome.selectedColor.name(),
										chrome.selectedTextColor.name()));
	}
	if (qtIconToolbar) {
		qtIconToolbar->setIconSize(QSize(13, 13));
		qtIconToolbar->setStyleSheet(
			QString::fromLatin1("QToolBar { background-color: %1; border: none; spacing: 0px; padding: 0px; }"
								"QToolBar::separator { background: transparent; width: 1px; margin: 2px 3px; }"
								"QToolBar QToolButton {"
								" border: none;"
								" border-radius: 2px;"
								" background: transparent;"
								" padding: 0px;"
								" margin: 0px;"
								"}"
								"QToolBar QToolButton:hover { background: %2; }"
								"QToolBar QToolButton:pressed, QToolBar QToolButton:checked { background: %3; }"
								"QToolBar QComboBox {"
								" background: %4;"
								" border: 1px solid %5;"
								" border-radius: 8px;"
								" color: %6;"
								" min-height: 16px;"
								" padding: 0px 5px;"
								" margin-left: 2px;"
								"}"
								"QToolBar QComboBox:hover { border-color: %2; }"
								"QToolBar QComboBox::drop-down { border: none; width: 18px; }")
				.arg(chrome.cardColor.name(), chrome.hoverColor.name(), chrome.selectedColor.name(),
					 chrome.elevatedCardColor.name(), chrome.dividerColor.name(), chrome.textColor.name()));
	}
	setStyleSheet(
		QString::fromLatin1(
			"QMainWindow::separator:vertical {"
			" background: transparent;"
			" width: 1px;"
			" margin: 8px 0px 4px 0px;"
			"}"
			"QMainWindow::separator:vertical:hover {"
			" background: transparent;"
			" border-radius: 0px;"
			" margin: 8px 0px 4px 0px;"
			"}"
			"QMainWindow::separator:horizontal {"
			" background: transparent;"
			" height: 8px;"
			" margin: 0px 8px 4px 8px;"
			"}"
			"QMainWindow::separator:horizontal:hover {"
			" background: %1;"
			" border-radius: 4px;"
			" margin: 2px 12px 2px 12px;"
			"}"
			"QTextBrowser#qteLog QScrollBar:vertical,"
			"QListWidget#qtePersistentChatHistory QScrollBar:vertical,"
			"ChatbarTextEdit#qteChat QScrollBar:vertical,"
			"QListWidget#qlwPersistentTextChannels QScrollBar:vertical,"
			"QTreeView#qtvUsers QScrollBar:vertical {"
			" background: transparent;"
			" width: 5px;"
			" margin: 2px 0px 2px 0px;"
			" border: none;"
			"}"
			"QTextBrowser#qteLog QScrollBar::handle:vertical,"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical,"
			"ChatbarTextEdit#qteChat QScrollBar::handle:vertical,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::handle:vertical,"
			"QTreeView#qtvUsers QScrollBar::handle:vertical {"
			" background: %2;"
			" min-height: 24px;"
			" border-radius: 3px;"
			"}"
			"QTextBrowser#qteLog QScrollBar:horizontal,"
			"QListWidget#qtePersistentChatHistory QScrollBar:horizontal,"
			"ChatbarTextEdit#qteChat QScrollBar:horizontal {"
			" background: transparent;"
			" height: 5px;"
			" margin: 0px;"
			" border: none;"
			"}"
			"QTextBrowser#qteLog QScrollBar::handle:vertical:hover,"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical:hover,"
			"ChatbarTextEdit#qteChat QScrollBar::handle:vertical:hover,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::handle:vertical:hover,"
			"QTreeView#qtvUsers QScrollBar::handle:vertical:hover,"
			"QTextBrowser#qteLog QScrollBar::handle:vertical:pressed,"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical:pressed,"
			"ChatbarTextEdit#qteChat QScrollBar::handle:vertical:pressed,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::handle:vertical:pressed,"
			"QTreeView#qtvUsers QScrollBar::handle:vertical:pressed {"
			" background: %3;"
			"}"
			"QTextBrowser#qteLog QScrollBar::handle:horizontal,"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:horizontal,"
			"ChatbarTextEdit#qteChat QScrollBar::handle:horizontal {"
			" background: %2;"
			" min-width: 24px;"
			" border-radius: 3px;"
			"}"
			"QTextBrowser#qteLog QScrollBar::handle:horizontal:hover,"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:horizontal:hover,"
			"ChatbarTextEdit#qteChat QScrollBar::handle:horizontal:hover,"
			"QTextBrowser#qteLog QScrollBar::handle:horizontal:pressed,"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:horizontal:pressed,"
			"ChatbarTextEdit#qteChat QScrollBar::handle:horizontal:pressed {"
			" background: %3;"
			"}"
			"QTextBrowser#qteLog QScrollBar::add-line:vertical,"
			"QTextBrowser#qteLog QScrollBar::sub-line:vertical,"
			"QTextBrowser#qteLog QScrollBar::add-page:vertical,"
			"QTextBrowser#qteLog QScrollBar::sub-page:vertical,"
			"QTextBrowser#qteLog QScrollBar::add-line:horizontal,"
			"QTextBrowser#qteLog QScrollBar::sub-line:horizontal,"
			"QTextBrowser#qteLog QScrollBar::add-page:horizontal,"
			"QTextBrowser#qteLog QScrollBar::sub-page:horizontal,"
			"QListWidget#qtePersistentChatHistory QScrollBar::add-line:vertical,"
			"QListWidget#qtePersistentChatHistory QScrollBar::sub-line:vertical,"
			"QListWidget#qtePersistentChatHistory QScrollBar::add-page:vertical,"
			"QListWidget#qtePersistentChatHistory QScrollBar::sub-page:vertical,"
			"QListWidget#qtePersistentChatHistory QScrollBar::add-line:horizontal,"
			"QListWidget#qtePersistentChatHistory QScrollBar::sub-line:horizontal,"
			"QListWidget#qtePersistentChatHistory QScrollBar::add-page:horizontal,"
			"QListWidget#qtePersistentChatHistory QScrollBar::sub-page:horizontal,"
			"ChatbarTextEdit#qteChat QScrollBar::add-line:vertical,"
			"ChatbarTextEdit#qteChat QScrollBar::sub-line:vertical,"
			"ChatbarTextEdit#qteChat QScrollBar::add-page:vertical,"
			"ChatbarTextEdit#qteChat QScrollBar::sub-page:vertical,"
			"ChatbarTextEdit#qteChat QScrollBar::add-line:horizontal,"
			"ChatbarTextEdit#qteChat QScrollBar::sub-line:horizontal,"
			"ChatbarTextEdit#qteChat QScrollBar::add-page:horizontal,"
			"ChatbarTextEdit#qteChat QScrollBar::sub-page:horizontal,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::add-line:vertical,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::sub-line:vertical,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::add-page:vertical,"
			"QListWidget#qlwPersistentTextChannels QScrollBar::sub-page:vertical,"
			"QTreeView#qtvUsers QScrollBar::add-line:vertical,"
			"QTreeView#qtvUsers QScrollBar::sub-line:vertical,"
			"QTreeView#qtvUsers QScrollBar::add-page:vertical,"
			"QTreeView#qtvUsers QScrollBar::sub-page:vertical {"
			" background: transparent;"
			" border: none;"
			" height: 0px;"
			"}"
			"QListWidget#qtePersistentChatHistory QScrollBar:vertical {"
			" width: 0px;"
			" margin: 0px;"
			"}"
			"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar:vertical {"
			" width: 5px;"
			"}"
			"QListWidget#qtePersistentChatHistory QScrollBar::handle:vertical {"
			" background: transparent;"
			" border-radius: 3px;"
			"}"
			"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar::handle:vertical {"
			" background: %2;"
			"}"
			"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar::handle:vertical:hover,"
			"QListWidget#qtePersistentChatHistory[scrollbarVisible=\"true\"] QScrollBar::handle:vertical:pressed {"
			" background: %3;"
			"}")
			.arg(chrome.dividerColor.name(), qssColor(alphaColor(chrome.textColor, 0.10)),
				 qssColor(alphaColor(chrome.textColor, 0.20))));

	if (m_persistentChatHistory) {
		QPalette historyPalette = m_persistentChatHistory->palette();
		historyPalette.setColor(QPalette::Base, historyColor);
		historyPalette.setColor(QPalette::AlternateBase, historyColor);
		historyPalette.setColor(QPalette::Window, historyColor);
		historyPalette.setColor(QPalette::Text, chrome.textColor);
		historyPalette.setColor(QPalette::Highlight, chrome.selectedColor);
		historyPalette.setColor(QPalette::HighlightedText, chrome.selectedTextColor);
		m_persistentChatHistory->setAutoFillBackground(true);
		m_persistentChatHistory->setPalette(historyPalette);
		m_persistentChatHistory->viewport()->setAutoFillBackground(true);
		m_persistentChatHistory->viewport()->setPalette(historyPalette);
		m_persistentChatHistory->viewport()->setStyleSheet(
			QString::fromLatin1("background-color: %1; border: none; outline: none;").arg(historyColor.name()));
	}

	if (qteLog) {
		QPalette logPalette = qteLog->palette();
		logPalette.setColor(QPalette::Base, historyColor);
		logPalette.setColor(QPalette::AlternateBase, historyColor);
		logPalette.setColor(QPalette::Window, historyColor);
		logPalette.setColor(QPalette::Text, chrome.textColor);
		logPalette.setColor(QPalette::Highlight, chrome.selectedColor);
		logPalette.setColor(QPalette::HighlightedText, chrome.selectedTextColor);
		qteLog->setAutoFillBackground(true);
		qteLog->setPalette(logPalette);
		qteLog->viewport()->setAutoFillBackground(true);
		qteLog->viewport()->setPalette(logPalette);
		qteLog->setStyleSheet(QString::fromLatin1("border-style: solid;"
												  "border-color: %1;"
												  "border-width: 1px 0px 1px 1px;"
												  "border-top-left-radius: 12px;"
												  "border-bottom-left-radius: 12px;"
												  "border-top-right-radius: 0px;"
												  "border-bottom-right-radius: 0px;"
												  "background-color: %2; padding: 0px; outline: none;")
								  .arg(chrome.borderColor.name(), historyColor.name()));
		qteLog->viewport()->setStyleSheet(
			QString::fromLatin1("background-color: %1; border: none; outline: none;").arg(historyColor.name()));
	}
	if (m_persistentChatLogView) {
		QPalette logPalette = m_persistentChatLogView->palette();
		logPalette.setColor(QPalette::Base, historyColor);
		logPalette.setColor(QPalette::AlternateBase, historyColor);
		logPalette.setColor(QPalette::Window, historyColor);
		logPalette.setColor(QPalette::Text, chrome.textColor);
		logPalette.setColor(QPalette::Highlight, chrome.selectedColor);
		logPalette.setColor(QPalette::HighlightedText, chrome.selectedTextColor);
		m_persistentChatLogView->setAutoFillBackground(true);
		m_persistentChatLogView->setPalette(logPalette);
		m_persistentChatLogView->viewport()->setAutoFillBackground(true);
		m_persistentChatLogView->viewport()->setPalette(logPalette);
		m_persistentChatLogView->setStyleSheet(
			QString::fromLatin1("border: none; background-color: %1; padding: 0px; outline: none;")
				.arg(historyColor.name()));
		m_persistentChatLogView->viewport()->setStyleSheet(
			QString::fromLatin1("background-color: %1; border: none; outline: none;").arg(historyColor.name()));
	}

	if (QWidget *logSurface = qdwLog->widget()) {
		QPalette logSurfacePalette = logSurface->palette();
		logSurfacePalette.setColor(QPalette::Window, chrome.railColor);
		logSurface->setAutoFillBackground(true);
		logSurface->setPalette(logSurfacePalette);
		logSurface->setStyleSheet(QString::fromLatin1("QWidget#qwLogSurface { background-color: %1; border: none; }")
									  .arg(chrome.railColor.name()));
	}

	if (qteChat) {
		QPalette inputPalette = qteChat->palette();
		inputPalette.setColor(QPalette::Base, Qt::transparent);
		inputPalette.setColor(QPalette::AlternateBase, Qt::transparent);
		inputPalette.setColor(QPalette::Window, Qt::transparent);
		inputPalette.setColor(QPalette::Text, chrome.textColor);
		inputPalette.setColor(QPalette::Highlight, chrome.selectedColor);
		inputPalette.setColor(QPalette::HighlightedText, chrome.selectedTextColor);
		inputPalette.setColor(QPalette::PlaceholderText, chrome.mutedTextColor);
		qteChat->setAutoFillBackground(false);
		qteChat->setPalette(inputPalette);
		qteChat->viewport()->setAutoFillBackground(false);
		qteChat->viewport()->setPalette(inputPalette);
		qteChat->setStyleSheet(QString());
		qteChat->viewport()->setStyleSheet(QString());
	}

	const QString persistentChatStyle = applyQssArgs(
		QString::fromLatin1(
			"QWidget#qwPersistentChat {"
			" background-color: %2;"
			"}"
			"QFrame#qfPersistentChatHeader {"
			" border: none;"
			" border-bottom: 1px solid %1;"
			" border-radius: 0px;"
			" background-color: %8;"
			"}"
			"QWidget#qwPersistentChatSurface {"
			" border: none;"
			" border-radius: 0px;"
			" background-color: transparent;"
			"}"
			"QWidget#qwPersistentChatPanel {"
			" border: none;"
			" border-radius: 0px;"
			" background-color: %2;"
			"}"
			"QLabel#qlPersistentChatHeaderEyebrow {"
			" color: %5;"
			" font-size: 9px;"
			" font-weight: 600;"
			" letter-spacing: 0.12em;"
			"}"
			"QLabel#qlPersistentChatHeaderSubtitle {"
			" color: %3;"
			" font-size: 10px;"
			" line-height: 1.2em;"
			"}"
			"QLabel#qlPersistentChatHeaderContext {"
			" color: %6;"
			" background-color: %12;"
			" border-radius: 999px;"
			" padding: 1px 7px;"
			" font-size: 9px;"
			"}"
			"QListWidget#qtePersistentChatHistory {"
			" border: none;"
			" border-radius: 0px;"
			" background-color: %2;"
			" padding: 0px;"
			" outline: none;"
			"}"
			"QListWidget#qtePersistentChatHistory:focus {"
			" border: none;"
			" outline: none;"
			"}"
			"QListWidget#qtePersistentChatHistory::item {"
			" border: none;"
			" margin: 0px;"
			" padding: 0px;"
			"}"
			"QFrame#qfPersistentChatComposer {"
			" border: none;"
			" border-top: none;"
			" background-color: %2;"
			" border-bottom-left-radius: 0px;"
			" border-bottom-right-radius: 0px;"
			"}"
			"QWidget#qwPersistentChatComposerInputRow {"
			" background: %4;"
			" border: 1px solid %1;"
			" border-radius: 8px;"
			" padding: 0px;"
			"}"
			"QFrame#qfPersistentChatReply {"
			" border: none;"
			" border-radius: 8px;"
			" background-color: %10;"
			"}"
			"QLabel#qlPersistentChatReplyLabel {"
			" color: %6;"
			" font-weight: 600;"
			"}"
			"QLabel#qlPersistentChatReplySnippet {"
			" color: %3;"
			"}"
			"QToolButton#qtbPersistentChatAttach {"
			" border: none;"
			" border-radius: 6px;"
			" background-color: transparent;"
			" min-width: 22px;"
			" min-height: 22px;"
			" max-width: 22px;"
			" max-height: 22px;"
			" padding: 0px;"
			"}"
			"QToolButton#qtbPersistentChatAttach:hover {"
			" background-color: %12;"
			"}"
			"QToolButton#qtbPersistentChatAttach:disabled {"
			" background-color: transparent;"
			"}"
			"QToolButton#qtbPersistentChatSend {"
			" border: none;"
			" border-radius: 6px;"
			" background-color: transparent;"
			" min-width: 22px;"
			" min-height: 22px;"
			" max-width: 22px;"
			" max-height: 22px;"
			" padding: 0px;"
			"}"
			"QToolButton#qtbPersistentChatSend:hover {"
			" background-color: %12;"
			"}"
			"QToolButton#qtbPersistentChatSend:disabled {"
			" background-color: transparent;"
			"}"
			"QFrame#qfPersistentChatBanner {"
			" border: 1px solid %1;"
			" border-radius: 18px;"
			" background-color: %8;"
			"}"
			"QLabel#qlPersistentChatBannerEyebrow {"
			" color: %5;"
			" letter-spacing: 0.12em;"
			"}"
			"QLabel#qlPersistentChatBannerTitle {"
			" color: %6;"
			" font-weight: 700;"
			"}"
			"QLabel#qlPersistentChatBannerBody {"
			" color: %3;"
			" line-height: 1.45em;"
			"}"
			"QWidget#qwPersistentChatBannerHints {"
			" background: transparent;"
			" border: none;"
			"}"
			"QLabel#qlPersistentChatBannerHint {"
			" color: %6;"
			" background-color: %12;"
			" border-radius: 999px;"
			" padding: 4px 10px;"
			"}"
			"QLabel#qlPersistentChatStatusPill,"
			"QLabel#qlPersistentChatLoadingPill,"
			"QLabel#qlPersistentChatInfoPill,"
			"QLabel#qlPersistentChatUnreadPill,"
			"QLabel#qlPersistentChatAggregateNotice,"
			"QLabel#qlPersistentChatEmptyPill,"
			"QPushButton#qpbPersistentChatLoadOlder {"
			" border: none;"
			" border-radius: 999px;"
			" background: %12;"
			" color: %3;"
			" padding: 5px 12px;"
			"}"
			"QPushButton#qpbPersistentChatLoadOlder:hover {"
			" color: %6;"
			"}"
			"QWidget#qwPersistentChatMessageGroup,"
			"QWidget#qwPersistentChatMessageColumn,"
			"QWidget#qwPersistentChatGroupHeader,"
			"QWidget#qwPersistentChatDateDivider,"
			"QWidget#qwPersistentChatLeadingSpacer {"
			" background: transparent;"
			" border: none;"
			"}"
			"QWidget#qwPersistentChatMessageGroup[selfAuthored=\"true\"] {"
			" background-color: transparent;"
			" border-radius: 0px;"
			"}"
			"QWidget#qwPersistentChatMessageGroup[rowActive=\"true\"] {"
			" background-color: %15;"
			" border-radius: 10px;"
			"}"
			"QWidget#qwPersistentChatMessageGroup[selfAuthored=\"true\"][rowActive=\"true\"] {"
			" background-color: %15;"
			" border-radius: 10px;"
			"}"
			"QWidget#qwPersistentChatMessageGroup[persistentChatSystem=\"true\"],"
			"QWidget#qwPersistentChatMessageGroup[persistentChatSystem=\"true\"][rowActive=\"true\"] {"
			" background: transparent;"
			"}"
			"QWidget#qwPersistentChatMessageGroup[compactTranscript=\"true\"],"
			"QWidget#qwPersistentChatMessageGroup[compactTranscript=\"true\"][rowActive=\"true\"] {"
			" background: transparent;"
			" border-radius: 0px;"
			"}"
			"QFrame#qfPersistentChatDateDividerLine {"
			" background-color: %1;"
			" border: none;"
			"}"
			"QFrame#qfPersistentChatDateDividerLine[compactTranscript=\"true\"] {"
			" background-color: %14;"
			"}"
			"QLabel#qlPersistentChatDateDividerText {"
			" color: %3;"
			" font-size: 0.72em;"
			" font-weight: 600;"
			" letter-spacing: 0.08em;"
			"}"
			"QLabel#qlPersistentChatDateDividerText[compactTranscript=\"true\"] {"
			" color: %11;"
			" font-size: 0.86em;"
			" font-weight: 700;"
			" letter-spacing: 0.14em;"
			"}"
			"QFrame#qfPersistentChatAvatarFrame {"
			" border: 1px solid %14;"
			" border-radius: 11px;"
			" background-color: %13;"
			"}"
			"QLabel#qlPersistentChatAvatarFallback {"
			" border: none;"
			" border-radius: 11px;"
			" background-color: transparent;"
			" color: %6;"
			" font-weight: 700;"
			"}"
			"QLabel#qlPersistentChatAvatar {"
			" background: transparent;"
			"}"
			"QLabel#qlPersistentChatGroupActor {"
			" color: %6;"
			" font-weight: 600;"
			" font-size: 0.80em;"
			"}"
			"QLabel#qlPersistentChatGroupTime {"
			" color: %3;"
			" font-size: 0.66em;"
			"}"
			"QToolButton#qtbPersistentChatGroupScope {"
			" border: none;"
			" background: transparent;"
			" color: %3;"
			" padding: 1px 5px;"
			"}"
			"QToolButton#qtbPersistentChatGroupScope:hover {"
			" color: %6;"
			"}"
			"QWidget#qwPersistentChatBubbleActions {"
			" background: %4;"
			" border: 1px solid %1;"
			" border-radius: 3px;"
			"}"
			"QToolButton#qtbPersistentChatBubbleAction {"
			" border: none;"
			" background: transparent;"
			" color: %3;"
			" min-width: 22px;"
			" min-height: 22px;"
			" max-width: 22px;"
			" max-height: 22px;"
			" padding: 0px;"
			"}"
			"QToolButton#qtbPersistentChatBubbleAction:hover {"
			" background: %1;"
			" color: %6;"
			"}"
			"QFrame#qfPersistentChatBubble {"
			" border: 1px solid %14;"
			" border-top-left-radius: 10px;"
			" border-top-right-radius: 10px;"
			" border-bottom-left-radius: 10px;"
			" border-bottom-right-radius: 10px;"
			" background-color: %7;"
			"}"
			"QFrame#qfPersistentChatBubble[selfAuthored=\"true\"] {"
			" border-color: %6;"
			" border-top-left-radius: 10px;"
			" border-top-right-radius: 10px;"
			" border-bottom-left-radius: 10px;"
			" border-bottom-right-radius: 10px;"
			" background-color: %9;"
			"}"
			"QFrame#qfPersistentChatBubble[bubbleActive=\"true\"] {"
			" border-color: %11;"
			"}"
			"QFrame#qfPersistentChatBubble[selfAuthored=\"true\"][bubbleActive=\"true\"] {"
			" border-color: %6;"
			"}"
			"QFrame#qfPersistentChatBubble[persistentChatSystem=\"true\"] {"
			" border: none;"
			" background: transparent;"
			"}"
			"QFrame#qfPersistentChatBubble[compactTranscript=\"true\"],"
			"QFrame#qfPersistentChatBubble[compactTranscript=\"true\"][selfAuthored=\"true\"],"
			"QFrame#qfPersistentChatBubble[compactTranscript=\"true\"][bubbleActive=\"true\"],"
			"QFrame#qfPersistentChatBubble[compactTranscript=\"true\"][selfAuthored=\"true\"][bubbleActive=\"true\"] {"
			" border: none;"
			" border-radius: 0px;"
			" background: transparent;"
			"}"
			"QFrame#qfPersistentChatBubbleQuote {"
			" border: 1px solid %14;"
			" border-left: 2px solid %13;"
			" border-radius: 9px;"
			" background-color: %10;"
			"}"
			"QFrame#qfPersistentChatBubbleQuote[compactTranscript=\"true\"] {"
			" border: none;"
			" border-left: 2px solid %13;"
			" border-radius: 0px;"
			" background: transparent;"
			"}"
			"QLabel#qlPersistentChatBubbleQuoteActor {"
			" color: %6;"
			"}"
			"QLabel#qlPersistentChatBubbleQuoteSnippet {"
			" color: %3;"
			"}"
			"ChatbarTextEdit#qteChat {"
			" border: none;"
			" border-radius: 0px;"
			" background-color: transparent;"
			" color: %6;"
			" min-height: 26px;"
			" max-height: 72px;"
			" padding: 3px 0px;"
			"}"
			"ChatbarTextEdit#qteChat:focus {"
			" border: none;"
			"}"
			"ChatbarTextEdit#qteChat > QWidget {"
			" border: none;"
			" border-radius: 0px;"
			" background-color: transparent;"
			"}"
			"QLabel#qlPersistentChatHeaderTitle {"
			" color: %6;"
			" font-size: 12px;"
			" font-weight: 600;"
			"}"),
		QStringList{ qssColor(chrome.borderColor), qssColor(historyColor), qssColor(chrome.mutedTextColor),
					 qssColor(inputColor), qssColor(chrome.eyebrowColor), qssColor(chrome.textColor),
					 qssColor(peerBubbleColor), qssColor(headerSurfaceColor), qssColor(selfBubbleColor),
					 qssColor(quoteColor), qssColor(bubbleActiveBorderColor), qssColor(pillColor),
					 qssColor(avatarBadgeColor), qssColor(bubbleBorderColor), qssColor(selfTintColor) });
	m_persistentChatContainer->setStyleSheet(persistentChatStyle);
	if (m_persistentChatHistory) {
		m_persistentChatHistory->setStyleSheet(persistentChatStyle);
		if (QWidget *viewport = m_persistentChatHistory->viewport()) {
			viewport->setStyleSheet(persistentChatStyle);
		}
		if (m_persistentChatHistoryDelegate) {
			m_persistentChatHistoryDelegate->clearCache();
		}
		m_persistentChatHistory->stabilizeVisibleContent();
	}
}

void MainWindow::refreshTextDocumentStylesheets() {
	const QString stylesheet = qApp->styleSheet();

	if (qteLog && qteLog->document()) {
		qteLog->document()->setDefaultStyleSheet(stylesheet);
	}

	if (qteChat && qteChat->document()) {
		qteChat->document()->setDefaultStyleSheet(stylesheet);
	}
}

void MainWindow::setPersistentChatWelcomeText(const QString &message) {
	if (m_persistentChatWelcomeText != message) {
		m_persistentChatMotdExpanded = false;
	}
	m_persistentChatWelcomeText = message;
	updatePersistentChatWelcome();
}

void MainWindow::updatePersistentChatWelcome() {
	if (!usesModernShell() || !m_persistentChatHeaderFrame) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}

	updatePersistentChatChrome(currentPersistentChatTarget());
}

void MainWindow::setPersistentChatReplyTarget(const std::optional< MumbleProto::ChatMessage > &message) {
	if (!m_persistentChatReplyFrame || !m_persistentChatReplyLabel || !m_persistentChatReplySnippet) {
		m_pendingPersistentChatReply = message;
		return;
	}

	if (!message) {
		clearPersistentChatReplyTarget(true);
		return;
	}

	m_pendingPersistentChatReply = message;
	m_persistentChatReplyLabel->setText(tr("Replying to %1").arg(persistentChatActorLabel(*message).toHtmlEscaped()));
	m_persistentChatReplySnippet->setText(
		persistentChatMessageTextSnippet(persistentChatMessageSourceText(*message)).toHtmlEscaped());
	m_persistentChatReplyFrame->show();
	updateChatBar();
}

void MainWindow::clearPersistentChatReplyTarget(bool refreshChatBar) {
	m_pendingPersistentChatReply.reset();

	if (m_persistentChatReplyFrame) {
		m_persistentChatReplyFrame->hide();
	}

	if (refreshChatBar) {
		updateChatBar();
	}
}

void MainWindow::markPersistentChatAvailable(bool refreshUi) {
	if (m_hasPersistentChatSupport) {
		return;
	}

	m_hasPersistentChatSupport = true;
	if (!refreshUi) {
		return;
	}

	rebuildPersistentChatChannelList();
	updateChatBar();
}

QString MainWindow::persistentChatScopeLabel(MumbleProto::ChatScope scope, unsigned int scopeID) const {
	switch (scope) {
		case MumbleProto::Channel: {
			Channel *channel = Channel::get(scopeID);
			if (channel) {
				return Log::formatChannel(channel);
			}

			return tr("Channel %1").arg(scopeID).toHtmlEscaped();
		}
		case MumbleProto::ServerGlobal:
			return tr("Server-wide chat").toHtmlEscaped();
		case MumbleProto::Aggregate:
			return tr("Combined activity").toHtmlEscaped();
		case MumbleProto::TextChannel: {
			const auto it = m_persistentTextChannels.constFind(scopeID);
			if (it != m_persistentTextChannels.cend()) {
				return tr("#%1").arg(it->name).toHtmlEscaped();
			}

			return tr("#text-%1").arg(scopeID).toHtmlEscaped();
		}
	}

	return tr("Unknown chat").toHtmlEscaped();
}

void MainWindow::rebuildPersistentChatChannelList() {
	if (!m_persistentChatChannelList) {
		return;
	}

	if (!usesModernShell()) {
		// Native layouts resolve chat targets through the legacy dock/tree path and never surface the
		// hidden modern-shell conversation list. Leaving the widget untouched here avoids Qt item/view
		// teardown on an off-screen list during connect/disconnect churn.
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		clearPersistentChatChannelSelection();
		return;
	}

	int previousScopeValue       = static_cast< int >(MumbleProto::TextChannel);
	unsigned int previousScopeID = m_defaultPersistentTextChannelID;
	if (m_persistentChatSelectedScopeValue.has_value()) {
		previousScopeValue = *m_persistentChatSelectedScopeValue;
		previousScopeID    = m_persistentChatSelectedScopeID;
	} else if (const QListWidgetItem *currentItem = persistentChatChannelListCurrentItem(); currentItem) {
		previousScopeValue = currentItem->data(PersistentChatScopeRole).toInt();
		previousScopeID    = currentItem->data(PersistentChatScopeIDRole).toUInt();
	}

	const bool oldSignalState = m_persistentChatChannelList->blockSignals(true);
	clearPersistentChatChannelSelection();
	m_persistentChatChannelList->clear();

	if (!hasPersistentChatCapabilities()) {
		m_persistentChatChannelList->blockSignals(oldSignalState);
		updatePersistentChatChrome(currentPersistentChatTarget());
		queueModernShellSnapshotSync();
		return;
	}

	QListWidgetItem *serverLogItem = new QListWidgetItem(m_persistentChatChannelList);
	serverLogItem->setData(PersistentChatScopeRole, LocalServerLogScope);
	serverLogItem->setData(PersistentChatScopeIDRole, 0U);
	serverLogItem->setToolTip(tr("Server log, connection status, notices, and client diagnostics."));

	QList< Channel * > compatChannels;
	const auto appendCompatChannel = [&compatChannels](Channel *channel) {
		if (!channel || compatChannels.contains(channel)) {
			return;
		}

		compatChannels.push_back(channel);
	};
	appendCompatChannel(currentVoiceChannel());
	if (m_persistentChatTargetUsesVoiceTree) {
		appendCompatChannel(selectedVoiceTreeChannel());
	}

	for (Channel *channel : compatChannels) {
		QListWidgetItem *item = new QListWidgetItem(m_persistentChatChannelList);
		item->setData(PersistentChatScopeRole, static_cast< int >(MumbleProto::Channel));
		item->setData(PersistentChatScopeIDRole, static_cast< unsigned int >(channel->iId));
		item->setToolTip(tr("Legacy-compatible room chat for %1.").arg(persistentTextAclChannelLabel(channel)));
	}

	QList< PersistentTextChannel > textChannels = m_persistentTextChannels.values();
	std::sort(textChannels.begin(), textChannels.end(),
			  [](const PersistentTextChannel &lhs, const PersistentTextChannel &rhs) {
				  if (lhs.position != rhs.position) {
					  return lhs.position < rhs.position;
				  }
				  if (lhs.name != rhs.name) {
					  return lhs.name.localeAwareCompare(rhs.name) < 0;
				  }

				  return lhs.textChannelID < rhs.textChannelID;
			  });

	for (const PersistentTextChannel &textChannel : textChannels) {
		QListWidgetItem *item = new QListWidgetItem(m_persistentChatChannelList);
		item->setData(PersistentChatScopeRole, static_cast< int >(MumbleProto::TextChannel));
		item->setData(PersistentChatScopeIDRole, textChannel.textChannelID);
		item->setToolTip(textChannel.description.isEmpty() ? tr("Persistent text channel") : textChannel.description);
	}

	if (previousScopeValue == LocalDirectMessageScope && previousScopeID != 0
		&& previousScopeID != Global::get().uiSession) {
		const ClientUser *user = ClientUser::get(previousScopeID);
		if (user) {
			QListWidgetItem *item = new QListWidgetItem(m_persistentChatChannelList);
			item->setData(PersistentChatScopeRole, LocalDirectMessageScope);
			item->setData(PersistentChatScopeIDRole, previousScopeID);
			item->setToolTip(tr("Direct message with %1.").arg(user->qsName));
		}
	}

	updatePersistentChatScopeSelectorLabels();

	QListWidgetItem *selectionItem = nullptr;
	for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
		QListWidgetItem *candidate = m_persistentChatChannelList->item(i);
		if (!candidate) {
			continue;
		}

		const int candidateScopeValue       = candidate->data(PersistentChatScopeRole).toInt();
		const unsigned int candidateScopeID = candidate->data(PersistentChatScopeIDRole).toUInt();
		if (candidateScopeValue == previousScopeValue && candidateScopeID == previousScopeID) {
			selectionItem = candidate;
			break;
		}
	}

	if (!selectionItem && previousScopeValue == static_cast< int >(MumbleProto::Channel)) {
		for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
			QListWidgetItem *candidate = m_persistentChatChannelList->item(i);
			if (!candidate) {
				continue;
			}

			if (candidate->data(PersistentChatScopeRole).toInt() == static_cast< int >(MumbleProto::Channel)) {
				selectionItem = candidate;
				break;
			}
		}
	}

	if (!selectionItem && m_persistentTextChannels.contains(m_defaultPersistentTextChannelID)) {
		for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
			QListWidgetItem *candidate = m_persistentChatChannelList->item(i);
			if (!candidate) {
				continue;
			}

			const int candidateScopeValue = candidate->data(PersistentChatScopeRole).toInt();
			if (candidateScopeValue == static_cast< int >(MumbleProto::TextChannel)
				&& candidate->data(PersistentChatScopeIDRole).toUInt() == m_defaultPersistentTextChannelID) {
				selectionItem = candidate;
				break;
			}
		}
	}

	if (!selectionItem) {
		for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
			QListWidgetItem *candidate = m_persistentChatChannelList->item(i);
			if (!candidate) {
				continue;
			}

			if (candidate->data(PersistentChatScopeRole).toInt() == static_cast< int >(MumbleProto::TextChannel)) {
				selectionItem = candidate;
				break;
			}
		}
	}

	if (!selectionItem && m_persistentChatChannelList->count() > 1) {
		selectionItem = m_persistentChatChannelList->item(1);
	}
	if (!selectionItem && m_persistentChatChannelList->count() > 0) {
		selectionItem = m_persistentChatChannelList->item(0);
	}

	const bool canApplySelection =
		usesModernShell() && Global::get().uiSession != 0 && Global::get().sh && Global::get().sh->isRunning();
	if (selectionItem && canApplySelection) {
		cachePersistentChatChannelSelection(selectionItem);
		m_persistentChatChannelList->setCurrentItem(selectionItem);
	} else if (selectionItem) {
		cachePersistentChatChannelSelection(selectionItem);
	}
	if (m_persistentChatTargetUsesVoiceTree) {
		m_persistentChatChannelList->clearSelection();
	}
	updatePersistentChatChannelListHeight();
	m_persistentChatChannelList->blockSignals(oldSignalState);
	updatePersistentChatChrome(currentPersistentChatTarget());
	queueModernShellSnapshotSync();
}

void MainWindow::handlePersistentTextChannelSync(const MumbleProto::TextChannelSync &msg) {
	const bool hiddenLegacyPersistentChatSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();
	const bool hadExistingTextChannels            = !m_persistentTextChannels.isEmpty();
	m_persistentTextChannels.clear();
	m_defaultPersistentTextChannelID = msg.has_default_text_channel_id() ? msg.default_text_channel_id() : 0;

	for (const MumbleProto::TextChannelInfo &protoChannel : msg.channels()) {
		if (!protoChannel.has_text_channel_id() || !protoChannel.has_name()) {
			continue;
		}

		PersistentTextChannel textChannel;
		textChannel.textChannelID = protoChannel.text_channel_id();
		textChannel.aclChannelID  = protoChannel.has_acl_channel_id() ? protoChannel.acl_channel_id() : 0;
		textChannel.position      = protoChannel.has_position() ? protoChannel.position() : 0;
		textChannel.name          = u8(protoChannel.name());
		textChannel.description   = protoChannel.has_description() ? u8(protoChannel.description()) : QString();

		m_persistentTextChannels.insert(textChannel.textChannelID, textChannel);
	}

	if (hiddenLegacyPersistentChatSafeMode) {
		queueModernShellSnapshotSync();
		updateChatBar();
		return;
	}

	rebuildPersistentChatChannelList();
	if (!hadExistingTextChannels && !m_persistentTextChannels.isEmpty()) {
		unsigned int preferredTextChannelID = 0;
		if (m_persistentTextChannels.contains(m_defaultPersistentTextChannelID)) {
			preferredTextChannelID = m_defaultPersistentTextChannelID;
		} else {
			QList< PersistentTextChannel > textChannels = m_persistentTextChannels.values();
			std::sort(textChannels.begin(), textChannels.end(),
					  [](const PersistentTextChannel &lhs, const PersistentTextChannel &rhs) {
						  if (lhs.position != rhs.position) {
							  return lhs.position < rhs.position;
						  }
						  if (lhs.name != rhs.name) {
							  return lhs.name.localeAwareCompare(rhs.name) < 0;
						  }

						  return lhs.textChannelID < rhs.textChannelID;
					  });
			if (!textChannels.isEmpty()) {
				preferredTextChannelID = textChannels.front().textChannelID;
			}
		}

		if (usesModernShell() && preferredTextChannelID != 0) {
			navigateToPersistentChatScope(MumbleProto::TextChannel, preferredTextChannelID);
		}
	}
	updatePersistentTextChannelControls();
	updateChatBar();
}

void MainWindow::setPersistentChatTargetUsesVoiceTree(bool useVoiceTree) {
	if (!usesModernShell()) {
		m_persistentChatTargetUsesVoiceTree = useVoiceTree;
		return;
	}

	if (!m_persistentChatChannelList) {
		m_persistentChatTargetUsesVoiceTree = useVoiceTree;
		return;
	}

	if (m_persistentChatTargetUsesVoiceTree == useVoiceTree) {
		return;
	}

	m_persistentChatTargetUsesVoiceTree = useVoiceTree;

	const QSignalBlocker signalBlocker(m_persistentChatChannelList);
	if (useVoiceTree) {
		m_persistentChatChannelList->clearSelection();
	}

	m_persistentChatChannelList->viewport()->update();
}

Channel *MainWindow::currentVoiceChannel() const {
	if (const ClientUser *self = ClientUser::get(Global::get().uiSession); self) {
		return self->cChannel;
	}

	return nullptr;
}

Channel *MainWindow::selectedVoiceTreeChannel() const {
	if (!pmModel) {
		return currentVoiceChannel();
	}

	Channel *selectedChannel = pmModel->getSelectedChannel();
	if (!selectedChannel && qtvUsers) {
		const QModelIndex currentIndex = qtvUsers->currentIndex();
		selectedChannel                = pmModel->getChannel(currentIndex);
		if (!selectedChannel) {
			if (ClientUser *selectedUser = pmModel->getUser(currentIndex); selectedUser) {
				selectedChannel = selectedUser->cChannel;
			}
		}
	}
	if (!selectedChannel) {
		selectedChannel = currentVoiceChannel();
	}

	return selectedChannel;
}

void MainWindow::focusPersistentChatVoiceChannel(Channel *channel) {
	if (!channel || !pmModel || !qtvUsers) {
		return;
	}

	const QModelIndex channelIndex = pmModel->index(channel);
	if (channelIndex.isValid()) {
		qtvUsers->setCurrentIndex(channelIndex);
		qtvUsers->scrollTo(channelIndex);
	}

	setPersistentChatTargetUsesVoiceTree(true);
	rebuildPersistentChatChannelList();
	setPersistentChatTargetUsesVoiceTree(false);
	navigateToPersistentChatScope(MumbleProto::Channel, channel->iId);
	updateServerNavigatorChrome();
}

void MainWindow::refreshUserPresenceStats() {
	if (!Global::get().sh || !Global::get().sh->isRunning() || Global::get().uiSession == 0) {
		return;
	}

	const bool hiddenLegacyUserModelSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();
	if (hiddenLegacyUserModelSafeMode) {
		if (!m_userIdleSeconds.isEmpty()) {
			m_userIdleSeconds.clear();
		}
		return;
	}

	if (Global::get().s.iPresenceIdleTimeoutMinutes <= 0) {
		if (!m_userIdleSeconds.isEmpty()) {
			m_userIdleSeconds.clear();
			if (pmModel) {
				pmModel->forceVisualUpdate();
			}
		}
		return;
	}

	QList< unsigned int > sessions;
	{
		QReadLocker locker(&ClientUser::c_qrwlUsers);
		sessions.reserve(ClientUser::c_qmUsers.size());
		for (auto it = ClientUser::c_qmUsers.cbegin(); it != ClientUser::c_qmUsers.cend(); ++it) {
			sessions << it.key();
		}
	}

	for (unsigned int session : sessions) {
		Global::get().sh->requestUserStats(session, true);
	}
}

std::optional< unsigned int > MainWindow::userIdleSeconds(unsigned int session) const {
	const auto it = m_userIdleSeconds.constFind(session);
	if (it == m_userIdleSeconds.cend()) {
		return std::nullopt;
	}

	return *it;
}

bool MainWindow::isUserIdle(unsigned int session) const {
	if (Global::get().s.iPresenceIdleTimeoutMinutes <= 0) {
		return false;
	}

	const std::optional< unsigned int > idleSeconds = userIdleSeconds(session);
	if (!idleSeconds) {
		return false;
	}

	return *idleSeconds >= static_cast< unsigned int >(Global::get().s.iPresenceIdleTimeoutMinutes * 60);
}

bool MainWindow::isServerNavigatorCompactHeight() const {
	if (!m_serverNavigatorContentFrame) {
		return false;
	}

	const QRect contentRect = m_serverNavigatorContentFrame->contentsRect();
	return (contentRect.height() > 0 && contentRect.height() < 560)
		   || (contentRect.width() > 0 && contentRect.width() < 272);
}

void MainWindow::updateServerNavigatorVoiceTreeHeight() {
	if (!qtvUsers || !qtvUsers->model()) {
		return;
	}

	const int visibleRows = std::max(countVisibleUserTreeRows(qtvUsers), 1);
	int rowHeight         = qtvUsers->sizeHintForRow(0);
	if (rowHeight <= 0) {
		rowHeight = std::max(qtvUsers->fontMetrics().height() + 10, 24);
	}

	const int visibleRowsBeforeScroll = std::min(visibleRows, isServerNavigatorCompactHeight() ? 3 : 4);
	const int minimumHeight           = (visibleRowsBeforeScroll * rowHeight) + (qtvUsers->frameWidth() * 2);

	qtvUsers->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	qtvUsers->setMinimumHeight(minimumHeight);
	qtvUsers->setMaximumHeight(QWIDGETSIZE_MAX);
	qtvUsers->updateGeometry();
}

void MainWindow::updatePersistentChatChannelListHeight() {
	if (!m_persistentChatChannelList) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}

	const int itemCount = m_persistentChatChannelList->count();
	if (itemCount <= 0) {
		m_persistentChatChannelList->setMinimumHeight(0);
		m_persistentChatChannelList->setMaximumHeight(0);
		m_persistentChatChannelList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		return;
	}

	int rowHeight = m_persistentChatChannelList->sizeHintForRow(0);
	if (rowHeight <= 0) {
		rowHeight = std::max(m_persistentChatChannelList->fontMetrics().height() + 10, 24);
	}

	const int desiredHeight = (itemCount * rowHeight)
							  + std::max(0, itemCount - 1) * m_persistentChatChannelList->spacing()
							  + (m_persistentChatChannelList->frameWidth() * 2);
	const int visibleItemsBeforeScroll = std::min(itemCount, isServerNavigatorCompactHeight() ? 2 : 3);
	const int minimumHeight            = (visibleItemsBeforeScroll * rowHeight)
							  + std::max(0, visibleItemsBeforeScroll - 1) * m_persistentChatChannelList->spacing()
							  + (m_persistentChatChannelList->frameWidth() * 2);

	m_persistentChatChannelList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_persistentChatChannelList->setMinimumHeight(std::min(desiredHeight, minimumHeight));
	m_persistentChatChannelList->setMaximumHeight(QWIDGETSIZE_MAX);
	m_persistentChatChannelList->updateGeometry();
}

void MainWindow::updatePersistentChatScopeSelectorLabels() {
	mumble::chatperf::ScopedDuration trace("chat.scope_selector.update");
	if (!m_persistentChatChannelList || !usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}

	mumble::chatperf::recordValue("chat.scope_selector.items", m_persistentChatChannelList->count());
	for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
		QListWidgetItem *item = m_persistentChatChannelList->item(i);
		if (!item) {
			continue;
		}

		const int scopeValue       = item->data(PersistentChatScopeRole).toInt();
		const unsigned int scopeID = item->data(PersistentChatScopeIDRole).toUInt();

		QString scopeLabel;
		std::size_t unreadCount = 0;
		if (scopeValue == LocalServerLogScope) {
			scopeLabel = tr("Activity");
		} else if (scopeValue == LocalDirectMessageScope) {
			if (const ClientUser *user = ClientUser::get(scopeID); user) {
				scopeLabel = user->qsName;
			} else {
				scopeLabel = tr("Direct message");
			}
		} else {
			const MumbleProto::ChatScope scope = static_cast< MumbleProto::ChatScope >(scopeValue);
			switch (scope) {
				case MumbleProto::Aggregate:
					scopeLabel = tr("Combined activity");
					break;
				case MumbleProto::ServerGlobal:
					scopeLabel = tr("Server-wide chat");
					break;
				case MumbleProto::TextChannel: {
					const auto it = m_persistentTextChannels.constFind(scopeID);
					scopeLabel =
						it == m_persistentTextChannels.cend() ? tr("#text-%1").arg(scopeID) : tr("#%1").arg(it->name);
					break;
				}
				case MumbleProto::Channel: {
					Channel *channel = Channel::get(scopeID);
					scopeLabel       = channel ? channel->qsName : tr("Channel %1").arg(scopeID);
					break;
				}
				default:
					scopeLabel = persistentChatScopeLabel(scope, scopeID);
					break;
			}
			unreadCount = scope == MumbleProto::Aggregate ? totalCachedPersistentChatUnreadCount()
														  : cachedPersistentChatUnreadCount(scope, scopeID);
		}

		item->setData(PersistentChatLabelRole, scopeLabel);
		item->setData(PersistentChatUnreadRole, static_cast< qulonglong >(unreadCount));
		item->setText(scopeLabel);
	}
}

bool MainWindow::canCreateVoiceRoom(Channel *channel) const {
	if (!channel || !Global::get().sh || !Global::get().sh->isRunning() || Global::get().uiSession == 0) {
		return false;
	}

	ChanACL::Permissions permissions = static_cast< ChanACL::Permissions >(channel->uiPermissions);
	if (!permissions) {
		Global::get().sh->requestChannelPermissions(channel->iId);
		permissions            = channel->iId == Mumble::ROOT_CHANNEL_ID ? Global::get().pPermissions : ChanACL::All;
		channel->uiPermissions = permissions;
	}

	return permissions & (ChanACL::Write | ChanACL::MakeChannel | ChanACL::MakeTempChannel);
}

bool MainWindow::canCreateAnyVoiceRoom() const {
	QList< Channel * > channels = Channel::c_qhChannels.values();
	std::sort(channels.begin(), channels.end(), [](const Channel *lhs, const Channel *rhs) {
		if (lhs == rhs) {
			return false;
		}
		if (!lhs || !rhs) {
			return lhs != nullptr;
		}
		if (lhs->iId == Mumble::ROOT_CHANNEL_ID || rhs->iId == Mumble::ROOT_CHANNEL_ID) {
			return lhs->iId == Mumble::ROOT_CHANNEL_ID;
		}
		return persistentTextAclChannelLabel(lhs).localeAwareCompare(persistentTextAclChannelLabel(rhs)) < 0;
	});

	for (Channel *channel : channels) {
		if (canCreateVoiceRoom(channel)) {
			return true;
		}
	}

	return false;
}

bool MainWindow::voiceRoomCreationForcesTemporary(Channel *channel) const {
	if (!channel) {
		return false;
	}

	ChanACL::Permissions permissions = static_cast< ChanACL::Permissions >(channel->uiPermissions);
	if (!permissions) {
		canCreateVoiceRoom(channel);
		permissions = static_cast< ChanACL::Permissions >(channel->uiPermissions);
	}

	return (permissions & ChanACL::Cached) && !(permissions & (ChanACL::Write | ChanACL::MakeChannel));
}

bool MainWindow::canCreateTextRoom() const {
	return canManagePersistentTextChannels() && hasPersistentChatCapabilities();
}

bool MainWindow::canManagePersistentTextChannels() const {
	return Global::get().sh && Global::get().sh->isRunning() && (Global::get().pPermissions & ChanACL::Write);
}

std::optional< MainWindow::PersistentTextChannel > MainWindow::selectedPersistentTextChannel() const {
	if (!m_persistentChatChannelList || !persistentChatChannelListCurrentItem()) {
		return std::nullopt;
	}

	const QListWidgetItem *item = persistentChatChannelListCurrentItem();
	if (item->data(PersistentChatScopeRole).toInt() != static_cast< int >(MumbleProto::TextChannel)) {
		return std::nullopt;
	}

	const auto it = m_persistentTextChannels.constFind(item->data(PersistentChatScopeIDRole).toUInt());
	if (it == m_persistentTextChannels.cend()) {
		return std::nullopt;
	}

	return *it;
}

void MainWindow::createRoom(RoomCreateType preferredType, Channel *preferredVoiceParent) {
	if (!preferredVoiceParent) {
		preferredVoiceParent = selectedVoiceTreeChannel();
	}
	if (!preferredVoiceParent) {
		preferredVoiceParent = Channel::get(Mumble::ROOT_CHANNEL_ID);
	}

	QList< Channel * > channels = Channel::c_qhChannels.values();
	std::sort(channels.begin(), channels.end(), [](const Channel *lhs, const Channel *rhs) {
		if (lhs == rhs) {
			return false;
		}
		if (!lhs || !rhs) {
			return lhs != nullptr;
		}
		if (lhs->iId == Mumble::ROOT_CHANNEL_ID || rhs->iId == Mumble::ROOT_CHANNEL_ID) {
			return lhs->iId == Mumble::ROOT_CHANNEL_ID;
		}
		return persistentTextAclChannelLabel(lhs).localeAwareCompare(persistentTextAclChannelLabel(rhs)) < 0;
	});

	const bool textAvailable = canCreateTextRoom();

	QDialog dialog(this);
	dialog.setWindowTitle(tr("Create room"));

	QVBoxLayout *layout = new QVBoxLayout(&dialog);

	QFormLayout *formLayout = new QFormLayout();
	layout->addLayout(formLayout);

	QComboBox *typeCombo        = new QComboBox(&dialog);
	QComboBox *voiceParentCombo = new QComboBox(&dialog);
	for (Channel *channel : channels) {
		if (!channel || !canCreateVoiceRoom(channel)) {
			continue;
		}
		voiceParentCombo->addItem(persistentTextAclChannelLabel(channel), channel->iId);
	}

	const bool voiceAvailable = voiceParentCombo->count() > 0;
	if (!voiceAvailable && !textAvailable) {
		return;
	}

	if (voiceAvailable) {
		typeCombo->addItem(tr("Voice room"), static_cast< int >(RoomCreateType::Voice));
	}
	if (textAvailable) {
		typeCombo->addItem(tr("Text room"), static_cast< int >(RoomCreateType::Text));
	}
	const int preferredTypeIndex = typeCombo->findData(static_cast< int >(preferredType));
	typeCombo->setCurrentIndex(preferredTypeIndex >= 0 ? preferredTypeIndex : 0);
	formLayout->addRow(tr("Type"), typeCombo);

	QLineEdit *nameEdit = new QLineEdit(&dialog);
	nameEdit->setPlaceholderText(tr("general"));
	formLayout->addRow(tr("Name"), nameEdit);

	RichTextEditor *descriptionEdit = new RichTextEditor(&dialog);
	descriptionEdit->setMinimumHeight(116);
	descriptionEdit->setText(QString());
	formLayout->addRow(tr("Topic"), descriptionEdit);

	QGroupBox *advancedGroup = new QGroupBox(tr("Advanced"), &dialog);
	advancedGroup->setCheckable(true);
	advancedGroup->setChecked(false);
	QVBoxLayout *advancedGroupLayout = new QVBoxLayout(advancedGroup);
	QFrame *advancedFrame            = new QFrame(advancedGroup);
	QVBoxLayout *advancedLayout      = new QVBoxLayout(advancedFrame);
	advancedLayout->setContentsMargins(0, 0, 0, 0);
	advancedGroupLayout->addWidget(advancedFrame);

	QFrame *voiceAdvancedFrame       = new QFrame(advancedFrame);
	QFormLayout *voiceAdvancedLayout = new QFormLayout(voiceAdvancedFrame);
	voiceAdvancedLayout->addRow(tr("Parent room"), voiceParentCombo);

	QSpinBox *voiceOrderSpin = new QSpinBox(voiceAdvancedFrame);
	voiceOrderSpin->setRange(std::numeric_limits< int >::min(), std::numeric_limits< int >::max());
	voiceOrderSpin->setValue(0);
	voiceAdvancedLayout->addRow(tr("Order"), voiceOrderSpin);

	QCheckBox *voiceTemporaryCheck = new QCheckBox(voiceAdvancedFrame);
	voiceAdvancedLayout->addRow(tr("Temporary"), voiceTemporaryCheck);

	QSpinBox *voiceMaxUsersSpin = new QSpinBox(voiceAdvancedFrame);
	voiceMaxUsersSpin->setRange(0, std::numeric_limits< int >::max());
	voiceMaxUsersSpin->setValue(0);
	voiceMaxUsersSpin->setSpecialValueText(tr("Default server value"));
	QLabel *voiceMaxUsersLabel = new QLabel(tr("Max users"), voiceAdvancedFrame);
	voiceAdvancedLayout->addRow(voiceMaxUsersLabel, voiceMaxUsersSpin);
	const bool supportsVoiceMaxUsers =
		Global::get().sh && Global::get().sh->m_version >= Version::fromComponents(1, 3, 0);
	voiceMaxUsersLabel->setVisible(supportsVoiceMaxUsers);
	voiceMaxUsersSpin->setVisible(supportsVoiceMaxUsers);
	advancedLayout->addWidget(voiceAdvancedFrame);

	QFrame *textAdvancedFrame       = new QFrame(advancedFrame);
	QFormLayout *textAdvancedLayout = new QFormLayout(textAdvancedFrame);
	QComboBox *textVisibilityCombo  = new QComboBox(textAdvancedFrame);
	for (Channel *channel : channels) {
		if (!channel) {
			continue;
		}
		textVisibilityCombo->addItem(persistentTextAclChannelLabel(channel), channel->iId);
	}
	int rootVisibilityIndex = textVisibilityCombo->findData(Mumble::ROOT_CHANNEL_ID);
	if (rootVisibilityIndex < 0) {
		rootVisibilityIndex = 0;
	}
	textVisibilityCombo->setCurrentIndex(rootVisibilityIndex);
	textAdvancedLayout->addRow(tr("Visibility source"), textVisibilityCombo);

	QSpinBox *textOrderSpin = new QSpinBox(textAdvancedFrame);
	textOrderSpin->setRange(0, 9999);
	textOrderSpin->setValue(static_cast< int >(m_persistentTextChannels.size()));
	textAdvancedLayout->addRow(tr("Order"), textOrderSpin);
	advancedLayout->addWidget(textAdvancedFrame);

	layout->addWidget(advancedGroup);

	if (preferredVoiceParent) {
		const int preferredParentIndex = voiceParentCombo->findData(preferredVoiceParent->iId);
		if (preferredParentIndex >= 0) {
			voiceParentCombo->setCurrentIndex(preferredParentIndex);
		}
	}

	auto currentType = [typeCombo]() {
		return typeCombo->currentData().toInt() == static_cast< int >(RoomCreateType::Voice) ? RoomCreateType::Voice
																							 : RoomCreateType::Text;
	};

	auto syncVoicePermissionControls = [this, voiceParentCombo, voiceTemporaryCheck]() {
		Channel *parent           = Channel::get(voiceParentCombo->currentData().toUInt());
		const bool forceTemporary = voiceRoomCreationForcesTemporary(parent);
		voiceTemporaryCheck->setEnabled(!forceTemporary);
		if (forceTemporary) {
			voiceTemporaryCheck->setChecked(true);
		}
	};

	auto syncTypeControls = [advancedGroup, advancedFrame, voiceAdvancedFrame, textAdvancedFrame, nameEdit, currentType,
							 syncVoicePermissionControls]() {
		const bool advancedVisible = advancedGroup->isChecked();
		const bool voiceSelected   = currentType() == RoomCreateType::Voice;
		advancedFrame->setVisible(advancedVisible);
		voiceAdvancedFrame->setVisible(advancedVisible && voiceSelected);
		textAdvancedFrame->setVisible(advancedVisible && !voiceSelected);
		nameEdit->setPlaceholderText(voiceSelected ? QObject::tr("Raid room") : QObject::tr("links"));
		if (voiceSelected) {
			syncVoicePermissionControls();
		}
	};

	connect(typeCombo, qOverload< int >(&QComboBox::currentIndexChanged), &dialog,
			[syncTypeControls](int) { syncTypeControls(); });
	connect(advancedGroup, &QGroupBox::toggled, &dialog, [syncTypeControls](bool) { syncTypeControls(); });
	connect(voiceParentCombo, qOverload< int >(&QComboBox::currentIndexChanged), &dialog,
			[syncVoicePermissionControls](int) { syncVoicePermissionControls(); });
	syncTypeControls();

	QDialogButtonBox *buttons =
		new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	if (QPushButton *createButton = buttons->button(QDialogButtonBox::Ok)) {
		createButton->setText(tr("Create"));
	}
	connect(buttons, &QDialogButtonBox::accepted, &dialog, [&]() {
		if (nameEdit->text().trimmed().isEmpty()) {
			QMessageBox::warning(&dialog, tr("Create room"), tr("A room needs a name."));
			nameEdit->setFocus();
			return;
		}

		if (currentType() == RoomCreateType::Voice) {
			Channel *parent = Channel::get(voiceParentCombo->currentData().toUInt());
			if (!canCreateVoiceRoom(parent)) {
				QMessageBox::warning(&dialog, tr("Create room"), tr("You cannot create a voice room there."));
				return;
			}
		} else if (!canCreateTextRoom()) {
			QMessageBox::warning(&dialog, tr("Create room"), tr("You cannot create text rooms on this server."));
			return;
		}

		dialog.accept();
	});
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	layout->addWidget(buttons);

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	const QString name        = nameEdit->text().trimmed();
	const QString description = descriptionEdit->text().trimmed();
	if (currentType() == RoomCreateType::Voice) {
		Channel *parent = Channel::get(voiceParentCombo->currentData().toUInt());
		if (!parent || !Global::get().sh || !Global::get().sh->isRunning()) {
			return;
		}

		const bool temporary = voiceRoomCreationForcesTemporary(parent) || voiceTemporaryCheck->isChecked();
		const unsigned int maxUsers =
			supportsVoiceMaxUsers ? static_cast< unsigned int >(voiceMaxUsersSpin->value()) : 0;
		Global::get().sh->createChannel(parent->iId, name, description,
										static_cast< unsigned int >(voiceOrderSpin->value()), temporary, maxUsers);
		return;
	}

	if (!Global::get().sh || !Global::get().sh->isRunning()) {
		return;
	}

	Global::get().sh->upsertTextChannel(0, name, description, textVisibilityCombo->currentData().toUInt(),
										static_cast< unsigned int >(textOrderSpin->value()), true);
}

bool MainWindow::promptForPersistentTextChannel(PersistentTextChannel &textChannel, bool isNew) {
	QDialog dialog(this);
	dialog.setWindowTitle(isNew ? tr("Create room") : tr("Edit text room"));

	QVBoxLayout *layout     = new QVBoxLayout(&dialog);
	QFormLayout *formLayout = new QFormLayout();
	layout->addLayout(formLayout);

	QLineEdit *nameEdit = new QLineEdit(textChannel.name, &dialog);
	nameEdit->setPlaceholderText(tr("links"));
	formLayout->addRow(tr("Name"), nameEdit);

	QLineEdit *descriptionEdit = new QLineEdit(textChannel.description, &dialog);
	descriptionEdit->setPlaceholderText(tr("What this room is for"));
	formLayout->addRow(tr("Description"), descriptionEdit);

	QComboBox *aclChannelCombo  = new QComboBox(&dialog);
	QList< Channel * > channels = Channel::c_qhChannels.values();
	std::sort(channels.begin(), channels.end(), [](const Channel *lhs, const Channel *rhs) {
		if (lhs == rhs) {
			return false;
		}
		if (lhs->iId == Mumble::ROOT_CHANNEL_ID || rhs->iId == Mumble::ROOT_CHANNEL_ID) {
			return lhs->iId == Mumble::ROOT_CHANNEL_ID;
		}
		return persistentTextAclChannelLabel(lhs).localeAwareCompare(persistentTextAclChannelLabel(rhs)) < 0;
	});

	int currentAclIndex = -1;
	for (Channel *channel : channels) {
		if (!channel) {
			continue;
		}

		const QString channelLabel = persistentTextAclChannelLabel(channel);
		aclChannelCombo->addItem(channelLabel, channel->iId);
		if (channel->iId == textChannel.aclChannelID) {
			currentAclIndex = aclChannelCombo->count() - 1;
		}
	}

	if (currentAclIndex < 0) {
		currentAclIndex = std::max(0, aclChannelCombo->findData(Mumble::ROOT_CHANNEL_ID));
	}
	aclChannelCombo->setCurrentIndex(currentAclIndex);
	formLayout->addRow(tr("Visibility source"), aclChannelCombo);

	QSpinBox *positionSpin = new QSpinBox(&dialog);
	positionSpin->setRange(0, 9999);
	positionSpin->setValue(static_cast< int >(textChannel.position));
	formLayout->addRow(tr("Order"), positionSpin);

	QDialogButtonBox *buttons =
		new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	layout->addWidget(buttons);

	if (dialog.exec() != QDialog::Accepted) {
		return false;
	}

	const QString name = nameEdit->text().trimmed();
	if (name.isEmpty()) {
		QMessageBox::warning(this, tr("Text room"), tr("A text room needs a name."));
		return false;
	}

	textChannel.name         = name;
	textChannel.description  = descriptionEdit->text().trimmed();
	textChannel.aclChannelID = aclChannelCombo->currentData().toUInt();
	textChannel.position     = static_cast< unsigned int >(positionSpin->value());
	return true;
}

void MainWindow::openServerSettingsDialog() {
	if (!canManagePersistentTextChannels() || !Global::get().sh || !Global::get().sh->isRunning()) {
		return;
	}

	QDialog dialog(this);
	dialog.setWindowTitle(tr("Server settings"));

	QVBoxLayout *layout = new QVBoxLayout(&dialog);
	layout->setSpacing(10);

	QGroupBox *chatGroup     = new QGroupBox(tr("Chat"), &dialog);
	QFormLayout *chatForm    = new QFormLayout(chatGroup);
	QPlainTextEdit *motdEdit = new QPlainTextEdit(m_persistentChatWelcomeText, chatGroup);
	motdEdit->setObjectName(QLatin1String("qpteServerSettingsWelcomeText"));
	motdEdit->setMinimumHeight(92);
	motdEdit->setPlaceholderText(tr("Welcome message shown after joining"));
	chatForm->addRow(tr("Welcome text"), motdEdit);

	QCheckBox *allowHtmlCheck = new QCheckBox(chatGroup);
	allowHtmlCheck->setChecked(Global::get().bAllowHTML);
	chatForm->addRow(tr("Allow HTML"), allowHtmlCheck);

	QCheckBox *persistentGlobalCheck = new QCheckBox(chatGroup);
	persistentGlobalCheck->setChecked(Global::get().bPersistentGlobalChatEnabled);
	chatForm->addRow(tr("Server-wide chat"), persistentGlobalCheck);
	layout->addWidget(chatGroup);

	const int spinMax   = std::numeric_limits< int >::max();
	auto clampSpinValue = [spinMax](unsigned int value) {
		return static_cast< int >(std::min(value, static_cast< unsigned int >(spinMax)));
	};

	QGroupBox *limitsGroup  = new QGroupBox(tr("Limits"), &dialog);
	QFormLayout *limitsForm = new QFormLayout(limitsGroup);

	QSpinBox *bandwidthSpin = new QSpinBox(limitsGroup);
	bandwidthSpin->setRange(0, spinMax);
	bandwidthSpin->setSpecialValueText(tr("Server default"));
	bandwidthSpin->setSuffix(tr(" bit/s"));
	bandwidthSpin->setValue(std::max(Global::get().iMaxBandwidth, 0));
	limitsForm->addRow(tr("Audio bandwidth"), bandwidthSpin);

	QSpinBox *maxUsersSpin = new QSpinBox(limitsGroup);
	maxUsersSpin->setRange(0, spinMax);
	maxUsersSpin->setSpecialValueText(tr("Server default"));
	maxUsersSpin->setValue(clampSpinValue(Global::get().uiMaxUsers));
	limitsForm->addRow(tr("Max users"), maxUsersSpin);

	QSpinBox *textLengthSpin = new QSpinBox(limitsGroup);
	textLengthSpin->setRange(0, spinMax);
	textLengthSpin->setSpecialValueText(tr("Server default"));
	textLengthSpin->setSuffix(tr(" bytes"));
	textLengthSpin->setValue(clampSpinValue(Global::get().uiMessageLength));
	limitsForm->addRow(tr("Text message length"), textLengthSpin);

	QSpinBox *imageLengthSpin = new QSpinBox(limitsGroup);
	imageLengthSpin->setRange(0, spinMax);
	imageLengthSpin->setSpecialValueText(tr("Server default"));
	imageLengthSpin->setSuffix(tr(" bytes"));
	imageLengthSpin->setValue(clampSpinValue(Global::get().uiImageLength));
	limitsForm->addRow(tr("Image message length"), imageLengthSpin);
	layout->addWidget(limitsGroup);

	QGroupBox *screenShareGroup  = new QGroupBox(tr("Screen sharing"), &dialog);
	QFormLayout *screenShareForm = new QFormLayout(screenShareGroup);

	QCheckBox *screenShareEnabledCheck = new QCheckBox(screenShareGroup);
	screenShareEnabledCheck->setChecked(Global::get().bScreenShareEnabled);
	screenShareForm->addRow(tr("Enabled"), screenShareEnabledCheck);

	QCheckBox *screenShareRecordingCheck = new QCheckBox(screenShareGroup);
	screenShareRecordingCheck->setChecked(Global::get().bScreenShareRecordingEnabled);
	screenShareForm->addRow(tr("Recording allowed"), screenShareRecordingCheck);

	QCheckBox *screenShareHelperCheck = new QCheckBox(screenShareGroup);
	screenShareHelperCheck->setChecked(Global::get().bScreenShareHelperRequired);
	screenShareForm->addRow(tr("Helper required"), screenShareHelperCheck);

	QSpinBox *screenShareWidthSpin = new QSpinBox(screenShareGroup);
	screenShareWidthSpin->setRange(0, Mumble::ScreenShare::HARD_MAX_WIDTH);
	screenShareWidthSpin->setSpecialValueText(tr("Server default"));
	screenShareWidthSpin->setSuffix(tr(" px"));
	screenShareWidthSpin->setValue(static_cast< int >(Mumble::ScreenShare::sanitizeLimit(
		Global::get().uiScreenShareMaxWidth, 0, Mumble::ScreenShare::HARD_MAX_WIDTH)));
	screenShareForm->addRow(tr("Max width"), screenShareWidthSpin);

	QSpinBox *screenShareHeightSpin = new QSpinBox(screenShareGroup);
	screenShareHeightSpin->setRange(0, Mumble::ScreenShare::HARD_MAX_HEIGHT);
	screenShareHeightSpin->setSpecialValueText(tr("Server default"));
	screenShareHeightSpin->setSuffix(tr(" px"));
	screenShareHeightSpin->setValue(static_cast< int >(Mumble::ScreenShare::sanitizeLimit(
		Global::get().uiScreenShareMaxHeight, 0, Mumble::ScreenShare::HARD_MAX_HEIGHT)));
	screenShareForm->addRow(tr("Max height"), screenShareHeightSpin);

	QSpinBox *screenShareFpsSpin = new QSpinBox(screenShareGroup);
	screenShareFpsSpin->setRange(0, Mumble::ScreenShare::HARD_MAX_FPS);
	screenShareFpsSpin->setSpecialValueText(tr("Server default"));
	screenShareFpsSpin->setSuffix(tr(" fps"));
	screenShareFpsSpin->setValue(static_cast< int >(
		Mumble::ScreenShare::sanitizeLimit(Global::get().uiScreenShareMaxFps, 0, Mumble::ScreenShare::HARD_MAX_FPS)));
	screenShareForm->addRow(tr("Max frame rate"), screenShareFpsSpin);

	QLineEdit *screenShareRelayEdit = new QLineEdit(Global::get().qsScreenShareRelayUrl, screenShareGroup);
	screenShareRelayEdit->setPlaceholderText(tr("https://relay.example.com"));
	screenShareForm->addRow(tr("Relay URL"), screenShareRelayEdit);

	auto syncScreenShareControls = [screenShareEnabledCheck, screenShareRecordingCheck, screenShareHelperCheck,
									screenShareWidthSpin, screenShareHeightSpin, screenShareFpsSpin,
									screenShareRelayEdit]() {
		const bool enabled = screenShareEnabledCheck->isChecked();
		screenShareRecordingCheck->setEnabled(enabled);
		screenShareHelperCheck->setEnabled(enabled);
		screenShareWidthSpin->setEnabled(enabled);
		screenShareHeightSpin->setEnabled(enabled);
		screenShareFpsSpin->setEnabled(enabled);
		screenShareRelayEdit->setEnabled(enabled);
	};
	connect(screenShareEnabledCheck, &QCheckBox::toggled, &dialog, syncScreenShareControls);
	syncScreenShareControls();
	layout->addWidget(screenShareGroup);

	QDialogButtonBox *buttons =
		new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
	if (QPushButton *applyButton = buttons->button(QDialogButtonBox::Ok)) {
		applyButton->setText(tr("Apply"));
	}
	connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	layout->addWidget(buttons);

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	MumbleProto::ServerConfig config;
	config.set_welcome_text(u8(motdEdit->toPlainText()));
	config.set_max_bandwidth(static_cast< unsigned int >(bandwidthSpin->value()));
	config.set_allow_html(allowHtmlCheck->isChecked());
	config.set_message_length(static_cast< unsigned int >(textLengthSpin->value()));
	config.set_image_message_length(static_cast< unsigned int >(imageLengthSpin->value()));
	config.set_max_users(static_cast< unsigned int >(maxUsersSpin->value()));
	config.set_persistent_global_chat_enabled(persistentGlobalCheck->isChecked());
	config.set_screen_share_enabled(screenShareEnabledCheck->isChecked());
	config.set_screen_share_recording_enabled(screenShareRecordingCheck->isChecked());
	config.set_screen_share_helper_required(screenShareHelperCheck->isChecked());
	config.set_screen_share_max_width(static_cast< unsigned int >(screenShareWidthSpin->value()));
	config.set_screen_share_max_height(static_cast< unsigned int >(screenShareHeightSpin->value()));
	config.set_screen_share_max_fps(static_cast< unsigned int >(screenShareFpsSpin->value()));
	config.set_screen_share_relay_url(u8(screenShareRelayEdit->text().trimmed()));
	Global::get().sh->sendMessage(config);
}

void MainWindow::createPersistentTextChannel() {
	createRoom(RoomCreateType::Text);
}

void MainWindow::editPersistentTextChannel() {
	if (!canManagePersistentTextChannels()) {
		return;
	}

	std::optional< PersistentTextChannel > textChannel = selectedPersistentTextChannel();
	if (!textChannel || textChannel->textChannelID == 0) {
		return;
	}

	PersistentTextChannel updated = *textChannel;
	if (!promptForPersistentTextChannel(updated, false)) {
		return;
	}

	Global::get().sh->upsertTextChannel(updated.textChannelID, updated.name, updated.description, updated.aclChannelID,
										updated.position, false);
}

void MainWindow::removePersistentTextChannel() {
	if (!canManagePersistentTextChannels()) {
		return;
	}

	std::optional< PersistentTextChannel > textChannel = selectedPersistentTextChannel();
	if (!textChannel || textChannel->textChannelID == 0) {
		return;
	}

	const int result =
		QMessageBox::question(this, tr("Delete text room"),
							  tr("Delete text room #%1? Existing history will no longer be visible in this room.")
								  .arg(textChannel->name.toHtmlEscaped()),
							  QMessageBox::Yes, QMessageBox::No);
	if (result != QMessageBox::Yes) {
		return;
	}

	Global::get().sh->removeTextChannel(textChannel->textChannelID);
}

void MainWindow::editPersistentTextChannelACL() {
	std::optional< PersistentTextChannel > textChannel = selectedPersistentTextChannel();
	if (!textChannel) {
		return;
	}

	Channel *channel = Channel::get(textChannel->aclChannelID);
	if (!channel) {
		return;
	}

	cContextChannel = channel;
	on_qaChannelACL_triggered();
	cContextChannel.clear();
}

void MainWindow::setDefaultPersistentTextChannel() {
	if (!canManagePersistentTextChannels() || !Global::get().sh || !Global::get().sh->isRunning()) {
		return;
	}

	std::optional< PersistentTextChannel > textChannel = selectedPersistentTextChannel();
	if (!textChannel || textChannel->textChannelID == 0
		|| textChannel->textChannelID == m_defaultPersistentTextChannelID) {
		return;
	}

	Global::get().sh->setDefaultTextChannel(textChannel->textChannelID);
}

void MainWindow::showPersistentTextChannelContextMenu(const QPoint &position) {
	if (!m_persistentChatChannelList) {
		return;
	}

	QListWidgetItem *menuItem = m_persistentChatChannelList->itemAt(position);
	if (menuItem) {
		cachePersistentChatChannelSelection(menuItem);
		m_persistentChatChannelList->setCurrentItem(menuItem);
	}

	const bool canManage        = canManagePersistentTextChannels();
	const QPoint globalPosition = m_persistentChatChannelList->viewport()->mapToGlobal(position);

	auto selectVoiceTreeChannel = [this](Channel *channel) {
		if (!channel || !pmModel || !qtvUsers) {
			return;
		}

		const QModelIndex channelIndex = pmModel->index(channel);
		if (!channelIndex.isValid()) {
			return;
		}

		qtvUsers->setCurrentIndex(channelIndex);
		qtvUsers->scrollTo(channelIndex);
		rebuildPersistentChatChannelList();
		updateServerNavigatorChrome();
	};

	if (!menuItem) {
		if (!canManage) {
			return;
		}

		QMenu menu(this);
		QAction *createAction = menu.addAction(tr("Create room"));
		if (menu.exec(globalPosition) == createAction) {
			createRoom(RoomCreateType::Text);
		}
		return;
	}

	const int scopeValue = menuItem->data(PersistentChatScopeRole).toInt();
	if (scopeValue == LocalServerLogScope) {
		return;
	}

	const unsigned int scopeID = menuItem->data(PersistentChatScopeIDRole).toUInt();
	QMenu menu(this);

	if (scopeValue == static_cast< int >(MumbleProto::TextChannel)) {
		const auto textChannelIt = m_persistentTextChannels.constFind(scopeID);
		if (textChannelIt == m_persistentTextChannels.cend()) {
			return;
		}

		Channel *linkedChannel    = Channel::get(textChannelIt->aclChannelID);
		QAction *openAction       = menu.addAction(tr("Open"));
		QAction *createAction     = nullptr;
		QAction *setDefaultAction = nullptr;
		if (canManage) {
			createAction     = menu.addAction(tr("Create room"));
			setDefaultAction = menu.addAction(tr("Set as default"));
			setDefaultAction->setEnabled(scopeID != m_defaultPersistentTextChannelID);
		}
		QAction *goToVoiceRoomAction = menu.addAction(tr("Go to visibility source"));
		goToVoiceRoomAction->setEnabled(linkedChannel != nullptr);
		QAction *editAction    = nullptr;
		QAction *editAclAction = nullptr;
		QAction *deleteAction  = nullptr;
		if (canManage) {
			editAction    = menu.addAction(tr("Edit text room"));
			editAclAction = menu.addAction(tr("Edit visibility rules"));
			deleteAction  = menu.addAction(tr("Delete text room"));
		}

		QAction *chosenAction = menu.exec(globalPosition);
		if (chosenAction == openAction) {
			navigateToPersistentChatScope(MumbleProto::TextChannel, scopeID);
		} else if (chosenAction == createAction) {
			createRoom(RoomCreateType::Text);
		} else if (chosenAction == setDefaultAction) {
			setDefaultPersistentTextChannel();
		} else if (chosenAction == goToVoiceRoomAction) {
			selectVoiceTreeChannel(linkedChannel);
		} else if (chosenAction == editAction) {
			editPersistentTextChannel();
		} else if (chosenAction == editAclAction) {
			editPersistentTextChannelACL();
		} else if (chosenAction == deleteAction) {
			removePersistentTextChannel();
		}
		return;
	}

	if (scopeValue != static_cast< int >(MumbleProto::Channel)) {
		return;
	}

	Channel *voiceChannel        = Channel::get(scopeID);
	QAction *openAction          = menu.addAction(tr("Open room chat"));
	QAction *goToVoiceRoomAction = menu.addAction(tr("Go to voice room"));
	goToVoiceRoomAction->setEnabled(voiceChannel != nullptr);

	QAction *chosenAction = menu.exec(globalPosition);
	if (chosenAction == openAction) {
		focusPersistentChatVoiceChannel(voiceChannel);
	} else if (chosenAction == goToVoiceRoomAction) {
		selectVoiceTreeChannel(voiceChannel);
	}
}

void MainWindow::updatePersistentTextChannelControls() {
	if (!usesModernShell() || !m_persistentChatChannelList) {
		return;
	}

	const bool canManage = canManagePersistentTextChannels();
	if (m_serverNavigatorServerSettingsButton) {
		m_serverNavigatorServerSettingsButton->setVisible(canManage);
		m_serverNavigatorServerSettingsButton->setEnabled(canManage);
	}
	if (m_serverNavigatorCreateTextChannelButton) {
		m_serverNavigatorCreateTextChannelButton->setVisible(canManage);
		m_serverNavigatorCreateTextChannelButton->setEnabled(canManage);
	}

	m_persistentChatChannelList->viewport()->update();
}

std::size_t MainWindow::cachedPersistentChatUnreadCount(MumbleProto::ChatScope scope, unsigned int scopeID) const {
	return m_persistentChatController
			   ? static_cast< std::size_t >(m_persistentChatController->unreadCount(scope, scopeID))
			   : 0;
}

void MainWindow::setCachedPersistentChatUnreadCount(MumbleProto::ChatScope scope, unsigned int scopeID,
													unsigned int lastReadMessageID, std::size_t unreadCount) {
	Q_UNUSED(scope);
	Q_UNUSED(scopeID);
	Q_UNUSED(lastReadMessageID);
	Q_UNUSED(unreadCount);
	updatePersistentChatScopeSelectorLabels();
}

std::size_t MainWindow::totalCachedPersistentChatUnreadCount() const {
	return m_persistentChatController ? static_cast< std::size_t >(m_persistentChatController->totalUnreadCount()) : 0;
}

bool MainWindow::navigateToPersistentChatScope(MumbleProto::ChatScope scope, unsigned int scopeID) {
	if (!m_persistentChatChannelList || !usesModernShell()) {
		return false;
	}

	for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
		QListWidgetItem *item = m_persistentChatChannelList->item(i);
		if (!item) {
			continue;
		}

		const MumbleProto::ChatScope itemScope =
			static_cast< MumbleProto::ChatScope >(item->data(PersistentChatScopeRole).toInt());
		const unsigned int itemScopeID = item->data(PersistentChatScopeIDRole).toUInt();
		if (itemScope == scope && itemScopeID == scopeID) {
			setPersistentChatTargetUsesVoiceTree(false);
			cachePersistentChatChannelSelection(item);
			if (persistentChatChannelListCurrentItem() == item) {
				updateChatBar();
				refreshPersistentChatView(true);
			} else {
				m_persistentChatChannelList->setCurrentItem(item);
			}
			return true;
		}
	}

	return false;
}

bool MainWindow::hasPersistentChatCapabilities() const {
	return m_hasPersistentChatSupport || !m_persistentTextChannels.isEmpty();
}

MainWindow::PersistentChatTarget MainWindow::legacyChatTarget() const {
	PersistentChatTarget target;

	if (Global::get().uiSession == 0 || !Global::get().sh || !Global::get().sh->isRunning()) {
		target.label = tr("Not connected");
		return target;
	}

	const ClientUser *self   = ClientUser::get(Global::get().uiSession);
	ClientUser *selectedUser = pmModel ? pmModel->getSelectedUser() : nullptr;
	if (selectedUser && selectedUser->uiSession != Global::get().uiSession) {
		target.valid          = true;
		target.directMessage  = true;
		target.legacyTextPath = true;
		target.user           = selectedUser;
		target.label          = selectedUser->qsName;
		target.description    = tr("Classic direct message");
		return target;
	}

	Channel *selectedChannel = self ? self->cChannel : nullptr;

	if (!selectedChannel) {
		target.label = tr("No channel selected");
		return target;
	}

	target.valid          = true;
	target.legacyTextPath = true;
	target.channel        = selectedChannel;
	target.scope          = MumbleProto::Channel;
	target.scopeID        = selectedChannel->iId;
	target.label          = selectedChannel->qsName;
	target.description    = tr("Classic channel chat");
	return target;
}

MainWindow::PersistentChatTarget MainWindow::currentPersistentChatTarget() const {
	PersistentChatTarget target;

	if (!usesModernShell()) {
		return legacyChatTarget();
	}
	if (modernShellMinimalSnapshotEnabled()) {
		if (Global::get().uiSession != 0 && Global::get().sh && Global::get().sh->isRunning()) {
			target.valid       = true;
			target.readOnly    = true;
			target.serverLog   = true;
			target.scope       = MumbleProto::Aggregate;
			target.scopeID     = 0;
			target.label       = tr("Activity");
			target.description = tr("Shared shell safe mode keeps the modern bootstrap on a reduced snapshot payload.");
		} else {
			target.label = tr("Not connected");
		}
		return target;
	}

	if (Global::get().uiSession != 0 && Global::get().sh && Global::get().sh->isRunning()
		&& !hasPersistentChatCapabilities()) {
		return legacyChatTarget();
	}

	if (!m_persistentChatChannelList) {
		target.label = tr("Not connected");
		if (Global::get().uiSession != 0 && Global::get().sh && Global::get().sh->isRunning()) {
			target.label = tr("No conversation selected");
		}
		return target;
	}

	if (m_persistentChatTargetUsesVoiceTree && Global::get().uiSession != 0 && Global::get().sh
		&& Global::get().sh->isRunning()) {
		Channel *selectedChannel = selectedVoiceTreeChannel();
		if (!selectedChannel) {
			target.label = tr("No current room");
			return target;
		}

		target.valid       = true;
		target.scope       = MumbleProto::Channel;
		target.scopeID     = selectedChannel->iId;
		target.channel     = selectedChannel;
		target.label       = selectedChannel->qsName;
		target.description = tr("Legacy-compatible room chat.");
		return target;
	}

	const QListWidgetItem *currentItem = persistentChatChannelListCurrentItem();

	if (!currentItem) {
		target.label = tr("Not connected");
		if (Global::get().uiSession != 0 && Global::get().sh && Global::get().sh->isRunning()) {
			target.label = tr("No conversation selected");
		}
		return target;
	}

	const int scopeValue = currentItem->data(PersistentChatScopeRole).toInt();
	if (scopeValue == LocalServerLogScope) {
		target.valid       = true;
		target.readOnly    = true;
		target.serverLog   = true;
		target.label       = tr("Activity");
		target.description = tr("Server log, connection status, notices, and client diagnostics.");
		return target;
	}

	if (scopeValue == LocalDirectMessageScope) {
		target.valid          = true;
		target.directMessage  = true;
		target.legacyTextPath = true;
		target.user           = ClientUser::get(currentItem->data(PersistentChatScopeIDRole).toUInt());
		target.channel        = target.user ? target.user->cChannel : nullptr;
		target.label          = target.user ? target.user->qsName : tr("Direct message");
		target.description    = tr("Classic direct message");
		if (!target.user) {
			target.readOnly      = true;
			target.statusMessage = tr("This direct conversation is no longer available.");
		} else {
			target.statusMessage = tr(
				"Direct messages still use the classic text-message transport and do not have persistent history yet.");
		}
		return target;
	}

	if (Global::get().uiSession == 0 || !Global::get().sh || !Global::get().sh->isRunning()) {
		target.label = tr("Not connected");
		return target;
	}

	const MumbleProto::ChatScope scope = static_cast< MumbleProto::ChatScope >(scopeValue);
	const unsigned int scopeID         = currentItem->data(PersistentChatScopeIDRole).toUInt();

	switch (scope) {
		case MumbleProto::TextChannel: {
			const auto it = m_persistentTextChannels.constFind(scopeID);
			if (it == m_persistentTextChannels.cend()) {
				target.label = tr("Text channel is unavailable");
				return target;
			}

			target.valid       = true;
			target.scope       = MumbleProto::TextChannel;
			target.scopeID     = scopeID;
			target.channel     = Channel::get(it->aclChannelID);
			target.label       = tr("#%1").arg(it->name);
			target.description = it->description;
			if (!target.channel) {
				target.readOnly      = true;
				target.statusMessage = tr("This text channel is linked to an unavailable ACL channel.");
			}
			return target;
		}
		case MumbleProto::Channel: {
			target.valid   = true;
			target.scope   = MumbleProto::Channel;
			target.scopeID = scopeID;
			target.channel = Channel::get(scopeID);
			if (target.channel) {
				target.label       = target.channel->qsName;
				target.description = tr("Legacy-compatible room chat.");
			} else {
				target.readOnly      = true;
				target.label         = tr("Channel %1").arg(scopeID);
				target.statusMessage = tr("This channel is unavailable.");
			}
			return target;
		}
		case MumbleProto::Aggregate:
			target.valid         = true;
			target.readOnly      = true;
			target.scope         = MumbleProto::Aggregate;
			target.scopeID       = 0;
			target.label         = tr("Combined activity");
			target.description   = tr("This legacy conversation is no longer shown in the text room list.");
			target.statusMessage = target.description;
			return target;
		case MumbleProto::ServerGlobal:
			target.valid         = true;
			target.readOnly      = true;
			target.scope         = MumbleProto::ServerGlobal;
			target.scopeID       = 0;
			target.label         = tr("Server-wide chat");
			target.description   = tr("This legacy conversation is no longer shown in the text room list.");
			target.statusMessage = target.description;
			if (!Global::get().bPersistentGlobalChatEnabled) {
				target.statusMessage = tr("Legacy server-wide chat is disabled by this server.");
			}
			return target;
		default:
			break;
	}

	target.label = tr("Unknown chat");
	return target;
}

bool MainWindow::isServerLogViewVisible() const {
	return (qdwLog && qdwLog->isVisible()) || (m_persistentChatLogPanel && m_persistentChatLogPanel->isVisible());
}

void MainWindow::setPersistentChatContentMode(bool showServerLog, bool preserveScrollPosition, bool showComposer) {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		appendModernShellConnectTrace(
			QStringLiteral("UI setPersistentChatContentMode minimal-skip serverLog=%1 preserve=%2 composer=%3")
				.arg(showServerLog ? 1 : 0)
				.arg(preserveScrollPosition ? 1 : 0)
				.arg(showComposer ? 1 : 0));
		return;
	}

	const bool wasShowingServerLog = m_persistentChatLogPanel && m_persistentChatLogPanel->isVisible();
	if (m_persistentChatConversationPanel) {
		m_persistentChatConversationPanel->setVisible(true);
	}
	if (m_persistentChatHistory) {
		m_persistentChatHistory->setVisible(!showServerLog);
	}
	if (m_persistentChatLogPanel) {
		m_persistentChatLogPanel->setVisible(showServerLog);
	}
	if (m_persistentChatComposerFrame) {
		m_persistentChatComposerFrame->setVisible(!showServerLog || showComposer);
	}

	if (!showServerLog || !m_persistentChatLogView) {
		return;
	}

	QScrollBar *scrollBar = m_persistentChatLogView->verticalScrollBar();
	if (!scrollBar) {
		return;
	}

	const bool shouldSnapToBottom = !preserveScrollPosition || !wasShowingServerLog || m_persistentChatLogStickToBottom;
	if (shouldSnapToBottom) {
		m_persistentChatLogStickToBottom = true;
		scrollBar->setValue(scrollBar->maximum());
	}
}

void MainWindow::renderLegacyActivityView(bool preserveScrollPosition) {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		appendModernShellConnectTrace(
			QStringLiteral("UI renderLegacyActivityView minimal-skip preserve=%1").arg(preserveScrollPosition ? 1 : 0));
		return;
	}

	if (!m_persistentChatLogView) {
		clearPersistentChatView(tr("Activity is unavailable."), tr("Activity"));
		return;
	}

	setPersistentChatContentMode(true, preserveScrollPosition, true);
}

void MainWindow::clearPersistentChatView(const QString &message, const QString &title, const QStringList &hints) {
	if (!usesModernShell()) {
		return;
	}

	setPersistentChatContentMode(false);
	const PersistentChatTarget target = currentPersistentChatTarget();
	QString resolvedEyebrow           = tr("Text");
	QString resolvedTitle             = title.trimmed();
	QString resolvedBody              = message.trimmed();
	QStringList resolvedHints         = hints;
	if (resolvedBody.isEmpty()) {
		resolvedBody = tr("Nothing to show yet.");
	}

	if (resolvedTitle.isEmpty()) {
		if (Global::get().uiSession == 0 || !Global::get().sh || !target.valid) {
			resolvedTitle = tr("Start a conversation");
			if (resolvedHints.isEmpty()) {
				resolvedHints << tr("Open Server to connect") << tr("Room chat and history appear here");
			}
		} else if (target.directMessage) {
			resolvedEyebrow = tr("Direct");
			resolvedTitle   = tr("Direct messages");
			if (resolvedHints.isEmpty()) {
				resolvedHints << tr("Classic chat still handles direct messages");
			}
		} else if (target.readOnly) {
			resolvedTitle = tr("Read-only conversation");
			if (resolvedHints.isEmpty()) {
				resolvedHints << tr("Choose another room to reply");
			}
		} else {
			if (target.scope == MumbleProto::ServerGlobal || target.scope == MumbleProto::Aggregate) {
				resolvedEyebrow = tr("Legacy");
			} else if (target.scope == MumbleProto::TextChannel) {
				resolvedEyebrow = tr("Room");
			}

			resolvedTitle = target.label.isEmpty() ? tr("Conversation") : target.label;
			if (resolvedHints.isEmpty()) {
				resolvedHints << tr("Messages stay with this room");
			}
		}
	}

	m_persistentChatMessages.clear();
	m_persistentChatInlineDataImageSources.clear();
	m_pendingPersistentChatRender.reset();
	m_visiblePersistentChatScope.reset();
	m_visiblePersistentChatScopeID           = 0;
	m_visiblePersistentChatLastReadMessageID = 0;
	m_visiblePersistentChatHasMore           = false;
	m_persistentChatLoadingOlder             = false;
	m_persistentChatPreviewRefreshPending    = false;
	if (m_persistentChatScrollIdleTimer) {
		m_persistentChatScrollIdleTimer->stop();
	}
	clearPersistentChatReplyTarget(false);
	updatePersistentChatScopeSelectorLabels();

	if (!m_persistentChatHistoryModel) {
		return;
	}

	PersistentChatHistoryRow row;
	row.kind      = PersistentChatHistoryRowKind::State;
	row.rowId     = QString::fromLatin1("state:%1:%2").arg(resolvedEyebrow, resolvedTitle);
	row.signature = QString::number(qHash(resolvedEyebrow + QLatin1Char('|') + resolvedTitle + QLatin1Char('|')
										  + resolvedBody + resolvedHints.join(QLatin1Char('|'))));
	PersistentChatStateRowSpec stateSpec;
	stateSpec.eyebrow = resolvedEyebrow;
	stateSpec.title   = resolvedTitle;
	stateSpec.body    = resolvedBody;
	stateSpec.hints   = resolvedHints;
	stateSpec.minimumHeight =
		m_persistentChatHistory ? std::max(220, m_persistentChatHistory->viewport()->height() - 4) : 220;
	row.state                            = stateSpec;
	m_persistentChatRestoreAnchorPending = false;
	m_persistentChatPendingAnchorRowId.clear();
	m_persistentChatPendingAnchorOffset = 0;
	m_persistentChatHistoryModel->setRows({ row });
	if (m_persistentChatHistory && m_persistentChatHistory->verticalScrollBar()) {
		m_persistentChatHistory->verticalScrollBar()->setValue(0);
	}
}

std::optional< QString > MainWindow::persistentChatPreviewKey(const MumbleProto::ChatMessage &message) const {
	mumble::chatperf::ScopedDuration trace("chat.preview.key");
	if (!Global::get().s.bEnableLinkPreviews) {
		return std::nullopt;
	}

	if (const std::optional< MumbleProto::ChatEmbedRef > embed = persistentChatPrimaryEmbed(message); embed) {
		return QString::fromLatin1("embed:%1")
			.arg(QString::fromUtf8(QUrl::toPercentEncoding(u8(embed->canonical_url()))));
	}

	QString messageContent =
		message.has_body_text() ? persistentChatMessageBodyHtml(message) : persistentChatMessageRawBody(message);
	if (!message.has_body_text()) {
		extractPersistentChatReplyReference(messageContent, &messageContent);
	}
	const QList< QUrl > urls = extractPreviewableUrls(messageContent);
	for (const QUrl &url : urls) {
		if (const std::optional< QString > videoId = extractYouTubeVideoId(url); videoId) {
			return QString::fromLatin1("youtube:%1").arg(*videoId);
		}
		if (isYouTubeHost(url.host())) {
			// Only create custom previews for actual videos. Generic previews for YouTube
			// landing pages are noisy, expensive, and were unstable in Windows testing.
			continue;
		}

		if (isDirectImageUrl(url)) {
			return QString::fromLatin1("image:%1").arg(normalizedPreviewUrl(url));
		}

		return QString::fromLatin1("url:%1").arg(normalizedPreviewUrl(url));
	}

	return std::nullopt;
}

std::optional< MumbleProto::ChatEmbedRef >
	MainWindow::persistentChatPrimaryEmbed(const MumbleProto::ChatMessage &message) const {
	for (int i = 0; i < message.embeds_size(); ++i) {
		const MumbleProto::ChatEmbedRef &embed = message.embeds(i);
		if (embed.has_canonical_url() && !u8(embed.canonical_url()).trimmed().isEmpty()) {
			return embed;
		}
	}

	return std::nullopt;
}

void MainWindow::ensurePersistentChatPreview(const QString &previewKey) {
	mumble::chatperf::ScopedDuration trace("chat.preview.ensure");
	if (previewKey.isEmpty()) {
		return;
	}

	if (m_persistentChatPreviews.contains(previewKey)) {
		return;
	}

	const auto renderIfVisible = [this, previewKey]() {
		QPointer< MainWindow > guardedThis(this);
		QMetaObject::invokeMethod(
			this,
			[guardedThis, previewKey]() {
				if (!guardedThis) {
					return;
				}

				guardedThis->queueModernShellSnapshotSync();
				guardedThis->updatePersistentChatPreviewViewIfVisible(previewKey);
			},
			Qt::QueuedConnection);
	};

	PersistentChatPreview preview;
	preview.openLabel = tr("Open link");

	if (previewKey.startsWith(QLatin1String("embed:"))) {
		const QString canonicalUrl =
			QUrl::fromPercentEncoding(previewKey.mid(QStringLiteral("embed:").size()).toUtf8());
		const QUrl previewUrl(canonicalUrl);
		if (!previewUrl.isValid()) {
			return;
		}
		const std::optional< PersistentChatPreviewProviderInfo > provider = previewProviderInfo(previewUrl);

		std::optional< MumbleProto::ChatEmbedRef > embed;
		for (const MumbleProto::ChatMessage &message : m_persistentChatMessages) {
			for (int i = 0; i < message.embeds_size(); ++i) {
				const MumbleProto::ChatEmbedRef &currentEmbed = message.embeds(i);
				if (currentEmbed.has_canonical_url() && u8(currentEmbed.canonical_url()) == canonicalUrl) {
					embed = currentEmbed;
					break;
				}
			}
			if (embed) {
				break;
			}
		}

		if (!embed) {
			return;
		}

		const MumbleProto::ChatEmbedRef &resolvedEmbed = *embed;
		const MumbleProto::ChatEmbedStatus status =
			resolvedEmbed.has_status() ? resolvedEmbed.status() : MumbleProto::ChatEmbedStatusPending;
		preview.canonicalUrl = canonicalUrl;
		preview.title        = resolvedEmbed.has_title() && !u8(resolvedEmbed.title()).trimmed().isEmpty()
							? u8(resolvedEmbed.title())
							: (provider ? provider->fallbackTitle : previewDisplayHost(previewUrl));
		preview.subtitle = resolvedEmbed.has_site_name() && !u8(resolvedEmbed.site_name()).trimmed().isEmpty()
							   ? u8(resolvedEmbed.site_name())
							   : (provider ? provider->siteLabel : previewDisplayHost(previewUrl));
		preview.description = resolvedEmbed.has_description() ? u8(resolvedEmbed.description()) : QString();
		if (provider) {
			preview.openLabel = provider->openLabel;
		}
		preview.previewAssetID    = resolvedEmbed.has_preview_asset_id() ? resolvedEmbed.preview_asset_id() : 0;
		preview.metadataFinished  = status != MumbleProto::ChatEmbedStatusPending;
		preview.thumbnailFinished = preview.previewAssetID == 0;
		preview.failed = status == MumbleProto::ChatEmbedStatusBlocked || status == MumbleProto::ChatEmbedStatusFailed;

		if (status == MumbleProto::ChatEmbedStatusBlocked && preview.description.isEmpty()) {
			preview.description = tr("Automatic previews are disabled for localhost and private-network targets.");
		} else if (status == MumbleProto::ChatEmbedStatusFailed && preview.description.isEmpty()) {
			preview.description = tr("Preview unavailable");
		}

		m_persistentChatPreviews.insert(previewKey, preview);
		if (preview.previewAssetID > 0) {
			ensurePersistentChatPreviewAssetDownload(preview.previewAssetID, previewKey);
		} else {
			ensurePersistentChatPreviewSiteSnapshot(previewKey);
			renderIfVisible();
		}
		return;
	}

	if (previewKey.startsWith(QLatin1String("youtube:"))) {
		const QString videoId = previewKey.mid(QStringLiteral("youtube:").size());
		preview.canonicalUrl  = QString::fromLatin1("https://www.youtube.com/watch?v=%1").arg(videoId);
		preview.title         = tr("Loading YouTube preview...");
		preview.subtitle      = tr("Fetching title and thumbnail");
		preview.openLabel     = tr("Open on YouTube");
		m_persistentChatPreviews.insert(previewKey, preview);

		QUrl oembedUrl(QLatin1String("https://www.youtube.com/oembed"));
		QUrlQuery oembedQuery;
		oembedQuery.addQueryItem(QLatin1String("url"),
								 QString::fromLatin1("https://www.youtube.com/watch?v=%1").arg(videoId));
		oembedQuery.addQueryItem(QLatin1String("format"), QLatin1String("json"));
		oembedUrl.setQuery(oembedQuery);

		QNetworkRequest oembedRequest(oembedUrl);
		preparePreviewRequest(oembedRequest);
		QNetworkReply *oembedReply = Global::get().nam->get(oembedRequest);
		applyPreviewReplyGuards(oembedReply, PREVIEW_MAX_PAGE_BYTES);
		connect(oembedReply, &QNetworkReply::finished, this, [this, oembedReply, previewKey, renderIfVisible]() {
			const QByteArray response = oembedReply->readAll();
			const bool success        = oembedReply->error() == QNetworkReply::NoError;
			const QString failureText = previewFailureText(oembedReply);
			oembedReply->deleteLater();

			auto it = m_persistentChatPreviews.find(previewKey);
			if (it == m_persistentChatPreviews.end()) {
				return;
			}

			it->metadataFinished = true;

			if (success) {
				QJsonParseError error;
				const QJsonDocument document = QJsonDocument::fromJson(response, &error);
				if (error.error == QJsonParseError::NoError && document.isObject()) {
					const QJsonObject object = document.object();
					const QString title      = object.value(QLatin1String("title")).toString().trimmed();
					const QString author     = object.value(QLatin1String("author_name")).toString().trimmed();
					it->title                = title.isEmpty() ? tr("YouTube video") : title;
					it->subtitle             = author.isEmpty() ? tr("YouTube") : tr("YouTube by %1").arg(author);
					renderIfVisible();
					return;
				}
			}

			if (it->title == tr("Loading YouTube preview...")) {
				it->title = tr("YouTube video");
			}
			if (it->subtitle.isEmpty() || it->subtitle == tr("Fetching title and thumbnail")) {
				it->subtitle = failureText;
			}

			if (it->thumbnailFinished && it->thumbnailImage.isNull()) {
				it->failed = true;
			}
			renderIfVisible();
		});

		QUrl thumbnailUrl(QString::fromLatin1("https://i.ytimg.com/vi/%1/hqdefault.jpg").arg(videoId));
		QNetworkRequest thumbnailRequest(thumbnailUrl);
		preparePreviewRequest(thumbnailRequest);
		QNetworkReply *thumbnailReply = Global::get().nam->get(thumbnailRequest);
		applyPreviewReplyGuards(thumbnailReply, PREVIEW_MAX_IMAGE_BYTES);
		connect(thumbnailReply, &QNetworkReply::finished, this, [this, thumbnailReply, previewKey, renderIfVisible]() {
			const QByteArray data     = thumbnailReply->readAll();
			const bool success        = thumbnailReply->error() == QNetworkReply::NoError;
			const QString failureText = previewFailureText(thumbnailReply);
			thumbnailReply->deleteLater();

			auto it = m_persistentChatPreviews.find(previewKey);
			if (it == m_persistentChatPreviews.end()) {
				return;
			}

			it->thumbnailFinished = true;

			if (success) {
				const QImage image = decodePersistentChatThumbnailImage(data);
				if (!image.isNull()) {
					it->thumbnailImage = persistentChatThumbnailImage(image);
					renderIfVisible();
					return;
				}
			}

			if (it->metadataFinished && it->title == tr("Loading YouTube preview...")) {
				it->title = tr("YouTube video");
			}
			if (it->metadataFinished && it->subtitle.isEmpty()) {
				it->subtitle = failureText;
			}
			if (it->metadataFinished && it->thumbnailImage.isNull()) {
				it->failed = true;
			}
			renderIfVisible();
		});
		return;
	}

	const bool isImagePreview = previewKey.startsWith(QLatin1String("image:"));
	const QString encodedUrl =
		previewKey.mid(isImagePreview ? QStringLiteral("image:").size() : QStringLiteral("url:").size());
	const QUrl previewUrl = QUrl::fromEncoded(encodedUrl.toUtf8());
	if (!previewUrl.isValid()) {
		return;
	}
	const std::optional< PersistentChatPreviewProviderInfo > provider = previewProviderInfo(previewUrl);

	preview.canonicalUrl = previewUrl.toString();
	preview.subtitle     = provider ? provider->siteLabel : previewDisplayHost(previewUrl);
	preview.openLabel    = isImagePreview ? tr("Open image") : (provider ? provider->openLabel : tr("Open link"));

	if (!isSafePreviewTarget(previewUrl)) {
		preview.title             = isImagePreview ? tr("Image preview blocked") : tr("Link preview blocked");
		preview.description       = tr("Automatic previews are disabled for localhost and private-network targets.");
		preview.metadataFinished  = true;
		preview.thumbnailFinished = true;
		m_persistentChatPreviews.insert(previewKey, preview);
		renderIfVisible();
		return;
	}

	if (isImagePreview) {
		const QString fileName = QFileInfo(previewUrl.path()).fileName();
		preview.title = fileName.isEmpty() ? (provider ? provider->fallbackTitle : tr("Image preview")) : fileName;
		preview.description = tr("Direct image preview");
		m_persistentChatPreviews.insert(previewKey, preview);

		QNetworkRequest imageRequest(previewUrl);
		preparePreviewRequest(imageRequest);
		QNetworkReply *imageReply = Global::get().nam->get(imageRequest);
		applyPreviewReplyGuards(imageReply, PREVIEW_MAX_IMAGE_BYTES);
		connect(imageReply, &QNetworkReply::finished, this, [this, imageReply, previewKey, renderIfVisible]() {
			const QByteArray data     = imageReply->readAll();
			const bool success        = imageReply->error() == QNetworkReply::NoError;
			const QString failureText = previewFailureText(imageReply);
			imageReply->deleteLater();

			auto it = m_persistentChatPreviews.find(previewKey);
			if (it == m_persistentChatPreviews.end()) {
				return;
			}

			it->metadataFinished  = true;
			it->thumbnailFinished = true;

			if (success) {
				const QImage image = decodePersistentChatThumbnailImage(data);
				if (!image.isNull()) {
					it->thumbnailImage = persistentChatThumbnailImage(image);
					renderIfVisible();
					return;
				}
			}

			it->failed      = true;
			it->description = failureText;
			renderIfVisible();
		});
		return;
	}

	preview.title       = provider ? provider->fallbackTitle : tr("Loading link preview...");
	preview.description = tr("Fetching page metadata");
	m_persistentChatPreviews.insert(previewKey, preview);

	QNetworkRequest pageRequest(previewUrl);
	preparePreviewRequest(pageRequest);
	QNetworkReply *pageReply = Global::get().nam->get(pageRequest);
	applyPreviewReplyGuards(pageReply, PREVIEW_MAX_PAGE_BYTES, false);
	connect(
		pageReply, &QNetworkReply::finished, this,
		[this, pageReply, previewKey, previewUrl, provider, renderIfVisible]() {
			const QByteArray data       = pageReply->readAll();
			const bool success          = pageReply->error() == QNetworkReply::NoError;
			const QString contentType   = pageReply->header(QNetworkRequest::ContentTypeHeader).toString().toLower();
			const QString failureText   = previewFailureText(pageReply);
			const bool allowPartialHtml = previewAbortReason(pageReply) == QLatin1String("too_large") && !data.isEmpty()
										  && previewContentTypeLooksHtml(contentType);
			pageReply->deleteLater();

			auto it = m_persistentChatPreviews.find(previewKey);
			if (it == m_persistentChatPreviews.end()) {
				return;
			}

			it->metadataFinished = true;

			if ((!success && !allowPartialHtml) || !previewContentTypeLooksHtml(contentType)) {
				it->thumbnailFinished = true;
				if (it->title == tr("Loading link preview...") || (provider && it->title == provider->fallbackTitle)) {
					it->title = provider ? provider->fallbackTitle : previewDisplayHost(previewUrl);
				}
				it->description = (success || allowPartialHtml) ? tr("Preview unavailable") : failureText;
				it->failed      = !allowPartialHtml && !success;
				ensurePersistentChatPreviewSiteSnapshot(previewKey);
				renderIfVisible();
				return;
			}

			// Modern SPA pages often advertise a very large Content-Length even though the useful
			// preview metadata is available near the start of <head>. Parse the prefix we did receive
			// instead of failing the entire preview outright.
			const QByteArray htmlBytes =
				data.size() > PREVIEW_MAX_PAGE_BYTES ? data.left(PREVIEW_MAX_PAGE_BYTES) : data;
			const QString html                       = QString::fromUtf8(htmlBytes);
			const QHash< QString, QString > metaTags = extractMetaTags(html);
			const QString title                      = metaTags.value(
                QLatin1String("og:title"), metaTags.value(QLatin1String("twitter:title"), extractHtmlTitle(html)));
			const QString description = metaTags.value(
				QLatin1String("og:description"),
				metaTags.value(QLatin1String("twitter:description"), metaTags.value(QLatin1String("description"))));
			const QString siteName       = metaTags.value(QLatin1String("og:site_name"),
                                                    provider ? provider->siteLabel : previewDisplayHost(previewUrl));
			const QString imageUrlString = previewImageMetaTag(metaTags);

			it->title = title.isEmpty() ? (provider ? provider->fallbackTitle : previewDisplayHost(previewUrl)) : title;
			it->subtitle =
				siteName.isEmpty() ? (provider ? provider->siteLabel : previewDisplayHost(previewUrl)) : siteName;
			it->description = description;

			if (imageUrlString.isEmpty()) {
				it->thumbnailFinished = true;
				ensurePersistentChatPreviewSiteSnapshot(previewKey);
				renderIfVisible();
				return;
			}

			const QUrl imageUrl = previewUrl.resolved(QUrl(imageUrlString));
			if (!isSafePreviewTarget(imageUrl)) {
				it->thumbnailFinished = true;
				ensurePersistentChatPreviewSiteSnapshot(previewKey);
				renderIfVisible();
				return;
			}

			QNetworkRequest imageRequest(imageUrl);
			preparePreviewRequest(imageRequest);
			QNetworkReply *imageReply = Global::get().nam->get(imageRequest);
			applyPreviewReplyGuards(imageReply, PREVIEW_MAX_IMAGE_BYTES);
			connect(imageReply, &QNetworkReply::finished, this, [this, imageReply, previewKey, renderIfVisible]() {
				const QByteArray imageData = imageReply->readAll();
				const bool imageSuccess    = imageReply->error() == QNetworkReply::NoError;
				const QString failureText  = previewFailureText(imageReply);
				imageReply->deleteLater();

				auto it = m_persistentChatPreviews.find(previewKey);
				if (it == m_persistentChatPreviews.end()) {
					return;
				}

				it->thumbnailFinished = true;

				if (imageSuccess) {
					const QImage image = decodePersistentChatThumbnailImage(imageData);
					if (!image.isNull()) {
						it->thumbnailImage = persistentChatThumbnailImage(image);
						renderIfVisible();
						return;
					}
				}

				if (it->description.isEmpty() || it->description == tr("Fetching page metadata")) {
					it->description = failureText;
				}
				ensurePersistentChatPreviewSiteSnapshot(previewKey);
				renderIfVisible();
			});
			renderIfVisible();
		});
}

void MainWindow::ensurePersistentChatPreviewAssetDownload(unsigned int assetID, const QString &previewKey) {
	if (assetID == 0 || previewKey.isEmpty() || !Global::get().sh || !Global::get().sh->isRunning()) {
		return;
	}

	auto it = m_persistentChatAssetDownloads.find(assetID);
	if (it == m_persistentChatAssetDownloads.end()) {
		PersistentChatAssetDownload download;
		download.assetID = assetID;
		download.previewKeys.insert(previewKey);
		it = m_persistentChatAssetDownloads.insert(assetID, download);
	} else {
		it->previewKeys.insert(previewKey);
		if (it->totalSize > 0 && static_cast< quint64 >(it->bytes.size()) >= it->totalSize) {
			return;
		}
	}

	MumbleProto::ChatAssetRequest request;
	request.set_asset_id(assetID);
	request.set_offset(it->nextOffset);
	request.set_max_bytes(262144);
	Global::get().sh->sendMessage(request);
}

void MainWindow::ensurePersistentChatPreviewSiteSnapshot(const QString &previewKey) {
#if !defined(MUMBLE_HAS_MODERN_LAYOUT)
	Q_UNUSED(previewKey);
#else
	if (previewKey.isEmpty() || previewKey.startsWith(QLatin1String("image:"))
		|| previewKey.startsWith(QLatin1String("youtube:"))) {
		return;
	}

	auto it = m_persistentChatPreviews.find(previewKey);
	if (it == m_persistentChatPreviews.end()) {
		return;
	}

	PersistentChatPreview &preview = it.value();
	if (!preview.thumbnailImage.isNull() || preview.siteSnapshotRequested || preview.siteSnapshotFinished) {
		return;
	}
	if (preview.previewAssetID > 0 && !preview.thumbnailFinished) {
		return;
	}
	if (!preview.metadataFinished && !preview.failed) {
		return;
	}

	const QUrl previewUrl(preview.canonicalUrl);
	if (!isSafePreviewTarget(previewUrl)) {
		preview.siteSnapshotFinished = true;
		return;
	}

	PersistentChatPreviewSnapshotRenderer *renderer =
		static_cast< PersistentChatPreviewSnapshotRenderer * >(m_persistentChatPreviewSnapshotRenderer);
	if (!renderer) {
		renderer                                = new PersistentChatPreviewSnapshotRenderer(this);
		m_persistentChatPreviewSnapshotRenderer = renderer;
		renderer->setResultCallback([this](const QString &key, const QImage &image, bool success) {
			handlePersistentChatPreviewSiteSnapshotResult(key, image, success);
		});
	}

	preview.siteSnapshotRequested = true;
	renderer->requestSnapshot(previewKey, previewUrl);
#endif
}

void MainWindow::handlePersistentChatPreviewSiteSnapshotResult(const QString &previewKey, const QImage &image,
															   bool success) {
	auto it = m_persistentChatPreviews.find(previewKey);
	if (it == m_persistentChatPreviews.end()) {
		return;
	}

	it->siteSnapshotRequested = false;
	it->siteSnapshotFinished  = true;
	if (success && !image.isNull()) {
		it->thumbnailImage    = persistentChatThumbnailImage(image);
		it->thumbnailFinished = true;
		it->failed            = false;
	} else if (!it->thumbnailFinished) {
		it->thumbnailFinished = true;
	}

	queueModernShellSnapshotSync();
	updatePersistentChatPreviewViewIfVisible(previewKey);
}

int MainWindow::persistentChatPreviewContentWidth(int leftPadding) const {
	if (!m_persistentChatHistory) {
		return 360;
	}

	const int viewportWidth     = m_persistentChatHistory->viewport()->width();
	const int rightSafetyMargin = 18;
	int availableWidth          = viewportWidth - leftPadding - rightSafetyMargin;
	availableWidth              = std::max(220, std::min(PERSISTENT_CHAT_PREVIEW_CARD_MAX_WIDTH, availableWidth));
	if (availableWidth <= 220) {
		return 220;
	}

	const int quantizedWidth =
		220 + (((availableWidth - 220) / PERSISTENT_CHAT_PREVIEW_WIDTH_STEP) * PERSISTENT_CHAT_PREVIEW_WIDTH_STEP);
	return std::max(220, quantizedWidth);
}

QString MainWindow::persistentChatPreviewHtml(const QString &previewKey, int availableWidth) const {
	const auto it = m_persistentChatPreviews.constFind(previewKey);
	if (it == m_persistentChatPreviews.cend()) {
		return QString();
	}

	const PersistentChatPreview &preview = it.value();
	const QString cardUrl                = preview.canonicalUrl.toHtmlEscaped();
	const QUrl canonicalUrl(preview.canonicalUrl);
	const QString domainText = previewDisplayHost(canonicalUrl).toHtmlEscaped();
	const QString titleText =
		trimmedPreviewText(preview.title.isEmpty() ? tr("Link preview") : preview.title, 90).toHtmlEscaped();
	const QString descriptionText = trimmedPreviewText(preview.description, 140).toHtmlEscaped();
	const QString secondaryText   = trimmedPreviewText(preview.subtitle, 80).toHtmlEscaped();
	const bool isImagePreview     = previewKey.startsWith(QLatin1String("image:"));
	const int cardWidth           = std::max(isImagePreview ? 240 : 220, availableWidth);

	QColor cardBackground;
	QColor borderColor;
	QColor accentColor;
	QColor titleColor;
	QColor bodyColor;
	QColor mutedColor;
	QColor placeholderColor;
	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		cardBackground   = tokens->surface0;
		borderColor      = tokens->surface1;
		accentColor      = tokens->accent;
		titleColor       = tokens->text;
		bodyColor        = tokens->subtext0;
		mutedColor       = tokens->overlay0;
		placeholderColor = uiThemeColorWithAlpha(tokens->surface1, 0.7);
	} else {
		const ChromePaletteColors chrome =
			buildChromePalette(m_persistentChatContainer ? m_persistentChatContainer->palette() : palette());
		cardBackground   = chrome.elevatedCardColor;
		borderColor      = chrome.dividerColor;
		accentColor      = chrome.accentColor;
		titleColor       = chrome.textColor;
		bodyColor        = chrome.mutedTextColor;
		mutedColor       = chrome.eyebrowColor;
		placeholderColor = chrome.hoverColor;
	}

	if (isImagePreview && !preview.thumbnailImage.isNull()) {
		return QString::fromLatin1("<table cellspacing='0' cellpadding='0' width='%2' "
								   "style='border-collapse:collapse; border:none; background:transparent;'>"
								   "<tr><td style='padding-top:6px;'>%1</td></tr></table>")
			.arg(persistentChatThumbnailHtml(previewKey, preview.thumbnailImage, 360, 300,
											 QStringLiteral("border-radius: 6px;"),
											 persistentChatPreviewImageUrl(previewKey).toString(QUrl::FullyEncoded)))
			.arg(std::min(cardWidth, 360));
	}

	QString detailLine;
	if (!descriptionText.isEmpty()) {
		detailLine =
			QString::fromLatin1("<div style='margin-top:3px; color:%1; font-size:0.82em; line-height:1.4;'>%2</div>")
				.arg(bodyColor.name(), descriptionText);
	} else if (!secondaryText.isEmpty() && secondaryText != domainText) {
		detailLine =
			QString::fromLatin1("<div style='margin-top:3px; color:%1; font-size:0.78em; line-height:1.35;'>%2</div>")
				.arg(bodyColor.name(), secondaryText);
	}

	QString statusLine;
	if (!preview.metadataFinished || !preview.thumbnailFinished) {
		statusLine = QString::fromLatin1("<div style='margin-top:5px; color:%1; font-size:0.76em;'>%2</div>")
						 .arg(mutedColor.name(), tr("Loading preview...").toHtmlEscaped());
	} else if (preview.failed && detailLine.isEmpty()) {
		statusLine = QString::fromLatin1("<div style='margin-top:5px; color:%1; font-size:0.76em;'>%2</div>")
						 .arg(mutedColor.name(), tr("Preview unavailable").toHtmlEscaped());
	}

	QString mediaColumns;
	if (!preview.thumbnailImage.isNull()) {
		mediaColumns = QString::fromLatin1("<td width='120' valign='top'>%1</td><td width='12'></td>")
						   .arg(persistentChatThumbnailHtml(previewKey, preview.thumbnailImage, 120, 68,
															QStringLiteral("border-radius: 3px;")));
	} else if (!isImagePreview && (!preview.metadataFinished || !preview.thumbnailFinished)) {
		mediaColumns =
			QString::fromLatin1("<td width='120' valign='top'>"
								"<div style='width:120px; height:68px; border-radius:3px; background:%1;'></div>"
								"</td><td width='12'></td>")
				.arg(placeholderColor.name(QColor::HexArgb));
	}

	const QString contentHtml =
		QString::fromLatin1(
			"<table cellspacing='0' cellpadding='0' width='%1' "
			"style='border-collapse:separate; border:none; background:%2; border-left:3px solid %3; "
			"border-radius:4px;'>"
			"<tr><td style='padding:10px;'>"
			"<table cellspacing='0' cellpadding='0' width='100%%' style='border-collapse:collapse; border:none;'>"
			"<tr>%4"
			"<td valign='top'>"
			"<a href=\"%5\" style='text-decoration:none; color:%6; font-weight:500;'>%7</a>"
			"%8"
			"<div style='margin-top:6px; color:%9; font-size:0.76em;'>"
			"<a href=\"%5\" style='text-decoration:none; color:%9;'>%10</a></div>"
			"%11"
			"</td></tr></table>"
			"</td></tr></table>")
			.arg(cardWidth)
			.arg(cardBackground.name(QColor::HexArgb))
			.arg(accentColor.name())
			.arg(mediaColumns)
			.arg(cardUrl)
			.arg(titleColor.name())
			.arg(titleText)
			.arg(detailLine)
			.arg(mutedColor.name())
			.arg(domainText)
			.arg(statusLine);

	if (isImagePreview) {
		return QString::fromLatin1("<table cellspacing='0' cellpadding='0' width='%2' "
								   "style='border-collapse:collapse; border:none; background:transparent;'>"
								   "<tr><td style='padding-top:6px;'>%1</td></tr></table>")
			.arg(contentHtml)
			.arg(std::min(cardWidth, PERSISTENT_CHAT_PREVIEW_CARD_MAX_WIDTH));
	}

	return contentHtml;
}

PersistentChatPreviewSpec MainWindow::persistentChatPreviewSpec(const QString &previewKey) const {
	PersistentChatPreviewSpec spec;
	if (previewKey.isEmpty()) {
		return spec;
	}

	const auto it = m_persistentChatPreviews.constFind(previewKey);
	if (it == m_persistentChatPreviews.cend()) {
		return spec;
	}

	const PersistentChatPreview &preview = it.value();
	const QUrl canonicalUrl(preview.canonicalUrl);
	const QString domainText = previewDisplayHost(canonicalUrl);
	const QString titleText  = trimmedPreviewText(preview.title.isEmpty() ? tr("Link preview") : preview.title, 90);
	const QString descriptionText = trimmedPreviewText(preview.description, 140);
	const QString secondaryText   = trimmedPreviewText(preview.subtitle, 80);
	const bool isImagePreview     = previewKey.startsWith(QLatin1String("image:"));

	QString statusText;
	if (!preview.metadataFinished || !preview.thumbnailFinished) {
		statusText = tr("Loading preview...");
	} else if (preview.failed && descriptionText.isEmpty()) {
		statusText = tr("Preview unavailable");
	}

	if (isImagePreview) {
		spec.kind                     = PersistentChatPreviewKind::Image;
		spec.actionUrl                = persistentChatPreviewImageUrl(previewKey);
		spec.thumbnailImage           = preview.thumbnailImage;
		spec.statusText               = statusText;
		spec.showThumbnailPlaceholder = preview.thumbnailImage.isNull();
		return spec;
	}

	spec.kind        = PersistentChatPreviewKind::LinkCard;
	spec.actionUrl   = canonicalUrl;
	spec.title       = titleText;
	spec.description = !descriptionText.isEmpty()
						   ? descriptionText
						   : ((secondaryText.isEmpty() || secondaryText == domainText) ? QString() : secondaryText);
	spec.subtitle       = domainText;
	spec.statusText     = statusText;
	spec.thumbnailImage = preview.thumbnailImage;
	spec.showThumbnailPlaceholder =
		preview.thumbnailImage.isNull() && (!preview.metadataFinished || !preview.thumbnailFinished);
	return spec;
}

void MainWindow::updatePersistentChatPreviewViewIfVisible(const QString &previewKey) {
	mumble::chatperf::ScopedDuration trace("chat.preview.update_visible");
	if (!m_persistentChatHistory) {
		return;
	}

	if (m_persistentChatScrollIdleTimer && m_persistentChatScrollIdleTimer->isActive()) {
		for (const MumbleProto::ChatMessage &message : m_persistentChatMessages) {
			if (const std::optional< QString > messagePreviewKey = persistentChatPreviewKey(message);
				messagePreviewKey && *messagePreviewKey == previewKey) {
				m_persistentChatPreviewRefreshPending = true;
				return;
			}
		}
		return;
	}

	bool hasMatchingBubble    = false;
	bool updatedModelBubble   = false;
	bool updatedVisibleBubble = false;
	const bool wasAtBottom    = m_persistentChatHistory->isScrolledToBottom();
	const PersistentChatViewportAnchor viewportAnchor =
		wasAtBottom ? PersistentChatViewportAnchor() : m_persistentChatHistory->captureViewportAnchor();
	const PersistentChatPreviewSpec previewSpec = persistentChatPreviewSpec(previewKey);

	mumble::chatperf::recordValue("chat.preview.update_visible.messages", m_persistentChatMessages.size());
	for (const MumbleProto::ChatMessage &message : m_persistentChatMessages) {
		if (const std::optional< QString > messagePreviewKey = persistentChatPreviewKey(message);
			messagePreviewKey && *messagePreviewKey == previewKey) {
			hasMatchingBubble = true;
			if (m_persistentChatHistoryModel) {
				updatedModelBubble = m_persistentChatHistoryModel->updateBubblePreview(message.message_id(),
																					   message.thread_id(), previewSpec)
									 || updatedModelBubble;
			}
			if (m_persistentChatHistoryDelegate && m_persistentChatHistoryModel) {
				updatedVisibleBubble =
					m_persistentChatHistoryDelegate->updateBubblePreview(
						m_persistentChatHistoryModel, message.message_id(), message.thread_id(), previewSpec)
					|| updatedVisibleBubble;
			}
		}
	}

	if (!hasMatchingBubble) {
		return;
	}

	if (updatedVisibleBubble) {
		m_persistentChatHistory->stabilizeVisibleContent();
		if (!wasAtBottom && viewportAnchor.isValid()) {
			m_persistentChatHistory->restoreViewportAnchor(viewportAnchor);
		} else if (wasAtBottom) {
			if (QScrollBar *scrollBar = m_persistentChatHistory->verticalScrollBar()) {
				scrollBar->setValue(scrollBar->maximum());
			}
		}
		if (QWidget *viewport = m_persistentChatHistory->viewport()) {
			viewport->update();
		}
		return;
	}

	if (updatedModelBubble) {
		return;
	}

	renderPersistentChatView(QString(), wasAtBottom, !wasAtBottom);
}

void MainWindow::renderPersistentChatView(const QString &statusMessage, bool scrollToBottom,
										  bool preserveScrollPosition) {
	m_pendingPersistentChatRender =
		PersistentChatRenderRequest{ statusMessage, scrollToBottom, preserveScrollPosition };
	if (m_persistentChatRenderQueued) {
		return;
	}

	m_persistentChatRenderQueued = true;
	QPointer< MainWindow > guardedThis(this);
	QMetaObject::invokeMethod(
		this,
		[guardedThis]() {
			if (!guardedThis) {
				return;
			}

			guardedThis->flushPersistentChatRender();
		},
		Qt::QueuedConnection);
}

void MainWindow::flushPersistentChatRender() {
	m_persistentChatRenderQueued = false;
	if (!m_pendingPersistentChatRender) {
		return;
	}

	const PersistentChatRenderRequest request = *m_pendingPersistentChatRender;
	m_pendingPersistentChatRender.reset();
	renderPersistentChatViewImmediately(request.statusMessage, request.scrollToBottom, request.preserveScrollPosition);
}

void MainWindow::renderPersistentChatViewImmediately(const QString &statusMessage, bool scrollToBottom,
													 bool preserveScrollPosition) {
	mumble::chatperf::ScopedDuration trace("chat.render.immediate");
	if (!m_persistentChatHistory || !m_persistentChatHistoryModel) {
		return;
	}
	setPersistentChatContentMode(false);
	m_lastPersistentChatViewportWidth = m_persistentChatHistory->viewport()->width();
	const bool enforceBottomLock      = m_persistentChatBottomLockRendersRemaining > 0 && !m_persistentChatLoadingOlder;
	if (enforceBottomLock) {
		scrollToBottom         = true;
		preserveScrollPosition = false;
	}

	const PersistentChatTarget target = currentPersistentChatTarget();
	const bool showInlinePreviews     = Global::get().s.bEnableLinkPreviews && target.scope != MumbleProto::Aggregate;
	const bool canDeleteMessages      = canDeletePersistentChatMessages(target, true);
	m_persistentChatInlineDataImageSources.clear();
	if (mumble::chatperf::enabled()) {
		mumble::chatperf::recordNote(
			"chat.render.target",
			QString::fromLatin1("scope=%1 scope_id=%2 label=\"%3\" messages=%4 unread=%5 has_more=%6 loading_older=%7")
				.arg(static_cast< int >(target.scope))
				.arg(target.scopeID)
				.arg(target.label)
				.arg(m_persistentChatMessages.size())
				.arg(persistentChatUnreadCount())
				.arg(m_visiblePersistentChatHasMore ? 1 : 0)
				.arg(m_persistentChatLoadingOlder ? 1 : 0));
	}

	PersistentChatViewportAnchor viewportAnchor;
	if (m_persistentChatRestoreAnchorPending && !m_persistentChatPendingAnchorRowId.isEmpty()) {
		viewportAnchor.rowId          = m_persistentChatPendingAnchorRowId;
		viewportAnchor.intraRowOffset = m_persistentChatPendingAnchorOffset;
		preserveScrollPosition        = true;
	} else if (preserveScrollPosition) {
		viewportAnchor = m_persistentChatHistory->captureViewportAnchor();
	}

	mumble::chatperf::recordValue("chat.render.messages", m_persistentChatMessages.size());
	QVector< PersistentChatHistoryRow > rows;
	auto makeHash = [](const QStringList &parts) { return QString::number(qHash(parts.join(QLatin1String("||")))); };
	auto addStateCard = [&](const QString &eyebrow, const QString &title, const QString &body,
							const QStringList &hints) {
		PersistentChatHistoryRow row;
		row.kind      = PersistentChatHistoryRowKind::State;
		row.rowId     = QString::fromLatin1("state:%1:%2").arg(eyebrow, title);
		row.signature = makeHash(QStringList{ eyebrow, title, body, hints.join(QLatin1Char('|')) });
		PersistentChatStateRowSpec stateSpec;
		stateSpec.eyebrow       = eyebrow;
		stateSpec.title         = title;
		stateSpec.body          = body;
		stateSpec.hints         = hints;
		stateSpec.minimumHeight = std::max(220, m_persistentChatHistory->viewport()->height() - 4);
		row.state               = stateSpec;
		rows.push_back(row);
	};
	auto addUnreadDivider = [&](const QString &text, const QString &rowId) {
		PersistentChatHistoryRow row;
		row.kind      = PersistentChatHistoryRowKind::UnreadDivider;
		row.rowId     = rowId;
		row.signature = makeHash(QStringList{ rowId, text });
		row.text      = PersistentChatTextRowSpec{ text, PersistentChatDisplayMode::Bubble };
		rows.push_back(row);
	};
	// Keep voice-room chat on the same bubble rendering path as text rooms.
	const bool compactTranscriptMode = false;
	auto addDateDivider              = [&](const QString &text, const QString &rowId) {
        PersistentChatHistoryRow row;
        row.kind      = PersistentChatHistoryRowKind::DateDivider;
        row.rowId     = rowId;
        row.signature = makeHash(QStringList{ rowId, text });
        row.text = PersistentChatTextRowSpec{ text, compactTranscriptMode ? PersistentChatDisplayMode::CompactTranscript
                                                                          : PersistentChatDisplayMode::Bubble };
        rows.push_back(row);
	};
	auto addLoadOlder = [&](const QString &text, bool loading, bool enabled) {
		PersistentChatHistoryRow row;
		row.kind      = PersistentChatHistoryRowKind::LoadOlder;
		row.rowId     = QString::fromLatin1("load-older:%1").arg(m_visiblePersistentChatOldestMessageID);
		row.signature = makeHash(QStringList{ row.rowId, text, loading ? QStringLiteral("1") : QStringLiteral("0"),
											  enabled ? QStringLiteral("1") : QStringLiteral("0") });
		row.loadOlder = PersistentChatLoadOlderRowSpec{ text, loading, enabled };
		rows.push_back(row);
	};

	auto buildRenderGroups = [&](std::size_t beginIndex, std::size_t endIndex,
								 const PersistentChatRender::SelfIdentity &selfIdentity) {
		std::vector< MumbleProto::ChatMessage > slice(m_persistentChatMessages.begin() + beginIndex,
													  m_persistentChatMessages.begin() + endIndex);
		auto groups = PersistentChatRender::buildGroups(slice, selfIdentity, compactTranscriptMode);
		for (PersistentChatRender::PersistentChatRenderGroup &group : groups) {
			for (PersistentChatRender::PersistentChatRenderBubble &bubble : group.bubbles) {
				bubble.messageIndex += static_cast< int >(beginIndex);
			}
		}
		return groups;
	};

	if (m_persistentChatMessages.empty()) {
		const bool suppressRoomEmptyState =
			statusMessage.isEmpty() && !target.readOnly && !target.directMessage && !target.legacyTextPath
			&& (target.scope == MumbleProto::TextChannel || target.scope == MumbleProto::Channel);
		if (suppressRoomEmptyState) {
			m_persistentChatRestoreAnchorPending = false;
			m_persistentChatPendingAnchorRowId.clear();
			m_persistentChatPendingAnchorOffset = 0;
		} else {
			QString eyebrow = tr("Text");
			QString title;
			QString body;
			QStringList hints;
			if (target.scope == MumbleProto::ServerGlobal || target.scope == MumbleProto::Aggregate) {
				eyebrow = tr("Legacy");
			} else if (target.scope == MumbleProto::TextChannel) {
				eyebrow = tr("Room");
			}

			if (!statusMessage.isEmpty()) {
				title = target.label.isEmpty() ? tr("Loading conversation") : tr("Loading %1").arg(target.label);
				body  = statusMessage;
				hints << tr("Fetching recent messages and read state");
			} else if (target.readOnly && !target.statusMessage.trimmed().isEmpty()) {
				title = tr("Read-only conversation");
				body  = target.statusMessage;
				hints << tr("Choose another room to reply");
			} else if (target.scope == MumbleProto::Aggregate) {
				title = tr("Nothing here yet");
				body  = tr("Combined activity only shows conversations you can currently read.");
				hints << tr("Unread state stays with each room");
			} else {
				title = tr("Nothing here yet");
				body  = target.readOnly ? tr("No accessible messages yet.")
									   : tr("No messages in %1 yet.").arg(target.label);
				if (target.readOnly) {
					hints << tr("Choose another room to reply");
				} else {
					hints << tr("Start the first message below");
					hints << tr("Messages stay with this room");
				}
			}

			addStateCard(eyebrow, title, body, hints);
		}
	} else {
		qsizetype largestSourceChars        = -1;
		qsizetype largestRenderedChars      = -1;
		unsigned int largestMessageID       = 0;
		unsigned int largestMessageThreadID = 0;
		QString largestMessageSourceSample;
		bool largestMessageHasImgTag       = false;
		bool largestMessageHasDataImage    = false;
		bool largestMessageHasBase64Marker = false;
		if (m_persistentChatLoadingOlder) {
			addLoadOlder(tr("Loading older messages..."), true, false);
		} else if (m_visiblePersistentChatHasMore) {
			addLoadOlder(tr("Load older messages"), false, true);
		}

		const std::size_t unreadCount = persistentChatUnreadCount();
		std::optional< std::size_t > firstUnreadIndex;
		if (unreadCount > 0 && target.scope != MumbleProto::Aggregate) {
			for (std::size_t i = 0; i < m_persistentChatMessages.size(); ++i) {
				if (m_persistentChatMessages[i].message_id() > m_visiblePersistentChatLastReadMessageID) {
					firstUnreadIndex = i;
					break;
				}
			}
		}

		PersistentChatRender::SelfIdentity selfIdentity;
		selfIdentity.session = Global::get().uiSession;
		if (const ClientUser *self = ClientUser::get(Global::get().uiSession)) {
			selfIdentity.userID = self->iId;
			selfIdentity.name   = self->qsName;
		}

		std::vector< PersistentChatRender::PersistentChatRenderGroup > renderGroups;
		std::optional< std::size_t > unreadGroupBoundary;
		if (firstUnreadIndex && *firstUnreadIndex > 0 && *firstUnreadIndex < m_persistentChatMessages.size()) {
			auto readGroups = buildRenderGroups(0, *firstUnreadIndex, selfIdentity);
			renderGroups.insert(renderGroups.end(), readGroups.begin(), readGroups.end());
			unreadGroupBoundary = renderGroups.size();
			auto unreadGroups   = buildRenderGroups(*firstUnreadIndex, m_persistentChatMessages.size(), selfIdentity);
			renderGroups.insert(renderGroups.end(), unreadGroups.begin(), unreadGroups.end());
		} else {
			renderGroups = buildRenderGroups(0, m_persistentChatMessages.size(), selfIdentity);
		}

		QDate previousDate;
		bool hasRenderedDateSeparator = false;
		mumble::chatperf::recordValue("chat.render.groups", renderGroups.size());
		for (std::size_t groupIndex = 0; groupIndex < renderGroups.size(); ++groupIndex) {
			const PersistentChatRender::PersistentChatRenderGroup &group = renderGroups[groupIndex];
			if (unreadGroupBoundary && *unreadGroupBoundary == groupIndex) {
				addUnreadDivider(tr("New since last read"),
								 QString::fromLatin1("unread:%1").arg(m_visiblePersistentChatLastReadMessageID));
			}

			const QDate messageDate = group.date;
			if (!hasRenderedDateSeparator || previousDate != messageDate) {
				addDateDivider(persistentChatDateLabel(messageDate),
							   QString::fromLatin1("date:%1").arg(messageDate.isValid()
																	  ? messageDate.toString(Qt::ISODate)
																	  : QStringLiteral("invalid")));
				previousDate             = messageDate;
				hasRenderedDateSeparator = true;
			}

			const MumbleProto::ChatMessage &firstMessage = m_persistentChatMessages[group.bubbles.front().messageIndex];
			PersistentChatGroupHeaderSpec headerSpec;
			headerSpec.selfAuthored   = group.selfAuthored;
			headerSpec.aggregateScope = target.scope == MumbleProto::Aggregate;
			headerSpec.systemMessage  = group.systemMessage;
			headerSpec.displayMode    = compactTranscriptMode ? PersistentChatDisplayMode::CompactTranscript
														   : PersistentChatDisplayMode::Bubble;
			headerSpec.actorLabel            = persistentChatActorLabel(firstMessage);
			headerSpec.actorColor            = persistentChatActorAccentColor(headerSpec.actorLabel);
			headerSpec.avatarForegroundColor = headerSpec.actorColor;
			headerSpec.avatarBackgroundColor = persistentChatActorAccentBackground(headerSpec.actorColor);
			headerSpec.timeLabel             = persistentChatTimestampLabel(group.startedAt);
			headerSpec.timeToolTip           = persistentChatTimestampToolTip(group.startedAt);
			headerSpec.scope                 = group.scope;
			headerSpec.scopeID               = group.scopeID;
			headerSpec.scopeLabel            = target.scope == MumbleProto::Aggregate
										? persistentChatScopeLabel(group.scope, group.scopeID)
										: QString();

			const QString avatarFallbackText =
				group.systemMessage ? QString() : persistentChatInitials(headerSpec.actorLabel);
			PersistentChatHistoryRow row;
			row.kind  = PersistentChatHistoryRowKind::MessageGroup;
			row.rowId = QString::fromLatin1("group:%1:%2").arg(group.firstMessageID).arg(group.lastThreadID);
			PersistentChatMessageGroupRowSpec groupRowSpec;
			groupRowSpec.header             = headerSpec;
			groupRowSpec.avatarFallbackText = avatarFallbackText;
			groupRowSpec.firstMessageID     = group.firstMessageID;
			groupRowSpec.firstThreadID      = group.lastThreadID;
			QStringList groupSignatureParts{ row.rowId, headerSpec.actorLabel, headerSpec.timeLabel,
											 headerSpec.scopeLabel,
											 QString::number(static_cast< int >(headerSpec.displayMode)) };

			for (const PersistentChatRender::PersistentChatRenderBubble &bubble : group.bubbles) {
				const MumbleProto::ChatMessage &message = m_persistentChatMessages[bubble.messageIndex];
				const bool deletedMessage               = message.has_deleted_at() && message.deleted_at() > 0;
				QString bodyHtml;
				QString messageMarkupSource;
				QVector< QPair< QUrl, QImage > > bubbleImageResources;
				std::optional< PersistentChatReplyReference > replyReference;
				if (bubble.systemMessage) {
					const QString systemText =
						persistentChatSystemMessageText(message).value_or(tr("System event")).toHtmlEscaped();
					const QColor systemDividerColor = activeUiThemeTokens()
														  ? activeUiThemeTokens()->surface1
														  : m_persistentChatHistory->palette().color(QPalette::Mid);
					const QColor systemTextColor = activeUiThemeTokens()
													   ? activeUiThemeTokens()->overlay0
													   : m_persistentChatHistory->palette().color(QPalette::Mid);
					bodyHtml = QString::fromLatin1(
								   "<table width='100%%' cellspacing='0' cellpadding='0' "
								   "style='margin:4px 0; border:none; background:transparent;'>"
								   "<tr>"
								   "<td style='border-top:1px solid %1;'></td>"
								   "<td style='padding:0 12px; text-align:center; color:%2; font-style:italic; "
								   "font-size:0.8em; white-space:nowrap;'>%3</td>"
								   "<td style='border-top:1px solid %1;'></td>"
								   "</tr>"
								   "</table>")
								   .arg(systemDividerColor.name(QColor::HexArgb), systemTextColor.name(QColor::HexArgb),
										systemText);
				} else if (deletedMessage) {
					bodyHtml = QString::fromLatin1("<em>%1</em>").arg(tr("[message deleted]").toHtmlEscaped());
				} else {
					QString rawBodyHtml = persistentChatMessageRawBody(message);
					messageMarkupSource = rawBodyHtml;
					replyReference      = resolvedPersistentChatReplyReference(m_persistentChatMessages, message);
					if (!replyReference && !message.has_body_text()) {
						replyReference = extractPersistentChatReplyReference(rawBodyHtml, &rawBodyHtml);
					}
					const auto buildInlineDataImageReplacement = [this, &bubbleImageResources](
																	 const QString &source, const QString &altText,
																	 const PersistentChatInlineDataImageInfo &info) {
						mumble::chatperf::recordNote("chat.inline_data_image.builder",
													 QString::fromLatin1("estimated=%1 alt=\"%2\" prefix=\"%3\"")
														 .arg(info.estimatedBytes)
														 .arg(altText.left(48))
														 .arg(source.left(64)));
						const QImage previewImage = persistentChatInlineDataImagePreviewImage(source, info);
						if (previewImage.isNull()) {
							mumble::chatperf::recordValue("chat.inline_data_image.preview_failed", 1);
							return persistentChatInlineDataImagePlaceholderHtml(
								info, altText,
								persistentChatInlineDataImageOpenUrl(
									registerPersistentChatInlineDataImageSource(source))
									.toString(QUrl::FullyEncoded));
						}

						const QString token    = registerPersistentChatInlineDataImageSource(source);
						const QUrl openUrl     = persistentChatInlineDataImageOpenUrl(token);
						const QUrl resourceUrl = persistentChatInlineDataImageResourceUrl(token);
						bubbleImageResources.push_back(qMakePair(resourceUrl, previewImage));
						mumble::chatperf::recordValue("chat.inline_data_image.preview_ready", 1);
						return persistentChatInlineDataImageThumbnailHtml(resourceUrl.toString(QUrl::FullyEncoded),
																		  openUrl.toString(QUrl::FullyEncoded), altText,
																		  previewImage.size(), info.estimatedBytes);
					};
					bodyHtml = message.has_body_text()
								   ? persistentChatMessageBodyHtml(message, buildInlineDataImageReplacement)
								   : persistentChatContentHtml(rawBodyHtml, buildInlineDataImageReplacement);
				}

				if (!deletedMessage && message.has_edited_at() && message.edited_at() > 0) {
					bodyHtml += QString::fromLatin1(" <em>%1</em>").arg(tr("(edited)").toHtmlEscaped());
				}
				const QString messageSourceText =
					bubble.systemMessage ? persistentChatSystemMessageText(message).value_or(QString())
										 : (deletedMessage ? QString() : persistentChatMessageSourceText(message));
				if (messageSourceText.size() > 100000 || bodyHtml.size() > 10000) {
					mumble::chatperf::recordNote(
						"chat.inline_data_image.body_html",
						QString::fromLatin1("message_id=%1 source_chars=%2 body_chars=%3 has_data_image=%4 "
											"has_mumble_chat=%5 has_thumb_data=%6")
							.arg(message.message_id())
							.arg(messageSourceText.size())
							.arg(bodyHtml.size())
							.arg(bodyHtml.contains(QLatin1String("data:image"), Qt::CaseInsensitive) ? 1 : 0)
							.arg(
								bodyHtml.contains(QLatin1String("mumble-chat://inline-data-image"), Qt::CaseInsensitive)
									? 1
									: 0)
							.arg(bodyHtml.contains(QLatin1String("Click to open or save"), Qt::CaseInsensitive) ? 1
																												: 0));
				}
				bodyHtml = persistentChatCondensedBodyHtml(bodyHtml, messageSourceText);
				if (messageSourceText.size() > largestSourceChars) {
					largestSourceChars         = messageSourceText.size();
					largestRenderedChars       = bodyHtml.size();
					largestMessageID           = message.message_id();
					largestMessageThreadID     = message.thread_id();
					largestMessageSourceSample = messageSourceText.left(120);
					largestMessageHasImgTag = messageMarkupSource.contains(QLatin1String("<img"), Qt::CaseInsensitive);
					largestMessageHasDataImage =
						messageMarkupSource.contains(QLatin1String("data:image"), Qt::CaseInsensitive);
					largestMessageHasBase64Marker =
						messageMarkupSource.contains(QLatin1String("base64,"), Qt::CaseInsensitive)
						|| messageSourceText.contains(QLatin1String("base64,"), Qt::CaseInsensitive);
				}

				PersistentChatBubbleSpec bubbleSpec;
				bubbleSpec.messageID      = message.message_id();
				bubbleSpec.threadID       = message.thread_id();
				bubbleSpec.displayMode    = headerSpec.displayMode;
				bubbleSpec.bodyHtml       = bodyHtml;
				bubbleSpec.imageResources = bubbleImageResources;
				bubbleSpec.selfAuthored   = bubble.selfAuthored;
				bubbleSpec.copyText       = bubble.systemMessage
										  ? persistentChatSystemMessageText(message).value_or(QString())
										  : persistentChatMessageSourceText(message);
				bubbleSpec.systemMessage = bubble.systemMessage;
				bubbleSpec.timeToolTip   = persistentChatTimestampToolTip(bubble.createdAt);
				bubbleSpec.replyEnabled =
					!bubble.systemMessage && (target.scope == MumbleProto::Aggregate || !deletedMessage);
				bubbleSpec.deleteEnabled  = canDeleteMessages && !bubble.systemMessage && !deletedMessage;
				bubbleSpec.readOnlyAction = target.scope == MumbleProto::Aggregate;
				bubbleSpec.actionText     = bubble.systemMessage
											? QString()
											: (target.scope == MumbleProto::Aggregate ? tr("Open room") : tr("Reply"));
				bubbleSpec.actionScope          = message.has_scope() ? message.scope() : MumbleProto::Channel;
				bubbleSpec.actionScopeID        = message.has_scope_id() ? message.scope_id() : 0;
				bubbleSpec.transcriptActorLabel = headerSpec.actorLabel;
				bubbleSpec.transcriptActorColor = headerSpec.actorColor;
				bubbleSpec.transcriptTimeLabel  = persistentChatTimestampLabel(bubble.createdAt);

				if (replyReference) {
					bubbleSpec.hasReply       = true;
					bubbleSpec.replyMessageID = replyReference->messageID;
					bubbleSpec.replyActor     = replyReference->actor;
					bubbleSpec.replySnippet   = replyReference->snippet;
				}

				if (!bubble.systemMessage && !deletedMessage && showInlinePreviews) {
					if (const std::optional< QString > previewKey = persistentChatPreviewKey(message); previewKey) {
						bubbleSpec.previewKey  = *previewKey;
						bubbleSpec.previewSpec = persistentChatPreviewSpec(*previewKey);
						if (persistentChatSourceTextIsSinglePreviewableUrl(messageSourceText)) {
							bubbleSpec.bodyHtml = persistentChatPreviewSourceUrlHtml(messageSourceText);
						}
					}
				}

				groupSignatureParts << QString::number(bubbleSpec.messageID)
									<< QString::number(static_cast< int >(bubbleSpec.displayMode))
									<< bubbleSpec.bodyHtml << bubbleSpec.transcriptActorLabel
									<< bubbleSpec.transcriptTimeLabel << bubbleSpec.replyActor
									<< bubbleSpec.replySnippet << bubbleSpec.actionText
									<< (bubbleSpec.deleteEnabled ? QStringLiteral("1") : QStringLiteral("0"))
									<< QString::number(static_cast< int >(bubbleSpec.previewSpec.kind));
				groupRowSpec.bubbles.push_back(bubbleSpec);
			}

			row.signature    = makeHash(groupSignatureParts);
			row.messageGroup = groupRowSpec;
			rows.push_back(row);
		}

		if (mumble::chatperf::enabled() && largestSourceChars >= 0) {
			mumble::chatperf::recordNote(
				"chat.render.max_message",
				QString::fromLatin1(
					"scope=%1 scope_id=%2 label=\"%3\" message_id=%4 thread_id=%5 source_chars=%6 rendered_chars=%7")
					.arg(static_cast< int >(target.scope))
					.arg(target.scopeID)
					.arg(target.label)
					.arg(largestMessageID)
					.arg(largestMessageThreadID)
					.arg(largestSourceChars)
					.arg(largestRenderedChars));
			mumble::chatperf::recordNote(
				"chat.render.max_message.flags",
				QString::fromLatin1("message_id=%1 img_tag=%2 data_image=%3 base64=%4 sample=\"%5\"")
					.arg(largestMessageID)
					.arg(largestMessageHasImgTag ? 1 : 0)
					.arg(largestMessageHasDataImage ? 1 : 0)
					.arg(largestMessageHasBase64Marker ? 1 : 0)
					.arg(largestMessageSourceSample));
		}
	}

	mumble::chatperf::recordValue("chat.render.rows", rows.size());
	m_persistentChatHistoryModel->setRows(rows);
	m_persistentChatHistory->stabilizeVisibleContent();

	if (preserveScrollPosition && viewportAnchor.isValid()) {
		m_persistentChatHistory->restoreViewportAnchor(viewportAnchor);
	} else if (scrollToBottom) {
		m_persistentChatHistory->verticalScrollBar()->setValue(m_persistentChatHistory->verticalScrollBar()->maximum());
	} else {
		m_persistentChatHistory->verticalScrollBar()->setValue(0);
	}
	m_persistentChatRestoreAnchorPending = false;
	m_persistentChatPendingAnchorRowId.clear();
	m_persistentChatPendingAnchorOffset = 0;

	if (enforceBottomLock && m_persistentChatBottomLockRendersRemaining > 0) {
		--m_persistentChatBottomLockRendersRemaining;
	}

	queueModernShellSnapshotSync();
}

bool MainWindow::canMarkPersistentChatRead(bool willScrollToBottom) const {
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	const bool modernShellVisible = usesModernShell() && m_modernShellHost && m_modernShellHost->isVisible();
#else
	const bool modernShellVisible = false;
#endif
	return m_visiblePersistentChatScope && *m_visiblePersistentChatScope != MumbleProto::Aggregate
		   && !m_persistentChatMessages.empty() && Global::get().sh && Global::get().sh->isRunning() && isActiveWindow()
		   && m_persistentChatHistory && (modernShellVisible || qdwChat->isVisible())
		   && (modernShellVisible || willScrollToBottom
			   || (m_persistentChatHistoryModel
				   && m_persistentChatHistory->isRowVisible(m_persistentChatHistoryModel->lastMessageGroupRowId())));
}

std::size_t MainWindow::persistentChatUnreadCount() const {
	if (!m_persistentChatController || !m_visiblePersistentChatScope
		|| *m_visiblePersistentChatScope == MumbleProto::Aggregate) {
		return 0;
	}

	return static_cast< std::size_t >(
		m_persistentChatController->unreadCount(*m_visiblePersistentChatScope, m_visiblePersistentChatScopeID));
}

bool MainWindow::markPersistentChatRead(bool rerender, bool willScrollToBottom) {
	if (!m_persistentChatController || !canMarkPersistentChatRead(willScrollToBottom)) {
		return false;
	}

	if (!m_persistentChatController->markActiveScopeRead()) {
		return false;
	}

	m_visiblePersistentChatLastReadMessageID =
		m_persistentChatMessages.empty() ? 0 : m_persistentChatMessages.back().message_id();
	if (rerender) {
		renderPersistentChatView(QString(), true, false);
	}
	queueModernShellSnapshotSync();
	return true;
}

void MainWindow::refreshPersistentChatView(bool forceReload) {
	const PersistentChatTarget target = currentPersistentChatTarget();
	syncPersistentChatGatewayHandler();

	if (!target.valid) {
		if (m_persistentChatController) {
			m_persistentChatController->clearActiveScope();
		}
		clearPersistentChatView(target.label.isEmpty() ? tr("Not connected") : target.label, tr("Start a conversation"),
								{ tr("Open Server to connect"), tr("Room chat and history appear here") });
		return;
	}

	if (target.serverLog) {
		if (m_persistentChatController) {
			m_persistentChatController->clearActiveScope();
		}
		renderServerLogView(!forceReload);
		return;
	}

	if (target.legacyTextPath) {
		if (m_persistentChatController) {
			m_persistentChatController->clearActiveScope();
		}
		renderLegacyActivityView(!forceReload);
		return;
	}

	if (target.directMessage) {
		if (m_persistentChatController) {
			m_persistentChatController->clearActiveScope();
		}
		clearPersistentChatView(tr("Direct messages still use the classic text message path and do not have persistent "
								   "history yet."),
								tr("Direct messages"), { tr("Classic chat still handles direct messages") });
		return;
	}

	if (target.scope == MumbleProto::ServerGlobal && !Global::get().bPersistentGlobalChatEnabled) {
		if (m_persistentChatController) {
			m_persistentChatController->clearActiveScope();
		}
		clearPersistentChatView(
			target.statusMessage.isEmpty() ? tr("Legacy server-wide chat is disabled by this server.")
										   : target.statusMessage,
			tr("Server-wide chat is unavailable"), { tr("Choose a text room or voice room chat instead") });
		return;
	}

	if (target.scope == MumbleProto::Aggregate || target.scope == MumbleProto::ServerGlobal) {
		if (m_persistentChatController) {
			m_persistentChatController->clearActiveScope();
		}
		clearPersistentChatView(
			target.statusMessage.isEmpty() ? tr("This legacy conversation is no longer shown in the text room list.")
										   : target.statusMessage,
			tr("Conversation unavailable"), { tr("Choose a text room or voice room chat instead") });
		return;
	}

	const bool targetChanged =
		!m_persistentChatController || !m_persistentChatController->activeScopeMatches(target.scope, target.scopeID);
	if (!forceReload && !targetChanged) {
		return;
	}

	if (targetChanged) {
		clearPersistentChatReplyTarget(false);
	}

	m_persistentChatBottomLockRendersRemaining = 3;
	if (!m_persistentChatController) {
		clearPersistentChatView(tr("Persistent chat controller is unavailable."), tr("Conversation unavailable"));
		return;
	}

	m_persistentChatController->setActiveScope(PersistentChatScopeKey::fromScope(target.scope, target.scopeID),
											   forceReload);
	const PersistentChatScopeStateSnapshot snapshot = m_persistentChatController->activeSnapshot();
	if (!snapshot.initialLoaded && snapshot.messages.isEmpty()) {
		renderPersistentChatView(tr("Loading %1...").arg(target.label));
		return;
	}

	renderPersistentChatView(QString(), m_persistentChatHistory ? m_persistentChatHistory->isScrolledToBottom() : true,
							 false);
}

void MainWindow::renderServerLogView(bool preserveScrollPosition) {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		appendModernShellConnectTrace(
			QStringLiteral("UI renderServerLogView minimal-skip preserve=%1").arg(preserveScrollPosition ? 1 : 0));
		return;
	}

	if (!m_persistentChatLogView) {
		clearPersistentChatView(tr("Server log is unavailable."), tr("Activity"));
		return;
	}

	setPersistentChatContentMode(true, preserveScrollPosition);
}

void MainWindow::requestOlderPersistentChatHistory() {
	const PersistentChatTarget target = currentPersistentChatTarget();
	if (!target.valid || target.directMessage || !m_visiblePersistentChatScope || !m_visiblePersistentChatHasMore
		|| m_persistentChatLoadingOlder || !m_persistentChatController) {
		return;
	}

	if (m_persistentChatHistory) {
		const PersistentChatViewportAnchor anchor = m_persistentChatHistory->captureViewportAnchor();
		if (anchor.isValid()) {
			m_persistentChatRestoreAnchorPending = true;
			m_persistentChatPendingAnchorRowId   = anchor.rowId;
			m_persistentChatPendingAnchorOffset  = anchor.intraRowOffset;
		}
	}

	if (!m_persistentChatController->requestOlderForActiveScope()) {
		m_persistentChatRestoreAnchorPending = false;
		m_persistentChatPendingAnchorRowId.clear();
		m_persistentChatPendingAnchorOffset = 0;
	}
}

void MainWindow::attachPersistentChatImages(const QList< QUrl > &urls) {
	if (!qteChat || !qteChat->isEnabled() || !Global::get().bAllowHTML || urls.isEmpty()) {
		return;
	}

	qteChat->sendImagesFromUrls(urls);
}

bool MainWindow::attachPersistentChatClipboardImage() {
	if (!qteChat || !qteChat->isEnabled() || !Global::get().bAllowHTML) {
		return false;
	}

	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData   = clipboard ? clipboard->mimeData() : nullptr;
	if (!mimeData) {
		return false;
	}

	if (mimeData->hasImage()) {
		const QVariant imageData = mimeData->imageData();
		QImage image             = qvariant_cast< QImage >(imageData);
		if (image.isNull()) {
			const QPixmap pixmap = qvariant_cast< QPixmap >(imageData);
			if (!pixmap.isNull()) {
				image = pixmap.toImage();
			}
		}

		if (!image.isNull()) {
			attachPersistentChatImage(image);
			return true;
		}
	}

	if (mimeData->hasUrls()) {
		QList< QUrl > imageUrls;
		const QList< QUrl > urls = mimeData->urls();
		for (const QUrl &url : urls) {
			if (!url.isLocalFile()) {
				continue;
			}

			const QString localPath = url.toLocalFile();
			if (QImageReader::imageFormat(localPath).isEmpty()) {
				continue;
			}

			imageUrls.push_back(url);
		}

		if (!imageUrls.isEmpty()) {
			attachPersistentChatImages(imageUrls);
			return true;
		}
	}

	return false;
}

void MainWindow::attachPersistentChatImage(const QImage &image) {
	if (!qteChat || !qteChat->isEnabled() || !Global::get().bAllowHTML || image.isNull()) {
		return;
	}

	const QString processedImage = Log::imageToImg(image, static_cast< int >(Global::get().uiImageLength));
	if (processedImage.isEmpty()) {
		Global::get().l->log(Log::Information, tr("Unable to send image: too large."));
		return;
	}

	sendChatbarMessage(processedImage);
}

bool MainWindow::attachPersistentChatImageData(const QString &dataUrl) {
	if (dataUrl.trimmed().isEmpty() || !dataUrl.startsWith(QLatin1String("data:image/"), Qt::CaseInsensitive)) {
		return false;
	}

	const QImage image = persistentChatInlineDataImageFromSource(dataUrl);
	if (image.isNull()) {
		return false;
	}

	attachPersistentChatImage(image);
	return true;
}

#if defined(MUMBLE_HAS_MODERN_LAYOUT)
void MainWindow::togglePreferredModernShellLayout() {
	if (Global::get().s.modernLayoutPolicy == Settings::ModernLayoutForced) {
		if (Global::get().s.wlWindowLayout == Settings::LayoutModern) {
			Global::get().s.wlWindowLayout = Settings::LayoutHybrid;
			Global::get().s.save();
		}
		queueModernShellSnapshotSync();
		return;
	}

	Global::get().s.wlWindowLayout =
		effectiveWindowLayout() == Settings::LayoutModern ? Settings::LayoutClassic : Settings::LayoutModern;
	if (Global::get().s.wlWindowLayout == Settings::LayoutModern) {
		Global::get().s.modernLayoutPolicy = Settings::ModernLayoutForced;
		Global::get().s.wlWindowLayout     = Settings::LayoutHybrid;
	}
	Global::get().s.save();
	refreshShellLayout();
}
#endif

void MainWindow::openPersistentChatImagePicker() {
	if (!qteChat || !qteChat->isEnabled() || !Global::get().bAllowHTML) {
		return;
	}

	QFileDialog dialog(this, tr("Attach image"));
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(tr("Images (*.png *.jpg *.jpeg *.gif *.webp *.bmp)"));
	const QString picturesLocation = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
	if (!picturesLocation.isEmpty()) {
		dialog.setDirectory(picturesLocation);
	}

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	attachPersistentChatImages(dialog.selectedUrls());
}

void MainWindow::handlePersistentChatMessage(const MumbleProto::ChatMessage &msg) {
	if (!m_persistentChatGateway) {
		return;
	}

	syncPersistentChatGatewayHandler();
	m_persistentChatGateway->handleIncomingMessage(msg);
}

void MainWindow::handlePersistentChatHistory(const MumbleProto::ChatHistoryResponse &msg) {
	mumble::chatperf::ScopedDuration trace("chat.handle.history");
	if (!m_persistentChatGateway) {
		return;
	}

	syncPersistentChatGatewayHandler();
	m_persistentChatGateway->handleIncomingHistory(msg);
}

void MainWindow::handlePersistentChatReadState(const MumbleProto::ChatReadStateUpdate &msg) {
	mumble::chatperf::ScopedDuration trace("chat.handle.read_state");
	if (!m_persistentChatGateway) {
		return;
	}

	syncPersistentChatGatewayHandler();
	m_persistentChatGateway->handleIncomingReadState(msg);
}

void MainWindow::handlePersistentChatEmbedState(const MumbleProto::ChatEmbedState &msg) {
	mumble::chatperf::ScopedDuration trace("chat.handle.embed_state");
	if (!msg.has_thread_id() || !msg.has_message_id() || !m_persistentChatController) {
		return;
	}

	const MumbleProto::ChatScope scope = msg.has_scope() ? msg.scope() : MumbleProto::Channel;
	const unsigned int scopeID         = msg.has_scope_id() ? msg.scope_id() : 0;
	const bool activeScopeMatches      = m_persistentChatController->activeScopeMatches(scope, scopeID);

	QString oldPreviewKey;
	if (activeScopeMatches) {
		if (const MumbleProto::ChatMessage *existingMessage =
				findPersistentChatMessage(m_persistentChatMessages, msg.thread_id(), msg.message_id())) {
			if (const std::optional< QString > key = persistentChatPreviewKey(*existingMessage); key) {
				oldPreviewKey = *key;
			}
		}
	}

	if (!m_persistentChatController->applyEmbedState(msg)) {
		return;
	}

	MumbleProto::ChatMessage *updatedLocalMessage = nullptr;
	QString newPreviewKey;
	if (activeScopeMatches) {
		updatedLocalMessage = findPersistentChatMessage(m_persistentChatMessages, msg.thread_id(), msg.message_id());
		if (updatedLocalMessage) {
			updatedLocalMessage->clear_embeds();
			for (const MumbleProto::ChatEmbedRef &embed : msg.embeds()) {
				*updatedLocalMessage->add_embeds() = embed;
			}

			if (const std::optional< QString > key = persistentChatPreviewKey(*updatedLocalMessage); key) {
				newPreviewKey = *key;
			}
		}
	}

	if (!oldPreviewKey.isEmpty() && oldPreviewKey != newPreviewKey) {
		m_persistentChatPreviews.remove(oldPreviewKey);
	}
	if (!newPreviewKey.isEmpty()) {
		m_persistentChatPreviews.remove(newPreviewKey);
		ensurePersistentChatPreview(newPreviewKey);
	}

	if (!activeScopeMatches || !m_persistentChatHistory) {
		return;
	}

	const bool wasAtBottom = m_persistentChatHistory->isScrolledToBottom();
	const PersistentChatViewportAnchor viewportAnchor =
		wasAtBottom ? PersistentChatViewportAnchor() : m_persistentChatHistory->captureViewportAnchor();
	const PersistentChatPreviewSpec previewSpec =
		newPreviewKey.isEmpty() ? PersistentChatPreviewSpec() : persistentChatPreviewSpec(newPreviewKey);
	const bool updatedModelBubble =
		m_persistentChatHistoryModel
		&& m_persistentChatHistoryModel->updateBubblePreview(msg.message_id(), msg.thread_id(), previewSpec);
	const bool updatedVisibleBubble =
		updatedLocalMessage && m_persistentChatHistoryDelegate && m_persistentChatHistoryModel
		&& m_persistentChatHistoryDelegate->updateBubblePreview(m_persistentChatHistoryModel, msg.message_id(),
																msg.thread_id(), previewSpec);

	if (!updatedModelBubble && !updatedVisibleBubble) {
		renderPersistentChatView(QString(), wasAtBottom, !wasAtBottom);
		return;
	}

	if (updatedVisibleBubble) {
		m_persistentChatHistory->stabilizeVisibleContent();
		if (!wasAtBottom && viewportAnchor.isValid()) {
			m_persistentChatHistory->restoreViewportAnchor(viewportAnchor);
		} else if (wasAtBottom) {
			if (QScrollBar *scrollBar = m_persistentChatHistory->verticalScrollBar()) {
				scrollBar->setValue(scrollBar->maximum());
			}
		}

		if (QWidget *viewport = m_persistentChatHistory->viewport()) {
			viewport->update();
		}
	}
}

void MainWindow::handlePersistentChatReactionState(const MumbleProto::ChatReactionState &msg) {
	mumble::chatperf::ScopedDuration trace("chat.handle.reaction_state");
	if (!msg.has_thread_id() || !msg.has_message_id() || !m_persistentChatController) {
		return;
	}

	const MumbleProto::ChatScope scope = msg.has_scope() ? msg.scope() : MumbleProto::Channel;
	const unsigned int scopeID         = msg.has_scope_id() ? msg.scope_id() : 0;
	const bool activeScopeMatches      = m_persistentChatController->activeScopeMatches(scope, scopeID);

	if (!m_persistentChatController->applyReactionState(msg)) {
		return;
	}

	if (activeScopeMatches) {
		if (MumbleProto::ChatMessage *updatedLocalMessage =
				findPersistentChatMessage(m_persistentChatMessages, msg.thread_id(), msg.message_id())) {
			updatedLocalMessage->clear_reactions();
			for (const MumbleProto::ChatReactionAggregate &reaction : msg.reactions()) {
				*updatedLocalMessage->add_reactions() = reaction;
			}
		}
	}

	queueModernShellSnapshotSync();
}

bool MainWindow::canSendToPersistentChatTarget(const PersistentChatTarget &target, bool requestPermissions) const {
	if (Global::get().uiSession == 0 || !target.valid || target.readOnly || target.serverLog) {
		return false;
	}

	if (target.scope == MumbleProto::ServerGlobal) {
		return Global::get().bPersistentGlobalChatEnabled
			   && (Global::get().pPermissions & (ChanACL::Write | ChanACL::TextMessage));
	}

	Channel *permissionChannel = nullptr;
	if (target.directMessage) {
		if (!target.user || !target.channel) {
			return false;
		}
		permissionChannel = target.channel;
	} else if (target.channel
			   && (target.scope == MumbleProto::TextChannel || target.legacyTextPath
				   || target.scope == MumbleProto::Channel)) {
		permissionChannel = target.channel;
	}

	if (!permissionChannel) {
		return true;
	}

	ChanACL::Permissions textPermissions = static_cast< ChanACL::Permissions >(permissionChannel->uiPermissions);
	if (!textPermissions && requestPermissions && Global::get().sh) {
		Global::get().sh->requestChannelPermissions(permissionChannel->iId);
		textPermissions                  = permissionChannel->iId == 0 ? Global::get().pPermissions : ChanACL::All;
		permissionChannel->uiPermissions = textPermissions;
	}

	return textPermissions & (ChanACL::Write | ChanACL::TextMessage);
}

bool MainWindow::canDeletePersistentChatMessages(const PersistentChatTarget &target, bool requestPermissions) const {
	if (Global::get().uiSession == 0 || !target.valid || target.readOnly || target.serverLog || target.directMessage
		|| target.legacyTextPath) {
		return false;
	}

	if (target.scope == MumbleProto::ServerGlobal) {
		return Global::get().bPersistentGlobalChatEnabled
			   && (Global::get().pPermissions & (ChanACL::Write | ChanACL::DeleteTextMessage));
	}

	if (target.scope != MumbleProto::TextChannel && target.scope != MumbleProto::Channel) {
		return false;
	}

	Channel *permissionChannel = target.channel;
	if (!permissionChannel) {
		return false;
	}

	ChanACL::Permissions textPermissions = static_cast< ChanACL::Permissions >(permissionChannel->uiPermissions);
	if (!textPermissions && requestPermissions && Global::get().sh) {
		Global::get().sh->requestChannelPermissions(permissionChannel->iId);
		textPermissions                  = permissionChannel->iId == 0 ? Global::get().pPermissions : ChanACL::All;
		permissionChannel->uiPermissions = textPermissions;
	}

	return textPermissions & (ChanACL::Write | ChanACL::DeleteTextMessage);
}

bool MainWindow::deletePersistentChatMessage(unsigned int messageID) {
	if (!m_persistentChatGateway || messageID == 0) {
		return false;
	}

	const PersistentChatTarget target = currentPersistentChatTarget();
	if (!canDeletePersistentChatMessages(target, true)) {
		return false;
	}

	const MumbleProto::ChatMessage *message = findPersistentChatMessageByID(m_persistentChatMessages, messageID);
	if (!message || persistentChatSystemMessageText(*message).has_value()
		|| (message->has_deleted_at() && message->deleted_at() > 0)) {
		return false;
	}

	QMessageBox confirm(this);
	confirm.setIcon(QMessageBox::Warning);
	confirm.setWindowTitle(tr("Delete message"));
	confirm.setText(tr("Delete this message from chat history?"));
	confirm.setInformativeText(tr("The message body, attachments, link previews, and reactions will be removed."));
	QPushButton *deleteButton = confirm.addButton(tr("Delete"), QMessageBox::DestructiveRole);
	confirm.addButton(QMessageBox::Cancel);
	confirm.setDefaultButton(QMessageBox::Cancel);
	confirm.exec();
	if (confirm.clickedButton() != deleteButton) {
		return false;
	}

	syncPersistentChatGatewayHandler();
	m_persistentChatGateway->deleteMessage(target.scope, target.scopeID, message->thread_id(), message->message_id());
	return true;
}

void MainWindow::syncPersistentChatInputState(bool baseEnabled) {
	if (!qteChat) {
		return;
	}

	const PersistentChatTarget target = currentPersistentChatTarget();
	bool enableInput                  = baseEnabled && target.valid && !target.readOnly;
	if (target.valid && !target.readOnly
		&& (target.directMessage || target.scope == MumbleProto::TextChannel || target.legacyTextPath
			|| target.scope == MumbleProto::Channel || target.scope == MumbleProto::ServerGlobal)) {
		enableInput = canSendToPersistentChatTarget(target, true);
	}
	qteChat->setEnabled(enableInput);
	updatePersistentChatSendButton();
}

QList< QListWidgetItem * > MainWindow::persistentChatChannelListSelectedItems() const {
	if (!m_persistentChatChannelList) {
		return {};
	}

	return m_persistentChatChannelList->selectedItems();
}

void MainWindow::cachePersistentChatChannelSelection(const QListWidgetItem *item) {
	if (!item) {
		clearPersistentChatChannelSelection();
		return;
	}

	m_persistentChatSelectedScopeValue = item->data(PersistentChatScopeRole).toInt();
	m_persistentChatSelectedScopeID    = item->data(PersistentChatScopeIDRole).toUInt();
}

void MainWindow::clearPersistentChatChannelSelection() {
	m_persistentChatSelectedScopeValue.reset();
	m_persistentChatSelectedScopeID = 0;
}

QListWidgetItem *MainWindow::findPersistentChatChannelItem(int scopeValue, unsigned int scopeID) const {
	if (!m_persistentChatChannelList) {
		return nullptr;
	}

	for (int i = 0; i < m_persistentChatChannelList->count(); ++i) {
		QListWidgetItem *item = m_persistentChatChannelList->item(i);
		if (!item) {
			continue;
		}

		if (item->data(PersistentChatScopeRole).toInt() == scopeValue
			&& item->data(PersistentChatScopeIDRole).toUInt() == scopeID) {
			return item;
		}
	}

	return nullptr;
}

QListWidgetItem *MainWindow::persistentChatChannelListCurrentItem() {
	if (!m_persistentChatChannelList || !m_persistentChatSelectedScopeValue.has_value()) {
		return nullptr;
	}

	return findPersistentChatChannelItem(*m_persistentChatSelectedScopeValue, m_persistentChatSelectedScopeID);
}

const QListWidgetItem *MainWindow::persistentChatChannelListCurrentItem() const {
	return const_cast< MainWindow * >(this)->persistentChatChannelListCurrentItem();
}

void MainWindow::updatePersistentChatChrome(const PersistentChatTarget &target) {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}

	const bool showConversationList = hasPersistentChatCapabilities();
	const bool showMotd             = !m_persistentChatWelcomeText.trimmed().isEmpty();
	const bool compactRoomHeader =
		target.valid && !target.serverLog && !target.directMessage && !target.legacyTextPath
		&& (target.scope == MumbleProto::TextChannel || target.scope == MumbleProto::Channel);
	const bool compactRailSections             = compactRoomHeader;
	const bool nativeConversationChromeVisible = qdwChat && qdwChat->isVisible();
	const bool nativeNavigatorChromeVisible    = m_serverNavigatorContainer && m_serverNavigatorContainer->isVisible();

	if (QVBoxLayout *headerLayout = qobject_cast< QVBoxLayout * >(
			m_persistentChatHeaderFrame ? m_persistentChatHeaderFrame->layout() : nullptr)) {
		headerLayout->setContentsMargins(7, 0, 7, compactRoomHeader ? 0 : 1);
		headerLayout->setSpacing(compactRoomHeader ? 0 : 1);
	}

	if (m_persistentChatHeaderEyebrow) {
		QString eyebrow = tr("Text");
		if (target.serverLog) {
			eyebrow = tr("Activity");
		} else if (target.directMessage) {
			eyebrow = tr("Direct");
		} else if (target.scope == MumbleProto::Aggregate || target.scope == MumbleProto::ServerGlobal) {
			eyebrow = tr("Legacy");
		} else if (target.scope == MumbleProto::TextChannel) {
			eyebrow = tr("Room");
		}
		m_persistentChatHeaderEyebrow->setText(eyebrow);
		if (nativeConversationChromeVisible) {
			m_persistentChatHeaderEyebrow->setVisible(!compactRoomHeader);
		}
	}

	if (m_persistentChatHeaderTitle) {
		m_persistentChatHeaderTitle->setText(target.valid ? target.label : tr("Chat"));
	}

	if (m_persistentChatHeaderContext) {
		m_persistentChatHeaderContext->clear();
		if (nativeConversationChromeVisible) {
			m_persistentChatHeaderContext->setVisible(false);
		}
	}

	QString subtitle;
	if (Global::get().uiSession == 0) {
		subtitle = tr("Join a server to chat.");
	} else if (!target.valid) {
		subtitle = target.label;
	} else if (target.serverLog) {
		subtitle = tr("Connection, notices, and diagnostics.");
	} else if (target.legacyTextPath) {
		subtitle = tr("Classic server chat.");
	} else if (target.directMessage) {
		subtitle = tr("Classic direct chat.");
	} else if (target.scope == MumbleProto::Aggregate) {
		subtitle = !target.statusMessage.trimmed().isEmpty()
					   ? target.statusMessage
					   : tr("Combined activity is retired in the new text room list.");
	} else if (target.scope == MumbleProto::ServerGlobal) {
		if (!target.description.trimmed().isEmpty()) {
			subtitle = target.description;
		} else if (const ClientUser *self = ClientUser::get(Global::get().uiSession); self && self->cChannel) {
			subtitle = tr("Legacy server-wide chat from %1.").arg(persistentTextAclChannelLabel(self->cChannel));
		} else {
			subtitle = tr("Legacy server-wide chat is retired in the new text room list.");
		}
	} else if (target.scope == MumbleProto::TextChannel && target.channel) {
		subtitle = QString();
	} else if (target.scope == MumbleProto::Channel && target.channel) {
		subtitle = QString();
	} else if (!target.statusMessage.trimmed().isEmpty()) {
		subtitle = target.statusMessage;
	} else if (!target.description.trimmed().isEmpty()) {
		subtitle = target.description;
	} else {
		subtitle = tr("Persistent chat with shared history and read state.");
	}

	if (m_persistentChatHeaderSubtitle) {
		m_persistentChatHeaderSubtitle->setText(subtitle);
		if (nativeConversationChromeVisible) {
			m_persistentChatHeaderSubtitle->setVisible(!subtitle.trimmed().isEmpty());
		}
	}
	if (m_persistentChatHeaderFrame && nativeConversationChromeVisible) {
		m_persistentChatHeaderFrame->setVisible(!compactRoomHeader);
	}
	if (nativeNavigatorChromeVisible && m_serverNavigatorTextChannelsMotdFrame && m_serverNavigatorTextChannelsMotdTitle
		&& m_serverNavigatorTextChannelsMotdBody && m_serverNavigatorTextChannelsMotdToggleButton) {
		m_serverNavigatorTextChannelsMotdFrame->setVisible(showMotd);
		if (showMotd) {
			m_serverNavigatorTextChannelsMotdTitle->setText(tr("MOTD"));
			m_serverNavigatorTextChannelsMotdBody->setVisible(true);
			static const int motdRefreshDelaysMs[] = { 0, 150, 600, 1800, 4500 };
			for (const int delayMs : motdRefreshDelaysMs) {
				QTimer::singleShot(delayMs, this, [this]() { refreshServerNavigatorSectionHeights(); });
			}
		} else {
			m_serverNavigatorTextChannelsMotdToggleButton->hide();
		}
	}

	if (m_serverNavigatorVoiceSectionEyebrow) {
		m_serverNavigatorVoiceSectionEyebrow->setText(tr("Rooms"));
		if (nativeNavigatorChromeVisible) {
			m_serverNavigatorVoiceSectionEyebrow->setVisible(!compactRailSections);
		}
	}
	if (m_serverNavigatorVoiceSectionSubtitle) {
		if (nativeNavigatorChromeVisible) {
			m_serverNavigatorVoiceSectionSubtitle->setVisible(false);
		}
		m_serverNavigatorVoiceSectionSubtitle->setText(tr("Voice rooms, people, and text rooms"));
	}
	if (m_serverNavigatorTextChannelsEyebrow) {
		if (nativeNavigatorChromeVisible) {
			m_serverNavigatorTextChannelsEyebrow->setVisible(showConversationList && !compactRailSections);
		}
		m_serverNavigatorTextChannelsEyebrow->setText(tr("Text"));
	}
	if (m_serverNavigatorTextChannelsTitle) {
		if (nativeNavigatorChromeVisible) {
			m_serverNavigatorTextChannelsTitle->setVisible(false);
		}
		m_serverNavigatorTextChannelsTitle->setText(tr("Text rooms"));
	}
	if (m_serverNavigatorTextChannelsSubtitle) {
		const bool showCreateHint =
			showConversationList && canManagePersistentTextChannels() && m_persistentTextChannels.isEmpty();
		if (nativeNavigatorChromeVisible) {
			m_serverNavigatorTextChannelsSubtitle->setVisible(showCreateHint);
		}
		m_serverNavigatorTextChannelsSubtitle->setText(showCreateHint ? tr("Right-click to create a text room")
																	  : tr("Named text rooms and voice room chat"));
	}
	if (m_persistentChatChannelList && nativeNavigatorChromeVisible) {
		m_persistentChatChannelList->setVisible(showConversationList);
	}
	if (m_serverNavigatorTextChannelsFrame && nativeNavigatorChromeVisible) {
		m_serverNavigatorTextChannelsFrame->setVisible(showConversationList);
		if (m_serverNavigatorTextChannelsDivider) {
			m_serverNavigatorTextChannelsDivider->setVisible(false);
		}
	}
	if (nativeNavigatorChromeVisible) {
		refreshServerNavigatorSectionHeights();
	}

	qdwLog->setWindowTitle(tr("Server log"));
}

void MainWindow::updateWindowTitle() {
	QString title;
	if (Global::get().s.bMinimalView) {
		title = tr("Mumble - Minimal View");
	} else {
		title = tr("Mumble");
	}

	if (!Global::get().windowTitlePostfix.isEmpty()) {
		title += QString::fromLatin1(" | %1").arg(Global::get().windowTitlePostfix);
	}

	setWindowTitle(title);
}

void MainWindow::updateServerNavigatorChrome() {
	if (!usesModernShell()) {
		return;
	}
	if (modernShellMinimalSnapshotEnabled()) {
		return;
	}

	if (!m_serverNavigatorHeaderFrame || !m_serverNavigatorTitle || !m_serverNavigatorSubtitle
		|| !m_serverNavigatorFooter || !m_serverNavigatorFooterFrame || !m_serverNavigatorFooterPresenceButton
		|| !m_serverNavigatorFooterAvatar || !m_serverNavigatorFooterName || !m_serverNavigatorFooterPresence) {
		return;
	}

	if (Global::get().uiSession == 0 || !Global::get().sh) {
		m_serverNavigatorEyebrow->setText(tr("Voice"));
		m_serverNavigatorTitle->setText(tr("Join a server"));
		m_serverNavigatorSubtitle->setText(tr("Rooms, people, and live state"));
		m_serverNavigatorFooterPresenceButton->hide();
		m_serverNavigatorFooter->setTextFormat(Qt::PlainText);
		m_serverNavigatorFooter->setText(tr("Disconnected\nOpen Server"));
		m_serverNavigatorFooter->setVisible(true);
		m_serverNavigatorHeaderFrame->setVisible(true);
		m_serverNavigatorFooterFrame->setVisible(true);
		closeServerNavigatorUserMenu();
		return;
	}

	const PersistentChatTarget chatTarget = currentPersistentChatTarget();
	const bool compactRoomShell =
		hasPersistentChatCapabilities() && chatTarget.valid && !chatTarget.serverLog && !chatTarget.directMessage
		&& !chatTarget.legacyTextPath
		&& (chatTarget.scope == MumbleProto::TextChannel || chatTarget.scope == MumbleProto::Channel);
	QString host;
	QString username;
	QString password;
	unsigned short port = 0;
	Global::get().sh->getConnectionInfo(host, port, username, password);

	const QString serverLabel = port == 0 || port == DEFAULT_MUMBLE_PORT ? host : tr("%1:%2").arg(host).arg(port);

	const ClientUser *selfUser = ClientUser::get(Global::get().uiSession);
	QString eyebrow            = tr("Voice");
	QString title              = serverLabel;
	QString subtitle           = serverLabel;
	QString footer             = tr("Connected\n%1").arg(serverLabel);
	bool showCompactFooter     = false;

	if (hasPersistentChatCapabilities() && chatTarget.valid && !chatTarget.legacyTextPath) {
		eyebrow = tr("Live context");
		title   = chatTarget.label;
		if (chatTarget.serverLog) {
			eyebrow  = tr("Activity");
			subtitle = tr("Server log and client diagnostics");
		} else if (chatTarget.directMessage && chatTarget.user) {
			eyebrow  = tr("Direct");
			subtitle = tr("With %1").arg(chatTarget.user->qsName);
		} else if (chatTarget.scope == MumbleProto::Aggregate) {
			eyebrow  = tr("Legacy");
			subtitle = tr("Combined activity is retired");
		} else if (chatTarget.scope == MumbleProto::ServerGlobal) {
			eyebrow = tr("Legacy");
			if (const ClientUser *self = ClientUser::get(Global::get().uiSession); self && self->cChannel) {
				subtitle = tr("Legacy server-wide chat from %1").arg(persistentTextAclChannelLabel(self->cChannel));
			} else {
				subtitle = tr("Legacy server-wide chat");
			}
		} else if (chatTarget.scope == MumbleProto::TextChannel && chatTarget.channel) {
			eyebrow  = tr("Room");
			subtitle = tr("Visible like %1").arg(persistentTextAclChannelLabel(chatTarget.channel));
		} else if (chatTarget.scope == MumbleProto::Channel && chatTarget.channel) {
			eyebrow  = tr("Voice");
			subtitle = tr("History for %1").arg(persistentTextAclChannelLabel(chatTarget.channel));
		} else if (!chatTarget.description.trimmed().isEmpty()) {
			subtitle = chatTarget.description;
		}
	} else if (ClientUser *self = ClientUser::get(Global::get().uiSession); self && self->cChannel) {
		eyebrow  = tr("Current room");
		title    = self->cChannel->qsName;
		subtitle = roomPopulationLabel(self->cChannel->qlUsers.count());
	} else {
		footer = tr("Connected to %1").arg(serverLabel);
	}

	if (selfUser) {
		const bool idle            = isUserIdle(selfUser->uiSession);
		const QString presenceText = Global::get().s.bDeaf
										 ? tr("Deafened")
										 : (Global::get().s.bMute ? tr("Muted") : (idle ? tr("Away") : tr("Online")));
		QColor avatarBackground  = palette().color(QPalette::Highlight);
		QColor avatarText        = palette().color(QPalette::HighlightedText);
		QColor nameColor         = palette().color(QPalette::Text);
		QColor presenceColor     = palette().color(QPalette::Mid);
		QColor presenceRingColor = palette().color(QPalette::Window);
		if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
			avatarBackground  = alphaColor(tokens->accent, 0.34);
			avatarText        = tokens->text;
			nameColor         = tokens->text;
			presenceRingColor = tokens->mantle;
			presenceColor     = Global::get().s.bDeaf
								? mixColors(tokens->rosewater, tokens->red, 0.48)
								: (Global::get().s.bMute ? mixColors(tokens->peach, tokens->yellow, 0.36)
														 : (idle ? mixColors(tokens->yellow, tokens->text, 0.22)
																 : mixColors(tokens->green, tokens->text, 0.12)));
		} else if (m_serverNavigatorContentFrame) {
			presenceRingColor = m_serverNavigatorContentFrame->palette().color(QPalette::Window);
			if (idle && !Global::get().s.bMute && !Global::get().s.bDeaf) {
				presenceColor = QColor(207, 155, 65);
			}
		}

		const QString selfName = selfUser->qsName.trimmed().isEmpty() ? tr("You") : selfUser->qsName.trimmed();
		const QString initial  = selfName.left(1).toUpper();
		m_serverNavigatorFooterAvatar->setPixmap(
			serverNavigatorSelfAvatarPixmap(initial, m_serverNavigatorFooterAvatar->size(), avatarBackground,
											avatarText, presenceColor, presenceRingColor));
		m_serverNavigatorFooterName->setText(selfName);
		m_serverNavigatorFooterPresence->setText(presenceText);
		m_serverNavigatorFooterName->setStyleSheet(
			QString::fromLatin1("color:%1;").arg(nameColor.name(QColor::HexArgb)));
		m_serverNavigatorFooterPresence->setStyleSheet(
			QString::fromLatin1("color:%1;").arg(presenceColor.name(QColor::HexArgb)));
		showCompactFooter = true;
	}

	m_serverNavigatorHeaderFrame->setVisible(!compactRoomShell);
	m_serverNavigatorEyebrow->setText(eyebrow);
	m_serverNavigatorTitle->setText(title);
	m_serverNavigatorSubtitle->setText(subtitle);
	m_serverNavigatorFooterPresenceButton->setVisible(showCompactFooter);
	m_serverNavigatorFooter->setTextFormat(Qt::PlainText);
	m_serverNavigatorFooter->setText(footer);
	m_serverNavigatorFooter->setVisible(!showCompactFooter && !footer.isEmpty());
	m_serverNavigatorFooterFrame->setVisible(showCompactFooter || !footer.isEmpty());
	if (!showCompactFooter) {
		closeServerNavigatorUserMenu();
	} else {
		syncServerNavigatorUserMenu();
	}
}

void MainWindow::syncServerNavigatorUserMenu() {
	if (!m_serverNavigatorFooterPresenceButton) {
		return;
	}

	if (!m_serverNavigatorUserMenuPopup) {
		auto *popup = new ServerNavigatorUserMenuPopup(this);
		popup->hide();
		m_serverNavigatorUserMenuPopup = popup;

		connect(popup->presenceButton(ServerNavigatorPresenceState::Online), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			AudioInputPtr ai = Global::get().ai;
			if (ai) {
				// Mirror the existing global-shortcut behavior so an explicit
				// "Online" choice can return the local client from idle.
				if (ai->activityState == AudioInput::ActivityStateIdle) {
					ai->activityState = AudioInput::ActivityStateReturnedFromIdle;
				}
				ai->tIdle.restart();
			}
			if (Global::get().s.bDeaf) {
				setAudioDeaf(false);
			}
			if (Global::get().s.bMute) {
				setAudioMute(false);
			}
		});
		connect(popup->presenceButton(ServerNavigatorPresenceState::Away), &QPushButton::clicked, this, [popup]() {
			if (QPushButton *button = popup->presenceButton(ServerNavigatorPresenceState::Away)) {
				QToolTip::showText(button->mapToGlobal(QPoint(button->width() / 2, button->height())),
								   QObject::tr("Away is shown automatically when you are idle."), button);
			}
		});
		connect(popup->presenceButton(ServerNavigatorPresenceState::Muted), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			if (Global::get().s.bDeaf) {
				setAudioDeaf(false);
			}
			if (!Global::get().s.bMute) {
				setAudioMute(true);
			}
		});
		connect(popup->presenceButton(ServerNavigatorPresenceState::Deafened), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			if (!Global::get().s.bDeaf) {
				setAudioDeaf(true);
			}
		});
		connect(popup->muteButton(), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			if (qaAudioMute) {
				qaAudioMute->trigger();
			}
		});
		connect(popup->deafButton(), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			if (qaAudioDeaf) {
				qaAudioDeaf->trigger();
			}
		});
		connect(popup->settingsButton(), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			if (qaConfigDialog) {
				qaConfigDialog->trigger();
			}
		});
		connect(popup->disconnectButton(), &QPushButton::clicked, this, [this]() {
			closeServerNavigatorUserMenu();
			if (qaServerDisconnect && qaServerDisconnect->isEnabled()) {
				qaServerDisconnect->trigger();
			} else if (Global::get().uiSession != 0 && Global::get().sh) {
				disconnectFromServer();
			}
		});
	}

	const ClientUser *selfUser = ClientUser::get(Global::get().uiSession);
	auto *popup                = static_cast< ServerNavigatorUserMenuPopup * >(m_serverNavigatorUserMenuPopup.data());
	if (!popup || !selfUser || !m_serverNavigatorFooterPresenceButton->isVisible()) {
		closeServerNavigatorUserMenu();
		return;
	}

	const bool idle = isUserIdle(selfUser->uiSession);
	ServerNavigatorPresenceDisplay display;
	display.currentState = Global::get().s.bDeaf
							   ? ServerNavigatorPresenceState::Deafened
							   : (Global::get().s.bMute ? ServerNavigatorPresenceState::Muted
														: (idle ? ServerNavigatorPresenceState::Away
																: ServerNavigatorPresenceState::Online));
	display.name                  = selfUser->qsName.trimmed().isEmpty() ? tr("You") : selfUser->qsName.trimmed();
	display.label                 = serverNavigatorPresenceStateLabel(display.currentState);
	display.textColor             = palette().color(QPalette::Text);
	display.mutedTextColor        = palette().color(QPalette::Mid);
	display.avatarBackgroundColor = palette().color(QPalette::Highlight);
	display.avatarTextColor       = palette().color(QPalette::HighlightedText);
	display.presenceColor         = palette().color(QPalette::Highlight);
	display.onlineColor           = QColor(74, 186, 122);
	display.awayColor             = QColor(239, 159, 39);
	display.mutedStateColor       = QColor(219, 156, 64);
	display.deafenedColor         = QColor(226, 75, 74);
	display.presenceRingColor     = palette().color(QPalette::Window);
	display.panelColor            = palette().color(QPalette::Window);
	display.borderColor           = alphaColor(palette().color(QPalette::Mid), 0.38);
	display.hoverColor            = alphaColor(palette().color(QPalette::Highlight), 0.10);
	display.selectedColor         = alphaColor(palette().color(QPalette::Highlight), 0.14);
	display.dividerColor          = alphaColor(palette().color(QPalette::Mid), 0.28);
	display.dangerColor           = QColor(214, 92, 92);
	if (const std::optional< UiThemeTokens > tokens = activeUiThemeTokens(); tokens) {
		display.textColor             = tokens->text;
		display.mutedTextColor        = tokens->textMuted;
		display.avatarBackgroundColor = alphaColor(tokens->accent, 0.34);
		display.avatarTextColor       = tokens->text;
		display.onlineColor           = mixColors(tokens->green, tokens->text, 0.12);
		display.awayColor             = mixColors(tokens->yellow, tokens->text, 0.22);
		display.mutedStateColor       = mixColors(tokens->peach, tokens->yellow, 0.36);
		display.deafenedColor         = mixColors(tokens->rosewater, tokens->red, 0.48);
		display.presenceRingColor     = tokens->mantle;
		display.panelColor            = alphaColor(tokens->surface0, 0.985);
		display.borderColor           = tokens->surface1;
		display.hoverColor            = alphaColor(tokens->surface1, 0.64);
		display.dividerColor          = tokens->surface1;
		display.dangerColor           = mixColors(tokens->red, tokens->rosewater, 0.20);
		display.presenceColor         = serverNavigatorPresenceStateColor(display, display.currentState);
		display.selectedColor         = alphaColor(display.presenceColor, 0.20);
	} else if (idle && !Global::get().s.bMute && !Global::get().s.bDeaf) {
		display.awayColor     = QColor(207, 155, 65);
		display.presenceColor = display.awayColor;
		display.selectedColor = alphaColor(display.presenceColor, 0.16);
	} else {
		display.presenceColor = serverNavigatorPresenceStateColor(display, display.currentState);
	}

	popup->sync(display, qaAudioMute && qaAudioMute->isChecked(), qaAudioDeaf && qaAudioDeaf->isChecked(),
				qaConfigDialog != nullptr, Global::get().uiSession != 0 && Global::get().sh);
	if (popup->isVisible()) {
		positionServerNavigatorUserMenu();
	}
}

void MainWindow::positionServerNavigatorUserMenu() {
	auto *popup = static_cast< ServerNavigatorUserMenuPopup * >(m_serverNavigatorUserMenuPopup.data());
	if (!popup || !m_serverNavigatorFooterPresenceButton || !m_serverNavigatorFooterPresenceButton->isVisible()) {
		return;
	}

	popup->adjustSize();
	const QPoint anchorTopLeft = m_serverNavigatorFooterPresenceButton->mapTo(this, QPoint(0, 0));
	const QRect anchorRect(anchorTopLeft, m_serverNavigatorFooterPresenceButton->size());
	const int x           = qBound(8, anchorRect.right() - popup->width() + 22, width() - popup->width() - 8);
	const int y           = qMax(8, anchorRect.top() - popup->height() + 10);
	const int arrowCenter = qBound(20, anchorRect.right() - x - 18, popup->width() - 20);
	popup->setArrowCenterX(arrowCenter);
	popup->move(x, y);
	popup->raise();
}

void MainWindow::toggleServerNavigatorUserMenu() {
	auto *popup = static_cast< ServerNavigatorUserMenuPopup * >(m_serverNavigatorUserMenuPopup.data());
	if (popup && popup->isVisible()) {
		closeServerNavigatorUserMenu();
		return;
	}

	syncServerNavigatorUserMenu();
	popup = static_cast< ServerNavigatorUserMenuPopup * >(m_serverNavigatorUserMenuPopup.data());
	if (!popup) {
		return;
	}

	positionServerNavigatorUserMenu();
	popup->show();
	popup->raise();
	if (QPushButton *focusButton = popup->preferredFocusButton()) {
		focusButton->setFocus();
	}
	qApp->installEventFilter(this);
}

void MainWindow::closeServerNavigatorUserMenu() {
	if (m_serverNavigatorUserMenuPopup) {
		m_serverNavigatorUserMenuPopup->hide();
	}
	qApp->removeEventFilter(this);
}

void MainWindow::updateToolbar() {
	if (!qtIconToolbar) {
		return;
	}

	const bool layoutIsCustom = effectiveWindowLayout() == Settings::LayoutCustom;
	qtIconToolbar->setMovable(layoutIsCustom && !Global::get().s.bLockLayout);

	// Avoid detaching the toolbar during startup. Qt serializes toolbar-area
	// topology into saveState(), and mutating a restored custom toolbar layout
	// here can produce a state blob that later crashes in
	// QToolBarAreaLayoutInfo::sizeHint().
	const Qt::ToolBarArea currentArea = toolBarArea(qtIconToolbar);
	if (layoutIsCustom) {
		if (currentArea == Qt::NoToolBarArea) {
			addToolBar(Qt::TopToolBarArea, qtIconToolbar);
		}
		return;
	}

	if (currentArea != Qt::TopToolBarArea) {
		addToolBar(Qt::TopToolBarArea, qtIconToolbar);
	}
}

void MainWindow::updatePersistentChatSendButton() {
	if (!qteChat) {
		return;
	}

	const bool hasUserText =
		qteChat->isEnabled() && !qteChat->isShowingDefaultText() && !qteChat->toPlainText().trimmed().isEmpty();
	if (m_persistentChatSendButton) {
		m_persistentChatSendButton->setEnabled(hasUserText);
	}
	if (m_persistentChatAttachButton) {
		m_persistentChatAttachButton->setEnabled(qteChat->isEnabled() && Global::get().bAllowHTML);
	}
}

void MainWindow::updateFavoriteButton() {
	if (Global::get().uiSession == 0) {
		qaServerAddToFavorites->setEnabled(false);
	} else {
		QString host, uname, pw;
		unsigned short port;
		Global::get().sh->getConnectionInfo(host, port, uname, pw);
		qaServerAddToFavorites->setEnabled(!Global::get().db->isFavorite(host, port));
	}
}

// Sets whether or not to show the title bars on the MainWindow's
// dock widgets.
void MainWindow::setShowDockTitleBars(bool doShow) {
	if (dtbLogDockTitle) {
		dtbLogDockTitle->setEnabled(doShow);
	}
	if (dtbChatDockTitle) {
		dtbChatDockTitle->setEnabled(doShow);
	}
}

MainWindow::~MainWindow() {
	delete qwPTTButtonWidget;
	delete qdwLog->titleBarWidget();
	delete pmModel;
	delete qtvUsers;
	delete Channel::get(Mumble::ROOT_CHANNEL_ID);
}

void MainWindow::msgBox(QString msg) {
	MessageBoxEvent *mbe = new MessageBoxEvent(msg);
	QApplication::postEvent(this, mbe);
}

#ifdef Q_OS_WIN
bool MainWindow::nativeEvent(const QByteArray &, void *message, qintptr *) {
	MSG *msg = reinterpret_cast< MSG * >(message);
	switch (msg->message) {
		case WM_ENTERSIZEMOVE:
			beginNativeWindowMoveOrResize();
			break;
		case WM_EXITSIZEMOVE:
			endNativeWindowMoveOrResize();
			break;
		case WM_CANCELMODE:
		case WM_CAPTURECHANGED:
		case WM_ACTIVATEAPP:
			endNativeWindowMoveOrResize();
			break;
		case WM_DEVICECHANGE:
			if (msg->wParam == DBT_DEVNODES_CHANGED) {
				uiNewHardware++;
			}
			break;
		default:
			break;
	}

	return false;
}
#endif

void MainWindow::closeEvent(QCloseEvent *e) {
	ServerHandlerPtr sh               = Global::get().sh;
	QuitBehavior quitBehavior         = Global::get().s.quitBehavior;
	const bool alwaysAsk              = quitBehavior == QuitBehavior::ALWAYS_ASK;
	const bool askDueToConnected      = sh && sh->isRunning() && quitBehavior == QuitBehavior::ASK_WHEN_CONNECTED;
	const bool alwaysMinimize         = quitBehavior == QuitBehavior::ALWAYS_MINIMIZE;
	const bool minimizeDueToConnected = sh && sh->isRunning() && quitBehavior == QuitBehavior::MINIMIZE_WHEN_CONNECTED;

	if (!forceQuit && (alwaysAsk || askDueToConnected)) {
		QMessageBox mb(QMessageBox::Warning, QLatin1String("Mumble"),
					   tr("Are you sure you want to close Mumble? Perhaps you prefer to minimize it instead?"),
					   QMessageBox::NoButton, this);
		QCheckBox *qcbRemember   = new QCheckBox(tr("Remember this setting"));
		QPushButton *qpbClose    = mb.addButton(tr("Close"), QMessageBox::YesRole);
		QPushButton *qpbMinimize = mb.addButton(tr("Minimize"), QMessageBox::NoRole);
		QPushButton *qpbCancel   = mb.addButton(tr("Cancel"), QMessageBox::RejectRole);
		mb.setDefaultButton(qpbClose);
		mb.setEscapeButton(qpbCancel);
		mb.setCheckBox(qcbRemember);
		mb.exec();
		if (mb.clickedButton() == qpbMinimize) {
			setWindowState(windowState() | Qt::WindowMinimized);
			e->ignore();

			// If checkbox is checked and not connected, always minimize
			// If checkbox is checked and connected, always minimize when connected
			if (qcbRemember->isChecked() && !(sh && sh->isRunning())) {
				Global::get().s.quitBehavior = QuitBehavior::ALWAYS_MINIMIZE;
			} else if (qcbRemember->isChecked()) {
				Global::get().s.quitBehavior = QuitBehavior::MINIMIZE_WHEN_CONNECTED;
			}

			return;
		} else if (mb.clickedButton() == qpbCancel) {
			e->ignore();
			return;
		}

		// If checkbox is checked, quit always
		if (qcbRemember->isChecked()) {
			Global::get().s.quitBehavior = QuitBehavior::ALWAYS_QUIT;
		}
	} else if (!forceQuit && (alwaysMinimize || minimizeDueToConnected)) {
		setWindowState(windowState() | Qt::WindowMinimized);
		e->ignore();
		return;
	}

	sh.reset();

	storeState(Global::get().s.bMinimalView);

	if (Global::get().talkingUI && Global::get().talkingUI->isVisible()) {
		// Save the TalkingUI's position if it is visible
		// Note that we explicitly don't save the whole geometry as the TalkingUI's size
		// is a flexible thing that'll adjust automatically anyways.
		Global::get().s.qpTalkingUI_Position = Global::get().talkingUI->pos();
	}

	if (m_searchDialog) {
		// Save position of search dialog
		Global::get().s.searchDialogPosition = { m_searchDialog->x(), m_searchDialog->y() };
	}

	if (qwPTTButtonWidget) {
		qwPTTButtonWidget->close();
		qwPTTButtonWidget->deleteLater();
		qwPTTButtonWidget = nullptr;
	}
	Global::get().bQuit = true;

	QMainWindow::closeEvent(e);

	qApp->exit(restartOnQuit ? MUMBLE_EXIT_CODE_RESTART : 0);
}

void MainWindow::hideEvent(QHideEvent *e) {
#ifdef USE_OVERLAY
	if (Global::get().ocIntercept) {
		QMetaObject::invokeMethod(Global::get().ocIntercept, "hideGui", Qt::QueuedConnection);
		e->ignore();
		return;
	}
#endif
	QMainWindow::hideEvent(e);
}

void MainWindow::showEvent(QShowEvent *e) {
	QMainWindow::showEvent(e);
#ifdef Q_OS_WIN
	applyNativeTitleBarTheme(this);
#endif
}

void MainWindow::changeEvent(QEvent *e) {
	// Parse minimize event
	if (e->type() == QEvent::WindowStateChange) {
		// This code block is not triggered on (X)Wayland due to a Qt bug we can do nothing about (QTBUG-74310)
		QWindowStateChangeEvent *windowStateEvent = static_cast< QWindowStateChangeEvent * >(e);
		if (windowStateEvent) {
			bool wasMinimizedState = (windowStateEvent->oldState() & Qt::WindowMinimized);
			bool isMinimizedState  = (windowState() & Qt::WindowMinimized);
			if (!wasMinimizedState && isMinimizedState) {
				emit windowMinimized();
			}
			return;
		}
	}

	// The window has just received focus after being in the background
	if (e->type() == QEvent::ActivationChange) {
		if (isActiveWindow()) {
			emit windowActivated();
		}
		return;
	}

	if (e->type() == QEvent::ThemeChange) {
		Themes::apply();
		refreshCustomChromeStyles();
	}

	QWidget::changeEvent(e);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
	if (event && watched
		&& (watched == m_serverNavigatorContentFrame || watched == m_serverNavigatorTextChannelsMotdFrame
			|| watched == m_serverNavigatorTextChannelsMotdBody)
		&& (event->type() == QEvent::Resize || event->type() == QEvent::Show
			|| event->type() == QEvent::LayoutRequest)) {
		QTimer::singleShot(0, this, [this]() { refreshServerNavigatorSectionHeights(); });
	}

	if (event && event->type() == QEvent::Wheel && watched
		&& watched->property("persistentChatEmbeddedBrowser").toBool() && m_persistentChatHistory) {
		QWheelEvent *wheelEvent = static_cast< QWheelEvent * >(event);
		if (QScrollBar *scrollBar = m_persistentChatHistory->verticalScrollBar()) {
			int delta = wheelEvent->pixelDelta().y();
			if (delta == 0 && !wheelEvent->angleDelta().isNull()) {
				delta = (wheelEvent->angleDelta().y() / 120) * std::max(scrollBar->singleStep(), 24);
			}
			if (delta != 0) {
				scrollBar->setValue(scrollBar->value() - delta);
				m_persistentChatHistory->stabilizeVisibleContent();
				wheelEvent->accept();
				return true;
			}
		}
	}

	if (m_serverNavigatorUserMenuPopup && m_serverNavigatorUserMenuPopup->isVisible() && event) {
		const auto widgetContainsTarget = [watched](QWidget *root) {
			QWidget *widget = qobject_cast< QWidget * >(watched);
			while (widget) {
				if (widget == root) {
					return true;
				}
				widget = widget->parentWidget();
			}
			return false;
		};

		if (event->type() == QEvent::MouseButtonPress) {
			if (!widgetContainsTarget(m_serverNavigatorUserMenuPopup.data())
				&& !(m_serverNavigatorFooterPresenceButton
					 && widgetContainsTarget(m_serverNavigatorFooterPresenceButton))) {
				closeServerNavigatorUserMenu();
			}
		} else if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast< QKeyEvent * >(event);
			if (keyEvent->key() == Qt::Key_Escape) {
				closeServerNavigatorUserMenu();
				return true;
			}
		} else if (event->type() == QEvent::Hide && watched == m_serverNavigatorUserMenuPopup.data()) {
			closeServerNavigatorUserMenu();
		}
	}

	return QMainWindow::eventFilter(watched, event);
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	// Pressing F6 switches between the main
	// window's main widgets, making it easier
	// to navigate Mumble's MainWindow with only
	// a keyboard.
	if (e->key() == Qt::Key_F6) {
		focusNextMainWidget();
	} else {
		QMainWindow::keyPressEvent(e);
	}
}

/// focusNextMainWidget switches the focus to the next main
/// widget of the MainWindow.
///
/// This is used to implement behavior where pressing F6
/// switches between major elements of an application.
/// This behavior is for example seen in Windows's (File) Explorer.
///
/// The main widgets are qteLog (the log view), the persistent chat controls
/// and qtvUsers (users tree view).
void MainWindow::focusNextMainWidget() {
	QWidget *mainFocusWidgets[] = {
		qteLog, m_persistentChatChannelList, m_persistentChatHistory, qteChat, qtvUsers,
	};
	const int numMainFocusWidgets = sizeof(mainFocusWidgets) / sizeof(mainFocusWidgets[0]);

	int currentMainFocusWidgetIndex = -1;

	QWidget *w = focusWidget();
	for (int i = 0; i < numMainFocusWidgets; i++) {
		QWidget *mainFocusWidget = mainFocusWidgets[i];
		if (mainFocusWidget && (w == mainFocusWidget || mainFocusWidget->isAncestorOf(w))) {
			currentMainFocusWidgetIndex = i;
			break;
		}
	}

	Q_ASSERT(currentMainFocusWidgetIndex != -1);

	int nextMainFocusWidgetIndex = (currentMainFocusWidgetIndex + 1) % numMainFocusWidgets;
	QWidget *nextMainFocusWidget = mainFocusWidgets[nextMainFocusWidgetIndex];
	nextMainFocusWidget->setFocus();
}

void MainWindow::updateAudioToolTips() {
	if (Global::get().s.bMute)
		qaAudioMute->setToolTip(tr("Unmute yourself"));
	else
		qaAudioMute->setToolTip(tr("Mute yourself"));

	if (Global::get().s.bDeaf)
		qaAudioDeaf->setToolTip(tr("Undeafen yourself"));
	else
		qaAudioDeaf->setToolTip(tr("Deafen yourself"));
}

void MainWindow::updateUserModel() {
	UserModel *um = static_cast< UserModel * >(qtvUsers->model());
	um->forceVisualUpdate();
}

void MainWindow::updateTransmitModeComboBox(Settings::AudioTransmit newMode) {
	switch (newMode) {
		case Settings::Continuous:
			qcbTransmitMode->setCurrentIndex(0);
			return;
		case Settings::VAD:
			qcbTransmitMode->setCurrentIndex(1);
			return;
		case Settings::PushToTalk:
			qcbTransmitMode->setCurrentIndex(2);
			return;
	}
}

QMenu *MainWindow::createPopupMenu() {
	if ((effectiveWindowLayout() == Settings::LayoutCustom) && !Global::get().s.bLockLayout) {
		// We have to explicitly create a menu here instead of simply referring to QMainWindow::createPopupMenu as we
		// don't want a toggle for showing/hiding the minimal view note (which is also present as a QDockWidget). Thus,
		// we have to explicitly add only those widgets that we really want to be toggleable.
		QMenu *menu = new QMenu(this);
		menu->addAction(qdwChat->toggleViewAction());
		menu->addAction(qdwLog->toggleViewAction());
		menu->addAction(qtIconToolbar->toggleViewAction());

		return menu;
	}

	return nullptr;
}

Channel *MainWindow::getContextMenuChannel() {
	if (cContextChannel)
		return cContextChannel.data();

	return nullptr;
}

ClientUser *MainWindow::getContextMenuUser() {
	if (cuContextUser)
		return cuContextUser.data();

	return nullptr;
}

ContextMenuTarget MainWindow::getContextMenuTargets() {
	ContextMenuTarget target;
	const bool hiddenLegacyUserModelSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();
	const bool hasExplicitContext            = cuContextUser || cContextChannel;

	if (Global::get().uiSession != 0) {
		target.user    = getContextMenuUser();
		target.channel = getContextMenuChannel();

		if (hiddenLegacyUserModelSafeMode) {
			if (!target.user) {
				target.user = ClientUser::get(Global::get().uiSession);
			}
			if (!target.channel && target.user) {
				target.channel = target.user->cChannel;
			}
		} else if (!hasExplicitContext) {
			QModelIndex idx;

			if (!qpContextPosition.isNull())
				idx = qtvUsers->indexAt(qpContextPosition);

			if (!idx.isValid())
				idx = qtvUsers->currentIndex();

			target.user    = pmModel->getUser(idx);
			target.channel = pmModel->getChannel(idx);
		}

		if (!target.channel && target.user) {
			target.channel = target.user->cChannel;
		}
	}

	cuContextUser     = target.user;
	cContextChannel   = target.channel;
	qpContextPosition = QPoint();

	return target;
}

QString MainWindow::screenShareStreamForChannel(const Channel *channel) const {
	if (!channel || !m_screenShareManager) {
		return QString();
	}

	const QHash< QString, ScreenShareSession > &sessions = m_screenShareManager->sessions();
	for (auto it = sessions.cbegin(); it != sessions.cend(); ++it) {
		const ScreenShareSession &session = it.value();
		if (session.scope == MumbleProto::ScreenShareScopeChannel && session.scopeID == channel->iId) {
			return it.key();
		}
	}

	return QString();
}

bool MainWindow::handleSpecialContextMenu(const QUrl &url, const QPoint &pos_, bool focus) {
	// This method abuses QUrls for internal data serialization
	// The protocol, host and path parts of the URL may contain
	// special values which are only parsable by this method.

	if (url.scheme() == QString::fromLatin1("clientid")) {
		bool ok = false;
		QString maybeUserHash(url.host());
		if (maybeUserHash.length() == 40) {
			ClientUser *cu = pmModel->getUser(maybeUserHash);
			if (cu) {
				cuContextUser = cu;
				ok            = true;
			}
		} else {
			// We expect the host part of the URL to contain the user id in the format
			// id.<id>
			// where <id> is the user id as integer. This is necessary, because QUrl parses
			// plain integers in the host field as IP addresses
			QByteArray qbaServerDigest = QByteArray::fromBase64(url.path().remove(0, 1).toLatin1());
			QString id                 = url.host().split(".").value(1, "-1");
			cuContextUser              = ClientUser::get(id.toUInt(&ok, 10));
			ServerHandlerPtr sh        = Global::get().sh;
			ok                         = ok && sh && (qbaServerDigest == sh->qbaDigest);
		}
		if (ok && cuContextUser) {
			if (focus) {
				qtvUsers->setCurrentIndex(pmModel->index(cuContextUser.data()));
				qteChat->setFocus();
			} else {
				qpContextPosition = QPoint();
				qmUser->exec(pos_, nullptr);
			}
		}
		cuContextUser.clear();
	} else if (url.scheme() == QString::fromLatin1("channelid")) {
		// We expect the host part of the URL to contain the channel id in the format
		// id.<id>
		// where <id> is the channel id as integer. This is necessary, because QUrl parses
		// plain integers in the host field as IP addresses
		bool ok;
		QByteArray qbaServerDigest = QByteArray::fromBase64(url.path().remove(0, 1).toLatin1());
		QString id                 = url.host().split(".").value(1, "-1");
		cContextChannel            = Channel::get(id.toUInt(&ok, 10));
		ServerHandlerPtr sh        = Global::get().sh;
		ok                         = ok && sh && (qbaServerDigest == sh->qbaDigest);
		if (ok) {
			if (focus) {
				qtvUsers->setCurrentIndex(pmModel->index(cContextChannel.data()));
				qteChat->setFocus();
			} else {
				qpContextPosition = QPoint();
				qmChannel->exec(pos_, nullptr);
			}
		}
		cContextChannel.clear();
	} else {
		return false;
	}
	return true;
}

void MainWindow::showUsersContextMenu(const QPoint &mpos, bool usePositionForGettingContext) {
	QModelIndex idx;
	if (usePositionForGettingContext) {
		idx = resolveTreeViewportIndex(qtvUsers, mpos);
		if (!idx.isValid()) {
			return;
		}
		qtvUsers->setCurrentIndex(idx);
	} else {
		idx = qtvUsers->currentIndex();
		if (!idx.isValid()) {
			return;
		}
	}

	ClientUser *p    = pmModel->getUser(idx);
	Channel *channel = pmModel->getChannel(idx);

	qpContextPosition = qtvUsers->visualRect(idx).center();
	if (pmModel->isChannelListener(idx)) {
		// Have a separate context menu for listeners
		QModelIndex parent = idx.parent();

		if (parent.isValid()) {
			// Find the channel in which the action was triggered and set it
			// in order to be able to obtain it in the action itself
			cContextChannel = pmModel->getChannel(parent);
		}
		cuContextUser.clear();
		qmListener->exec(qtvUsers->viewport()->mapToGlobal(mpos), nullptr);
		cuContextUser.clear();
		cContextChannel.clear();
	} else {
		if (p) {
			cuContextUser.clear();
			if (!usePositionForGettingContext) {
				cuContextUser = p;
			}

			qmUser->exec(qtvUsers->viewport()->mapToGlobal(mpos), nullptr);
			cuContextUser.clear();
		} else {
			cContextChannel.clear();

			if (!usePositionForGettingContext && channel) {
				cContextChannel = channel;
			}

			qmChannel->exec(qtvUsers->viewport()->mapToGlobal(mpos), nullptr);
			cContextChannel.clear();
		}
	}
	qpContextPosition = QPoint();
}

void MainWindow::on_qtvUsers_customContextMenuRequested(const QPoint &mpos, bool usePositionForGettingContext) {
	if (!qtvUsers || !qtvUsers->viewport()) {
		return;
	}

	showUsersContextMenu(qtvUsers->viewport()->mapFrom(qtvUsers, mpos), usePositionForGettingContext);
}

void MainWindow::showLogContextMenu(LogTextBrowser *browser, const QPoint &mpos) {
	if (!browser) {
		return;
	}

	m_selectedLogImage = QImage();

	QString link = browser->anchorAt(mpos);
	if (!link.isEmpty()) {
		QUrl l(link);
		if (l.scheme() == QLatin1String("mumble-chat") && l.host() == QLatin1String("inline-data-image")) {
			m_selectedLogImage = persistentChatInlineDataImageFromUrl(l);
			if (!m_selectedLogImage.isNull()) {
				QMenu menu(browser);
				menu.addAction(tr("Open Image"), this, &MainWindow::showImageDialog);
				menu.addAction(tr("Save Image As..."), this, SLOT(saveImageAs(void)));
				menu.exec(browser->mapToGlobal(mpos));
				return;
			}
		}

		if (handleSpecialContextMenu(l, browser->mapToGlobal(mpos)))
			return;
	}

	QPoint contentPosition =
		QPoint(QApplication::isRightToLeft()
				   ? (browser->horizontalScrollBar()->maximum() - browser->horizontalScrollBar()->value())
				   : browser->horizontalScrollBar()->value(),
			   browser->verticalScrollBar()->value());
	QMenu *menu = browser->createStandardContextMenu(mpos + contentPosition);

	QTextCursor cursor = browser->imageCursorAt(mpos);
	if (!cursor.isNull()) {
		m_selectedLogImage = imageFromLogBrowser(browser, cursor);
		if (!m_selectedLogImage.isNull()) {
			menu->addSeparator();
			menu->addAction(tr("Save Image As..."), this, SLOT(saveImageAs(void)));

			QAction *testItem = menu->addAction(tr("Open Image"));
			connect(testItem, &QAction::triggered, this, &MainWindow::showImageDialog);
		}
	}

	if (browser == qteLog || browser == m_persistentChatLogView) {
		menu->addSeparator();
		menu->addAction(tr("Clear"), browser, SLOT(clear(void)));
	}
	menu->exec(browser->mapToGlobal(mpos));
	delete menu;
}

void MainWindow::on_qteLog_customContextMenuRequested(const QPoint &mpos) {
	LogTextBrowser *browser = qobject_cast< LogTextBrowser * >(sender());
	if (!browser) {
		browser = qteLog;
	}

	showLogContextMenu(browser, mpos);
}

QImage MainWindow::imageFromLogBrowser(const LogTextBrowser *browser, const QTextCursor &cursor) const {
	if (!browser || cursor.isNull() || !cursor.charFormat().isImageFormat()) {
		return QImage();
	}

	const QString resourceName = cursor.charFormat().toImageFormat().name();
	const QVariant resource    = browser->document()->resource(QTextDocument::ImageResource, resourceName);
	return resource.value< QImage >();
}

QString MainWindow::registerPersistentChatInlineDataImageSource(const QString &source) {
	const QString token = persistentChatInlineDataImageToken(source);
	m_persistentChatInlineDataImageSources.insert(token, source);

	return token;
}

QUrl MainWindow::persistentChatInlineDataImageOpenUrl(const QString &token) const {
	QUrl url;
	url.setScheme(QLatin1String("mumble-chat"));
	url.setHost(QLatin1String("inline-data-image"));
	url.setPath(QString::fromLatin1("/%1").arg(token));
	return url;
}

QUrl MainWindow::persistentChatInlineDataImageResourceUrl(const QString &token) const {
	QUrl url;
	url.setScheme(QLatin1String("mumble-chat-image"));
	url.setHost(QLatin1String("inline-data-image"));
	url.setPath(QString::fromLatin1("/%1").arg(token));
	return url;
}

QImage MainWindow::persistentChatInlineDataImageFromSource(const QString &source) const {
	const PersistentChatInlineDataImageInfo info = persistentChatInlineDataImageInfo(source);
	if (!info.valid) {
		return QImage();
	}

	const QByteArray bytes = persistentChatInlineDataImageBytes(source, info);
	if (bytes.isEmpty()) {
		return QImage();
	}

	QBuffer buffer;
	buffer.setData(bytes);
	if (!buffer.open(QIODevice::ReadOnly)) {
		return QImage();
	}

	QImageReader reader(&buffer);
	reader.setAutoTransform(true);
	return reader.read();
}

QImage MainWindow::persistentChatInlineDataImageFromUrl(const QUrl &url) const {
	if (url.scheme() != QLatin1String("mumble-chat") || url.host() != QLatin1String("inline-data-image")) {
		return QImage();
	}

	const QString token = QUrl::fromPercentEncoding(url.path().mid(1).toUtf8());
	const auto it       = m_persistentChatInlineDataImageSources.constFind(token);
	if (it == m_persistentChatInlineDataImageSources.cend()) {
		return QImage();
	}

	return persistentChatInlineDataImageFromSource(*it);
}

void MainWindow::openImageDialog(const QImage &image) {
	if (image.isNull()) {
		QMessageBox::warning(this, tr("Error"), tr("Failed to decode image."));
		return;
	}

	const QPixmap pixmap = QPixmap::fromImage(image);
	ResponsiveImageDialog dialog(pixmap, this);
	dialog.exec();
}

void MainWindow::openImageDialog(LogTextBrowser *browser, const QTextCursor &cursor) {
	m_selectedLogImage = imageFromLogBrowser(browser, cursor);
	openImageDialog(m_selectedLogImage);
}

void MainWindow::saveImageAs() {
	QDateTime now = QDateTime::currentDateTime();
	QString defaultFname =
		QString::fromLatin1("Mumble-%1.jpg").arg(now.toString(QString::fromLatin1("yyyy-MM-dd-HHmmss")));

	QString fname = QFileDialog::getSaveFileName(this, tr("Save Image File"), getImagePath(defaultFname),
												 tr("Images (*.png *.jpg *.jpeg)"));
	if (fname.isNull()) {
		return;
	}

	const QImage img = m_selectedLogImage;
	if (img.isNull()) {
		QMessageBox::warning(this, tr("Error"), tr("Failed to decode image."));
		return;
	}
	bool ok = img.save(fname);
	if (!ok) {
		// In case fname did not contain a file extension, try saving with an
		// explicit format.
		ok = img.save(fname, "PNG");
	}

	updateImagePath(fname);

	if (!ok) {
		Global::get().l->log(Log::Warning, tr("Could not save image: %1").arg(fname.toHtmlEscaped()));
	}
}

QString MainWindow::getImagePath(QString filename) const {
	if (Global::get().s.qsImagePath.isEmpty() || !QDir(Global::get().s.qsImagePath).exists()) {
		Global::get().s.qsImagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
	}
	if (filename.isEmpty()) {
		return Global::get().s.qsImagePath;
	}
	return Global::get().s.qsImagePath + QDir::separator() + filename;
}

void MainWindow::updateImagePath(QString filepath) const {
	QFileInfo fi(filepath);
	Global::get().s.qsImagePath = fi.absolutePath();
}

void MainWindow::setTransmissionMode(Settings::AudioTransmit mode) {
	if (Global::get().s.atTransmit != mode) {
		Global::get().s.atTransmit = mode;

		switch (mode) {
			case Settings::Continuous:
				Global::get().l->log(Log::Information, tr("Transmit Mode set to Continuous"));
				break;
			case Settings::VAD:
				Global::get().l->log(Log::Information, tr("Transmit Mode set to Voice Activity"));
				break;
			case Settings::PushToTalk:
				Global::get().l->log(Log::Information, tr("Transmit Mode set to Push-to-Talk"));
				break;
		}

		emit transmissionModeChanged(mode);
	}
}

void MainWindow::on_qaSearch_triggered() {
	toggleSearchDialogVisibility();
}

void MainWindow::toggleSearchDialogVisibility() {
	if (!m_searchDialog) {
		m_searchDialog = new Search::SearchDialog(this);

		QPoint position = Global::get().s.searchDialogPosition;

		if (position == Settings::UNSPECIFIED_POSITION) {
			// Get MainWindow's position on screen
			position = mapToGlobal(QPoint(0, 0));
		}

		if (Mumble::QtUtils::positionIsOnScreen(position)) {
			// Move the search dialog to the same origin as the MainWindow is
			m_searchDialog->move(position);
		}
	}

	m_searchDialog->setVisible(!m_searchDialog->isVisible());
}

void MainWindow::enableRecording(bool recordingAllowed) {
	qaRecording->setEnabled(recordingAllowed);

	Global::get().recordingAllowed = recordingAllowed;

	if (!recordingAllowed && voiceRecorderDialog) {
		voiceRecorderDialog->reject();
	}
}

void MainWindow::handleUserMoved(unsigned int sessionID, const std::optional< unsigned int > &prevChannelID,
								 unsigned int newChannelID) {
	if (sessionID == Global::get().uiSession && prevChannelID.has_value()) {
		if (prevChannelID != m_movedBackFromChannel) {
			// Add to stack of previous channels
			m_previousChannels.push(prevChannelID.value());
			qaMoveBack->setEnabled(true);
		} else {
			m_movedBackFromChannel.reset();
		}

		QPointer< MainWindow > guardedThis(this);
		QMetaObject::invokeMethod(
			this,
			[guardedThis]() {
				if (!guardedThis) {
					return;
				}

				guardedThis->rebuildPersistentChatChannelList();
				guardedThis->updateChatBar();
				guardedThis->updateServerNavigatorChrome();
			},
			Qt::QueuedConnection);
	}

	(void) newChannelID;
}

void MainWindow::on_qaMoveBack_triggered() {
	if (m_previousChannels.empty()) {
		return;
	}

	Channel *prevChannel = Channel::get(m_previousChannels.top());
	m_previousChannels.pop();

	if (!prevChannel) {
		Global::get().l->log(Log::Warning,
							 tr("The channel you have been in previously no longer exists on this server."));
		qaMoveBack->setEnabled(false);
		return;
	}

	ClientUser *self = ClientUser::get(Global::get().uiSession);
	if (!self) {
		qaMoveBack->setEnabled(false);
		return;
	}

	ServerHandlerPtr handler = Global::get().sh;
	if (!handler) {
		qaMoveBack->setEnabled(false);
		return;
	}

	// Setting this prevents the user's current channel to be added to the stack
	// of last visited channels. If it was added, the user could only ever cycle
	// between the last channel and the current one.
	m_movedBackFromChannel = self->cChannel->iId;
	handler->joinChannel(Global::get().uiSession, prevChannel->iId);

	qaMoveBack->setEnabled(!m_previousChannels.empty());
}

static void recreateServerHandler() {
	ServerHandlerPtr sh = Global::get().sh;
	if (sh && sh->isRunning()) {
		Global::get().mw->on_qaServerDisconnect_triggered();
		sh->disconnect();
		sh->wait();
		QCoreApplication::instance()->processEvents();
	}

	Global::get().sh.reset();
	while (sh && sh.use_count() > 1) {
		QThread::yieldCurrentThread();
	}
	sh.reset();

	sh = ServerHandlerPtr(new ServerHandler());
	sh->moveToThread(sh.get());
	Global::get().sh = sh;
	Global::get().mw->connect(sh.get(), SIGNAL(connected()), Global::get().mw, SLOT(serverConnected()));
	Global::get().mw->connect(sh.get(), SIGNAL(disconnected(QAbstractSocket::SocketError, QString)), Global::get().mw,
							  SLOT(serverDisconnected(QAbstractSocket::SocketError, QString)));
	Global::get().mw->connect(sh.get(), SIGNAL(error(QAbstractSocket::SocketError, QString)), Global::get().mw,
							  SLOT(resolverError(QAbstractSocket::SocketError, QString)));

	QObject::connect(sh.get(), &ServerHandler::disconnected, Global::get().talkingUI,
					 &TalkingUI::on_serverDisconnected);

	// We have to use direct connections for these here as the PluginManager must be able to access the connection's ID
	// and in order for that to be possible the (dis)connection process must not proceed in the background.
	Global::get().pluginManager->connect(sh.get(), &ServerHandler::connected, Global::get().pluginManager,
										 &PluginManager::on_serverConnected, Qt::DirectConnection);
	// We connect the plugin manager to "aboutToDisconnect" instead of "disconnect" in order for the slot to be
	// guaranteed to be completed *before* the actual disconnect logic (e.g. MainWindow::serverDisconnected) kicks in.
	// In order for that to work it is ESSENTIAL to use a DIRECT CONNECTION!
	Global::get().pluginManager->connect(sh.get(), &ServerHandler::aboutToDisconnect, Global::get().pluginManager,
										 &PluginManager::on_serverDisconnected, Qt::DirectConnection);
}

void MainWindow::openUrl(const QUrl &url) {
	Global::get().l->log(Log::Information,
						 tr("Opening URL %1").arg(url.toString(QUrl::RemovePassword).toHtmlEscaped()));
	if (url.scheme() == QLatin1String("file")) {
		QFile f(url.toLocalFile());
		if (!f.exists() || !f.open(QIODevice::ReadOnly)) {
			Global::get().l->log(Log::Warning, tr("File does not exist"));
			return;
		}
		f.close();

		try {
			Settings newSettings;
			newSettings.load(f.fileName());

			std::swap(newSettings, Global::get().s);

			Global::get().l->log(Log::Warning, tr("Settings merged from file."));
		} catch (const std::exception &) {
			Global::get().l->log(Log::Warning, tr("Invalid settings file encountered."));
		}

		return;
	}

	if (url.scheme() != QLatin1String("mumble")) {
		Global::get().l->log(Log::Warning, tr("URL scheme is not 'mumble'"));
		return;
	}

	Version::full_t thisVersion   = Version::get();
	Version::full_t targetVersion = Version::UNKNOWN;

	QUrlQuery query(url);
	QString version = query.queryItemValue(QLatin1String("version"));
	if (version.size() > 0) {
		targetVersion = Version::fromString(version);
		if (targetVersion == Version::UNKNOWN) {
			// The version format is invalid
			Global::get().l->log(Log::Warning,
								 QObject::tr("The provided URL uses an invalid version format: \"%1\"").arg(version));
			return;
		}
	}

	// With no version parameter given assume the link refers to our version
	if (targetVersion == Version::UNKNOWN) {
		targetVersion = thisVersion;
	}

	// We can't handle URLs for versions < 1.2.0
	const bool isPre_120 = targetVersion < Version::fromComponents(1, 2, 0);
	// We also can't handle URLs for versions newer than the running Mumble instance
	const bool isFuture = thisVersion < targetVersion;

	if (isPre_120 || isFuture) {
		Global::get().l->log(
			Log::Warning,
			tr("This version of Mumble can't handle URLs for Mumble version %1").arg(Version::toString(targetVersion)));
		return;
	}

	QString host        = url.host();
	unsigned short port = static_cast< unsigned short >(url.port(DEFAULT_MUMBLE_PORT));
	QString user        = url.userName();
	QString pw          = url.password();
	qsDesiredChannel    = url.path();
	QString name;

	if (query.hasQueryItem(QLatin1String("title")))
		name = query.queryItemValue(QLatin1String("title"));

	if (Global::get().sh && Global::get().sh->isRunning()) {
		QString oHost, oUser, oPw;
		unsigned short oport;
		Global::get().sh->getConnectionInfo(oHost, oport, oUser, oPw);
		ClientUser *p = ClientUser::get(Global::get().uiSession);

		if ((user.isEmpty() || (p && p->iId >= 0) || (user == oUser))
			&& (host.isEmpty() || ((host == oHost) && (port == oport)))) {
			findDesiredChannel();
			return;
		}
	}

	Global::get().db->fuzzyMatch(name, user, pw, host, port);

	if (user.isEmpty()) {
		bool ok;
		user = QInputDialog::getText(this, tr("Connecting to %1").arg(url.toString()), tr("Enter username"),
									 QLineEdit::Normal, Global::get().s.qsUsername, &ok);
		if (!ok || user.isEmpty())
			return;
		Global::get().s.qsUsername = user;
	}

	if (name.isEmpty())
		name = QString::fromLatin1("%1@%2").arg(user).arg(host);

	recreateServerHandler();

	Global::get().s.qsLastServer = name;
	rtLast                       = MumbleProto::Reject_RejectType_None;
	bRetryServer                 = true;
	qaServerDisconnect->setEnabled(true);
	Global::get().l->log(Log::Information,
						 tr("Connecting to server %1.").arg(Log::msgColor(host.toHtmlEscaped(), Log::Server)));
	Global::get().sh->setConnectionInfo(host, port, user, pw);
	Global::get().sh->start(QThread::TimeCriticalPriority);
}

/**
 * This function tries to join a desired channel on connect. It gets called
 * directly after server synchronization is completed.
 * @see void MainWindow::msgServerSync(const MumbleProto::ServerSync &msg)
 */
void MainWindow::findDesiredChannel() {
	const bool hiddenLegacyUserModelSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();
	bool found                               = false;
	QStringList qlChans                      = qsDesiredChannel.split(QLatin1String("/"));
	Channel *chan                            = Channel::get(Mumble::ROOT_CHANNEL_ID);
	QString str                              = QString();
	while (chan && qlChans.count() > 0) {
		QString elem = qlChans.takeFirst().toLower();
		if (elem.isEmpty())
			continue;
		if (str.isNull())
			str = elem;
		else
			str = str + QLatin1String("/") + elem;
		for (Channel *c : chan->qlChannels) {
			if (c->qsName.toLower() == str) {
				str   = QString();
				found = true;
				chan  = c;
				break;
			}
		}
	}
	if (found) {
		if (chan != ClientUser::get(Global::get().uiSession)->cChannel) {
			Global::get().sh->joinChannel(Global::get().uiSession, chan->iId);
		}
		if (!hiddenLegacyUserModelSafeMode) {
			qtvUsers->setCurrentIndex(pmModel->index(chan));
		}
	} else if (Global::get().uiSession && !hiddenLegacyUserModelSafeMode) {
		qtvUsers->setCurrentIndex(pmModel->index(ClientUser::get(Global::get().uiSession)->cChannel));
	}
	if (hiddenLegacyUserModelSafeMode) {
		queueModernShellSnapshotSync();
	}
	updateMenuPermissions();
}

void MainWindow::setOnTop(bool top) {
	Qt::WindowFlags wf = windowFlags();
	if (wf.testFlag(Qt::WindowStaysOnTopHint) != top) {
		if (top)
			wf |= Qt::WindowStaysOnTopHint;
		else
			wf &= ~Qt::WindowStaysOnTopHint;
		setWindowFlags(wf);
		show();
	}
}

void MainWindow::loadState(const bool minimalView) {
	const bool modernShell = usesModernShell();
	if (minimalView) {
		const QByteArray &geometry =
			modernShell ? Global::get().s.qbaModernMinimalViewGeometry : Global::get().s.qbaMinimalViewGeometry;
		if (!geometry.isNull()) {
			restoreGeometry(geometry);
		}
		const QByteArray &state =
			modernShell ? Global::get().s.qbaModernMinimalViewState : Global::get().s.qbaMinimalViewState;
		if (!state.isNull()) {
			restoreState(state, stateVersion(modernShell));
		}
	} else {
		const QByteArray &geometry =
			modernShell ? Global::get().s.qbaModernMainWindowGeometry : Global::get().s.qbaMainWindowGeometry;
		if (!geometry.isNull()) {
			restoreGeometry(geometry);
		}
		const QByteArray &state =
			modernShell ? Global::get().s.qbaModernMainWindowState : Global::get().s.qbaMainWindowState;
		if (!state.isNull()) {
			restoreState(state, stateVersion(modernShell));
		}
	}
}

void MainWindow::storeState(const bool minimalView) {
	const bool modernShell = usesModernShell();
	if (minimalView) {
		if (modernShell) {
			Global::get().s.qbaModernMinimalViewGeometry = saveGeometry();
			Global::get().s.qbaModernMinimalViewState    = saveState(stateVersion(true));
		} else {
			Global::get().s.qbaMinimalViewGeometry = saveGeometry();
			Global::get().s.qbaMinimalViewState    = saveState(stateVersion(false));
		}
	} else {
		if (modernShell) {
			Global::get().s.qbaModernMainWindowGeometry = saveGeometry();
			Global::get().s.qbaModernMainWindowState    = saveState(stateVersion(true));
		} else {
			Global::get().s.qbaMainWindowGeometry = saveGeometry();
			Global::get().s.qbaMainWindowState    = saveState(stateVersion(false));
		}
	}
}

void MainWindow::setupView(bool toggle_minimize) {
	applyShellLayout();
	bool showit                               = !Global::get().s.bMinimalView;
	const Settings::WindowLayout windowLayout = effectiveWindowLayout();

	switch (windowLayout) {
		case Settings::LayoutClassic:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::LeftDockWidgetArea, qdwLog);
			qdwLog->show();
			splitDockWidget(qdwLog, qdwChat, Qt::Vertical);
			qdwChat->show();
			break;
		case Settings::LayoutStacked:
			removeDockWidget(qdwLog);
			addDockWidget(Qt::BottomDockWidgetArea, qdwLog);
			qdwLog->show();
			splitDockWidget(qdwLog, qdwChat, Qt::Vertical);
			qdwChat->show();
			break;
		case Settings::LayoutHybrid:
			removeDockWidget(qdwLog);
			removeDockWidget(qdwChat);
			addDockWidget(Qt::LeftDockWidgetArea, qdwLog);
			qdwLog->show();
			addDockWidget(Qt::BottomDockWidgetArea, qdwChat);
			qdwChat->show();
			break;
		case Settings::LayoutModern:
			removeDockWidget(qdwChat);
			removeDockWidget(qdwLog);
			qdwLog->hide();
			qdwChat->hide();
			break;
		default:
			break;
	}

	if (windowLayout == Settings::LayoutModern) {
		qdwLog->hide();
		qdwChat->hide();
	} else if (windowLayout != Settings::LayoutCustom) {
		const Qt::Orientation dockResizeOrientation =
			windowLayout == Settings::LayoutStacked ? Qt::Vertical : Qt::Horizontal;
		const int dockResizeTarget = dockResizeOrientation == Qt::Vertical ? 720 : 740;
		resizeDocks(QList< QDockWidget * >() << qdwChat, QList< int >() << dockResizeTarget, dockResizeOrientation);
	}

	setDockSplitterHandleWidth(this, 1);

	updateToolbar();

	qteChat->updateGeometry();

	QRect geom = frameGeometry();

	if (toggle_minimize) {
		storeState(showit);
	}

	Qt::WindowFlags f = Qt::Window;
	if (!showit) {
		if (Global::get().s.bHideFrame) {
			f |= Qt::FramelessWindowHint;
		}
	}

	if (Global::get().s.aotbAlwaysOnTop == Settings::OnTopAlways
		|| (Global::get().s.bMinimalView && Global::get().s.aotbAlwaysOnTop == Settings::OnTopInMinimal)
		|| (!Global::get().s.bMinimalView && Global::get().s.aotbAlwaysOnTop == Settings::OnTopInNormal)) {
		f |= Qt::WindowStaysOnTopHint;
	}

	if (!graphicsProxyWidget())
		setWindowFlags(f);

	if (Global::get().s.bShowContextMenuInMenuBar) {
		bool found = false;
		for (QAction *a : menuBar()->actions()) {
			if (a == qmUser->menuAction()) {
				found = true;
				break;
			}
		}

		if (!found) {
			menuBar()->insertMenu(qmConfig->menuAction(), qmUser);
			menuBar()->insertMenu(qmConfig->menuAction(), qmChannel);
		}
	} else {
		menuBar()->removeAction(qmUser->menuAction());
		menuBar()->removeAction(qmChannel->menuAction());
	}

	if (Global::get().s.bEnableDeveloperMenu) {
		bool found = false;
		for (QAction *a : menuBar()->actions()) {
			if (a == qmDeveloper->menuAction()) {
				found = true;
				break;
			}
		}

		if (!found) {
			menuBar()->insertMenu(qmHelp->menuAction(), qmDeveloper);
		}
	} else {
		menuBar()->removeAction(qmDeveloper->menuAction());
	}

	if (toggle_minimize) {
		loadState(!showit);
	} else {
		QRect newgeom = frameGeometry();
		resize(geometry().width() - newgeom.width() + geom.width(),
			   geometry().height() - newgeom.height() + geom.height());
		move(geom.x(), geom.y());
	}

	// Explicitly hide UI elements, if we're entering minimal view
	// Note that showing them again is handled above via restoreState/restoreGeometry calls
	if (!showit) {
		qdwLog->setVisible(false);
		qdwChat->setVisible(false);
		if (qtIconToolbar) {
			qtIconToolbar->setVisible(false);
		}
	}
	const bool showNativeChrome = showit && windowLayout != Settings::LayoutModern;
	menuBar()->setVisible(showNativeChrome);
	if (qtIconToolbar) {
		qtIconToolbar->toggleViewAction()->setChecked(showNativeChrome);
		qtIconToolbar->setVisible(showNativeChrome);
	}

	if (showit) {
		qdwMinimalViewNote->hide();
	} else if (!Global::get().sh) {
		// Show the note, if we're not connected to a server
		qdwMinimalViewNote->show();
	}

	// Display the Transmit Mode Dropdown, if configured to do so, otherwise
	// hide it.
	if (Global::get().s.bShowTransmitModeComboBox) {
		qaTransmitMode->setVisible(true);
		qaTransmitModeSeparator->setVisible(true);
	} else {
		qaTransmitMode->setVisible(false);
		qaTransmitModeSeparator->setVisible(false);
	}

	// If activated show the PTT window
	if (Global::get().s.bShowPTTButtonWindow && Global::get().s.atTransmit == Settings::PushToTalk) {
		if (qwPTTButtonWidget) {
			qwPTTButtonWidget->show();
		} else {
			qwPTTButtonWidget = new PTTButtonWidget();
			qwPTTButtonWidget->show();
			connect(qwPTTButtonWidget, SIGNAL(triggered(bool, QVariant)),
					SLOT(on_PushToTalk_triggered(bool, QVariant)));
		}
	} else {
		if (qwPTTButtonWidget) {
			qwPTTButtonWidget->deleteLater();
			qwPTTButtonWidget = nullptr;
		}
	}
}

void MainWindow::on_qaServerConnect_triggered(bool autoconnect) {
	openServerConnectDialog(autoconnect);
}

void MainWindow::on_Reconnect_timeout() {
	if (Global::get().sh->isRunning()) {
		return;
	}

	if (!m_reconnectSoundBlocker) {
		m_reconnectSoundBlocker = std::make_unique< NotificationSoundBlocker >(Log::MsgType::ServerDisconnected);
	}

	Global::get().l->log(Log::Information, tr("Reconnecting."));
	Global::get().sh->start(QThread::TimeCriticalPriority);
}

void MainWindow::on_qmSelf_aboutToShow() {
	ClientUser *user = ClientUser::get(Global::get().uiSession);

	qaServerTexture->setEnabled(user != nullptr);
	qaSelfComment->setEnabled(user != nullptr);

	qaServerTextureRemove->setEnabled(user && !user->qbaTextureHash.isEmpty());

	const bool canSelfRegister = user && (user->iId < 0) && !user->qsHash.isEmpty()
								 && (Global::get().pPermissions & (ChanACL::SelfRegister | ChanACL::Write));
	qaSelfRegister->setEnabled(canSelfRegister);
	if (user && user->iId >= 0) {
		qaSelfRegister->setToolTip(tr("This certificate is already registered on this server."));
		qaSelfRegister->setStatusTip(qaSelfRegister->toolTip());
	} else if (user && user->qsHash.isEmpty()) {
		qaSelfRegister->setToolTip(tr("Connect with a certificate before registering on this server."));
		qaSelfRegister->setStatusTip(qaSelfRegister->toolTip());
	} else if (user && !(Global::get().pPermissions & (ChanACL::SelfRegister | ChanACL::Write))) {
		qaSelfRegister->setToolTip(tr("The server is not allowing self-registration for this account."));
		qaSelfRegister->setStatusTip(qaSelfRegister->toolTip());
	} else {
		qaSelfRegister->setToolTip(QString());
		qaSelfRegister->setStatusTip(QString());
	}
	if (Global::get().sh && Global::get().sh->m_version >= Version::fromComponents(1, 2, 3)) {
		qaSelfPrioritySpeaker->setEnabled(user && Global::get().pPermissions & (ChanACL::Write | ChanACL::MuteDeafen));
		qaSelfPrioritySpeaker->setChecked(user && user->bPrioritySpeaker);
	} else {
		qaSelfPrioritySpeaker->setEnabled(false);
		qaSelfPrioritySpeaker->setChecked(false);
	}
}

void MainWindow::on_qaSelfComment_triggered() {
	openSelfCommentDialog();
}

void MainWindow::on_qaSelfRegister_triggered() {
	selfRegister();
}

void MainWindow::qcbTransmitMode_activated(int index) {
	switch (index) {
		case 0: // Continuous
			setTransmissionMode(Settings::Continuous);
			break;
		case 1: // Voice Activity
			setTransmissionMode(Settings::VAD);
			break;
		case 2: // Push-to-Talk
			setTransmissionMode(Settings::PushToTalk);
			break;
	}
}

void MainWindow::on_qmServer_aboutToShow() {
	qmServer->clear();
	qmServer->addAction(qaServerConnect);
	qmServer->addSeparator();
	qmServer->addAction(qaServerDisconnect);
	qmServer->addAction(qaServerInformation);
	qmServer->addAction(qaServerAddToFavorites);
	qmServer->addAction(qaSearch);
	qmServer->addAction(qaServerTokens);
	qmServer->addAction(qaServerUserList);
	qmServer->addAction(qaServerBanList);
	qmServer->addSeparator();
	qmServer->addAction(qaCreateTextRoom);
	qmServer->addAction(qaServerSettings);
#ifndef Q_OS_MACOS
	// On macOS, the "Quit" action is automatically placed in the application menu
	// by Qt when the QAction's menuRole is set to QAction::QuitRole (see MainWindow.ui).
	// Adding it manually to the "Server" menu would result in duplicate entries.
	// See GitHub issue #7151: Move the Quit button to the "Mumble" application menu on macOS.
	qmServer->addSeparator();
	qmServer->addAction(qaQuit);
#endif

	qaServerBanList->setEnabled(Global::get().pPermissions & (ChanACL::Ban | ChanACL::Write));
	qaServerUserList->setEnabled(Global::get().pPermissions & (ChanACL::Register | ChanACL::Write));
	qaServerInformation->setEnabled(Global::get().uiSession != 0);
	updateFavoriteButton();
	qaServerTokens->setEnabled(Global::get().uiSession != 0);
	if (qaCreateTextRoom) {
		qaCreateTextRoom->setEnabled(canCreateTextRoom() || canCreateAnyVoiceRoom());
	}
	if (qaServerSettings) {
		qaServerSettings->setEnabled(canManagePersistentTextChannels());
	}

	if (!qlServerActions.isEmpty()) {
		qmServer->addSeparator();
		for (QAction *a : qlServerActions) {
			qmServer->addAction(a);
		}
	}
}

void MainWindow::on_qaServerDisconnect_triggered() {
	disconnectFromServer();
}

void MainWindow::on_qaServerBanList_triggered() {
	openServerBanListDialog();
}

void MainWindow::on_qaServerUserList_triggered() {
	openServerUserListDialog();
}

void MainWindow::on_qaServerInformation_triggered() {
	openServerInformationDialog();
}

void MainWindow::on_qaServerSettings_triggered() {
	openServerSettingsDialog();
}

void MainWindow::on_qaCreateTextRoom_triggered() {
	createRoom(RoomCreateType::Text);
}

void MainWindow::on_qaServerTexture_triggered() {
	changeServerTexture();
}

void MainWindow::on_qaServerTextureRemove_triggered() {
	removeServerTexture();
}

void MainWindow::on_qaServerTokens_triggered() {
	openServerTokensDialog();
}

void MainWindow::voiceRecorderDialog_finished(int) {
	voiceRecorderDialog->deleteLater();
	voiceRecorderDialog = nullptr;
}

void MainWindow::qmUser_aboutToShow() {
	ClientUser *p = getContextMenuTargets().user;

	const ClientUser *self = ClientUser::get(Global::get().uiSession);
	bool isSelf            = p == self;

	qmUser->clear();

	if (self && p && !isSelf && self->cChannel != p->cChannel) {
		qmUser->addAction(qaUserJoin);
		qmUser->addAction(qaUserMove);
		qmUser->addSeparator();
	}

	if (p && p->cChannel && m_screenShareManager) {
		const QString streamID = screenShareStreamForChannel(p->cChannel);
		if (!streamID.isEmpty()) {
			const ScreenShareSession session = m_screenShareManager->sessions().value(streamID);
			if (session.ownerSession == p->uiSession) {
				bool addedShareAction = false;
				if (session.ownerSession == Global::get().uiSession) {
					if (m_screenShareManager->isPublishingSession(streamID)
						|| m_screenShareManager->hasDetachedWindow(streamID)) {
						qmUser->addAction(qaChannelScreenShareOpenWindow);
						qaChannelScreenShareOpenWindow->setEnabled(true);
					}
					qmUser->addAction(qaChannelScreenShareStop);
					qaChannelScreenShareStop->setEnabled(true);
					addedShareAction = true;
				} else if (m_screenShareManager->isViewingSession(streamID)) {
					qmUser->addAction(qaChannelScreenShareOpenWindow);
					qaChannelScreenShareOpenWindow->setEnabled(true);
					qmUser->addAction(qaChannelScreenShareStopWatching);
					qaChannelScreenShareStopWatching->setEnabled(true);
					addedShareAction = true;
				} else {
					qmUser->addAction(qaChannelScreenShareWatch);
					qaChannelScreenShareWatch->setEnabled(m_screenShareManager->canViewSession(streamID));
					addedShareAction = true;
				}

				if (addedShareAction) {
					qmUser->addSeparator();
				}
			}
		}
	}

	if (Global::get().pPermissions & (ChanACL::Kick | ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserKick);
	if (Global::get().pPermissions & (ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserBan);
	qmUser->addAction(qaUserMute);
	qmUser->addAction(qaUserDeaf);
	if (Global::get().sh && Global::get().sh->m_version >= Version::fromComponents(1, 2, 3))
		qmUser->addAction(qaUserPrioritySpeaker);
	qmUser->addAction(qaUserLocalMute);
#ifdef USE_RNNOISE
	qmUser->addAction(qaUserRemoteSpeechCleanup);
#endif
	qmUser->addAction(qaUserLocalIgnore);
	if (Global::get().s.bTTS)
		qmUser->addAction(qaUserLocalIgnoreTTS);

	if (p && !isSelf) {
		qmUser->addSeparator();
		qmUser->addAction(m_localVolumeLabel.get());
		m_userLocalVolumeSlider->setUser(p->uiSession);
		qmUser->addAction(m_userLocalVolumeSlider.get());
		qmUser->addSeparator();
	}

	qmUser->addAction(qaUserLocalNickname);

	if (isSelf)
		qmUser->addAction(qaSelfComment);
	else {
		qmUser->addAction(qaUserCommentView);
		qmUser->addAction(qaUserCommentReset);
		qmUser->addAction(qaUserTextureReset);
	}

	qmUser->addAction(qaUserTextMessage);
	if (Global::get().sh && Global::get().sh->m_version >= Version::fromComponents(1, 2, 2))
		qmUser->addAction(qaUserInformation);

	const bool canOfferSelfRegister = isSelf && p && (p->iId < 0);
	const bool canOfferUserRegister = !isSelf && p && (p->iId < 0) && !p->qsHash.isEmpty()
									  && (Global::get().pPermissions & (ChanACL::Register | ChanACL::Write));
	if (canOfferSelfRegister || canOfferUserRegister) {
		qmUser->addSeparator();
		qaUserRegister->setEnabled(isSelf ? qaSelfRegister->isEnabled() : !p->qsHash.isEmpty());
		qmUser->addAction(qaUserRegister);
	}

	if (p && !isSelf) {
		qmUser->addSeparator();
		qaUserFriendAdd->setEnabled(!p->qsHash.isEmpty());
		qaUserFriendUpdate->setEnabled(!p->qsHash.isEmpty());
		qaUserFriendRemove->setEnabled(!p->qsHash.isEmpty());
		if (p->qsFriendName.isEmpty())
			qmUser->addAction(qaUserFriendAdd);
		else {
			if (p->qsFriendName != p->qsName)
				qmUser->addAction(qaUserFriendUpdate);
			qmUser->addAction(qaUserFriendRemove);
		}
	}

	if (isSelf) {
		qmUser->addSeparator();
		qmUser->addAction(qaAudioMute);
		qmUser->addAction(qaAudioDeaf);
	}

#ifndef Q_OS_MAC
	if (Global::get().s.bMinimalView) {
		qmUser->addSeparator();
		qmUser->addMenu(qmServer);
		qmUser->addMenu(qmSelf);
		qmUser->addMenu(qmConfig);
		qmUser->addMenu(qmHelp);
	}
#endif

	if (!qlUserActions.isEmpty()) {
		qmUser->addSeparator();
		for (QAction *a : qlUserActions) {
			qmUser->addAction(a);
		}
	}

	if (!p) {
		qaUserKick->setEnabled(false);
		qaUserBan->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
		qaUserLocalNickname->setEnabled(false);
		qaUserLocalMute->setEnabled(false);
		qaUserRemoteSpeechCleanup->setEnabled(false);
		qaUserRemoteSpeechCleanup->setChecked(false);
		qaUserLocalIgnore->setEnabled(false);
		qaUserLocalIgnoreTTS->setEnabled(false);
		qaUserCommentReset->setEnabled(false);
		qaUserTextureReset->setEnabled(false);
		qaUserCommentView->setEnabled(false);
	} else {
		qaUserKick->setEnabled(!isSelf);
		qaUserBan->setEnabled(!isSelf);
		qaUserTextMessage->setEnabled(true);
		qaUserLocalNickname->setEnabled(!isSelf);
		qaUserLocalMute->setEnabled(!isSelf);
		qaUserRemoteSpeechCleanup->setEnabled(!isSelf);
		qaUserRemoteSpeechCleanup->setChecked(!isSelf && p->isRemoteSpeechCleanupEnabled());
		qaUserLocalIgnore->setEnabled(!isSelf);
		qaUserLocalIgnoreTTS->setEnabled(!isSelf);
		// If the server's version is less than 1.4.0 it won't support the new permission to reset a comment/avatar, so
		// fall back to the old method
		if (Global::get().sh->m_version < Version::fromComponents(1, 4, 0)) {
			qaUserCommentReset->setEnabled(!p->qbaCommentHash.isEmpty()
										   && (Global::get().pPermissions & (ChanACL::Move | ChanACL::Write)));
			qaUserTextureReset->setEnabled(!p->qbaTextureHash.isEmpty()
										   && (Global::get().pPermissions & (ChanACL::Move | ChanACL::Write)));
		} else {
			qaUserCommentReset->setEnabled(
				!p->qbaCommentHash.isEmpty()
				&& (Global::get().pPermissions & (ChanACL::ResetUserContent | ChanACL::Write)));
			qaUserTextureReset->setEnabled(
				!p->qbaTextureHash.isEmpty()
				&& (Global::get().pPermissions & (ChanACL::ResetUserContent | ChanACL::Write)));
		}
		qaUserCommentView->setEnabled(!p->qbaCommentHash.isEmpty());

		qaUserMute->setChecked(p->bMute || p->bSuppress);
		qaUserDeaf->setChecked(p->bDeaf);
		qaUserPrioritySpeaker->setChecked(p->bPrioritySpeaker);
		qaUserLocalMute->setChecked(p->bLocalMute);
		qaUserLocalIgnore->setChecked(p->bLocalIgnore);
		qaUserLocalIgnoreTTS->setChecked(p->bLocalIgnoreTTS);
	}
	updateMenuPermissions();
}

void MainWindow::qmListener_aboutToShow() {
	ClientUser *p = getContextMenuTargets().user;

	bool self = p && (p->uiSession == Global::get().uiSession);

	qmListener->clear();

	if (self) {
		qmListener->addAction(m_localVolumeLabel.get());
		Channel *channel = getContextMenuChannel();
		if (channel) {
			m_listenerVolumeSlider->setListenedChannel(*channel);
			qmListener->addAction(m_listenerVolumeSlider.get());
			qmListener->addSeparator();
		}

		if (cContextChannel) {
			qmListener->addAction(qaChannelListen);
			qaChannelListen->setChecked(
				Global::get().channelListenerManager->isListening(Global::get().uiSession, cContextChannel->iId));
		}
	} else {
		qmListener->addAction(qaEmpty);
	}
}

void MainWindow::startChannelScreenShare() {
	Channel *c = getContextMenuChannel();
	if (!c || !m_screenShareManager) {
		return;
	}

	m_screenShareManager->requestStartChannelShare(static_cast< unsigned int >(c->iId));
}

void MainWindow::stopChannelScreenShare() {
	Channel *c = getContextMenuChannel();
	if (!c || !m_screenShareManager) {
		return;
	}

	const QString streamID = screenShareStreamForChannel(c);
	if (streamID.isEmpty()) {
		return;
	}

	m_screenShareManager->requestStopShare(streamID);
}

void MainWindow::watchChannelScreenShare() {
	Channel *c = getContextMenuChannel();
	if (!c || !m_screenShareManager) {
		return;
	}

	const QString streamID = screenShareStreamForChannel(c);
	if (streamID.isEmpty()) {
		return;
	}

	m_screenShareManager->requestStartViewing(streamID);
}

void MainWindow::stopWatchingChannelScreenShare() {
	Channel *c = getContextMenuChannel();
	if (!c || !m_screenShareManager) {
		return;
	}

	const QString streamID = screenShareStreamForChannel(c);
	if (streamID.isEmpty()) {
		return;
	}

	m_screenShareManager->requestStopViewing(streamID);
}

void MainWindow::openChannelScreenShareWindow() {
	Channel *c = getContextMenuChannel();
	if (!c || !m_screenShareManager) {
		return;
	}

	const QString streamID = screenShareStreamForChannel(c);
	if (streamID.isEmpty()) {
		return;
	}

	m_screenShareManager->focusOrReopenDetachedWindow(streamID);
}

void MainWindow::on_qaUserMute_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	if (p->bMute || p->bSuppress) {
		if (p->bMute)
			mpus.set_mute(false);
		if (p->bSuppress)
			mpus.set_suppress(false);
	} else {
		mpus.set_mute(true);
	}
	Global::get().sh->sendMessage(mpus);
}

void MainWindow::on_qaUserLocalMute_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p) {
		return;
	}

	bool muted = qaUserLocalMute->isChecked();

	p->setLocalMute(muted);
	if (!p->qsHash.isEmpty()) {
		Global::get().db->setLocalMuted(p->qsHash, muted);
	} else {
		logChangeNotPermanent(QObject::tr("Local Mute"), p);
	}
}

void MainWindow::triggerUserRemoteSpeechCleanup() {
	ClientUser *p = getContextMenuUser();
	if (!p) {
		return;
	}

	const bool enabled = qaUserRemoteSpeechCleanup->isChecked();
	const std::optional< bool > override =
		enabled == Global::get().s.remoteSpeechCleanupEnabled ? std::nullopt : std::make_optional(enabled);

	p->setRemoteSpeechCleanupOverride(override);
	if (!p->qsHash.isEmpty()) {
		Global::get().db->setUserRemoteSpeechCleanup(p->qsHash, override);
	} else if (override.has_value()) {
		logChangeNotPermanent(QObject::tr("Remote Speech Cleanup"), p);
	}
}

void MainWindow::on_qaUserLocalIgnore_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p) {
		return;
	}

	bool ignored = qaUserLocalIgnore->isChecked();

	p->setLocalIgnore(ignored);
	if (!p->qsHash.isEmpty()) {
		Global::get().db->setLocalIgnored(p->qsHash, ignored);
	} else {
		logChangeNotPermanent(QObject::tr("Ignore Messages"), p);
	}
}

void MainWindow::on_qaUserLocalIgnoreTTS_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p) {
		return;
	}

	bool ignoredTTS = qaUserLocalIgnoreTTS->isChecked();

	p->setLocalIgnoreTTS(ignoredTTS);
	if (!p->qsHash.isEmpty()) {
		Global::get().db->setLocalIgnoredTTS(p->qsHash, ignoredTTS);
	} else {
		logChangeNotPermanent(QObject::tr("Disable Text-To-Speech"), p);
	}
}

void MainWindow::on_qaUserDeaf_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_deaf(!p->bDeaf);
	Global::get().sh->sendMessage(mpus);
}

void MainWindow::on_qaSelfPrioritySpeaker_triggered() {
	toggleSelfPrioritySpeaker();
}

void MainWindow::on_qaUserPrioritySpeaker_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_priority_speaker(!p->bPrioritySpeaker);
	Global::get().sh->sendMessage(mpus);
}

void MainWindow::on_qaUserRegister_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	unsigned int session = p->uiSession;

	QMessageBox::StandardButton result;

	if (session == Global::get().uiSession)
		result = QMessageBox::question(
			this, tr("Register yourself as %1").arg(p->qsName),
			tr("<p>You are about to register yourself on this server. This action cannot be undone, and your username "
			   "cannot be changed once this is done. You will forever be known as '%1' on this server.</p><p>Are you "
			   "sure you want to register yourself?</p>")
				.arg(p->qsName.toHtmlEscaped()),
			QMessageBox::Yes | QMessageBox::No);
	else
		result = QMessageBox::question(
			this, tr("Register user %1").arg(p->qsName),
			tr("<p>You are about to register %1 on the server. This action cannot be undone, the username cannot be "
			   "changed, and as a registered user, %1 will have access to the server even if you change the server "
			   "password.</p><p>From this point on, %1 will be authenticated with the certificate currently in "
			   "use.</p><p>Are you sure you want to register %1?</p>")
				.arg(p->qsName.toHtmlEscaped()),
			QMessageBox::Yes | QMessageBox::No);

	if (result == QMessageBox::Yes) {
		p = ClientUser::get(session);
		if (!p)
			return;
		Global::get().sh->registerUser(p->uiSession);
	}
}

void MainWindow::on_qaUserFriendAdd_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p || p->qsHash.isEmpty())
		return;

	Global::get().db->addFriend(p->qsName, p->qsHash);
	pmModel->setFriendName(p, p->qsName);
}

void MainWindow::on_qaUserFriendUpdate_triggered() {
	on_qaUserFriendAdd_triggered();
}

void MainWindow::on_qaUserFriendRemove_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p || p->qsHash.isEmpty())
		return;

	Global::get().db->removeFriend(p->qsHash);
	pmModel->setFriendName(p, QString());
}

void MainWindow::on_qaUserKick_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p) {
		return;
	}

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking user %1").arg(p->qsName), tr("Enter reason"),
										   QLineEdit::Normal, QString(), &ok);

	p = ClientUser::get(session);
	if (!p) {
		return;
	}

	if (ok) {
		Global::get().sh->kickUser(p->uiSession, reason);
	}
}

void MainWindow::on_qaUserBan_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p) {
		return;
	}

	BanDialog *banDialog = new BanDialog(*p, this);
	banDialog->show();
}

void MainWindow::on_qaUserTextMessage_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	openTextMessageDialog(p);
}

void MainWindow::openTextMessageDialog(ClientUser *p) {
	unsigned int session = p->uiSession;

	::TextMessage *texm = new ::TextMessage(this, tr("Sending message to %1").arg(p->qsName));
	int res             = texm->exec();

	// Try to get find the user using the session id.
	// This will return nullptr if the user disconnected while typing the message.
	p = ClientUser::get(session);

	if (p && (res == QDialog::Accepted)) {
		QString msg = texm->message();

		if (!msg.isEmpty()) {
			Global::get().sh->sendUserTextMessage(p->uiSession, msg);
			Global::get().l->log(Log::TextMessage,
								 tr("To %1: %2").arg(Log::formatClientUser(p, Log::Target), texm->message()),
								 tr("Message to %1").arg(p->qsName), true);
		}
	}
	delete texm;
}

void MainWindow::on_qaUserLocalNickname_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	openUserLocalNicknameDialog(*p);
}

void MainWindow::openUserLocalNicknameDialog(const ClientUser &p) {
	unsigned int session = p.uiSession;
	UserLocalNicknameDialog::present(session, qmUserNicknameTracker, this);
}

void MainWindow::on_qaUserCommentView_triggered() {
	ClientUser *p = getContextMenuUser();
	// This has to be done here because UserModel could've set it.
	cuContextUser.clear();

	if (!p)
		return;

	if (!p->qbaCommentHash.isEmpty() && p->qsComment.isEmpty()) {
		p->qsComment = QString::fromUtf8(Global::get().db->blob(p->qbaCommentHash));
		if (p->qsComment.isEmpty()) {
			pmModel->uiSessionComment = ~(p->uiSession);
			MumbleProto::RequestBlob mprb;
			mprb.add_session_comment(p->uiSession);
			Global::get().sh->sendMessage(mprb);
			return;
		}
	}

	pmModel->seenComment(pmModel->index(p));

	::TextMessage *texm = new ::TextMessage(this, tr("View comment on user %1").arg(p->qsName));

	texm->rteMessage->setText(p->qsComment, true);
	texm->setAttribute(Qt::WA_DeleteOnClose, true);
	texm->show();
}

void MainWindow::on_qaUserCommentReset_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	unsigned int session = p->uiSession;

	int ret = QMessageBox::question(
		this, QLatin1String("Mumble"),
		tr("Are you sure you want to reset the comment of user %1?").arg(p->qsName.toHtmlEscaped()), QMessageBox::Yes,
		QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		Global::get().sh->setUserComment(session, QString());
	}
}

void MainWindow::on_qaUserTextureReset_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	unsigned int session = p->uiSession;

	int ret = QMessageBox::question(
		this, QLatin1String("Mumble"),
		tr("Are you sure you want to reset the avatar of user %1?").arg(p->qsName.toHtmlEscaped()), QMessageBox::Yes,
		QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		Global::get().sh->setUserTexture(session, QByteArray());
	}
}

void MainWindow::on_qaUserInformation_triggered() {
	ClientUser *p = getContextMenuUser();

	if (!p)
		return;

	m_pendingUserInformationSessions.insert(p->uiSession);
	Global::get().sh->requestUserStats(p->uiSession, false);
}

void MainWindow::on_qaQuit_triggered() {
	forceQuit = true;
	this->close();
}

void MainWindow::sendChatbarText(QString qsText, bool plainText) {
	const PersistentChatTarget target = currentPersistentChatTarget();
	if (target.valid && !target.readOnly && !target.directMessage && !target.legacyTextPath
		&& m_persistentChatGateway) {
		const QString normalizedText =
			qsText.replace(QLatin1String("\r\n"), QLatin1String("\n")).replace(QLatin1Char('\r'), QLatin1Char('\n'));
		syncPersistentChatGatewayHandler();
		m_persistentChatGateway->send(
			target.scope, target.scopeID, normalizedText,
			plainText ? MumbleProto::ChatBodyFormatPlainText : MumbleProto::ChatBodyFormatMarkdownLite,
			m_pendingPersistentChatReply ? std::optional< unsigned int >(m_pendingPersistentChatReply->message_id())
										 : std::nullopt);
		setPersistentChatReplyTarget(std::nullopt);
		qteChat->clear();
		return;
	}

	if (plainText) {
		// Escape HTML, unify line endings, then convert spaces to non-breaking ones to prevent multiple
		// simultaneous ones from being collapsed into one (as a normal HTML renderer does).
		qsText = qsText.toHtmlEscaped()
					 .replace("\r\n", "\n")
					 .replace("\r", "\n")
					 .replace("\n", "<br>")
					 .replace(" ", "&nbsp;");
	} else {
		// Markdown::markdownToHTML also takes care of replacing line breaks (\n) with the respective
		// HTML code <br/>. Therefore if Markdown support is ever going to be removed from this
		// function, this job has to be done explicitly as otherwise line breaks won't be shown on
		// the receiving end of this text message.
		qsText = Markdown::markdownToHTML(qsText);
	}

	sendChatbarMessage(qsText);

	qteChat->clear();
}

void MainWindow::sendChatbarMessage(QString qsMessage) {
	if (Global::get().uiSession == 0)
		return; // Check if text & connection is available

	const PersistentChatTarget target = currentPersistentChatTarget();
	if (!target.valid || target.readOnly) {
		return;
	}

	if (m_pendingPersistentChatReply) {
		if (target.directMessage || target.legacyTextPath) {
			qsMessage = buildPersistentChatReplyHtml(*m_pendingPersistentChatReply, qsMessage);
		}
	}

	if (target.directMessage && target.user) {
		Global::get().sh->sendUserTextMessage(target.user->uiSession, qsMessage);
		Global::get().l->log(Log::TextMessage,
							 tr("To %1: %2").arg(Log::formatClientUser(target.user, Log::Target), qsMessage),
							 tr("Message to %1").arg(target.user->qsName), true);
		setPersistentChatReplyTarget(std::nullopt);
		return;
	}

	if (target.legacyTextPath && target.channel) {
		Global::get().sh->sendChannelTextMessage(target.channel->iId, qsMessage, false);
		Global::get().l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatChannel(target.channel), qsMessage),
							 tr("Message to channel %1").arg(target.channel->qsName), true);
		setPersistentChatReplyTarget(std::nullopt);
		return;
	}

	if (m_persistentChatGateway) {
		syncPersistentChatGatewayHandler();
		m_persistentChatGateway->send(target.scope, target.scopeID, qsMessage, MumbleProto::ChatBodyFormatPlainText,
									  m_pendingPersistentChatReply
										  ? std::optional< unsigned int >(m_pendingPersistentChatReply->message_id())
										  : std::nullopt);
	}
	setPersistentChatReplyTarget(std::nullopt);
}

/// Handles Backtab/Shift-Tab for qteChat, which allows
/// users to move focus to the previous widget in
/// MainWindow.
void MainWindow::on_qteChat_backtabPressed() {
	focusPreviousChild();
}

void MainWindow::on_qteChat_ctrlSpacePressed() {
	autocompleteUsername();
}

void MainWindow::on_qteChat_tabPressed() {
	// Only autocomplete the username, if the user entered text starts with a "@".
	// Otherwise TAB should be reserved for accessible keyboard navigation.
	QString currentText = qteChat->toPlainText();
	if (currentText.startsWith("@")) {
		currentText.remove(0, 1);

		qteChat->clear();
		QTextCursor tc = qteChat->textCursor();
		tc.insertText(currentText);
		qteChat->setTextCursor(tc);

		autocompleteUsername();
		return;
	}

	focusNextMainWidget();
}

void MainWindow::autocompleteUsername() {
	unsigned int res = qteChat->completeAtCursor();
	if (res == 0) {
		return;
	}
	qtvUsers->setCurrentIndex(pmModel->index(ClientUser::get(res)));
}

void MainWindow::on_qmConfig_aboutToShow() {
	// Don't remove the config, as that messes up OSX.
	for (QAction *a : qmConfig->actions()) {
		if (a != qaConfigDialog) {
			qmConfig->removeAction(a);
		}
	}
	qmConfig->addAction(qaAudioWizard);
	qmConfig->addAction(qaConfigCert);
	qmConfig->addSeparator();
	qaAudioTTS->setChecked(Global::get().s.bTTS);
	qmConfig->addAction(qaAudioTTS);
	qmConfig->addSeparator();
	qmConfig->addAction(qaConfigMinimal);
	qmConfig->addAction(qaFilterToggle);

	qaTalkingUIToggle->setChecked(Global::get().talkingUI && Global::get().talkingUI->isVisible());

	qmConfig->addAction(qaTalkingUIToggle);
	if (Global::get().s.bMinimalView)
		qmConfig->addAction(qaConfigHideFrame);
}

void MainWindow::qmChannel_aboutToShow() {
	Channel *c                         = getContextMenuTargets().channel;
	const ClientUser *self             = ClientUser::get(Global::get().uiSession);
	const QString startScreenShareText = tr("Start Screen Share");

	qmChannel->clear();
	qaChannelScreenShareStart->setText(startScreenShareText);
	qaChannelScreenShareStart->setToolTip(QString());
	qaChannelScreenShareStart->setStatusTip(QString());

	if (c && self && self->cChannel && c->iId != self->cChannel->iId) {
		qmChannel->addAction(qaChannelJoin);
	}

	if (c && Global::get().sh && Global::get().sh->m_version >= Version::fromComponents(1, 4, 0)) {
		// If the server's version is less than 1.4, the listening feature is not supported yet
		// and thus it doesn't make sense to show the action for it
		qmChannel->addAction(qaChannelListen);
		qaChannelListen->setChecked(Global::get().channelListenerManager->isListening(Global::get().uiSession, c->iId));
	}

	if (c && self && self->cChannel && m_screenShareManager) {
		const QString streamID      = screenShareStreamForChannel(c);
		const bool hasScreenShare   = !streamID.isEmpty();
		const bool isCurrentChannel = c->iId == self->cChannel->iId;
		bool addedScreenShareAction = false;

		if (isCurrentChannel && !hasScreenShare) {
			const QString unavailableReason = m_screenShareManager->localShareUnavailableReason();
			qmChannel->addAction(qaChannelScreenShareStart);
			qaChannelScreenShareStart->setEnabled(unavailableReason.isEmpty());
			if (unavailableReason.isEmpty()) {
				qaChannelScreenShareStart->setText(startScreenShareText);
			} else {
				qaChannelScreenShareStart->setText(tr("Start Screen Share (Unavailable)"));
			}
			qaChannelScreenShareStart->setToolTip(unavailableReason);
			qaChannelScreenShareStart->setStatusTip(unavailableReason);
			addedScreenShareAction = true;
		} else if (hasScreenShare) {
			const ScreenShareSession session = m_screenShareManager->sessions().value(streamID);
			if (session.ownerSession == self->uiSession) {
				if (m_screenShareManager->isPublishingSession(streamID)
					|| m_screenShareManager->hasDetachedWindow(streamID)) {
					qmChannel->addAction(qaChannelScreenShareOpenWindow);
					qaChannelScreenShareOpenWindow->setEnabled(true);
				}
				qmChannel->addAction(qaChannelScreenShareStop);
				qaChannelScreenShareStop->setEnabled(true);
			} else if (m_screenShareManager->isViewingSession(streamID)) {
				qmChannel->addAction(qaChannelScreenShareOpenWindow);
				qaChannelScreenShareOpenWindow->setEnabled(true);
				qmChannel->addAction(qaChannelScreenShareStopWatching);
				qaChannelScreenShareStopWatching->setEnabled(true);
			} else {
				qmChannel->addAction(qaChannelScreenShareWatch);
				qaChannelScreenShareWatch->setEnabled(m_screenShareManager->canViewSession(streamID));
			}
			addedScreenShareAction = true;
		}

		if (addedScreenShareAction) {
			qmChannel->addSeparator();
		}
	}

	qmChannel->addSeparator();

	qmChannel->addAction(qaChannelAdd);
	qmChannel->addAction(qaChannelACL);
	qmChannel->addAction(qaChannelRemove);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelLink);
	qmChannel->addAction(qaChannelUnlink);
	qmChannel->addAction(qaChannelUnlinkAll);
	qmChannel->addSeparator();
	qmChannel->addAction(qaChannelCopyURL);
	qmChannel->addAction(qaChannelSendMessage);

	// hiding the root is nonsense
	if (c && c->cParent) {
		qmChannel->addSeparator();
		qmChannel->addAction(qaChannelHide);
		qmChannel->addAction(qaChannelPin);
	}

#ifndef Q_OS_MAC
	if (Global::get().s.bMinimalView) {
		qmChannel->addSeparator();
		qmChannel->addMenu(qmServer);
		qmChannel->addMenu(qmSelf);
		qmChannel->addMenu(qmConfig);
		qmChannel->addMenu(qmHelp);
	}
#endif

	if (!qlChannelActions.isEmpty()) {
		qmChannel->addSeparator();
		for (QAction *a : qlChannelActions) {
			qmChannel->addAction(a);
		}
	}

	bool add, remove, acl, link, unlink, unlinkall, msg;
	add = remove = acl = link = unlink = unlinkall = msg = false;

	if (Global::get().uiSession != 0) {
		add = true;
		acl = true;
		msg = true;

		Channel *home = ClientUser::get(Global::get().uiSession)->cChannel;

		if (c && c->iId != 0) {
			remove = true;
		}
		if (!c) {
			c = Channel::get(Mumble::ROOT_CHANNEL_ID);
		}
		unlinkall = (home->qhLinks.count() > 0);
		if (home != c) {
			if (c->allLinks().contains(home)) {
				unlink = true;
			} else {
				link = true;
			}
		}
	}

	if (c) {
		qaChannelHide->setChecked(c->m_filterMode == ChannelFilterMode::HIDE);
		qaChannelPin->setChecked(c->m_filterMode == ChannelFilterMode::PIN);
	}

	qaChannelAdd->setEnabled(add || canCreateTextRoom());
	qaChannelRemove->setEnabled(remove);
	qaChannelACL->setEnabled(acl);
	qaChannelLink->setEnabled(link);
	qaChannelUnlink->setEnabled(unlink);
	qaChannelUnlinkAll->setEnabled(unlinkall);
	qaChannelSendMessage->setEnabled(msg);
	updateMenuPermissions();
}

void MainWindow::on_qaChannelJoin_triggered() {
	Channel *c = getContextMenuChannel();

	if (c) {
		Global::get().sh->joinChannel(Global::get().uiSession, c->iId);
	}
}

void MainWindow::on_qaUserJoin_triggered() {
	const ClientUser *user = getContextMenuUser();

	if (user) {
		const Channel *channel = user->cChannel;

		if (channel) {
			Global::get().sh->joinChannel(Global::get().uiSession, channel->iId);
		}
	}
}

void MainWindow::on_qaUserMove_triggered() {
	const ClientUser *user = getContextMenuUser();

	if (user) {
		const Channel *channel = ClientUser::get(Global::get().uiSession)->cChannel;

		if (channel) {
			Global::get().sh->joinChannel(user->uiSession, channel->iId);
		}
	}
}

void MainWindow::on_qaChannelListen_triggered() {
	Channel *c = getContextMenuChannel();

	if (c) {
		if (qaChannelListen->isChecked()) {
			Global::get().sh->startListeningToChannel(c->iId);
		} else {
			Global::get().sh->stopListeningToChannel(c->iId);
		}
	}
}

void MainWindow::on_qaChannelHide_triggered() {
	Channel *c = getContextMenuChannel();

	if (c) {
		UserModel *um = static_cast< UserModel * >(qtvUsers->model());
		if (qaChannelHide->isChecked()) {
			c->setFilterMode(ChannelFilterMode::HIDE);
		} else {
			c->setFilterMode(ChannelFilterMode::NORMAL);
		}
		um->forceVisualUpdate(c);
	}
}

void MainWindow::on_qaChannelPin_triggered() {
	Channel *c = getContextMenuChannel();

	if (c) {
		UserModel *um = static_cast< UserModel * >(qtvUsers->model());
		if (qaChannelPin->isChecked()) {
			c->setFilterMode(ChannelFilterMode::PIN);
		} else {
			c->setFilterMode(ChannelFilterMode::NORMAL);
		}
		um->forceVisualUpdate(c);
	}
}

void MainWindow::on_qaChannelAdd_triggered() {
	Channel *c = getContextMenuChannel();
	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = nullptr;
	}

	createRoom(RoomCreateType::Voice, c);
}

void MainWindow::on_qaChannelRemove_triggered() {
	int ret;
	Channel *c = getContextMenuChannel();
	if (!c)
		return;

	unsigned int id = c->iId;

	ret = QMessageBox::question(
		this, QLatin1String("Mumble"),
		tr("Are you sure you want to delete %1 and all its sub-channels?").arg(c->qsName.toHtmlEscaped()),
		QMessageBox::Yes, QMessageBox::No);

	c = Channel::get(id);
	if (!c)
		return;

	if (ret == QMessageBox::Yes) {
		Global::get().sh->removeChannel(c->iId);
	}
}

void MainWindow::on_qaChannelACL_triggered() {
	Channel *c = getContextMenuChannel();
	if (!c)
		c = Channel::get(Mumble::ROOT_CHANNEL_ID);
	unsigned int id = c->iId;

	if (!c->qbaDescHash.isEmpty() && c->qsDesc.isEmpty()) {
		c->qsDesc = QString::fromUtf8(Global::get().db->blob(c->qbaDescHash));
		if (c->qsDesc.isEmpty()) {
			MumbleProto::RequestBlob mprb;
			mprb.add_channel_description(id);
			Global::get().sh->sendMessage(mprb);
		}
	}

	Global::get().sh->requestACL(id);

	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = nullptr;
	}
}

void MainWindow::on_qaChannelLink_triggered() {
	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
	Channel *l = getContextMenuChannel();
	if (!l)
		l = Channel::get(Mumble::ROOT_CHANNEL_ID);

	Global::get().sh->addChannelLink(c->iId, l->iId);
}

void MainWindow::on_qaChannelUnlink_triggered() {
	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
	Channel *l = getContextMenuChannel();
	if (!l)
		l = Channel::get(Mumble::ROOT_CHANNEL_ID);

	Global::get().sh->removeChannelLink(c->iId, l->iId);
}

void MainWindow::on_qaChannelUnlinkAll_triggered() {
	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(c->iId);
	for (Channel *l : c->qsPermLinks) {
		mpcs.add_links_remove(l->iId);
	}
	Global::get().sh->sendMessage(mpcs);
}

void MainWindow::on_qaChannelSendMessage_triggered() {
	Channel *c = getContextMenuChannel();

	if (!c)
		return;

	unsigned int id = c->iId;

	::TextMessage *texm = new ::TextMessage(this, tr("Sending message to channel %1").arg(c->qsName), true);
	int res             = texm->exec();

	c = Channel::get(id);

	if (c && (res == QDialog::Accepted)) {
		Global::get().sh->sendChannelTextMessage(id, texm->message(), texm->bTreeMessage);

		if (texm->bTreeMessage)
			Global::get().l->log(Log::TextMessage, tr("To %1 (Tree): %2").arg(Log::formatChannel(c), texm->message()),
								 tr("Message to tree %1").arg(c->qsName), true);
		else
			Global::get().l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatChannel(c), texm->message()),
								 tr("Message to channel %1").arg(c->qsName), true);
	}
	delete texm;
}

void MainWindow::on_qaChannelCopyURL_triggered() {
	Channel *c = getContextMenuChannel();
	QString host, uname, pw, channel;
	unsigned short port;

	if (!c)
		return;

	Global::get().sh->getConnectionInfo(host, port, uname, pw);
	// walk back up the channel list to build the URL.
	while (c->cParent) {
		channel.prepend(c->qsName);
		channel.prepend(QLatin1String("/"));
		c = c->cParent;
	}

	QApplication::clipboard()->setMimeData(ServerItem::toMimeData(c->qsName, host, port, channel),
										   QClipboard::Clipboard);
}

/**
 * This function updates the UI according to the permission of the user in the current channel.
 * If possible the permissions are fetched from a cache. Otherwise they are requested by the server
 * via a PermissionQuery call (whose reply updates the cache and calls this function again).
 * @see MainWindow::msgPermissionQuery(const MumbleProto::PermissionQuery &msg)
 */
void MainWindow::updateMenuPermissions() {
	ContextMenuTarget target = getContextMenuTargets();

	ChanACL::Permissions p =
		target.channel ? static_cast< ChanACL::Permissions >(target.channel->uiPermissions) : ChanACL::None;

	if (target.channel && !p) {
		Global::get().sh->requestChannelPermissions(target.channel->iId);
		if (target.channel->iId == 0)
			p = Global::get().pPermissions;
		else
			p = ChanACL::All;

		target.channel->uiPermissions = p;
	}

	ClientUser *user           = Global::get().uiSession ? ClientUser::get(Global::get().uiSession) : nullptr;
	Channel *homec             = user ? user->cChannel : nullptr;
	ChanACL::Permissions homep = homec ? static_cast< ChanACL::Permissions >(homec->uiPermissions) : ChanACL::None;
	bool isCurrentChannel      = target.channel && homec == target.channel;

	if (homec && !homep) {
		Global::get().sh->requestChannelPermissions(homec->iId);
		if (homec->iId == 0)
			homep = Global::get().pPermissions;
		else
			homep = ChanACL::All;

		homec->uiPermissions = homep;
	}

	if (target.user) {
		qaUserMute->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen)
							   && ((target.user != user) || target.user->bMute || target.user->bSuppress));
		qaUserDeaf->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen)
							   && ((target.user != user) || target.user->bDeaf));
		qaUserPrioritySpeaker->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen));
		qaUserTextMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
		qaUserInformation->setEnabled((Global::get().pPermissions & (ChanACL::Write | ChanACL::Register))
									  || (p & (ChanACL::Write | ChanACL::Enter)) || (target.user == user));
	} else {
		qaUserMute->setEnabled(false);
		qaUserDeaf->setEnabled(false);
		qaUserPrioritySpeaker->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
		qaUserInformation->setEnabled(false);
	}

	qaChannelJoin->setEnabled(p & (ChanACL::Write | ChanACL::Enter));

	qaChannelAdd->setEnabled((p & (ChanACL::Write | ChanACL::MakeChannel | ChanACL::MakeTempChannel))
							 || canCreateTextRoom());
	qaChannelRemove->setEnabled(p & ChanACL::Write);
	qaChannelACL->setEnabled((p & ChanACL::Write) || (Global::get().pPermissions & ChanACL::Write));

	qaChannelLink->setEnabled(!isCurrentChannel && (p & (ChanACL::Write | ChanACL::LinkChannel))
							  && (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlink->setEnabled(
		!isCurrentChannel
		&& ((p & (ChanACL::Write | ChanACL::LinkChannel)) || (homep & (ChanACL::Write | ChanACL::LinkChannel))));
	qaChannelUnlinkAll->setEnabled(p & (ChanACL::Write | ChanACL::LinkChannel));
	qaChannelCopyURL->setEnabled(target.channel);
	qaChannelSendMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
	qaChannelHide->setEnabled(target.channel);
	qaChannelPin->setEnabled(target.channel);

	bool chatBarEnabled = false;
	if (Global::get().uiSession) {
		if (Global::get().s.bChatBarUseSelection && (target.channel || target.user)) {
			chatBarEnabled = p & (ChanACL::Write | ChanACL::TextMessage);
		} else if (homec) {
			chatBarEnabled = homep & (ChanACL::Write | ChanACL::TextMessage);
		}
	}
	syncPersistentChatInputState(chatBarEnabled);
}

void MainWindow::userStateChanged() {
	emit talkingStatusChanged();
#if defined(MUMBLE_HAS_MODERN_LAYOUT)
	queueModernShellSnapshotSync();
#endif

	ClientUser *user = ClientUser::get(Global::get().uiSession);
	if (!user) {
		Global::get().bAttenuateOthers              = false;
		Global::get().prioritySpeakerActiveOverride = false;

		return;
	}

	switch (user->tsState) {
		case Settings::Talking:
		case Settings::Whispering:
		case Settings::Shouting:
			Global::get().bAttenuateOthers = Global::get().s.bAttenuateOthersOnTalk;

			Global::get().prioritySpeakerActiveOverride =
				Global::get().s.bAttenuateUsersOnPrioritySpeak && user->bPrioritySpeaker;

			break;
		case Settings::Passive:
		case Settings::MutedTalking:
		default:
			Global::get().bAttenuateOthers              = false;
			Global::get().prioritySpeakerActiveOverride = false;
			break;
	}
}

void MainWindow::on_channelStateChanged(Channel *channel, bool forceUpdateTree) {
	if (modernShellMinimalSnapshotEnabled() && usesModernShell()) {
		queueModernShellSnapshotSync();
		return;
	}

	if (channel == pmModel->getChannel(qtvUsers->currentIndex())) {
		updateChatBar();
	}

	if (forceUpdateTree) {
		pmModel->forceVisualUpdate();
	}

	updateServerNavigatorChrome();
}

void MainWindow::on_qaAudioReset_triggered() {
	AudioInputPtr ai = Global::get().ai;
	if (ai)
		ai->bResetProcessor = true;
}

void MainWindow::on_qaFilterToggle_triggered() {
	Global::get().s.bFilterActive = qaFilterToggle->isChecked();
	if (!Global::get().s.bFilterActive) {
		qtvUsers->setAccessibleName(tr("Channels and users"));
	} else {
		qtvUsers->setAccessibleName(tr("Filtered channels and users"));
	}
	updateUserModel();
}

void MainWindow::on_qaAudioMute_triggered() {
	if (Global::get().bInAudioWizard) {
		qaAudioMute->setChecked(!qaAudioMute->isChecked());
		return;
	}

	AudioInputPtr ai = Global::get().ai;
	if (ai)
		ai->tIdle.restart();

	Global::get().s.bMute = qaAudioMute->isChecked();

	if (!Global::get().s.bMute && Global::get().s.bDeaf) {
		Global::get().s.bDeaf = false;
		qaAudioDeaf->setChecked(false);
		Global::get().l->log(Log::SelfUndeaf, tr("Unmuted and undeafened."));
	} else if (!Global::get().s.bMute) {
		Global::get().l->log(Log::SelfUnmute, tr("Unmuted."));
	} else {
		Global::get().l->log(Log::SelfMute, tr("Muted."));
	}

	if (Global::get().sh) {
		Global::get().sh->setSelfMuteDeafState(Global::get().s.bMute, Global::get().s.bDeaf);
	}

	updateAudioToolTips();
	emit talkingStatusChanged();
	queueModernShellSnapshotSync();
}

void MainWindow::setAudioMute(bool mute) {
	// Pretend the user pushed the button manually
	qaAudioMute->setChecked(mute);
	qaAudioMute->triggered(mute);
}

void MainWindow::on_qaAudioDeaf_triggered() {
	if (Global::get().bInAudioWizard) {
		qaAudioDeaf->setChecked(!qaAudioDeaf->isChecked());
		return;
	}

	if (!qaAudioDeaf->isChecked() && Global::get().s.unmuteOnUndeaf) {
		qaAudioDeaf->setChecked(true);
		qaAudioMute->setChecked(false);
		on_qaAudioMute_triggered();
		return;
	}

	AudioInputPtr ai = Global::get().ai;
	if (ai)
		ai->tIdle.restart();

	Global::get().s.bDeaf = qaAudioDeaf->isChecked();

	if (Global::get().s.bDeaf && !Global::get().s.bMute) {
		Global::get().s.unmuteOnUndeaf = true;
		Global::get().s.bMute          = true;
		qaAudioMute->setChecked(true);
		Global::get().l->log(Log::SelfDeaf, tr("Muted and deafened."));
	} else if (Global::get().s.bDeaf) {
		Global::get().l->log(Log::SelfDeaf, tr("Deafened."));
		Global::get().s.unmuteOnUndeaf = false;
	} else {
		Global::get().l->log(Log::SelfUndeaf, tr("Undeafened."));
	}

	if (Global::get().sh) {
		Global::get().sh->setSelfMuteDeafState(Global::get().s.bMute, Global::get().s.bDeaf);
	}

	updateAudioToolTips();
	emit talkingStatusChanged();
	queueModernShellSnapshotSync();
}

void MainWindow::setAudioDeaf(bool deaf) {
	// Pretend the user pushed the button manually
	qaAudioDeaf->setChecked(deaf);
	qaAudioDeaf->triggered(deaf);
}

void MainWindow::on_qaRecording_triggered() {
	recording();
}

void MainWindow::on_qaAudioTTS_triggered() {
	enableAudioTTS(qaAudioTTS->isChecked());
}

void MainWindow::on_qaAudioStats_triggered() {
	openAudioStatsDialog();
}

void MainWindow::on_qaAudioUnlink_triggered() {
	Global::get().pluginManager->unlinkPositionalData();
}

void MainWindow::on_qaConfigDialog_triggered() {
	openConfigDialog();
}

void MainWindow::on_qaConfigMinimal_triggered() {
	Global::get().s.bMinimalView = qaConfigMinimal->isChecked();
	updateWindowTitle();
	setupView();
}

void MainWindow::on_qaConfigHideFrame_triggered() {
	Global::get().s.bHideFrame = qaConfigHideFrame->isChecked();
	setupView(false);
}

void MainWindow::on_qaConfigCert_triggered() {
	openCertWizardDialog();
}

void MainWindow::on_qaAudioWizard_triggered() {
	openAudioWizardDialog();
}

void MainWindow::on_qaDeveloperConsole_triggered() {
	Global::get().c->show();
}

void MainWindow::on_qaPositionalAudioViewer_triggered() {
	if (m_paViewer) {
		m_paViewer->raise();
	} else {
		m_paViewer = std::make_unique< PositionalAudioViewer >(this);
		connect(m_paViewer.get(), &PositionalAudioViewer::finished, this, [this]() { m_paViewer.reset(); });
		m_paViewer->show();
	}
}

void MainWindow::on_qaHelpWhatsThis_triggered() {
	QWhatsThis::enterWhatsThisMode();
}

void MainWindow::on_qaHelpAbout_triggered() {
	openAboutDialog();
}

void MainWindow::on_qaHelpAboutQt_triggered() {
	openAboutQtDialog();
}

void MainWindow::on_qaHelpVersionCheck_triggered() {
	versionCheck();
}

void MainWindow::on_gsMuteSelf_down(QVariant v) {
	int val = v.toInt();
	if (((val > 0) && !Global::get().s.bMute) || ((val < 0) && Global::get().s.bMute) || (val == 0)) {
		qaAudioMute->setChecked(!qaAudioMute->isChecked());
		on_qaAudioMute_triggered();
	}
}

void MainWindow::on_gsDeafSelf_down(QVariant v) {
	int val = v.toInt();
	if (((val > 0) && !Global::get().s.bDeaf) || ((val < 0) && Global::get().s.bDeaf) || (val == 0)) {
		qaAudioDeaf->setChecked(!qaAudioDeaf->isChecked());
		on_qaAudioDeaf_triggered();
	}
}

void MainWindow::on_PushToTalk_triggered(bool down, QVariant) {
	Global::get().iPrevTarget = 0;
	if (down) {
		Global::get().uiDoublePush = static_cast< quint64 >(Global::get().tDoublePush.restart().count());
		Global::get().iPushToTalk++;
	} else if (Global::get().iPushToTalk > 0) {
		QTimer::singleShot(static_cast< int >(Global::get().s.pttHold), this, SLOT(pttReleased()));
	}
}

void MainWindow::pttReleased() {
	if (Global::get().iPushToTalk > 0) {
		Global::get().iPushToTalk--;
	}
}

void MainWindow::on_PushToMute_triggered(bool down, QVariant) {
	Global::get().bPushToMute = down;
	updateUserModel();
	emit talkingStatusChanged();
}

void MainWindow::on_VolumeUp_triggered(bool down, QVariant) {
	if (down) {
		float vol = Global::get().s.fVolume + 0.1f;
		if (vol > 2.0f) {
			vol = 2.0f;
		}
		Global::get().s.fVolume = vol;
	}
}

void MainWindow::on_VolumeDown_triggered(bool down, QVariant) {
	if (down) {
		float vol = Global::get().s.fVolume - 0.1f;
		if (vol < 0.0f) {
			vol = 0.0f;
		}
		Global::get().s.fVolume = vol;
	}
}

Channel *MainWindow::mapChannel(int idx) const {
	if (!Global::get().uiSession)
		return nullptr;

	Channel *c = nullptr;

	if (idx < 0) {
		switch (idx) {
			case SHORTCUT_TARGET_ROOT:
				c = Channel::get(Mumble::ROOT_CHANNEL_ID);
				break;
			case SHORTCUT_TARGET_PARENT:
			case SHORTCUT_TARGET_CURRENT:
				c = ClientUser::get(Global::get().uiSession)->cChannel;
				if (idx == SHORTCUT_TARGET_PARENT)
					c = c->cParent;
				break;
			default:
				if (idx <= SHORTCUT_TARGET_PARENT_SUBCHANNEL)
					c = pmModel->getSubChannel(ClientUser::get(Global::get().uiSession)->cChannel->cParent,
											   SHORTCUT_TARGET_PARENT_SUBCHANNEL - idx);
				else
					c = pmModel->getSubChannel(ClientUser::get(Global::get().uiSession)->cChannel,
											   SHORTCUT_TARGET_SUBCHANNEL - idx);
				break;
		}
	} else {
		c = Channel::get(static_cast< unsigned int >(idx));
	}
	return c;
}

void MainWindow::updateTarget() {
	Global::get().iPrevTarget = Global::get().iTarget;

	if (qmCurrentTargets.isEmpty()) {
		Global::get().bCenterPosition = false;
		Global::get().iTarget         = 0;
	} else {
		bool center = false;
		QList< ShortcutTarget > ql;
		for (const ShortcutTarget &st : qmCurrentTargets.keys()) {
			ShortcutTarget nt;
			center               = center || st.bForceCenter;
			nt.bUsers            = st.bUsers;
			nt.bCurrentSelection = st.bCurrentSelection;

			if (nt.bCurrentSelection) {
				Channel *c = pmModel->getSelectedChannel();
				if (c) {
					nt.bUsers    = false;
					nt.iChannel  = static_cast< int >(c->iId);
					nt.bLinks    = st.bLinks;
					nt.bChildren = st.bChildren;

					ql << nt;
				} else {
					ClientUser *user = pmModel->getSelectedUser();

					if (user) {
						nt.bUsers = true;
						nt.qlSessions << user->uiSession;

						ql << nt;
					}
				}
			} else if (st.bUsers) {
				for (const QString &hash : st.qlUsers) {
					ClientUser *p = pmModel->getUser(hash);
					if (p)
						nt.qlSessions.append(p->uiSession);
				}
				if (!nt.qlSessions.isEmpty())
					ql << nt;
			} else {
				Channel *c = mapChannel(st.iChannel);
				if (c) {
					nt.bLinks    = st.bLinks;
					nt.bChildren = st.bChildren;
					nt.iChannel  = static_cast< int >(c->iId);
					nt.qsGroup   = st.qsGroup;
					ql << nt;
				}
			}
		}
		if (ql.isEmpty()) {
			Global::get().iTarget = -1;
		} else {
			++iTargetCounter;

			int idx = qmTargets.value(ql);
			if (idx == 0) {
				// An idx of 0 means that we don't have a mapping for this shortcut yet
				// Thus we'll register it here
				QMap< int, int > qm;
				QMap< int, int >::const_iterator i;
				// We reverse the qmTargetsUse map into qm so that each key becomes a value and vice versa
				for (i = qmTargetUse.constBegin(); i != qmTargetUse.constEnd(); ++i) {
					qm.insert(i.value(), i.key());
				}

				// The reversal and the promise that when iterating over a QMap, the keys will appear sorted
				// leads to us now being able to get the next target ID as the value of the first entry in
				// the map.
				i   = qm.constBegin();
				idx = i.value();



				// Sets up a VoiceTarget (which is identified by the targetID idx) on the server for the given set
				// of ShortcutTargets
				MumbleProto::VoiceTarget mpvt;
				mpvt.set_id(static_cast< unsigned int >(idx));

				for (const ShortcutTarget &st : ql) {
					MumbleProto::VoiceTarget_Target *t = mpvt.add_targets();
					// st.bCurrentSelection has been taken care of at this point already (if it was set) so
					// we don't have to check for that here.
					if (st.bUsers) {
						for (unsigned int uisession : st.qlSessions) {
							t->add_session(uisession);
						}
					} else {
						t->set_channel_id(static_cast< unsigned int >(st.iChannel));
						if (st.bChildren)
							t->set_children(true);
						if (st.bLinks)
							t->set_links(true);
						if (!st.qsGroup.isEmpty())
							t->set_group(u8(st.qsGroup));
					}
				}
				Global::get().sh->sendMessage(mpvt);

				// Store a mapping of the list of ShortcutTargets and the used targetID
				qmTargets.insert(ql, idx);

				// Advance the iteration of qm (which contains the reverse mapping of qmTargetUse) by two.
				// Note that qmTargetUse is first populated in Messages.cpp so we will not overflow the map
				// by this.
				++i;
				++i;

				// Get the target ID for the targetID after next
				int oldidx = i.value();
				if (oldidx) {
					QHash< QList< ShortcutTarget >, int >::iterator mi;
					for (mi = qmTargets.begin(); mi != qmTargets.end(); ++mi) {
						if (mi.value() == oldidx) {
							// If we have used the targetID after next before, we clear the VoiceTarget for that
							// targetID on the server in order to be able to reuse that ID once we need it. We do
							// it 2 steps in advance as to not run into timing problems where the server might
							// receive this clearing message too late for us to recycle the ID.
							qmTargets.erase(mi);

							mpvt.Clear();
							mpvt.set_id(static_cast< unsigned int >(oldidx));
							Global::get().sh->sendMessage(mpvt);

							break;
						}
					}
				}
			}

			// This is where the magic happens. We replace the old value the used targetID was mapped to with
			// iTargetCounter. iTargetCounter is guaranteed to be bigger than any number a targetID is currently
			// mapped to in this map. This causes the mapping for the most recently used targetID to appear last
			// in the qm map the next time this function gets called. This causes targetIDs to be sorted according
			// to the time they have been assigned for the last time so that the targetID that comes last in qm will
			// be the one that has been assigned most recently. This trick turns qmTargetUse (or rather qm) into
			// something similar to a RingBuffer inside this method.
			qmTargetUse.insert(idx, iTargetCounter);
			Global::get().bCenterPosition = center;
			Global::get().iTarget         = idx;
		}
	}
}

void MainWindow::on_gsWhisper_triggered(bool down, QVariant scdata) {
	ShortcutTarget st = scdata.value< ShortcutTarget >();

	if (down) {
		if (gsJoinChannel->active()) {
			if (!st.bUsers) {
				Channel *c = mapChannel(st.iChannel);
				if (c) {
					Global::get().sh->joinChannel(Global::get().uiSession, c->iId);
				}
				return;
			}
		}

		if (gsLinkChannel->active()) {
			if (!st.bUsers) {
				Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
				Channel *l = mapChannel(st.iChannel);
				if (l) {
					if (c->qsPermLinks.contains(l)) {
						Global::get().sh->removeChannelLink(c->iId, l->iId);
					} else {
						Global::get().sh->addChannelLink(c->iId, l->iId);
					}
				}
				return;
			}
		}

		addTarget(&st);
		updateTarget();

		Global::get().iPushToTalk++;
	} else if (Global::get().iPushToTalk > 0) {
		SignalCurry *fwd = new SignalCurry(scdata, true, this);
		connect(fwd, SIGNAL(called(QVariant)), SLOT(whisperReleased(QVariant)));
		QTimer::singleShot(static_cast< int >(Global::get().s.pttHold), fwd, SLOT(call()));
	}
}

/* Add and remove ShortcutTargets from the qmCurrentTargets Map, which counts
 * the number of push-to-talk events for a given ShortcutTarget.  If this number
 * reaches 0, the ShortcutTarget is removed from qmCurrentTargets.
 */
void MainWindow::addTarget(ShortcutTarget *st) {
	if (qmCurrentTargets.contains(*st))
		qmCurrentTargets[*st] += 1;
	else
		qmCurrentTargets[*st] = 1;
}

void MainWindow::removeTarget(ShortcutTarget *st) {
	if (!qmCurrentTargets.contains(*st))
		return;

	if (qmCurrentTargets[*st] == 1)
		qmCurrentTargets.remove(*st);
	else
		qmCurrentTargets[*st] -= 1;
}

void MainWindow::on_gsCycleTransmitMode_triggered(bool down, QVariant) {
	if (down) {
		QString qsNewMode;

		switch (Global::get().s.atTransmit) {
			case Settings::Continuous:
				setTransmissionMode(Settings::VAD);
				break;
			case Settings::VAD:
				setTransmissionMode(Settings::PushToTalk);
				break;
			case Settings::PushToTalk:
				setTransmissionMode(Settings::Continuous);
				break;
		}
	}
}

void MainWindow::on_gsToggleMainWindowVisibility_triggered(bool down, QVariant) {
	if (down) {
		emit windowVisibilityToggled();
	}
}

void MainWindow::on_gsListenChannel_triggered(bool down, QVariant scdata) {
	ChannelTarget target = scdata.value< ChannelTarget >();
	const Channel *c     = Channel::get(target.channelID);

	if (down && c) {
		if (!Global::get().channelListenerManager->isListening(Global::get().uiSession, c->iId)) {
			Global::get().sh->startListeningToChannel(c->iId);
		} else {
			Global::get().sh->stopListeningToChannel(c->iId);
		}
	}
}

void MainWindow::on_gsTransmitModePushToTalk_triggered(bool down, QVariant) {
	if (down) {
		setTransmissionMode(Settings::PushToTalk);
	}
}

void MainWindow::on_gsTransmitModeContinuous_triggered(bool down, QVariant) {
	if (down) {
		setTransmissionMode(Settings::Continuous);
	}
}

void MainWindow::on_gsTransmitModeVAD_triggered(bool down, QVariant) {
	if (down) {
		setTransmissionMode(Settings::VAD);
	}
}

void MainWindow::on_gsSendTextMessage_triggered(bool down, QVariant scdata) {
	if (!down || !Global::get().sh || !Global::get().sh->isRunning() || Global::get().uiSession == 0) {
		return;
	}

	QString qsText = scdata.toString();
	if (qsText.isEmpty()) {
		return;
	}

	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
	Global::get().sh->sendChannelTextMessage(c->iId, qsText, false);
	Global::get().l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatChannel(c), qsText),
						 tr("Message to channel %1").arg(c->qsName), true);
}

void MainWindow::on_gsSendClipboardTextMessage_triggered(bool down, QVariant) {
	if (!down || (QApplication::clipboard()->text().isEmpty())) {
		return;
	}

	// call sendChatbarMessage() instead of on_gsSendTextMessage_triggered() to handle
	// formatting of the content in the clipboard, i.e., href.
	sendChatbarMessage(QApplication::clipboard()->text());
}

void MainWindow::on_gsToggleTalkingUI_triggered(bool down, QVariant) {
	if (down) {
		qaTalkingUIToggle->trigger();
	}
}

void MainWindow::on_gsToggleSearch_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	toggleSearchDialogVisibility();
}

void MainWindow::on_gsServerConnect_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openServerConnectDialog();
}

void MainWindow::on_gsServerDisconnect_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	disconnectFromServer();
}

void MainWindow::on_gsServerInformation_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openServerInformationDialog();
}

void MainWindow::on_gsServerTokens_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openServerTokensDialog();
}

void MainWindow::on_gsServerUserList_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openServerUserListDialog();
}

void MainWindow::on_gsServerBanList_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openServerBanListDialog();
}

void MainWindow::on_gsSelfPrioritySpeaker_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	toggleSelfPrioritySpeaker();
}

void MainWindow::on_gsRecording_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	recording();
}

void MainWindow::on_gsSelfComment_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openSelfCommentDialog();
}

void MainWindow::on_gsServerTexture_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	changeServerTexture();
}

void MainWindow::on_gsServerTextureRemove_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	removeServerTexture();
}

void MainWindow::on_gsSelfRegister_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	selfRegister();
}

void MainWindow::on_gsAudioStats_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openAudioStatsDialog();
}

void MainWindow::on_gsConfigDialog_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openConfigDialog();
}

void MainWindow::on_gsAudioWizard_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openAudioWizardDialog();
}

void MainWindow::on_gsConfigCert_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openCertWizardDialog();
}

void MainWindow::on_gsAudioTTS_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	enableAudioTTS(!Global::get().s.bTTS);
}

void MainWindow::on_gsHelpAbout_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openAboutDialog();
}

void MainWindow::on_gsHelpAboutQt_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	openAboutQtDialog();
}

void MainWindow::on_gsHelpVersionCheck_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	versionCheck();
}

void MainWindow::on_gsTogglePositionalAudio_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	enablePositionalAudio(!Global::get().s.bPositionalAudio);
}

void MainWindow::on_gsMoveBack_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	on_qaMoveBack_triggered();
}

void MainWindow::on_gsCycleListenerAttenuationMode_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	Global::get().s.alwaysAttenuateListeners = !Global::get().s.alwaysAttenuateListeners;
}

void MainWindow::on_gsListenerAttenuationUp_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	Global::get().s.listenerAttenuationFactor = std::min(Global::get().s.listenerAttenuationFactor + 0.1f, 1.0f);
}

void MainWindow::on_gsListenerAttenuationDown_triggered(bool down, QVariant) {
	if (!down) {
		return;
	}

	Global::get().s.listenerAttenuationFactor = std::max(Global::get().s.listenerAttenuationFactor - 0.1f, 0.0f);
}

void MainWindow::on_gsAdaptivePush_triggered(bool down, QVariant variant) {
	if (Global::get().s.atTransmit == Settings::PushToTalk) {
		on_PushToTalk_triggered(down, std::move(variant));
	} else {
		on_PushToMute_triggered(down, std::move(variant));
	}
}

void MainWindow::whisperReleased(QVariant scdata) {
	if (Global::get().iPushToTalk <= 0)
		return;

	ShortcutTarget st = scdata.value< ShortcutTarget >();

	Global::get().iPushToTalk--;

	removeTarget(&st);
	updateTarget();
}

void MainWindow::onResetAudio() {
	qWarning("MainWindow: Start audio reset");
	Audio::stop();
	Audio::start();
	qWarning("MainWindow: Audio reset complete");
}

void MainWindow::viewCertificate(bool) {
	ViewCert vc(Global::get().sh->qscCert, this);
	vc.exec();
}

/**
 * This function prepares the UI for receiving server data. It gets called once the
 * connection to the server is established but before the server Sync is complete.
 */
void MainWindow::serverConnected() {
	m_reconnectSoundBlocker.reset();

	Global::get().uiSession                  = 0;
	Global::get().pPermissions               = ChanACL::None;
	const bool hiddenLegacyNavigatorSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();

#ifdef Q_OS_MAC
	// Suppress AppNap while we're connected to a server.
	MUSuppressAppNap(true);
#endif

	if (!hiddenLegacyNavigatorSafeMode) {
		Global::get().l->clearIgnore();
		Global::get().l->setIgnore(Log::UserJoin);
		Global::get().l->setIgnore(Log::OtherSelfMute);
	}
	QString host, uname, pw;
	unsigned short port;
	Global::get().sh->getConnectionInfo(host, port, uname, pw);
	if (!hiddenLegacyNavigatorSafeMode) {
		Global::get().l->log(Log::ServerConnected, tr("Connected."));
	}
	qaServerDisconnect->setEnabled(true);
	qaServerInformation->setEnabled(true);
	updateFavoriteButton();
	qaServerBanList->setEnabled(true);

	Channel *root = Channel::get(Mumble::ROOT_CHANNEL_ID);
	if (!hiddenLegacyNavigatorSafeMode) {
		pmModel->renameChannel(root, tr("Root"));
		pmModel->setCommentHash(root, QByteArray());
	}
	root->uiPermissions = 0;

	if (!hiddenLegacyNavigatorSafeMode) {
		qtvUsers->setRowHidden(0, QModelIndex(), false);
	}

	Global::get().bAllowHTML                   = true;
	Global::get().bPersistentGlobalChatEnabled = false;
	Global::get().uiMessageLength              = 5000;
	Global::get().uiImageLength                = 131072;
	Global::get().uiMaxUsers                   = 0;
	m_modernLayoutCompatibleServer             = false;
	m_modernShellRuntimeDisabled               = false;
	m_hasPersistentChatSupport                 = false;
	m_defaultPersistentTextChannelID           = 0;
	m_persistentTextChannels.clear();
	m_userIdleSeconds.clear();
	m_pendingUserInformationSessions.clear();
	m_persistentChatPreviews.clear();
	m_persistentChatAssetDownloads.clear();
	syncPersistentChatGatewayHandler();
	if (m_persistentChatController) {
		// serverConnected() also rebuilds the conversation list immediately after resetting the
		// controller, so keep reset() from re-entering the list/view update path on stale items.
		const QSignalBlocker controllerSignalBlocker(m_persistentChatController);
		m_persistentChatController->reset();
	}
	setPersistentChatWelcomeText(QString());
	rebuildPersistentChatChannelList();
	updateServerNavigatorChrome();
	refreshUserPresenceStats();

	enableRecording(true);

	if (Global::get().s.bMute || Global::get().s.bDeaf) {
		Global::get().sh->setSelfMuteDeafState(Global::get().s.bMute, Global::get().s.bDeaf);
	}

	// Update QActions and menus
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

#ifdef Q_OS_WIN
	TaskList::addToRecentList(Global::get().s.qsLastServer, uname, host, port);
#endif

	qdwMinimalViewNote->hide();
	queueModernShellSnapshotSync();
}

void MainWindow::serverDisconnected(QAbstractSocket::SocketError err, QString reason) {
	const bool hiddenLegacyNavigatorSafeMode = modernShellMinimalSnapshotEnabled() && usesModernShell();
	appendModernShellConnectTrace(QStringLiteral("serverDisconnected enter err=%1 minimal=%2 reason=%3")
									  .arg(static_cast< int >(err))
									  .arg(hiddenLegacyNavigatorSafeMode ? 1 : 0)
									  .arg(reason));
	// clear ChannelListener
	Global::get().channelListenerManager->clear();

	// Reset move-back history
	qaMoveBack->setEnabled(false);
	m_previousChannels = {};
	m_movedBackFromChannel.reset();

	Global::get().uiSession                    = 0;
	Global::get().pPermissions                 = ChanACL::None;
	Global::get().bAttenuateOthers             = false;
	Global::get().bPersistentGlobalChatEnabled = false;
	m_modernLayoutCompatibleServer             = false;
	m_modernShellRuntimeDisabled               = false;
	m_hasPersistentChatSupport                 = false;
	qaServerDisconnect->setEnabled(false);
	qaServerAddToFavorites->setEnabled(false);
	qaServerInformation->setEnabled(false);
	qaServerBanList->setEnabled(false);
	if (!hiddenLegacyNavigatorSafeMode) {
		qtvUsers->setCurrentIndex(QModelIndex());
	}
	qteChat->setEnabled(false);
	m_defaultPersistentTextChannelID = 0;
	m_persistentTextChannels.clear();
	m_userIdleSeconds.clear();
	m_pendingUserInformationSessions.clear();
	m_persistentChatPreviews.clear();
	m_persistentChatAssetDownloads.clear();
	m_persistentChatLastReadByScope.clear();
	m_persistentChatUnreadByScope.clear();
	syncPersistentChatGatewayHandler();
	if (m_persistentChatController) {
		// serverDisconnected() rebuilds the conversation list and clears the visible chat surface
		// explicitly below. Suppress reset() signals here so stale room items are not relabeled
		// while the disconnect teardown is still in progress.
		const QSignalBlocker controllerSignalBlocker(m_persistentChatController);
		m_persistentChatController->reset();
	}
	if (m_activeShellLayout != effectiveWindowLayout()) {
		refreshShellLayout();
	}
	setPersistentChatWelcomeText(QString());
	rebuildPersistentChatChannelList();
	clearPersistentChatView(tr("Disconnected from server."), tr("Connection ended"),
							{ tr("Open Server to reconnect"), tr("Room history stays with each channel") });
	updateServerNavigatorChrome();
	queueModernShellSnapshotSync();

#ifdef Q_OS_MAC
	// Remove App Nap suppression now that we're disconnected.
	MUSuppressAppNap(false);
#endif

	QString uname, pw, host;
	unsigned short port;
	Global::get().sh->getConnectionInfo(host, port, uname, pw);

	if (Global::get().sh->hasSynchronized()) {
		QList< Shortcut > &shortcuts = Global::get().s.qlShortcuts;
		// Only save server-specific shortcuts if the client and server have been synchronized before as only then
		// did the client actually load them from the DB. If we store them without having loaded them, we will
		// effectively clear the server-specific shortcuts for this server.
		Global::get().db->setShortcuts(Global::get().sh->qbaDigest, shortcuts);

		// Clear server-specific shortcuts from the list of known shortcuts
		auto it = std::remove_if(shortcuts.begin(), shortcuts.end(),
								 [](const Shortcut &shortcut) { return shortcut.isServerSpecific(); });
		if (it != shortcuts.end()) {
			// Some shortcuts have to be removed
			shortcuts.erase(it, shortcuts.end());

			GlobalShortcutEngine::engine->bNeedRemap = true;
		}
	}

	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = nullptr;
	}

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = nullptr;
	}

	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = nullptr;
	}

	if (tokenEdit) {
		tokenEdit->reject();
		delete tokenEdit;
		tokenEdit = nullptr;
	}

	QSet< QAction * > qs;
	qs += QSet< QAction * >(qlServerActions.begin(), qlServerActions.end());
	qs += QSet< QAction * >(qlChannelActions.begin(), qlChannelActions.end());
	qs += QSet< QAction * >(qlUserActions.begin(), qlUserActions.end());

	for (QAction *a : qs) {
		delete a;
	}

	qlServerActions.clear();
	qlChannelActions.clear();
	qlUserActions.clear();

	if (hiddenLegacyNavigatorSafeMode) {
		clearConnectedStateWithoutUserModel();
		appendModernShellConnectTrace(QStringLiteral("serverDisconnected safe-clear-finished"));
	} else {
		pmModel->removeAll();
		qtvUsers->setRowHidden(0, QModelIndex(), true);
	}

	// Update QActions and menus
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	if (!hiddenLegacyNavigatorSafeMode) {
		qmChannel_aboutToShow();
		qmUser_aboutToShow();
	}
	on_qmConfig_aboutToShow();

	// We can't record without a server anyway, so we disable the functionality here
	enableRecording(false);
	Global::get().bPersistentGlobalChatEnabled = false;

	if (!Global::get().sh->qlErrors.isEmpty()) {
		for (const QSslError &e : Global::get().sh->qlErrors) {
			Global::get().l->log(Log::Warning, tr("SSL Verification failed: %1").arg(e.errorString().toHtmlEscaped()));
		}
		if (!Global::get().sh->qscCert.isEmpty()) {
			QSslCertificate c = Global::get().sh->qscCert.at(0);
			QString basereason;
			QString actual_digest = QString::fromLatin1(c.digest(QCryptographicHash::Sha1).toHex());
			QString digests_section =
				tr("<li>Server certificate digest (SHA-1):\t%1</li>").arg(ViewCert::prettifyDigest(actual_digest));
			QString expected_digest = Global::get().db->getDigest(host, port);
			if (!expected_digest.isNull()) {
				basereason =
					tr("<b>WARNING:</b> The server presented a certificate that was different from the stored one.");
				digests_section.append(tr("<li>Expected certificate digest (SHA-1):\t%1</li>")
										   .arg(ViewCert::prettifyDigest(expected_digest)));
			} else {
				basereason = tr("Server presented a certificate which failed verification.");
			}
			QStringList qsl;
			for (const QSslError &e : Global::get().sh->qlErrors) {
				qsl << QString::fromLatin1("<li>%1</li>").arg(e.errorString().toHtmlEscaped());
			}

			QMessageBox qmb(QMessageBox::Warning, QLatin1String("Mumble"),
							tr("<p>%1</p><ul>%2</ul><p>The specific errors with this certificate are:</p><ol>%3</ol>"
							   "<p>Do you wish to accept this certificate anyway?<br />(It will also be stored so you "
							   "won't be asked this again.)</p>")
								.arg(basereason)
								.arg(digests_section)
								.arg(qsl.join(QString())),
							QMessageBox::Yes | QMessageBox::No, this);

			qmb.setDefaultButton(QMessageBox::No);
			qmb.setEscapeButton(QMessageBox::No);

			QPushButton *qp = qmb.addButton(tr("&View Certificate"), QMessageBox::ActionRole);
			forever {
				int res = qmb.exec();

				if ((res == 0) && (qmb.clickedButton() == qp)) {
					ViewCert vc(Global::get().sh->qscCert, this);
					vc.exec();
					continue;
				} else if (res == QMessageBox::Yes) {
					Global::get().db->setDigest(host, port,
												QString::fromLatin1(c.digest(QCryptographicHash::Sha1).toHex()));
					qaServerDisconnect->setEnabled(true);
					on_Reconnect_timeout();
				}
				break;
			}
		}
	} else if (err == QAbstractSocket::SslHandshakeFailedError) {
		QMessageBox msgBox;
		msgBox.addButton(QMessageBox::Ok);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setTextFormat(Qt::RichText);
		msgBox.setWindowTitle(tr("SSL error"));
		msgBox.setText(tr("Mumble is unable to establish a secure connection to the server. (\"%1\")").arg(reason));
		// clang-format off
		msgBox.setInformativeText(
			tr("This could be caused by one of the following scenarios:"
			   "<ul>"
			       "<li>Your client and the server use different encryption standards. This could be because you are using "
			       "a very old client or the server you are connecting to is very old. In the first case, you should update "
			       "your client and in the second case you should contact the server administrator so that they can update "
				   "their server.</li>"
				   "<li>Either your client or the server is using an old operating system that doesn't provide up-to-date "
				   "encryption methods. In this case you should consider updating your OS or contacting the server admin "
				   "so that they can update theirs.</li>"
				   "<li>The server you are connecting to isn't actually a Mumble server. Please ensure that the used server "
				   "address really belongs to a Mumble server and not e.g. to a game server.</li>"
				   "<li>The port you are connecting to does not belong to a Mumble server but instead is bound to a "
				   "completely unrelated process on the server-side. Please double-check you have used the correct port.</li>"
				"</ul>"));
		// clang-format on

		msgBox.exec();
	} else {
		if (!reason.isEmpty()) {
			Global::get().l->log(Log::ServerDisconnected,
								 tr("Server connection failed: %1.").arg(reason.toHtmlEscaped()));
		} else {
			Global::get().l->log(Log::ServerDisconnected, tr("Disconnected from server."));
		}

		ConnectDetails details;
		Global::get().sh->getConnectionInfo(details.host, details.port, details.username, details.password);

		switch (rtLast) {
			case MumbleProto::Reject_RejectType_InvalidUsername:
				(new FailedConnectionDialog(std::move(details), ConnectionFailType::InvalidUsername, this))->show();
				break;
			case MumbleProto::Reject_RejectType_UsernameInUse:
				(new FailedConnectionDialog(std::move(details), ConnectionFailType::UsernameAlreadyInUse, this))
					->show();
				break;
			case MumbleProto::Reject_RejectType_WrongUserPW:
				(new FailedConnectionDialog(std::move(details), ConnectionFailType::AuthenticationFailure, this))
					->show();
				break;
			case MumbleProto::Reject_RejectType_WrongServerPW:
				(new FailedConnectionDialog(std::move(details), ConnectionFailType::InvalidServerPassword, this))
					->show();
				break;
			default:
				if (Global::get().s.bReconnect && !reason.isEmpty()) {
					qaServerDisconnect->setEnabled(true);
					if (bRetryServer) {
						qtReconnect->start();
					}
				}
				break;
		}
	}
	AudioInput::setMaxBandwidth(-1);

	if (Global::get().s.bMinimalView) {
		qdwMinimalViewNote->show();
	}

	emit disconnectedFromServer();
}

void MainWindow::resolverError(QAbstractSocket::SocketError, QString reason) {
	if (!reason.isEmpty()) {
		Global::get().l->log(Log::ServerDisconnected, tr("Server connection failed: %1.").arg(reason.toHtmlEscaped()));
	} else {
		Global::get().l->log(Log::ServerDisconnected, tr("Server connection failed."));
	}

	if (Global::get().s.bReconnect) {
		qaServerDisconnect->setEnabled(true);
		if (bRetryServer) {
			qtReconnect->start();
		}
	}
}

void MainWindow::showRaiseWindow() {
	endNativeWindowMoveOrResize();
	setWindowState(windowState() & ~Qt::WindowMinimized);
	QTimer::singleShot(0, [this]() {
		show();
		raise();
		activateWindow();
		setWindowState(windowState() | Qt::WindowActive);
	});
}

void MainWindow::highlightWindow() {
	QApplication::alert(this);
}

void MainWindow::on_qaTalkingUIToggle_triggered() {
	if (!Global::get().talkingUI) {
		qCritical("MainWindow: Attempting to show Talking UI before it has been created!");
		return;
	}

	Global::get().talkingUI->setVisible(!Global::get().talkingUI->isVisible());

	Global::get().s.bShowTalkingUI = Global::get().talkingUI->isVisible();
}

/**
 * This function updates the qteChat bar default text according to
 * the selected user/channel in the users treeview.
 */
void MainWindow::qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &) {
	if (modernShellMinimalSnapshotEnabled() && usesModernShell()) {
		setPersistentChatTargetUsesVoiceTree(false);
		queueModernShellSnapshotSync();
		return;
	}

	bool useVoiceTree = false;
	if (hasPersistentChatCapabilities() && pmModel) {
		const QModelIndex currentIndex = qtvUsers ? qtvUsers->currentIndex() : QModelIndex();
		const bool treeOwnsChatTarget =
			m_persistentChatTargetUsesVoiceTree
			|| (qtvUsers && (qtvUsers->hasFocus() || (qtvUsers->viewport() && qtvUsers->viewport()->hasFocus())));
		if (treeOwnsChatTarget && currentIndex.isValid()) {
			useVoiceTree = pmModel->getChannel(currentIndex) != nullptr;
			if (!useVoiceTree && m_persistentChatTargetUsesVoiceTree && pmModel->getUser(currentIndex)) {
				useVoiceTree = true;
			}
		}
	}
	setPersistentChatTargetUsesVoiceTree(useVoiceTree);
	updateChatBar();
	updateServerNavigatorChrome();
}

void MainWindow::on_persistentChatScopeChanged(int) {
	setPersistentChatTargetUsesVoiceTree(false);
	updateServerNavigatorChrome();

	QPointer< MainWindow > guardedThis(this);
	QMetaObject::invokeMethod(
		this,
		[guardedThis]() {
			if (!guardedThis) {
				return;
			}

			guardedThis->updatePersistentTextChannelControls();
			guardedThis->updateChatBar(false);
		},
		Qt::QueuedConnection);
}

void MainWindow::updateChatBar(bool forcePersistentChatReload) {
	appendModernShellConnectTrace(QStringLiteral("updateChatBar enter force=%1 session=%2")
									  .arg(forcePersistentChatReload ? 1 : 0)
									  .arg(Global::get().uiSession));
	if (modernShellMinimalSnapshotEnabled() && usesModernShell()) {
		queueModernShellSnapshotSync();
		appendModernShellConnectTrace(QStringLiteral("updateChatBar minimal-return"));
		return;
	}

	const PersistentChatTarget target = currentPersistentChatTarget();
	updatePersistentChatChrome(target);
	updatePersistentChatScopeSelectorLabels();
	updatePersistentTextChannelControls();
	if (Global::get().uiSession == 0 || !target.valid) {
		qteChat->setDefaultText(tr("<div>Connect to chat</div>"), true);
		clearPersistentChatView(tr("Connect to a server to load conversations and history."),
								tr("Start a conversation"),
								{ tr("Open Server to connect"), tr("Room chat and history appear here") });
	} else if (target.serverLog) {
		clearPersistentChatReplyTarget(false);
		qteChat->setDefaultText(tr("<div>Read-only activity</div>"), true);
		renderServerLogView(true);
	} else if (target.legacyTextPath && target.directMessage && target.user) {
		qteChat->setDefaultText(tr("<div>Write to %1...</div>").arg(target.user->qsName.toHtmlEscaped()));
		refreshPersistentChatView(forcePersistentChatReload);
	} else if (target.legacyTextPath && target.channel) {
		qteChat->setDefaultText(tr("<div>Write in %1...</div>").arg(target.channel->qsName.toHtmlEscaped()));
		refreshPersistentChatView(forcePersistentChatReload);
	} else if (target.directMessage && target.user) {
		qteChat->setDefaultText(tr("<div>Write to %1...</div>").arg(target.user->qsName.toHtmlEscaped()));
		clearPersistentChatView(tr("Direct messages still use the classic text message path and do not have persistent "
								   "history yet."),
								tr("Direct messages"), { tr("Classic chat still handles direct messages") });
	} else if (target.scope == MumbleProto::ServerGlobal && !Global::get().bPersistentGlobalChatEnabled) {
		qteChat->setDefaultText(tr("<div>Server-wide chat is unavailable</div>"), true);
		clearPersistentChatView(
			target.statusMessage.isEmpty() ? tr("Legacy server-wide chat is disabled by this server.")
										   : target.statusMessage,
			tr("Server-wide chat is unavailable"), { tr("Choose a text room or voice room chat instead") });
	} else if (target.readOnly) {
		qteChat->setDefaultText(
			tr("<div>Choose a conversation to write in</div><div><small>This conversation is read-only</small></div>"),
			true);
		refreshPersistentChatView(forcePersistentChatReload);
	} else if (target.scope == MumbleProto::TextChannel) {
		qteChat->setDefaultText(tr("<div>Write in %1...</div>").arg(target.label.toHtmlEscaped()), true);
		refreshPersistentChatView(forcePersistentChatReload);
	} else if (target.channel) {
		qteChat->setDefaultText(tr("<div>Write in %1...</div>").arg(target.channel->qsName.toHtmlEscaped()));
		refreshPersistentChatView(forcePersistentChatReload);
	} else {
		qteChat->setDefaultText(tr("<div>Write...</div>"), true);
		refreshPersistentChatView(forcePersistentChatReload);
	}

	updateMenuPermissions();
	queueModernShellSnapshotSync();
}

void MainWindow::customEvent(QEvent *evt) {
	if (evt->type() == MB_QEVENT) {
		MessageBoxEvent *mbe = static_cast< MessageBoxEvent * >(evt);
		Global::get().l->log(Log::Information, mbe->msg);
		return;
	} else if (evt->type() == OU_QEVENT) {
		OpenURLEvent *oue = static_cast< OpenURLEvent * >(evt);
		openUrl(oue->url);
		return;
	} else if (evt->type() != SERVERSEND_EVENT) {
		return;
	}

	ServerHandlerMessageEvent *shme = static_cast< ServerHandlerMessageEvent * >(evt);
	const bool traceServerMessages  = qEnvironmentVariableIntValue("MUMBLE_CONNECT_TRACE") != 0;
	auto appendServerMessageTrace   = [&](const char *phase, const char *messageName) {
        if (!traceServerMessages) {
            return;
        }

        QFile traceFile(Global::get().qdBasePath.filePath(QLatin1String("shared-modern-connect-trace.log")));
        if (!traceFile.open(QIODevice::Append | QIODevice::Text)) {
            return;
        }

        const QByteArray line = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs).toUtf8() + ' '
                                + QByteArray(phase) + ' ' + QByteArray(messageName)
                                + " size=" + QByteArray::number(shme->qbaMsg.size()) + '\n';
        traceFile.write(line);
        traceFile.flush();
	};

#ifdef QT_NO_DEBUG
#	define PROCESS_MUMBLE_TCP_MESSAGE(name, value)                                                      \
		case Mumble::Protocol::TCPMessageType::name: {                                                   \
			appendServerMessageTrace("begin", #name);                                                    \
			MumbleProto::name msg;                                                                       \
			if (msg.ParseFromArray(shme->qbaMsg.constData(), static_cast< int >(shme->qbaMsg.size()))) { \
				msg##name(msg);                                                                          \
				appendServerMessageTrace("end", #name);                                                  \
			} else {                                                                                     \
				appendServerMessageTrace("parse-fail", #name);                                           \
			}                                                                                            \
			break;                                                                                       \
		}
#else
#	define PROCESS_MUMBLE_TCP_MESSAGE(name, value)                                                      \
		case Mumble::Protocol::TCPMessageType::name: {                                                   \
			appendServerMessageTrace("begin", #name);                                                    \
			MumbleProto::name msg;                                                                       \
			if (msg.ParseFromArray(shme->qbaMsg.constData(), static_cast< int >(shme->qbaMsg.size()))) { \
				printf("%s:\n", #name);                                                                  \
				msg.PrintDebugString();                                                                  \
				msg##name(msg);                                                                          \
				appendServerMessageTrace("end", #name);                                                  \
			} else {                                                                                     \
				appendServerMessageTrace("parse-fail", #name);                                           \
			}                                                                                            \
			break;                                                                                       \
		}
#endif
	switch (shme->type) { MUMBLE_ALL_TCP_MESSAGES }


#undef PROCESS_MUMBLE_TCP_MESSAGE
}


void MainWindow::on_qteLog_anchorClicked(const QUrl &url) {
	if (url.scheme() == QLatin1String("mumble-chat") && url.host() == QLatin1String("load-older")) {
		requestOlderPersistentChatHistory();
		return;
	}

	const QString encodedUrl = QString::fromLatin1(url.toEncoded());
	if (encodedUrl.startsWith(QLatin1String("data:image/"), Qt::CaseInsensitive)) {
		m_selectedLogImage = persistentChatInlineDataImageFromSource(encodedUrl);
		if (!m_selectedLogImage.isNull()) {
			openImageDialog(m_selectedLogImage);
			return;
		}
	}

	if (url.scheme() == QLatin1String("mumble-chat") && url.host() == QLatin1String("inline-data-image")) {
		m_selectedLogImage = persistentChatInlineDataImageFromUrl(url);
		if (!m_selectedLogImage.isNull()) {
			openImageDialog(m_selectedLogImage);
			return;
		}
	}

	if (url.scheme() == QLatin1String("mumble-chat") && url.host() == QLatin1String("preview-image")) {
		const QString previewKey = QUrl::fromPercentEncoding(url.path().mid(1).toUtf8());
		const auto previewIt     = m_persistentChatPreviews.constFind(previewKey);
		if (previewIt != m_persistentChatPreviews.cend() && !previewIt->thumbnailImage.isNull()) {
			openImageDialog(previewIt->thumbnailImage);
			return;
		}
	}

	if (url.scheme() == QLatin1String("mumble-chat") && url.host() == QLatin1String("scope")) {
		const QStringList pathParts = url.path().split(QLatin1Char('/'), Qt::SkipEmptyParts);
		if (pathParts.size() == 2 && pathParts.front() == QLatin1String("text")) {
			bool ok                    = false;
			const unsigned int scopeID = pathParts.back().toUInt(&ok);
			if (ok && navigateToPersistentChatScope(MumbleProto::TextChannel, scopeID)) {
				return;
			}
		}
	}

	if (!handleSpecialContextMenu(url, QCursor::pos(), true)) {
#if defined(Q_OS_MAC) && defined(USE_OVERLAY)
		// Clicking a link can cause the user's default browser to pop up while
		// we're intercepting all events. This can be very confusing (because
		// the user can't click on anything before they dismiss the overlay
		// by hitting their toggle hotkey), so let's disallow clicking links
		// when embedded into the overlay for now.
		if (Global::get().ocIntercept)
			return;
#endif
		if (url.scheme() != QLatin1String("file") && url.scheme() != QLatin1String("qrc") && !url.isRelative()) {
			const QUrl externalUrl = url;
			// Let the current click/paint work finish before we hand the URL to the OS.
			QTimer::singleShot(0, this, [externalUrl]() { QDesktopServices::openUrl(externalUrl); });
		}
	}
}

void MainWindow::on_qteLog_highlighted(const QUrl &url) {
	if (url.scheme() == QString::fromLatin1("clientid") || url.scheme() == QString::fromLatin1("channelid")
		|| url.scheme() == QString::fromLatin1("mumble-chat") || url.scheme() == QString::fromLatin1("data"))
		return;

	if (!url.isValid())
		QToolTip::hideText();
	else {
		if (isActiveWindow()) {
			LogTextBrowser *browser = qobject_cast< LogTextBrowser * >(sender());
			QToolTip::showText(QCursor::pos(), url.toString(), browser ? browser : qteLog, QRect());
		}
	}
}

void MainWindow::context_triggered() {
	QAction *a = qobject_cast< QAction * >(sender());

	Channel *c    = pmModel->getChannel(qtvUsers->currentIndex());
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	triggerContextAction(a ? a->data().toString() : QString(), p, c);
}

/**
 * Presents a file open dialog, opens the selected picture and returns it.
 * @return Pair consisting of the raw file contents and the image. Uninitialized on error or cancel.
 */
QPair< QByteArray, QImage > MainWindow::openImageFile() {
	QPair< QByteArray, QImage > retval;

	QString fname =
		QFileDialog::getOpenFileName(this, tr("Choose image file"), getImagePath(), tr("Images (*.png *.jpg *.jpeg)"));

	if (fname.isNull())
		return retval;

	QFile f(fname);
	if (!f.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Could not open file for reading."));
		return retval;
	}

	updateImagePath(fname);

	QByteArray qba = f.readAll();
	f.close();

	QBuffer qb(&qba);
	qb.open(QIODevice::ReadOnly);

	QImageReader qir;
	qir.setAutoDetectImageFormat(false);

	QByteArray fmt;
	if (!RichTextImage::isValidImage(qba, fmt)) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Image format not recognized."));
		return retval;
	}

	qir.setFormat(fmt);
	qir.setDevice(&qb);

	QImage img = qir.read();
	if (img.isNull()) {
		QMessageBox::warning(this, tr("Failed to load image"), tr("Image format not recognized."));
		return retval;
	}

	retval.first  = qba;
	retval.second = img;

	return retval;
}

void MainWindow::logChangeNotPermanent(const QString &actionName, ClientUser *const p) const {
	Global::get().l->log(
		Log::Warning,
		QObject::tr(
			"\"%1\" could not be saved permanently and is lost on restart because %2 does not have a certificate.")
			.arg(actionName)
			.arg(Log::formatClientUser(p, Log::Target)));
}

void MainWindow::destroyUserInformation() {
	UserInformation *ui = static_cast< UserInformation * >(sender());
	QMap< unsigned int, UserInformation * >::iterator i;
	for (i = qmUserInformations.begin(); i != qmUserInformations.end(); ++i) {
		if (i.value() == ui) {
			qmUserInformations.erase(i);
			return;
		}
	}
}

void MainWindow::openServerConnectDialog(bool autoconnect) {
	// Wait for this window to be mapped before opening the dialog, otherwise
	// Wayland compositors may not recognize the parent-child relationship.
	if (!windowHandle() || !windowHandle()->isExposed()) {
		// Ensure windowHandle() is non-null by forcing native window creation
		setAttribute(Qt::WA_NativeWindow);
		windowHandle()->installEventFilter(
			new ExposeEventFilter(this, [this, autoconnect]() { openServerConnectDialog(autoconnect); }));
		return;
	}

	ConnectDialog *cd = new ConnectDialog(this, autoconnect);
	int res           = cd->exec();

	if (cd->qsServer.isEmpty() || (cd->usPort == 0) || cd->qsUsername.isEmpty())
		res = QDialog::Rejected;

	if (res == QDialog::Accepted) {
		recreateServerHandler();
		qsDesiredChannel = QString();
		rtLast           = MumbleProto::Reject_RejectType_None;
		bRetryServer     = true;
		qaServerDisconnect->setEnabled(true);
		Global::get().l->log(
			Log::Information,
			tr("Connecting to server %1.").arg(Log::msgColor(cd->qsServer.toHtmlEscaped(), Log::Server)));
		Global::get().sh->setConnectionInfo(cd->qsServer, cd->usPort, cd->qsUsername, cd->qsPassword);
		Global::get().sh->start(QThread::TimeCriticalPriority);
	}
	delete cd;

	// update because the user might have changed his favorites
	updateFavoriteButton();
}

void MainWindow::disconnectFromServer() {
	if (qtReconnect->isActive()) {
		qtReconnect->stop();
		qaServerDisconnect->setEnabled(false);
	}

	m_reconnectSoundBlocker.reset();

	if (Global::get().sh && Global::get().sh->isRunning()) {
		Global::get().sh->disconnect();
	}
}

void MainWindow::addServerAsFavorite() {
	if (Global::get().uiSession == 0) {
		return;
	}
	QString host, username, password;
	unsigned short port;
	Global::get().sh->getConnectionInfo(host, port, username, password);
	ServerItem currentServer = ServerItem(host, host, port, username, password);
	Global::get().db->addFavorite(currentServer.toFavoriteServer());
	qaServerAddToFavorites->setEnabled(false);
	Global::get().l->log(Log::Information,
						 tr("Added %1 to favorites.").arg(Log::msgColor(host.toHtmlEscaped(), Log::Server)));
}

void MainWindow::openServerInformationDialog() {
	ServerInformation *infoDialog = new ServerInformation(this);
	infoDialog->show();
}

void MainWindow::openServerTokensDialog() {
	if (tokenEdit) {
		tokenEdit->reject();
		delete tokenEdit;
		tokenEdit = nullptr;
	}

	tokenEdit = new Tokens(this);
	tokenEdit->show();
}

void MainWindow::openServerUserListDialog() {
	Global::get().sh->requestUserList();

	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = nullptr;
	}
}

void MainWindow::openServerBanListDialog() {
	Global::get().sh->requestBanList();

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = nullptr;
	}
}

void MainWindow::toggleSelfPrioritySpeaker() {
	ClientUser *p = ClientUser::get(Global::get().uiSession);
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_priority_speaker(!p->bPrioritySpeaker);
	Global::get().sh->sendMessage(mpus);
}

void MainWindow::recording() {
	if (voiceRecorderDialog) {
		voiceRecorderDialog->show();
		voiceRecorderDialog->raise();
		voiceRecorderDialog->activateWindow();
	} else {
		voiceRecorderDialog = new VoiceRecorderDialog(this);
		connect(voiceRecorderDialog, SIGNAL(finished(int)), this, SLOT(voiceRecorderDialog_finished(int)));
		QObject::connect(Global::get().sh.get(), &ServerHandler::disconnected, voiceRecorderDialog, &QDialog::reject);
		voiceRecorderDialog->show();
	}
}

void MainWindow::openSelfCommentDialog() {
	ClientUser *p = ClientUser::get(Global::get().uiSession);
	if (!p)
		return;

	if (!p->qbaCommentHash.isEmpty() && p->qsComment.isEmpty()) {
		p->qsComment = QString::fromUtf8(Global::get().db->blob(p->qbaCommentHash));
		if (p->qsComment.isEmpty()) {
			pmModel->uiSessionComment = ~(p->uiSession);
			MumbleProto::RequestBlob mprb;
			mprb.add_session_comment(p->uiSession);
			Global::get().sh->sendMessage(mprb);
			return;
		}
	}

	unsigned int session = p->uiSession;

	::TextMessage *texm = new ::TextMessage(this, tr("Change your comment"));

	texm->rteMessage->setText(p->qsComment);
	int res = texm->exec();

	p = ClientUser::get(session);

	if (p && (res == QDialog::Accepted)) {
		const QString &msg = texm->message();
		MumbleProto::UserState mpus;
		mpus.set_session(session);
		mpus.set_comment(u8(msg));
		Global::get().sh->sendMessage(mpus);

		if (!msg.isEmpty())
			Global::get().db->setBlob(sha1(msg), msg.toUtf8());
	}
	delete texm;
}

void MainWindow::changeServerTexture() {
	QPair< QByteArray, QImage > choice = openImageFile();
	if (choice.first.isEmpty())
		return;

	const QImage &img = choice.second;

	if ((img.height() <= 1024) && (img.width() <= 1024))
		Global::get().sh->setUserTexture(Global::get().uiSession, choice.first);
}

void MainWindow::removeServerTexture() {
	Global::get().sh->setUserTexture(Global::get().uiSession, QByteArray());
}

void MainWindow::selfRegister() {
	ClientUser *p = ClientUser::get(Global::get().uiSession);
	if (!p)
		return;
	if (p->iId >= 0) {
		Global::get().l->log(Log::PermissionDenied, tr("You are already registered on this server."));
		return;
	}
	if (p->qsHash.isEmpty()) {
		Global::get().l->log(Log::PermissionDenied, tr("You need a certificate to perform this operation."));
		return;
	}
	if (!(Global::get().pPermissions & (ChanACL::SelfRegister | ChanACL::Write))) {
		Global::get().l->log(Log::PermissionDenied,
							 tr("The server is not allowing self-registration for this account."));
		return;
	}

	QMessageBox::StandardButton result;
	result =
		QMessageBox::question(this, tr("Register yourself as %1").arg(p->qsName),
							  tr("<p>You are about to register yourself on this server. This action cannot be undone, "
								 "and your username cannot be changed once this is done. You will forever be known as "
								 "'%1' on this server.</p><p>Are you sure you want to register yourself?</p>")
								  .arg(p->qsName.toHtmlEscaped()),
							  QMessageBox::Yes | QMessageBox::No);

	if (result == QMessageBox::Yes)
		Global::get().sh->registerUser(p->uiSession);
}

void MainWindow::openAudioStatsDialog() {
	AudioStats *as = new AudioStats(this);
	as->show();
}

void MainWindow::openConfigDialog() {
	openConfigDialogPage(QString());
}

void MainWindow::openConfigDialogPage(const QString &pageName) {
	ConfigDialog *dlg = new ConfigDialog(this);

	if (!pageName.trimmed().isEmpty()) {
		dlg->selectPage(pageName);
	}

	Global::get().inConfigUI = true;

	QObject::connect(dlg, &ConfigDialog::settingsAccepted, Global::get().talkingUI, &TalkingUI::on_settingsChanged);
	QObject::connect(dlg, &ConfigDialog::settingsAccepted, []() {
		if (Global::get().s.requireThemeApplication) {
			Themes::apply();
			if (Global::get().mw) {
				Global::get().mw->refreshCustomChromeStyles();
			}
		}
	});

	if (dlg->exec() == QDialog::Accepted) {
		setupView(false);
		showRaiseWindow();
		updateTransmitModeComboBox(Global::get().s.atTransmit);
		updateUserModel();
		emit talkingStatusChanged();
		queueModernShellSnapshotSync();

		if (Global::get().s.requireRestartToApply) {
			if (Global::get().s.requireRestartToApply
				&& QMessageBox::question(
					   this, tr("Restart Mumble?"),
					   tr("Some settings will only apply after a restart of Mumble. Restart Mumble now?"),
					   QMessageBox::Yes | QMessageBox::No)
					   == QMessageBox::Yes) {
				forceQuit     = true;
				restartOnQuit = true;

				close();
			}
		}
	}

	Global::get().inConfigUI = false;

	delete dlg;
}

void MainWindow::openAudioWizardDialog() {
	AudioWizard *aw = new AudioWizard(this);
	aw->exec();
	delete aw;
}

void MainWindow::openCertWizardDialog() {
	CertWizard *cw = new CertWizard(this);
	cw->exec();
	delete cw;
}

void MainWindow::enableAudioTTS(bool enable) {
	Global::get().s.bTTS = enable;
}

void MainWindow::openAboutDialog() {
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::openAboutQtDialog() {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::versionCheck() {
	new VersionCheck(false, this);
}

void MainWindow::enablePositionalAudio(bool enable) {
	Global::get().s.bPositionalAudio = enable;
}

void MainWindow::on_muteCuePopup_triggered() {
	showMuteCuePopup();
}

void MainWindow::showMuteCuePopup() {
	if (Global::get().s.muteCueShown || Global::get().inConfigUI) {
		return;
	}

	Global::get().s.muteCueShown = true;
	QMessageBox mb(
		QMessageBox::Warning, QLatin1String("Mumble"),
		tr("That sound was the mute cue. It activates when you speak while muted. Would you like to keep it enabled?"),
		QMessageBox::NoButton, this);
	QPushButton *accept = mb.addButton(tr("Yes"), QMessageBox::YesRole);
	QPushButton *reject = mb.addButton(tr("No"), QMessageBox::NoRole);
	mb.setDefaultButton(accept);
	mb.setEscapeButton(accept);
	mb.exec();

	if (mb.clickedButton() == reject) {
		Global::get().s.bTxMuteCue = false;
	}
}

void MainWindow::showImageDialog() {
	openImageDialog(m_selectedLogImage);
}
