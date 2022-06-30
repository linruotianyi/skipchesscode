#ifndef CONNECTFAI_H
#define CONNECTFAI_H

#include <QMainWindow>

namespace Ui {
class ConnectFai;
}

class ConnectFai : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectFai(QWidget *parent = nullptr);
    ~ConnectFai();
signals:

private slots:
    //void on_pushButton_clicked();


private:
    Ui::ConnectFai *ui;
};

#endif

