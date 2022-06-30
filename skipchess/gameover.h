
#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QMainWindow>
#include <QLabel>
namespace Ui {
class GameOver;
}

class GameOver : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameOver(QWidget *parent = nullptr);
    GameOver(QString str, QWidget *parent = nullptr);
    QLabel *label;
    ~GameOver();
signals:

private slots:

//    void on_pushButton_clicked();

private:
    Ui::GameOver *ui;
};

#endif // NETWORK_H
