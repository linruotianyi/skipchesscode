#include "widget1.h"
#include "ui_Widget.h"
#include <QVector>
#include <vector>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QDebug>
#include<QSize>
//#include "change.h"
#include <QWaitCondition>
#include <QTime>
#include <QLabel>
#include<QtMath>
#include<QDebug>
#include <QColor>
#include <QPair>
int Widget::flag=0;
Widget::Widget(int _type,bool _ifcon,  QString _name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
   //***** visual basic
    QPixmap pixmap(":/image2/back8.jpg");

     QPalette pale;
     pale.setBrush(backgroundRole(), QBrush(pixmap));
     this->setPalette(pale);
    ui->setupUi(this);  //使用ui文件
    //下面是设置固定的屏幕大小
    this->setMinimumSize(800, 700);
    this->setMaximumSize(800, 700);

   //**** end of basic


    //**** 只是获得了基本的坐标参数，没有固定模式，也没有展示画出棋盘， 在任何情况下这部分的是一致的
    chess_array.resize(10);
    setCoordinate();
    //初始化对角数组
    corner[1] = 2; corner[2] = 1;
    corner[3] = 4; corner[4] = 3;
    corner[5] = 6; corner[6] = 5;
    // 为AI模式预处理一下hash相关的数组
    hash_mark = 0;
    base_pow[0] = 1;
    for (int i = 1; i <= chess_total; i++)
       base_pow[i] = (base_pow[i-1] * (base_p)) % (int) mod_p;
    base_player[0] = 1;
    for (int i = 1; i < 6; i++)
       base_player[i] = (base_player[i-1] * base_pow[chess_total]) % (int)mod_p;

    AI_on = 0; // 初始化的时候不开启AI模式
    PathPainter = nullptr;
    success_num = 0; // 初始化情况下胜利的玩家人数为1
    coordinate_mark = 0;
    select_count = 0;
    Trail_off = 0; // 初始化的时候轨迹是开着的
    for (int i = 0; i < 10; i++)
    {
        level[1][i] = i;
        level[2][i] = i;
    }
    level[4][0] = 9; level[4][1] = 3; level[4][2] = 8;
    level[4][3] = 2; level[4][4] = 4; level[4][5] = 7;
    level[4][6] = 0; level[4][7] = 1; level[4][8] = 5; level[4][9] = 6;

    level[6][0] = 9; level[6][1] = 3; level[6][2] = 8;
    level[6][3] = 2; level[6][4] = 4; level[6][5] = 7;
    level[6][6] = 0; level[6][7] = 1; level[6][8] = 5; level[6][9] = 6;

    level[5][0] = 6; level[5][1] = 5; level[5][2] = 7;
    level[5][3] = 1; level[5][4] = 4; level[5][5] = 8;
    level[5][6] = 0; level[5][7] = 2; level[5][8] = 3; level[5][9] = 9;

    level[3][0] = 6; level[3][1] = 5; level[3][2] = 7;
    level[3][3] = 1; level[3][4] = 4; level[3][5] = 8;
    level[3][6] = 0; level[3][7] = 2; level[3][8] = 3; level[3][9] = 9;

    level_sep[0] = 0; level_sep[1] = 2;  level_sep[2] = 5; level_sep[3] = 9;


    // *****  end


    player_name = _name;
    this -> type = _type;
    ifconnected = _ifcon;
    if (!ifconnected) // 单机模式可以即刻绘制出棋盘
    {
        this->clickTime = 0;
        this->control = 1; // 初始化是第一个玩家操作
        this->select = nullptr;
        this->present_order = 1;
        if (type == 2)
            set2people_mode();
        else if (type == 3)
            set3people_mode();
        else
            set6people_mode();
    }

}

void Widget :: setplayer(int _player)  // 代表当前窗口的玩家对应哪一个角
{
    player = _player;
}
void Widget :: storeplayer(int _player, QString _name) // 存储所有参与到本局游戏里面的玩家的信息
{
    //将玩家加入到房间里面，初始化玩家还没有获胜
    ishere[_player] = 1;
    judgement[_player] = 0;
    id_name[_player] = _name;
}
void Widget :: setorder() // 根据传进来的参数设置玩家的顺序
{
    int temp = 0;
    //下面按照顺时针的顺序依次判断各个位置，看看哪个位置上面被分配了玩家
    // 轮换会由服务端进行控制，这里存储下顺序是为了AI部分的使用
    // player_order[i]依次存储了第一个下棋到最后一个下棋的玩家顺序
    if (ishere[1]) {temp++; player_order[temp] = 1;}
    if (ishere[3]) {temp++; player_order[temp] = 3;}
    if (ishere[5]) {temp++; player_order[temp] = 5;}
    if (ishere[2]) {temp++; player_order[temp] = 2;}
    if (ishere[4]) {temp++; player_order[temp] = 4;}
    if (ishere[6]) {temp++; player_order[temp] = 6;}
    //设置初始化参数
    this->clickTime = 0;
    this->control = player_order[1]; // 初始化是第一个玩家操作
    this->temp_control = player_order[1];
    this->select = nullptr;

    // 调整视角，使得玩家的棋子在正下方，顺时针旋转
    // 获取Coordinate_pos的值，对应表示每个玩家在展示画面的时候应该对应Coordinate的哪一个横坐标
    // re_Coordinate_pos则应该是一个反向的函数，表示这个坐标下对应展现的是哪一个玩家的棋子
    int temp_pos[10];
    temp_pos[1] = 1; temp_pos[2] = 3; temp_pos[3] = 5;
    temp_pos[4] = 2; temp_pos[5] = 4; temp_pos[6] = 6;
    // temp_bias[i]代表i号玩家在正下方的时候应该顺时针转动多少个位置

    // 这样设置就可以取消旋转的效果
  //  for (int i = 1; i <= 6; i++) temp_bias[i] = 0;

    // 下面是带有旋转效果的情况


    temp_bias[2] = 0; temp_bias[5] = 1; temp_bias[3] = 2;
    temp_bias[1] = 3; temp_bias[6] = 4; temp_bias[4] = 5;


    // 下面根据当前的玩家来确定每个玩家在展示的时候对应的位置
        temp = 0;
        for (int i = 1 + temp_bias[player]; i <= 6; i++)
        {
            temp++;
            Coordinate_pos[temp_pos[temp]] = temp_pos[i];
            re_Coordinate_pos[temp_pos[i]] = temp_pos[temp];
        }
        for (int i = 1; i <= temp_bias[player]; i++)
        {
            temp++;
            Coordinate_pos[temp_pos[temp]] = temp_pos[i];
            re_Coordinate_pos[temp_pos[i]] = temp_pos[temp];
        }


    // 根据参数绘制对应的棋盘
    if (type == 2)
        set2people_mode();
    else if (type == 3)
        set3people_mode();
    else
        set6people_mode();

}
void Widget :: player_turn(int _player)  // 响应服务器进行轮换
{
    // 移交控制权
    if (AI_on)
    {
        this -> temp_control = _player;
        this -> control = -1; // 此时禁止玩家自己下棋，而是AI托管
        AI_move(); // 自动调用AI托管的函数进行移动
    }
    else
        this -> control = _player;

    switch (_player)
    {
        case 1:
        {
            ui->label->setText("绿色下棋");
            break;
        }
        case 2:
        {
            ui->label->setText("紫色下棋");
            break;
        }
        case 3:
        {
            ui->label->setText("蓝色下棋");
            break;
        }
        case 4:
        {
            ui->label->setText("橙色下棋");
            break;
        }
        case 5:
        {
            ui->label->setText("黄色下棋");
            break;
        }
        case 6:
        {
            ui->label->setText("红色下棋");
            break;
        }
        default:
    ;

    }


}
void Widget :: set3people_mode() //三人模式的棋子，放在A，D，E三个角上
{

    if (!ifconnected) // 单机模式下的默认位置和顺序
    {
         for (int i = 1; i <= 6; i++ )
            if (i == 1 || i == 4 || i ==5)
                vis[i] = 1;
            else vis[i] = 0;
        player_order[1] = 1; player_order[2] = 5; player_order[3] = 4;
    }
    else // 联机模式下根据服务器传送的参数来调整顺序
    {
        for (int i = 1; i <= 6; i++) vis[i] = 0;
        for (int i = 1; i <= 3; i++) vis[player_order[i]] = 1;

    }
    set_mode();

}

void Widget :: set2people_mode()
{

    //  vis里面是初始化情况下表示每个角是否有玩家
    if (!ifconnected) // 单机模式
    {
        //双人模式的棋子，放在对角的A,B上
     for (int i = 1; i <= 6; i++ )
          if (i == 1 || i == 2)
              vis[i] = 1;
          else vis[i] = 0;
        player_order[1] = 1; player_order[2] = 2;
    }
    else
    {
        for (int i = 1; i <= 6; i++) vis[i] = 0;
        for (int i = 1; i <= 2; i++) vis[player_order[i]] = 1;
    }
    set_mode();
}

void Widget :: set6people_mode()  // 六人模式的棋子
{
    if (!ifconnected)
    {
         for (int i = 1; i <= 6; i++) vis[i] = 1;
        player_order[1] = 1; player_order[2] = 3; player_order[3] = 5;
        player_order[4] = 2; player_order[5] = 4; player_order[6] = 6;
    }
    else
    {
        for (int i = 1; i <= 6; i++) vis[i] = 0;
        for (int i = 1; i <= 6; i++) vis[player_order[i]] = 1;

    }
    set_mode();
}

