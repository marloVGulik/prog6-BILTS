#include "instuctorwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstuctorWindow w;
    w.show();
    return a.exec();
}
