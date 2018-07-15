#include <QCoreApplication>
#include "kcmcolorfulhelper.h"

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    KcmColorfulHelper kch(argc, argv);
    kch.run();

//    return a.exec();
    return 0;
}
