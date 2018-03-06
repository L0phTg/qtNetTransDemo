#include "loginform.h"
#include "ui_loginform.h"

loginForm::loginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginForm)
{
    ui->setupUi(this);
    loadSize = 4 * 1024;
    totalBytes = 0;
    bytesToWrite = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);
    // 当连接服务器成功时, 发出 connected() 信号, 我们开始准备录入指纹 then 登陆 传输指纹.jpg
    connect(tcpClient, SIGNAL(connected()), this, SLOT(changeStatus()));

    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,
           SLOT(updateClientProgress(qint64)));

//    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
//           SLOT(displayError(QAbstractSocket::SocketError)));

    ui->loginBtn->setEnabled(false);            // 设置 "login"不可用, 因为还没有连接, 也没用选择图片
    ui->clientProgressBar->setHidden(true);
}

loginForm::~loginForm()
{
    delete ui;
}

//void loginForm::connectToServer()              // 连接服务器, 准备发送指纹信息       delete
//{
//    ui->connectBtn->setEnabled(false);
//    bytesWritten = 0;
//    // 初始化已发送字节为 0
//    ui->clientStatusLabel->setText(tr("连接中"));
//    tcpClient->connectToHost(ui->hostLineEdit->text(),
//                             ui->portLineEdit->text().toInt());     // 连接
//}



void loginForm::changeStatus()
{
    ui->clientStatusLabel->setText(tr("连接成功, 录入指纹信息, 然后登录"));
}

void loginForm::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        ui->loginBtn->setEnabled(true);                             // 设置可以登陆btn
        ui->clientStatusLabel->setText(tr("打开图片 %1 成功. 可以登录")
                                       .arg(fileName));
    }
}

void loginForm::on_connectBtn_clicked()                             // 连接
{
    ui->connectBtn->setEnabled(false);
    bytesWritten = 0;
    // 初始化已发送字节为 0
    ui->clientStatusLabel->setText(tr("连接中"));
    tcpClient->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());     // 连接

}

void loginForm::on_enterFingerInforBtn_clicked()                    // 录入指纹
{

}

void loginForm::on_chooseFingerBtn_clicked()                        // 选择指纹
{
    openFile();
}

void loginForm::sendHeader()                                              // 发送指纹信息
{
    ui->clientProgressBar->setHidden(false);
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error";
        return ;
    }

    // 文件总大小
    totalBytes = localFile->size();

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_0);
    QString currentFileName = fileName.right(fileName.size() -
                                             fileName.lastIndexOf('/')-1);
    //依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;

    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += outBlock.size();

    sendOut.device()->seek(0);
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));

    //发送完头数据后剩余数据的大小
    bytesToWrite = totalBytes - tcpClient->write(outBlock);

//    ui->clientStatusLabel->setText(tr("指纹信息 header 已发送"));
    outBlock.resize(0);
}

//更新进度条，实现文件的传送
void loginForm::updateClientProgress(qint64 numBytes)
{
    //已经发送数据的大小
    bytesWritten += (int)numBytes;

    if(bytesToWrite > 0) //如果已经发送了数据
    {
   //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
   //就发送剩余数据的大小
       outBlock = localFile->read(qMin(bytesToWrite,loadSize));

       //发送完一次数据后还剩余数据的大小
       bytesToWrite -= (int)tcpClient->write(outBlock);

       //清空发送缓冲区
       outBlock.resize(0);

    } else {
       localFile->close(); //如果没有发送任何数据，则关闭文件
    }

    //更新进度条
    ui->clientProgressBar->setMaximum(totalBytes);
    ui->clientProgressBar->setValue(bytesWritten);

    if(bytesWritten == totalBytes) //发送完毕
    {
//     ui->clientStatusLabel->setText(tr("传送指纹信息 %1 成功")
//.arg(fileName));
       ui->clientStatusLabel->setText(tr("传送指纹信息成功"));
       localFile->close();
       connect(tcpClient, SIGNAL(readyRead()), this,
               SLOT(socketReadData));


    }
}

void loginForm::socketReadData()
{
//    inBlock = tcpClient->readAll();
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_5_0);
    QString allInfor;
    in >> allInfor;
    qDebug() << in;
    ui->clientStatusLabel->setText("登录中.");
    tcpClient->close();
}

void loginForm::on_loginBtn_clicked()                               // 登录 上传指纹信息 显示用户注册信息
{
    sendHeader();
}
