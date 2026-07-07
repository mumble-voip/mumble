// MUMBLE-TFAR
// "TFAR" page in Mumble's configuration dialog: connection status,
// resource path and debug helpers.

#ifndef MUMBLE_MUMBLE_TFAR_TFARCONFIG_H_
#define MUMBLE_MUMBLE_TFAR_TFARCONFIG_H_

#include "../ConfigWidget.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QTimer;

class TFARConfig : public ConfigWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TFARConfig)

public:
    /// The unique name of this ConfigWidget
    static const QString name;

    explicit TFARConfig(Settings &st);

    QString title() const override;
    const QString &getName() const override;
    QIcon icon() const override;

    void load(const Settings &r) override;
    void save() const override;

private slots:
    void updateStatus();
    void onBrowseResourcePath();
    void onDiagClicked();
    void onPosClicked();
    void onResetFiltersClicked();

private:
    QCheckBox *m_autoUpdateCheck   = nullptr;
    QLabel *m_gameConnectedLabel   = nullptr;
    QLabel *m_serverSupportLabel   = nullptr;
    QLabel *m_inGameLabel          = nullptr;
    QLabel *m_pluginVersionLabel   = nullptr;
    QLabel *m_addonVersionLabel    = nullptr;
    QLabel *m_nicknameLabel        = nullptr;
    QLabel *m_seriousChannelLabel  = nullptr;
    QLineEdit *m_resourcePathEdit  = nullptr;
    QTimer *m_statusTimer          = nullptr;
};

#endif // MUMBLE_MUMBLE_TFAR_TFARCONFIG_H_
