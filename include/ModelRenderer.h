#ifndef INC_KURUSVIEW_MODELRENDERER_H
#define INC_KURUSVIEW_MODELRENDERER_H

#include <QApplication>
#include <modelwindow.h>
#include <welcomewindow.h>

class ModelRenderer : public QApplication {
public:
    ModelRenderer(int &argc, char **argv);

private:
    ModelWindow modelWindow;
    WelcomeWindow welcomeWindow;
};


#endif //INC_KURUSVIEW_MODELRENDERER_H
