#include <QWidget>
#include <QVBoxLayout>
#include <QString>

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
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <vtkCubeAxesActor.h>

#include "View.h"

// set default instance count
unsigned short int View::ViewInstanceCount = 0;

View::View(const QString &borderColor, const QString &filePath, QWidget *parent) : QWidget(parent),
                                                                                   model(filePath.toStdString()) {

    //increment instance count
    ViewInstanceCount++;

    this->borderColor = borderColor;

    setStyleSheet("*{padding: 0; border-width: 3 ;border-style:solid;border-color: dark" + this->borderColor + ";}");


    verticalLayout = new QVBoxLayout(this);

    qVTKWidget = new QVTKOpenGLWidget(parent);
    qVTKWidget->setMinimumSize(QSize(300, 200)); // TODO make this dynamic

    verticalLayout->addWidget(qVTKWidget);

    verticalLayout->setMargin(6);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qVTKWidget->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkAlgorithm> modelAlgo = model.getVTKModel();
    vtkDataObject *dataObj = modelAlgo->GetOutputDataObject(0);
    auto *datSet = (vtkDataSet *) (dataObj);

    // Initialize filters
    clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();

    // Create a mapper and actor.
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();

    mapper->SetInputData(datSet);
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // Create a renderer, and render window
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    qVTKWidget->GetRenderWindow()->AddRenderer(renderer);

    renderer->AddActor(actor);

    // Set all parameters
    // Filters
    toggleClipFilter(false);
    toggleShrinkFilter(false);

    // Colours
    setModelColor(QColor("Green"));
    setBackgroundColor(QColor("Silver"));

    // Structure
    structure = 0;
    gridLinesEnabled = false;

    // Lighting
    resetLighting();

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
    isShrunk = enable;
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
    isClipped = enable;
}

View::~View() {
    //decrement instance count
    ViewInstanceCount++;
}

unsigned short int View::getCount() {
    return View::ViewInstanceCount;
}

void View::setModelColor(const QColor &color) {
    if (!color.isValid())
        return;

    // Get Actor
    vtkActorCollection *actors = qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();
    auto *actor = (vtkActor *) actors->GetItemAsObject(0);

    // Set the Color of the actor
    actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

    // Store the color locally
    modelColor = color.name();
}

void View::setBackgroundColor(const QColor &color) {
    if (!color.isValid())
        return;

    // Get Actor
    qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(color.redF(),
                                                                                     color.greenF(),
                                                                                     color.blueF()
    );

    backgroundColour = color.name();
}

void View::setLightIntensity(int value) {
    if (value < 0 || value > 100)
        return;

    // get light sources
    vtkSmartPointer<vtkLightCollection> col = vtkSmartPointer<vtkLightCollection>::New();
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();

    col = qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetLights();

    int lightSourceCount = col->GetNumberOfItems();

    // sanity check
    if (lightSourceCount < 1) {
        return;
    }

    // update light intensity of all sources
    for (int i = 0; i < lightSourceCount; i++) {
        light = (vtkLight *) col->GetItemAsObject(i);
        light->SetIntensity((double) value / 100.0f);
    }

    lightIntensity = value;
}

void View::setLightSpecularity(int value) {
    if (value < 0 || value > 100)
        return;

    // Store pointer to all actors
    vtkActorCollection *actors = qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();

    // update all actors (.mod might have multiple)
    for (int i = 0; i < actors->GetNumberOfItems(); i++) {
        auto actor = (vtkActor *) actors->GetItemAsObject(i);
        actor->GetProperty()->SetSpecular((double) value / 100.0f);
    }

    lightSpecularity = value;
}

void View::setModelOpacity(int value) {
    if (value < 0 || value > 100)
        return;

    vtkActorCollection *actors = qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();

    // update all actors (.mod might have multiple)
    for (int i = 0; i < actors->GetNumberOfItems(); i++) {
        auto actor = (vtkActor *) actors->GetItemAsObject(i);
        actor->GetProperty()->SetOpacity((double) value / 100.0f);
    }

    modelOpacity = value;
}

void View::resetLighting() {
    setLightIntensity(75);
    setModelOpacity(100);
    setLightSpecularity(0);
}

void View::toggleGridLines(bool enable) {
    auto *cubeAxesActor = (vtkCubeAxesActor *) (qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor());

    if (enable) {
        cubeAxesActor->DrawXGridlinesOn();
        cubeAxesActor->DrawYGridlinesOn();
        cubeAxesActor->DrawZGridlinesOn();

        cubeAxesActor->XAxisLabelVisibilityOn();
        cubeAxesActor->XAxisTickVisibilityOn();
        cubeAxesActor->XAxisVisibilityOn();

        cubeAxesActor->YAxisLabelVisibilityOn();
        cubeAxesActor->YAxisTickVisibilityOn();
        cubeAxesActor->YAxisVisibilityOn();

        cubeAxesActor->ZAxisLabelVisibilityOn();
        cubeAxesActor->ZAxisTickVisibilityOn();
        cubeAxesActor->ZAxisVisibilityOn();
    } else {
        cubeAxesActor->DrawXGridlinesOff();
        cubeAxesActor->DrawYGridlinesOff();
        cubeAxesActor->DrawZGridlinesOff();

        cubeAxesActor->XAxisLabelVisibilityOff();
        cubeAxesActor->XAxisTickVisibilityOff();
        cubeAxesActor->XAxisVisibilityOff();

        cubeAxesActor->YAxisLabelVisibilityOff();
        cubeAxesActor->YAxisTickVisibilityOff();
        cubeAxesActor->YAxisVisibilityOff();

        cubeAxesActor->ZAxisLabelVisibilityOff();
        cubeAxesActor->ZAxisTickVisibilityOff();
        cubeAxesActor->ZAxisVisibilityOff();
    }

    gridLinesEnabled = enable;
    qVTKWidget->GetRenderWindow()->Render();
}

void View::setStructure(int selectedStructure){
    vtkActorCollection *actors = qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();
    auto *actor = (vtkActor *) actors->GetItemAsObject(0);
    switch (selectedStructure) {
        case 0:
            actor->GetProperty()->SetRepresentationToSurface();
            break;
        case 1:
            actor->GetProperty()->SetRepresentationToWireframe();
            break;
        case 2:
            actor->GetProperty()->SetRepresentationToPoints();
            break;
    }
    structure = selectedStructure;
}
