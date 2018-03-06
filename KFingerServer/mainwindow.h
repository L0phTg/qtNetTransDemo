#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDialog>
#include <QtNetwork>
#include <QFileDevice>
#include <QFile>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_registUsrBtn_clicked();

    void on_listenningBtn_clicked();

    void on_listUserInforBtn_clicked();

    void updateServerProgress();  //更新进度条，接收数据
    void sendMessage();
    void acceptConnection();      // 收到连接

private:
    bool identifyFingerInfor();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocketConnection;
    qint64 totalBytes;      // 存放总大小信息
    qint64 bytesReceived;   // 已收到的数据的大小
    qint64 fileNameSize;    // 文件名的大小信息
    QString fileName;       // 存放文件名
    QFile  *localFile;      // 本地文件
    QByteArray inBlock;     // 数据缓冲区
    QByteArray outBlock;    // 输出数据缓冲区
};

#endif // MAINWINDOW_H
