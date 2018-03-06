#ifndef SERVERLOGDLG_H
#define SERVERLOGDLG_H

//#pragma execution_character_set("utf-8")

#include <QDialog>
#include <QMessageBox>
#include <QString>

namespace Ui {
class ServerLogDlg;
}

class ServerLogDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ServerLogDlg(QWidget *parent = 0);
    ~ServerLogDlg();

private slots:
    void on_loginBtn_clicked();

private:
    Ui::ServerLogDlg *ui;
};

#endif // SERVERLOGDLG_H
