#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registdlg.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    ui->serverProgressBar->setHidden(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_registUsrBtn_clicked()              // 注册用户
{
    registDlg *dialog = new registDlg;
    dialog->show();
}

void MainWindow::on_listenningBtn_clicked()             // 开始监听
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, ui->portLineEdit->text().toInt()))
    {
        qDebug() << tcpServer->errorString();
        close();
    }

    ui->listenningBtn->setEnabled(false);
    ui->serverStatusLabel->setText(tr("已经开始监听, 等待客户登陆."));
    // 连接信号和相应的槽函数
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void MainWindow::sendMessage()              // 向用户发送信息
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("qtDatabase");           // 数据库操作
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("qtUser");
    db.setPassword("qtL0phTg456");

    if (!db.open())
        qDebug() << "Failed to connect to qtUser admin";
    else
        qDebug() << "open database";

    QSqlQuery query(db);

    query.exec("select * from user");
    QString userName;
    QString userPwd;
    QString trueName;
    qint64 userAge;

    while(query.next()) {
        userName = query.value(1).toString();
        userPwd  = query.value(2).toString();
        trueName = query.value(3).toString();
        userAge  = query.value(4).toInt();
    }
//    tcpSocketConnection->write(id);
//    tcpSocketConnection->write(userName);
//    qint64 id = query.value(0).toInt();


    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_0);
    sendOut << "userName: " << userName << ";";
    sendOut << "userPasswd: " << userPwd << ";";
    sendOut << "trueName: " << trueName << ";";
    sendOut << "userAge: "  << userAge << ";";
    tcpSocketConnection->write(outBlock);
    outBlock.resize(0);
    qDebug() << "userName: " << userName << ";" << "userPasswd" << userPwd;
    ui->serverStatusLabel->setText("用户信息已发送, 用户可以登录");
}

void MainWindow::acceptConnection()         // 接收连接
{
    tcpSocketConnection = tcpServer->nextPendingConnection();

//    connect(tcpServerConnection, SIGNAL(readyRead()), this,
//            SLOT(updateServerProgress()));

//    tcpSocketConnection->write("hello client");
//    tcpSocketConnection->flush();
//    tcpSocketConnection->waitForBytesWritten(3000);
    ui->serverStatusLabel->setText(tr("收到用户连接, 等待用户发送数据"));
    ui->serverProgressBar->setHidden(false);
    connect(tcpSocketConnection, SIGNAL(readyRead()), this,
            SLOT(updateServerProgress()));                           // 等待用户发送信息
    //    ui->serverStatusLabel->setText(tr("已经接收到信息. 正在比对"));
    //    ui->serverProgressBar->setHidden(false);
        tcpServer->close();
    }

    void MainWindow::updateServerProgress()
    {
        QDataStream in(tcpSocketConnection);
        in.setVersion(QDataStream::Qt_5_0);
        if(bytesReceived <= sizeof(qint64)*2)
        { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
            if((tcpSocketConnection->bytesAvailable() >= sizeof(qint64)*2)
                && (fileNameSize == 0))
            { //接收数据总大小信息和文件名大小信息
                in >> totalBytes >> fileNameSize;
                bytesReceived += sizeof(qint64) * 2;
            }
            if((tcpSocketConnection->bytesAvailable() >= fileNameSize)
                && (fileNameSize != 0))
            {  //接收文件名，并建立文件
                in >> fileName;
                ui->serverStatusLabel->setText(tr("接收文件 %1 ...")
                                                .arg(fileName));
                bytesReceived += fileNameSize;
            localFile= new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                 qDebug() << "open file error!";
                 return;
            }
        }
        else return;
    }
    if(bytesReceived < totalBytes)
    {  //如果接收的数据小于总数据，那么写入文件
       bytesReceived += tcpSocketConnection->bytesAvailable();
       inBlock= tcpSocketConnection->readAll();
       localFile->write(inBlock);
       inBlock.resize(0);
    }
 //更新进度条
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesReceived);

    if(bytesReceived == totalBytes)
    { //接收数据完成时
        tcpSocketConnection->close();
        localFile->close();
        ui->listenningBtn->setEnabled(true);
        ui->serverStatusLabel->setText(tr("接收文件 %1 成功！正在准备比对")
 .arg(fileName));
    }
    ui->serverProgressBar->setHidden(true);
    if (identifyFingerInfor())
    {
        ui->serverStatusLabel->setText(tr("比对成功, 正在发送用户信息"));
        sendMessage();
    } else {


    }

}

bool MainWindow::identifyFingerInfor()
{
    return true;
}

void MainWindow::on_listUserInforBtn_clicked()          // 列出用户信息
{

}


