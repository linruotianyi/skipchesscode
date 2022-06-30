#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "networkdata.h"
#include <QtNetwork>

class NetworkServer : public QTcpServer {
    Q_OBJECT
public:
    explicit NetworkServer(QObject* parent = nullptr);

    void send(QTcpSocket* client, NetworkData data);  //发送客户名以及操作
    NetworkServer* server;
signals:
    void receive(QTcpSocket* client, NetworkData data);
    void parseError(const InvalidMessage&);
    void leave(QTcpSocket*);           //断开连接

private slots:
    void receiveData(QObject*);
    void disconnect(QObject*);
    void newconnection();

private:
    QList<QTcpSocket*> clients;        //客户名列表
    QSignalMapper* disconnMapper;
    QSignalMapper* recvMapper;
};

#endif // NETWORKSERVER_H
