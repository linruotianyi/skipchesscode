//#ifndef MAINWINDOW_H
//#define MAINWINDOW_H

//#include <QWidget>

/*
class mainwindow : public QWidget
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();
private slots:
    void on_pushButton_clicked();

   void on_pushButton_2_clicked();


private:
    Ui::mainwindow *ui;
};

 */

//#endif // MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QWidget
{
    Q_OBJECT

public :
    mainwindow (QWidget *parent = nullptr); //顶层窗口
    ~mainwindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private :
    Ui :: mainwindow *ui;

};
