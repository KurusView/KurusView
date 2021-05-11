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
#include <vtkCuller.h>
#include <math.h>
#include "welcomewindow.h"

#include <QFileDialog>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkDistanceRepresentation3D.h>
#include <vtkDistanceWidget.h>
#include <QVTKWidget.h>
#include <QColorDialog>
#include <QApplication>
#include <QWidget>
#include <QScreen>
#include <QSettings>
#include <QDesktopServices>

#include <vtkOrientationMarkerWidget.h>

#include "View.h"
#include "modelwindow.h"
#include "ui_modelwindow.h"
#include "dialog.h"
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

#include "settingsdialog.h"

ModelWindow::ModelWindow(const QStringList &filePaths, QWidget *parent) : QMainWindow(parent),
                                                                          ui(new Ui::ModelWindow),
                                                                          maxFileNr(
                                                                                  10), // this one should probably be hardcoded to 10 or less
                                                                          settings(QDir::currentPath() +
                                                                                   "/kurusview.ini",
                                                                                   QSettings::IniFormat) {
    //TODO: Make sure the model is properly initialized before loading the window
    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);

    this->setWindowTitle("KurusView: Model Viewer");

    // Get Primary Screen Height
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();
    int screenWidth = QGuiApplication::primaryScreen()->geometry().width();

    ui->bottomMenu->setMaximumHeight(screenHeight / 6);
    ui->statisticsGroupBox->setMinimumWidth(screenWidth / 6);

    int statisticsBoxWidth = ui->statisticsGroupBox->width();

    ui->centreOfGravLabel->setMinimumWidth(statisticsBoxWidth / 1.7);
    ui->densityLabel->setMinimumWidth(statisticsBoxWidth / 1.7);
    ui->weightLabel->setMinimumWidth(statisticsBoxWidth / 1.7);
    ui->numOfCellsLabel->setMinimumWidth(statisticsBoxWidth / 1.7);
    ui->volumeLabel->setMinimumWidth(statisticsBoxWidth / 1.7);

    std::cout << ui->centreOfGravLabel->width() << std::endl;

    for (int i = 0; i < filePaths.size() && i < 4; ++i) {
        addViewToFrame(new View(filePaths[i], parent));
    }

    for (auto &view : views) {
        setActiveView(view);
    }

    setActiveView(views[0]);

    std::cout << View::getCount();

    createActionsAndConnections();

    connect(ui->menuRecent_Files, &QMenu::aboutToShow, this, &ModelWindow::updateRecentActionList);

    show();

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
    connect(ui->modelBackFaceColourPushButton, &QPushButton::released, this, &ModelWindow::handleModelBackFaceColor);
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
    connect(ui->measurementButton, &QPushButton::released, this, &ModelWindow::handleMeasurement);

}

ModelWindow::~ModelWindow() {
    delete ui;
}

void ModelWindow::handleBackgroundColor() {
    QColor color = QColorDialog::getColor(QColor(activeView->backgroundColour), this);

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
    QColor color = QColorDialog::getColor(QColor(activeView->modelColour), this);

    if (!color.isValid())
        return;
    activeView->setModelColor(color);
    // update button color
    ui->modelColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");
    // refresh view
    activeView->qVTKWidget->GetRenderWindow()->Render();
}

void ModelWindow::handleModelBackFaceColor() {
    // request color
    QColor color = QColorDialog::getColor(QColor(activeView->modelColour), this);

    // sanity check
    if (!color.isValid())
        return;

    activeView->setModelBackFaceColor(color);

    // hack around QT bug(?): backface color is not updated if model colour is same as backface. Set the the model
    // colour to something else and back. Colour is offset by a small amount so the change is invisible.
    QColor currentModelColour = QColor(activeView->modelColour);
    QColor offByOne = QColor(activeView->modelColour);
    offByOne.setRedF(offByOne.redF() + 0.1);
    activeView->setModelColor(offByOne);
    activeView->setModelColor(currentModelColour);

    // get actor
    auto *actor = (vtkActor *) activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetItemAsObject(
            0);


    // QReal is a double
    vtkColor3d vtkcolor(color.redF(), color.greenF(), color.blueF());

    // set backface color (from docs: as the side effect of setting the ambient diffuse and specular colors as well.
    // This is basically a quick overall color setting method).
    vtkNew<vtkProperty> backFace;
    //backFaces->SetDiffuseColor(vtkcolor.GetData());
    backFace->SetDiffuseColor(color.redF(), color.greenF(), color.blueF());

    // apply color to actor
    actor->SetBackfaceProperty(backFace);

    // update button color
    ui->modelBackFaceColourPushButton->setStyleSheet("background-color: " + color.name() + "; border:none;");

    // refresh view
    activeView->qVTKWidget->GetRenderWindow()->Render();

}

