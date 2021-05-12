#include <iostream>
#include <QFileDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QColorDialog>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::settingsDialog) {

    // bit fiddling to remove the ? button on the top right
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // setup ui
    ui->setupUi(this);

    // update members on saved defaults or fresh-run defaults.
    defaultSaveLocation = getDefault_saveLocation();
    maxModelsPerWindow = getDefault_maxModelsPerWindow();
    displayRecents = getDefault_displayRecents();
    alwaysOpenModelToNewWindow = getDefault_alwaysOpenModelToNewWindow();
    defaultModelColour = getDefault_modelColour();
    defaultBackFaceColour = getDefault_modelBackFaceColour();
    defaultBackgroundColour = getDefault_backgroundColour();

    // update ui
    ui->displayPathLabel->setText(defaultSaveLocation);
    ui->modelsPerWindowSpin->setValue(maxModelsPerWindow);
    ui->alwaysOpenOnNewWindowCheck->setChecked(alwaysOpenModelToNewWindow);
    ui->displayRecentsSpin->setValue(displayRecents);

    ui->defaultModelColour->setStyleSheet(
            "background-color: " + defaultModelColour.name() + "; border:none;"
    );
    ui->defaultModelBackfaceColour->setStyleSheet(
            "background-color: " + defaultBackFaceColour.name() + "; border:none;"
    );

    ui->defaultModelBackgroundColour->setStyleSheet(
            "background-color: " + defaultBackgroundColour.name() + "; border:none;"
    );


    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptDialog()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(rejectDialog()));

    connect(ui->alwaysOpenOnNewWindowCheck, &QCheckBox::stateChanged, this, &settingsDialog::handleAlwaysOpenCheck);
    connect(ui->browseButton, &QPushButton::released, this, &settingsDialog::handleBrowseButton);
    connect(ui->defaultModelColour, &QPushButton::released, this, &settingsDialog::handleModelColour);
    connect(ui->defaultModelBackfaceColour, &QPushButton::released, this, &settingsDialog::handleModelBackfaceColour);
    connect(ui->defaultModelBackgroundColour, &QPushButton::released, this, &settingsDialog::handModelBackgroundColour);

    // spinbox are special
    connect(ui->modelsPerWindowSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &settingsDialog::handleModelsPerWindowSpin
    );

    connect(ui->displayRecentsSpin, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &settingsDialog::handleDisplayRecentsSpin
    );

    // function pointer syntax
//    connect(ui->modelsPerWindowSpin, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i){ handleModelsPerWindowSpin(i); });
//    connect(ui->displayRecentsSpin, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i){ handleDisplayRecentsSpin(i); });
}


settingsDialog::~settingsDialog() {
    delete ui;
}

void settingsDialog::rejectDialog() {
    this->close();
}

void settingsDialog::acceptDialog() {

    // instantiate settings
    QSettings appSettings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");

    // actually save to qsettings
    appSettings.setValue("defaultSaveLocation", defaultSaveLocation);
    appSettings.setValue("maxModelsPerWindow", maxModelsPerWindow);
    appSettings.setValue("displayRecents", displayRecents);
    appSettings.setValue("alwaysOpenModelToNewWindow", alwaysOpenModelToNewWindow);
    appSettings.setValue("defaultModelColour", defaultModelColour);
    appSettings.setValue("defaultBackFaceColour", defaultBackFaceColour);
    appSettings.setValue("defaultBackgroundColour", defaultBackgroundColour);

    // update
    appSettings.sync();

    // test ====================================================================
    std::cout << "default save loc " << defaultSaveLocation.toStdString() << std::endl;
    std::cout << "max models per window " << maxModelsPerWindow << std::endl;
    std::cout << "display recents " << displayRecents << std::endl;
    std::cout << "open to new window " << alwaysOpenModelToNewWindow << std::endl;
    std::cout << "model colour " << defaultModelColour.name().toStdString() << std::endl;
    std::cout << "backFace colour " << defaultBackFaceColour.name().toStdString() << std::endl;
    std::cout << "background colour " << defaultBackgroundColour.name().toStdString() << std::endl;

    this->close();
}

void settingsDialog::handleBrowseButton() {
    // ask user for filepath
    defaultSaveLocation = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                            QDir::currentPath(),
                                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    //populate display path label text
    ui->displayPathLabel->setText(defaultSaveLocation);
}

void settingsDialog::handleModelsPerWindowSpin(int i) {
    maxModelsPerWindow = i;
}

void settingsDialog::handleAlwaysOpenCheck() {
    alwaysOpenModelToNewWindow = ui->alwaysOpenOnNewWindowCheck->isChecked();
}

void settingsDialog::handleDisplayRecentsSpin(int i) {
    displayRecents = i;
}

void settingsDialog::handleModelColour() {
    // poll for colour
    defaultModelColour = QColorDialog::getColor(Qt::yellow, this);

    // sanity check
    if (!defaultModelColour.isValid())
        return;

    // update button color
    ui->defaultModelColour->setStyleSheet("background-color: " + defaultModelColour.name() + "; border:none;");

}

void settingsDialog::handleModelBackfaceColour() {
    // poll for colour
    defaultBackFaceColour = QColorDialog::getColor(Qt::yellow, this);

    // sanity check
    if (!defaultBackFaceColour.isValid())
        return;

    // update button color
    ui->defaultModelBackfaceColour->setStyleSheet(
            "background-color: " + defaultBackFaceColour.name() + "; border:none;");
}

void settingsDialog::handModelBackgroundColour() {
    // poll for colour
    defaultBackgroundColour = QColorDialog::getColor(Qt::yellow, this);

    // sanity check
    if (!defaultBackgroundColour.isValid())
        return;

    // update button color
    ui->defaultModelBackgroundColour->setStyleSheet(
            "background-color: " + defaultBackgroundColour.name() + "; border:none;");
}


QString settingsDialog::getDefault_saveLocation() {

    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("defaultSaveLocation", QDir::currentPath()).value<QString>();
}

unsigned short int settingsDialog::getDefault_maxModelsPerWindow() {

    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("maxModelsPerWindow", 4).value<unsigned short int>();
}

int settingsDialog::getDefault_displayRecents() {

    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("displayRecents", 20).value<unsigned int>();
}

bool settingsDialog::getDefault_alwaysOpenModelToNewWindow() {

    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("alwaysOpenModelToNewWindow", false).value<bool>();
}

QColor settingsDialog::getDefault_modelColour() {

    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("defaultModelColour", QColor("rosybrown")).value<QColor>();
}

QColor settingsDialog::getDefault_modelBackFaceColour() {

    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("defaultBackFaceColour", QColor("goldenrod")).value<QColor>();
}

QColor settingsDialog::getDefault_backgroundColour() {
    // connect to settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView");
    settings.sync();

    // get all time / configured defaults.
    return settings.value("defaultBackgroundColour", QColor("seashell")).value<QColor>();
}
