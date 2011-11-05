#include <QtGui/QApplication>
#include "controller/controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller c;

    c.init();

    return a.exec();
}
