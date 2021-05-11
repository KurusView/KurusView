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
#include <vtkDistanceRepresentation3D.h>
#include <QFileDialog>
#include <vtkTextProperty.h>

#include "View.h"

// set default instance count
unsigned short int View::ViewInstanceCount = 0;

View::View(const QString &filePath, QWidget *parent) : QWidget(parent) {


    //increment instance count
    ViewInstanceCount++;

    this->borderColor = borderColors[getCount() % 4];

    std::string fileExtension = filePath.toStdString().substr(filePath.toStdString().find_last_of('.') + 1);

    if (fileExtension == "kurus") {
        viewSettings = std::make_shared<QSettings>(filePath, QSettings::IniFormat);
        viewSettings->sync();
        this->filePath = filePath;

        QString modelFilePath = viewSettings->value("modelFilePath").value<QString>();
        model = std::make_shared<Model>(modelFilePath.toStdString());
    } else {
        viewSettings = std::make_shared<QSettings>();
        viewSettings->setValue("modelFilePath", filePath);
        model = std::make_shared<Model>(filePath.toStdString());
    }

    setStyleSheet("*{padding: 0; border-width: 3 ;border-style:solid;border-color: dark" + this->borderColor + ";}");


    verticalLayout = new QVBoxLayout(this);

    qVTKWidget = new QVTKOpenGLWidget(parent);
    qVTKWidget->setMinimumSize(QSize(300, 200)); // TODO make this dynamic

    verticalLayout->addWidget(qVTKWidget);

    verticalLayout->setMargin(6);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qVTKWidget->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkAlgorithm> modelAlgo = model->getVTKModel();
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

    gridlinesInit();

    // Set all parameters

    // Fix bug: light intensity needs to be initialized manually first
    lightIntensity = viewSettings->value("lightIntensity", 70).value<int>();

    // Filters
    toggleClipFilter(viewSettings->value("isClipped", false).value<bool>());
    toggleShrinkFilter(viewSettings->value("isShrunk", false).value<bool>());

    // Colours
    setModelColor(viewSettings->value("modelColor", settingsDialog::getDefault_modelColour()).value<QColor>());
    setModelBackFaceColor(viewSettings->value("modelBackFaceColor",
                                              settingsDialog::getDefault_modelBackFaceColour()).value<QColor>());
    setBackgroundColor(
            viewSettings->value("backgroundColor", settingsDialog::getDefault_backgroundColour()).value<QColor>());

    // Structure
    setStructure(viewSettings->value("structure", 0).value<int>());
    toggleGridLines(viewSettings->value("gridLinesEnabled", false).value<bool>());

    // Lighting
    setLightIntensity(viewSettings->value("lightIntensity", 70).value<int>());
    setModelOpacity(viewSettings->value("modelOpacity", 100).value<int>());
    setLightSpecularity(viewSettings->value("lightSpecularity", 0).value<int>());

    // Measurement
    measurementEnabled = false;

    buildChain();
    qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
    qVTKWidget->GetRenderWindow()->Render();
}

void View::buildChain() {
    // Store the source object, as it's used multiple times
    vtkDataSet *source = dynamic_cast<vtkDataSet *>(model->getVTKModel()->GetOutputDataObject(0));
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

    viewSettings->setValue("isShrunk", enable);
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
    viewSettings->setValue("isClipped", enable);
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

    // update settings
    viewSettings->setValue("modelColor", color);
    actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

    // Store the color locally
    modelColour = color.name();
}


