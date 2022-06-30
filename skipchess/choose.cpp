#include "choose.h"
#include "ui_choose.h"
#include "widget1.h"
//该窗口用于处理单机的窗口生成，所以bool直接传false
choose::choose(QWidget *parent)
    : QWidget{parent}, ui(new Ui :: choose)
{
    QPixmap pixmap(":/image2/back8.jpg");

     QPalette pale;
     pale.setBrush(backgroundRole(), QBrush(pixmap));
     this->setPalette(pale);
    ui->setupUi(this);
}
choose:: ~choose()
{
     delete ui;
}

void choose::on_pushButton_clicked()
{
    Widget *a=new Widget(2,false);
    a->setcontrol();                                                 //添加一个对control的赋值操作 control的循环在另一函数实现
    this->hide();
    a->show();
}


void choose::on_pushButton_2_clicked()
{
    Widget *a=new Widget(3,false);
    a->setcontrol();
    this->hide();
    a->show();
}


void choose::on_pushButton_3_clicked()
{
    Widget *a=new Widget(6,false);
    a->setcontrol();
    this->hide();
    a->show();
}

