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
#include <QColorDialog>
#include <QApplication>
#include <QScreen>
#include <vtkOrientationMarkerWidget.h>

#include "View.h"
#include "modelwindow.h"
#include "ui_modelwindow.h"

ModelWindow::ModelWindow(const QString &filePath, QWidget *parent) : QMainWindow(parent), ui(new Ui::ModelWindow),
                                                                     currentModelFilePath(filePath) {
    //TODO: Make sure the model is properly initialized before loading the window
    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);

    // Get Primary Screen Height
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();

    ui->bottomMenu->setMaximumHeight(screenHeight / 6);

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

    // Colour group slots
    connect(ui->backgroundColourPushButton, &QPushButton::released, this, &ModelWindow::handleBackgroundColor);
    connect(ui->modelColourPushButton, &QPushButton::released, this, &ModelWindow::handleModelColor);
    connect(ui->resetColoursPushButton, &QPushButton::released, this, &ModelWindow::handleResetColor);

    // Lighting group slots
    connect(ui->lightIntensitySlider, &QSlider::valueChanged, this, &ModelWindow::handleLightIntensitySlider);
    connect(ui->lightOpacitySlider, &QSlider::valueChanged, this, &ModelWindow::mux_handleLightActorSlider);
    connect(ui->lightSpecularitySlider, &QSlider::valueChanged, this, &ModelWindow::mux_handleLightActorSlider);
    connect(ui->resetLightingPushButton, &QPushButton::released, this, &ModelWindow::handleResetLighting);

    views.push_back(new View("#ff0000", "models/airbus_a400m.stl", parent));
    views.push_back(new View("#00ff00", "models/a-10-thunderbolt-mk2.stl", parent));
    views.push_back(new View("#ff00ff", "models/ExampleModel2.mod", parent));

    ui->viewFrame->addWidget(views[0], 0, 0, 1, 1);
    ui->viewFrame->addWidget(views[1], 0, 1, 2, 1);
    ui->viewFrame->addWidget(views[2], 1, 0, 1, 1);

    for (int i = 0; i < views.size(); ++i) {
        connect(views[i]->qVTKWidget, &QVTKOpenGLWidget::mouseEvent, this, &ModelWindow::viewActive);
    }

    setActiveView(views[0]);

    show();
}

ModelWindow::~ModelWindow() {
    for (int i = 0; i < views.size(); ++i) {
        delete views[i];
    }
    delete ui;
}

void ModelWindow::handleBackgroundColor() {
    // qvtkWidget would be the active view in ViewFrame

    QColor color = QColorDialog::getColor(Qt::yellow, this);
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

    if (color.isValid()) {
        activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(
                color.redF(), color.greenF(), color.blueF()
        );

        // update button color
        ui->backgroundColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
    }

    // refresh view
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleModelColor() {

    // qvtkWidget would be the active view in ViewFrame

    QColor color = QColorDialog::getColor(Qt::yellow, this);
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

    if (color.isValid()) {
        vtkActorCollection *actors = activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();

        // support multiple actors
        auto *actor = (vtkActor *) actors->GetItemAsObject(0);
        actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

        // update button color
        ui->modelColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
    }

    // refresh view
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleResetColor() {

    //reset buttons
    ui->modelColourPushButton->setStyleSheet("background-color: silver; border:none;");
    ui->backgroundColourPushButton->setStyleSheet("background-color: silver; border:none;");

    // reset background
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(
            colors->GetColor3d("Silver").GetData());

    // reset model
    auto *actors = (vtkActor *) activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetItemAsObject(
            0);
    actors->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

    // refresh view
    activeView->qVTKWidget->GetRenderWindow()->Render();
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
    col = activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetLights();

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
    activeView->qVTKWidget->GetRenderWindow()->Render();
}


void ModelWindow::mux_handleLightActorSlider(int position) {
    // find who raised the signal
    QObject *emitter = sender();

    vtkActorCollection *actors = activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();

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
    activeView->qVTKWidget->GetRenderWindow()->Render();
}


void ModelWindow::updateFilters() {
    // Check which button called this slot
    QObject *senderObj = sender();

    if (senderObj == ui->clipPushButton) {
        activeView->toggleClipFilter(ui->clipPushButton->isChecked());
    } else if (senderObj == ui->shrinkPushButton) {
        activeView->toggleShrinkFilter(ui->shrinkPushButton->isChecked());
    } else if (senderObj == ui->resetFiltersPushButton) {
        ui->clipPushButton->setChecked(false);
        ui->shrinkPushButton->setChecked(false);
        activeView->toggleClipFilter(false);
        activeView->toggleShrinkFilter(false);
    }

    // Rebuild pipeline Source -> Filters -> Mapper
    activeView->buildChain();
    // Re-render
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleChangePerspective() {
    // Check which button called this slot
    QObject *senderObj = sender();

    // Store pointer to the renderer, as it will be used multiple times
    vtkRenderer *qVTKRenderer = activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer();

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
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::viewActive(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setActiveView((View * )(((QVTKOpenGLWidget *) sender())->parentWidget()));
    }
}

void ModelWindow::setActiveView(View *newActiveView) {
    activeView = newActiveView;
    for (int i = 0; i < views.size(); ++i) {
        if (views[i] == newActiveView) {
            views[i]->setStyleSheet("background-color: #ffffff");
        } else {
            views[i]->setStyleSheet("background-color: " + views[i]->borderColor);
        }
    }
}
