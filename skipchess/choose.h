#ifndef CHOOSE_H
#define CHOOSE_H

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class choose; }
QT_END_NAMESPACE
class choose : public QWidget
{
    Q_OBJECT
public:
    explicit choose(QWidget *parent = nullptr);
    ~choose();
signals:

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private :
    Ui :: choose *ui;

};

#endif // CHOOSE_H
