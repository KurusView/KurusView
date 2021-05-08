#include "ModelRenderer.h"

#include <QSplashScreen>
#include <QSurfaceFormat>
#include <QScreen>
#include <QFile>
#include <QVTKOpenGLWidget.h>
#include <QStyleFactory>


ModelRenderer::ModelRenderer(int &argc, char **argv) : QApplication(argc, argv), welcomeWindow() {
    // Get Primary Screen Height
    int screenHeight = QGuiApplication::primaryScreen()->geometry().height();
    // Load in the splash screen image and scale it to 1/3 of the screen height, keeping the same aspect ratio
    QPixmap splashImage = QPixmap(":/Icons/splashscreen.png").scaledToHeight(screenHeight / 3);
    // Create and show the splash screen, keeping it on top
    QSplashScreen splashScreen(splashImage, Qt::WindowStaysOnTopHint);
    splashScreen.show();

    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

//    // Open Stylesheet file
//    QFile styleSheetFile(":/Icons/Combinear.qss");
//    styleSheetFile.open(QFile::ReadOnly);
//    // Store all the contents of the file in a string
//    QString styleSheet = QLatin1String(styleSheetFile.readAll());
//    // Load the stylesheet
//    setStyleSheet(styleSheet);

    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(15, 15, 15));
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);

    palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);

    // rules for disabled texts
    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

    // enable dark mode
    QApplication::setPalette(palette);

    // switch back to light mode
//    QPalette palletblank;
//    QApplication::setPalette(palletblank);

    // TODO Check if a path was given as an argument
    //  If so, then directly load up a model window with that model
    //  Otherwise, show the welcome window, which will then load the
    //  model window based on user selection

    // If path is not given as argument, load up welcomeWindow
    if (argc == 1) {
        welcomeWindow.show();
        return;
    }

    modelWindows.push_back(std::make_shared<ModelWindow>(QString(argv[1])));
}
