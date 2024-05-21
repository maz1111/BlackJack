#include "BlackJack.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BlackJack w;
    w.show();
    return a.exec();
}
