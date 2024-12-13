#include "instuctorwindow.h"
#include <Mqtt_client.h>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstuctorWindow w;
    w.show();
    return a.exec();

}
