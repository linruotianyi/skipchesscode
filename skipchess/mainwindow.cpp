
/*
void mainwindow::on_pushButton_clicked()
{
    main2w * j = new main2w;
    j->show();
    this->close();

}


void mainwindow::on_pushButton_2_clicked()
{
        this->close();
}

 */

// #include "main2w.h"
#include <networkserver.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPalette>
#include <QIcon>
#include "widget1.h"
#include "choose.h"
#define MainWin_Height (800)
#define MainWin_Width (700)
#include "choose1.h"
mainwindow :: mainwindow(QWidget *parent) : QWidget(parent), ui(new Ui :: mainwindow)
{
    ui->setupUi(this); //使用UI文件
    //设置固定的屏幕大小
    this->setMinimumSize(MainWin_Height, MainWin_Width);
    this->setMaximumSize(MainWin_Height, MainWin_Width);
     //设置为无边框
    //this->setWindowFlags(Qt::FramelessWindowHint);    还是要有边框
    //设置窗口部件的背景色
    //QPalette p;   // 调色板
    //p.setColor(QPalette::Window, Qt::blue);


//    QPixmap pixmap(":/image2/back.png.png");
//    QPalette pale;
//     pale.setBrush(backgroundRole(), QBrush(pixmap));
//    this->setPalette(pale);
    QPixmap pixmap(":/image2/back8.jpg");

     QPalette pale;
     pale.setBrush(backgroundRole(), QBrush(pixmap));
     this->setPalette(pale);

    this->setWindowIcon(QIcon(":/image/icon.jpg"));  //主窗口设置icon
    this->setWindowTitle("跳棋游戏"); // 设置窗口的名字

}
mainwindow :: ~mainwindow()
{
    delete ui; // 析构的时候删除ui文件
}

void mainwindow::on_pushButton_clicked()
{
   Choose1 *i=new Choose1;
    this->hide();
    i->show();
}


void mainwindow::on_pushButton_2_clicked()
{
    this->close();
}

