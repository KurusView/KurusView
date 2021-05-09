#include "Model.h"
#include "MVector.h"
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
#include <QApplication>
#include <QScreen>
#include <vtkOrientationMarkerWidget.h>

#include "View.h"
#include "modelwindow.h"
#include "ui_modelwindow.h"
#include <vtkCubeAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

#include <vtkActorCollection.h>
#include <vtkColor.h>
#include <vtkCubeAxesActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextProperty.h>
#include <vtkTriangleFilter.h>
#include <vtkMassProperties.h>
#include <vtkCenterOfMass.h>


ModelWindow::ModelWindow(const QString &filePath, QWidget *parent) : QMainWindow(parent), ui(new Ui::ModelWindow),
                                                                     currentModelFilePath(filePath) {
    //TODO: Make sure the model is properly initialized before loading the window
    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);
    // Get Primary Screen Height
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();

    ui->bottomMenu->setMaximumHeight(screenHeight / 5);

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

    // Structure Button Slots
    connect(ui->wireframeStructRadioButton, &QRadioButton::toggled, this, &ModelWindow::updateStructure);
    connect(ui->pointsStructRadioButton, &QRadioButton::toggled, this, &ModelWindow::updateStructure);
    connect(ui->normalStructRadioButton, &QRadioButton::toggled, this, &ModelWindow::updateStructure);
    connect(ui->gridLinesCheckBox, &QCheckBox::stateChanged, this, &ModelWindow::handleGridlines);

    // Lighting group slots
    connect(ui->lightIntensitySlider, &QSlider::valueChanged, this, &ModelWindow::handleLightIntensitySlider);
    connect(ui->lightOpacitySlider, &QSlider::valueChanged, this, &ModelWindow::mux_handleLightActorSlider);
    connect(ui->lightSpecularitySlider, &QSlider::valueChanged, this, &ModelWindow::mux_handleLightActorSlider);
    connect(ui->resetLightingPushButton, &QPushButton::released, this, &ModelWindow::handleResetLighting);

    //Measurement button
    connect(ui->measurementButton, &QPushButton::released, this, &ModelWindow::handleMeasurment);

    addViewToFrame(new View("red", "models/airbus_a400m.stl", parent));
    addViewToFrame(new View("blue", "models/a-10-thunderbolt-mk2.stl", parent));
    addViewToFrame(new View("cyan", "models/ExampleModel2.mod", parent));
    addViewToFrame(new View("magenta", "models/ExampleModel3.mod", parent));

    for (auto &view : views) {
        setActiveView(view);
        connect(view->qVTKWidget, &QVTKOpenGLWidget::mouseEvent, this, &ModelWindow::viewActive);
        gridlinesInit(view);
        handleResetLighting();
    }

    setActiveView(views[0]);

    std::cout << View::getCount();

    show();
}

ModelWindow::~ModelWindow() {
    for (auto &view : views) {
        delete view;
    }
    delete ui;
}

