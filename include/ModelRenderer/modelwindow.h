#ifndef KURUSVIEW_MODELWINDOW_H
#define KURUSVIEW_MODELWINDOW_H

#include <QMainWindow>

#include <vtkAlgorithm.h>
#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>

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

    void gridlinesInit();

private:
    Ui::ModelWindow *ui;
    // TODO Store file path in Model.h instead
    QString currentModelFilePath;
    Model currentModel;

    vtkSmartPointer<vtkDataSetMapper> mapper;
    std::vector<vtkSmartPointer<vtkAlgorithm>> filters;
    vtkSmartPointer<vtkClipDataSet> clipFilter;
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter;

    void buildChain();

    void toggleShrinkFilter(bool enable);

    void toggleClipFilter(bool enable);

public slots:

    void handleBackgroundColor();

    void handleModelColor();

    void handleResetColor();

    void handleResetLighting();

    void handleLightIntensitySlider(int position);

    void mux_handleLightActorSlider(int position);

    void handleChangePerspective();

    void updateFilters();

    void updateStructure();

    void handleGridlines();

    void toggleGridlines(bool enable);
};


#endif //KURUSVIEW_MODELWINDOW_H
