#include <QWidget>
#include <QVBoxLayout>

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkDataSet.h>
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkAlgorithm.h>
#include <vtkCallbackCommand.h>
#include <vtkPlane.h>

#include "View.h"

View::View(const QString &borderColor, const QString &filePath, QWidget *parent) : QWidget(parent),
                                                                                   model(filePath.toStdString()) {
    this->borderColor = borderColor;
    // Setup layout
//    setFrameShape(QFrame::StyledPanel);
//    setFrameShadow(QFrame::Raised);
    setStyleSheet("background-color: " + this->borderColor);

    verticalLayout = new QVBoxLayout(this);

    qVTKWidget = new QVTKOpenGLWidget(parent);
    qVTKWidget->setMinimumSize(QSize(600, 400));

//    setLayout(verticalLayout);
    verticalLayout->addWidget(qVTKWidget);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qVTKWidget->SetRenderWindow(renderWindow);

    vtkNew<vtkNamedColors> colors;

    vtkSmartPointer<vtkAlgorithm> modelAlgo = model.getVTKModel();
    vtkDataObject *dataObj = modelAlgo->GetOutputDataObject(0);
    vtkDataSet *datSet = (vtkDataSet *) (dataObj);

    // Initialize filters
    clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();

    // Create a mapper and actor.
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();

    mapper->SetInputData(datSet);
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d("Green").GetData());

    // Create a renderer, and render window
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    qVTKWidget->GetRenderWindow()->AddRenderer(renderer);

    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("Silver").GetData());

    qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
    qVTKWidget->GetRenderWindow()->Render();
}

void View::buildChain() {
    // Store the source object, as it's used multiple times
    vtkDataSet *source = dynamic_cast<vtkDataSet *>(model.getVTKModel()->GetOutputDataObject(0));
    // If no filters are applied
    if (filters.empty()) {
        // Source -> Mapper
        mapper->SetInputData(source);
        return;
    }
    // Otherwise Source -> Filter 0
    filters[0]->SetInputDataObject(source);
    filters[0]->Update();
    if (filters.size() == 1) {
        // Otherwise Filter 0 -> Mapper
        mapper->SetInputData(dynamic_cast<vtkDataSet *>(filters[0]->GetOutputDataObject(0)));
        return;
    }

    // Otherwise Filter 0 -> Filter 1
    filters[1]->SetInputDataObject(dynamic_cast<vtkDataSet *>(filters[0]->GetOutputDataObject(0)));
    filters[1]->Update();
    // Otherwise Filter 1 -> Mapper
    mapper->SetInputData(dynamic_cast<vtkDataSet *>(filters[1]->GetOutputDataObject(0)));
}

void View::toggleShrinkFilter(bool enable) {
    if (enable) {
        // TODO: Dialog Box asking for shrink value
        // Shrinks to 50%
        shrinkFilter->SetShrinkFactor(0.5);
        // Insert at the end of the filter list
        filters.emplace_back(shrinkFilter);
    } else if (std::find(filters.begin(), filters.end(), shrinkFilter) != filters.end()) {
        // Remove the shrink filter from the filter list
        filters.erase(std::find(filters.begin(), filters.end(), shrinkFilter));
    }
}

void View::toggleClipFilter(bool enable) {
    if (enable) {
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(-1.0, 0.0, 0.0);
        clipFilter->SetClipFunction(planeLeft.Get());
        // Insert at the end of the filter list
        filters.emplace_back(clipFilter);
    } else if (std::find(filters.begin(), filters.end(), clipFilter) != filters.end()) {
        // Remove the shrink filter from the filter list
        filters.erase(std::find(filters.begin(), filters.end(), clipFilter));
    }
}