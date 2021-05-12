#include <QFileDialog>
#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include "settingsdialog.h"
#include <QDesktopServices>
#include <QtCore>
#include <QLabel>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtAlgorithms>
#include <QClipboard>
#include <QScrollBar>


#include <iostream>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::WelcomeWindow),
        maxFileNr(settingsDialog::getDefault_displayRecents()),
        settings(QSettings::IniFormat, QSettings::UserScope, "Kurus", "KurusView") {

    ui->setupUi(this);
    img = new QImage(":/Icons/kurusview.png");
    p = QPixmap::fromImage(*img);

    std::cout << maxFileNr;

    this->setWindowTitle("Welcome to KurusView");

    xFrame = new QFrame();

    populateRecents();

    // Connect the signals to slots
    connect(ui->openPushButton, &QPushButton::released, this, &WelcomeWindow::handleOpenButton);
    connect(ui->aboutPushButton, &QPushButton::released, this, &WelcomeWindow::handleAboutButton);
    connect(ui->settingsPushButton, &QPushButton::released, this, &WelcomeWindow::handleSettingsButton);
}

WelcomeWindow::~WelcomeWindow() {
    delete ui;
}

void WelcomeWindow::handleOpenButton() {
    // Sync to make sure we have most recent lastOpenDirectory
    settings.sync();

    // Open the file dialog in the last directory, defaulting to save_models
    QStringList inputFileNames = QFileDialog::getOpenFileNames(this, tr("Load a Kurus View or Model"),
                                                               settings.value("lastOpenDirectory",
                                                                              "save_models").value<QString>(),
                                                               tr("Model, View or STL (*.mod;*.kurus;*.stl)"));

    // If the file loaded is empty, or no file is loaded...
    if (inputFileNames.isEmpty()) {
        return;
    }

    // Update lastOpenDirectory in settings and sync
    settings.setValue("lastOpenDirectory", QFileInfo(inputFileNames.at(0)).absoluteDir().absolutePath());
    settings.sync();

    // update recents for next time
    addToRecentFiles(inputFileNames);

    // uneecessary as this window is inmediatly closed. helps with debugging
    populateRecents();

    // open model
    loadModel(inputFileNames);
}

void WelcomeWindow::handleAboutButton() {
    QDesktopServices::openUrl(QUrl("https://github.com/KurusView/2020_GROUP_21", QUrl::TolerantMode));
}

void WelcomeWindow::addToRecentFiles(QStringList &inputFileNames) {
    for (auto currFile: inputFileNames) {
        setWindowFilePath(currFile);

        // Remove the file just opened from the list - so there won't be any duplicates in the
        recentFilePaths.removeAll(currFile);

        // Add the file just opened to the front of the list
        recentFilePaths.prepend(currFile);
    }

    // Remove last the last files in the list, if the list is greater than the maximum number of files.
    while (recentFilePaths.size() > maxFileNr) {
        recentFilePaths.removeLast();
    }

    settings.setValue("recentFiles", recentFilePaths);
    settings.sync();
}

// https://stackoverflow.com/a/61043869
QFrame *WelcomeWindow::CreateNewRow(int number, QString title, QString subtitle, int generalFontSize) {
    QString styleFrame = QString("*:hover {background: GhostWhite;}");
    QString styleNumber = QString("font-size: %1px; color: #8B8D8F;").arg(qRound(generalFontSize * 0.7));
    QString styleTitle = QString("font-size: %1px; color: RosyBrown;").arg(generalFontSize);
    QString styleSubtitle = QString("font-size: %1px; color: #8B8D8F;").arg(qRound(generalFontSize * 0.7));

    auto *frame = new QFrame();
    frame->setStyleSheet(styleFrame);
    frame->setCursor(Qt::PointingHandCursor);
    frame->installEventFilter(this);

    // make variables conveniently accessible to external event handlers
    frame->setProperty("FILEPATH", subtitle);


    auto *imgDisplayLabel = new QLabel("");
    imgDisplayLabel->setPixmap(p.scaled(generalFontSize * 2.5, generalFontSize * 2.5, Qt::KeepAspectRatio));
    imgDisplayLabel->adjustSize();
    imgDisplayLabel->setContentsMargins(5, 0, 10, 0);

    auto *layout = new QGridLayout(frame);
    // All rows no stretch
    layout->setColumnStretch(10, 10);
    layout->setHorizontalSpacing(0);
    layout->setSpacing(0);
    layout->setVerticalSpacing(0);
    //layout->setMargin(0);

    auto *labelNumber = new QLabel(QString::number(number));
    auto *labelTitle = new QLabel(title);
    auto *labelSubtitle = new QLabel(subtitle);

    // set object name so it can be found by removeEntryFromRecents and openModel
    labelSubtitle->setObjectName("FILEPATH");

    labelNumber->setStyleSheet(styleNumber);
    labelTitle->setStyleSheet(styleTitle);
    labelSubtitle->setStyleSheet(styleSubtitle);

    layout->addWidget(labelNumber, 0, 0);
    layout->addWidget(imgDisplayLabel, 0, 1, Qt::AlignVCenter);
    layout->addWidget(labelTitle, 0, 2);
    layout->addWidget(labelSubtitle, 1, 2);

    // make frame the sender of the customContextMenuRequested signal
    frame->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(frame, &QFrame::customContextMenuRequested, this, &WelcomeWindow::showContextMenu);

    return frame;
}

