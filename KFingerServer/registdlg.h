#ifndef REGISTDLG_H
#define REGISTDLG_H

#include <QDialog>

namespace Ui {
class registDlg;
}

class registDlg : public QDialog
{
    Q_OBJECT

public:
    explicit registDlg(QWidget *parent = 0);
    ~registDlg();

private slots:
    void on_registBtn_clicked();

private:
    Ui::registDlg *ui;
    qint8 userId;
};

#endif // REGISTDLG_H
