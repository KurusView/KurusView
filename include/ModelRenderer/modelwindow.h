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
#include <vtkColor.h>

#include "Model.h"
#include "KView.h"

// wrap the class in Ui::
namespace Ui {
    class ModelWindow;
}

/**
 * @brief Holds Views and handles ui elements
 */
class ModelWindow : public QMainWindow {
Q_OBJECT

public:
    /**
     * @brief ModelWindow - Initialises a ModelWindow and loads a Model with the filePath.
     * @param filePath - The path to the file selected
     * @param parent
     * @see Model | Model
     */
    explicit ModelWindow(const QStringList &filePaths, QWidget *parent = nullptr);

    /**
     * @brief
     */
    ~ModelWindow() override;

    /**
     * @brief addViewToFrame -
     * @param view
     */
    void addViewToFrame(KView *view);

    QMenu *fileMenu;
    QMenu *recentFilesMenu;

    QAction *openAction;
    QList<QAction *> recentFileActionList;
    const unsigned int maxFileNr;

    QString currentFilePath;

    /**
     * @brief createActionsAndConnections -
     */
    void createActionsAndConnections();
    /**
     * @brief adjustForCurrentFile - Updates the list of recently opened file paths
     * @param filePath - The path to the file most recently loaded
     */
    void adjustForCurrentFile(const QString &filePath);

    /**
     * @brief updateRecentActionList -
     */
    void updateRecentActionList();

    QSettings settings;

signals:
    /**
     * @brief openNewModelWindow - Opens a new window with selected model
     * @param filePaths - The path of the file selected
     */
    void openNewModelWindow(const QStringList &filePaths);

private:
    Ui::ModelWindow *ui;
    std::vector<KView *> views;
    KView *activeView;
    QString currentModelFilePath;

    /**
     * @brief setActiveView -
     * @param newActiveView -
     */
    void setActiveView(KView *newActiveView);


    QStringList recentFilePaths;

    /**
     * @breif loadFile -
     * @param filePaths -
     */
    void loadFile(const QStringList &filePaths);

    /**
     * @brief resetViewLayout -
     */
    void resetViewLayout();


private slots:

    /**
     * @brief openRecent - Opens the recent file is selected
     */
    void openRecent();

    /**
     * @breif open - Opens selected file
     */
    void open();

    /**
     * @breif closeView -
     */
    void closeView();

public slots:

    /**
     * @brief viewActive -
     * @param event -
     */
    void viewActive(QMouseEvent *event);

    /**
     * @brief handleBackgroundColor - When the background colour button is pressed it launches a colour picker to
     * change the colour of the background
     */
    void handleBackgroundColor();

    /**
     * @brief handleModelColor - Sets the model colour
     */
    void handleModelColor();
    /**
     * @brief handleModelBackFaceColor - Sets the colour of the model's backface
     */
    void handleModelBackFaceColor();

    /**
     * @brief handleResetColor - Resets colours to the defaults that are set in the settings.
     */
    void handleResetColor();

    /**
     * @brief handleResetLighting - Resets lighting to the original settings
     */
    void handleResetLighting();

    /**
     * @brief handleLightIntensitySlider - Changes the light intensity according to the position to the slider
     * @param position - The position of the slider along the bar
     */
    void handleLightIntensitySlider(int position);

    /**
     * @brief mux_handleLightActorSlider - Changes the opacity and specularity of the model depending on their position
     * on the sliders.
     * @param position - The position of the slider along the bar
     */
    void mux_handleLightActorSlider(int position);

    /**
     * @brief handleChangePerspective -
     */
    void handleChangePerspective();

    /**
     * @brief updateFilters - Applies the filters and updates them continuously
     */
    void updateFilters();

    /**
     * @brief updateStructure -
     */
    void updateStructure();

    /**
     * @brief handleGridlines - Toggles whether grid lines are present
     */
    void handleGridlines();

    /**
     * @brief handleMeasurment - Toggles if the measurement ruler is on
     */
    void handleMeasurement();


    /**
     * @brief getStatistics - This get the Weight, Volume, Density, Centre of Gravity and Number of Cells of the
     * model.
     */
    void getStatistics();

    /**
     * @brief handleHelpButton - When pressed the GitHub repository opens
     */
    void handleHelpButton();

    /**
     * @brief Open settings menu (QDialog)
     */
    void handleSettingsButton();

    /**
     * @brief If file has not been saved before, opens file explorer to select save location, otherwise, just saves
     */
    void handleSaveButton();
};


#endif //KURUSVIEW_MODELWINDOW_H