void Widget :: set_mode()
{

     // vis[i]可以理解为i号玩家是不是在开局的时候参与了本场游戏
     if (!ifconnected) // 单机模式
     {
         for (int i = 1; i <= 6; i++)
             if (!vis[i])
             {
                 for (int j = 0; j < 10; j++ )
                     Coordinate[i][j].ifOccupied = 0;
             }
         for (int i = 1; i <= 6; i++ )
         {
             if (!vis[i]) continue;
             for (int j = 0; j < 10; j++ )
             {
                 Coordinate[i][j].chess = new Chess(this);
                 //设置棋子图片和信号
                 switch (i)
                 {
                     case 1 :
                     {

                         Coordinate[i][j].chess->setIcon(QIcon(":/image2/12.png"));
                         break;
                     }
                     case 2 :
                     {
                         Coordinate[i][j].chess->setIcon(QIcon(":/image2/22.png"));
                         break;

                     }
                     case 3 :
                     {
                         Coordinate[i][j].chess->setIcon(QIcon(":/image2/32.png"));
                         break;
                     }
                     case 4 :
                     {

                         Coordinate[i][j].chess->setIcon(QIcon(":/image2/42.png"));
                         break;
                     }
                     case 5 :
                     {
                         Coordinate[i][j].chess->setIcon(QIcon(":/image2/52.png"));
                         break;

                     }
                     case 6 :
                     {
                         Coordinate[i][j].chess->setIcon(QIcon(":/image2/62.png"));
                         break;
                     }
                 default :
                     ;              //default
                 }
                 Coordinate[i][j].ifOccupied = i;
                 //调整图片大小和位置
                 Coordinate[i][j].chess->setIconSize(QSize(chess_size,chess_size));
                 Coordinate[i][j].chess->setGeometry(Coordinate[i][j].x-chess_bias,Coordinate[i][j].y-chess_bias,chess_size,chess_size);
                 //设置信号
                 connect(Coordinate[i][j].chess,SIGNAL(chessInfo(int,int,int)),this,SLOT(chessClick(int,int,int)));
                 //设置位置
                 Coordinate[i][j].chess -> setLocation(i,j);
                 Coordinate[i][j].chess -> x = Coordinate[i][j].x;
                 Coordinate[i][j].chess -> y = Coordinate[i][j].y;
                 Coordinate[i][j].chess -> setPlayer(i);

             }
         }

         return;
     }
    //初始化开局没有棋子的地方
    for (int i = 1; i <= 6; i++)
        if (!vis[re_Coordinate_pos[i]]) // 如果这个坐标对应的地方没有玩家
        {
            for (int j = 0; j < 10; j++ )
                Coordinate[i][j].ifOccupied = 0;
            QVector<QPair<int,int>> NULLVector;
            chess_array[re_Coordinate_pos[i]].swap(NULLVector);  // 将这个角落上对应玩家的chess_array清空
        }


    //下面是初始化有棋子的地方
    for (int i = 1; i <= 6; i++ )
    {
        if (!vis[re_Coordinate_pos[i]]) continue;
        // 初始化该玩家的chess array
        QVector<QPair<int,int>> NULLVector;
        chess_array[re_Coordinate_pos[i]].swap(NULLVector);
        for (int j = 0; j < 10; j++ )
        {
            Coordinate[i][j].chess = new Chess(this);
            QPair<int,int>p1(Coordinate[i][j].x,Coordinate[i][j].y);
            chess_array[re_Coordinate_pos[i]].append(p1);
            //设置棋子图片和信号
            // 这里展现的时候要调整视角，调整视角的方式是棋子摆放的坐标不一样
            switch (re_Coordinate_pos[i]) // 根据这个坐标在画面上对应的玩家来设置不同的颜色的棋子
            {
                case 1 :
                {

                    Coordinate[i][j].chess->setIcon(QIcon(":/image2/12.png"));
                    break;
                }
                case 2 :
                {
                    Coordinate[i][j].chess->setIcon(QIcon(":/image2/22.png"));
                    break;

                }
                case 3 :
                {
                    Coordinate[i][j].chess->setIcon(QIcon(":/image2/32.png"));
                    break;
                }
                case 4 :
                {

                    Coordinate[i][j].chess->setIcon(QIcon(":/image2/42.png"));
                    break;
                }
                case 5 :
                {
                    Coordinate[i][j].chess->setIcon(QIcon(":/image2/52.png"));
                    break;

                }
                case 6 :
                {
                    Coordinate[i][j].chess->setIcon(QIcon(":/image2/62.png"));
                    break;
                }
            default :
                ;              //default
            }
            Coordinate[i][j].ifOccupied = re_Coordinate_pos[i];
            //调整图片大小和位置
            Coordinate[i][j].chess->setIconSize(QSize(chess_size,chess_size));
            Coordinate[i][j].chess->setGeometry(Coordinate[i][j].x-chess_bias,Coordinate[i][j].y-chess_bias,chess_size,chess_size);
            //设置信号
            connect(Coordinate[i][j].chess,SIGNAL(chessInfo(int,int,int)),this,SLOT(chessClick(int,int,int)));
            //设置位置
            Coordinate[i][j].chess -> setLocation(i,j);
            Coordinate[i][j].chess -> x = Coordinate[i][j].x;
            Coordinate[i][j].chess -> y = Coordinate[i][j].y;
            Coordinate[i][j].chess -> setPlayer(re_Coordinate_pos[i]);

        }
    }

}

void Widget :: endGame(QString _list)
{
    // 弹出游戏结束的窗口，并且展示排名
    GameOver* g = new GameOver(_list);
    g -> show();
    // 关闭当前窗口即可
    this -> close();
}

int Widget :: isGameOver()
//在单机模式下才会调用这个函数
// 如果全部获胜，就结束游戏
{
    for (int i = 1; i <= 6; i++ )
    if (vis[i] && ishere[i] && !judgement[i]) // 如果参加比赛，并且还在房间里面，并且还没有胜利
    {
        judgement[i] = 1;
        for (int j = 0; j < 10; j++ )
        // 要求对角上全是当前玩家的棋子才算获胜
            if (Coordinate[corner[i]][j].ifOccupied != i)
            {
                judgement[i] = 0;
                break;
            }
        if (judgement[i]) // 如果在这个时候获胜，那么加入到对应的获胜列表里面;
        success_num++;
        success_player[success_num] = i;
    }
    // 全部胜利那么就游戏结束
    for (int i = 1; i <= 6; i++)
        if (vis[i] && !judgement[i])
            return 0;
    return 1;
}

void Widget :: setWin()  //响应服务端，表示该玩家已经胜利
{
    success_num++;
    success_player[success_num] = player;
    judgement[player] = 1;
    // 弹出窗口提示玩家已经获胜
    Win* a = new Win();
    a-> show();

}

//****** update for task3

int Widget :: convert_x(int _x, int _y)
{
    int ty = (_y - center_y) / _edge;
    ty = -ty;
    int tx = (_x - center_x - ty * half_edge) / (half_edge * 2);
    return tx;
}

int Widget :: convert_y(int _y)
{
    int ty = (_y - center_y) / _edge;
    ty = -ty;
    return ty;
}
int Widget :: reconvert_x(int _x, int _y)
{
    int tx = (_y * half_edge)  +  (_x * half_edge * 2) + center_x;
    return tx;
}

int Widget :: reconvert_y(int _y)
{
    int ty = - (_y * _edge);
    ty = ty + center_y;
    return ty;
}

//**** end of update


// **** new for extra

// 注意下面转换前后的都是斜坐标


int Widget :: rotate_x (int x, int y)  // 将玩家1朝上情况的坐标转换成画面展现的时候的坐标
{
    for (int i = 1; i <= temp_bias[player]; i++) // 代表当前玩家在正下方的时候需要旋转几个60度
    {
        int tx = x + y; int ty = -x;
        x = tx; y = ty;
    }
    return x;
}
int  Widget :: rotate_y (int x, int y)
{
    for (int i = 1; i <= temp_bias[player]; i++) // 代表当前玩家在正下方的时候需要旋转几个60度
    {
        int tx = x + y; int ty = -x;
        x = tx; y = ty;
    }
    return y;
}

int Widget :: re_rotate_x (int x, int y)  // 将画面展示时候的坐标转换成玩家1朝上情况下的坐标
{
    // 转回去本应该是逆时针的旋转
    // 但是这里可以等价于顺时针旋转互补的度数
    int rotate_times = (6 - temp_bias[player]) % 6;  // 注意360度的时候就直接不转，节省时间
    for (int i = 1; i <= rotate_times; i++)
    {
        int tx = x + y; int ty = -x;
        x = tx; y = ty;
    }
    return x;
}

int Widget :: re_rotate_y (int x, int y)
{
    // 转回去本应该是逆时针的旋转
    // 但是这里可以等价于顺时针旋转互补的度数
    int rotate_times = (6 - temp_bias[player]) % 6;
    for (int i = 1; i <= rotate_times; i++)
    {
        int tx = x + y; int ty = -x;
        x = tx; y = ty;
    }
    return y;
}


/*
int Widget :: rotate_x (int x, int y)  // 将玩家1朝上情况的坐标转换成画面展现的时候的坐标
{
    return x;
}
int Widget :: rotate_y (int x, int y)
{
    return y;
}

int Widget ::re_rotate_x (int x, int y)  // 将画面展示时候的坐标转换成玩家1朝上情况下的坐标
{
    return x;
}

int Widget ::re_rotate_y (int x, int y)
{
    return y;
}
 */

// *** end of new


QString Widget :: getTrail()  // 获取当前这个窗口的玩家的行棋轨迹
{

  //  qDebug() << "enter getTrail" ;
  //  qDebug() << player ;
    // 注意这里convert坐标的时候进行了一些修改
    QString ss;
    // 为了保证getTrail这个函数的可重复调用性，我们不应该把坐标转换后的结果覆盖原来的结果
    for (int i = trail_count; i >= 1; i--)
    {
        // 转换成斜坐标后，并且需要调整为玩家1在上方时的坐标，再压缩成路径
        int tx = convert_x(x_trail[i], y_trail[i]); int ty = convert_y(y_trail[i]);
        int x = re_rotate_x(tx, ty);
        int y = re_rotate_y(tx, ty);
        ss+=QString::number(x);
        ss += " ";
        ss+=QString::number(y);  //  将对应数字转换成为字符
        if (i != 1)
            ss += " ";
    }
    return ss;
}


void Widget :: moveFail(int _player)
// 响应服务端，在每个窗口上面都显示某个玩家非法行棋的提示语
// 并且棋子不会作出任何修改
{
    // 这里弹出一个窗口，提示玩家非法行棋
    illegalMove* g = new illegalMove();
    g -> show();

}

