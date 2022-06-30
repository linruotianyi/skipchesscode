
#ifndef ROOM_H
#define ROOM_H

#include <QMainWindow>

#include "networksocket.h"
#include "widget1.h"
#include "connectfai.h"
#include "connectsuc.h"
#include "joinfail.h"
#include "newroom.h"
#include <QDateTime>
#include <QTime>
namespace Ui {
class Room;
}

class Room : public QMainWindow
{
    Q_OBJECT

public:
    explicit Room(int,QWidget *parent = nullptr);
    ~Room();
    Widget* winArray;
    bool Readystate;
    bool inRoom; // 判断玩家有没有在房间里面
    NetworkSocket* socket;
    void try_move();
    QLabel *label;    //显示玩家名称的label
    QLabel *label2;   //显示玩家状态的label
    QLabel *label3;
    QLabel *label4;   //用于过渡
signals:

private slots:

    void receive(NetworkData data);

    void displayError();

    void connected();

     // 棋盘窗口玩家自行按下了退出按钮，对应的客户端的相应函数


    void on_Join_clicked();

    void on_Ready_clicked();



    void on_pushButton_clicked();

    void on_Leave_clicked();
    void try_check_trail();

    void on_pushButton_2_clicked();

private:
    Ui::Room *ui;
    int type; // 表示加入的房间的类型
    //用来存储玩家分配到的位置
    int player_pos;
    QString player_name; //玩家输入的昵称
    QString room_num;  // 玩家输入的房间号
    QString host;
    quint16 port;


};

//int Room :: count1 = 0; // 初始化目前玩家的数量


#endif // NETWORK_H

/*
 setLeave(QString player_num)

 //add player_num




 */
