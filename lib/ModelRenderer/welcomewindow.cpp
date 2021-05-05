#include "welcomewindow.h"
#include "ui_welcomewindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}
