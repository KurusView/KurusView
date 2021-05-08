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

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator
    ui->qvtkWidget->SetRenderWindow(renderWindow);



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

    show();
}

ModelWindow::~ModelWindow() {
    delete ui;
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