void Widget :: setmove(int _player, QString player_trail)
// 传送进来的这个player_trail对应表示的是玩家1在上方时候的斜坐标轨迹
{
    int tempx[1000];  //存储行棋轨迹的坐标，这里存储的是展现在画面上的坐标轨迹
    int temp_num = 0;
    QString str = "";
    int len = player_trail.length();
    for (int i = 0; i <len; i++)
        if (player_trail.at(i) == ' ')
        {
            temp_num++;
            tempx[temp_num] = str.toInt();
            str = "";
        }
        else str = str + player_trail.at(i);
    // 最后一个没有空格不要忘记处理
    temp_num++;
    tempx[temp_num] = str.toInt();


    // 转换后得到实际画面上对应的坐标

    for (int i = 1; i <= temp_num; i += 2)
    {
        int x = tempx[i]; int y = tempx[i+1];
        tempx[i] = reconvert_x(rotate_x(x,y),rotate_y(x,y));
        tempx[i+1] = reconvert_y(rotate_y(x,y));

    }

    int startx = tempx[1]; int starty = tempx[2];  // 路径的起始点
    int endx = tempx[temp_num - 1]; int endy = tempx[temp_num];  // 路径的结束点


   // 最后我们更新一下内部保存的信息即可

    int sx = get_Coordinatex(g[startx][starty]); int sy = get_Coordinatey(g[startx][starty]);

    this->select = Coordinate[sx][sy].chess;

    //联机模式下这里只是更新了一下内部存储的信息，并不会展现直接移动到终点的效果
    Update(startx, starty, endx, endy);

    // 下面是将选中的图片改回未选中的效果
    switch (_player)
    {
        case 1:
       {
           this->select->setIcon(QIcon(":/image2/12.png"));
           break;
       }
       case 2:
       {
           this->select->setIcon(QIcon(":/image2/22.png"));
           break;
       }
       case 3:
       {
           this->select->setIcon(QIcon(":/image2/32.png"));
           break;
       }
       case 4:
       {
           this->select->setIcon(QIcon(":/image2/42.png"));
           break;
       }
       case 5:
       {
           this->select->setIcon(QIcon(":/image2/52.png"));
           break;
       }
       case 6:
       {
           this->select->setIcon(QIcon(":/image2/62.png"));
           break;
       }

   }

    displaypatch(player_trail, 0);

}


//判断坐标是不是在有效棋盘上面的函数
bool Widget :: onboard(int x, int y)
{
    if (x >= Max_Matrix|| x<1 || y >= Max_Matrix||y<1 ||g[x][y]==-1) return 0;
    return 1;
}

// 用于添加行棋的轨迹
void Widget :: addtrail(int _x, int _y)
{
    trail_count++;
    x_trail[trail_count] = _x;
    y_trail[trail_count] = _y;
}
//判断玩家的操作是否合法
bool Widget :: isLegalMove(Chess* prechess , Coordinate_struct* des)
{
    trail_count = 0;
    //目标移动位置被占有
    if (des -> ifOccupied != 0) return 0;
    //移动的目标位置不在棋盘上
    if (!onboard(des -> x, des -> y)) return 0;
    //横向平移
    if (abs(prechess -> x - des -> x) == half_edge * 2 && abs(prechess -> y - des -> y) == 0 )
    {
        addtrail(des -> x, des -> y);
        addtrail(prechess -> x, prechess -> y);
        return 1;
    }
    //斜向平移

    if (abs(prechess -> x - des -> x) == half_edge  && abs(prechess -> y - des -> y) == _edge)
    {
        addtrail(des -> x, des -> y);
        addtrail(prechess -> x, prechess -> y);
        return 1;
    }
    //下面考虑是连续跳跃的情况
    count++; // 定义为private，用来计数打标签

    return ifreach(prechess -> x, prechess -> y,  des);
}


//判断是否能够经过从某个位置经过连续的若干步跳跃到达目标位置
bool Widget :: ifreach(int x , int y , Coordinate_struct* des)
{
    // 如果已经递归到达目标
    if (x == des -> x && y == des -> y)
    {
        addtrail(x, y);
          return 1;
    }
    // 如果已经访问过，那么就不重复递归
    if (iftouch[x][y] == count) return 0;
    //打上已经访问过的标签
    iftouch[x][y] = count;
    //朝6个可能的方向跳跃
    for (int i = 0; i < 6; i++)
    {
        int tx1 =  x + dx[i]; int ty1 =  y + dy[i];
        int tx2 =  x + dx[i] * 2; int ty2 = y + dy[i] * 2;
        //如果这两个位置有任何一个不在有效的棋盘范围内，显然是不合法的
        if (!onboard(tx1,ty1) || !onboard(tx2,ty2)) continue;
        //下面我们判断作为中间点的位置上是否有棋子，若没有那么也不能跳跃。
        int mx1 = get_Coordinatex(g[tx1][ty1]); int my1 = get_Coordinatey(g[tx1][ty1]);
        if (Coordinate[mx1][my1].ifOccupied == 0) continue;
        //如果跳跃的目标地点上面有棋子，那么也不能跳跃。
        int mx2 = get_Coordinatex(g[tx2][ty2]); int my2 = get_Coordinatey(g[tx2][ty2]);
        if (Coordinate[mx2][my2].ifOccupied > 0) continue;
        //下面我们尝试移动
        if (ifreach(tx2,ty2,des))
        {
            addtrail(x, y);
            return 1;
        }
    }
    //否则没有递归到成功移动的情况
    return 0;
}

//更新棋盘上棋子的移动情况, 从1移动到2

void Widget :: Update (int x1, int y1, int x2, int y2)
// 将x1, x2位置的棋子移动到 x2, y2
{
    //根据传输值获取coordinate的坐标
    int mx1 = get_Coordinatex(g[x1][y1]); int my1 = get_Coordinatey(g[x1][y1]);
    int mx2 = get_Coordinatex(g[x2][y2]); int my2 = get_Coordinatey(g[x2][y2]);

    // update chess_array
    int _player = Coordinate[mx1][my1].ifOccupied;
    int size = chess_array[_player].size();
    for (int i = 0; i < size; i++)
        if (chess_array[_player][i].first == x1 && chess_array[_player][i].second == y1)
        {
            chess_array[_player][i].first = x2;
            chess_array[_player][i].second = y2;
        }

    //移动

    Coordinate[mx2][my2].ifOccupied = Coordinate[mx1][my1].ifOccupied; Coordinate[mx1][my1].ifOccupied = 0;
    //chess相关参数的重置
    Coordinate[mx2][my2].chess = Coordinate[mx1][my1].chess; Coordinate[mx1][my1].chess = nullptr;
    Coordinate[mx2][my2].chess -> setLocation(mx2,my2);
    Coordinate[mx2][my2].chess->x = Coordinate[mx2][my2].x;
    Coordinate[mx2][my2].chess->y = Coordinate[mx2][my2].y;
    if (!ifconnected) // 单机模式下执行这一句话
       Coordinate[mx2][my2].chess -> move(x2 - chess_bias , y2 - chess_bias);

}


/* 下面是处理点击的响应函数部分*/
void Widget::chessClick(int _player, int tx, int ty)
{

  //  qDebug() << "enter chessClick for " << player;
    // 注意这里player和_player的区别，前者表示当前界面操作的玩家，后者表示被点击选中的棋子所属的玩家
    bool canmove;
 //   qDebug()<<"this";
    if (ifconnected) // 联机模式下
    {
        // 限制为拥有控制权的玩家才能对自己当前窗口进行点击棋子的操作，否则不进行响应
        // 并且操作的得是自己的棋子，否则也不给予响应
        if (control == player && player == _player)
            canmove = 1;
        else canmove = 0;
    }
    else // 单机模式下
    {
        // 只需要判断是对应轮到的玩家的棋子即可
        if (control == _player) canmove = 1; else canmove = 0;
    }

    if (canmove)
    {
      //  qDebug() << "this!";
        switch (_player) //根据玩家分类设置不同的展示效果图
        {
            case 1:
            {
             //    qDebug()<<"this!!";
                if (clickTime == 0) //第一次点击选中棋子
                {
                    clickTime = 1;
                    this->select = this->Coordinate[tx][ty].chess;
                    this->select->setIcon(QIcon(":/image2/1.png"));
                }
                else //第二次点击自己的任何棋子就是取消选中
                {

                    clickTime = 0;
                    this->select->setIcon(QIcon(":/image2/12.png"));
                    this->select = nullptr; //取消选中
                }
                break;
            }
            case 2:
            {
                if (clickTime == 0) //第一次点击选中棋子
                {
                    clickTime = 1;
                    this->select = this->Coordinate[tx][ty].chess;
                    this->select->setIcon(QIcon(":/image2/2.png"));
                }
                else //第二次点击取消选中
                {

                    clickTime = 0;
                    this->select->setIcon(QIcon(":/image2/22.png"));
                    this->select = nullptr; //取消选中
                }
                break;
            }
            case 3:
            {
                if (clickTime == 0) //第一次点击选中棋子
                {
                    clickTime = 1;
                    this->select = this->Coordinate[tx][ty].chess;
                    this->select->setIcon(QIcon(":/image2/3.png"));
                }
                else //第二次点击取消选中
                {

                    clickTime = 0;
                    this->select->setIcon(QIcon(":/image2/32.png"));
                    this->select = nullptr; //取消选中
                }
                break;
            }
            case 4:
            {
                if (clickTime == 0) //第一次点击选中棋子
                {
                    clickTime = 1;
                    this->select = this->Coordinate[tx][ty].chess;
                    this->select->setIcon(QIcon(":/image2/4.png"));
                }
                else //第二次点击取消选中
                {

                    clickTime = 0;
                    this->select->setIcon(QIcon(":/image2/42.png"));
                    this->select = nullptr; //取消选中
                }
                break;
            }
            case 5:
            {
                if (clickTime == 0) //第一次点击选中棋子
                {
                    clickTime = 1;
                    this->select = this->Coordinate[tx][ty].chess;
                    this->select->setIcon(QIcon(":/image2/5.png"));
                }
                else //第二次点击取消选中
                {

                    clickTime = 0;
                    this->select->setIcon(QIcon(":/image2/52.png"));
                    this->select = nullptr; //取消选中
                }
                break;
            }
            case 6:
            {
                if (clickTime == 0) //第一次点击选中棋子
                {
                    clickTime = 1;
                    this->select = this->Coordinate[tx][ty].chess;
                    this->select->setIcon(QIcon(":/image2/6.png"));
                }
                else //第二次点击取消选中
                {

                    clickTime = 0;
                    this->select->setIcon(QIcon(":/image2/62.png"));
                    this->select = nullptr; //取消选中
                }
                break;
            }
            default :
            ;


        }
    }
}

