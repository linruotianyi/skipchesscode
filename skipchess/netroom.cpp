#include "netroom.h"
#include "ui_netroom.h"

NetRoom::NetRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetRoom)
{
    ui->setupUi(this);
}

NetRoom::~NetRoom()
{
    delete ui;
}
