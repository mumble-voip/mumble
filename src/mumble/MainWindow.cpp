// Copyright 2007-2022 The Mumble Developers. All rights reserved.
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
#include "CELTCodec.h"
#include "Cert.h"
#include "Channel.h"
#include "ConnectDialog.h"
#include "Connection.h"
#include "Database.h"
#include "DeveloperConsole.h"
#include "Log.h"
#include "Net.h"
#include "NetworkConfig.h"
#include "OpusCodec.h"
#include "GlobalShortcut.h"
#ifdef USE_OVERLAY
#	include "OverlayClient.h"
#endif
#include "../SignalCurry.h"
#include "ChannelListenerManager.h"
#include "ListenerLocalVolumeSlider.h"
#include "Markdown.h"
#include "MenuLabel.h"
#include "PTTButtonWidget.h"
#include "PluginManager.h"
#include "PositionalAudioViewer.h"
#include "QtWidgetUtils.h"
#include "RichTextEditor.h"
#include "Screen.h"
#include "SearchDialog.h"
#include "ServerHandler.h"
#include "ServerInformation.h"
#include "Settings.h"
#include "SvgIcon.h"
#include "TalkingUI.h"
#include "TextMessage.h"
#include "Themes.h"
#include "Tokens.h"
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
#include "Global.h"

#ifdef Q_OS_WIN
#	include "TaskList.h"
#endif

#ifdef Q_OS_MAC
#	include "AppNap.h"
#endif

#include <QtCore/QStandardPaths>
#include <QtCore/QUrlQuery>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>
#include <QtGui/QImageReader>
#include <QtGui/QScreen>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QWhatsThis>

#ifdef Q_OS_WIN
#	include <dbt.h>
#endif

#include <algorithm>

MessageBoxEvent::MessageBoxEvent(QString m) : QEvent(static_cast< QEvent::Type >(MB_QEVENT)) {
	msg = m;
}

OpenURLEvent::OpenURLEvent(QUrl u) : QEvent(static_cast< QEvent::Type >(OU_QEVENT)) {
	url = u;
}

MainWindow::MainWindow(QWidget *p)
	: QMainWindow(p), m_localVolumeLabel(make_qt_unique< MenuLabel >(tr("Local Volume Adjustment:"), this)),
	  m_userLocalVolumeSlider(make_qt_unique< UserLocalVolumeSlider >(this)),
	  m_listenerLocalVolumeSlider(make_qt_unique< ListenerLocalVolumeSlider >(this)) {
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
	bSuppressAskOnQuit = false;
	restartOnQuit      = false;
	bAutoUnmute        = false;

	Channel::add(Channel::ROOT_ID, tr("Root"));

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
	setupGui();
	qtvUsers->setAccessibleName(tr("Channels and users"));
	qteLog->setAccessibleName(tr("Activity log"));
	qteChat->setAccessibleName(tr("Chat message"));
	connect(qmUser, SIGNAL(aboutToShow()), this, SLOT(qmUser_aboutToShow()));
	connect(qmChannel, SIGNAL(aboutToShow()), this, SLOT(qmChannel_aboutToShow()));
	connect(qmListener, SIGNAL(aboutToShow()), this, SLOT(qmListener_aboutToShow()));
	connect(qteChat, SIGNAL(entered(QString)), this, SLOT(sendChatbarText(QString)));
	connect(qteChat, &ChatbarTextEdit::ctrlEnterPressed, [this](const QString &msg) { sendChatbarText(msg, true); });
	connect(qteChat, SIGNAL(pastedImage(QString)), this, SLOT(sendChatbarMessage(QString)));

	// Tray
	connect(qstiIcon, SIGNAL(messageClicked()), this, SLOT(showRaiseWindow()));
	connect(qaShow, SIGNAL(triggered()), this, SLOT(showRaiseWindow()));

	QObject::connect(this, &MainWindow::transmissionModeChanged, this, &MainWindow::updateTransmitModeComboBox);

	// Explicitly add actions to mainwindow so their shortcuts are available
	// if only the main window is visible (e.Global::get(). minimal mode)
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

	QObject::connect(this, &MainWindow::serverSynchronized, Global::get().pluginManager,
					 &PluginManager::on_serverSynchronized);
}

