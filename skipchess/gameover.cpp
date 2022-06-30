#include "gameover.h"
#include "ui_gameover.h"
#define MainWin_Height (800)
#define MainWin_Width (700)

GameOver:: GameOver(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameOver)
{
    ui->setupUi(this);
    // 设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    // 设置背景
//    QPixmap pixmap(""); // 添加图片路径
//    QPalette pale;
//    pale.setBrush(backgroundRole(), QBrush(pixmap));
//    this->setPalette(pale);
    //设置提示语，表示游戏结束
    this->setWindowTitle("游戏结束");

}

GameOver:: GameOver(QString str, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameOver)
{
    ui->setupUi(this);
    // 设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    // 设置背景
    QPixmap pixmap(":/image2/back8.jpg"); // 添加图片路径
    QPalette pale;
    pale.setBrush(backgroundRole(), QBrush(pixmap));
    this->setPalette(pale);
    //设置提示语，表示游戏结束
    this->setWindowTitle("游戏结束，排名如下");
    // 下面显示玩家的排名
    //这里具体显示的方式应该也需要调整一下
    int len = str.length();
    int rank = 0;
    QString name = "";
    for (int i = 0; i <= len; i++)
        if (i == len || str.at(i) == ' ')
        {

            rank++;

            //QString r = rank.toString();
            QString r=QString::number(rank);
            r+= "       ";
            r+= name;
           label=findChild<QLabel *>("label_"+QString::number(rank));
           label -> setText(r);
           name="";
        }
        else name = name + str.at(i);

}

// 此时页面上应该唯一的退出游戏的按钮,点击后关闭窗口
//void GameOver :: on_pushButton_clicked()
//{
//    this -> close();
//}

GameOver::~GameOver()
{
    delete ui;
}
