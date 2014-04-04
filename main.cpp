#include "qronos.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qronos w;
    w.show();

    return a.exec();
}
