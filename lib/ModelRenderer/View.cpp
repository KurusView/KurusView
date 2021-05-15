#include <QWidget>
#include <QVBoxLayout>
#include <QTime>
#include <QGuiApplication>
#include <QScreen>
#include <QInputDialog>

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
#include <vtkCallbackCommand.h>
#include <vtkPlane.h>
#include <vtkLight.h>
#include <vtkLightCollection.h>
#include <vtkCubeAxesActor.h>
#include <vtkDistanceRepresentation3D.h>
#include <QFileDialog>
#include <vtkTextProperty.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellData.h>
#include <vtkHexahedron.h>
#include <vtkTetra.h>
#include <vtkPyramid.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>
#include <vtkMinimalStandardRandomSequence.h>

#include "View.h"
#include "settingsdialog.h"

#define USE_QUANTUM_BLOCKCHAIN_ML

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

    if (model->fileType == "stl") {
        loadSTLModel();
    } else if (model->fileType == "mod") {
        buildVTKModelFromMod();
    }

    setStyleSheet("*{padding: 0; border-width: 3 ;border-style:solid;border-color: dark" + this->borderColor + ";}");


    verticalLayout = new QVBoxLayout(this);

    // Get Primary Screen Height
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();
    int screenWidth = QGuiApplication::primaryScreen()->geometry().width();

    qVTKWidget = new QVTKOpenGLWidget(parent);
    qVTKWidget->setMinimumSize(QSize(screenHeight / 10, screenWidth / 10));

    verticalLayout->addWidget(qVTKWidget);

    verticalLayout->setMargin(6);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    qVTKWidget->SetRenderWindow(renderWindow);

    vtkDataObject *dataObj = vtkModel->GetOutputDataObject(0);
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
    toggleShrinkFilter(viewSettings->value("isShrunk", false).value<bool>(),
                       viewSettings->value("shrinkFactor", NULL).value<double>());

    // Colours
    if (viewSettings->contains("modelColor") || model->fileType == "stl") {
        setModelColor(viewSettings->value("modelColor", settingsDialog::getDefault_modelColour()).value<QColor>());
        setModelBackFaceColor(viewSettings->value("modelBackFaceColor",
                                                  settingsDialog::getDefault_modelBackFaceColour()).value<QColor>()
        );

    } else {

        // mess with late night devs so they go to sleep
#if defined(USE_QUANTUM_BLOCKCHAIN_ML) && defined(QT_DEBUG)

        QString currentTime = QTime::currentTime().toString();

        QString hour = currentTime.mid(0, 2);
//        QString minute = currentTime.mid(3, 2);

//        // minute specific
//        if (!((hour == QString::fromStdString("21")) && (minute == QString::fromStdString("03")))) {
//            setModelColor();
//        }

        // even more rude
        if ((2 <= hour.toInt()) && (hour.toInt() >= 4)) {
            setModelColor();
        }

#else
        setModelColor();
#endif
    }


    setBackgroundColor(
            viewSettings->value("backgroundColor", settingsDialog::getDefault_backgroundColour()).value<QColor>()
    );

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

