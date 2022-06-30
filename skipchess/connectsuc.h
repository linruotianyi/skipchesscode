#ifndef CONNECTSUC_H
#define CONNECTSUC_H

#include <QMainWindow>

namespace Ui {
class ConnectSuc;
}

class ConnectSuc : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectSuc(QWidget *parent = nullptr);
    ~ConnectSuc();
signals:

private slots:
    //void on_pushButton_clicked();


private:
    Ui::ConnectSuc *ui;
};

#endif

