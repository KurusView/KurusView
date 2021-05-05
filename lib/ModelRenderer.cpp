#include "ModelRenderer.h"

#include <QSplashScreen>
#include <QSurfaceFormat>
#include <QVTKOpenGLWidget.h>

ModelRenderer::ModelRenderer(int &argc, char **argv) : QApplication(argc, argv), window(), welcomeWindow() {
    QPixmap splashImage(":/Icons/logo.png");
    QSplashScreen splashScreen(splashImage);
    splashScreen.show();
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    window.show();
    welcomeWindow.show();
}
