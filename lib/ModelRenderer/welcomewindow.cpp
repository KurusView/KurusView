#include <QFileDialog>
#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDesktopServices>
#include <QtCore>
#include <QLabel>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMessageBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <iostream>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::WelcomeWindow),
        maxFileNr(10),
        settings(QDir::currentPath() + "/kurusview.ini", QSettings::IniFormat) {

    ui->setupUi(this);

    populateRecents();

    // Connect the signal to the
    connect(ui->openPushButton, &QPushButton::released, this, &WelcomeWindow::handleOpenButton);
    connect(ui->aboutPushButton, &QPushButton::released, this, &WelcomeWindow::handleAboutButton);

    // Connect a double click for the listView.
    //connect(ui->recentListView, SIGNAL(QAbstractItemV), this, &WelcomWindow::);
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
    //recentFileNames.removeAll(inputFileName);
    // Add the file just opened to the front of the list
    recentFilePaths.prepend(currFile);
    recentFilePaths.prepend(inputFileName);
    // Remove last the last files in the list, if the list is greater than the maximum number of files.
    while (recentFilePaths.size() > maxFileNr) {
        recentFilePaths.removeLast();
        //recentFileNames.removeLast();
    }

    settings.setValue("recentFiles", recentFilePaths);
    settings.sync();
}

// https://stackoverflow.com/a/61043869
QFrame *WelcomeWindow::CreateNewRow(int number, QString title, QString subtitle, QString mouseReleaseValue,
                                    int generalFontSize) {
    QString styleFrame = QString("*:hover {background: GhostWhite;}");
    QString styleNumber = QString("font-size: %1px; color: #8B8D8F;").arg(qRound(generalFontSize * 0.7));
    QString styleTitle = QString("font-size: %1px; color: RosyBrown;").arg(generalFontSize);
    QString styleSubtitle = QString("font-size: %1px; color: #8B8D8F;").arg(qRound(generalFontSize * 0.7));

    auto *frame = new QFrame();
    frame->setStyleSheet(styleFrame);
    frame->setCursor(Qt::PointingHandCursor);
    frame->installEventFilter(this);
    frame->setProperty("mouseReleaseValue", mouseReleaseValue);

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

    // set object name so it can be found by removeEntryFromRecents
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
            QString prop = obj->property("mouseReleaseValue").toString();
            QMessageBox msgBox;
            msgBox.setText(QString("You have clicked an object with mouseReleaseValue: %1.").arg(prop));
            msgBox.exec();
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void WelcomeWindow::removeEntryFromRecents() {

    // vodoo to find the QLabel of the QFrame where the contextmenu was requested
    auto *pathLabel = (QLabel *) sender()->parent()->findChild<QLabel *>("FILEPATH");

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

    auto *mainLayout = new QGridLayout();

    // reserve 25
    mainLayout->setRowStretch(25, 25);

    // populate recents list
    size_t row = 0;
    for (auto &recent : recentFilePaths) {
        QFileInfo fi(recent);

        QFrame *entry = CreateNewRow(row + 1, fi.fileName(), fi.filePath(), "row_1");

        mainLayout->addWidget(entry, row, 0);

        row++;
    }

    // update frame
    ui->frame->setLayout(mainLayout);
}

void WelcomeWindow::showContextMenu(const QPoint &pos) {

    // configure context menu
    QMenu contextMenu(tr("Context menu"), (QFrame *) sender());

    // add actions TODO open, properties?
    QAction action_del("Remove Data Point", (QFrame *) sender());
    contextMenu.addAction(&action_del);

    // connect action_del signal to removeEntryFromRecents
    connect(&action_del, SIGNAL(triggered()), this, SLOT(removeEntryFromRecents()));

    // show context menu
    contextMenu.exec(QCursor::pos());
}
