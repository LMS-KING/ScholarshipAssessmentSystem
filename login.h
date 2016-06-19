#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "studentmain.h"
//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);

    Login(QString username);

    ~Login();

signals:
    void sendUsername(QString username, QString number);    //登录成时向主窗口发送用户姓名

    void sendFellowMessage(QSqlDatabase db);       //学生登录成功时更新主界面奖学金通知

    void sendDatabaseToRegister(QSqlDatabase db);   //点击注册时传递当前已连接数据库

    void sendStuOrMan(int);        //登录判断

protected:
    void paintEvent(QPaintEvent *);         //窗口重绘函数
    void error(QString title, QString messages);     //提示错误函数

private slots:
    void on_login_clicked();        //登录按钮

    void on_out_clicked();          //退出按钮

    void on_register_2_clicked();   //注册按钮

    void setUsername(QString username);     //和注册页面关联的槽函数，注册成功接收注册账号

private:
    Ui::Login *ui;
    StudentMain *stu;       //学生登录主界面
    QSqlDatabase db;
    int style;          //判断是学生或者管理员
};

#endif // LOGIN_H
