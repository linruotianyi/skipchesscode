#include "newroom.h"
#include "ui_newroom.h"
#define MainWin_Height (400)
#define MainWin_Width (400)

NewRoom::NewRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewRoom)
{
    ui->setupUi(this);
    // 设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    ui->label->setText("当前房间的游戏正在进行，请换一个房间加入");
}

NewRoom::~NewRoom()
{
    delete ui;
}
