// MUMBLE-TFAR

#include "TFARConfig.h"

// Qt headers first — TFAR's common.hpp defines macros (snprintf, ...) that
// must not leak into them.
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <QtGui/QDesktopServices>

#include "../Global.h"
#include "../MainWindow.h"

#include "StormBranding.h"
#include "StormUpdateCheck.h"

#include "TFARBridge.h"
#include "TS3Compat.h"
#include "Teamspeak.hpp"
#include "task_force_radio.hpp"
#include "version.h"

const QString TFARConfig::name = QLatin1String("tfar");

static ConfigWidget *TFARConfigNew(Settings &st) {
    return new TFARConfig(st);
}

static ConfigRegistrar registrarTFAR(7000, TFARConfigNew);

TFARConfig::TFARConfig(Settings &st) : ConfigWidget(st) {
    auto *mainLayout = new QVBoxLayout(this);

    // ---- Storm of The Galaxy --------------------------------------------
    auto *projectGroup  = new QGroupBox(QLatin1String(STORM_PROJECT_NAME), this);
    auto *projectLayout = new QVBoxLayout(projectGroup);

    auto *projectLabel = new QLabel(
        tr("Клиент связи проекта <b>%1</b> со встроенной поддержкой TFAR.<br/>"
           "Сервер проекта: <b>%2</b> (добавлен в избранное автоматически).")
            .arg(QLatin1String(STORM_PROJECT_NAME), QLatin1String(STORM_SERVER_HOST)),
        projectGroup);
    projectLabel->setWordWrap(true);
    projectLayout->addWidget(projectLabel);

    auto *projectButtons = new QHBoxLayout();
    auto *connectButton  = new QPushButton(tr("Подключиться к серверу"), projectGroup);
    connectButton->setToolTip(QLatin1String(STORM_SERVER_URL));
    auto *guideButton = new QPushButton(tr("Гайд по настройке"), projectGroup);
    guideButton->setToolTip(QLatin1String(STORM_DOCS_URL));
    auto *updateButton = new QPushButton(tr("Проверить обновления"), projectGroup);
    projectButtons->addWidget(connectButton);
    projectButtons->addWidget(guideButton);
    projectButtons->addWidget(updateButton);
    projectButtons->addStretch(1);
    projectLayout->addLayout(projectButtons);

    m_autoUpdateCheck = new QCheckBox(tr("Автоматически скачивать и устанавливать обновления"), projectGroup);
    m_autoUpdateCheck->setToolTip(tr("При запуске клиент проверяет GitHub-релизы %1. Если найдена новая версия, "
                                     "предложит скачать и установить её. Без галочки — только сообщение в чате.")
                                      .arg(QLatin1String(STORM_GITHUB_REPO)));
    projectLayout->addWidget(m_autoUpdateCheck);
    mainLayout->addWidget(projectGroup);

    connect(connectButton, &QPushButton::clicked, this, []() {
        if (Global::get().mw)
            Global::get().mw->openUrl(QUrl(QLatin1String(STORM_SERVER_URL)));
    });
    connect(guideButton, &QPushButton::clicked, this,
            []() { QDesktopServices::openUrl(QUrl(QLatin1String(STORM_DOCS_URL))); });
    connect(updateButton, &QPushButton::clicked, this, []() { StormUpdateCheck::checkForUpdates(true); });

    // ---- status --------------------------------------------------------
    auto *statusGroup  = new QGroupBox(tr("Status"), this);
    auto *statusLayout = new QFormLayout(statusGroup);

    m_gameConnectedLabel  = new QLabel(statusGroup);
    m_inGameLabel         = new QLabel(statusGroup);
    m_pluginVersionLabel  = new QLabel(QLatin1String(PLUGIN_VERSION), statusGroup);
    m_addonVersionLabel   = new QLabel(statusGroup);
    m_nicknameLabel       = new QLabel(statusGroup);
    m_seriousChannelLabel = new QLabel(statusGroup);

    statusLayout->addRow(tr("Connected to Arma 3:"), m_gameConnectedLabel);
    statusLayout->addRow(tr("Playing (in game):"), m_inGameLabel);
    m_serverSupportLabel = new QLabel(statusGroup);
    m_serverSupportLabel->setToolTip(tr("Серверные TFAR-расширения (mumble-tfar сервер): канальная рассылка "
                                        "команд и серверный кэш состояния. Работает и без них (режим совместимости)."));
    statusLayout->addRow(tr("Server TFAR extensions:"), m_serverSupportLabel);
    statusLayout->addRow(tr("Plugin version:"), m_pluginVersionLabel);
    statusLayout->addRow(tr("Addon version:"), m_addonVersionLabel);
    statusLayout->addRow(tr("Effective nickname:"), m_nicknameLabel);
    statusLayout->addRow(tr("Serious mode channel:"), m_seriousChannelLabel);
    mainLayout->addWidget(statusGroup);

    // ---- resources ------------------------------------------------------
    auto *pathGroup  = new QGroupBox(tr("Resources"), this);
    auto *pathLayout = new QHBoxLayout(pathGroup);
    m_resourcePathEdit = new QLineEdit(pathGroup);
    m_resourcePathEdit->setToolTip(tr("Directory containing the TFAR resources (radio-sounds folder). "
                                      "Leave empty to use the \"tfar\" folder next to the Storm Voice executable."));
    auto *browseButton = new QPushButton(tr("&Browse..."), pathGroup);
    pathLayout->addWidget(new QLabel(tr("Resource path:"), pathGroup));
    pathLayout->addWidget(m_resourcePathEdit, 1);
    pathLayout->addWidget(browseButton);
    mainLayout->addWidget(pathGroup);

    // ---- debug ----------------------------------------------------------
    auto *debugGroup  = new QGroupBox(tr("Debug"), this);
    auto *debugLayout = new QHBoxLayout(debugGroup);
    auto *diagButton  = new QPushButton(tr("Diagnostic report"), debugGroup);
    diagButton->setToolTip(tr("Prints the TFAR diagnostic report into the client log (same as \"/tfar diag\")."));
    auto *posButton = new QPushButton(tr("Position report"), debugGroup);
    posButton->setToolTip(tr("Prints the position report into the client log (same as \"/tfar pos\")."));
    auto *resetButton = new QPushButton(tr("Reset radio filters"), debugGroup);
    resetButton->setToolTip(tr("Resets all radio DSP filters (same as \"/tfar rstflt\")."));
    debugLayout->addWidget(diagButton);
    debugLayout->addWidget(posButton);
    debugLayout->addWidget(resetButton);
    debugLayout->addStretch(1);
    mainLayout->addWidget(debugGroup);

    auto *hintLabel = new QLabel(
        tr("Chat commands: <b>/tfar diag</b>, <b>/tfar pos</b>, <b>/tfar full</b>, <b>/tfar rstflt</b>.<br/>"
           "Most TFAR settings (serious channel, volumes, ...) are provided by the Arma 3 mission/mod "
           "and are configured in game (TFAR addon settings)."),
        this);
    hintLabel->setWordWrap(true);
    mainLayout->addWidget(hintLabel);
    mainLayout->addStretch(1);

    connect(browseButton, &QPushButton::clicked, this, &TFARConfig::onBrowseResourcePath);
    connect(diagButton, &QPushButton::clicked, this, &TFARConfig::onDiagClicked);
    connect(posButton, &QPushButton::clicked, this, &TFARConfig::onPosClicked);
    connect(resetButton, &QPushButton::clicked, this, &TFARConfig::onResetFiltersClicked);

    m_statusTimer = new QTimer(this);
    m_statusTimer->setInterval(1000);
    connect(m_statusTimer, &QTimer::timeout, this, &TFARConfig::updateStatus);
    m_statusTimer->start();
    updateStatus();
}

