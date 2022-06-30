#ifndef CHOOSE1_H
#define CHOOSE1_H

#include <QMainWindow>

namespace Ui {
class Choose1;
}

class Choose1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Choose1(QWidget *parent = nullptr);
    ~Choose1();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Choose1 *ui;
};

#endif // CHOOSE1_H