void View::buildVTKModelFromMod() {
    vtkNew<vtkNamedColors> colors;

    // vertex placeholder
    vtkNew<vtkPoints> vertex;

    // Store all points from the model vector list
    for (auto &vector : model->getVectors()) {
        vertex->InsertNextPoint(vector.getX(), vector.getY(), vector.getZ());
    }

    // cell colour placeholder
    std::vector<vtkColor3d> cellColours;

    // cells placeholder
    std::vector<vtkSmartPointer<vtkCell3D>> cells3D;

    vtkIdType counter = 0;
    for (auto &cell : model->cells) {
        // get cell colour
        QColor cellColour_qt;
        std::string cellColour_raw = "#" + cell->getMaterial()->getColour();
        cellColour_qt = QColor(QString::fromStdString(cellColour_raw));
        cellColours.emplace_back(cellColour_qt.redF(), cellColour_qt.greenF(), cellColour_qt.blueF());

        // store raw tetra hex pyra list
        switch (cell->getType()[0][0]) {
            case 'h':
                cells3D.emplace_back(vtkSmartPointer<vtkHexahedron>::New());
                break;
            case 't':
                cells3D.emplace_back(vtkSmartPointer<vtkTetra>::New());
                break;
            case 'p':
                cells3D.emplace_back(vtkSmartPointer<vtkPyramid>::New());
                break;
        }

        // Associate cell vertices with IDs
        for (int j = 0; j < cell->getVertices().size(); ++j) {
            cells3D[counter]->GetPointIds()->SetId(j, cell->getVertexIndices()[j]);
        }

        counter++;
    }

    // An unstructured grid allows any cell type to be combined in arbitrary combinations.
    vtkSmartPointer<vtkUnstructuredGrid> uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

    // Insert each cell into an unstructured grid.
    counter = 0;
    vtkNew<vtkUnsignedCharArray> cellData;

    // colors placeholder
    cellData->SetNumberOfComponents(3);
    cellData->SetNumberOfTuples(uGrid->GetNumberOfCells());

    vtkNew<vtkMinimalStandardRandomSequence> randomSequence;
    randomSequence->SetSeed(8775070);

    for (auto &cell: cells3D) {
        uGrid->InsertNextCell(cell->GetCellType(), cell->GetPointIds());

        // populate colours
        double rgb[3];
        rgb[0] = randomSequence->GetRangeValue(64, 255);
        randomSequence->Next();
        rgb[1] = randomSequence->GetRangeValue(64, 255);
        randomSequence->Next();
        rgb[2] = randomSequence->GetRangeValue(64, 255);// cellColours[counter].GetBlue() * 255.0;
        randomSequence->Next();
        cellData->InsertTuple(counter, rgb);

        counter++;
    }

    uGrid->GetCellData()->SetScalars(cellData);

    // Unstructured grid has a global list of vectors (points)
    // which it uses to construct the cells using indices
    uGrid->SetPoints(vertex);
    vtkSmartPointer<vtkUnstructuredGridReader> uGridReader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    uGridReader->SetOutput(uGrid);
    vtkModel = uGridReader;
}

void View::loadSTLModel() {
    STLModel = vtkSmartPointer<vtkSTLReader>::New();
    STLModel->SetFileName(model->filePath.c_str());
    STLModel->Update();
    vtkModel = STLModel;
}

