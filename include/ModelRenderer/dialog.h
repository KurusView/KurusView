#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

// wrap the class in Ui::
namespace Ui {
    class Dialog;
}

/// @brief Dialog object - inherits from QDialog, simple wrapper
class Dialog : public QDialog {
    /// @internal no need to have a Q_OBJECT class if we do not use slots here
Q_OBJECT

public:

    /**
    * @brief Dialog constructor
    * @param parent - the the parent widget. It is often used with *this so explicit. Defaults to null.
    */
    explicit Dialog(QWidget *parent = nullptr);

    /**
    * @brief destructor.
    */
    ~Dialog();

private:
    /// @brief ui - pointer to the ui object from QT creator. Deleted in dtor.
    Ui::Dialog *ui;
};

#endif // DIALOG_H
