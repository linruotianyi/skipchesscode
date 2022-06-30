#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  //创建一个应用程序对象
    mainwindow TopWin;
    TopWin.show();   //顶层窗口
    return a.exec(); //程序的生命循环和消息循环
}