void ModelWindow::handleResetColor() {

    //reset buttons
    ui->modelColourPushButton->setStyleSheet("background-color: silver; border:none;");
    ui->modelBackFaceColourPushButton->setStyleSheet("background-color: silver; border:none;");
    ui->backgroundColourPushButton->setStyleSheet("background-color: silver; border:none;");

    // convert QT to vtk colours
    QColor model_qt = settingsDialog::getDefault_modelColour();
    QColor backFace_qt = settingsDialog::getDefault_modelBackFaceColour();
    QColor background_qt = settingsDialog::getDefault_backgroundColour();

    vtkColor3d model_vtk(model_qt.redF(), model_qt.greenF(), model_qt.blueF());
    vtkColor3d backFace_vtk(backFace_qt.redF(), backFace_qt.greenF(), backFace_qt.blueF());
    vtkColor3d background_vtk(background_qt.redF(), background_qt.greenF(), background_qt.blueF());

    // reset background
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(
            background_vtk.GetData());

    // get actor
    auto *actor = (vtkActor *) activeView->qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetItemAsObject(
            0);

    // reset model
    actor->GetProperty()->SetColor(model_vtk.GetData());

    // reset model backface
    vtkNew<vtkProperty> backFace;
    backFace->SetDiffuseColor(backFace_vtk.GetData());
    actor->SetBackfaceProperty(backFace);

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
            "background-color: " + activeView->modelColour + "; border:none;");
    ui->modelBackFaceColourPushButton->setStyleSheet(
            "background-color: " + activeView->modelBackFaceColor + "; border:none;");

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
    ui->measurementButton->setChecked(activeView->measurementEnabled);

    std::cout << ui->statisticsGroupBox->width() << std::endl;
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

void ModelWindow::handleMeasurement() {
    if (ui->measurementButton->isChecked()) {
        ui->lightOpacitySlider->setValue(25);
    } else {
        ui->lightOpacitySlider->setValue(100);
    }
    activeView->toggleMeasurement(ui->measurementButton->isChecked());
}

void ModelWindow::addViewToFrame(View *view) {

    // active view count
    unsigned short int avc = View::getCount();
    size_t index = views.size();
    std::cout << "Total Views: " << avc << ", Views in Current Window: " << index << std::endl;

    // max 4 views allowed PER WINDOW
    if (views.size() > 4) {
        delete view;
        return;
    }

    // Store previous views.
    views.push_back(view);

    resetViewLayout();

    connect(view->qVTKWidget, &QVTKOpenGLWidget::mouseEvent, this, &ModelWindow::viewActive);
}

void ModelWindow::resetViewLayout() {
// fitting matrix
    static unsigned short int fm[4][4] = {
            {0, 0, 1, 1},   /*  row, column, rowSpan, columnSpan of first inserted */
            {0, 1, 1, 1},   /*  row, column, rowSpan, columnSpan of second inserted */
            {0, 1, 2, 1},   /*  row, column, rowSpan, columnSpan of third inserted */
            {1, 1, 1, 1},   /*  row, column, rowSpan, columnSpan of fourth inserted */
    };

    for (int i = 0; i < views.size(); ++i) {
        ui->viewFrame->addWidget(views[i], fm[i][0], fm[i][1], fm[i][2], fm[i][3]);
        // refit previous:
        //
        // There is no method for resetting the row/column-span after a widget has been added. However, addWidget can
        // be called again on the same widget to achieve the same affect, because re-adding a widget to the same layout
        // always implicitly removes it first.

        if (i + 1 == 3)
            ui->viewFrame->addWidget(views[1], 1, 0, 1, 1);

        if (i + 1 == 4)
            ui->viewFrame->addWidget(views[2], 0, 1, 1, 1);

    }
}

void ModelWindow::getStatistics() {
    if (activeView->model->fileType == "mod") {
        activeView->numOfCells = activeView->model->getCellCount();
        activeView->centreOfGrav = activeView->model->calcCentre();
        activeView->weight = activeView->model->calcWeight();
        activeView->volume = activeView->model->calcVolume();
        activeView->density = activeView->weight / activeView->volume;

        QString x = QString::number(activeView->centreOfGrav.getX(), 'g', 3);
        QString y = QString::number(activeView->centreOfGrav.getY(), 'g', 3);
        QString z = QString::number(activeView->centreOfGrav.getZ(), 'g', 3);


        ui->numOfCellsLabel->setText(QString::number(activeView->numOfCells));
        ui->centreOfGravLabel->setText(
                "X: " + QString::number(x.toDouble()) + " Y: " + QString::number(y.toDouble()) + " Z: " +
                QString::number(z.toDouble()));
        ui->weightLabel->setText(QString::number(activeView->weight));
        ui->volumeLabel->setText(QString::number(activeView->volume));
        ui->densityLabel->setText(QString::number(activeView->density));
    } else if (activeView->model->fileType == "stl") {
        vtkSmartPointer<vtkTriangleFilter> triFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triFilter->SetInputData(activeView->model->STLModel->GetOutput());
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

        QString x = QString::number(centreOfGrav.getX(), 'g', 3);
        QString y = QString::number(centreOfGrav.getY(), 'g', 3);
        QString z = QString::number(centreOfGrav.getZ(), 'g', 3);


        unsigned long numOfCells = activeView->model->STLModel->GetOutputDataObject(0)->GetNumberOfElements(
                vtkDataObject::CELL);

        activeView->volume = volume;
        activeView->centreOfGrav = centreOfGrav;
        activeView->numOfCells = numOfCells;

        ui->volumeLabel->setText(QString::number(activeView->volume));
        ui->centreOfGravLabel->setText(
                "X: " + QString::number(x.toDouble()) + " Y: " + QString::number(y.toDouble()) + " Z: " +
                QString::number(z.toDouble()));
        ui->numOfCellsLabel->setText(QString::number(activeView->numOfCells));
        ui->weightLabel->setText("-");
        ui->densityLabel->setText("-");


    }
}

