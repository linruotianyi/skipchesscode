#include "netchoose.h"
#include "ui_netchoose.h"

#define MainWin_Height (800)
#define MainWin_Width (700)

NetChoose::NetChoose(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetChoose)
{
    ui->setupUi(this);
    // 设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    // 设置背景
    QPixmap pixmap(":/image2/back8.jpg");

     QPalette pale;
     pale.setBrush(backgroundRole(), QBrush(pixmap));
     this->setPalette(pale);

}

NetChoose::~NetChoose()
{
    delete ui;
}


void NetChoose::on_pushButton_clicked() // 双人模式
{
    Room* r = new Room(2);
    r -> show();
    this->hide();
    // 注意当前窗口不需要隐藏，还要继续点击用
}


void NetChoose::on_pushButton_2_clicked() // 三人模式
{
    Room* r = new Room(3);
    r -> show();
    this->hide();
}

void NetChoose :: on_pushButton_3_clicked() // 六人模式
{
    Room* r = new Room(6);
    r -> show();
    this->hide();
}






