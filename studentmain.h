#ifndef STUDENTMAIN_H
#define STUDENTMAIN_H

#include <QMainWindow>
#include <QDebug>
#include <QEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QTextCodec>
#include <QFont>
#include "applicationfellow.h"
#include "application.h"
#include "personmessage.h"
#include "applicatermessage.h"
#include "allscoandhistory.h"
#include "alldofellowship.h"
#include "managermessage.h"
#include "addordelmanager.h"
#include "dofellow.h"

namespace Ui {
class StudentMain;
}

class StudentMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentMain(QWidget *parent = 0);
    ~StudentMain();

protected:
    QString Database(QString style, QString sql);   //style表示操作类型，
                                                    //分：奖学金信息查询:FM
                                                    //申请奖学金:PF
                                                    //查看申请记录:SF

    void Error(QString message);        //错误提示

    void closeEvent(QCloseEvent *event);

    void setBar(int man);      //设置目录

    QString checkTime(QString time);

signals:
    void sendMessage(QSqlDatabase sql, QString student);

    void sendStyle(QString sytle);

    void sendManager(bool tf);

    void sendMgOrSr(bool ms, bool all);

    void sendScoOrHis(bool sh, bool re );     //查询成绩或发放情况

    void sendFellOrClass(bool tf, bool lf);          //编辑奖学金或者课程

    void sendMaorSuper(bool tf);                   //个人中心，超级管理员增加管理员

protected slots:
    void changePicture();

    void setWelcome(QString username, QString number);

    void setMessage(QSqlDatabase db);

    void checkUser(int style);

    void lookMessageForApplication();       //查看申请人信息

    void lookScoreForApplication();         //查看申请人成绩

    void lookAllScore();            //查看所有同学智育成绩

    void lookHistory();                 //查看往届发放情况

    void loolAllMessage();              //查询所有学生信息

    void ResetPriority();               //恢复某人资格

    void Fellow();              //编辑奖学金信息

    void Class();               //编辑课程信息

    void ManagerM();            //管理员信息

    void AddorDEL();            //增删管理员

    void Dofew();          //奖学金发放

private slots:
    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_5_triggered();

    void on_action_19_triggered();

    void on_action_3_triggered();

    void on_action_17_triggered();

    void on_action_14_triggered();

    void on_action_4_triggered();

private:
    Ui::StudentMain *ui;
    QSqlDatabase thisdb;
    QString StudentNo;      //当前登录学生学号
    ApplicationFellow *app;
    QStringList styles;      //奖学金类型
    Application *record;    // 申请记录
    PersonMessage *per;     //个人信息
    ManagerMessage *mm;     //管理员个人中心
    bool scoOrM;    //分数，管理员
    ApplicaterMessage *apper;        //查看申请人信息
    AllScoAndHistory  *aah;           //看成绩或发放记录
    ALLDoFellowship *ado;           //编辑奖学金信息
    ADDorDELManager *aord;          //增删，特权
    DoFellow *df;                   //发放奖学金

    QFont font;
};

#endif // STUDENTMAIN_H
