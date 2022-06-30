#ifndef NETROOM_H
#define NETROOM_H

#include <QMainWindow>

namespace Ui {
class NetRoom;
}

class NetRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetRoom(QWidget *parent = nullptr);
    ~NetRoom();

private:
    Ui::NetRoom *ui;
};

#endif // NETROOM_H
