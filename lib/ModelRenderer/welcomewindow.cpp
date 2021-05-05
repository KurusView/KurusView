#include <QFileDialog>
#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDesktopServices>
#include <QtCore>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
    // Connect the signal from the
    connect(ui -> openPushButton, &QPushButton::released, this, &WelcomeWindow::handleOpenButton);
    connect(ui -> aboutPushButton, &QPushButton::released, this, &WelcomeWindow::handleAboutButton);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}
void WelcomeWindow::handleOpenButton() {
    // Open the file dialog.
    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Load a Kurus View or Model"), "save_models",
                                                         tr("Model or View (*.txt;*.kurus)"));

    // If the file loaded is empty, or no file is loaded...
    if (inputFileName.isEmpty()) {
        return;
    }
}

void WelcomeWindow::handleAboutButton() {
    QDesktopServices::openUrl(QUrl("https://github.com/KurusView/2020_GROUP_21", QUrl::TolerantMode));
}

