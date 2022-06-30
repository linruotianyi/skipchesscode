#include "choose1.h"
#include "ui_choose1.h"
#include"choose.h"
#include"network.h"
Choose1::Choose1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Choose1)
{
    QPixmap pixmap(":/image2/back8.jpg");

     QPalette pale;
     pale.setBrush(backgroundRole(), QBrush(pixmap));
     this->setPalette(pale);

    ui->setupUi(this);
}

Choose1::~Choose1()
{
    delete ui;
}

void Choose1::on_pushButton_clicked()
{
    choose* i =new choose;
    this->hide();
    i->show();
}


void Choose1::on_pushButton_2_clicked()
{
    NetWork *i=new NetWork;
    this->hide();
    i->show();
}

