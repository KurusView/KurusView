#ifndef KURUSVIEW_MODELWINDOW_H
#define KURUSVIEW_MODELWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QStringList>
#include <QList>

#include <memory>
#include <vector>

#include <vtkAlgorithm.h>
#include <vtkDataSetMapper.h>
#include <vtkSmartPointer.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>
#include <vtkDistanceWidget.h>
#include <QLabel>
#include <QSettings>

#include "Model.h"
#include "View.h"

namespace Ui {
    class ModelWindow;
}

/**
 * @brief Visualises a model in a user-friendly GUI
 */
class ModelWindow : public QMainWindow {
Q_OBJECT

public:
    /**
     * @brief Initialises a ModelWindow and loads a Model with the filePath.
     * @param filePath
     * @param parent
     * @see Model | Model
     */
    explicit ModelWindow(const QStringList &filePaths, QWidget *parent = nullptr);

    ~ModelWindow() override;

    void gridlinesInit(View *view);

    void addViewToFrame(View *view);

    QMenu *fileMenu;
    QMenu *recentFilesMenu;

    QAction *openAction;
    QList<QAction *> recentFileActionList;
    const int maxFileNr;

    QString currentFilePath;

    void createActionsAndConnections();

    void createMenus();

    void adjustForCurrentFile(const QString &filePath);

    void updateRecentActionList();

    QSettings settings;

signals:

    void openNewModelWindow(QStringList &filePaths);

private:
    Ui::ModelWindow *ui;
    std::vector<View *> views;
    View *activeView;
    // TODO Store file path in Model.h instead
    QString currentModelFilePath;

    void setActiveView(View *newActiveView);

    vtkSmartPointer<vtkDistanceWidget> distanceWidget;

private slots:

    void openRecent();

//    void open();

public slots:

    void viewActive(QMouseEvent *event);

    void handleBackgroundColor();

    void handleModelColor();

    void handleResetColor();

    void handleResetLighting();

    void handleLightIntensitySlider(int position);

    void mux_handleLightActorSlider(int position);

    void handleChangePerspective();

    void updateFilters();

    void updateStructure();

    void handleGridlines();

    void handleMeasurment();

    void getStatistics();
};


#endif //KURUSVIEW_MODELWINDOW_H
