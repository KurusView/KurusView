#include "Model.h"
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <vtkPlane.h>
#include <vtkDataSetMapper.h>
#include <vtkCubeSource.h>
#include <QVTKOpenGLWidget.h>
#include <math.h>

#include <QFileDialog>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkDistanceWidget.h>
#include <QVTKWidget.h>
#include <QColorDialog>

#include "modelwindow.h"
#include "ui_modelwindow.h"

ModelWindow::ModelWindow(const QString &filePath, QWidget *parent) : QMainWindow(parent), ui(new Ui::ModelWindow),
                                                                     currentModel(filePath.toStdString()),
                                                                     currentModelFilePath(filePath) {
    //TODO: Make sure the model is properly initialized before loading the window
    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);

    // Camera View Button Slots
    connect(ui->resetCameraViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->xPlaneViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->xNegPlaneViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->yPlaneViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->yNegPlaneViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->zPlaneViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->zNegPlaneViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->rotate15ViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);
    connect(ui->rotate15NegViewPushButton, &QPushButton::released, this, &ModelWindow::handleChangePerspective);

    // Filter Button Slots
    connect(ui->clipPushButton, &QPushButton::released, this, &ModelWindow::updateFilters);
    connect(ui->shrinkPushButton, &QPushButton::released, this, &ModelWindow::updateFilters);
    connect(ui->resetFiltersPushButton, &QPushButton::released, this, &ModelWindow::updateFilters);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator
    ui->qvtkWidget->SetRenderWindow(renderWindow);

    // Initialize filters
    clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();

    // viewFrame is the parent class of qvtkWidget
    ui->viewFrame->setStyleSheet("*{border-width: 3 ;border-style:solid;border-color: #ff0000;}");

    //TODO: User input of filepath for both .mod and .stl and have logic to decide between each

    // Create a mapper that will hold the object's geometry in a format suitable for rendering
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();

    mapper->SetInputData(dynamic_cast<vtkDataSet *>(currentModel.getVTKModel()->GetOutputDataObject(0)));

    // Create an actor that is used to set the object's properties for rendering and place it in the window
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

    // Create a renderer, and render window
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

    //vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer); // ### ask the QtVTKOpenGLWidget for its renderWindow ###

    // Add the actor to the scene
    renderer->AddActor(actor);
    renderer->SetBackground(colors->GetColor3d("Silver").GetData());

    // Setup the renderers's camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition(5, 5, 15);
    // light->SetPositional(true);
    light->SetConeAngle(80);
    light->SetFocalPoint(0, 0, 0);
    light->SetDiffuseColor(1, 1, 1);
    light->SetAmbientColor(1, 1, 1);
    light->SetSpecularColor(1, 1, 1);
    light->SetIntensity(0.5);
    renderer->AddLight(light);

    // Source: https://www.programmersought.com/article/75456501356/
    vtkDistanceWidget *distanceWidget = vtkDistanceWidget::New();
    distanceWidget->SetInteractor(ui->qvtkWidget->GetRenderWindow()->GetInteractor());
    vtkDistanceRepresentation3D *representation = vtkDistanceRepresentation3D::New();
    distanceWidget->SetRepresentation(representation);
    distanceWidget->SetPriority(0.9);
    dynamic_cast<vtkDistanceRepresentation *> (distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.2f mm");
    distanceWidget->ManagesCursorOn();
    distanceWidget->On();
    // Rebuild pipeline Source -> Filters -> Mapper
    buildChain();
    // Re-render
    ui->qvtkWidget->GetRenderWindow()->Render();

    show();

    // colour group
    connect(ui->backgroundColourPushButton, &QPushButton::released, this, &ModelWindow::handleBackgroundColor);
    connect(ui->modelColourPushButton, &QPushButton::released, this, &ModelWindow::handleModelColor);
    connect(ui->resetColoursPushButton, &QPushButton::released, this, &ModelWindow::handleResetColor);

    // lightning group
    connect(ui->lightIntensitySlider, &QSlider::valueChanged, this, &ModelWindow::handleLightIntensitySlider);
    connect(ui->lightOpacitySlider, &QSlider::valueChanged, this, &ModelWindow::mux_handleLightActorSlider);
    connect(ui->lightSpecularitySlider, &QSlider::valueChanged, this, &ModelWindow::mux_handleLightActorSlider);
    connect(ui->resetLightingPushButton, &QPushButton::released, this, &ModelWindow::handleResetLighting);
}

