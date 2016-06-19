#ifndef PASSWSURE_H
#define PASSWSURE_H

#include <QDialog>

namespace Ui {
class PasswSure;
}

class PasswSure : public QDialog
{
    Q_OBJECT

public:
    explicit PasswSure(QWidget *parent = 0);
    ~PasswSure();

signals:

private slots:
    void on_pushButton_clicked();

    void timeOut();

    void recvStudent();         //匹配学生密码

    void recvTeachOut();        //匹配教师密码

    void recvTeachRe();         //匹配教师恢复资格密码

    void recvFellowDe();        //删除奖学金记录密码匹配

    void recvSuper();           //超级管理员

    void closes();

private:
    Ui::PasswSure *ui;
    int No;
};

#endif // PASSWSURE_H
