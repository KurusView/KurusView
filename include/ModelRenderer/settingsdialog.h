#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class settingsDialog;
}

class settingsDialog : public QDialog {
Q_OBJECT

public:
    /**
     * @breif settingsDialog -
     * @param parent -
     */
    explicit settingsDialog(QWidget *parent = nullptr);

    /**
     * @brief ~settingsDialog - Destructor
     */
    ~settingsDialog();

    /**
     * @brief getDefault_saveLocation - Gets the default save location
     * @return - The path to the default save location
     */
    static QString getDefault_saveLocation();

    /**
     * @brief getDefault_maxModelsPerWindow - Get the default amount of maximum models per window
     * @return - The maximum number of models to display in a window
     */
    static unsigned short int getDefault_maxModelsPerWindow();

    /**
     * @brief getDefault_displayRecents - Get the maximum number of recently opened file displayed
     * @return - The maximum number of recents displayed
     */
    static int getDefault_displayRecents();

    /**
     * @brief getDefault_alwaysOpenModelToNewWindow - Whether everytime a new model is loaded a new window opens
     * @return - Whether a new window is opened everytime a new file is loaded
     */
    static bool getDefault_alwaysOpenModelToNewWindow();

    /**
     * @brief getDefault_modelColour - Gets the default model colour
     * @return - The default model colour
     */
    static QColor getDefault_modelColour();

    /**
     * @brief getDefault_modelBackFaceColour - Gets the default model colour
     * @return - The default model colour
     */
    static QColor getDefault_modelBackFaceColour();

    /**
     * @breif getDefault_backgroundColour - Gets the default background colour
     * @return - The default background colour
     */
    static QColor getDefault_backgroundColour();


private:
    Ui::settingsDialog *ui;

    QString defaultSaveLocation;

    unsigned short int maxModelsPerWindow;

    unsigned int displayRecents;

    bool alwaysOpenModelToNewWindow;

    QColor defaultModelColour, defaultBackFaceColour, defaultBackgroundColour;

private slots:

    /**
     * @brief acceptDialog - Saves the data in the settings dialog when they are changed
     */
    void acceptDialog();

    /**
     * @brief rejectDialog - Cancels the change in the settings dialoag data.
     */
    void rejectDialog();

    /**
     * @brief handleBrowseButton - Opens the file explorer to choose the default save path location
     */
    void handleBrowseButton();

    /**
     * @brief handleModelsPerWindowSpin - Determines the maximum number of models per window displayed
     * @param i - The number of models per window
     */
    void handleModelsPerWindowSpin(int i);

    /**
     * @brief handleAlwaysOpenCheck - Determines if a new model is always opened in a new window
     */
    void handleAlwaysOpenCheck();

    /**
     * @brief handleDisplaySpin - Determines the number recent files displayed
     * @param i - The number of recent files that are displayed
     */
    void handleDisplayRecentsSpin(int i);

    /**
     *  @brief handleModelColour - Sets the model colour
     */
    void handleModelColour();

    /**
     * @brief handleModelBackfaceColour - Sets the colour of model's backface
     */
    void handleModelBackfaceColour();

    /**
     * @brief handleModelBackgroundColour - Sets the background colour of the model
     */
    void handModelBackgroundColour();

};

#endif // SETTINGSDIALOG_H
