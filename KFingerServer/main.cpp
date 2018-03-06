#include "mainwindow.h"
#include <QApplication>
#include "serverlogdlg.h"


//#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ServerLogDlg dlg;

    if (dlg.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }

    return a.exec();
}
