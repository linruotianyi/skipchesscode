
#include "room.h"
#include "ui_room.h"
#include <QMessageBox>
#define MainWin_Height (800)
#define MainWin_Width (700)
#include <QDebug>

Room::Room(int _type, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    // 设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    // 设置背景，最后调整外观的时候可以设置一下下面的

    QPixmap pixmap(":/image2/back8.jpg");

     QPalette pale;
     pale.setBrush(backgroundRole(), QBrush(pixmap));
     this->setPalette(pale);


    // 将当前窗口和一个客户端进行绑定

    this->socket = new NetworkSocket(new QTcpSocket(), this);
    // 建立相应的信号与槽函数的连接
    connect(socket, &NetworkSocket::receive, this, &Room::receive);
    connect(socket->base(), &QAbstractSocket::disconnected, [=](){
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));

    });
    //QString host="10.46.54.135";
    //quint16 port=16666;
    QString host="10.46.33.135";
    quint16 port=9999;
    this -> socket->hello(host,port);
    connect(socket->base(), &QAbstractSocket::errorOccurred, this, &Room::displayError);
    connect(socket->base(), &QAbstractSocket::connected, this, &Room::connected);

    //输入房间号，并且进行存储
    room_num =ui->lineEdit_room->text();
    //输入玩家名字
    player_name = ui->lineEdit_name->text();

    Readystate = 0; //当前还没有进入ready状态
    inRoom = 0; // 游戏开始之前还不算正式加入到房间
    // 保存当前的模式类型
    type = _type;

}

Room::~Room()
{
    delete winArray;
    delete socket;
    delete ui;
}


void Room :: on_Join_clicked() // enter
// 点击了进入
{

    //应向 server 发送 JOIN_ROOM_OP 请求，并将数据段 data1 和 data2 分别设置为房间号和用户名。
    NetworkData sendata = NetworkData(OPCODE :: JOIN_ROOM_OP, room_num, player_name);
    this->socket->send(sendata);

    winArray = new Widget(type,1,player_name);  // 生成对应的棋盘窗口

    // 建立棋盘窗口到当前客户端窗口的响应函数
    connect(winArray, &Widget :: exit, this, &Room::on_Leave_clicked);
    connect(winArray, &Widget :: signal_move, this, &Room :: try_move);
    connect(winArray, &Widget :: checktrail, this, &Room :: try_check_trail);
}


void Room :: on_Ready_clicked()  // ready
// 点击了ready
{

  //当玩家准备就绪时，client 应向 server 发送 PLAYER_READY_OP 请求，并将数据段 data1 设置为用户名
    NetworkData sendata = NetworkData(OPCODE :: PLAYER_READY_OP, player_name,"");
    this->socket->send(sendata);

    Readystate = 1; // 进入ready状态
}

void Room :: displayError() // 处理连接失败的槽函数
{
    //弹出连接失败并且要求重试的窗口的提示窗口
    ConnectFai* a = new ConnectFai();
    a -> show();
    //关闭当前页面
    this -> close();

}

void Room :: connected() // 处理连接成功的槽函数
{
      // 弹出连接成功的窗口
    ConnectSuc* a = new ConnectSuc();
    a -> show();
}

void Room :: on_Leave_clicked()
{
    // 在 client 离开房间之前，应向 server 发送 LEAVE_ROOM_OP 请求，并将数据段 data1 和 data2 分别设置为房间号和当前用户名
    inRoom = 0; //代表离开了房间
    NetworkData sendata = NetworkData(OPCODE::LEAVE_ROOM_OP, room_num, player_name);
    //qDebug()<<2<<" "<<player_name;
    this -> socket->send(sendata);
    this -> close(); // 用户已经离开，关闭当前的页面

}

void Room :: try_move()  // 对应的client发出move_op的请求
{
    int pos = player_pos;
    QString field;
    switch (pos)
    {
        case 1:
        {
            field = "A";
            break;
        }
        case 3:
        {
            field = "B";
            break;
        }
        case 5:
        {
            field = "C";
            break;
        }
        case 2:
        {
            field = "D";
            break;
        }
        case 4:
        {
            field = "E";
            break;
        }
        case 6:
        {
            field = "F";
            break;
        }
    }
    QString str;
    //获取行棋的路径
    str = winArray -> getTrail();
  // qDebug()<<"发送move_op";
  // qDebug()<<field<<" "<<str;
    // 发送move_up的请求
    if (str == "") str = "0 1 0 1";
    // qDebug()<<field<<" "<<str;
    NetworkData sendata = NetworkData(OPCODE::MOVE_OP,field,str);
    this->socket->send(sendata);
    //qDebug()<<"发送成功";
}

