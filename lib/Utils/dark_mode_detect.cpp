#include <QtCore/QtCore>

// requires mac-specific app config
//bool macIsInDarkTheme()
//{
//    if (__builtin_available(macOS 10.14, *))
//    {
//        auto appearance = [NSApp.effectiveAppearance bestMatchFromAppearancesWithNames:
//        @[ NSAppearanceNameAqua, NSAppearanceNameDarkAqua ]];
//        return [appearance isEqualToString:NSAppearanceNameDarkAqua];
//    }
//    return false;
//}

// could be implemented in modelWindow/welcome window, and send a signal to modelRenderer.
// QPalette().color(QPalette.Window).name() should return '#edecec' on dark mode in linux/mac, but not reliable.
// changeEvent is a pure virtual slot on QWidget.
// potentially Not reliable, QEvent::PaletteChange is not always triggered on event change ...
// https://forum.qt.io/topic/99799/qt-5-12-1-and-dark-mode-on-macos-mojave-10-14/8 and tested on windows.

//void MainWindow::changeEvent( QEvent* e )
//{
//    if ( e->type() == QEvent::PaletteChange )
//    {
//        // send signal to modelRenderer to chek isDark()
//    }
//    QMainWindow::changeEvent( e );
//}


bool windowsIsInDarkTheme() {
    QSettings settings(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
                       QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
}


// https://successfulsoftware.net/2021/03/31/how-to-add-a-dark-theme-to-your-qt-application/
bool isDark() {
#ifdef Q_OS_MACX
    //return macIsInDarkTheme();
#elif defined(Q_OS_WIN)
    return windowsIsInDarkTheme();
#endif
}

