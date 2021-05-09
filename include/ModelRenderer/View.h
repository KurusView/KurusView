#ifndef KURUSVIEW_VIEW_H
#define KURUSVIEW_VIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <QColor>

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>
#include <vtkDistanceWidget.h>

#include "Model.h"


class View : public QWidget {
public:
    QString borderColor;

    QVBoxLayout *verticalLayout;

    QVTKOpenGLWidget *qVTKWidget;
    Model model;
    vtkSmartPointer<vtkDataSetMapper> mapper;
    std::vector<vtkSmartPointer<vtkAlgorithm>> filters;
    vtkSmartPointer<vtkClipDataSet> clipFilter;
    vtkSmartPointer<vtkShrinkFilter> shrinkFilter;

    int lightIntensity, lightSpecularity, modelOpacity, structure;

    QString backgroundColour, modelColor;

    bool isClipped, isShrunk, gridLinesEnabled, measurementEnabled;

    View(const QString &borderColor, const QString &filePath, QWidget *parent = nullptr);

    virtual ~View();

    void buildChain();

    void toggleShrinkFilter(bool enable);

    void toggleClipFilter(bool enable);

    // return instance count
    static unsigned short int getCount();

    void setModelColor(const QColor &color);

    void setBackgroundColor(const QColor &color);

    double density, volume, weight;

    unsigned long numOfCells;

    MVector centreOfGrav;

    vtkSmartPointer<vtkDistanceWidget> distanceWidget;

    void setLightIntensity(int value);

    void setLightSpecularity(int value);

    void setModelOpacity(int value);

    void resetLighting();

    void toggleGridLines(bool enable);

    void toggleMeasurement(bool enable);

    void setStructure(int selectedStructure);


private:

    /**
     *  @brief holds count of view windows
     *  @internal used to set fitting format and limit the number of views to 4
     */
    static unsigned short int ViewInstanceCount;
};


#endif //KURUSVIEW_VIEW_H
