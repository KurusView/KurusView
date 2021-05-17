#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSettings>
#include <QObject.h>
#include <QPoint.h>
#include <QGridLayout>
#include <QScrollArea>

// wrap the class in Ui
namespace Ui {
    class WelcomeWindow;
}

class QFrame;


class WelcomeWindow : public QMainWindow {
    /// @internal need to have declare class as Q_OBJECT to handle signals/slots
Q_OBJECT

public:

    explicit WelcomeWindow(QWidget *parent = nullptr);

    /**
     * @brief addToRecentFiles - adds the most recent file opened to the top of a recentFilePaths list
     * @param inputFileNames
     */
    void addToRecentFiles(QStringList &inputFileNames);

    /**
    * @brief ~WelcomeWindow - Default Destructor
    */
    ~WelcomeWindow();

private:
    /// @brief ui - pointer to the ui object from QT creator. Deleted in dtor.
    Ui::WelcomeWindow *ui;

    /// @brief recentFilePaths - list of paths to recently opened models
    QStringList recentFilePaths;

    /// @internal the a xFrames are constructed inside of mainLayout, which is placed in a ScrollArea, from the recents lsit
    QScrollArea *scrollAreaX;
    QGridLayout *mainLayout;

    /// @brief xFrame - each recent model gets its own xFrame, containing icon, index, filepath, and model name
    QFrame *xFrame;

    /// @brief maxFileNr - maximum number of models to display on recents, set by settings.
    const int maxFileNr;

    /// @brief settings - accessor to global settings
    QSettings settings;

    /// @binternal recents list icon placeholders. We use pixelmaps to apply size transformations.
    QImage *img;
    QPixmap p;

    /**
     *
     * @brief Adds a new row to the displayed recent files .
     * @param number Is the row index
     * @param title Is the name of the file
     * @param subtitle Is the path of the file
     * @param generalFontsize Is the font size of the text
     *
     */
    QFrame *CreateNewRow(int number, QString title, QString subtitle, int generalFontSize = 18);

    /**
     *
     * @brief Populates the recently opened files list in the opening window.
     */
    void populateRecents();

    /**
     * @brief Opens the right click context menu after a right click.
     * @param pos Is the position of the cursor.
     */
    void showContextMenu(const QPoint &pos);

    /**
     * @brief Opens the model after it is selected.
     * @param paths the path of the file(s).
     */
    void loadModel(const QStringList &paths);


public slots:

    /**
     * @breif Opens the file explorer to select a file to open.
     */
    void handleOpenButton();

    /**
     * @breif Opens the GitHub repository for the program
     */
    void handleAboutButton();

    /**
     * @brief Open settings menu (QDialog)
     */
    void handleSettingsButton();

    /**
     * @brief Removes the file selected from recents.
     */
    void removeEntryFromRecents();

    /**
     * @brief Copies the path of the file selected.
     */
    void copyEntryToClipboard();

    /**
     * @brief Loads the file(s) from recents.
     */
    void loadEntryAsModel();

signals:

    /// @brief signal caught by ModelWindow to render a new model when selected from the list or from the open menu
    void fileSelected(const QStringList &filePaths);

protected:
    /**
     * @brief used to catch events from mouse.
     * @internal right click dialog currently bypasses this function. Wrap around it or stop using it.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WELCOMEWINDOW_H
