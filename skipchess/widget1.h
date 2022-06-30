//#ifndef WIDGET1_H
//#define WIDGET1_H

#include <QWidget>         // 基本窗口widget
#include <QPen>            // 画笔
#include <QBrush>          // 画刷
#include <QPainter>        // 画布
#include <QPoint>          // 点
#include <QColor>          // 颜色
#include <QColorDialog>    // 颜色Dialog
#include <QMouseEvent>     // 鼠标事件
#include <QPaintEvent>     // 绘画事件
#include <vector>           //迭代器
#include <QRectF>
#include "chess.h"           //棋子结构
#include <Qlabel>
#include "gameover.h"
#include "win.h"
#include <QTime>
#include <QTimer>
#include "illegalmove.h"
#include<QPropertyAnimation>
#include <QSequentialAnimationGroup>
//  下面几个参数可以固定整个棋盘
#define half_edge (15)  //等边三角形一半的边长
#define _edge (26)  //等边三角形的高
#define A_top_x (260)
#define A_top_y (20) // A的第一个点的横纵坐标
#define MaxCountNum (15)
//下面是一些其它的参数
#define chess_size (25)  //棋子的图片的大小
#define chess_bias (10) //棋子相对于棋盘的中心点的位置的偏移

#define Max_Matrix (700)  //坐标系的大小
#define Dis_lim (20) // 允许点击的位置距离棋子中心的位置的最远的距离
#define select_limit (4)

//*****  new for task 3
#define int_limits (2147483647);

// 这里的素数对也可以换成别的
#define mod_p (1000000007)
#define base_p  (911)
#define chess_total (121)

#define extra_score (2000)

const int center_x = A_top_x;
const int center_y = A_top_y + 8 * _edge;

//*****  end of new

typedef struct
{
    int x;
    int y;
    int Value;
    int ifOccupied; //0代表上面没有棋子，否则代表上面棋子的编号。
    Chess* chess; //棋子指针
}Coordinate_struct;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
   // Widget(int type=2,QWidget *parent = nullptr);
    Ui::Widget *ui;
    Widget(int type=2,bool _ifcon=0 , QString = "",  QWidget *parent = nullptr);
    void set3people_mode();
    void set2people_mode();
    void set6people_mode();
    void setcontrol(){control=1;}
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mapping();
    void setCoordinate();
    void assign_xy(int area, int inv, int topx, int topy);
    void setplayer(int _player);
    void setLeave (QString _name);
    void setorder();
    void player_turn(int _player);
   // void setmove(int _player, int x, int y, int tx, int ty);
    void setmove(int _player, QString player_trail);
    void endGame(QString _list);
    void setWin();
    void moveFail(int _player);
    int isGameOver();
    QString getTrail();
    void storeplayer(int _player, QString _name);
    ~Widget();
    QTimer *timer ;
    QTime  time;
    int Countnum;
    static int flag;
    Coordinate_struct Coordinate[10][100];

    // ***** 关于超时判负
    void setOutofTime(int this_player); // 将某个玩家设置为超时判负
    // ****

    void displaypatch(QString player_trail, bool flag);
    QString checkplayer;


signals :
    void exit();
    void signal_move();
    void checktrail();

public slots :
      void chessClick(int _player, int tx , int ty);
      void on_pushButton_clicked();  // 对应按下了退出游戏的按钮
      void Countdown();
      void endShowTrail();
    //void on_push_AI_Button();   // 表示对应按下了AI模式的按钮
    // 玩家应在获得控制权，提示下棋的时候才按下这个按钮，并且按下这个按钮后会自动走棋


private slots:
      void on_AIButton_clicked();
      void on_GreenButton_clicked();
      void  on_PurpleButton_clicked();
      void  on_BlueButton_clicked();
      void on_OrangeButton_clicked();
      void on_YellowButton_clicked();
      void  on_RedButton_clicked();



private:
    //一些内部的辅助用的函数就先放在private里面
    void set_mode();
    void judgeResult();
    bool onboard(int x, int y);
    bool isLegalMove(Chess* prechess , Coordinate_struct* des);
    int get_Coordinatex(int value);
    int get_Coordinatey(int value);
    bool ifreach(int x , int y , Coordinate_struct* des);
    void Update (int x1, int y1, int x2, int y2);
    void draw_BottomCol(int area, int x, int y, int inv);
    const int dx[6] = {-(half_edge * 2), -half_edge, half_edge, half_edge * 2, half_edge , -half_edge};
    const int dy[6] = {0, -_edge, -_edge, 0, _edge, _edge};
    double get_dis(int x, int y, int tx, int ty)
    {
        return  sqrt((x - tx) * (x - tx) + (y - ty) * (y - ty));
    }