void View::buildChain() {
    // Store the source object, as it's used multiple times
    vtkDataSet *source = dynamic_cast<vtkDataSet *>(vtkModel->GetOutputDataObject(0));
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

bool View::toggleShrinkFilter(bool enable, double shrinkFactor) {
    bool gotNumber = false;
    if (enable) {
        if (shrinkFactor == NULL) {

            // Requests user for shrink factor, defaulting to stored value in kurus file, or 0.5 otherwise
            shrinkFactor = QInputDialog::getDouble(this, tr("Shrink Factor Dialog"),
                                                   tr("Please enter a shrink factor, between 0 and 1"),
                                                   viewSettings->value("shrinkFactor", 0.5).value<double>(), 0, 1, 1,
                                                   &gotNumber, Qt::WindowFlags(), 0.1);
            if (!gotNumber) return false;
        }
        // Shrinks to 50%
        shrinkFilter->SetShrinkFactor(shrinkFactor);
        // Insert at the end of the filter list
        filters.emplace_back(shrinkFilter);
    } else if (std::find(filters.begin(), filters.end(), shrinkFilter) != filters.end()) {
        // Remove the shrink filter from the filter list
        filters.erase(std::find(filters.begin(), filters.end(), shrinkFilter));
    }
    isShrunk = enable;

    viewSettings->setValue("isShrunk", enable);
    if (gotNumber)
        viewSettings->setValue("shrinkFactor", shrinkFactor);

    return true;
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
    // update settings
    viewSettings->setValue("modelColor", color);
    if (model->fileType == "mod") {
        vtkUnstructuredGridReader *uGridReader = (vtkUnstructuredGridReader *) vtkModel.Get();
        vtkUnstructuredGrid *uGrid = uGridReader->GetOutput();

        vtkNew<vtkUnsignedCharArray> cellData;

        // colors placeholder
        cellData->SetNumberOfComponents(3);
        cellData->SetNumberOfTuples(uGrid->GetNumberOfCells());

        for (int i = 0; i < model->getCellCount(); i++) {
            // populate colours
            double rgb[3];
            if (!color.isValid()) {
                std::string cellColour_raw = "#" + model->getCells()[i]->getMaterial()->getColour();
                QColor cellColour_qt = QColor(QString::fromStdString(cellColour_raw));
                rgb[0] = cellColour_qt.red();
                rgb[1] = cellColour_qt.green();
                rgb[2] = cellColour_qt.blue();
            } else {
                rgb[0] = color.red();
                rgb[1] = color.green();
                rgb[2] = color.blue();
            }
            std::cout << rgb[0] << "\t" << rgb[1] << "\t" << rgb[2] << "\t" << endl;
            cellData->SetTuple(i, rgb);
        }
        uGrid->GetCellData()->SetScalars(cellData);

        //QColor cellColour_qt = QColor(QString::fromStdString(modelColour));

        if (color.isValid()) {
            modelColour = color.name();
        } else {
            modelColour = nullptr;
        }

        qVTKWidget->GetRenderWindow()->Render();


    } else {
        vtkActorCollection *actors = qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors();
        auto *actor = (vtkActor *) actors->GetItemAsObject(0);
        actor->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());

        modelColour = color.name();
    }
}


void View::setModelBackFaceColor(const QColor &color) {
    if (!color.isValid())
        return;

    // update settings
    viewSettings->setValue("modelBackFaceColor", color);

    // hack around QT bug(?): backface color is not updated if model colour is same as backface. Set the the model
    // colour to something else and back. Colour is offset by a small amount so the change is invisible.
//    QColor currentModelColour = QColor(modelColour);
//    QColor offByOne = QColor(modelColour);
//    offByOne.setRedF(offByOne.redF() + 0.1);
//    setModelColor(offByOne);
//    setModelColor(currentModelColour);
    bool toggle = isClipped;
    if (isClipped || isShrunk) {
        toggleClipFilter(!toggle);
        toggleClipFilter(toggle);
    }

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

void View::resetColour() {
    // convert QT to vtk colours
    QColor model_qt = settingsDialog::getDefault_modelColour();
    QColor backFace_qt = settingsDialog::getDefault_modelBackFaceColour();
    QColor background_qt = settingsDialog::getDefault_backgroundColour();

    vtkColor3d model_vtk(model_qt.redF(), model_qt.greenF(), model_qt.blueF());
    vtkColor3d backFace_vtk(backFace_qt.redF(), backFace_qt.greenF(), backFace_qt.blueF());
    vtkColor3d background_vtk(background_qt.redF(), background_qt.greenF(), background_qt.blueF());

    // reset active view references
    modelColour = model_qt.name();
    backgroundColour = background_qt.name();

    if (model->fileType == "stl") {
        modelBackFaceColor = backFace_qt.name();
    }

    // reset background
    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->SetBackground(
            background_vtk.GetData());

    // get actor
    auto *actor = (vtkActor *) qVTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetItemAsObject(
            0);

    // reset model
    if (model->fileType == "stl") {
        // reset model colour
        setModelColor(settingsDialog::getDefault_modelColour());

        // reset model backface
        vtkNew<vtkProperty> backFace;
        backFace->SetDiffuseColor(backFace_vtk.GetData());
        actor->SetBackfaceProperty(backFace);

    } else {
        // reset model colour only (.mods dont have backface functionality)
        setModelColor();
    }
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
    viewSettings->setValue("modelFilePath", QString::fromStdString(model->filePath));

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
}
