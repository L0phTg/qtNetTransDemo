#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QtNetwork>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QDataStream>

namespace Ui {
class loginForm;
}

class loginForm : public QWidget
{
    Q_OBJECT

public:
    explicit loginForm(QWidget *parent = 0);
    ~loginForm();

private slots:
//    void connectToServer();    // 连接服务器       delete 不用了...
//    void startTransfer();       // 发送文件大小信息
    void updateClientProgress(qint64);  // 发送数据, 更新进度条
//    void displayError(QAbstractSocket::SocketError);    // 显示错误
    void sendHeader();                // 发送指纹信息
    void openFile();            // 打开文件
    void changeStatus();        // 改变状态
    void on_connectBtn_clicked();       // 连接服务器

    void on_enterFingerInforBtn_clicked();

    void on_chooseFingerBtn_clicked();

    void on_loginBtn_clicked();
    void socketReadData();

private:
    Ui::loginForm *ui;
    QTcpSocket *tcpClient;
    QFile *localFile;           // 要发送的文件
    qint64 totalBytes;          // 数据总大小
    qint64 bytesWritten;        // 已经发送数据大小
    qint64 bytesToWrite;        // 剩余数据大小
    qint64 loadSize;           // 每次发送数据的大小
    QString fileName;           // 保存文件路径
    QByteArray outBlock;        // 数据缓冲区, 即存放每次要发送的数据
    QByteArray inBlock;         // 数据缓冲区, 每次读入的数据
};

#endif // LOGINFORM_H
