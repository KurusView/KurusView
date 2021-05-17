#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog) {

    // setup the ui
    ui->setupUi(this);
}

Dialog::~Dialog() {
    // no other variables are allocated in the heap
    delete ui;
}
