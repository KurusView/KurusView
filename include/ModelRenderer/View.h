#ifndef KURUSVIEW_VIEW_H
#define KURUSVIEW_VIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFrame>

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>

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

    int lightIntensity, lightOpacity, lightSpecularity;

    QColor backgroundColor, modelColor;


    View(const QString &borderColor, const QString &filePath, QWidget *parent = nullptr);

    void buildChain();

    void toggleShrinkFilter(bool enable);

    void toggleClipFilter(bool enable);
};


#endif //KURUSVIEW_VIEW_H
