#ifndef NEWROOM_H
#define NEWROOM_H

#include <QMainWindow>

namespace Ui {
class NewRoom;
}

class NewRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewRoom(QWidget *parent = nullptr);
    ~NewRoom();

private:
    Ui::NewRoom *ui;
};

#endif // NEWROOM_H
