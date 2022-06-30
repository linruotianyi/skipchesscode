#include "network.h"
#include "ui_network.h"
#include "serverwindow.h"
#include "netchoose.h"
NetWork::NetWork(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetWork)
{
    QPixmap pixmap(":/image2/back8.jpg");

    QPalette pale;
    pale.setBrush(backgroundRole(), QBrush(pixmap));
    this->setPalette(pale);
    ui->setupUi(this);
}

NetWork::~NetWork()
{
    delete ui;
}

void NetWork::on_pushButton_clicked()
{
    ServerWindow *server=new ServerWindow;
    server->show();
    this->hide();

}



void NetWork::on_pushButton_2_clicked()
{
    NetChoose *netch=new NetChoose;
    netch->show();
    this->hide();

}

