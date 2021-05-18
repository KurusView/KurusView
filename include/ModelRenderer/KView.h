#ifndef KURUSVIEW_KVIEW_H
#define KURUSVIEW_KVIEW_H

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

#include "MModel.h"

// wrap the class in Ui::
namespace Ui {
    class View;
}

/**
 * @brief View class - abstracts all rendering duties. Has no effect on UI elements, which are handled by ModelWindow
 *
 * @note should be used as a standalone class. Intentionally disables inheritance capability.
 */
class KView final : public QWidget {
public:
    /// @TODO reconsider member visibility, use accessors
    QString borderColor;
    QVBoxLayout *verticalLayout;
    QVTKOpenGLWidget *qVTKWidget;
    std::shared_ptr<MModel> model;

    /**
     * @brief vtkModel - Unstructured grid containing all the cells of the model.
     *
     * @note used to render custom .mod models.
     */
    vtkSmartPointer<vtkAlgorithm> vtkModel;

    /// @brief STLModel - STL model source
    vtkSmartPointer<vtkSTLReader> STLModel;

    /// @brief mapper - sole rendering mapper
    vtkSmartPointer<vtkDataSetMapper> mapper;

    /// @brief filters - list or active filters.
    std::vector<vtkSmartPointer<vtkAlgorithm>> filters;

    /// @brief clipFilter - global instance of clipFilter
    vtkSmartPointer<vtkClipDataSet> clipFilter;

    /// @brief shrinkFilter - global instance of shrinkFilter
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter;

    /// @brief model properties
    int lightIntensity, lightSpecularity, modelOpacity, structure;
    QString backgroundColour, modelColour, modelBackFaceColor;
    bool isClipped, isShrunk, gridLinesEnabled, measurementEnabled;
    double density, volume, weight;
    size_t numOfCells;
    MVector centreOfGrav;
    vtkSmartPointer<vtkDistanceWidget> distanceWidget;
    QString filePath;
    std::shared_ptr<QSettings> viewSettings;

    /**
     * @brief View constructor - renders model provided by filePath. Internally calls ModelRenderer.
     * @param filePath - path to the model to be rendered
     * @param parent
     *
     * @internal Constructors that are callable with a single argument must be marked explicit to avoid unintentional
     *           implicit conversions if we implement polymorphic ctors.
     */
    explicit KView(const QString &filePath, QWidget *parent = nullptr);

    /**
     * @brief destructor - decrements instance count
     *
     * @internal final because this class is not meant to be inherited from. Doing so without final will easily lead to
     *           accidentally bypassing the instance count decrement.
     */
    ~KView() final;

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
     * @param shrinkFactor - If enable is true, and shrinkFactor is set, model will shrink to shrinkFactor,
     *                          otherwise, shrinkFactor dialog will open
     */
    bool toggleShrinkFilter(bool enable, double shrinkFactor = NULL);

    /**
     * @brief toggleClipFilter - Adds the clip filter to the back of the filter list if enabled and removes it from
     * the list of filters
     * @param enable - If the filter is activated by the user
     */
    void toggleClipFilter(bool enable);

    /**
     * @brief get the count of existing View objects
     * @return instance count
     */
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

    /**
     * @breif save - Saves the model to the opened file
     * @return Success parameter
     */
    bool save();

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

    /**
     * @internal dynamic View border colour setting
     *
     * @TODO fix bug: when opening a view in a new window, the counter should reset
     */
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


#endif //KURUSVIEW_KVIEW_H
