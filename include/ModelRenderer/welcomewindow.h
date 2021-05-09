#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSettings>

namespace Ui {
    class WelcomeWindow;
}
class QFrame;

class WelcomeWindow : public QMainWindow {
Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

    void addToRecentFiles(QString &inputFileName);

    ~WelcomeWindow();

private:
    Ui::WelcomeWindow *ui;
    QStringList recentFilePaths;
    QStringList recentFileNames;
    QStringListModel *model;
    const int maxFileNr;
    QSettings settings;
    QImage *img = new QImage(":/Icons/kurusview.png");
    QPixmap p = QPixmap::fromImage(*img);

    QFrame *
    CreateNewRow(int number, QString title, QString subtitle, QString mouseReleaseValue, int generalFontSize = 18);


public slots:

    void handleOpenButton();

    void handleAboutButton();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WELCOMEWINDOW_H
