#ifndef INC_KURUSVIEW_MODELRENDERER_H
#define INC_KURUSVIEW_MODELRENDERER_H

#include <QApplication>
#include <modelwindow.h>
#include <welcomewindow.h>
#include <vector>
#include <memory>

class ModelRenderer : public QApplication {
public:
    ModelRenderer(int &argc, char **argv);

private:
    std::vector<std::shared_ptr<ModelWindow>> modelWindows;
    WelcomeWindow welcomeWindow;
};


#endif //INC_KURUSVIEW_MODELRENDERER_H
