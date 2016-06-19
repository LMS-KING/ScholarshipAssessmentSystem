#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login w;
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();

    return a.exec();
}
