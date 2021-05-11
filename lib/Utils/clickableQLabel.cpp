#include "clickableQLabel.h"

#include <QMessageBox>

clickableQLabel::clickableQLabel(QWidget *parent, Qt::WindowFlags f)
        : QLabel(parent) {

    // redudnat, set in QT creator (for reference)
    this->setStyleSheet(
            "QLabel { border-bottom-width: 1px; border-bottom-style: solid; border-radius: 0px; border-bottom-color: #2a82da; color : #2a82da; }"
    );
}

void clickableQLabel::mousePressEvent(QMouseEvent *event) {
    QMessageBox msgBox;
    msgBox.setText(QString("Feature coming soon (vtkImplicitPlaneWidget2)"));
    msgBox.exec();
}
