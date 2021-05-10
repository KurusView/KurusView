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
        ui(new Ui::settingsDialog),
        appSettings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat) {

    ui->setupUi(this);

    appSettings.sync();

    // update members on saved defaults or fresh-run defaults.

    if (appSettings.childGroups().contains("defaultSaveLocation", Qt::CaseSensitive)) {
        defaultSaveLocation = appSettings.value("defaultSaveLocation", defaultSaveLocation).value<QString>();

        std::cout << "found save entry: ";

    } else {
        defaultSaveLocation = QDir::currentPath();

        std::cout << "did not found save entry: ";
    }
    std::cout << defaultSaveLocation.toStdString() << std::endl;

    if (appSettings.childGroups().contains("maxModelsPerWindow", Qt::CaseSensitive)) {
        maxModelsPerWindow = appSettings.value("maxModelsPerWindow", defaultSaveLocation).value<unsigned short int>();
    } else {
        maxModelsPerWindow = 4;
    }

    if (appSettings.childGroups().contains("displayRecents", Qt::CaseSensitive)) {
        displayRecents = appSettings.value("displayRecents", defaultSaveLocation).value<unsigned int>();
    } else {
        displayRecents = 20;
    }

    if (appSettings.childGroups().contains("alwaysOpenModelToNewWindow", Qt::CaseSensitive)) {
        alwaysOpenModelToNewWindow = appSettings.value("alwaysOpenModelToNewWindow", defaultSaveLocation).value<bool>();
    } else {
        alwaysOpenModelToNewWindow = false;
    }

    if (appSettings.childGroups().contains("defaultModelColour", Qt::CaseSensitive)) {
        defaultModelColour = appSettings.value("defaultModelColour", defaultSaveLocation).value<QColor>();
    } else {
        defaultModelColour = QColor("rosybrown");
    }

    if (appSettings.childGroups().contains("defaultBackFaceColour", Qt::CaseSensitive)) {
        defaultBackFaceColour = appSettings.value("defaultBackFaceColour", defaultSaveLocation).value<QColor>();
    } else {
        defaultBackFaceColour = QColor("goldenrod");
    }

    if (appSettings.childGroups().contains("defaultBackgroundColour", Qt::CaseSensitive)) {
        defaultBackgroundColour = appSettings.value("defaultBackgroundColour", defaultSaveLocation).value<QColor>();
    } else {
        defaultBackgroundColour = QColor("seashell");
    }

    // update ui
    ui->displayPathLabel->setText(defaultSaveLocation);
    ui->modelsPerWindowSpin->setValue(maxModelsPerWindow);
    ui->alwaysOpenOnNewWindowCheck->setChecked(alwaysOpenModelToNewWindow);
    ui->displayRecentsSpin->setValue(displayRecents);
    ui->defaultModelColour->setStyleSheet("background-color: " + defaultModelColour.name() + "; border:none;");
    ui->defaultModelBackfaceColour->setStyleSheet(
            "background-color: " + defaultBackFaceColour.name() + "; border:none;");
    ui->defaultModelBackgroundColour->setStyleSheet(
            "background-color: " + defaultBackgroundColour.name() + "; border:none;");


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
    appSettings.sync();

    // actually save to qsettings
    appSettings.setValue("defaultSaveLocation", defaultSaveLocation);
    appSettings.setValue("maxModelsPerWindow", maxModelsPerWindow);
    appSettings.setValue("displayRecents", displayRecents);
    appSettings.setValue("alwaysOpenModelToNewWindow", alwaysOpenModelToNewWindow);
    appSettings.setValue("defaultModelColour", defaultModelColour);
    appSettings.setValue("defaultBackFaceColour", defaultBackFaceColour);
    appSettings.setValue("defaultBackgroundColour", defaultBackgroundColour);

    appSettings.sync();

    // read value poc
    //settings.value("lastOpenDirectory","save_models").value<QString>()

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