ModelWindow::~ModelWindow() {
    delete ui;
}

void ModelWindow::handleBackgroundColor() {
    // qvtkWidget would be the active view in ViewFrame

    QColor color = QColorDialog::getColor(Qt::yellow, this);
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

    if (color.isValid()) {
        ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(
                color.redF(), color.greenF(), color.blueF()
        );

        // update button color
        ui->backgroundColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
    }

    // refresh view
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleModelColor() {

    // qvtkWidget would be the active view in ViewFrame

    QColor color = QColorDialog::getColor(Qt::yellow, this);
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

    if (color.isValid()) {
        vtkActorCollection *actors = ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();

        // support multiple actors
        auto *actor = (vtkActor *) actors->GetItemAsObject(0);
        actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

        // update button color
        ui->modelColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
    }

    // refresh view
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleResetColor() {

    //reset buttons
    ui->modelColourPushButton->setStyleSheet("background-color: silver; border:none;");
    ui->backgroundColourPushButton->setStyleSheet("background-color: silver; border:none;");

    // reset background
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(
            colors->GetColor3d("Silver").GetData());

    // reset model
    auto *actors = (vtkActor *) ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetItemAsObject(
            0);
    actors->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

    // refresh view
    ui->qvtkWidget->GetRenderWindow()->Render();
}


void ModelWindow::handleResetLighting() {
    ui->lightIntensitySlider->setValue(50);
    ui->lightOpacitySlider->setValue(100);
    ui->lightSpecularitySlider->setValue(0);
}

void ModelWindow::handleLightIntensitySlider(int position) {

    // get light sources
    vtkSmartPointer<vtkLightCollection> col = vtkSmartPointer<vtkLightCollection>::New();
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    col = ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetLights();

    int lightSourceCount = col->GetNumberOfItems();

    // sanity check
    if (lightSourceCount < 1) {
        return;
    }

    // update light intensity of all sources
    for (int i = 0; i < lightSourceCount; i++) {
        light = (vtkLight *) col->GetItemAsObject(i);
        light->SetIntensity((double) position / 100.0f);
    }

    // refresh
    ui->qvtkWidget->GetRenderWindow()->Render();
}


void ModelWindow::mux_handleLightActorSlider(int position) {
    // find who raised the signal
    QObject *emitter = sender();

    vtkActorCollection *actors = ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();

    // update all actors (.mod might have multiple)
    for (int i = 0; i < actors->GetNumberOfItems(); i++) {
        auto actor = (vtkActor *) actors->GetItemAsObject(i);

        if (emitter == ui->lightSpecularitySlider) {
            actor->GetProperty()->SetSpecular((double) position / 100.0f);
        } else { // opacity
            actor->GetProperty()->SetOpacity((double) position / 100.0f);
        }
    }

    // refresh
    ui->qvtkWidget->GetRenderWindow()->Render();
}


void ModelWindow::updateFilters() {
    // Check which button called this slot
    QObject *senderObj = sender();

    if (senderObj == ui->clipPushButton){
        toggleClipFilter(ui->clipPushButton->isChecked());
    } else if(senderObj == ui->shrinkPushButton){
        toggleShrinkFilter(ui->shrinkPushButton->isChecked());
    } else if (senderObj == ui->resetFiltersPushButton) {
        ui->clipPushButton->setChecked(false);
        ui->shrinkPushButton->setChecked(false);
        toggleClipFilter(false);
        toggleShrinkFilter(false);
    }

    // Rebuild pipeline Source -> Filters -> Mapper
    buildChain();
    // Re-render
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void ModelWindow::buildChain() {
    // Store the source object, as it's used multiple times
    vtkDataSet *source = dynamic_cast<vtkDataSet *>(currentModel.getVTKModel()->GetOutputDataObject(0));
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

void ModelWindow::toggleShrinkFilter(bool enable){
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

void ModelWindow::toggleClipFilter(bool enable) {
    if (enable) {
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(-1.0, 0.0, 0.0);
        clipFilter->SetClipFunction(planeLeft.Get());
        // Insert at the end of the filter list
        filters.emplace_back(clipFilter);
    } else if (std::find(filters.begin(), filters.end(), clipFilter) != filters.end()){
        // Remove the shrink filter from the filter list
        filters.erase(std::find(filters.begin(), filters.end(), clipFilter));
    }
}

void ModelWindow::handleChangePerspective() {
    // Check which button called this slot
    QObject *senderObj = sender();

    // Store pointer to the renderer, as it will be used multiple times
    vtkRenderer *qVTKRenderer = ui->qvtkWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer();

    // Rotation does not require resetting the camera, so first check if rotation was requested
    if (senderObj == ui->rotate15ViewPushButton) {
        // Rotate 15 degrees to the right
        qVTKRenderer->GetActiveCamera()->Azimuth(15);
    } else if (senderObj == ui->rotate15NegViewPushButton) {
        // Rotate 15 degrees to the left
        qVTKRenderer->GetActiveCamera()->Azimuth(-15);
    } else {
        // Resets the camera to point to the center of the actors and moves it so all actors can be seen
        qVTKRenderer->ResetCamera();

        // Calculate the distance from the focal point
        double *focalPoint = qVTKRenderer->GetActiveCamera()->GetFocalPoint();
        double *position = qVTKRenderer->GetActiveCamera()->GetPosition();
        double distance = std::sqrt(std::pow(position[0] - focalPoint[0], 2)
                                    + std::pow(position[1] - focalPoint[1], 2)
                                    + std::pow(position[2] - focalPoint[2], 2));

        if (senderObj == ui->resetCameraViewPushButton) {
            // Set position and view to initial values based on focal point and distance
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0], focalPoint[1], focalPoint[2] + distance);
            qVTKRenderer->GetActiveCamera()->SetViewUp(0.0, 1.0, 0.0);
        } else if (senderObj == ui->xPlaneViewPushButton) {
            // Move in the positive X direction
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0] + distance, focalPoint[1], focalPoint[2]);
            // Set Up to +Z Axis
            qVTKRenderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
        } else if (senderObj == ui->xNegPlaneViewPushButton) {
            // Move in the negative X direction
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0] - distance, focalPoint[1], focalPoint[2]);
            // Set Up to +Z Axis
            qVTKRenderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
        } else if (senderObj == ui->yPlaneViewPushButton) {
            // Move in the positive Y direction
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0], focalPoint[1] + distance, focalPoint[2]);
            // Set Up to +Z Axis
            qVTKRenderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
        } else if (senderObj == ui->yNegPlaneViewPushButton) {
            // Move in the negative Y direction
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0], focalPoint[1] - distance, focalPoint[2]);
            // Set Up to +Z Axis
            qVTKRenderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
        } else if (senderObj == ui->zPlaneViewPushButton) {
            // Move in the positive Z direction
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0], focalPoint[1], focalPoint[2] + distance);
            // Set Up to +X Axis
            qVTKRenderer->GetActiveCamera()->SetViewUp(1.0, 0.0, 0.0);
        } else if (senderObj == ui->zNegPlaneViewPushButton) {
            // Move in the negative Z direction
            qVTKRenderer->GetActiveCamera()->SetPosition(focalPoint[0], focalPoint[1], focalPoint[2] - distance);
            // Set Up to +X Axis
            qVTKRenderer->GetActiveCamera()->SetViewUp(1.0, 0.0, 0.0);
        }

        // Resets the camera clipping range based on the bounds of the visible actors
        qVTKRenderer->ResetCameraClippingRange();
    }
    // Re-render the model
    ui->qvtkWidget->GetRenderWindow()->Render();
}