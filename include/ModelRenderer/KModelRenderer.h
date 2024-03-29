#ifndef INC_KURUSVIEW_KMODELRENDERER_H
#define INC_KURUSVIEW_KMODELRENDERER_H

#include <QApplication>
#include <QString>
#include <QStringList>
#include <modelwindow.h>
#include <welcomewindow.h>
#include <vector>
#include <memory>

/**
 * @brief This class acts as the base for the KurusView GUI Application, handling windows and initialisation.
 */
class KModelRenderer : public QApplication {
Q_OBJECT
public:
    /**
     * @brief Pass a model file path as an argument to directly load that model.
     * @param argc Number of arguments passed to the program
     * @param argv Array of strings containing the arguments
     */
    KModelRenderer(int &argc, char **argv);

    virtual ~KModelRenderer();

public slots:
    void openFile(const QStringList &filePaths);

    void applyLightMode();

private:
    /**
     * @brief List of modelWindows, each visualising a different model that was loaded
     */
    std::vector<std::shared_ptr<ModelWindow>> modelWindows;
    /**
     * @brief Welcome window, only shows if no model is currently open
     */
    WelcomeWindow welcomeWindow;

    QPalette LightPalette;
    QPalette DarkPalette;

    QTimer *activeTimer;
};


#endif //INC_KURUSVIEW_KMODELRENDERER_H