void Room::receive(NetworkData data)
{
   if(data.op==OPCODE::END_GAME_OP)
   {
       //此时应该结束游戏，并且客户端将data1内包含的玩家排名进行展示
       winArray -> endGame(data.data1);
       this -> close();  // 棋盘窗口关闭后，也关闭当前页面
   }
   else if (data.op == OPCODE :: END_TURN_OP)
   {
       // 表示目前这个玩家已经获得了胜利
       winArray -> setWin();
   }
   else if(data.op==OPCODE::JOIN_ROOM_OP)
   {
       //下面需要在每个room页面的列表里面都加上对应的玩家
       //找到第一个为空的用户名 加上新玩家的信息
       for(int i=1;i<=type;i++)
       {
         label=findChild<QLabel *>("Player"+QString::number(i)+"_name");
         label2=findChild<QLabel *>("Player"+QString::number(i)+"_state");
         if(label->text()=="")
         {
             label->setText(data.data1);
             label2->setText("0");
             break;
         }
       }

   }

   else if(data.op==OPCODE::JOIN_ROOM_REPLY_OP)
   {
       // 接收到这个请求的是一个刚刚进入房间的新玩家
       QString str = "";
       int len = data . data1.length();
       int j = 0; //表示玩家状态序号
       // 显示用户名和状态
       //qDebug()<<"this!";
       if(!data.data1.isEmpty())
       {
           for (int i = 0; i < len; i++)
               if (data . data1.at(i) == ' ')
               {
                   label=findChild<QLabel *>("Player"+QString::number(j+1)+"_name");
                   label2=findChild<QLabel *>("Player"+QString::number(j+1)+"_state");
                   label->setText(str);
                   label2->setText(data.data2.at(j));
                   str = "";
                   j++;
               }
               else  str = str + data . data1.at(i);
         // 最后一个用户末尾没有空格，不要忘记处理
         label=findChild<QLabel *>("Player"+QString::number(j+1)+"_name");
         label2=findChild<QLabel *>("Player"+QString::number(j+1)+"_state");
         label -> setText(str);
         label2 -> setText(data . data2.at(j));

         // 同时还要加上玩家自己
         label=findChild<QLabel *>("Player"+QString::number(j+2)+"_name");
         label2=findChild<QLabel *>("Player"+QString::number(j+2)+"_state");
         label -> setText(player_name);
         label2 -> setText("0");
       }
       else
       {
           ui->Player1_name->setText(player_name);
           ui->Player1_state->setText("0");
       }

   }

   else if(data.op==OPCODE::LEAVE_ROOM_OP)
   {
       //表示有一名玩家离开房间, server向剩下的所有玩家发送该op
       //data1 为离开的房间名，data2 为离开玩家的用户名
    //   qDebug()<<4<<" "<<inRoom;


      //     qDebug()<<5;
           winArray -> setLeave(data.data1);

       // 下面是将对应的玩家列表进行修改
       /*for(int i=1;i<=type;i++)
       {
           label=findChild<QLabel *>("Player"+QString::number(i)+"_name");
           if(label->text()==data.data1)
           {
               label2= findChild<QLabel *>("Player"+QString::number(i)+"_state");
               label->setText(" ");
               label2->setText(" ");
               for(int j=i;j<=type;j++)
               {
                   if(j+1<=type)
                   {
                       label3=findChild<QLabel *>("Player"+QString::number(j+1)+"_name");
                       label4=findChild<QLabel *>("Player"+QString::number(j+1)+"_state");
                       label->setText(label3->text());
                       label2->setText(label4->text());
                   }
                   else
                   {
                       label3=findChild<QLabel *>("Player"+QString::number(j)+"_name");
                       label4=findChild<QLabel *>("Player"+QString::number(j)+"_state");
                       label3->setText(" ");
                       label4->setText(" ");
                   }
               }
           break;
           }
       }*/
   }

   else if(data.op==OPCODE::MOVE_OP)
   {
       /*
          MOVE-OP data2为-1的情况：
          超时判负，此时的MOVE-OP为服务端向全体玩家发出的op
          data1为该玩家所处区域，data2为-1
          此时所有玩家应该更新界面并将超时玩家的棋子清空
       */

       //data1为该玩家的初始区域编号，data2为该玩家的移动轨迹
       // 获取行棋区域的字母
       QChar ch = data.data1.at(0);
       int this_player;  // 得到对应的玩家
       if(ch =='A')
              this_player = 1;
       else if (ch =='B')
              this_player = 3;
       else if(ch =='C')
              this_player = 5;
       else if(ch == 'D')
             this_player = 2;
       else if(ch == 'E')
             this_player = 4;
       else if(ch=='F')
             this_player = 6;

       if(data.data2 != "-1")
       {
           /*
           //下面获取该玩家行棋的起点和终点即可。
           int tempx[1000];
           int temp_num = 0;
           QString str = "";
           int len = data . data2.length();
           for (int i = 0; i <len; i++)
               if (data . data2.at(i) == ' ')
               {
                   temp_num++;
                   tempx[temp_num] = str.toInt();
                   str = "";
               }
               else str = str + data . data2.at(i);
           // 最后一个没有空格不要忘记处理
           temp_num++;
           tempx[temp_num] = str.toInt();
           int startx = tempx[1]; int starty = tempx[2];
           int endx = tempx[temp_num - 1]; int endy = tempx[temp_num];
           // 下面对棋盘做出对应的修改
           winArray -> setmove(this_player, startx, starty, endx, endy);

          */

           // 这里作出更改，需要显示完整的行棋路径
           winArray -> setmove(this_player, data.data2);

       }
       else
       {
           winArray -> setOutofTime(this_player);
       }

   }

   else if(data.op==OPCODE::PLAYER_READY_OP)
   {
      //将相应列表上玩家的状态设置为1
       for(int i=1;i<=type;i++)
       {
           label=findChild<QLabel *>("Player"+QString::number(i)+"_name");
           if(label->text()==data.data1)
             {
              label2= findChild<QLabel *>("Player"+QString::number(i)+"_state");
              label2->setText("1");
              break;
             }
       }
   }

   else if(data.op==OPCODE::START_GAME_OP)
   {
       //data1为玩家列表，data2为每个玩家被分配的初始区域
       //A B C D E F
       //1 3 5 2 4 6
       QString str = "";
       int len = data . data1.length();
       int j = 0;
       // 显示用户名和状态
       for (int i = 0; i <= len; i++)
           //最后一个后面没有空格，不要忘记处理
             if (i == len || data . data1.at(i) == ' ')
             {
                 // 找到用户名对应的编号
                 QChar field = data . data2.at(j);
                 int _pos;
                 if(field=='A')
                 {
                     _pos = 1;
                 }
                 else if(field=='B')
                 {
                     _pos = 3;
                 }
                 else if(field=='C')
                 {
                     _pos = 5;
                 }
                 else if(field=='D')
                 {
                     _pos = 2;
                 }
                 else if(field=='E')
                 {
                     _pos = 4;
                 }
                 else if(field=='F')
                 {
                      _pos = 6;
                 }
                if (player_name == str)  // 如果对应是对应当前玩家的信息
                {
                     winArray -> setplayer(_pos);  // 设置对应玩家在棋盘的位置
                    player_pos = _pos;
                }
                 winArray -> storeplayer(_pos,str);  // 不管是不是当前玩家的信息都要进行存储
                 str = "";
                 j+=2;
             }
             else  str = str + data . data1.at(i);

      // 根据传送过去的player设置对应的游戏顺序，和画出对应有棋子地方的棋盘，
       winArray -> setorder();
      //下面开始游戏，展示出棋盘画面
       winArray -> show();
       this -> hide();

   }

   else if(data.op==OPCODE::START_TURN_OP)
   {
    // 当轮到某玩家移动棋子时，server 应向全体 client 发送 START_TURN_OP 请求，并将数据段 data1 和 data2 分别设置为「行棋方的初始区域编号」和「回合开始时刻的 Unix 时间戳」
       QChar ch = data.data1.at(0); // 获取行棋区域的字母
       int this_player;
        if (ch == 'A')
           this_player = 1;
       else if (ch == 'B')
           this_player = 3;
       else if (ch == 'C')
           this_player = 5;
       else if (ch == 'D')
           this_player = 2;
       else if (ch == 'E')
           this_player = 4;
       else if (ch == 'F')
           this_player = 6;
       //在所有窗口设置好新的倒计时
       if(winArray->flag==0)
       {
           winArray->timer=new QTimer(this);
       }
       qint64 time1= QDateTime::currentSecsSinceEpoch();
       qint64 time2=data.data2.toUInt();
       qDebug()<<time1<<" "<<time2<<" "<<time1-time2;
       winArray->timer->start(1000);
       winArray->Countnum = 15-(time1-time2);
       if(winArray->flag == 0)
       {
           connect(winArray->timer,&QTimer::timeout,this->winArray,&Widget::Countdown);
           winArray->flag=1;
       }
       else
       {
           //qDebug()<<"0";
           //delete winArray->timer;
           disconnect(winArray->timer,&QTimer::timeout,this->winArray,&Widget::Countdown);
           connect(winArray->timer,&QTimer::timeout,this->winArray,&Widget::Countdown);
       }
       winArray -> player_turn(this_player);  //在所有玩家窗口设置轮换
   }

//********************************************************
   else if (data.op == OPCODE :: CHECK_OP)
   {
      // 这个来自服务端的请求，收到的客户端要求给出上一次行棋的历史路径发送给服务端
     //  qDebug() << "recieve check op " ;
       QString str;
       //获取行棋的路径
       str = winArray -> getTrail();
   //    qDebug() << str;
       // 发送display_op的请求, data1表示行棋的路径
       NetworkData sendata = NetworkData(OPCODE ::DISPLAY_OP,str,"");
       this -> socket->send(sendata);
   }
   else if (data.op == OPCODE :: DISPLAY_OP)
   {
      // 此时为查看历史路径的操作，所以对应flag为1
       winArray ->  displaypatch(data.data1,1);  // 接收到的路径之后调用接口展示
   }
//*********************************************************
   else if(data.op==OPCODE::ERROR_OP)
   {
        // 这里应该是错误码的数值的字符串形式
       if(data.data1=="400003") // 表示加入房间的时候失败
       {
           // 当前页面弹出加入失败的窗口，并且直接关闭当前窗口，提示用户重新来
           JoinFail* a = new JoinFail();
           a -> show();
           this -> close();
       }
       else if(data.data1=="400005") //非法行棋
       {
           //在所有这个房间的用户页面上提示对应的某一方非法行棋
           winArray -> moveFail(player_pos);
       }
       else if(data.data1=="400006")  // is not supposed to happen
       {
           qDebug() << "INVALID_REQ";
       }
       else if(data.data1=="400000") // is not supposed to happen
       {
           qDebug() << "NOT_IN_ROOM";
       }
       else if(data.data1=="400004") // is not supposed to happen
       {
           qDebug() << "OUTTURN_MOVE";
       }
       else if(data.data1=="400001")
       {
           //表示当前房间正在进行游戏，是在玩家申请加入房间时服务端给出的提示
           // 弹出窗口，提示当前用户换一个房间重新加入，正在进行游戏的房间不再让新的玩家加入
           NewRoom* a =new NewRoom();
           a -> show();
           this -> close();
       }
       else if(data.data1=="400002") // is not supposed to happen
       {
            qDebug() << "ROOM_NOTRUNNING";
       }
       else if(data.data1=="400007") // is not supposed to happen
       {
           //表示有除上述错误以外的错误
           qDebug() << "OTHER_ERROR";
       }
   }
}


void Room::on_pushButton_clicked()
{
    room_num =ui->lineEdit_room->text();
    player_name = ui->lineEdit_name->text();
  //   qDebug()<<room_num<<" "<<player_name;
}


void Room :: try_check_trail() // 发出查看历史路径的请求
{
 //   qDebug() << " enter try_check_trail()  " << winArray -> checkplayer ;
    NetworkData sendata = NetworkData(OPCODE ::CHECK_OP,winArray -> checkplayer,"");
    this -> socket->send(sendata);
}

void Room::on_pushButton_2_clicked()
{
    host=ui->lineEdit_Host->text();
    QString str=ui->lineEdit_Port->text();
    port=str.toUShort();
    this->socket->hello(host,port);
}

