#ifndef WIN_H
#define WIN_H

#include <QMainWindow>

namespace Ui {
class Win;
}

class Win : public QMainWindow
{
    Q_OBJECT

public:
    explicit Win(QWidget *parent = nullptr);
    ~Win();

private:
    Ui::Win *ui;
};

#endif // WIN_H