// 这里删除掉了一个chess_array
    QVector<Coordinate_struct*> places;      //可点击的有效区域
    int g[Max_Matrix][Max_Matrix]; //坐标到传输值的映射
    int corner[10];
    int judgement[10]; //存储最后胜负的结果
    int count; //计数打标签用
    int iftouch[Max_Matrix][Max_Matrix]; //判断某个点是不是已经递归过
    int control; //当前由哪一个玩家控制
    bool vis[10];
    int clickTime; // 点击次数
    int type;
    Chess* select; //选中的棋子
    bool ifconnected;  // 判断是否是联机模式
    int player; // 存储当前玩家的信息
    int success_num;
    int success_player[10];
    int x_trail[1000];
    int y_trail[1000];
    int trail_count;
    bool ishere[10];    //判断玩家是否在
    void addtrail(int _x, int _y);

    // ***** update for task3
    int convert_x(int _x, int _y);
    int convert_y(int _y);
    int reconvert_x(int _x, int _y);
    int reconvert_y(int _y);

    // *****  end of update
    int player_order[10];
    int present_order;
    QString id_name[10]; // 存储玩家和昵称的对应关系
    QString player_name;

    // ******* new for task3
    QMap<int,int> if_repeated;
    long long base_pow[200];
    long long base_player[10];
    bool judgeWin(int _player);  // 判断这个玩家是否是一个新增的获胜者
    int AI_dis(int x, int y, int tx, int ty);
    bool backwards(int _player, int x, int y, int tx, int ty);
    bool checkin(int _player, int win_state);
    int AI_assess(int _player,int search_step,int search_limit);
    int AI_pos[Max_Matrix][Max_Matrix];
    void AI_hash_preprocessing();
    int AI_mark[Max_Matrix][Max_Matrix];
    void leap_search(QVector<QPair<int,int>>& ans, int x, int y);
    QVector<QPair<int,int>> search(int x,int y);
    long long hash_value_player[10];  // 存储每个玩家对应的hash值
    long long hash_value;   //存储整个棋盘的hash值
    void AI_update(int _player, int x,int y, int tx, int ty);
    int hash_mark; // 用来标记是不是被访问的格局
    void AI_dfs(QVector<int>& evaluate_p, int _player, int now_player, int dep,int limit, int win_state);

    void AI_choose(int _player,int& source_x, int& source_y, int& terminal_x, int& terminal_y);
    QVector<QVector<QPair<int,int>>> chess_array; // 维护每个玩家棋子所在的坐标，更新的时候进行维护
    int coordinate_mark;
    int next_player(int,int);
    //  ******  end of new

    int Coordinate_pos[10];
    int re_Coordinate_pos[10];
    int rotate_x(int x, int y);
    int rotate_y(int x, int y);
    int re_rotate_x(int x, int y);
    int re_rotate_y(int x, int y);
    int temp_bias[10]; // 代表不同player在下面的时候坐标应该转过几个60度


    // ***** new for extra tasks
    int hash_update(int,int,int,int,int);
    int s_trail_num;
    int s_Trail_x[1000];
    int s_Trail_y[1000];
    void showTrail();
    Chess* temp_chess[1000];
    //void endShowTrail();
    QMap<int,bool> hash_history;
    int select_count;
    int ifselected[Max_Matrix][Max_Matrix];
    int AI_corner_dis(int _player, int x, int y);
    bool Trail_off;
    // ****** end of extra

    int level[10][10];
    int level_sep[5];
    int AI_occupied[Max_Matrix][Max_Matrix];
    bool AI_on;
    void AI_move();
    QPainter* PathPainter;
    int temp_control;
    QPropertyAnimation* animation[2000];

};


//#endif



/*
 一些需要更改的地方：



 1、下面内容需要更改一下
 setleave里面的标语，用别的label
setOutofTime里面和setleave里面用同一个label

2、实测一下AI的效果，进行调试



3、图片美化

4、 所有的异常情况都要尝试一下，看看是否正常



 */
