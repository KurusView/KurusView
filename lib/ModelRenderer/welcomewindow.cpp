#include <QFileDialog>
#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDesktopServices>
#include <QtCore>
#include <QLabel>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMessageBox>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::WelcomeWindow),
        maxFileNr(10),
        settings(QDir::currentPath() + "/kurusview.ini", QSettings::IniFormat) {
    ui->setupUi(this);

//    std::vector<QFrame*> row;
////    int numberOfFiles = recentFilePaths.size();
//    for (int i = 0; i <recentFilePaths.size(); i++){
//        row.push_back(CreateNewRow((i+1), "First row", recentFilePaths[i], "row_1"));
//        mainLayout->addWidget(row[i], i, 0);
//    }


    settings.sync();
    recentFilePaths = settings.value("recentFiles").value<QStringList>();



    QGridLayout *mainLayout = new QGridLayout();

    // reserve 25
    mainLayout->setRowStretch(25, 25);

    size_t row = 0;
    for (auto &recent : recentFilePaths) {
        QFrame *entry = CreateNewRow(row +1, "file name", recent, "row_1");

        mainLayout->addWidget(entry, row, 0);

        row++;
    }

    //QFrame *row1 = CreateNewRow(1, "First row", "First subtitle row", "row_1");
    //QFrame *row2 = CreateNewRow(2, "Second row", "Second subtitle row", "row_2");

    // All rows no stretch
    // Not sure about the magic numbers, but it works..
//    mainLayout->addWidget(row1, 0, 0);
//    mainLayout->addWidget(row2, 1, 0);


//    model = new QStringListModel(this);
//
//    ui->recentListView->setModel(model);

    // Connect the signal to the
    connect(ui->openPushButton, &QPushButton::released, this, &WelcomeWindow::handleOpenButton);
    connect(ui->aboutPushButton, &QPushButton::released, this, &WelcomeWindow::handleAboutButton);
    // Connect a double click for the listView.
//    connect(ui->recentListView, SIGNAL(QAbstractItemV), this, &WelcomWindow::);

//    settings.sync();
//    recentFilePaths = settings.value("recentFiles").value<QStringList>();
//    model->setStringList(recentFilePaths);
    ui->frame->setLayout(mainLayout);
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
    recentFileNames. removeAll(inputFileName);
    // Add the file just opened to the front of the list
    recentFilePaths.prepend(currFile);
    recentFilePaths.prepend(inputFileName);
    // Remove last the last files in the list, if the list is greater than the maximum number of files.
    while (recentFilePaths.size() > maxFileNr){
        recentFilePaths.removeLast();
        recentFileNames.removeLast();
    }


    settings.setValue("recentFiles", recentFilePaths);
    settings.sync();
}

QFrame* WelcomeWindow::CreateNewRow(int number, QString title, QString subtitle, QString mouseReleaseValue, int generalFontSize)
{
    QString styleFrame = QString("*:hover {background: #F6F6F6;}");
    QString styleNumber = QString("font-size: %1px; color: #8B8D8F;").arg(qRound(generalFontSize * 0.7));
    QString styleTitle = QString("font-size: %1px; color: #5CAA15;").arg(generalFontSize);
    QString styleSubtitle = QString("font-size: %1px; color: #8B8D8F;").arg(qRound(generalFontSize * 0.7));

    QFrame *frame = new QFrame();
    frame->setStyleSheet(styleFrame);
    frame->setCursor(Qt::PointingHandCursor);
    frame->installEventFilter(this);
    frame->setProperty("mouseReleaseValue", mouseReleaseValue);

    QLabel *imgDisplayLabel = new QLabel("");
    imgDisplayLabel->setPixmap(p.scaled(generalFontSize, generalFontSize, Qt::KeepAspectRatio));
    imgDisplayLabel->adjustSize();
    imgDisplayLabel->setContentsMargins(5, 0, 10, 0);

    QGridLayout *layout= new QGridLayout(frame);
    // All rows no stretch
    layout->setColumnStretch(25, 25);

    QLabel *labelNumber = new QLabel(QString::number(number));
    QLabel *labelTitle = new QLabel(title);
    QLabel *labelSubtitle = new QLabel(subtitle);

    labelNumber->setStyleSheet(styleNumber);
    labelTitle->setStyleSheet(styleTitle);
    labelSubtitle->setStyleSheet(styleSubtitle);

    layout->addWidget(labelNumber, 0, 0);
    layout->addWidget(imgDisplayLabel, 0, 1, Qt::AlignVCenter);
    layout->addWidget(labelTitle, 0, 2);
    layout->addWidget(labelSubtitle, 1, 2);

    return frame;
}

bool WelcomeWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (event);
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