//处理点击事件，完成所有操作
void Widget::mousePressEvent(QMouseEvent *event)
{
    // 如果有对应选中的地方和选择的棋子，那么就尝试行棋
    if (select != nullptr && event -> button()  == Qt :: LeftButton)
    {
        // 循环所有棋子的位置
        for (QVector<Coordinate_struct*>::iterator iter = places.begin();iter != places.end();iter++)
        {
            Coordinate_struct* ptr = *iter;
            //如果和当前棋子的距离足够近，那么就判断是操作当前这个位置
            if(get_dis(event->x() , event->y() , ptr->x , ptr->y) < Dis_lim  )
            {
                // 假如是联机模式
                if (ifconnected)
                {
                    //qDebug()<<"this3";
                    Trail_off = 0;
                    bool canmove =  isLegalMove(select, ptr); // 我们需要这个步骤来获取trail
                    emit signal_move(); // 发送信号给客户端请求移动
                    //由于自己将不再接收到move up的信号，所以自行根据判断是不是合法的结果行棋
                    // 这里直接转换成为调用本身这个窗口的setmove
                    if (canmove)
                    {
                       // qDebug()<<"this4";
                        QString my_trail = getTrail();
                        this -> setmove(player, my_trail);
                    }
                    this->clickTime=0;
                //    this->select = nullptr;
                    break;
                }

                // 下面是单机模式下需要做的事情
                if(isLegalMove(select , ptr))
                {
                    //更新数据
                    Update(this->select->x,this->select->y,ptr->x,ptr->y);
                       //根据当前是哪一个玩家进行分类，展现更新效果
                       switch (control)
                       {
                           case 1:
                          {
                              this->select->setIcon(QIcon(":/image2/12.png"));
                              break;
                          }
                          case 2:
                          {
                              this->select->setIcon(QIcon(":/image2/22.png"));
                              break;
                          }
                          case 3:
                          {
                              this->select->setIcon(QIcon(":/image2/32.png"));
                              break;
                          }
                          case 4:
                          {
                              this->select->setIcon(QIcon(":/image2/42.png"));
                              break;
                          }
                          case 5:
                          {
                              this->select->setIcon(QIcon(":/image2/52.png"));
                              break;
                          }
                          case 6:
                          {
                              this->select->setIcon(QIcon(":/image2/62.png"));
                              break;
                          }
                          default:
                      ;


                      }

                    // 判断游戏是否结束
                    if (isGameOver())
                    {
                        // 弹出提示窗口，提示游戏已经结束
                        GameOver *win = new GameOver();
                        win->show();
                        this->close(); // 关闭当前游戏界面
                        break;
                    }
                      // 将控制权交给次序当中的下一个玩家，玩家需要在对应的房间里面，并且还没有获胜
                      if (present_order == type)
                          present_order = 1;
                      else present_order++;
                      while (!ishere[player_order[present_order]] || !judgement[player_order[present_order]])
                      {
                          if (present_order == type) present_order = 1; else present_order++;
                      }
                      control = player_order[present_order];
                      //根据下一个玩家是谁进行分类，并且给出相应的提示语
                    switch (control)
                    {
                        case 1:
                        {
                            ui->label->setText("绿色下棋");
                            break;
                        }
                        case 2:
                        {
                            ui->label->setText("紫色下棋");
                            break;
                        }
                        case 3:
                        {
                            ui->label->setText("蓝色下棋");
                            break;
                        }
                        case 4:
                        {
                            ui->label->setText("橙色下棋");
                            break;
                        }
                        case 5:
                        {
                            ui->label->setText("黄色下棋");
                            break;
                        }
                        case 6:
                        {
                            ui->label->setText("红色下棋");
                            break;
                        }
                        default:
                    ;

                    }


                }
                //else this->select->setIcon(QIcon(""));

                this->clickTime=0;
                this->select = nullptr;
                break;
            }
        }
    }

}

/* end */


/* 下面是计算棋盘的坐标，以及画出没有棋子状态的棋盘，是一些基本上不会修改的东西，当成接口调用即可*/

//下面是两个根据传输值来获取坐标的函数
int Widget :: get_Coordinatex(int value)
{
     if (value >= 'A' << 16)
         return ((value >> 16) - 'A' + 1);
     else return 0;
}

int Widget :: get_Coordinatey(int value)
{
     if (value >= 'A' << 16)
         return (value - ((value >> 16) << 16));
     else return value;
}

void Widget :: mapping()  //构建坐标矩阵和传输值的关系
{
    // 初始化开始地方的传输值都为-1，禁止走到棋盘的外面
    for (int i = 0; i < Max_Matrix; i++)
        for (int j = 0; j < Max_Matrix; j++)
            g[i][j] = -1;
    for (int i = 1; i <= 6; i++)
        for (int j = 0; j < 10; j++)
            g[Coordinate[i][j].x][Coordinate[i][j].y] = Coordinate[i][j].Value;
    for (int i = 0 ; i < 61 ; i++)
        g[Coordinate[0][i].x][Coordinate[0][i].y] = Coordinate[0][i].Value;

    //在获取g的值之后就可以进行哈希的预处理
    AI_hash_preprocessing();
}


void Widget:: assign_xy(int area, int inv, int topx, int topy)  // 用来分配六个区域的坐标
// area 代表A，B，C，D中的某个区域，inv代表是正三角还是倒三角
{

    int pre_edge = inv * _edge;
    Coordinate[area][0].x = topx; Coordinate[area][0].y = topy;
    Coordinate[area][1].x = topx + half_edge; Coordinate[area][1].y = topy + pre_edge;
    Coordinate[area][2].x = topx - half_edge; Coordinate[area][2].y = topy + pre_edge;
    Coordinate[area][3].x = Coordinate[area][2].x - half_edge;
    Coordinate[area][3].y = Coordinate[area][2].y + pre_edge;
    for (int i = 4; i <= 5; i++)
    {
        Coordinate[area][i].x = Coordinate[area][i-1].x + half_edge * 2;
        Coordinate[area][i].y = Coordinate[area][i-1].y;
    }
    Coordinate[area][6].x = Coordinate[area][5].x + half_edge;
    Coordinate[area][6].y = Coordinate[area][5].y + pre_edge;
    for (int i = 7; i <= 9; i++)
    {
        Coordinate[area][i].x = Coordinate[area][i-1].x - half_edge * 2;
        Coordinate[area][i].y = Coordinate[area][i-1].y;
    }

}

void Widget:: setCoordinate()
{
    // A(1): 上， B(2)： 下， C(3)：右上， D(4)：左下， E(5)：右下， F(6)：左上
    //每个节点的内部按顺时针进行标号

    //首先得到对应的用来传输和转换的int的数据
    for (int j = 0; j < 6; j++ )
      for (int i = 0; i < 10; i++)
          Coordinate[j+1][i].Value = (('A'+ j) << 16) + i;
    //中间空白的部分共有 (5 + 6 + 7 + 8) * 2 + 9 = 61 个位置，用Coodinate[0]表示
    for (int j = 0; j < 61; j++)
        Coordinate[0][j].Value = j;

    //下面我们为每个部分来设置绘图时候需要的坐标
    //首先是棋子摆放的部分
    assign_xy(1, 1, A_top_x, A_top_y);
    assign_xy(2, -1, A_top_x, A_top_y + _edge * 16);
    assign_xy(3, -1, A_top_x + half_edge * 9, A_top_y + _edge * 7);
    assign_xy(4, 1, A_top_x - half_edge * 9, A_top_y + _edge * 9);
    assign_xy(5, 1, A_top_x + half_edge * 9, A_top_y + _edge * 9);
    assign_xy(6, -1, A_top_x - half_edge * 9, A_top_y + _edge * 7);
    //for(int i=0;i<=9;i++)
    //    qDebug()<<"("<<Coordinate[2][i].x<<","<<Coordinate[2][i].y<<")";
    // 然后是中间空白部分的坐标，我们就用行列遍历的顺序来标号
    // 5 + 6 + 7 + 8 + 9 + 8 + 7 + 6 + 5
    Coordinate[0][0].x = Coordinate[1][9].x - half_edge;
    Coordinate[0][0].y = Coordinate[1][9].y +_edge; //改
    // temp数组计算出每行开始的节点
    int temp[10]; temp[0] = 0;
    for (int i = 1; i < 5; i++) temp[i] = temp[i-1] + i + 4;
    for (int i = 5; i < 9; i++) temp[i] = temp[i-1] + 14 - i;
    int p = 1;
    for (int i = 1; i < 61; i++)
        if (i == temp[p])
        {
            int f;
            if (p < 5) f = -half_edge; else f = half_edge;
            Coordinate[0][i].x = Coordinate[0][temp[p-1]].x + f;
            Coordinate[0][i].y = Coordinate[0][temp[p-1]].y + _edge;
            p++;
        }
        else
        {
            Coordinate[0][i].x = Coordinate[0][i-1].x + half_edge * 2;
            Coordinate[0][i].y = Coordinate[0][i-1].y;
        }

    //下面设置空白位置的初始状态
    for (int i = 0; i < 61; i++) Coordinate[0][i].ifOccupied = 0;

    //构建矩阵坐标和传输值的关系
    mapping();

    // 注意这里存放的仅仅是地址，好处是对应坐标处的信息更改，places里面并不需要修改
    for (int i = 1 ; i <= 6 ; i++)
        for (int j = 0 ; j < 10 ; j++)
            places.push_back(&Coordinate[i][j]);
    for (int i = 0 ; i < 61 ; i++) places.push_back(&Coordinate[0][i]);

}



//辅助函数，画底色
void  Widget :: draw_BottomCol(int area, int x, int y, int inv)
{
    QPen pen(Qt::SolidLine);
    QPainter p(this);

    p.setPen(pen);
    // 确定所画底色的三角形区域的三个点
     const QPointF pointSet[3] =
    {
       QPointF(x,y),
       QPointF(x - 4 * half_edge , y + inv * 4 * _edge),
       QPointF(x + 4 * half_edge , y + inv * 4 * _edge)
     };

    //下面根据不同的玩家画上不同的底色
    switch (re_Coordinate_pos[area])
    {
        case 1 :
        {
            QBrush brush(Qt::green ); //设置某种需要的颜色
            p.setBrush(brush);
            p.drawPolygon(pointSet,3);
            break;
        }
        case 2 :
        {
            QBrush brush(QColor(255,1,255) ); //设置某种需要的颜色      紫色
            p.setBrush(brush);
            p.drawPolygon(pointSet,3);
            break;
        }
        case 3 :
        {
            QBrush brush(QColor(0,0,255,200) ); //设置某种需要的颜色
            p.setBrush(brush);
            p.drawPolygon(pointSet,3);
            break;
        }
        case 4 :
        {
            QBrush brush(QColor(255,110,25) ); //设置某种需要的颜色     橙色
            p.setBrush(brush);
            p.drawPolygon(pointSet,3);
            break;

        }
        case 5 :
        {
            QBrush brush(Qt ::yellow ); //设置某种需要的颜色
            p.setBrush(brush);
            p.drawPolygon(pointSet,3);
            break;
        }
        case 6 :
        {
            QBrush brush(Qt ::red); //设置某种需要的颜色    红色
            p.setBrush(brush);
            p.drawPolygon(pointSet,3);
            break;

        }
    default:
        ;
    }
}