QString TFARConfig::title() const {
    return tr("TFAR");
}

const QString &TFARConfig::getName() const {
    return TFARConfig::name;
}

QIcon TFARConfig::icon() const {
    return QIcon(QLatin1String("skin:config_plugin.png"));
}

void TFARConfig::load(const Settings &r) {
    Q_UNUSED(r)
    QSettings settings(QLatin1String("mumble-tfar"), QLatin1String("TFAR"));
    m_resourcePathEdit->setText(settings.value(QLatin1String("resourcePath")).toString());
    m_autoUpdateCheck->setChecked(StormUpdateCheck::autoUpdateEnabled());
}

void TFARConfig::save() const {
    QSettings settings(QLatin1String("mumble-tfar"), QLatin1String("TFAR"));
    const QString path = m_resourcePathEdit->text().trimmed();
    if (path.isEmpty())
        settings.remove(QLatin1String("resourcePath"));
    else
        settings.setValue(QLatin1String("resourcePath"), path);
    StormUpdateCheck::setAutoUpdateEnabled(m_autoUpdateCheck->isChecked());
}

void TFARConfig::updateStatus() {
    const bool gameConnected = TFARBridge::isGameConnected();
    const bool inGame        = TFARBridge::isInGame();

    m_gameConnectedLabel->setText(gameConnected ? tr("<b>Yes</b>") : tr("No"));
    m_inGameLabel->setText(inGame ? tr("<b>Yes</b>") : tr("No"));
    if (!ts3compat::isConnectedAndSynchronized())
        m_serverSupportLabel->setText(tr("(not connected)"));
    else
        m_serverSupportLabel->setText(ts3compat::serverHasTFARSupport() ? tr("<b>Yes</b>") : tr("No (compat mode)"));
    m_addonVersionLabel->setText(
        QString::fromStdString(TFAR::config.get< std::string >(Setting::addon_version)));

    const auto nickname = Teamspeak::getMyNickname(Teamspeak::getCurrentServerConnection());
    m_nicknameLabel->setText(nickname.empty() ? tr("(not connected)") : QString::fromStdString(nickname));

    const auto seriousChannel = TFAR::config.get< std::string >(Setting::serious_channelName);
    m_seriousChannelLabel->setText(seriousChannel.empty() ? tr("(none)")
                                                          : QString::fromStdString(seriousChannel));
}

void TFARConfig::onBrowseResourcePath() {
    const QString dir =
        QFileDialog::getExistingDirectory(this, tr("Select the TFAR resource directory"), m_resourcePathEdit->text());
    if (!dir.isEmpty())
        m_resourcePathEdit->setText(dir);
}

void TFARConfig::onDiagClicked() {
    ts3compat::processChatCommand("diag");
}

void TFARConfig::onPosClicked() {
    ts3compat::processChatCommand("pos");
}

void TFARConfig::onResetFiltersClicked() {
    ts3compat::processChatCommand("rstflt");
}
