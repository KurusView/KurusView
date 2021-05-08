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

#include <QFileDialog>

#include "modelwindow.h"
#include "ui_modelwindow.h"

ModelWindow::ModelWindow(const QString &filePath, QWidget *parent) : QMainWindow(parent), ui(new Ui::ModelWindow),
                                                                     currentModel(filePath.toStdString()),
                                                                     currentModelFilePath(filePath) {
    //TODO: Make sure the model is properly initialized before loading the window
    // standard call to setup Qt UI (same as previously)
    ui->setupUi(this);

    // Now need to create a VTK render window and link it to the QtVTK widget
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    ui->qvtkWidget->SetRenderWindow(
            renderWindow);            // note that vtkWidget is the name I gave to my QtVTKOpenGLWidget in Qt creator

    //TODO: User input of filepath for both .mod and .stl and have logic to decide between each

    // Create a mapper that will hold the object's geometry in a format suitable for rendering
    mapper = vtkSmartPointer<vtkDataSetMapper>::New();
//    mapper->SetInputConnection(currentModel.getVTKModel()->GetOutputPort());
    mapper->SetInputData(dynamic_cast<vtkDataSet *>(currentModel.getVTKModel()->GetOutputDataObject(0)));

    // Create an actor that is used to set the object's properties for rendering and place it in the window
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();
    actor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());

    // Create a renderer, and render window
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    //vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();		// ###### We've already created the renderWindow this time ######
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
//    light->SetPositional(true);
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
