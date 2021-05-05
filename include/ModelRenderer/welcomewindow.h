#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QDialog>

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private:
    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