void ModelWindow::updateRecentActionList() {
    settings.sync();
    recentFilePaths =
            settings.value("recentFiles").toStringList();

    auto itEnd = 0u;
    if (recentFilePaths.size() <= maxFileNr)
        itEnd = recentFilePaths.size();
    else
        itEnd = maxFileNr;

    for (auto i = 0u; i < itEnd; ++i) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }

    for (auto i = itEnd; i < maxFileNr; ++i)
        recentFileActionList.at(i)->setVisible(false);
}

void ModelWindow::adjustForCurrentFile(const QString &filePath) {
    currentFilePath = filePath;
    setWindowFilePath(currentFilePath);

    settings.sync();
    recentFilePaths =
            settings.value("recentFiles").value<QStringList>();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > maxFileNr)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);
    settings.sync();
    // see note
    updateRecentActionList();
}

void ModelWindow::openRecent() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QStringList fileList;
        fileList.append(action->data().value<QString>());
        loadFile(fileList);
    }
}

void ModelWindow::createActionsAndConnections() {
    ui->actionOpenView->setShortcuts(QKeySequence::Open);
    connect(ui->actionOpenView, &QAction::triggered, this, &ModelWindow::open);
    connect(ui->actionSaveView, &QAction::triggered, activeView, &View::save);
    connect(ui->actionCloseView, &QAction::triggered, this, &ModelWindow::closeView);
    connect(ui->actionHelp, &QAction::triggered, this, &ModelWindow::handleHelpButton);
    connect(ui->actionSettings, &QAction::triggered, this, &ModelWindow::handleSettingsButton);


    for (auto i = 0; i < maxFileNr; ++i) {
        QAction *recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        connect(recentFileAction, &QAction::triggered, this, &ModelWindow::openRecent);
        recentFileActionList.append(recentFileAction);
    }
    recentFilesMenu = ui->menuRecent_Files;
    for (auto i = 0; i < maxFileNr; ++i)
        recentFilesMenu->addAction(recentFileActionList.at(i));

    updateRecentActionList();
}

void ModelWindow::open() {
    settings.sync();
    QStringList inputFileNames = QFileDialog::getOpenFileNames(this, tr("Load a Kurus View or Model"),
                                                               settings.value("lastOpenDirectory",
                                                                              "save_models").value<QString>(),
                                                               tr("Model, View or STL (*.mod;*.kurus;*.stl)"));
    if (inputFileNames.isEmpty()) {
        return;
    }

    // Update lastOpenDirectory in settings and sync
    settings.setValue("lastOpenDirectory", QFileInfo(inputFileNames.at(0)).absoluteDir().absolutePath());
    settings.sync();

    loadFile(inputFileNames);
}

void ModelWindow::loadFile(const QStringList &filePaths) {
    bool newWindow = true;

    if (views.size() + filePaths.size() <= 4) {
        Dialog *dialog = new Dialog(this);
        dialog->setModal(true);
        newWindow = dialog->exec();
    }

    if (newWindow)
            emit openNewModelWindow(filePaths);
    else {
        for (int i = 0; i < filePaths.size(); ++i)
            addViewToFrame(new View(filePaths[i]));
        setActiveView(views[views.size() - 1]);
    }
    for (int i = 0; i < filePaths.size(); ++i)
        adjustForCurrentFile(filePaths[i]);
}

void ModelWindow::closeView() {
    for (int i = 0; i < views.size(); ++i) {
        if (views[i] == activeView) {
            View *view = views[i];
            views.erase(views.begin() + i);
            delete view;
        }
    }
    if (views.empty()) {
        close();
    }
    resetViewLayout();
}

void ModelWindow::handleHelpButton() {
    QDesktopServices::openUrl(QUrl("https://github.com/KurusView/2020_GROUP_21", QUrl::TolerantMode));
}

void ModelWindow::handleSettingsButton() {
    settingsDialog open_settings;

    open_settings.exec();
}
