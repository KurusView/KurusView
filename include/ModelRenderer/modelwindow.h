#ifndef KURUSVIEW_MODELWINDOW_H
#define KURUSVIEW_MODELWINDOW_H

#include <QMainWindow>

#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>

#include "Model.h"

namespace Ui {
    class ModelWindow;
}

class ModelWindow : public QMainWindow {
Q_OBJECT

public:
    explicit ModelWindow(const QString& filePath, QWidget *parent = nullptr);

    ~ModelWindow() override;

private:
    Ui::ModelWindow *ui;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    // TODO Store file path in Model.h instead
    QString currentModelFilePath;
    Model currentModel;

public slots:
};


#endif //KURUSVIEW_MODELWINDOW_H
