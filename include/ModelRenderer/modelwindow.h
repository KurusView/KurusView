#ifndef KURUSVIEW_MODELWINDOW_H
#define KURUSVIEW_MODELWINDOW_H

#include <QMainWindow>

#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>

#include "Model.h"

namespace Ui {
    class ModelWindow;
}

/**
 * @brief Visualises a model in a user-friendly GUI
 */
class ModelWindow : public QMainWindow {
Q_OBJECT

public:
    /**
     * @brief Initialises a ModelWindow and loads a Model with the filePath.
     * @param filePath
     * @param parent
     * @see Model | Model
     */
    explicit ModelWindow(const QString &filePath, QWidget *parent = nullptr);

    ~ModelWindow() override;

private:
    Ui::ModelWindow *ui;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    // TODO Store file path in Model.h instead
    QString currentModelFilePath;
    Model currentModel;

public slots:

    void handleBackgroundColor();
    void handleModelColor();
};


#endif //KURUSVIEW_MODELWINDOW_H
