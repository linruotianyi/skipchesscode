#ifndef JOINFAIL_H
#define JOINFAIL_H

#include <QMainWindow>

namespace Ui {
class JoinFail;
}

class JoinFail : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoinFail(QWidget *parent = nullptr);
    ~JoinFail();

private:
    Ui::JoinFail *ui;
};

#endif // JOINFAIL_H
