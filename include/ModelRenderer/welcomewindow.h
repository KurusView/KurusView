#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QSettings>
#include <QObject.h>
#include <QPoint.h>
#include <QGridLayout>
#include <QScrollArea>

namespace Ui {

    class WelcomeWindow;
}

class QFrame;


class WelcomeWindow : public QMainWindow {
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
    Ui::WelcomeWindow *ui;
    QStringList recentFilePaths;

    QScrollArea *scrollAreaX;
    QGridLayout *mainLayout;
    QFrame *xFrame;

    const int maxFileNr;

    QSettings settings;

    QImage *img;
    QPixmap p;

    QFrame *
    /**
     *
     * @brief Adds a new row to the displayed recent files .
     * @param number Is the row index
     * @param title Is the name of the file
     * @param subtitle Is the path of the file
     * @param generalFontsize Is the font size of the text
     *
     */
    CreateNewRow(int number, QString title, QString subtitle, int generalFontSize = 18);

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

    void fileSelected(const QStringList &filePaths);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // WELCOMEWINDOW_H