//画棋盘的函数
void Widget :: paintEvent(QPaintEvent *)
{
    //整个图形可以看作是两个大的等边三角形的拼接
    //在这里最重要的是先确定好三个顶角的位置

    //Step 1: 首先画出每个角落的不同的玩家的不同的底色
    draw_BottomCol(1, A_top_x ,A_top_y , 1);
    draw_BottomCol(2, A_top_x ,A_top_y + 16 * _edge , -1);
    draw_BottomCol(3, A_top_x + 8 * half_edge ,A_top_y + 8 * _edge , -1);
    draw_BottomCol(4, A_top_x - 8 * half_edge ,A_top_y + 8 * _edge , 1);
    draw_BottomCol(5, A_top_x + 8 * half_edge ,A_top_y + 8 * _edge , 1);
    draw_BottomCol(6, A_top_x - 8 * half_edge ,A_top_y + 8 * _edge , -1);

    //Step 2: 分别画两个三角形里面的线，与此同时可以画出轮廓
    //思路 ：分别从每个顶点出发画出横线
    QPen pen(Qt::SolidLine);
    QPainter p(this);
    p.setPen(pen);

    const int B_top_x = A_top_x; const int B_top_y = A_top_y + 16 * _edge;
    const int C_top_x = A_top_x + 12 * half_edge; const int C_top_y = A_top_y + 4 * _edge;
    const int D_top_x = A_top_x - 12 * half_edge; const int D_top_y = A_top_y + 12 * _edge;
    const int E_top_x = A_top_x + 12 * half_edge; const int E_top_y = A_top_y + 12 * _edge;
    const int F_top_x = A_top_x - 12 * half_edge; const int F_top_y = A_top_y + 4 * _edge;

        //正三角形中的线
    for (int i = 1; i <= 12; i ++)
    {
        p.drawLine(A_top_x - i * half_edge,A_top_y + i * _edge, A_top_x + i *half_edge, A_top_y + i * _edge); //表示从左边的点出发到右边画一条线

        p.drawLine(D_top_x + i * 2 * half_edge, D_top_y, D_top_x + i * half_edge, D_top_y - i * _edge);

        p.drawLine(E_top_x - i * 2 * half_edge, E_top_y, E_top_x - i * half_edge, E_top_y - i * _edge);
    }
        //倒三角形中的线
    for (int i = 1; i <= 12; i ++)
    {
        p.drawLine(B_top_x - i * half_edge,B_top_y - i * _edge, B_top_x + i *half_edge, B_top_y - i * _edge); //表示从左边的点出发到右边画一条线

        p.drawLine(F_top_x + i * 2 * half_edge, F_top_y, F_top_x + i * half_edge, F_top_y + i * _edge);

        p.drawLine(C_top_x - i * 2 * half_edge, C_top_y, C_top_x - i * half_edge, C_top_y + i * _edge);
    }
    //Step 3: 画出棋盘上面的圆圈
    QBrush brush(QColor(255,255,255));
    //brush.setColor(QColor(255,225,255)); //棋盘上的圆圈用白色填涂
    p.setBrush(brush);

    //正三角画圆
    for(int i = 0; i <= 12; i++)
        for(int j = 0; j <= i; j++)
            p.drawEllipse(QPoint(A_top_x - i * half_edge + j * 2 * half_edge ,A_top_y + i * _edge),chess_bias,chess_bias); //在矩形里面画内切圆

    //倒三角里面画圆
    for(int i = 0; i <= 12; i++)
        for(int j = 0; j <= i; j++)
            p.drawEllipse(QPoint(B_top_x - i * half_edge + j * 2 * half_edge ,B_top_y - i * _edge),chess_bias,chess_bias);


}

/* end */


/* 下面是一些与客户端连接的操作 */


void Widget ::  on_pushButton_clicked() // 玩家按下退出，自行决定退出游戏
{
    emit exit(); // 发送离开的信号
 //   qDebug()<<1;
    this -> close(); //关闭当前窗口即可，显然维护当前窗口的信号是没有意义的
}

void Widget::Countdown()
{
    //qDebug()<<"this00";
    this->ui->label_2->setText(QString::number(Countnum));
    Countnum--;
    if(Countnum<=0)
    {
       // qDebug()<<"this000";
        timer->stop();
    }
    //delete timer;
}

void Widget :: setLeave(QString _name)
{
   // qDebug()<<3;
    int _player;
    // 将对应的ishere设置为0
    for (int i = 0; i < 10; i++)
         if (id_name[i] == _name) // 找到对应离开的玩家
         {
              ishere[i] = 0;
             _player = i;
             break;
         }

    // 弹出提示标语，表示对应的玩家离开了
    // 这里可能需要调整一下效果
    switch (_player)
    {
        case 1:
        {
            ui->label_3->setText("绿方玩家离开了");
            break;
        }
        case 2:
        {
            ui->label_3->setText("紫方玩家离开了");
            break;
        }
        case 3:
        {
            ui->label_3->setText("蓝方玩家离开了");
            break;
        }
        case 4:
        {
            ui->label_3->setText("橙方玩家离开了");
            break;
        }
        case 5:
        {
            ui->label_3->setText("黄方玩家离开了");
            break;
        }
        case 6:
        {
            ui->label_3->setText("红方玩家离开了");
            break;
        }
        default:
    ;

    }

    // 离开房间需要清空相应玩家的棋子
    for (int i = 1; i <= 6; i++)
        for (int j = 0; j < 10; j++)
           if (Coordinate[i][j].ifOccupied == _player)
           {
               delete Coordinate[i][j].chess;
               Coordinate[i][j].chess = nullptr;
               Coordinate[i][j].ifOccupied = 0;
           }
    for (int i = 0 ; i < 61 ; i++)
        if (Coordinate[0][i].ifOccupied == _player)
        {
            delete Coordinate[0][i].chess;
            Coordinate[0][i].chess = nullptr;
            Coordinate[0][i].ifOccupied = 0;
        }

    QVector<QPair<int,int>> NULLVector;
    chess_array[_player].swap(NULLVector);  // 清空对应玩家的chess_array


}

void Widget :: setOutofTime(int this_player)
{
    // 下面在每个棋盘上面提示一下该名玩家被超时判负
    ishere[this_player] = 0;
    switch (this_player)
    {
        case 1:
        {
            ui->label_3->setText("绿色超时判负");
            break;
        }
        case 2:
        {
            ui->label_3->setText("紫色超时判负");
            break;
        }
        case 3:
        {
            ui->label_3->setText("蓝色超时判负");
            break;
        }
        case 4:
        {
            ui->label_3->setText("橙色超时判负");
            break;
        }
        case 5:
        {
            ui->label_3->setText("黄色超时判负");
            break;
        }
        case 6:
        {
            ui->label_3->setText("红色超时判负");
            break;
        }
        default:
    ;

    }
     // 搜索6个角看是否有该玩家的棋子
    for (int i = 1; i <= 6; i++)
        for (int j = 0; j < 10; j++)
           if (Coordinate[i][j].ifOccupied == this_player)
           {
               Coordinate[i][j].chess = nullptr;
               Coordinate[i][j].ifOccupied = 0;
           }

    // 搜索中间部分是否有该玩家的棋子
    for (int i = 0 ; i < 61 ; i++)
        if (Coordinate[0][i].ifOccupied == this_player)
        {
            Coordinate[0][i].chess = nullptr;
            Coordinate[0][i].ifOccupied = 0;
        }

    QVector<QPair<int,int>> NULLVector;
    chess_array[this_player].swap(NULLVector);  // 清空对应玩家的chess_array

}






Widget::~Widget()
{
    delete ui;
}


// ******** new for task3

bool  Widget :: judgeWin(int _player)  // 判断这个玩家是否是一个新增的获胜者
{
     // 判断当前玩家的棋子是否全部走到了对角
    int size = chess_array[_player].size();
    for (int i = 0; i < size; i++)
    {

        int x = chess_array[_player][i].first;
        int y = chess_array[_player][i].second;
        if (get_Coordinatex(g[x][y]) != Coordinate_pos[corner[_player]]) return 0;
    }
    return 1;
}

int Widget :: AI_dis(int x, int y, int tx, int ty)
{
    //  此处的距离函数，我们假设只采用平移的方法，需要移动多少步才能到达，距离就是多少
    /*
    int x1 = convert_x(x,y); int y1 = convert_y(y);
    int tx1 = convert_x(tx,ty); int ty1 = convert_y(ty);
    return (abs(x1 -tx1) + abs(y1 - ty1));
     */
    int stepy = (abs(ty - y) / _edge);
    int stepx = (abs(tx - x) - stepy * half_edge) / (half_edge * 2);
    return (stepx + stepy);

}

int Widget :: AI_corner_dis(int _player, int x, int y)  // 找到当前点到对角的距离
{
    // 首先找到对角的玩家在画面上显示的坐标
   int anti = Coordinate_pos[corner[_player]];
    // 然后分层看是否有空余的位置

    //int min_dis = (int_limits);
   int min_dis= int_limits;
    for (int i = 0; i <= 3; i++)
    {
        int startj;
        if (i == 0) startj = 0; else startj = level_sep[i-1] - 1;
        bool flag = 1;
        for (int j = startj; j <= level_sep[i]; j++)
        {
            int cy = level[anti][j];
            int tx = Coordinate[anti][cy].x; int ty = Coordinate[anti][cy].y;
            if (AI_mark[tx][ty] == coordinate_mark && AI_occupied[tx][ty] == _player) // 如果对方的这个位置已经我方的棋子占据
            {
                // 排除就是当前棋子的情况
                if (x == tx && y == ty)
                {
                    flag = 0;  min_dis= 0;
                    break;
                }
                continue;
            }
            else
            {
                flag = 0;
                int cur_dis = AI_dis(x, y, tx, ty);
                if (cur_dis < min_dis) min_dis = cur_dis;   // 否则求到这个空白点的距离
            }
        }
        if (!flag) break; // 如果这一层有空位置，那么就返回到这一层的空着的位置的距离的最小值。
    }

    /*
   // 否则判断移动后的位置是不是比原来的位置要更远，是的话就返回true
   int corner_top;
   // 首先根据对角玩家所在的横坐标进行分类，然后找到对应顶点的纵坐标
   if (anti == 1 || anti == 2)
       corner_top = 0;
   else if (anti == 4 || anti == 6)
       corner_top = 9;
   else corner_top = 6;
    return AI_dis(x, y, Coordinate[anti][corner_top].x, Coordinate[anti][corner_top].y);
     */

    return min_dis;

}

bool Widget :: backwards(int _player, int x, int y, int tx, int ty)
// 即判断x，y是不是相比tx，ty离对角顶点更近
{
    /*
   int anti = Coordinate_pos[corner[_player]];
   int corner_top;
   if (anti == 1 || anti == 2)
       corner_top = 0;
   else if (anti == 4 || anti == 6)
       corner_top = 9;
   else corner_top = 6;
   if (AI_dis(x, y, Coordinate[anti][corner_top].x, Coordinate[anti][corner_top].y) < AI_dis(tx, ty, Coordinate[anti][corner_top].x, Coordinate[anti][corner_top].y))
       return 1;
   else
       return 0;

   */

    if (AI_corner_dis(_player, x, y) <= AI_corner_dis(_player, tx, ty)) return 1; else return 0;
}

