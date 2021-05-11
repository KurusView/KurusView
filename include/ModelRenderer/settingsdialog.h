#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class settingsDialog;
}

class settingsDialog : public QDialog {
Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = nullptr);

    ~settingsDialog();

    static QString getDefault_saveLocation();

    static unsigned short int getDefault_maxModelsPerWindow();

    static int getDefault_displayRecents();

    static bool getDefault_alwaysOpenModelToNewWindow();

    static QColor getDefault_modelColour();

    static QColor getDefault_modelBackFaceColour();

    static QColor getDefault_backgroundColour();


private:
    Ui::settingsDialog *ui;

    QString defaultSaveLocation;

    unsigned short int maxModelsPerWindow;

    unsigned int displayRecents;

    bool alwaysOpenModelToNewWindow;

    QColor defaultModelColour, defaultBackFaceColour, defaultBackgroundColour;

private slots:

    void acceptDialog();

    void rejectDialog();

    void handleBrowseButton();

    void handleModelsPerWindowSpin(int i);

    void handleAlwaysOpenCheck();

    void handleDisplayRecentsSpin(int i);

    void handleModelColour();

    void handleModelBackfaceColour();

    void handModelBackgroundColour();

};

#endif // SETTINGSDIALOG_H
