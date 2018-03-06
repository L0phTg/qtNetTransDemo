#include "servermanger.h"
#include <QHBoxLayout>
#include <QLabel>

ServerManger::ServerManger(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel(tr("hi"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
}
