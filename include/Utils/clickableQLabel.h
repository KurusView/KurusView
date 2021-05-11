#ifndef KURUSVIEW_CLICKABLEQLABEL_H
#define KURUSVIEW_CLICKABLEQLABEL_H


#include <QLabel>
#include <QWidget>
#include <Qt>

class clickableQLabel : public QLabel {
Q_OBJECT

public:
    explicit clickableQLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~clickableQLabel() override =default;


protected:
    void mousePressEvent(QMouseEvent* event) override;

};

#endif //KURUSVIEW_CLICKABLEQLABEL_H
