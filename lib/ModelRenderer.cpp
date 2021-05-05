#include "ModelRenderer.h"

#include <QSurfaceFormat>
#include <QVTKOpenGLWidget.h>

ModelRenderer::ModelRenderer(int &argc, char **argv) : QApplication(argc, argv), window(), welcomeWindow() {
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    window.show();
    welcomeWindow.show();
}
