#ifndef NETWORK_H
#define NETWORK_H

#include <QMainWindow>

namespace Ui {
class NetWork;
}

class NetWork : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetWork(QWidget *parent = nullptr);
    ~NetWork();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::NetWork *ui;
};

#endif // NETWORK_H