bool Widget :: checkin(int _player, int win_state)  // 代表已经记录在获胜的状态中
{
    if  (((1 << (_player - 1)) & win_state) > 0) return 1; else return 0;
}


int Widget :: next_player(int now_player,int win_state) // 获取下一个玩家
{
    int i;
    for (i = 1; i <= type; i++)
        if (player_order[i] == now_player) break;
    if (i == type) i = 1; else i = i + 1;

    // 单机模式这里轮换有错误，对应的&&需要改成||

    while (!ishere[player_order[i]] || checkin(player_order[i], win_state)) // 不在房间或者是已经获胜则需要轮空
    {
        if (i == type) i = 1; else i = i + 1;
    }

    return player_order[i];
}

int Widget :: AI_assess(int _player, int search_step, int search_limit) //评估当前局面的有利程度，给出一个函数
{
    // 这里需要结合决策方和对抗方的综合情况来进行评判
    // 每一方都用距离函数来衡量，即所有棋子距离对角点的斜坐标上的距离之和，显然这个值越小越好
    int value = 0;
    for (int i = 1; i <= 6; i++)
        if (ishere[i] && !judgement[i]) // 如果当前玩家还在房间里面，并且AI决策之前还没有胜利，那么就参与到这个局势的评分里面
        {

            /*
            int corner_top;
            if (Coordinate_pos[corner[i]] == 1 || Coordinate_pos[corner[i]] == 2)
                corner_top = 0;
            else if (Coordinate_pos[corner[i]] == 4 || Coordinate_pos[corner[i]] == 6)
                corner_top = 9;
            else corner_top = 6;
            int x = Coordinate[Coordinate_pos[corner[i]]][corner_top].x;
            int y = Coordinate[Coordinate_pos[corner[i]]][corner_top].y;
             */

            int size = chess_array[i].size();
            int sum = 0;
            for (int j = 0; j < size; j++)
            {
                sum+= AI_corner_dis(i, chess_array[i][j].first, chess_array[i][j].second);
            }
            // 我们可以通过更改这个评估函数来调整决策的偏重点
            // 比如我会认为让自己获胜会比让别人不利更重要
            // 于是我会调整我自己的sum的值更大一些的权重
            if (i == _player)
                value = value - sum * 3;
            else value = value + sum;
        }

    // 如果在到达步数限制之前就进入到评估函数，代表决策方在较短的步数内就获得了胜利，显然是一个比较好的决策，我们需要加上额外的权重
    // 这里相关的参数可以调整
    if (search_step <= search_limit)
       value +=  extra_score * (search_limit - search_step + 1);

    return value;

}

//如果冲突比较多的话我们可以考虑双哈希
//但或许有些冲突可以减少相似的局面的搜索，从而搜索更多多样化的可能性
//在初始化棋盘的时候就要调用这个函数，初始化字符串哈希一些必须的值
void Widget :: AI_hash_preprocessing()
{
    // 首先我们把棋盘上面的每个位置给定一个顺序，看作是一个高位到低位的排序
    // 这样每次更新某个位置，我们都可以在O(1)的时间内计算出棋盘新的字符串哈希的值。
    int temp = -1;

    for (int i = 1;  i < Max_Matrix; i++)
        for (int j = 1; j < Max_Matrix; j++)
            if (g[i][j] != -1) // 如果是棋盘上的有效区域，那么就进行编号
            {
                temp++; AI_pos[i][j] = temp;
            }
}

void  Widget :: leap_search(QVector<QPair<int,int>>& ans, int x, int y)
{
    // 原则上是可以在跳跃的任何中间一步停下来，但是我们这里选择能跳远一些就远一些
    iftouch[x][y] = count; // 标记为已经访问
    bool can_jump = 0;
    for (int i = 0; i < 6; i++) // 朝六个可能的方向跳跃
    {
        int tx1 =  x + dx[i]; int ty1 =  y + dy[i];
        int tx2 =  x + dx[i] * 2; int ty2 = y + dy[i] * 2;
        //如果这两个位置有任何一个不在有效的棋盘范围内，显然是不合法的
        if (!onboard(tx1,ty1) || !onboard(tx2,ty2)) continue;
        //如果作为中间点的位置上面没有棋子，那么是不能跳跃的
        if (AI_mark[tx1][ty1] != coordinate_mark) continue;
        //如果跳跃的目的地上面有棋子，那么也不能跳跃。
        if (AI_mark[tx2][ty2] == coordinate_mark) continue;
        //否则说明是一个可以跳跃的选择
        if (iftouch[tx2][ty2] != count) // 未访问
        {
            can_jump = 1;
            leap_search(ans,tx2,ty2);
        }
    }
    if (!can_jump) // 已经是尽可能跳的最远
    {
        QPair<int, int> p2(x,y);
        ans.append(p2);
    }

}


QVector<QPair<int,int>>  Widget :: search(int x,int y)
{
    QVector<QPair<int,int>> ans;
    // 考虑平移的部分
    // 朝六个可能的方向平移
    count++; // 用来计数打标签，避免重复地递归
    for (int i = 0; i < 6; i++)
    {
        int tx = x + dx[i]; int ty = y + dy[i];
        if (!onboard(tx,ty)) continue; // 不在棋盘的有效范围内，越界
        if (AI_mark[tx][ty] == coordinate_mark) // 如果上面有棋子，那么也不能移动
            continue;
        QPair<int,int> p1(tx,ty);
        ans.append(p1);
    }
    // 考虑递归跳跃

    leap_search(ans,x,y);

    return ans;

}

void Widget :: AI_update(int _player, int x,int y, int tx, int ty)
{
    // 修改对应的chess_array
    int size = chess_array[_player].size();
    for (int i = 0; i < size; i++)
        if (chess_array[_player][i].first == x && chess_array[_player][i].second == y)
        {
            chess_array[_player][i].first = tx;
            chess_array[_player][i].second = ty;
        }
    // 修改对应的坐标标记
    AI_mark[x][y] = -1;
    AI_mark[tx][ty] = coordinate_mark;
    AI_occupied[tx][ty] = _player;
    hash_value = (hash_value - (hash_value_player[_player] * base_player[_player]) % mod_p + mod_p) % (int)mod_p;
    // 修改对应玩家的hash值
    hash_value_player[_player] = (hash_value_player[_player] - base_pow[AI_pos[x][y]] + mod_p) % mod_p;
    hash_value_player[_player] = (hash_value_player[_player] + base_pow[AI_pos[tx][ty]]) % mod_p;
    // 修改对应的总的hash值
    hash_value = (hash_value + hash_value_player[_player] * base_player[_player]) % mod_p;
}

int Widget :: hash_update(int _player, int x,int y, int tx, int ty)
{
    long long temp_hash = hash_value;
    long long temp_hash_player = hash_value_player[_player];
    temp_hash = (temp_hash - (temp_hash_player * base_player[_player]) % mod_p + mod_p) % (int)mod_p;
    // 修改对应玩家的hash值
    temp_hash_player = (temp_hash_player - base_pow[AI_pos[x][y]] + mod_p) % mod_p;
    temp_hash_player = (temp_hash_player + base_pow[AI_pos[tx][ty]]) % mod_p;
    // 修改对应的总的hash值
    temp_hash = (temp_hash + temp_hash_player * base_player[_player]) % mod_p;
    return temp_hash;
}

void Widget :: AI_dfs(QVector<int>& evaluate_p, int _player, int now_player, int dep,int limit, int win_state)
{

    if  (if_repeated[hash_value] == hash_mark)  return;
    // 如果是之前已经存在的格局或者是已经决策过的格局，那么就不再进行搜索
    if_repeated[hash_value] = hash_mark;
    if (judgeWin(_player) || dep > limit)
        // 如果用来决策的这一方已经获得胜利，那么就没有继续搜索下去的必要了
        // 如果超过了步数的限制那么也要直接结束
    {
        evaluate_p.append(AI_assess(_player, dep, limit));  // 评估当前局面，并加入分数
        return;
    }
    //同时我们需要判断当前玩家是不是一个新增的获胜者，如果是的话，那么此轮需要轮空
    if (judgeWin(now_player))
    {
         int new_win_state = win_state + (1 << (now_player - 1));
         AI_dfs(evaluate_p, _player, next_player(now_player, new_win_state), dep, limit, new_win_state);
         return;
    }

    // 否则我们来对当前玩家的行棋进行选择，这里采用的是AI_choose里面相同的决策方法

    int size =  chess_array[now_player].size();
    // 首先排序一下，按照距离从远到近
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
            if (backwards(now_player, chess_array[now_player][i].first, chess_array[now_player][i].second, chess_array[now_player][j].first, chess_array[now_player][j].second))
            {
                int t = chess_array[now_player][i].first;  chess_array[now_player][i].first = chess_array[now_player][j].first; chess_array[now_player][j].first = t;
                t = chess_array[now_player][i].second;  chess_array[now_player][i].second = chess_array[now_player][j].second; chess_array[now_player][j].second = t;
            }
    }

    QVector<QPair<int,int>> select_p; // 存储选择走棋的位置
    QVector<QPair<int,int>> target_p; // 存储选择目标的位置

    // 下面找到所有可行的行棋方案

    select_count++;
    for (int i = 0; i < size; i++)
    {
        int x = chess_array[now_player][i].first;
        int y = chess_array[now_player][i].second;
        QPair<int,int> now_pair(x,y);
        QVector<QPair<int,int>> target = search(x,y);
        int t_size = target.size();
        int target_num = 0;
        int target_point_x[10];
        int target_point_y[10];
        int target_point_dis[10];
        //  这里首先做一遍筛选，往后走的点不要，前面已经选中的可能的目标点不要，在这之中选出距离顶点坐标最近的6个点
        for (int j = 0; j < t_size; j++)
        {
            if (target[j].first == x && target[j].second == y) continue;
            if ((!backwards(now_player, x, y, target[j].first, target[j].second)) && ifselected[target[j].first][target[j].second] != select_count)
            {
                    if (target_num < select_limit) // 还没满6个点，那么就直接加进去
                    {
                        target_point_x[target_num] = target[j].first;
                        target_point_y[target_num] = target[j].second;
                        target_point_dis[target_num] = AI_corner_dis(now_player, target[j].first, target[j].second);
                        target_num++;
                    }
                    else // 否则和已经选择的6个点进行比较，看能否取代距离顶点最远的那一个
                    {
                       int temp_max = -2147483648;
                       for (int k = 0; k < target_num; k++)
                          if (target_point_dis[k] > temp_max) temp_max = target_point_dis[k];
                       int cur_dis = AI_corner_dis(now_player, target[j].first, target[j].second);
                       if (cur_dis < temp_max) // 如果可以取代最远的那一个，那么就取代
                       {
                           for (int k = 0; k <target_num; k++)
                               if (target_point_dis[k] == temp_max)  // 找到距离最远的那一个
                               {
                                   target_point_x[k] = target[j].first;
                                   target_point_y[k] = target[j].second;
                                   target_point_dis[k] = cur_dis;
                                   break;
                               }
                       }
                   }
            }
        }
        // 然后我们要把当前棋子选中的目标点，标注为已选，并且加入到select的里面
        for (int j = 0; j < target_num; j++)
        {
            ifselected[target_point_x[j]][target_point_y[j]] = select_count;
            select_p.append(now_pair);
            QPair<int,int> t2(target_point_x[j],target_point_y[j]);
            target_p.append(t2);
        }
    }

    size = select_p.size();
    for (int i = 0; i < size; i++)
    {

        int x = select_p[i].first; int y = select_p[i].second;
        int tx = target_p[i].first; int ty = target_p[i].second;
        AI_update(_player,x,y,tx,ty); //假设按照这样进行选择
        AI_dfs(evaluate_p, _player, next_player(now_player,win_state), dep + 1, limit, win_state);
        AI_update(_player, tx,ty,x,y); //undo
    }

}

