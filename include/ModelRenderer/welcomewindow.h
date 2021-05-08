#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
    class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow {
Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

    void addToRecentFiles(QString &inputFileName);

    void updateRecentActionList();

    ~WelcomeWindow();

private:
    Ui::WelcomeWindow *ui;
    QStringList recentFilePaths;
    QStringListModel *model;
    const int maxFileNr;

public slots:

    void handleOpenButton();

    void handleAboutButton();
};

#endif // WELCOMEWINDOW_H
