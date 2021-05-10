#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSettings>
#include <QObject.h>
#include <QPoint.h>
#include <QGridLayout>
#include <QScrollArea>

namespace Ui {
    class WelcomeWindow;
}
class QFrame;

class WelcomeWindow : public QMainWindow {
Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

    void addToRecentFiles(QStringList &inputFileNames);

    ~WelcomeWindow();

private:
    Ui::WelcomeWindow *ui;
    QStringList recentFilePaths;

    QScrollArea *scrollAreaX;
    QGridLayout *mainLayout;
    QFrame *xFrame;

    const int maxFileNr;

    QSettings settings;

    QImage *img = new QImage(":/Icons/kurusview.png");
    QPixmap p = QPixmap::fromImage(*img);

    QFrame *
    CreateNewRow(int number, QString title, QString subtitle, int generalFontSize = 18);

    void populateRecents();

    void showContextMenu(const QPoint &pos);

    void loadModel(const QStringList &paths);


public slots:

    void handleOpenButton();

    void handleAboutButton();

    void removeEntryFromRecents();

    void copyEntryToClipboard();

    void loadEntryAsModel();

signals:

    void fileSelected(const QStringList &filePaths);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WELCOMEWINDOW_H
