#include "illegalmove.h"
#include "ui_illegalmove.h"
#define MainWin_Height (400)
#define MainWin_Width (400)

illegalMove::illegalMove(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::illegalMove)
{
    ui->setupUi(this);
    // 设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);

    // 设置提示成功连接的标语
    ui->label->setText("当前玩家非法行棋");

}

illegalMove::~illegalMove()
{
    delete ui;
}