bool WelcomeWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        auto *mouseEvent = static_cast<QMouseEvent *> (event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QString modelPath = obj->property("FILEPATH").toString();

            QStringList modelPathList;
            modelPathList << modelPath;
            addToRecentFiles(modelPathList);
            // open model
            loadModel(modelPathList);

//            QMessageBox msgBox;
//            msgBox.setText(QString("You have clicked an object with mouseReleaseValue: %1.").arg(prop));
//            msgBox.exec();

            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void WelcomeWindow::removeEntryFromRecents() {

    // vodoo to find the QLabel of the QFrame where the contextmenu was requested
    auto *pathLabel = (QLabel *) sender()->parent()->findChild<QLabel *>("FILEPATH");

    // debug
    std::cout << pathLabel->text().toStdString() << std::endl;

    // remove the filepath from recents list
    recentFilePaths.removeAll(pathLabel->text());

    // update storage with recents list
    settings.setValue("recentFiles", recentFilePaths);
    settings.sync();

    // refresh ui list
    populateRecents();
}

void WelcomeWindow::populateRecents() {
    settings.sync();
    recentFilePaths = settings.value("recentFiles").value<QStringList>();


    // Update layout: Cant set a layout on a widget that already has a layout, see https://forum.qt.io/topic/14898/howto-change-layout-of-a-widget/7
    if (xFrame->layout()) {

        QLayoutItem *item;
        while ((item = mainLayout->takeAt(0)) != 0)
            if (item->widget()) {
                item->widget()->setParent(NULL);
                delete item;
            }

        // need to delete the layout
        delete mainLayout;
    }


    mainLayout = new QGridLayout();

    // reserve 25
    mainLayout->setRowStretch(25, 25);

    // populate recents list
    size_t row = 0;
    for (auto &recent : recentFilePaths) {
        QFileInfo fi(recent);

        QFrame *entry = CreateNewRow(row + 1, fi.fileName(), fi.filePath());

        mainLayout->addWidget(entry, row, 0);

        row++;
    }

    // update frame
    //ui->frame->setLayout(mainLayout);

    xFrame->setLayout(mainLayout);

//    auto *imageLabel = new QLabel;
//    imageLabel->setPixmap(p);

    ui->scrollArea->setWidget(xFrame);

    // disable and hide horizontal scrollbars (need to be disabled for touchscreens)
    ui->scrollArea->horizontalScrollBar()->setEnabled(false);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // fit frame horizontally and some extra padding
    ui->scrollArea->setMinimumWidth(xFrame->width() + 10);
    ui->scrollArea->setMinimumHeight(xFrame->width() / 2);
}

void WelcomeWindow::showContextMenu(const QPoint &pos) {

    // configure context menu
    QMenu contextMenu(tr("Context menu"), (QFrame *) sender());

    QAction action_del("Remove from Recents", (QFrame *) sender());
    QAction action_opn("Open Model", (QFrame *) sender());
    QAction action_cpy("Copy Path", (QFrame *) sender());

    contextMenu.addAction(&action_del);
    contextMenu.addAction(&action_opn);
    contextMenu.addAction(&action_cpy);

    // connect action_del signal to removeEntryFromRecents
    connect(&action_del, SIGNAL(triggered()), this, SLOT(removeEntryFromRecents()));
    connect(&action_cpy, SIGNAL(triggered()), this, SLOT(copyEntryToClipboard()));
    connect(&action_opn, SIGNAL(triggered()), this, SLOT(loadEntryAsModel()));

    // show context menu
    contextMenu.exec(QCursor::pos());
}


void WelcomeWindow::loadModel(const QStringList &paths) {
    emit fileSelected(paths);
}

void WelcomeWindow::copyEntryToClipboard() {
    // find the text of the QLabel of the QFrame where the contextmenu was requested
    QString pathLabel = sender()->parent()->findChild<QLabel *>("FILEPATH")->text();

    // paste it to clipboard
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(pathLabel);
}

void WelcomeWindow::loadEntryAsModel() {
    // find the text of the QLabel of the QFrame where the contextmenu was requested
    QString pathLabel = sender()->parent()->findChild<QLabel *>("FILEPATH")->text();

    // load the entry
    loadModel((QStringList) pathLabel);
}

void WelcomeWindow::handleSettingsButton() {
    settingsDialog settings;

    settings.exec();
}
