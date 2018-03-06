#ifndef SERVERMANGER_H
#define SERVERMANGER_H

#include <QWidget>

namespace Ui {
class ServerManger;
}

class ServerManger : public QWidget
{
    Q_OBJECT
public:
    explicit ServerManger(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // SERVERMANGER_H
