#include "connectsuc.h"
#include "ui_connectsuc.h"
#define MainWin_Height (400)
#define MainWin_Width (400)

ConnectSuc::ConnectSuc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectSuc)
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

    // 设置提示成功连接的标语
    ui->label->setText("连接成功");

}

ConnectSuc::~ConnectSuc()
{
    delete ui;
}


//void ConnectSuc::on_pushButton_clicked() // 点击关闭按钮，关闭当前窗口
//{
//    this -> close();
//}




