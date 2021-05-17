#ifndef KURUSVIEW_CLICKABLEQLABEL_H
#define KURUSVIEW_CLICKABLEQLABEL_H


#include <QLabel>
#include <QWidget>
#include <Qt>

/**
 * @brief clickableQLabel - a wraper around QLabel to provide right-click functionality. Underlines and highlights text
 *        in blue.
 *
 * @TODO get highlight colour from theme palette.
 */

class clickableQLabel : public QLabel {
    /// @internal need to have declare class as Q_OBJECT to handle signals/slots
Q_OBJECT

public:
    /**
     * @brief constructor - simply wraps QLabel params
     */
    explicit clickableQLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    /// @brief default destructor. Can be overriden by children
    ~clickableQLabel() override = default;


protected:
    /**
     * @brief
     * @param event
     *
     * @internal this class may potentially be inherited from often, carefully consider private vs protected visibility.
     */
    void mousePressEvent(QMouseEvent *event) override;

};

#endif //KURUSVIEW_CLICKABLEQLABEL_H
