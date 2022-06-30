#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "networkserver.h"
#include "networkdata.h"
int ServerWindow::curplaynum=0;
ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    this->server = new NetworkServer(this);
    connect(this->server, &NetworkServer::receive, this, &ServerWindow::receive);
    quint16 port=9999;                      //测试时替换
    server->listen(QHostAddress::Any,port); //监听请求加入的客户端
    for(int i=0;i<6;i++)
    {
        ready[i]="0";                      //将初始的状态全部设置为0
    }
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::receive(QTcpSocket *player,NetworkData data)
{
    if(data.op==OPCODE::JOIN_ROOM_OP)
    {
        if(this->room=="")   //表示没有房间，需要创建一个新房间
        {
            room=data.data1;     //data1字段为客户端发送的房间号
            this->socket[curplaynum]=player;   //将客户端接入
            users[curplaynum++]=data.data2;     //第二个字段为客户端设置的用户名
            NetworkData backdata(OPCODE::JOIN_ROOM_REPLY_OP,"","");
            server->send(player,backdata);                                  //回馈信息给客户端，使其知道已经成功加入
        }
        else
        {
            if(data.data1!=room)  //？
            {
                NetworkData backdata(OPCODE::ERROR_OP,"房间号错误","");
                server->send(player,backdata);
            }
            else               //表示找到房间,接下来需要查询房间内是否有重名玩家以及游戏是否正在进行
            {
                for(int i=0;i<curplaynum;i++)     //查询是否有重名
                {
                    if(data.data2==users[i])      //表示有重名
                    {
                        NetworkData backdata(OPCODE::ERROR_OP,"重名","");
                        server->send(player,backdata);
                    }
                }
                if(curplaynum==6)    //表示人数超出上限
                {
                    NetworkData backdata(OPCODE::ERROR_OP,"超出上限","");
                    server->send(player,backdata);
                }
                else               //查询游戏是否已经开始
                {
                    int readynum=0;         //表示已经准备的人数
                    for(int i= 0; i < curplaynum;i++)
                    {
                        if(ready[i]=="1")
                        {
                            readynum++;
                        }
                    }
                    if(readynum==curplaynum)   //表示游戏已经开始
                    {
                        NetworkData backdata(OPCODE::ERROR_OP,"游戏已经开始","");
                        server->send(player,backdata);
                    }
                    else                     //无其他问题，玩家可以加入
                    {
                        users[curplaynum]=data.data2;
                        socket[curplaynum]=player;
                        QString data1=users[0];             //准备发送其他玩家的用户名
                        QString data2=ready[0];             //其它玩家的状态
                        for(int i=1;i<curplaynum;i++)
                        {
                            data1=data1+" "+users[i];
                            data2=data2+ready[i];
                        }
                        NetworkData tosingle(OPCODE::JOIN_ROOM_REPLY_OP,data1,data2);//给新加入的玩家发送其他玩家信息；
                        NetworkData toother(OPCODE::JOIN_ROOM_OP,data.data2,"");//给其他玩家发送新加入玩家的信息；
                        server->send(player,tosingle);
                        for(int i=0;i<curplaynum;i++)
                        {
                            server->send(socket[i],toother);
                        }
                        curplaynum++;                                  //表示房间增加了一个人
                    }
                }
            }
        }
    }
    else if(data.op==OPCODE::LEAVE_ROOM_OP)       //玩家离开房间
    {
        if(data.data1==room)
        {
            int leavenum;                //表示请求离开的玩家的标号
            for(leavenum=0;leavenum<6;leavenum++)
            {
                if(users[leavenum]==data.data2)
                    break;
            }
            for(int i=leavenum+1;i<6;i++)
            {
                users[i-1]=users[i];
                socket[i-1]=socket[i];
                ready[i-1]=ready[i];              //去除离开的玩家的信息
            }
            users[5]="";
            ready[5]='0';
            socket[5]=NULL;                       //将最后一个数据清零
            curplaynum--;
            NetworkData backdata(OPCODE::LEAVE_ROOM_OP,data.data2,"");
            for(int i=0;i<curplaynum;i++)
            {
                server->send(socket[i],backdata); //向其他人发送有人离开的信息
            }

        }
    }
    else if(data.op==OPCODE::PLAYER_READY_OP)
    {
        time1=QDateTime::currentSecsSinceEpoch();
        time2=QStringLiteral("%1").arg(time1);
        for(int i=0;i<curplaynum;i++)       //找到该用户
        {
            if(users[i]==data.data1)
            {
                ready[i]="1";               //修改该用户状态
                break;
            }
        }
        for(int i=0;i<curplaynum;i++)
            server->send(socket[i],data);    //像所有玩家转发该消息，表示该玩家已经准备
        //接下来考虑如果房间内人数等于2，3，6且全部准备就绪的情况，应该向所有玩家发送START_GAME_OP,并且分配好区域，提示玩家进行绘图。
        int ifready=0;    //判断所有人是否都准备完成
        for(int i=0;i<curplaynum;i++)
        {
            if(ready[i]=="1")
                ifready++;
        }
        if(ifready==curplaynum)   //所有玩家都已经准备完成
        {
            QString data1=users[0];             //准备发送其他玩家的用户名 创建用户名列表
            for(int i=1;i<curplaynum;i++)
            {
                data1=data1+" "+users[i];
            }
            if(curplaynum==2)
            {
                NetworkData backdata(OPCODE::START_GAME_OP,data1,"A D");
                //两人模式  注意，这里服务端并未给出具体多少人，客户端要根据用户列表判断当前人数
                area[0]='A';area[1]='D';
                for(int i=0;i<curplaynum;i++)
                    server->send(socket[i],backdata);
                NetworkData startturn(OPCODE::START_TURN_OP,"A",time2);     //在开始游戏后自动向第一名玩家，即定位为A区域的玩家发送开始回合的信息
                for(int i=0;i<curplaynum;i++)
                server->send(socket[i],startturn);
            }
            else if(curplaynum==3)
            {
                NetworkData backdata(OPCODE::START_GAME_OP,data1,"A C E");  //三人模式
                area[0]='A';area[1]='C';area[2]='E';
                for(int i=0;i<curplaynum;i++)
                    server->send(socket[i],backdata);
                NetworkData startturn(OPCODE::START_TURN_OP,"A",time2);     //在开始游戏后自动向第一名玩家，即定位为A区域的玩家发送开始回合的信息
                for(int i=0;i<curplaynum;i++)
                server->send(socket[i],startturn);
            }
            else if(curplaynum==6)
            {
                NetworkData backdata(OPCODE::START_GAME_OP,data1,"A B C D E F");  //六人模式
                area[0]='A';area[1]='B';area[2]='C';area[3]='D';area[4]='E';area[5]='F';
                for(int i=0;i<curplaynum;i++)
                    server->send(socket[i],backdata);
                NetworkData startturn(OPCODE::START_TURN_OP,"A",time2); //在开始游戏后自动向第一名玩家，即定位为A区域的玩家发送开始回合的信息
                for(int i=0;i<curplaynum;i++)
                server->send(socket[i],startturn);
            }
        }
    }
    else if(data.op==OPCODE::MOVE_OP) //玩家发送了行棋请求  暂时默认是合法的行棋
    {
        int curplay=0;     //发送行棋的玩家的序号
        time1=QDateTime::currentSecsSinceEpoch();
        time2=QStringLiteral("%1").arg(time1);
        for(int i=0;i<curplaynum;i++)
        {
            if(socket[i]!=player)
            server->send(socket[i],data);    //给其他玩家发送行棋信息，提示他们更新棋盘
        }
        for (curplay=0;curplay<curplaynum;curplay++)       //找到是那一个玩家发送的MOVE_OP
        {
            if(player==socket[curplay])
                break;
        }
         NetworkData startturn(OPCODE::START_TURN_OP,area[(curplay+1)%curplaynum],time2);
         for(int i=0;i<curplaynum;i++)
         server->send(socket[i],startturn);    //向下一名玩家发送开始游戏的请求
    }
    else if(data.op==OPCODE::CHECK_OP)
    {
        int curplay=0;
        for(curplay=0;curplay<curplaynum;curplay++)
        {
            if(data.data1==users[curplay])
                break;
        }
        NetworkData Check(OPCODE::CHECK_OP,"","");
        server->send(socket[curplay],Check);

    }
    else if(data.op==OPCODE::DISPLAY_OP)
    {
        for(int i=0;i<curplaynum;i++)
            server->send(socket[i],data);
    }
    // END_GAME_OP 以及 判断玩家胜利的 END_TURN_OP 暂时不写
}
