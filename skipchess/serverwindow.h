#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include"networkserver.h"
#include"networkdata.h"
#include <QtNetwork>
#include <QDateTime>
#include <QTime>
namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString ready[6]={0};              //用于表示接入的客户端的状态的数组，初始为零，
    QString users[6]={""};             //表示用户名
    QString room="";                   //房间号
    QTcpSocket *socket[6];             //用于存放客户端的接口，客户端发送hello后将其存入socket数组
    QString area[6];
    static int curplaynum;                           //表示当前房间人数
    qint64 time1;
    QString time2;
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
    NetworkServer* server;
private:
    Ui::ServerWindow *ui;
public slots:
    void receive(QTcpSocket *s,NetworkData data);
};

#endif // SERVERWINDOW_H
