#include "Eshop.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Eshop w;
    w.show();
    return a.exec();
}
