//#include "clientwindow.h"
//#include "ui_clientwindow.h"
//#include "networkdata.h"
//#include <QMessageBox>
//#include "networksocket.h"
//ClientWindow::ClientWindow(QWidget *parent) :
//    QMainWindow(parent),
//    ui(new Ui::ClientWindow)
//{
//    ui->setupUi(this);
//    this->socket = new NetworkSocket(new QTcpSocket(), this);

//    connect(socket, &NetworkSocket::receive, this, &ClientWindow::receive);
//    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
//        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
//    });
//    QString host="10.46.33.135";   //不同电脑的host不同；测试时替换
//    quint16 port=9999;            //房间号 测试时替换；
//    socket->hello(host,port);    //尝试接入服务端
//}

//ClientWindow::~ClientWindow()
//{
//    delete ui;
//}

//void ClientWindow::receive(NetworkData *data)
//{
//   if(data->op==OPCODE::END_GAME_OP)
//   {
//       //此时应该结束游戏，并且客户端将data1内包含的玩家排名进行展示
//   }

//   else if(data->op==OPCODE::JOIN_ROOM_OP)
//   {
//       //表示有一名新玩家将要加入房间，要更新房间界面
//       //其中data1 为新玩家的用户名
//   }

//   else if(data->op==OPCODE::JOIN_ROOM_REPLY_OP)
//   {
//       //表示服务端已经接受新客户端的进入房间请求，申请加入的客户端更新自己的界面
//       //data1 表示已经在房间内的玩家的用户名，不同玩家之间用空格隔开
//       //data2 表示已经在房间内的玩家的状态，0表示未准备，1表示准备，新玩家进入后默认为0
//   }

//   else if(data->op==OPCODE::LEAVE_ROOM_OP)
//   {
//       //表示有一名玩家离开房间，服务端向剩下的所有玩家发送该op
//       //data1 为离开的房间名，data2 为离开玩家的用户名
//   }

//   else if(data->op==OPCODE::MOVE_OP)
//   {
//       //表示一名玩家进行了移动，服务端向其他玩家发送该玩家的移动操作，提示其他玩家更新棋盘
//       //data1为该玩家的初始区域编号，data2为该玩家的移动轨迹
//   }


//   /*
//      MOVE-OP 有两种错误情况，第一种为行棋非法，该op为errorop中的一部分
//      第二种为超时判负，此时的MOVE-OP为服务端向全体玩家发出的op
//      data1为该玩家所处区域，data2为-1
//      此时所有玩家应该更新界面并将超时玩家的棋子清空
//   */


//   else if(data->op==OPCODE::PLAYER_READY_OP)
//   {
//       //表示有一名玩家发送了准备请求，提示其他玩家改变该玩家的状态
//       //data1为该玩家的用户名
//   }

//   else if(data->op==OPCODE::START_GAME_OP)
//   {
//       //服务端在检查所有玩家状态并且确认所有玩家都处于准备状态后，向所有客户端发送该op
//       //data1为玩家列表，data2为每个玩家被分配的初始区域，从正上方顺时针分别为A B C D E F
//       //客户端应该对数据端进行处理，并且更新界面
//   }

//   else if(data->op==OPCODE::START_TURN_OP)
//   {
//       //当到某一个玩家下棋时，服务端向该客户端发送该指令
//       //收到该op的客户端应该重置自己的计时器并开始计时 已获胜的玩家不会收到该op
//   }

//   else if(data->op==OPCODE::END_TURN_OP)
//   {
//       //服务端在处理完客户端的 MOVE-OP 后 会向客户端传输该信号
//       //如果客户端在这一步下棋后获得胜利，应该弹出提示自己取得胜利的窗口，此后不能下棋
//   }
//   else if(data->op==OPCODE::ERROR_OP)
//   {
//       //当收到错误信息时向发送错误信息的客户端发送该op
//       //data1为错误码，data2为错误信息，错误信息可以自行处理或者留空
//       if(data->data1=="INVALID_JOIN")
//       {
//           //表示加入房间失败，可能是由用户名冲突引起的
//           //客户端需要重新发送JOIN_ROOM_OP
//       }
//       else if(data->data1=="INVALID_MOVE")
//       {
//           //表示该客户端的这一步行棋非法，应该重新下棋并发送MOVE-OP
//           //此时的倒计时不应该重置，因为服务端并不会再次向该客户端发送START-TURN-OP
//       }
//       else if(data->data1=="INVALID_REQ")
//       {
//           //表示无法解析该客户端发送的请求，客户端需要重新发送请求
//       }
//       else if(data->data1=="NOT_IN_ROOM")
//       {
//           //这个我也不是很明白，但是从字面意义上来看是说明该客户端不在房间
//           //但是不在房间，服务端为什么要理他？。。。
//       }
//       else if(data->data1=="OUTTURN_MOVE")
//       {
//           //表示还没轮到该客户端行棋，客户端只有在收到服务端的START-TURN-OP之后才能行棋
//       }
//       else if(data->data1=="ROOM_IS_RUNNING")
//       {
//           //表示当前房间正在进行游戏，应该是在玩家申请加入房间时服务端给出的提示
//       }
//       else if(data->data1=="ROOM_NOTRUNNING")
//       {
//           //表示该房间没有游戏正在进行？ 不清楚有什么作用。。。
//       }
//       else if(data->data1=="OTHER_ERROR")
//       {
//           //表示有除上述错误以外的错误
//       }
//   }
//}
