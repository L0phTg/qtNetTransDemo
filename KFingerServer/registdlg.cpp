#include "registdlg.h"
#include "ui_registdlg.h"
#include <QSql>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

registDlg::registDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registDlg)
{
    ui->setupUi(this);
}

registDlg::~registDlg()
{
    delete ui;
}

void registDlg::on_registBtn_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("qtDatabase");
    db.setUserName("qtUser");
    db.setPassword("qtL0phTg456");
    if (!db.open())
        qDebug() << "failed login mysql qtUser";
    qDebug() << "open";

    QSqlDatabase::database().transaction();
    QSqlQuery query(db);


    query.prepare("insert into user (id, userName, userPasswd, trueName, userAge) values (:id, :userName, :userPasswd, :trueName, :userAge");

    query.bindValue(":userName", ui->userNameLineEdit->text());
    query.bindValue(":userPasswd", ui->userPwdLabel->text());
    query.bindValue(":trueName", ui->trueNameLineEdit->text());
    query.bindValue(":userAge", ui->ageLineEdit->text().toInt());

//    if (!query.execBatch()) {
//        QMessageBox::critical(0, QObject::tr("Database Error"),
//                              tr("hihi"));
//    }
    query.exec();

    qDebug() << "insert ok";
    query.exec("select * from user");
    while (query.next()) {
        qint32 id = query.value(0).toInt();
        qDebug() << id << " ";
    }
    QSqlDatabase::database().commit();
}
