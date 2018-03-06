#include "mainwindow.h"
#include <QApplication>
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    loginForm login;
    login.show();
//    w.show();

    return a.exec();
}
