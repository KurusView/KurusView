#ifndef KURUSVIEW_MAINWINDOW_H
#define KURUSVIEW_MAINWINDOW_H

#include <QMainWindow>

#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkDataSetMapper> mapper;

public slots:
};


#endif //KURUSVIEW_MAINWINDOW_H
