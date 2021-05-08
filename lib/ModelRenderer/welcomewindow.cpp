#include <QFileDialog>
#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDesktopServices>
#include <QtCore>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::WelcomeWindow),
        maxFileNr(10) {
    ui->setupUi(this);

    model = new QStringListModel(this);

    ui->recentListView->
            setModel(model);
// Connect the signal from the
    connect(ui
                    ->openPushButton, &QPushButton::released, this, &WelcomeWindow::handleOpenButton);
    connect(ui
                    ->aboutPushButton, &QPushButton::released, this, &WelcomeWindow::handleAboutButton);
}

WelcomeWindow::~WelcomeWindow() {
    delete ui;
}

void WelcomeWindow::handleOpenButton() {
    // Open the file dialog.
    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Load a Kurus View or Model"), "save_models",
                                                         tr("Model, View or STL (*.mod;*.kurus;*.stl)"));

    // If the file loaded is empty, or no file is loaded...
    if (inputFileName.isEmpty()) {
        return;
    }

    addToRecentFiles(inputFileName);
}

void WelcomeWindow::handleAboutButton() {
    QDesktopServices::openUrl(QUrl("https://github.com/KurusView/2020_GROUP_21", QUrl::TolerantMode));
}

void WelcomeWindow::addToRecentFiles(QString &inputFileName) {
    QString currFile = inputFileName;
    setWindowFilePath(currFile);

    // Remove the file just opened from the list - so there won't be any duplicates in the
    recentFilePaths.removeAll(currFile);
    // Add the file just opened to the front of the list
    recentFilePaths.prepend(currFile);
    // Remove last the last files in the list, if the list is greater than the maximum number of files.
    while (recentFilePaths.size() > maxFileNr)
        recentFilePaths.removeLast();

    // Populate the model with the data.
    model->setStringList(recentFilePaths);

    for (int i = 0; i < recentFilePaths.size(); ++i) {
        qDebug() << recentFilePaths[i];
    }
}

