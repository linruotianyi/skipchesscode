#include "joinfail.h"
#include "ui_joinfail.h"
#define MainWin_Height (400)
#define MainWin_Width (400)

JoinFail::JoinFail(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoinFail)
{
    ui->setupUi(this);
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    ui->label->setText("加入房间失败");
}

JoinFail::~JoinFail()
{
    delete ui;
}
