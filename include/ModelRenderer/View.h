#ifndef KURUSVIEW_VIEW_H
#define KURUSVIEW_VIEW_H

#include <QWidget>
#include <QVTKOpenGLWidget.h>


class View : public QWidget {
public:
    /// @brief set if current view is focused
    bool isActive;

    QVTKOpenGLWidget qvtkWidget;


};


#endif //KURUSVIEW_VIEW_H
