#ifndef NETCHOOSE_H
#define NETCHOOSE_H

#include <QMainWindow>
#include "room.h"
namespace Ui {
class NetChoose;
}

class  NetChoose : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetChoose(QWidget *parent = nullptr);
    ~NetChoose();
signals:

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::NetChoose *ui;
};

#endif // NETWORK_H