void View::setModelBackFaceColor(const QColor &color) {
    if (!color.isValid())
        return;

    // update settings
    viewSettings->setValue("modelBackFaceColor", color);

    // hack around QT bug(?): backface color is not updated if model colour is same as backface. Set the the model
    // colour to something else and back. Colour is offset by a small amount so the change is invisible.
    QColor currentModelColour = QColor(modelColour);
    QColor offByOne = QColor(modelColour);
    offByOne.setRedF(offByOne.redF() + 0.1);
    setModelColor(offByOne);
    setModelColor(currentModelColour);

    // get actor
    auto *actor = (vtkActor *) qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetItemAsObject(
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

    // Store the color locally
    modelBackFaceColor = color.name();
}


void View::setBackgroundColor(const QColor &color) {
    if (!color.isValid())
        return;

    // Get renderer
    qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(color.redF(),
                                                                                     color.greenF(),
                                                                                     color.blueF()
    );

    viewSettings->setValue("background", color);
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
    viewSettings->setValue("lightIntensity", value);
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
    viewSettings->setValue("lightSpecularity", value);
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
    viewSettings->setValue("modelOpacity", value);
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
    viewSettings->setValue("gridLinesEnabled", gridLinesEnabled);
    qVTKWidget->GetRenderWindow()->Render();
}

void View::toggleMeasurement(bool enable) {
    if (enable) {
        distanceWidget = vtkDistanceWidget::New();
        distanceWidget->SetInteractor(qVTKWidget->GetRenderWindow()->GetInteractor());
        vtkSmartPointer<vtkDistanceRepresentation3D> representation = vtkDistanceRepresentation3D::New();
        distanceWidget->SetRepresentation(representation);
        distanceWidget->SetPriority(0.9);
        dynamic_cast<vtkDistanceRepresentation *> (distanceWidget->GetRepresentation())->SetLabelFormat(
                "%-#6.2f mm");
        distanceWidget->ManagesCursorOn();
        distanceWidget->On();
    } else {
        distanceWidget->Off();
    }
    measurementEnabled = enable;
}

void View::setStructure(int selectedStructure) {
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
    viewSettings->setValue("structure", selectedStructure);
}

void View::populateSettings() {
    // Set all parameters
    // Model
    viewSettings->setValue("modelFilePath", model->filePath);

    // Filters
    viewSettings->setValue("isClipped", isClipped);
    viewSettings->setValue("isShrunk", isShrunk);

    // Colours
    viewSettings->setValue("modelColor", modelColour);
    viewSettings->setValue("backgroundColor", backgroundColour);
    viewSettings->setValue("modelBackFaceColor", modelBackFaceColor);

    // Structure
    viewSettings->setValue("structure", structure);
    viewSettings->setValue("gridLinesEnabled", gridLinesEnabled);

    // Lighting
    viewSettings->setValue("lightIntensity", lightIntensity);
    viewSettings->setValue("modelOpacity", modelOpacity);
    viewSettings->setValue("lightSpecularity", lightSpecularity);
}

void View::save() {
    populateSettings();
    if (filePath.isEmpty())
        saveAs();
    viewSettings->sync();
}

void View::saveAs() {
    QSettings globalSettings(QDir::currentPath() + "/kurusview.ini", QSettings::IniFormat);
    globalSettings.sync();

    QString savePath = QFileDialog::getSaveFileName(this, tr("Save Kurus View"),
                                                    globalSettings.value("lastOpenDirectory",
                                                                         "save_models").value<QString>(),
                                                    tr("Kurus View (*.kurus)"));

    if (savePath.isEmpty()) {
        return;
    }
    this->filePath = savePath;
    std::cout << filePath.toStdString() << std::endl;
    viewSettings = std::make_shared<QSettings>(savePath, QSettings::IniFormat);
    save();

    std::cout << globalSettings.status() << std::endl;
    std::cout << viewSettings->status() << std::endl;
}

void View::gridlinesInit() {
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    vtkColor3d axis1Color = colors->GetColor3d("Salmon");
    vtkColor3d axis2Color = colors->GetColor3d("PaleGreen");
    vtkColor3d axis3Color = colors->GetColor3d("LightSkyBlue");

    vtkNew<vtkCubeAxesActor> cubeAxesActor;

    cubeAxesActor->SetUseTextActor3D(1);
    cubeAxesActor->SetBounds(
            qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetBounds());
    cubeAxesActor->SetCamera(
            qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera());
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

    qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(cubeAxesActor);

    toggleGridLines(false);
}}
