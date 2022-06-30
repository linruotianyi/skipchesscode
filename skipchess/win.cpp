#include "win.h"
#include "ui_win.h"
#define MainWin_Height (400)
#define MainWin_Width (400)

Win::Win(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Win)
{
    ui->setupUi(this);
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
    ui->label->setText("你已经取得胜利");
}

Win::~Win()
{
    delete ui;
}
