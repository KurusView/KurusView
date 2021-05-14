#ifndef KURUSVIEW_VIEW_H
#define KURUSVIEW_VIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QColor>
#include <QSettings>
#include <QString>

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>
#include <vtkDistanceWidget.h>
#include <vtkAlgorithm.h>
#include <vtkSTLReader.h>

#include "Model.h"


class View : public QWidget {
public:
    QString borderColor;

    QVBoxLayout *verticalLayout;

    QVTKOpenGLWidget *qVTKWidget;
    std::shared_ptr<Model> model;
    /** @brief vtkModel - Unstructured grid containing all the cells of the model */
    vtkSmartPointer<vtkAlgorithm> vtkModel;
    vtkSmartPointer<vtkSTLReader> STLModel;

    vtkSmartPointer<vtkDataSetMapper> mapper;
    std::vector<vtkSmartPointer<vtkAlgorithm>> filters;
    vtkSmartPointer<vtkClipDataSet> clipFilter;
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter;

    int lightIntensity, lightSpecularity, modelOpacity, structure;

    QString backgroundColour, modelColour, modelBackFaceColor;

    bool isClipped, isShrunk, gridLinesEnabled, measurementEnabled;

    View(const QString &filePath, QWidget *parent = nullptr);

    virtual ~View();

    /**
     * @brief buildVTKModelFromMod - Builds model from cells and vectors loaded from a proprietary model file
     */
    void buildVTKModelFromMod();

    /**
     * @brief loadSTLModel - Loads an model with STL filetype
     * @param filePath - The path of the file that is selected
     */
    void loadSTLModel();

    /**
     * @brief buildChain -
     */
    void buildChain();

    /**
     * @brief toggleShrinkFilter - Adds the clip filter to the back of the filter list if enabled and removes it from
     * the list of filters
     * @param enable - If the filter is activated by the user
     */
    void toggleShrinkFilter(bool enable);

    /**
     * @brief toggleClipFilter - Adds the clip filter to the back of the filter list if enabled and removes it from
     * the list of filters
     * @param enable - If the filter is activated by the user
     */
    void toggleClipFilter(bool enable);

    // return instance count
    static unsigned short int getCount();

    /**
     * @brief setModelColor - Sets the colour of the model
     * @param color - The colour the model is set to
     */
    void setModelColor(const QColor &color = nullptr);

    /**
     * @brief setModelBackFaceColor - Sets the colour of the face of the model that is but by the clip filter
     * @param color - The colour the back face of the model is set to
     */
    void setModelBackFaceColor(const QColor &color);

    /**
     * @brief setBackgroundColor - Sets the background colour of the model
     * @param color - The colour the model is set to
     */

    void setBackgroundColor(const QColor &color);

    /**
     * @brief resetColour - Resets all colours of the model according to defaults.
     */
    void resetColour();


    double density, volume, weight;

    unsigned long numOfCells;

    MVector centreOfGrav;


    vtkSmartPointer<vtkDistanceWidget> distanceWidget;


    /**
     * @brief setLightIntensity - Sets the light intensity of the model
     * @param value - The value of light intensity depending on the position of the slider
     */
    void setLightIntensity(int value);

    /**
     * @brief setLightSpecularity - Set the specularity of the model
     * @param value - The value of the specularity depending on the position of the slider
     */
    void setLightSpecularity(int value);

    /**
     * @brief setModelOpacity - Sets the opacity of the model
     * @param value - The value of opacity depending on the position of the slider
     */
    void setModelOpacity(int value);

    /**
     * @brief resetLighting - Resets the light parameters
     */
    void resetLighting();

    /**
     * @brief toggleGridLines - Toggles the appearence of gridlines on the model
     * @param enable - Whether the checkbox is enabled or disabled
     */
    void toggleGridLines(bool enable);

    /**
     * @brief toggleMeasurements - Toggles the ability to measure the distance between point on the model
     * @param enable - If the measurement has been toggled on or off
     */
    void toggleMeasurement(bool enable);

    /**
     * @brief setStructure -
     * @param selectedStructure -
     */
    void setStructure(int selectedStructure);

    QString filePath;

    std::shared_ptr<QSettings> viewSettings;

    /**
     * @breif save - Saves the model to the opened file
     */
    void save();

    /**
     * @breif saveAs - Saves the current model to new file
     */
    void saveAs();

private:

    /**
     *  @brief holds count of view windows
     *  @internal used to set fitting format and limit the number of views to 4
     */
    static unsigned short int ViewInstanceCount;

    QString borderColors[4] = {"red", "blue", "cyan", "magenta"};

    /**
     * @brief gridlinesInit - Initialises the gridlines
     */
    void gridlinesInit();

    /**
     * @brief populateSettings - Sets all the parameters
     */
    void populateSettings();

};


#endif //KURUSVIEW_VIEW_H
