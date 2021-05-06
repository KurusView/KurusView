#ifndef INC_KURUSVIEW_MODELRENDERER_H
#define INC_KURUSVIEW_MODELRENDERER_H

#include <QApplication>
#include <mainwindow.h>
#include <welcomewindow.h>

class ModelRenderer : public QApplication {
public:
    ModelRenderer(int &argc, char **argv);

private:
    MainWindow modelWindow;
    WelcomeWindow welcomeWindow;
};


#endif //INC_KURUSVIEW_MODELRENDERER_H