void MainWindow::createActions() {
	int idx    = 1;
	gsPushTalk = new GlobalShortcut(this, idx++, tr("Push-to-Talk", "Global Shortcut"));
	gsPushTalk->setObjectName(QLatin1String("PushToTalk"));
	gsPushTalk->qsToolTip   = tr("Push and hold this button to send voice.", "Global Shortcut");
	gsPushTalk->qsWhatsThis = tr(
		"This configures the push-to-talk button, and as long as you hold this button down, you will transmit voice.",
		"Global Shortcut");


	gsResetAudio = new GlobalShortcut(this, idx++, tr("Reset Audio Processor", "Global Shortcut"));
	gsResetAudio->setObjectName(QLatin1String("ResetAudio"));

	gsMuteSelf = new GlobalShortcut(this, idx++, tr("Mute Self", "Global Shortcut"), 0);
	gsMuteSelf->setObjectName(QLatin1String("gsMuteSelf"));
	gsMuteSelf->qsToolTip = tr("Set self-mute status.", "Global Shortcut");
	gsMuteSelf->qsWhatsThis =
		tr("This will set or toggle your muted status. If you turn this off, you will also disable self-deafen.",
		   "Global Shortcut");

	gsDeafSelf = new GlobalShortcut(this, idx++, tr("Deafen Self", "Global Shortcut"), 0);
	gsDeafSelf->setObjectName(QLatin1String("gsDeafSelf"));
	gsDeafSelf->qsToolTip = tr("Set self-deafen status.", "Global Shortcut");
	gsDeafSelf->qsWhatsThis =
		tr("This will set or toggle your deafened status. If you turn this on, you will also enable self-mute.",
		   "Global Shortcut");

	gsUnlink = new GlobalShortcut(this, idx++, tr("Unlink Plugin", "Global Shortcut"));
	gsUnlink->setObjectName(QLatin1String("UnlinkPlugin"));

	gsPushMute = new GlobalShortcut(this, idx++, tr("Push-to-Mute", "Global Shortcut"));
	gsPushMute->setObjectName(QLatin1String("PushToMute"));

	gsJoinChannel = new GlobalShortcut(this, idx++, tr("Join Channel", "Global Shortcut"));
	gsJoinChannel->setObjectName(QLatin1String("MetaChannel"));
	gsJoinChannel->qsToolTip = tr("Use in conjunction with Whisper to.", "Global Shortcut");

#ifdef USE_OVERLAY
	gsToggleOverlay = new GlobalShortcut(this, idx++, tr("Toggle Overlay", "Global Shortcut"));
	gsToggleOverlay->setObjectName(QLatin1String("ToggleOverlay"));
	gsToggleOverlay->qsToolTip   = tr("Toggle state of in-game overlay.", "Global Shortcut");
	gsToggleOverlay->qsWhatsThis = tr("This will switch the states of the in-game overlay.", "Global Shortcut");

	connect(gsToggleOverlay, SIGNAL(down(QVariant)), Global::get().o, SLOT(toggleShow()));
#endif

	gsMinimal = new GlobalShortcut(this, idx++, tr("Toggle Minimal", "Global Shortcut"));
	gsMinimal->setObjectName(QLatin1String("ToggleMinimal"));

	gsVolumeUp = new GlobalShortcut(this, idx++, tr("Volume Up (+10%)", "Global Shortcut"));
	gsVolumeUp->setObjectName(QLatin1String("VolumeUp"));

	gsVolumeDown = new GlobalShortcut(this, idx++, tr("Volume Down (-10%)", "Global Shortcut"));
	gsVolumeDown->setObjectName(QLatin1String("VolumeDown"));

	qstiIcon = new QSystemTrayIcon(qiIcon, this);
	qstiIcon->setToolTip(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	qstiIcon->setObjectName(QLatin1String("Icon"));

	gsWhisper = new GlobalShortcut(this, idx++, tr("Whisper/Shout"), QVariant::fromValue(ShortcutTarget()));
	gsWhisper->setObjectName(QLatin1String("gsWhisper"));

	gsLinkChannel = new GlobalShortcut(this, idx++, tr("Link Channel", "Global Shortcut"));
	gsLinkChannel->setObjectName(QLatin1String("MetaLink"));
	gsLinkChannel->qsToolTip = tr("Use in conjunction with Whisper to.", "Global Shortcut");

	gsCycleTransmitMode = new GlobalShortcut(this, idx++, tr("Cycle Transmit Mode", "Global Shortcut"));
	gsCycleTransmitMode->setObjectName(QLatin1String("gsCycleTransmitMode"));

	gsToggleMainWindowVisibility = new GlobalShortcut(this, idx++, tr("Hide/show main window", "Global Shortcut"));
	gsToggleMainWindowVisibility->setObjectName(QLatin1String("gsToggleMainWindowVisibility"));

	gsTransmitModePushToTalk =
		new GlobalShortcut(this, idx++, tr("Set Transmit Mode to Push-To-Talk", "Global Shortcut"));
	gsTransmitModePushToTalk->setObjectName(QLatin1String("gsTransmitModePushToTalk"));

	gsTransmitModeContinuous =
		new GlobalShortcut(this, idx++, tr("Set Transmit Mode to Continuous", "Global Shortcut"));
	gsTransmitModeContinuous->setObjectName(QLatin1String("gsTransmitModeContinuous"));

	gsTransmitModeVAD = new GlobalShortcut(this, idx++, tr("Set Transmit Mode to VAD", "Global Shortcut"));
	gsTransmitModeVAD->setObjectName(QLatin1String("gsTransmitModeVAD"));

	gsSendTextMessage =
		new GlobalShortcut(this, idx++, tr("Send Text Message", "Global Shortcut"), QVariant(QString()));
	gsSendTextMessage->setObjectName(QLatin1String("gsSendTextMessage"));

	gsSendClipboardTextMessage = new GlobalShortcut(this, idx++, tr("Send Clipboard Text Message", "Global Shortcut"));
	gsSendClipboardTextMessage->setObjectName(QLatin1String("gsSendClipboardTextMessage"));
	gsSendClipboardTextMessage->qsWhatsThis =
		tr("This will send your Clipboard content to the channel you are currently in.", "Global Shortcut");

	gsToggleTalkingUI = new GlobalShortcut(this, idx++, tr("Toggle TalkingUI", "Global shortcut"));
	gsToggleTalkingUI->setObjectName(QLatin1String("gsToggleTalkingUI"));
	gsToggleTalkingUI->qsWhatsThis = tr("Toggles the visibility of the TalkingUI.", "Global Shortcut");

	gsToggleSearch = new GlobalShortcut(this, idx++, tr("Toggle search dialog", "Global Shortcut"));
	gsToggleSearch->setObjectName(QLatin1String("gsToggleSearch"));
	gsToggleSearch->qsWhatsThis =
		tr("This will open or close the search dialog depending on whether it is currently opened already");

#ifndef Q_OS_MAC
	qstiIcon->show();
#endif
}

void MainWindow::setupGui() {
	updateWindowTitle();
	setCentralWidget(qtvUsers);
	setAcceptDrops(true);

#ifdef Q_OS_MAC
	QMenu *qmWindow = new QMenu(tr("&Window"), this);
	menubar->insertMenu(qmHelp->menuAction(), qmWindow);
	qmWindow->addAction(tr("Minimize"), this, SLOT(showMinimized()), QKeySequence(tr("Ctrl+M")));

	qtvUsers->setAttribute(Qt::WA_MacShowFocusRect, false);
	qteChat->setAttribute(Qt::WA_MacShowFocusRect, false);
	qteChat->setFrameShape(QFrame::NoFrame);
	qteLog->setFrameStyle(QFrame::NoFrame);
#endif

	LogDocument *ld = new LogDocument(qteLog);
	qteLog->setDocument(ld);

	qteLog->document()->setMaximumBlockCount(Global::get().s.iMaxLogBlocks);
	qteLog->document()->setDefaultStyleSheet(qApp->styleSheet());

	pmModel = new UserModel(qtvUsers);
	qtvUsers->setModel(pmModel);
	qtvUsers->setRowHidden(0, QModelIndex(), true);
	qtvUsers->ensurePolished();

	QObject::connect(this, &MainWindow::userAddedChannelListener, pmModel, &UserModel::addChannelListener);
	QObject::connect(
		this, &MainWindow::userRemovedChannelListener, pmModel,
		static_cast< void (UserModel::*)(const ClientUser *, const Channel *) >(&UserModel::removeChannelListener));
	QObject::connect(Global::get().channelListenerManager.get(), &ChannelListenerManager::localVolumeAdjustmentsChanged,
					 pmModel, &UserModel::on_channelListenerLocalVolumeAdjustmentChanged);

	// connect slots to PluginManager
	QObject::connect(pmModel, &UserModel::userAdded, Global::get().pluginManager, &PluginManager::on_userAdded);
	QObject::connect(pmModel, &UserModel::userRemoved, Global::get().pluginManager, &PluginManager::on_userRemoved);
	QObject::connect(pmModel, &UserModel::channelAdded, Global::get().pluginManager, &PluginManager::on_channelAdded);
	QObject::connect(pmModel, &UserModel::channelRemoved, Global::get().pluginManager,
					 &PluginManager::on_channelRemoved);
	QObject::connect(pmModel, &UserModel::channelRenamed, Global::get().pluginManager,
					 &PluginManager::on_channelRenamed);

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

	qaHelpWhatsThis->setShortcuts(QKeySequence::WhatsThis);

	qaConfigMinimal->setChecked(Global::get().s.bMinimalView);
	qaConfigHideFrame->setChecked(Global::get().s.bHideFrame);

	connect(gsResetAudio, SIGNAL(down(QVariant)), qaAudioReset, SLOT(trigger()));
	connect(gsUnlink, SIGNAL(down(QVariant)), qaAudioUnlink, SLOT(trigger()));
	connect(gsMinimal, SIGNAL(down(QVariant)), qaConfigMinimal, SLOT(trigger()));

	dtbLogDockTitle = new DockTitleBar();
	qdwLog->setTitleBarWidget(dtbLogDockTitle);

	foreach (QWidget *w, qdwLog->findChildren< QWidget * >()) {
		w->installEventFilter(dtbLogDockTitle);
		w->setMouseTracking(true);
	}

	dtbChatDockTitle = new DockTitleBar();
	qdwChat->setTitleBarWidget(dtbChatDockTitle);
	qdwChat->installEventFilter(dtbChatDockTitle);
	qteChat->setDefaultText(tr("<center>Not connected</center>"), true);
	qteChat->setEnabled(false);

	QWidget *dummyTitlebar = new QWidget(qdwMinimalViewNote);
	qdwMinimalViewNote->setTitleBarWidget(dummyTitlebar);

	setShowDockTitleBars((Global::get().s.wlWindowLayout == Settings::LayoutCustom) && !Global::get().s.bLockLayout);

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

	if (Global::get().s.bMinimalView && !Global::get().s.qbaMinimalViewGeometry.isNull())
		restoreGeometry(Global::get().s.qbaMinimalViewGeometry);
	else if (!Global::get().s.bMinimalView && !Global::get().s.qbaMainWindowGeometry.isNull())
		restoreGeometry(Global::get().s.qbaMainWindowGeometry);

	if (Global::get().s.bMinimalView && !Global::get().s.qbaMinimalViewState.isNull())
		restoreState(Global::get().s.qbaMinimalViewState);
	else if (!Global::get().s.bMinimalView && !Global::get().s.qbaMainWindowState.isNull())
		restoreState(Global::get().s.qbaMainWindowState);

	setupView(false);

	qmTray = new QMenu(this);
	connect(qmTray, SIGNAL(aboutToShow()), this, SLOT(trayAboutToShow()));
	trayAboutToShow();
	qstiIcon->setContextMenu(qmTray);

	updateTrayIcon();

#ifdef Q_OS_MAC
	setWindowOpacity(1.0f);
#endif
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

void MainWindow::updateToolbar() {
	bool layoutIsCustom = Global::get().s.wlWindowLayout == Settings::LayoutCustom;
	qtIconToolbar->setMovable(layoutIsCustom && !Global::get().s.bLockLayout);

	// Update the toolbar so the movable flag takes effect.
	if (layoutIsCustom) {
		// Update the toolbar, but keep it in place.
		addToolBar(toolBarArea(qtIconToolbar), qtIconToolbar);
	} else {
		// Update the toolbar, and ensure it is at the top of the window.
		addToolBar(Qt::TopToolBarArea, qtIconToolbar);
	}
}

// Sets whether or not to show the title bars on the MainWindow's
// dock widgets.
void MainWindow::setShowDockTitleBars(bool doShow) {
	dtbLogDockTitle->setEnabled(doShow);
	dtbChatDockTitle->setEnabled(doShow);
}

MainWindow::~MainWindow() {
	delete qwPTTButtonWidget;
	delete qdwLog->titleBarWidget();
	delete pmModel;
	delete qtvUsers;
	delete Channel::get(Channel::ROOT_ID);
}

void MainWindow::msgBox(QString msg) {
	MessageBoxEvent *mbe = new MessageBoxEvent(msg);
	QApplication::postEvent(this, mbe);
}

#ifdef Q_OS_WIN
bool MainWindow::nativeEvent(const QByteArray &, void *message, long *) {
	MSG *msg = reinterpret_cast< MSG * >(message);
	if (msg->message == WM_DEVICECHANGE && msg->wParam == DBT_DEVNODES_CHANGED)
		uiNewHardware++;

	return false;
}
#endif

void MainWindow::closeEvent(QCloseEvent *e) {
#ifndef Q_OS_MAC
	ServerHandlerPtr sh = Global::get().sh;
	if (sh && sh->isRunning() && Global::get().s.bAskOnQuit && !bSuppressAskOnQuit) {
		QMessageBox mb(QMessageBox::Warning, QLatin1String("Mumble"),
					   tr("Mumble is currently connected to a server. Do you want to Close or Minimize it?"),
					   QMessageBox::NoButton, this);
		QPushButton *qpbClose    = mb.addButton(tr("Close"), QMessageBox::YesRole);
		QPushButton *qpbMinimize = mb.addButton(tr("Minimize"), QMessageBox::NoRole);
		QPushButton *qpbCancel   = mb.addButton(tr("Cancel"), QMessageBox::RejectRole);
		mb.setDefaultButton(qpbClose);
		mb.setEscapeButton(qpbCancel);
		mb.exec();
		if (mb.clickedButton() == qpbMinimize) {
			showMinimized();
			e->ignore();
			return;
		} else if (mb.clickedButton() == qpbCancel) {
			e->ignore();
			return;
		}
	}
	sh.reset();
#endif

	if (Global::get().s.bMinimalView) {
		Global::get().s.qbaMinimalViewGeometry = saveGeometry();
		Global::get().s.qbaMinimalViewState    = saveState();
	} else {
		Global::get().s.qbaMainWindowGeometry = saveGeometry();
		Global::get().s.qbaMainWindowState    = saveState();
		Global::get().s.qbaHeaderState        = qtvUsers->header()->saveState();
	}

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
#ifndef Q_OS_MAC
#	ifdef Q_OS_UNIX
	if (!qApp->activeModalWidget() && !qApp->activePopupWidget())
#	endif
		if (Global::get().s.bHideInTray && qstiIcon->isSystemTrayAvailable() && e->spontaneous())
			QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
#endif
	QMainWindow::hideEvent(e);
}

void MainWindow::showEvent(QShowEvent *e) {
#ifndef Q_OS_MAC
#	ifdef Q_OS_UNIX
	if (!qApp->activeModalWidget() && !qApp->activePopupWidget())
#	endif
		if (Global::get().s.bHideInTray && qstiIcon->isSystemTrayAvailable() && e->spontaneous())
			QMetaObject::invokeMethod(this, "show", Qt::QueuedConnection);
#endif
	QMainWindow::showEvent(e);
}

void MainWindow::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);

#ifdef Q_OS_MAC
	// On modern macOS/Qt combinations, the code below causes Mumble's
	// MainWindow to not be interactive after returning from being minimized.
	// (See issue mumble-voip/mumble#2171)
	// So, let's not do it on macOS.

#else
	if (isMinimized() && qstiIcon->isSystemTrayAvailable() && Global::get().s.bHideInTray) {
		// Workaround http://qt-project.org/forums/viewthread/4423/P15/#50676
		QTimer::singleShot(0, this, SLOT(hide()));
	}
#endif
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
/// The main widgets are qteLog (the log view), qteChat (chat input bar)
/// and qtvUsers (users tree view).
void MainWindow::focusNextMainWidget() {
	QWidget *mainFocusWidgets[] = {
		qteLog,
		qteChat,
		qtvUsers,
	};
	const int numMainFocusWidgets = sizeof(mainFocusWidgets) / sizeof(mainFocusWidgets[0]);

	int currentMainFocusWidgetIndex = -1;

	QWidget *w = focusWidget();
	for (int i = 0; i < numMainFocusWidgets; i++) {
		QWidget *mainFocusWidget = mainFocusWidgets[i];
		if (w == mainFocusWidget || w->isAncestorOf(mainFocusWidget)) {
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

void MainWindow::updateTrayIcon() {
	ClientUser *p = ClientUser::get(Global::get().uiSession);

	if (Global::get().s.bDeaf) {
		qstiIcon->setIcon(qiIconDeafSelf);
	} else if (p && p->bDeaf) {
		qstiIcon->setIcon(qiIconDeafServer);
	} else if (Global::get().s.bMute) {
		qstiIcon->setIcon(qiIconMuteSelf);
	} else if (p && p->bMute) {
		qstiIcon->setIcon(qiIconMuteServer);
	} else if (p && p->bSuppress) {
		qstiIcon->setIcon(qiIconMuteSuppressed);
	} else if (Global::get().s.bStateInTray && Global::get().bPushToMute) {
		qstiIcon->setIcon(qiIconMutePushToMute);
	} else if (p && Global::get().s.bStateInTray) {
		switch (p->tsState) {
			case Settings::Talking:
			case Settings::MutedTalking:
				qstiIcon->setIcon(qiTalkingOn);
				break;
			case Settings::Whispering:
				qstiIcon->setIcon(qiTalkingWhisper);
				break;
			case Settings::Shouting:
				qstiIcon->setIcon(qiTalkingShout);
				break;
			case Settings::Passive:
			default:
				qstiIcon->setIcon(qiTalkingOff);
				break;
		}
	} else {
		qstiIcon->setIcon(qiIcon);
	}
}

void MainWindow::updateUserModel() {
	UserModel *um = static_cast< UserModel * >(qtvUsers->model());
	um->toggleChannelFiltered(nullptr); // Force a UI refresh
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
	if ((Global::get().s.wlWindowLayout == Settings::LayoutCustom) && !Global::get().s.bLockLayout) {
		return QMainWindow::createPopupMenu();
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

	if (Global::get().uiSession != 0) {
		QModelIndex idx;

		if (!qpContextPosition.isNull())
			idx = qtvUsers->indexAt(qpContextPosition);

		if (!idx.isValid())
			idx = qtvUsers->currentIndex();

		target.user    = pmModel->getUser(idx);
		target.channel = pmModel->getChannel(idx);

		if (!target.user)
			target.user = getContextMenuUser();

		if (!target.channel)
			target.channel = getContextMenuChannel();
	}

	cuContextUser     = target.user;
	cContextChannel   = target.channel;
	qpContextPosition = QPoint();

	return target;
}

bool MainWindow::handleSpecialContextMenu(const QUrl &url, const QPoint &pos_, bool focus) {
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
			QByteArray qbaServerDigest = QByteArray::fromBase64(url.path().remove(0, 1).toLatin1());
			cuContextUser              = ClientUser::get(url.host().toInt(&ok, 10));
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
		bool ok;
		QByteArray qbaServerDigest = QByteArray::fromBase64(url.path().remove(0, 1).toLatin1());
		cContextChannel            = Channel::get(url.host().toInt(&ok, 10));
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

void MainWindow::on_qtvUsers_customContextMenuRequested(const QPoint &mpos, bool usePositionForGettingContext) {
	QModelIndex idx = qtvUsers->indexAt(mpos);
	if (!idx.isValid() || !usePositionForGettingContext) {
		idx = qtvUsers->currentIndex();
	} else {
		qtvUsers->setCurrentIndex(idx);
	}

	ClientUser *p    = pmModel->getUser(idx);
	Channel *channel = pmModel->getChannel(idx);

	qpContextPosition = mpos;
	if (pmModel->isChannelListener(idx)) {
		// Have a separate context menu for listeners
		QModelIndex parent = idx.parent();

		if (parent.isValid()) {
			// Find the channel in which the action was triggered and set it
			// in order to be able to obtain it in the action itself
			cContextChannel = pmModel->getChannel(parent);
		}
		cuContextUser.clear();
		qmListener->exec(qtvUsers->mapToGlobal(mpos), nullptr);
		cuContextUser.clear();
		cContextChannel.clear();
	} else {
		if (p) {
			cuContextUser.clear();
			if (!usePositionForGettingContext) {
				cuContextUser = p;
			}

			qmUser->exec(qtvUsers->mapToGlobal(mpos), nullptr);
			cuContextUser.clear();
		} else {
			cContextChannel.clear();

			if (!usePositionForGettingContext && channel) {
				cContextChannel = channel;
			}

			qmChannel->exec(qtvUsers->mapToGlobal(mpos), nullptr);
			cContextChannel.clear();
		}
	}
	qpContextPosition = QPoint();
}

void MainWindow::on_qteLog_customContextMenuRequested(const QPoint &mpos) {
	QString link = qteLog->anchorAt(mpos);
	if (!link.isEmpty()) {
		QUrl l(link);

		if (handleSpecialContextMenu(l, qteLog->mapToGlobal(mpos)))
			return;
	}

	QPoint contentPosition =
		QPoint(QApplication::isRightToLeft()
				   ? (qteLog->horizontalScrollBar()->maximum() - qteLog->horizontalScrollBar()->value())
				   : qteLog->horizontalScrollBar()->value(),
			   qteLog->verticalScrollBar()->value());
	QMenu *menu = qteLog->createStandardContextMenu(mpos + contentPosition);

	QTextCursor cursor  = qteLog->cursorForPosition(mpos);
	QTextCharFormat fmt = cursor.charFormat();

	// Work around imprecise cursor image identification
	// Apparently, the cursor is shifted half the characters width to the right on the image
	// element. This is in contrast to hyperlinks for example, which have correct edge detection.
	// For the image, we get the right half (plus the left half of the next character) for the
	// image, and have to move the cursor forward to also detect on the left half of the image
	// (plus the right half of the previous character).
	// It is unclear why we have to use NextCharacter instead of PreviousCharacter.
	if (fmt.objectType() == QTextFormat::NoObject) {
		cursor.movePosition(QTextCursor::NextCharacter);
		fmt = cursor.charFormat();
	}
	if (cursor.charFormat().isImageFormat()) {
		menu->addSeparator();
		menu->addAction(tr("Save Image As..."), this, SLOT(saveImageAs(void)));

		qtcSaveImageCursor = cursor;
	}

	menu->addSeparator();
	menu->addAction(tr("Clear"), qteLog, SLOT(clear(void)));
	menu->exec(qteLog->mapToGlobal(mpos));
	delete menu;
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

	QString resName = qtcSaveImageCursor.charFormat().toImageFormat().name();
	QVariant res    = qteLog->document()->resource(QTextDocument::ImageResource, resName);
	QImage img      = res.value< QImage >();
	bool ok         = img.save(fname);
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

static void recreateServerHandler() {
	ServerHandlerPtr sh = Global::get().sh;
	if (sh && sh->isRunning()) {
		Global::get().mw->on_qaServerDisconnect_triggered();
		sh->disconnect();
		sh->wait();
		QCoreApplication::instance()->processEvents();
	}

	Global::get().sh.reset();
	while (sh && !sh.unique())
		QThread::yieldCurrentThread();
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
		} catch (const std::exception &e) {
			Global::get().l->log(Log::Warning, tr("Invalid settings file encountered."));
		}

		return;
	}

	if (url.scheme() != QLatin1String("mumble")) {
		Global::get().l->log(Log::Warning, tr("URL scheme is not 'mumble'"));
		return;
	}

	int major, minor, patch;
	int thismajor, thisminor, thispatch;
	Version::get(&thismajor, &thisminor, &thispatch);

	// With no version parameter given assume the link refers to our version
	major = thismajor;
	minor = thisminor;
	patch = thispatch;

	QUrlQuery query(url);
	QString version = query.queryItemValue(QLatin1String("version"));
	if (version.size() > 0) {
		if (!Version::get(&major, &minor, &patch, version)) {
			// The version format is invalid
			Global::get().l->log(Log::Warning,
								 QObject::tr("The provided URL uses an invalid version format: \"%1\"").arg(version));
			return;
		}
	}

	// We can't handle URLs for versions < 1.2.0
	const int minMajor   = 1;
	const int minMinor   = 2;
	const int minPatch   = 0;
	const bool isPre_120 = major < minMajor || (major == minMajor && minor < minMinor)
						   || (major == minMajor && minor == minMinor && patch < minPatch);
	// We also can't handle URLs for versions newer than the running Mumble instance
	const bool isFuture = major > thismajor || (major == thismajor && minor > thisminor)
						  || (major == thismajor && minor == thisminor && patch > thispatch);

	if (isPre_120 || isFuture) {
		Global::get().l->log(Log::Warning, tr("This version of Mumble can't handle URLs for Mumble version %1.%2.%3")
											   .arg(major)
											   .arg(minor)
											   .arg(patch));
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
	bool found          = false;
	QStringList qlChans = qsDesiredChannel.split(QLatin1String("/"));
	Channel *chan       = Channel::get(Channel::ROOT_ID);
	QString str         = QString();
	while (chan && qlChans.count() > 0) {
		QString elem = qlChans.takeFirst().toLower();
		if (elem.isEmpty())
			continue;
		if (str.isNull())
			str = elem;
		else
			str = str + QLatin1String("/") + elem;
		foreach (Channel *c, chan->qlChannels) {
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
		qtvUsers->setCurrentIndex(pmModel->index(chan));
	} else if (Global::get().uiSession) {
		qtvUsers->setCurrentIndex(pmModel->index(ClientUser::get(Global::get().uiSession)->cChannel));
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

void MainWindow::setupView(bool toggle_minimize) {
	bool showit = !Global::get().s.bMinimalView;

	switch (Global::get().s.wlWindowLayout) {
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
		default:
			break;
	}

	updateToolbar();

	qteChat->updateGeometry();

	QRect geom = frameGeometry();

	if (toggle_minimize) {
		if (!showit) {
			Global::get().s.qbaMainWindowGeometry = saveGeometry();
			Global::get().s.qbaMainWindowState    = saveState();
			Global::get().s.qbaHeaderState        = qtvUsers->header()->saveState();
		} else {
			Global::get().s.qbaMinimalViewGeometry = saveGeometry();
			Global::get().s.qbaMinimalViewState    = saveState();
		}
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
		foreach (QAction *a, menuBar()->actions()) {
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
		foreach (QAction *a, menuBar()->actions()) {
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
		if (!showit) {
			if (!Global::get().s.qbaMinimalViewGeometry.isNull())
				restoreGeometry(Global::get().s.qbaMinimalViewGeometry);
			if (!Global::get().s.qbaMinimalViewState.isNull())
				restoreState(Global::get().s.qbaMinimalViewState);
		} else {
			if (!Global::get().s.qbaMainWindowGeometry.isNull())
				restoreGeometry(Global::get().s.qbaMainWindowGeometry);
			if (!Global::get().s.qbaMainWindowState.isNull())
				restoreState(Global::get().s.qbaMainWindowState);
		}
	} else {
		QRect newgeom = frameGeometry();
		resize(geometry().width() - newgeom.width() + geom.width(),
			   geometry().height() - newgeom.height() + geom.height());
		move(geom.x(), geom.y());
	}


	// Hide/Show respective UI elements
	qdwLog->setVisible(showit);
	qdwChat->setVisible(showit);
	qtIconToolbar->setVisible(showit);
	menuBar()->setVisible(showit);

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

	show();
	activateWindow();

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
}

void MainWindow::on_Reconnect_timeout() {
	if (Global::get().sh->isRunning())
		return;
	Global::get().l->log(Log::Information, tr("Reconnecting."));
	Global::get().sh->start(QThread::TimeCriticalPriority);
}

void MainWindow::on_qmSelf_aboutToShow() {
	ClientUser *user = ClientUser::get(Global::get().uiSession);

	qaServerTexture->setEnabled(user != nullptr);
	qaSelfComment->setEnabled(user != nullptr);

	qaServerTextureRemove->setEnabled(user && !user->qbaTextureHash.isEmpty());

	qaSelfRegister->setEnabled(user && (user->iId < 0) && !user->qsHash.isEmpty()
							   && (Global::get().pPermissions & (ChanACL::SelfRegister | ChanACL::Write)));
	if (Global::get().sh && Global::get().sh->uiVersion >= 0x010203) {
		qaSelfPrioritySpeaker->setEnabled(user && Global::get().pPermissions & (ChanACL::Write | ChanACL::MuteDeafen));
		qaSelfPrioritySpeaker->setChecked(user && user->bPrioritySpeaker);
	} else {
		qaSelfPrioritySpeaker->setEnabled(false);
		qaSelfPrioritySpeaker->setChecked(false);
	}
}

void MainWindow::on_qaSelfComment_triggered() {
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

void MainWindow::on_qaSelfRegister_triggered() {
	ClientUser *p = ClientUser::get(Global::get().uiSession);
	if (!p)
		return;

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
	qmServer->addAction(qaServerTokens);
	qmServer->addAction(qaServerUserList);
	qmServer->addAction(qaServerBanList);
	qmServer->addSeparator();
#if !defined(Q_OS_MAC)
	// Don't add qaHide on macOS.
	// There is no way to bring the window back (no 'tray' for Mumble on macOS),
	// and the system has built-in hide functionality via Cmd-H.
	if (qstiIcon->isSystemTrayAvailable())
		qmServer->addAction(qaHide);
#endif
	qmServer->addAction(qaQuit);

	qaServerBanList->setEnabled(Global::get().pPermissions & (ChanACL::Ban | ChanACL::Write));
	qaServerUserList->setEnabled(Global::get().pPermissions & (ChanACL::Register | ChanACL::Write));
	qaServerInformation->setEnabled(Global::get().uiSession != 0);
	qaServerTokens->setEnabled(Global::get().uiSession != 0);

	if (!qlServerActions.isEmpty()) {
		qmServer->addSeparator();
		foreach (QAction *a, qlServerActions)
			qmServer->addAction(a);
	}
}

void MainWindow::on_qaServerDisconnect_triggered() {
	if (qtReconnect->isActive()) {
		qtReconnect->stop();
		qaServerDisconnect->setEnabled(false);
	}
	if (Global::get().sh && Global::get().sh->isRunning())
		Global::get().sh->disconnect();
}

void MainWindow::on_qaServerBanList_triggered() {
	Global::get().sh->requestBanList();

	if (banEdit) {
		banEdit->reject();
		delete banEdit;
		banEdit = nullptr;
	}
}

void MainWindow::on_qaServerUserList_triggered() {
	Global::get().sh->requestUserList();

	if (userEdit) {
		userEdit->reject();
		delete userEdit;
		userEdit = nullptr;
	}
}

void MainWindow::on_qaServerInformation_triggered() {
	ServerInformation *infoDialog = new ServerInformation(this);
	infoDialog->show();
}

void MainWindow::on_qaServerTexture_triggered() {
	QPair< QByteArray, QImage > choice = openImageFile();
	if (choice.first.isEmpty())
		return;

	const QImage &img = choice.second;

	if ((img.height() <= 1024) && (img.width() <= 1024))
		Global::get().sh->setUserTexture(Global::get().uiSession, choice.first);
}

void MainWindow::on_qaServerTextureRemove_triggered() {
	Global::get().sh->setUserTexture(Global::get().uiSession, QByteArray());
}

void MainWindow::on_qaServerTokens_triggered() {
	if (tokenEdit) {
		tokenEdit->reject();
		delete tokenEdit;
		tokenEdit = nullptr;
	}

	tokenEdit = new Tokens(this);
	tokenEdit->show();
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

	if (self && p && !isSelf) {
		qmUser->addAction(qaUserJoin);
		qaUserJoin->setEnabled(self->cChannel != p->cChannel);

		qmUser->addSeparator();
	}

	if (Global::get().pPermissions & (ChanACL::Kick | ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserKick);
	if (Global::get().pPermissions & (ChanACL::Ban | ChanACL::Write))
		qmUser->addAction(qaUserBan);
	qmUser->addAction(qaUserMute);
	qmUser->addAction(qaUserDeaf);
	if (Global::get().sh && Global::get().sh->uiVersion >= 0x010203)
		qmUser->addAction(qaUserPrioritySpeaker);
	qmUser->addAction(qaUserLocalMute);
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
	if (Global::get().sh && Global::get().sh->uiVersion >= 0x010202)
		qmUser->addAction(qaUserInformation);

	if (p && (p->iId < 0) && !p->qsHash.isEmpty()
		&& (Global::get().pPermissions & ((isSelf ? ChanACL::SelfRegister : ChanACL::Register) | ChanACL::Write))) {
		qmUser->addSeparator();
		qmUser->addAction(qaUserRegister);
	}

	if (p && !p->qsHash.isEmpty() && (!p->qsFriendName.isEmpty() || (p->uiSession != Global::get().uiSession))) {
		qmUser->addSeparator();
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
		foreach (QAction *a, qlUserActions)
			qmUser->addAction(a);
	}

	if (!p) {
		qaUserKick->setEnabled(false);
		qaUserBan->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
		qaUserLocalNickname->setEnabled(false);
		qaUserLocalMute->setEnabled(false);
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
		qaUserLocalIgnore->setEnabled(!isSelf);
		qaUserLocalIgnoreTTS->setEnabled(!isSelf);
		// If the server's version is less than 1.4.0 it won't support the new permission to reset a comment/avatar, so
		// fall back to the old method
		if (Global::get().sh->uiVersion < 0x010400) {
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
			m_listenerLocalVolumeSlider->setListenedChannel(*channel);
			qmListener->addAction(m_listenerLocalVolumeSlider.get());
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
	ClientUser *p = ClientUser::get(Global::get().uiSession);
	if (!p)
		return;

	MumbleProto::UserState mpus;
	mpus.set_session(p->uiSession);
	mpus.set_priority_speaker(!p->bPrioritySpeaker);
	Global::get().sh->sendMessage(mpus);
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
	if (!p)
		return;

	Global::get().db->addFriend(p->qsName, p->qsHash);
	pmModel->setFriendName(p, p->qsName);
}

void MainWindow::on_qaUserFriendUpdate_triggered() {
	on_qaUserFriendAdd_triggered();
}

void MainWindow::on_qaUserFriendRemove_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	Global::get().db->removeFriend(p->qsHash);
	pmModel->setFriendName(p, QString());
}

void MainWindow::on_qaUserKick_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Kicking user %1").arg(p->qsName), tr("Enter reason"),
										   QLineEdit::Normal, QString(), &ok);

	p = ClientUser::get(session);
	if (!p)
		return;

	if (ok) {
		Global::get().sh->kickBanUser(p->uiSession, reason, false);
	}
}

void MainWindow::on_qaUserBan_triggered() {
	ClientUser *p = getContextMenuUser();
	if (!p)
		return;

	unsigned int session = p->uiSession;

	bool ok;
	QString reason = QInputDialog::getText(this, tr("Banning user %1").arg(p->qsName), tr("Enter reason"),
										   QLineEdit::Normal, QString(), &ok);
	p              = ClientUser::get(session);
	if (!p)
		return;

	if (ok) {
		Global::get().sh->kickBanUser(p->uiSession, reason, true);
	}
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
	UserLocalNicknameDialog::present(session, qmUserNicknameTracker);
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

	Global::get().sh->requestUserStats(p->uiSession, false);
}

void MainWindow::on_qaHide_triggered() {
	hide();
}

void MainWindow::on_qaQuit_triggered() {
	bSuppressAskOnQuit = true;
	this->close();
}

void MainWindow::sendChatbarText(QString qsText, bool plainText) {
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

	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());
	Channel *c    = pmModel->getChannel(qtvUsers->currentIndex());

	if (!Global::get().s.bChatBarUseSelection || !p || p->uiSession == Global::get().uiSession) {
		// Channel message
		if (!Global::get().s.bChatBarUseSelection || !c) // If no channel selected fallback to current one
			c = ClientUser::get(Global::get().uiSession)->cChannel;

		Global::get().sh->sendChannelTextMessage(c->iId, qsMessage, false);
		Global::get().l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatChannel(c), qsMessage),
							 tr("Message to channel %1").arg(c->qsName), true);
	} else {
		// User message
		Global::get().sh->sendUserTextMessage(p->uiSession, qsMessage);
		Global::get().l->log(Log::TextMessage, tr("To %1: %2").arg(Log::formatClientUser(p, Log::Target), qsMessage),
							 tr("Message to %1").arg(p->qsName), true);
	}
}

/**
 * Controls tab username completion for the chatbar.
 * @see ChatbarLineEdit::completeAtCursor()
 */
void MainWindow::on_qteChat_tabPressed() {
	qteChat->completeAtCursor();
}

/// Handles Backtab/Shift-Tab for qteChat, which allows
/// users to move focus to the previous widget in
/// MainWindow.
void MainWindow::on_qteChat_backtabPressed() {
	focusPreviousChild();
}

/**
 * Controls ctrl space username completion and selection for the chatbar.
 * @see ChatbarLineEdit::completeAtCursor()
 */
void MainWindow::on_qteChat_ctrlSpacePressed() {
	unsigned int res = qteChat->completeAtCursor();
	if (res == 0)
		return;
	qtvUsers->setCurrentIndex(pmModel->index(ClientUser::get(res)));
}

void MainWindow::on_qmConfig_aboutToShow() {
	// Don't remove the config, as that messes up OSX.
	foreach (QAction *a, qmConfig->actions())
		if (a != qaConfigDialog)
			qmConfig->removeAction(a);
	qmConfig->addAction(qaAudioWizard);
	qmConfig->addAction(qaConfigCert);
	qmConfig->addSeparator();
	qaAudioTTS->setChecked(Global::get().s.bTTS);
	qmConfig->addAction(qaAudioTTS);
	qmConfig->addSeparator();
	qmConfig->addAction(qaConfigMinimal);

	qaTalkingUIToggle->setChecked(Global::get().talkingUI && Global::get().talkingUI->isVisible());

	qmConfig->addAction(qaTalkingUIToggle);
	if (Global::get().s.bMinimalView)
		qmConfig->addAction(qaConfigHideFrame);
}

void MainWindow::qmChannel_aboutToShow() {
	Channel *c = getContextMenuTargets().channel;

	qmChannel->clear();

	if (c && c->iId != ClientUser::get(Global::get().uiSession)->cChannel->iId) {
		qmChannel->addAction(qaChannelJoin);

		qmChannel->addSeparator();
	}

	if (c && Global::get().sh && Global::get().sh->uiVersion >= 0x010400) {
		// If the server's version is less than 1.4, the listening feature is not supported yet
		// and thus it doesn't make sense to show the action for it
		qmChannel->addAction(qaChannelListen);
		qaChannelListen->setChecked(Global::get().channelListenerManager->isListening(Global::get().uiSession, c->iId));
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
		qmChannel->addAction(qaChannelFilter);
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
		foreach (QAction *a, qlChannelActions)
			qmChannel->addAction(a);
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
		if (!c)
			c = Channel::get(Channel::ROOT_ID);
		unlinkall = (home->qhLinks.count() > 0);
		if (home != c) {
			if (c->allLinks().contains(home))
				unlink = true;
			else
				link = true;
		}
	}

	if (c)
		qaChannelFilter->setChecked(c->bFiltered);

	qaChannelAdd->setEnabled(add);
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

void MainWindow::on_qaChannelFilter_triggered() {
	Channel *c = getContextMenuChannel();

	if (c) {
		UserModel *um = static_cast< UserModel * >(qtvUsers->model());
		um->toggleChannelFiltered(c);
	}
}

void MainWindow::on_qaChannelAdd_triggered() {
	Channel *c = getContextMenuChannel();
	if (aclEdit) {
		aclEdit->reject();
		delete aclEdit;
		aclEdit = nullptr;
	}

	aclEdit = new ACLEditor(c ? c->iId : 0, this);
	if (c && (c->uiPermissions & ChanACL::Cached) && !(c->uiPermissions & (ChanACL::Write | ChanACL::MakeChannel))) {
		aclEdit->qcbChannelTemporary->setEnabled(false);
		aclEdit->qcbChannelTemporary->setChecked(true);
	}

	aclEdit->show();
}

void MainWindow::on_qaChannelRemove_triggered() {
	int ret;
	Channel *c = getContextMenuChannel();
	if (!c)
		return;

	int id = c->iId;

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
		c = Channel::get(Channel::ROOT_ID);
	int id = c->iId;

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
		l = Channel::get(Channel::ROOT_ID);

	Global::get().sh->addChannelLink(c->iId, l->iId);
}

void MainWindow::on_qaChannelUnlink_triggered() {
	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
	Channel *l = getContextMenuChannel();
	if (!l)
		l = Channel::get(Channel::ROOT_ID);

	Global::get().sh->removeChannelLink(c->iId, l->iId);
}

void MainWindow::on_qaChannelUnlinkAll_triggered() {
	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(c->iId);
	foreach (Channel *l, c->qsPermLinks)
		mpcs.add_links_remove(l->iId);
	Global::get().sh->sendMessage(mpcs);
}

void MainWindow::on_qaChannelSendMessage_triggered() {
	Channel *c = getContextMenuChannel();

	if (!c)
		return;

	int id = c->iId;

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
	ClientUser *cu = nullptr;
	Channel *c     = nullptr;

	if (Global::get().uiSession) {
		cu = getContextMenuUser();
		if (!cu)
			cu = pmModel->getUser(qtvUsers->currentIndex());

		c = cu ? cu->cChannel : getContextMenuChannel();
		if (!c)
			c = pmModel->getChannel(qtvUsers->currentIndex());
	}

	ChanACL::Permissions p = c ? static_cast< ChanACL::Permissions >(c->uiPermissions) : ChanACL::None;

	if (c && !p) {
		Global::get().sh->requestChannelPermissions(c->iId);
		if (c->iId == 0)
			p = Global::get().pPermissions;
		else
			p = ChanACL::All;

		c->uiPermissions = p;
	}

	Channel *cparent = c ? c->cParent : nullptr;
	ChanACL::Permissions pparent =
		cparent ? static_cast< ChanACL::Permissions >(cparent->uiPermissions) : ChanACL::None;

	if (cparent && !pparent) {
		Global::get().sh->requestChannelPermissions(cparent->iId);
		if (cparent->iId == 0)
			pparent = Global::get().pPermissions;
		else
			pparent = ChanACL::All;

		cparent->uiPermissions = pparent;
	}

	ClientUser *user           = Global::get().uiSession ? ClientUser::get(Global::get().uiSession) : nullptr;
	Channel *homec             = user ? user->cChannel : nullptr;
	ChanACL::Permissions homep = homec ? static_cast< ChanACL::Permissions >(homec->uiPermissions) : ChanACL::None;

	if (homec && !homep) {
		Global::get().sh->requestChannelPermissions(homec->iId);
		if (homec->iId == 0)
			homep = Global::get().pPermissions;
		else
			homep = ChanACL::All;

		homec->uiPermissions = homep;
	}

	if (cu) {
		qaUserMute->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen)
							   && ((cu != user) || cu->bMute || cu->bSuppress));
		qaUserDeaf->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen) && ((cu != user) || cu->bDeaf));
		qaUserPrioritySpeaker->setEnabled(p & (ChanACL::Write | ChanACL::MuteDeafen));
		qaUserTextMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
		qaUserInformation->setEnabled((Global::get().pPermissions & (ChanACL::Write | ChanACL::Register))
									  || (p & (ChanACL::Write | ChanACL::Enter)) || (cu == user));
	} else {
		qaUserMute->setEnabled(false);
		qaUserDeaf->setEnabled(false);
		qaUserPrioritySpeaker->setEnabled(false);
		qaUserTextMessage->setEnabled(false);
		qaUserInformation->setEnabled(false);
	}

	qaChannelJoin->setEnabled(p & (ChanACL::Write | ChanACL::Enter));

	qaChannelAdd->setEnabled(p & (ChanACL::Write | ChanACL::MakeChannel | ChanACL::MakeTempChannel));
	qaChannelRemove->setEnabled(p & ChanACL::Write);
	qaChannelACL->setEnabled((p & ChanACL::Write) || (pparent & ChanACL::Write));

	qaChannelLink->setEnabled((p & (ChanACL::Write | ChanACL::LinkChannel))
							  && (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlink->setEnabled((p & (ChanACL::Write | ChanACL::LinkChannel))
								|| (homep & (ChanACL::Write | ChanACL::LinkChannel)));
	qaChannelUnlinkAll->setEnabled(p & (ChanACL::Write | ChanACL::LinkChannel));

	qaChannelCopyURL->setEnabled(c);
	qaChannelSendMessage->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
	qaChannelFilter->setEnabled(true);
	qteChat->setEnabled(p & (ChanACL::Write | ChanACL::TextMessage));
}

void MainWindow::userStateChanged() {
	if (Global::get().s.bStateInTray) {
		updateTrayIcon();
	}

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

void MainWindow::on_qaAudioReset_triggered() {
	AudioInputPtr ai = Global::get().ai;
	if (ai)
		ai->bResetProcessor = true;
}

void MainWindow::on_qaFilterToggle_triggered() {
	Global::get().s.bFilterActive = qaFilterToggle->isChecked();
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
	updateTrayIcon();
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

	if (!qaAudioDeaf->isChecked() && bAutoUnmute) {
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
		bAutoUnmute           = true;
		Global::get().s.bMute = true;
		qaAudioMute->setChecked(true);
		Global::get().l->log(Log::SelfDeaf, tr("Muted and deafened."));
	} else if (Global::get().s.bDeaf) {
		Global::get().l->log(Log::SelfDeaf, tr("Deafened."));
		bAutoUnmute = false;
	} else {
		Global::get().l->log(Log::SelfUndeaf, tr("Undeafened."));
	}

	if (Global::get().sh) {
		Global::get().sh->setSelfMuteDeafState(Global::get().s.bMute, Global::get().s.bDeaf);
	}

	updateAudioToolTips();
	updateTrayIcon();
}

void MainWindow::setAudioDeaf(bool deaf) {
	// Pretend the user pushed the button manually
	qaAudioDeaf->setChecked(deaf);
	qaAudioDeaf->triggered(deaf);
}

void MainWindow::on_qaRecording_triggered() {
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

void MainWindow::on_qaAudioTTS_triggered() {
	Global::get().s.bTTS = qaAudioTTS->isChecked();
}

void MainWindow::on_qaAudioStats_triggered() {
	AudioStats *as = new AudioStats(this);
	as->show();
}

void MainWindow::on_qaAudioUnlink_triggered() {
	Global::get().pluginManager->unlinkPositionalData();
}

void MainWindow::on_qaConfigDialog_triggered() {
	ConfigDialog *dlg = new ConfigDialog(this);

	QObject::connect(dlg, &ConfigDialog::settingsAccepted, Global::get().talkingUI, &TalkingUI::on_settingsChanged);

	if (dlg->exec() == QDialog::Accepted) {
		setupView(false);
		updateTransmitModeComboBox(Global::get().s.atTransmit);
		updateTrayIcon();
		updateUserModel();

		if (Global::get().s.requireRestartToApply) {
			if (Global::get().s.requireRestartToApply
				&& QMessageBox::question(
					   this, tr("Restart Mumble?"),
					   tr("Some settings will only apply after a restart of Mumble. Restart Mumble now?"),
					   QMessageBox::Yes | QMessageBox::No)
					   == QMessageBox::Yes) {
				bSuppressAskOnQuit = true;
				restartOnQuit      = true;

				close();
			}
		}
	}

	delete dlg;
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
	CertWizard *cw = new CertWizard(this);
	cw->exec();
	delete cw;
}

void MainWindow::on_qaAudioWizard_triggered() {
	AudioWizard *aw = new AudioWizard(this);
	aw->exec();
	delete aw;
}

void MainWindow::on_qaDeveloperConsole_triggered() {
	Global::get().c->show();
}

void MainWindow::on_qaPositionalAudioViewer_triggered() {
	if (m_paViewer) {
		m_paViewer->raise();
	} else {
		m_paViewer = std::make_unique< PositionalAudioViewer >();
		connect(m_paViewer.get(), &PositionalAudioViewer::finished, this, [this]() { m_paViewer.reset(); });
		m_paViewer->show();
	}
}

void MainWindow::on_qaHelpWhatsThis_triggered() {
	QWhatsThis::enterWhatsThisMode();
}

void MainWindow::on_qaHelpAbout_triggered() {
	AboutDialog adAbout(this);
	adAbout.exec();
}

void MainWindow::on_qaHelpAboutQt_triggered() {
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::on_qaHelpVersionCheck_triggered() {
	new VersionCheck(false, this);
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
		Global::get().uiDoublePush = Global::get().tDoublePush.restart();
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
	updateTrayIcon();
	updateUserModel();
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
				c = Channel::get(Channel::ROOT_ID);
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
		c = Channel::get(idx);
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
		foreach (const ShortcutTarget &st, qmCurrentTargets.keys()) {
			ShortcutTarget nt;
			center               = center || st.bForceCenter;
			nt.bUsers            = st.bUsers;
			nt.bCurrentSelection = st.bCurrentSelection;

			if (nt.bCurrentSelection) {
				Channel *c = pmModel->getSelectedChannel();
				if (c) {
					nt.bUsers    = false;
					nt.iChannel  = c->iId;
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
				foreach (const QString &hash, st.qlUsers) {
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
					nt.iChannel  = c->iId;
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
				mpvt.set_id(idx);

				foreach (const ShortcutTarget &st, ql) {
					MumbleProto::VoiceTarget_Target *t = mpvt.add_targets();
					// st.bCurrentSelection has been taken care of at this point already (if it was set) so
					// we don't have to check for that here.
					if (st.bUsers) {
						foreach (unsigned int uisession, st.qlSessions)
							t->add_session(uisession);
					} else {
						t->set_channel_id(st.iChannel);
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
							mpvt.set_id(oldidx);
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
		if (Global::get().mw->isVisible()) {
			Global::get().mw->hide();
		} else {
			Global::get().mw->show();
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
	Global::get().uiSession    = 0;
	Global::get().pPermissions = ChanACL::None;
	Global::get().iCodecAlpha  = 0x8000000b;
	Global::get().bPreferAlpha = true;
	Global::get().bOpus        = true;
	Global::get().iCodecBeta   = 0;

#ifdef Q_OS_MAC
	// Suppress AppNap while we're connected to a server.
	MUSuppressAppNap(true);
#endif

	Global::get().l->clearIgnore();
	Global::get().l->setIgnore(Log::UserJoin);
	Global::get().l->setIgnore(Log::OtherSelfMute);
	QString host, uname, pw;
	unsigned short port;
	Global::get().sh->getConnectionInfo(host, port, uname, pw);
	Global::get().l->log(Log::ServerConnected, tr("Connected."));
	qaServerDisconnect->setEnabled(true);
	qaServerInformation->setEnabled(true);
	qaServerBanList->setEnabled(true);

	Channel *root = Channel::get(Channel::ROOT_ID);
	pmModel->renameChannel(root, tr("Root"));
	pmModel->setCommentHash(root, QByteArray());
	root->uiPermissions = 0;

	qtvUsers->setRowHidden(0, QModelIndex(), false);

	Global::get().bAllowHTML      = true;
	Global::get().uiMessageLength = 5000;
	Global::get().uiImageLength   = 131072;
	Global::get().uiMaxUsers      = 0;

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
}

void MainWindow::serverDisconnected(QAbstractSocket::SocketError err, QString reason) {
	// clear ChannelListener
	Global::get().channelListenerManager->clear();

	Global::get().uiSession        = 0;
	Global::get().pPermissions     = ChanACL::None;
	Global::get().bAttenuateOthers = false;
	qaServerDisconnect->setEnabled(false);
	qaServerInformation->setEnabled(false);
	qaServerBanList->setEnabled(false);
	qtvUsers->setCurrentIndex(QModelIndex());
	qteChat->setEnabled(false);
	updateTrayIcon();

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
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	qs += QSet< QAction * >(qlServerActions.begin(), qlServerActions.end());
	qs += QSet< QAction * >(qlChannelActions.begin(), qlChannelActions.end());
	qs += QSet< QAction * >(qlUserActions.begin(), qlUserActions.end());
#else
	// In Qt 5.14 QList::toSet() has been deprecated as there exists a dedicated constructor of QSet for this now
	qs += qlServerActions.toSet();
	qs += qlChannelActions.toSet();
	qs += qlUserActions.toSet();
#endif

	foreach (QAction *a, qs)
		delete a;

	qlServerActions.clear();
	qlChannelActions.clear();
	qlUserActions.clear();

	pmModel->removeAll();
	qtvUsers->setRowHidden(0, QModelIndex(), true);

	// Update QActions and menus
	on_qmServer_aboutToShow();
	on_qmSelf_aboutToShow();
	qmChannel_aboutToShow();
	qmUser_aboutToShow();
	on_qmConfig_aboutToShow();

	// We can't record without a server anyway, so we disable the functionality here
	enableRecording(false);

	if (!Global::get().sh->qlErrors.isEmpty()) {
		foreach (QSslError e, Global::get().sh->qlErrors)
			Global::get().l->log(Log::Warning, tr("SSL Verification failed: %1").arg(e.errorString().toHtmlEscaped()));
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
			foreach (QSslError e, Global::get().sh->qlErrors)
				qsl << QString::fromLatin1("<li>%1</li>").arg(e.errorString().toHtmlEscaped());

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
		QMessageBox::warning(this, tr("SSL Version mismatch"),
							 tr("This server is using an older encryption standard, and is no longer supported by "
								"modern versions of Mumble."),
							 QMessageBox::Ok);
	} else {
		bool ok = false;


		if (!reason.isEmpty()) {
			Global::get().l->log(Log::ServerDisconnected,
								 tr("Server connection failed: %1.").arg(reason.toHtmlEscaped()));
		} else {
			Global::get().l->log(Log::ServerDisconnected, tr("Disconnected from server."));
		}

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
		// Qt 5.15 introduced a default constructor that initializes the flags to be set to no flags
		Qt::WindowFlags wf;
#elif defined(Q_OS_MAC)
		Qt::WindowFlags wf = Qt::Sheet;
#else
		// Before Qt 5.15 we have emulate the default constructor by assigning a literal zero
		Qt::WindowFlags wf = 0;
#endif

		bool matched = true;
		switch (rtLast) {
			case MumbleProto::Reject_RejectType_InvalidUsername:
				uname = QInputDialog::getText(this, tr("Invalid username"),
											  tr("You connected with an invalid username, please try another one."),
											  QLineEdit::Normal, uname, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_UsernameInUse:
				uname = QInputDialog::getText(this, tr("Username in use"),
											  tr("That username is already in use, please try another username."),
											  QLineEdit::Normal, uname, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_WrongUserPW:
				pw = QInputDialog::getText(this, tr("Wrong certificate or password"),
										   tr("Wrong certificate or password for registered user. If you are\n"
											  "certain this user is protected by a password please retry.\n"
											  "Otherwise abort and check your certificate and username."),
										   QLineEdit::Password, pw, &ok, wf);
				break;
			case MumbleProto::Reject_RejectType_WrongServerPW:
				pw = QInputDialog::getText(this, tr("Wrong password"),
										   tr("Wrong server password for unregistered user account, please try again."),
										   QLineEdit::Password, pw, &ok, wf);
				break;
			default:
				matched = false;
				break;
		}
		if (ok && matched) {
			if (!Global::get().s.bSuppressIdentity)
				Global::get().db->setPassword(host, port, uname, pw);
			qaServerDisconnect->setEnabled(true);
			Global::get().sh->setConnectionInfo(host, port, uname, pw);
			on_Reconnect_timeout();
		} else if (!matched && Global::get().s.bReconnect && !reason.isEmpty()) {
			qaServerDisconnect->setEnabled(true);
			if (bRetryServer) {
				qtReconnect->start();
			}
		}
	}
	qstiIcon->setToolTip(tr("Mumble -- %1").arg(QLatin1String(MUMBLE_RELEASE)));
	AudioInput::setMaxBandwidth(-1);

	if (Global::get().s.bMinimalView) {
		qdwMinimalViewNote->show();
	}
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

void MainWindow::trayAboutToShow() {
	bool top = false;

	QPoint p = qstiIcon->geometry().center();
	if (p.isNull()) {
		p = QCursor::pos();
	}

	QScreen *screen = Mumble::Screen::screenAt(p);
	if (screen) {
		QRect qr = screen->geometry();

		if (p.y() < (qr.height() / 2))
			top = true;

		qmTray->clear();
		if (top) {
			qmTray->addAction(qaQuit);
			qmTray->addAction(qaShow);
			qmTray->addSeparator();
			qmTray->addAction(qaAudioDeaf);
			qmTray->addAction(qaAudioMute);
		} else {
			qmTray->addAction(qaAudioMute);
			qmTray->addAction(qaAudioDeaf);
			qmTray->addSeparator();
			qmTray->addAction(qaShow);
			qmTray->addAction(qaQuit);
		}
	}
}

void MainWindow::showRaiseWindow() {
	if (isMinimized()) {
		setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	}

	show();
	raise();
	activateWindow();
}

void MainWindow::on_Icon_activated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
		case QSystemTrayIcon::MiddleClick:
			if (isMinimized()) {
				showRaiseWindow();
			} else {
				showMinimized();
			}
		default:
			break;
	}
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
	updateChatBar();
}

void MainWindow::updateChatBar() {
	User *p    = pmModel->getUser(qtvUsers->currentIndex());
	Channel *c = pmModel->getChannel(qtvUsers->currentIndex());

	if (Global::get().uiSession == 0) {
		qteChat->setDefaultText(tr("<center>Not connected</center>"), true);
	} else if (!Global::get().s.bChatBarUseSelection || !p || p->uiSession == Global::get().uiSession) {
		// Channel tree target
		if (!Global::get().s.bChatBarUseSelection || !c) // If no channel selected fallback to current one
			c = ClientUser::get(Global::get().uiSession)->cChannel;

		qteChat->setDefaultText(
			tr("<center>Type message to channel '%1' here</center>").arg(c->qsName.toHtmlEscaped()));
	} else {
		// User target
		qteChat->setDefaultText(tr("<center>Type message to user '%1' here</center>").arg(p->qsName.toHtmlEscaped()));
	}

	updateMenuPermissions();
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

#ifdef QT_NO_DEBUG
#	define PROCESS_MUMBLE_TCP_MESSAGE(name, value)                                \
		case Mumble::Protocol::TCPMessageType::name: {                             \
			MumbleProto::name msg;                                                 \
			if (msg.ParseFromArray(shme->qbaMsg.constData(), shme->qbaMsg.size())) \
				msg##name(msg);                                                    \
			break;                                                                 \
		}
#else
#	define PROCESS_MUMBLE_TCP_MESSAGE(name, value)                                  \
		case Mumble::Protocol::TCPMessageType::name: {                               \
			MumbleProto::name msg;                                                   \
			if (msg.ParseFromArray(shme->qbaMsg.constData(), shme->qbaMsg.size())) { \
				printf("%s:\n", #name);                                              \
				msg.PrintDebugString();                                              \
				msg##name(msg);                                                      \
			}                                                                        \
			break;                                                                   \
		}
#endif
	switch (shme->type) { MUMBLE_ALL_TCP_MESSAGES }


#undef PROCESS_MUMBLE_TCP_MESSAGE
}


void MainWindow::on_qteLog_anchorClicked(const QUrl &url) {
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
		if (url.scheme() != QLatin1String("file") && url.scheme() != QLatin1String("qrc") && !url.isRelative())
			QDesktopServices::openUrl(url);
	}
}

void MainWindow::on_qteLog_highlighted(const QUrl &url) {
	if (url.scheme() == QString::fromLatin1("clientid") || url.scheme() == QString::fromLatin1("channelid"))
		return;

	if (!url.isValid())
		QToolTip::hideText();
	else {
		if (isActiveWindow()) {
			QToolTip::showText(QCursor::pos(), url.toString(), qteLog, QRect());
		}
	}
}

void MainWindow::context_triggered() {
	QAction *a = qobject_cast< QAction * >(sender());

	Channel *c    = pmModel->getChannel(qtvUsers->currentIndex());
	ClientUser *p = pmModel->getUser(qtvUsers->currentIndex());

	MumbleProto::ContextAction mpca;
	mpca.set_action(u8(a->data().toString()));
	if (p && p->uiSession)
		mpca.set_session(p->uiSession);
	if (c)
		mpca.set_channel_id(c->iId);
	Global::get().sh->sendMessage(mpca);
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
