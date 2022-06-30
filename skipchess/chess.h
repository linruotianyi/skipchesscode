
#include <QPushButton>
#include <string>
using  std::string;
//棋子类，属于按钮
class Chess : public QPushButton
{
    Q_OBJECT
public:
    Chess(QWidget *parent = 0);
    Chess(int _x , int _y , int _line , int _col,int _player);
    ~Chess();
    void setLocation(int line,int col);
    int getLine();
    void setLine(int guess);
    int getCol();
    void setCol(int col);
    void setClick(int click);
    int getClick();
    void setStatus(int s);
    int getStatus();
    void setPlayer(int Player);
    int getPlayer();
    int x;
    int y;
protected:
    void mousePressEvent(QMouseEvent* event);
private:

    int line;           //所在棋盘的行数
    int col;            //所在棋盘的列数
    int player;         //表明棋子属于哪个玩家
    int click;          //是否被选中
//下面是信号
signals:
    void chessInfo(int,int,int);                                //改多一个参数 int 增加了玩家参数
};

//#endif // PIECE_H
