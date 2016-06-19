#ifndef REGISTER_H
#define REGISTER_H

#include "login.h"
#include <QDialog>
#include <QDebug>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

protected:
    void paintEvent(QPaintEvent *);

    bool Database(QString sql);    //查询数据库，检查当前账号和邮箱是否存在

    bool UserRegister();           //注册数据库操作

signals:

protected slots:
    void recvDatabaseFromLogin(QSqlDatabase dbl);    //登录sendDatabaseToRegister的槽函数

private slots:
    bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

    void on_out_clicked();          //退出按钮

    void on_register_2_clicked();   //注册按钮

    void TimeOut();                 //计时器关联函数

    void on_time_2_clicked();

    void on_timetable_clicked(const QDate &date);

private:
    Ui::Register *ui;
    int passcount;       //通过此字段判断是否可以注册
    QSqlDatabase db;
};

#endif // REGISTER_H