void ModelWindow::handleBackgroundColor() {
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if (!color.isValid())
        return;

    activeView->setBackgroundColor(color);

    // update button color
    ui->backgroundColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
    activeView->backgroundColour = color.name();

    // refresh view
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleModelColor() {
    QColor color = QColorDialog::getColor(Qt::yellow, this);

    if (!color.isValid())
        return;
    activeView->setModelColor(color);
    // update button color
    ui->modelColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
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
    activeView->resetLighting();
    ui->lightIntensitySlider->setValue(activeView->lightIntensity);
    ui->lightOpacitySlider->setValue(activeView->modelOpacity);
    ui->lightSpecularitySlider->setValue(activeView->lightSpecularity);
}

void ModelWindow::handleLightIntensitySlider(int position) {
    // Set the light intensity
    activeView->setLightIntensity(position);
    // refresh
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::mux_handleLightActorSlider(int position) {
    // find who raised the signal
    QObject *emitter = sender();

    if (emitter == ui->lightSpecularitySlider)
        activeView->setLightSpecularity(position);
    else // opacity
        activeView->setModelOpacity(position);

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
        setActiveView((View *) (((QVTKOpenGLWidget *) sender())->parentWidget()));
    }
}

void ModelWindow::setActiveView(View *newActiveView) {
    activeView = newActiveView;
    for (auto &view : views) {
        if (view == newActiveView) {
            view->setStyleSheet(
                    "*{padding: 0; border-width: 3 ;border-style:solid;border-color: " + view->borderColor + ";}");
        } else {
            view->setStyleSheet(
                    "*{padding: 0; border-width: 3 ;border-style:solid;border-color: dark" + view->borderColor + ";}");

        }
    }
    // Change button and slider states to match view state
    // Filters
    ui->clipPushButton->setChecked(activeView->isClipped);
    ui->shrinkPushButton->setChecked(activeView->isShrunk);

    // Colours
    ui->backgroundColourPushButton->setStyleSheet(
            "background-color: " + activeView->backgroundColour + "; border:none;");
    ui->modelColourPushButton->setStyleSheet(
            "background-color: " + activeView->modelColor + "; border:none;");

    // Structure
    switch (activeView->structure) {
        case 0:
            ui->normalStructRadioButton->click();
            break;
        case 1:
            ui->wireframeStructRadioButton->click();
            break;
        case 2:
            ui->pointsStructRadioButton->click();
            break;
    }
    ui->gridLinesCheckBox->setChecked(activeView->gridLinesEnabled);

    // Light
    ui->lightIntensitySlider->setValue(activeView->lightIntensity);
    ui->lightOpacitySlider->setValue(activeView->modelOpacity);
    ui->lightSpecularitySlider->setValue(activeView->lightSpecularity);

    // Statistics
    getStatistics();
}

void ModelWindow::updateStructure() {

    if (ui->normalStructRadioButton->isChecked()) {
        activeView->setStructure(0);
    } else if (ui->wireframeStructRadioButton->isChecked()) {
        activeView->setStructure(1);
    } else if (ui->pointsStructRadioButton->isChecked()) {
        activeView->setStructure(2);
    }
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleGridlines() {
    activeView->toggleGridLines(ui->gridLinesCheckBox->isChecked());
}

void ModelWindow::gridlinesInit(View *view) {
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d axis1Color = colors->GetColor3d("Salmon");
    vtkColor3d axis2Color = colors->GetColor3d("PaleGreen");
    vtkColor3d axis3Color = colors->GetColor3d("LightSkyBlue");

    vtkNew<vtkCubeAxesActor> cubeAxesActor;

    cubeAxesActor->SetUseTextActor3D(1);
    cubeAxesActor->SetBounds(
            view->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetBounds());
    cubeAxesActor->SetCamera(
            view->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera());
    cubeAxesActor->GetTitleTextProperty(0)->SetColor(axis1Color.GetData());
    cubeAxesActor->GetTitleTextProperty(0)->SetFontSize(48);
    cubeAxesActor->GetLabelTextProperty(0)->SetColor(axis1Color.GetData());

    cubeAxesActor->GetTitleTextProperty(1)->SetColor(axis2Color.GetData());
    cubeAxesActor->GetLabelTextProperty(1)->SetColor(axis2Color.GetData());

    cubeAxesActor->GetTitleTextProperty(2)->SetColor(axis3Color.GetData());
    cubeAxesActor->GetLabelTextProperty(2)->SetColor(axis3Color.GetData());

    cubeAxesActor->SetGridLineLocation(cubeAxesActor->VTK_GRID_LINES_FURTHEST);
    cubeAxesActor->SetFlyModeToStaticEdges();

    cubeAxesActor->XAxisMinorTickVisibilityOff();
    cubeAxesActor->YAxisMinorTickVisibilityOff();
    cubeAxesActor->ZAxisMinorTickVisibilityOff();

    view->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(cubeAxesActor);

    view->toggleGridLines(false);
}

void ModelWindow::handleMeasurment() {
    if (ui->measurementButton->isChecked()) {
        ui->lightOpacitySlider->setValue(25);
        distanceWidget = vtkDistanceWidget::New();
        distanceWidget->SetInteractor(activeView->qVTKWidget->GetRenderWindow()->GetInteractor());
        vtkSmartPointer<vtkDistanceRepresentation3D> representation = vtkDistanceRepresentation3D::New();
        distanceWidget->SetRepresentation(representation);
        distanceWidget->SetPriority(0.9);
        dynamic_cast<vtkDistanceRepresentation *> (distanceWidget->GetRepresentation())->SetLabelFormat("%-#6.2f mm");
        distanceWidget->ManagesCursorOn();
        distanceWidget->On();
    } else {
        ui->lightOpacitySlider->setValue(100);
        distanceWidget->Off();
    }
}

void ModelWindow::addViewToFrame(View *view) {

    // active view count
    unsigned short int avc = View::getCount();
    std::cout << avc << std::endl;

    // max 4 views allowed
    if (avc > 4) {
        delete view;
        return;
    }

    // Store previous views.
    views.push_back(view);

    // fiting matrix
    static unsigned short int fm[4][4] = {
            {0, 0, 2, 1},   /*  row, column, rowSpan, columnSpan of first inserted */
            {0, 1, 2, 1},   /*  row, column, rowSpan, columnSpan of second inserted */
            {1, 0, 1, 2},   /*  row, column, rowSpan, columnSpan of third inserted */
            {1, 1, 1, 1},   /*  row, column, rowSpan, columnSpan of fourth inserted */
    };


    ui->viewFrame->addWidget(view, fm[avc - 1][0], fm[avc - 1][1], fm[avc - 1][2], fm[avc - 1][3]);

    // refit previous:
    //
    // There is no method for resetting the row/column-span after a widget has been added. However, addWidget can
    // be called again on the same widget to achieve the same affect, because re-adding a widget to the same layout
    // always implicitly removes it first.

    if (avc == 3) {
        ui->viewFrame->addWidget(views[0], 0, 0, 1, 1);
        ui->viewFrame->addWidget(views[1], 0, 1, 1, 1);
    }

    if (avc == 4) {
        ui->viewFrame->addWidget(views[2], 1, 0, 1, 1);
    }
}

void ModelWindow::getStatistics() {
    if (activeView->model.fileType == "mod") {
        activeView->numOfCells = activeView->model.displayCells();
        activeView->centreOfGrav = activeView->model.calcCentre();
        activeView->weight = activeView->model.calcWeight();
        activeView->volume = activeView->model.calcVolume();
        activeView->density = activeView->weight / activeView->volume;

        ui->numOfCellsLabel->setText(QString::number(activeView->numOfCells));
        ui->centreOfGravLabel->setText("X: " + QString::number(activeView->centreOfGrav.getX()) + "\nY: " +
                                       QString::number(activeView->centreOfGrav.getY()) + "\nZ: " +
                                       QString::number(activeView->centreOfGrav.getZ()));
        ui->weightLabel->setText(QString::number(activeView->weight));
        ui->volumeLabel->setText(QString::number(activeView->volume));
        ui->densityLabel->setText(QString::number(activeView->density));
    } else if (activeView->model.fileType == "stl") {
        vtkSmartPointer<vtkTriangleFilter> triFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triFilter->SetInputData(activeView->model.STLModel->GetOutput());
        triFilter->Update();

        vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();
        massProp->SetInputData(triFilter->GetOutput());
        double volume = massProp->GetVolume();

        double center[3];
        vtkSmartPointer<vtkCenterOfMass> centreOfMass = vtkSmartPointer<vtkCenterOfMass>::New();
        centreOfMass->SetInputData(triFilter->GetOutput());
        centreOfMass->GetCenter(center);
        centreOfMass->Update();
        MVector centreOfGrav(center[0], center[1], center[2]);


        unsigned long numOfCells = activeView->model.STLModel->GetOutputDataObject(0)->GetNumberOfElements(
                vtkDataObject::CELL);

        activeView->volume = volume;
        activeView->centreOfGrav = centreOfGrav;
        activeView->numOfCells = numOfCells;

        ui->volumeLabel->setText(QString::number(activeView->volume));
        ui->centreOfGravLabel->setText("X: " + QString::number(activeView->centreOfGrav.getX()) + "\nY: " +
                                       QString::number(activeView->centreOfGrav.getY()) + "\nZ: " +
                                       QString::number(activeView->centreOfGrav.getZ()));
        ui->numOfCellsLabel->setText(QString::number(activeView->numOfCells));
        ui->weightLabel->setText("-");
        ui->densityLabel->setText("-");


    }
}


