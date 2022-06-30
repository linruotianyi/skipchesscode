#include"chess.h"
#include<QString>
#include<QDebug>
#include<QMouseEvent>
#include<QToolTip>
using namespace std;
Chess :: Chess(QWidget *parent) :
    QPushButton(parent)
{
    setClick(0);
}
Chess :: ~Chess()
{

}
void Chess :: setLocation(int line,int col)
{
    this->line = line;
    this->col = col;
}
void Chess :: setLine(int line)
{
    this->line = line;
}

void Chess :: setCol(int col)
{
    this->col = col;
}

void Chess :: setClick(int click)
{
    this->click = click;
}

void Chess :: setPlayer(int _player)
{
    this -> player = _player;
}

int Chess :: getLine()
{
    return this->line;
}

int Chess :: getCol()
{
    return this->col;
}


int Chess :: getClick()
{
    return this->click;
}

int Chess :: getPlayer()
{
    return this -> player;
}

Chess :: Chess (int _x , int _y , int _line , int _col, int _player) :x(_x) , y(_y) , line(_line) , col(_col), player(_player)
{
    click = 0; // 初始化chess没有被点击的状态
}

void Chess :: mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        //qDebug()<<"a";
        this->setClick(1);
        emit chessInfo(this -> getPlayer(),this->getLine(), this->getCol());
        //qDebug()<<"a";
    }

}


