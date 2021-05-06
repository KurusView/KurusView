#include "ModelRenderer.h"

#include <QSplashScreen>
#include <QSurfaceFormat>
#include <QScreen>
#include <QFile>
#include <QVTKOpenGLWidget.h>

ModelRenderer::ModelRenderer(int &argc, char **argv) : QApplication(argc, argv), window(), welcomeWindow() {
    // Get Primary Screen Height
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();
    // Load in the splash screen image and scale it to 1/3 of the screen height, keeping the same aspect ratio
    QPixmap splashImage = QPixmap(":/Icons/splashscreen.png").scaledToHeight(screenHeight / 3);
    // Create and show the splash screen, keeping it on top
    QSplashScreen splashScreen(splashImage, Qt::WindowStaysOnTopHint);
    splashScreen.show();

    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
    // Open Stylesheet file
    QFile styleSheetFile(":/Icons/Combinear.qss");
    styleSheetFile.open(QFile::ReadOnly);
    // Store all the contents of the file in a string
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    // Load the stylesheet
    setStyleSheet(styleSheet);

    // TODO Check if a path was given as an argument
    //  If so, then directly load up a model window with that model
    //  Otherwise, show the welcome window, which will then load the
    //  model window based on user selection
    modelWindow.show();
    welcomeWindow.show();
}
