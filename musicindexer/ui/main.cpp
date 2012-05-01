#include <QtGui/QApplication>
#include "controller/controller.h"
#include "fullscreenview.h"
#include "qtwin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Controller c;

    //c.init();
    FullScreenView *screen = new FullScreenView();

#ifdef Q_WS_X11
    screen->setAttribute(Qt::WA_TranslucentBackground);
    screen->setAttribute(Qt::WA_NoSystemBackground, false);
    QPalette pal = window.palette();
    QColor bg = pal.window().color();
    bg.setAlpha(180);
    pal.setColor(QPalette::Window, bg);
    screen->setPalette(pal);
    screen->ensurePolished(); // workaround Oxygen filling the background
    screen->setAttribute(Qt::WA_StyledBackground, false);
#endif

#ifdef Q_OS_WIN
    if (QtWin::isCompositionEnabled()) {
        //QtWin::extendFrameIntoClientArea(screen);
        QtWin::enableBlurBehindWindow(screen);
        screen->setContentsMargins(0, 0, 0, 0);
    }
#endif

    screen->show();

    return a.exec();
}
