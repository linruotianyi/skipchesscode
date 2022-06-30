#include "connectfai.h"
#include "ui_connectfai.h"
#define MainWin_Height (400)
#define MainWin_Width (400)

ConnectFai::ConnectFai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectFai)
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
    ui->label->setText("连接失败，请重新尝试");

}

ConnectFai::~ConnectFai()
{
    delete ui;
}


//void ConnectFai::on_pushButton_clicked() // 点击关闭按钮，关闭当前窗口
//{
//    this -> close();
//}