void Widget :: AI_choose(int _player,int& source_x, int& source_y, int& terminal_x, int& terminal_y)
{

  //  qDebug() << "prepare to choose for _player " ;
    hash_value = 0; // 棋盘的hash值，定义在累里面

    coordinate_mark++;
    for (int i = 1; i <= 6; i++) // i代表每一个玩家，遍历每一个玩家的所有棋子，计算哈希值
    {
        int size = chess_array[i].size();
        hash_value_player[i] = 0;
        for (int j = 0; j < size; j++)
        {
            int x = chess_array[i][j].first; int y = chess_array[i][j].second;
            //表示这个位置有棋子
            AI_mark[x][y] = coordinate_mark;
            AI_occupied[x][y] = i;
            //每个玩家各自的has值
            hash_value_player[i] = (hash_value_player[i] + base_pow[AI_pos[x][y]]) % mod_p;
        }
        // 每个玩家得到各自的hash值以后，我们还需要按照玩家的顺序进行整合，得到一个总体的棋盘的hash值
        hash_value = (hash_value + hash_value_player[i] * base_player[i])% mod_p;
    }
    //保存下当前格局的hash值
    hash_mark++;
    if_repeated[hash_value] = hash_mark;
    // AI部分对于参与决策时候的格局和AI给出的决策后的格局保证不会有重复的出现
    hash_history[hash_value] = 1;
   // qDebug() << " original hash value " << hash_value  ;

    QVector<QPair<int,int>> select_p; // 存储选择走棋的位置
    QVector<QPair<int,int>> target_p; // 存储选择目标的位置

    // 备选的一些行棋方案
    QVector<QPair<int,int>> can_select_p;
    QVector<QPair<int,int>> can_target_p;

    int size =  chess_array[_player].size();
    // 这里首先将目前玩家拥有的棋子进行排序，距离对角顶点比较远的棋子排在前面，具有对跳跃到的目标的优先选择权
    //因为我们希望滞后的棋子能够尽快走到靠前的位置
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
            if (backwards(_player, chess_array[_player][i].first, chess_array[_player][i].second, chess_array[_player][j].first, chess_array[_player][j].second))
            {
                int t = chess_array[_player][i].first;  chess_array[_player][i].first = chess_array[_player][j].first; chess_array[_player][j].first = t;
                t = chess_array[_player][i].second;  chess_array[_player][i].second = chess_array[_player][j].second; chess_array[_player][j].second = t;
            }
    }

   // qDebug() << "original chess " ;
    for (int i = 0; i < size; i++)
     //   qDebug() << convert_x(chess_array[_player][i].first, chess_array[_player][i].second) << " " << convert_y(chess_array[_player][i].second) ;

 //   qDebug() << "*************************" ;

    select_count++; // 给前面选择过的目标点打上标签，保证一个目标点不会有两个棋子跳到它

    for (int i = 0; i < size; i++)
    {
        int x = chess_array[_player][i].first;
        int y = chess_array[_player][i].second;
        QPair<int,int> now_pair(x,y);
        QVector<QPair<int,int>> target = search(x,y);
        int t_size = target.size();
        int target_num = 0;
        int target_point_x[10];
        int target_point_y[10];
        int target_point_dis[10];

     //   qDebug() << " search target points for "<< x << " " << y ;

        //  这里首先做一遍筛选，往后走的点不要，前面已经选中的可能的目标点不要，在这之中选出距离顶点坐标最近的6个点
        for (int j = 0; j < t_size; j++)
        {
      //      qDebug() << target[j].first << " " << target[j].second ;
            if (target[j].first == x && target[j].second == y) continue;
           // if (backwards(_player, x, y, target[j].first, target[j].second)) qDebug() << "backwards "  ;
          //  if (ifselected[target[j].first][target[j].second] == select_count) qDebug() << "ifselected" ;

            if ((!backwards(_player, x, y, target[j].first, target[j].second)) && ifselected[target[j].first][target[j].second] != select_count)
            {
                int temp_hash = hash_update(_player, x, y , target[j].first, target[j].second);
                if (!hash_history[temp_hash])  //  判断该决策是否会造成重复的格局，是的话就不选
                {
                    if (target_num < select_limit) // 还没满6个点，那么就直接加进去
                    {
                        target_point_x[target_num] = target[j].first;
                        target_point_y[target_num] = target[j].second;
                        target_point_dis[target_num] = AI_corner_dis(_player, target[j].first, target[j].second);
                        target_num++;
                    }
                    else // 否则和已经选择的6个点进行比较，看能否取代距离顶点最远的那一个
                    {
                       int temp_max = -2147483648;
                       for (int k = 0; k < target_num; k++)
                          if (target_point_dis[k] > temp_max) temp_max = target_point_dis[k];
                       int cur_dis = AI_corner_dis(_player, target[j].first, target[j].second);
                       if (cur_dis < temp_max) // 如果可以取代最远的那一个，那么就取代
                       {
                           for (int k = 0; k <target_num; k++)
                               if (target_point_dis[k] == temp_max)  // 找到距离最远的那一个
                               {
                                   target_point_x[k] = target[j].first;
                                   target_point_y[k] = target[j].second;
                                   target_point_dis[k] = cur_dis;
                                   break;
                               }
                       }
                   }
                }
            }
            else // 否则的话依然要加入到备选的里面
            {
                // 如果会造成重复的格局，那么备选也是不加入的
                int temp_hash = hash_update(_player, x, y , target[j].first, target[j].second);
                if (!hash_history[temp_hash])
                {
                    can_select_p.append(now_pair);
                    can_target_p.append(target[j]);
                }
            }
        }

     //   qDebug() << " ++++++++++++++++++++++++++ " ;
    //    qDebug() << "final choose ";
        // 然后我们要把当前棋子选中的目标点，标注为已选，并且加入到select的里面
        for (int j = 0; j < target_num; j++)
        {
            qDebug() << target_point_x[j] << " " << target_point_y[j] ;
            ifselected[target_point_x[j]][target_point_y[j]] = select_count;
            select_p.append(now_pair);
            QPair<int,int> t2(target_point_x[j],target_point_y[j]);
            target_p.append(t2);
        }
     //   qDebug() << " ****************************** " ;
    }

    //qDebug << " have searched the choices " << endl;

    // 根据不同的模式设置搜索后继步数的限制
    int step_limit;
    if (type == 2) step_limit = 4; else step_limit = 3;

    size = select_p.size();

    if (size == 0) // 无处可走
    {
        source_x = -1;  source_y = -1;
        terminal_x = -1; terminal_y = -1;

        //注意这里也不一定是真正的无处可走，而是在启发式的决策下没有相应符合要求的行棋策略
        // 这个时候，如果备选行棋方案里面，存在选择，那么就在备选的方案中选择一个评分函数比较高的决策作为此次AI的决策

        int n = can_select_p.size();
        if (!n) return;
        int max_score = -2147483648;
        for (int i = 0; i < n; i++)
        {
            int x = can_select_p[i].first; int y = can_select_p[i].second;
            int tx = can_target_p[i].first; int ty = can_target_p[i].second;
            AI_update(_player, x, y, tx, ty);
            int temp_score = AI_assess(_player, 0, 0);  // 表示这一步走棋后的局势评分
            AI_update(_player, tx, ty, x, y);
            if (temp_score > max_score)
            {
                max_score = temp_score;
                source_x = x; source_y = y;
                terminal_x = tx; terminal_y = ty;  // 记录这一步的行棋方案
            }
        }
        int temp_hash = hash_update(_player, source_x, source_y, terminal_x, terminal_y);
      //  if (hash_history[temp_hash] == 1)
         //   qDebug() << "whoops" ;
        hash_history[temp_hash] = 1; // 将已经决策过的hash值标记为1，避免陷入循环
      //  qDebug() << "final decision" ;
      //  qDebug() << source_x << " " << source_y << " " << terminal_x << " " << terminal_y ;
      //  qDebug()  << temp_hash ;
        // 找到评分最大的返回即可
        return;

    }

    //qDebug << "have optimistic choices " << endl;

    // 当前的胜负状态
    // 二进制压位，作为状态传送
    int win_state = 0;
    for (int i = 1; i <= 6; i++)
    {
         win_state+=judgement[i] * (1 << (i - 1));
    }

    QVector <int> Min;
    QVector <double> Avg;
    // 下面循环所有走棋的选择
    for (int i = 0; i < size; i++)
    {

        int x = select_p[i].first; int y = select_p[i].second;
        int tx = target_p[i].first; int ty = target_p[i].second;

        AI_update(_player,x,y,tx,ty); //假设按照这样进行选择
        //这里有可能会需要使用更大的整数类型
        QVector <int> evaluate_p; // 得到后继状态的评分函数值的集合

        AI_dfs(evaluate_p, _player, next_player(_player,win_state), 1, step_limit, win_state);

        AI_update(_player, tx,ty,x,y); //undo

       // qDebug << "********* Evaluation end" << x << " " << y << " " << tx << " " << ty << endl;
        int e_size = evaluate_p.size();
        int now_min = int_limits;
        double total = 0;  // 分别求得这些局面的评分函数的平均值和最小值
        for (int j = 0; j < e_size; j++)
        {
            if (evaluate_p[j] < now_min) now_min = evaluate_p[j];
            total = total + (double)evaluate_p[j];
        }
        double now_avg = total/(double)e_size;
        Min.append(now_min);
        Avg.append(now_avg);
    }

    // 现在我们可以结合上面两个指标来选择一个最优的解
    // 在假设对手是最优决策的情况下，我们应该选择最小值最大的那个
    // 在假设对手是随机选择的情况下，我们应该选择平均值最大的那个
    // 这里我们先选择采用平均值的决策
    int now_max = 0;
    for (int i = 0; i < size; i++)
        if (Avg[i] > Avg[now_max]) now_max = i;
    //做出选择
    source_x = select_p[now_max].first; source_y = select_p[now_max].second;
    terminal_x = target_p[now_max].first; terminal_y = target_p[now_max].second;
    // 将这个决策后的hash值记录下来，后面不能再重复这个选择
    int temp_hash = hash_update(_player, source_x, source_y, terminal_x, terminal_y);
