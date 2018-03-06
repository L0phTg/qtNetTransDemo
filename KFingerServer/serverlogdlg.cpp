#include "serverlogdlg.h"
#include "ui_serverlogdlg.h"


ServerLogDlg::ServerLogDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerLogDlg)
{
    ui->setupUi(this);
}

ServerLogDlg::~ServerLogDlg()
{
    delete ui;
}

void ServerLogDlg::on_loginBtn_clicked()
{
    if (ui->rootNameEditLine->text() == tr("root") &&
            ui->rootPwdEditLine->text() == tr("123456"))
    {
        accept();
    }
    else {
        QMessageBox::warning(this, tr("Warnning"),
                             tr("用户名密码错误"),
                             QMessageBox::Yes);
        ui->rootNameEditLine->clear();
        ui->rootPwdEditLine->clear();
        ui->rootNameEditLine->setFocus();
    }
}
