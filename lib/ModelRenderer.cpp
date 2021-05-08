#include "ModelRenderer.h"

#include <QSplashScreen>
#include <QSurfaceFormat>
#include <QScreen>
#include <QFile>
#include <QVTKOpenGLWidget.h>
#include <QStyleFactory>
#include <QtCore/QtCore>

#include "Utils/dark_mode_detect.h"

#define USE_FUSION

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

#ifndef USE_FUSION
    // Open Stylesheet file
    QFile styleSheetFile(":/Icons/Combinear.qss");
    styleSheetFile.open(QFile::ReadOnly);
    // Store all the contents of the file in a string
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    // Load the stylesheet
    setStyleSheet(styleSheet);
#endif

    // palette pulled heavily from https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle
    DarkPalette.setColor(QPalette::Window, QColor("#353535"));
    DarkPalette.setColor(QPalette::WindowText, Qt::white);
    DarkPalette.setColor(QPalette::Base, QColor("#2a2a2a"));
    DarkPalette.setColor(QPalette::AlternateBase, QColor("#424242"));
    DarkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    DarkPalette.setColor(QPalette::ToolTipText, QColor("#353535"));
    DarkPalette.setColor(QPalette::Text, Qt::white);
    DarkPalette.setColor(QPalette::Dark, QColor("#232323"));
    DarkPalette.setColor(QPalette::Shadow, QColor("#141414"));
    DarkPalette.setColor(QPalette::Button, QColor("#353535"));
    DarkPalette.setColor(QPalette::ButtonText, Qt::white);
    DarkPalette.setColor(QPalette::BrightText, Qt::red);
    DarkPalette.setColor(QPalette::Link, QColor("#2a82da"));
    DarkPalette.setColor(QPalette::Highlight, QColor("#2a82da"));
    DarkPalette.setColor(QPalette::HighlightedText, Qt::white);

    // rules for disabled texts
    DarkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#7f7f7f"));
    DarkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor("#7f7f7f"));
    DarkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#7f7f7f"));
    DarkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor("#505050"));
    DarkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor("#7f7f7f"));


    // configure PIT
    auto *activeTimer = new QTimer(this);
    activeTimer->setInterval(2 * 1000); //5 seconds
    activeTimer->setSingleShot(false);

#ifdef USE_FUSION
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // apply mode on start-up
    applyLightMode();

    // connect timer signal to slot
    connect(activeTimer, SIGNAL(timeout()), this, SLOT(applyLightMode()));
#endif

    // start timer - can be disabled through stop(). Useful if settings forces light/dark mode.
    activeTimer->start();


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

// live dark/light mode change
void ModelRenderer::applyLightMode() {
    if (isDark()) {
        QApplication::setPalette(DarkPalette);
    } else {
        QApplication::setPalette(LightPalette);
    }
}
