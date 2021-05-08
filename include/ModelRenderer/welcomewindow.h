#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
    class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow {
Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

    ~WelcomeWindow();

private:
    Ui::WelcomeWindow *ui;

public slots:

    void handleOpenButton();

    void handleAboutButton();
};

#endif // WELCOMEWINDOW_H
