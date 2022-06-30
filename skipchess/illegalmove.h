#ifndef ILLEGALMOVE_H
#define ILLEGALMOVE_H

#include <QMainWindow>

namespace Ui {
class illegalMove;
}

class illegalMove : public QMainWindow
{
    Q_OBJECT

public:
    explicit illegalMove(QWidget *parent = nullptr);
    ~illegalMove();
signals:

private slots:
    //void on_pushButton_clicked();


private:
    Ui::illegalMove *ui;
};

#endif