//    if (hash_history[temp_hash] == 1)
//        qDebug() << "whoops" ;
    hash_history[temp_hash] = 1;
//    qDebug() << "final decision" ;
//    qDebug() << source_x << " " << source_y << " " << terminal_x << " " << terminal_y ;
//    qDebug()  << temp_hash ;

}


void Widget :: on_AIButton_clicked()
{
    if (AI_on) // 如果此时AI模式是开启的
    {
        // 需要关闭AI模式
        AI_on = 0;
        this -> control = this -> temp_control; // 将控制权返还给玩家
    }
    else // 如果此时AI模式是关闭的
    {
        // 需要开启AI模式
        AI_on = 1;
        this -> temp_control = this -> control;
        this -> control = -1;
        AI_move(); // 调用AI进行行走
    }
}

void  Widget :: AI_move()
{
    if (this -> temp_control != player) return;   //不是当前玩家的轮次的时候禁止下棋
  //  this -> control = -1; // 按下AI模式后，就不允许当前玩家自行进行操作

    int source_x, source_y, terminal_x, terminal_y;

    //qDebug << "prepare to enter AI_choose" << endl;

    AI_choose(player,source_x,source_y,terminal_x, terminal_y);  // 获得对应的行棋方案

    if (source_x == -1 && source_y == -1 && terminal_x == -1 && terminal_y == -1) // 这种情况代表AI找不到任何决策
    {
       // it is not supposed to happen
        return;
    }

    Trail_off = 1; // 关闭轨迹显示
    //qDebug << "get the choice " << source_x << " " << source_y << " " << terminal_x << " " << terminal_y << endl;

    int cx = get_Coordinatex(g[source_x][source_y]); int cy = get_Coordinatey(g[source_x][source_y]);
    this -> select = Coordinate[cx][cy].chess;
    int cx2 = get_Coordinatex(g[terminal_x][terminal_y]); int cy2 = get_Coordinatey(g[terminal_x][terminal_y]);
    Coordinate_struct* ptr = &Coordinate[cx2][cy2];
    bool canmove =  isLegalMove(select, ptr);
    // 在发送signal_move之前要确保已经保存好了行棋的路径, 这里借助一下isLegalMove的函数来获取一下路径
    emit signal_move(); // 发送信号给客户端请求移动
    //由于自己将不再接收到move up的信号，所以会自行行棋
    if (canmove)
    {
        QString my_trail = getTrail();
        this -> setmove(player, my_trail);
    }

    //恢复配置
  //  this->select = nullptr;
  //  this -> control = player;
}



// ******** end of new

// ******* new for extra tasks


void Widget :: displaypatch(QString player_trail, bool flag)
{

    int tempx[1000];  //存储行棋轨迹的坐标，这里存储的是展现在画面上的坐标轨迹
    int temp_num = 0;
    QString str = "";
    int len = player_trail.length();
    for (int i = 0; i <len; i++)
        if (player_trail.at(i) == ' ')
        {
            temp_num++;
            tempx[temp_num] = str.toInt();
            str = "";
        }
        else str = str + player_trail.at(i);
    // 最后一个没有空格不要忘记处理
    temp_num++;
    tempx[temp_num] = str.toInt();

    // 转换后得到实际画面上对应的坐标

    s_trail_num = 0;
    for (int i = 1; i <= temp_num; i += 2)
    {
        int x = tempx[i]; int y = tempx[i+1];
        s_Trail_x[s_trail_num] = reconvert_x(rotate_x(x,y),rotate_y(x,y));
        s_Trail_y[s_trail_num] = reconvert_y(rotate_y(x,y));
        s_trail_num++;
    }

    if (flag) // 此时是展示历史路径的操作，所以这里需要先定义一下select
    {
        int x = s_Trail_x[s_trail_num -1];
        int y = s_Trail_y[s_trail_num -1];
        int cx = get_Coordinatex(g[x][y]); int cy = get_Coordinatey(g[x][y]);
        this -> select = Coordinate[cx][cy].chess; // 此时需要展示的棋子一定在这个路径的终点处
        // 下面将这个棋子的图片移动到路径的起点处
        x = s_Trail_x[0];
        y = s_Trail_y[0];
        this -> select -> move(x - chess_bias , y - chess_bias);
    }
    if ((!flag) && (Trail_off)) // 如果不是玩家手动操作路径展示并且此时设置关闭了路径展示，那么就不要展示此时的路径
    {
        // 但还是需要执行一下预后的部分
        endShowTrail();
        return;
    }

    showTrail(); //开始展现路径的动画效果


}

void Widget :: showTrail()
{
    /*
     if (i == s_trail_num - 1) // 已经是最后一个不要再加上动画效果了
      {
          endShowTrail();  // 结束路径动态展示后的处理
          return;
      }
      QPropertyAnimation *animation = new QPropertyAnimation(select, "pos");
      animation->setDuration(300);
      animation->setStartValue(QPoint(s_Trail_x[i] - chess_bias, s_Trail_y[i] - chess_bias));
      animation->setEndValue(QPoint(s_Trail_x[i+1] - chess_bias, s_Trail_y[i+1] - chess_bias));
      animation -> start(QAbstractAnimation::DeleteWhenStopped);
      connect(animation, SIGNAL(finished()), this, SLOT(showTrail(i+1)));
     */
    // 串行执行的动画组
   // qDebug() << " enter show Trail()" ;
    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    int a_count = 0;
    for  (int i = 0; i < s_trail_num - 1; i++)
    {
     //   qDebug() << "add animation " << i;
         animation[a_count] = new QPropertyAnimation(select, "pos");
         animation[a_count]->setDuration(300);
         animation[a_count]->setStartValue(QPoint(s_Trail_x[i] - chess_bias, s_Trail_y[i] - chess_bias));
         animation[a_count]->setEndValue(QPoint(s_Trail_x[i+1] - chess_bias, s_Trail_y[i+1] - chess_bias));
    //    qDebug() << "here1 " << i;
         group->addAnimation(animation[a_count]); // 将当前动画片段添加到动画组中
      //  qDebug() << "here2 " << i;

        a_count++;
        if (i != (s_trail_num - 2)) // 如果还是中间点
        {
          // 下面添加一个停顿，这样视觉效果会更加清晰一些
          animation[a_count] = new QPropertyAnimation(select, "pos");
          animation[a_count]->setDuration(100);
          animation[a_count]->setStartValue(QPoint(s_Trail_x[i+1] - chess_bias, s_Trail_y[i+1] - chess_bias));
          animation[a_count]->setEndValue(QPoint(s_Trail_x[i+1] - chess_bias, s_Trail_y[i+1] - chess_bias));
          group->addAnimation(animation[a_count]);
          a_count++;
        }
    }
 //   qDebug() << " finish adding";
    group->start(QAbstractAnimation::DeleteWhenStopped); // 执行动画组中的所有片段
 //   qDebug() << "after start";
    connect(group, &QSequentialAnimationGroup::finished, this, &Widget::endShowTrail); // 执行完毕动画后执行endShowTrail()里面的内容
 //   qDebug() << "after connect";

}

void Widget :: endShowTrail()
{
 //   qDebug() << "enter endShowTrail";
    // 最后需要将图片定格在终点的位置
    int x = s_Trail_x[s_trail_num -1];
    int y = s_Trail_y[s_trail_num -1];
   // qDebug() << "get x and y successfully"
    this -> select ->setGeometry(x - chess_bias, y - chess_bias,chess_size,chess_size);
  //  qDebug() << " set Geometry successfully";
    // 恢复设置
    this -> select = nullptr;
  //  qDebug() << "finish endShowTrail";
}


void  Widget :: on_GreenButton_clicked()
{
 //   qDebug() << "enter on_GreenButton_clicked()" ;
     checkplayer = id_name[1];
 //   qDebug() << checkplayer ;
     emit  checktrail();

}

void  Widget :: on_PurpleButton_clicked()
{
   // qDebug() << "enter on_PurpleButton_clicked()" ;
    checkplayer = id_name[2];
  //  qDebug() << checkplayer ;
    emit  checktrail();
}

void  Widget :: on_BlueButton_clicked()
{
    checkplayer = id_name[3];
    emit  checktrail();
}

void  Widget :: on_OrangeButton_clicked()
{
    checkplayer = id_name[4];
    emit  checktrail();
}

void  Widget :: on_YellowButton_clicked()
{
    checkplayer = id_name[5];
    emit  checktrail();
}

void  Widget :: on_RedButton_clicked()
{
    checkplayer = id_name[6];
    emit  checktrail();
}

/*

Coordinate[i][j].chess->setIconSize(QSize(chess_size,chess_size));
Coordinate[i][j].chess->setGeometry(Coordinate[i][j].x-chess_bias,Coordinate[i][j].y-chess_bias,chess_size,chess_size);
//设置信号
connect(Coordinate[i][j].chess,SIGNAL(chessInfo(int,int,int)),this,SLOT(chessClick(int,int,int)));
//设置位置
Coordinate[i][j].chess -> setLocation(i,j);
Coordinate[i][j].chess -> x = Coordinate[i][j].x;
Coordinate[i][j].chess -> y = Coordinate[i][j].y;
Coordinate[i][j].chess -> setPlayer(re_Coordinate_pos[i]);


Coordinate[mx2][my2].chess = Coordinate[mx1][my1].chess; Coordinate[mx1][my1].chess = nullptr;
Coordinate[mx2][my2].chess -> setLocation(mx2,my2);
Coordinate[mx2][my2].chess->x = Coordinate[mx2][my2].x;
Coordinate[mx2][my2].chess->y = Coordinate[mx2][my2].y;
Coordinate[mx2][my2].chess -> move(x2 - chess_bias , y2 - chess_bias);

 */


// ****** end of extra tasks
