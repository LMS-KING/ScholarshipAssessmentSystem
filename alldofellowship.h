#ifndef ALLDOFELLOWSHIP_H
#define ALLDOFELLOWSHIP_H

#include <QDialog>
#include <QStandardItem>
#include "fellowship.h"
#include "classmessage.h"

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class ALLDoFellowship;
}

class ALLDoFellowship : public QDialog
{
    Q_OBJECT

public:
    explicit ALLDoFellowship(QWidget *parent = 0);
    ~ALLDoFellowship();

private:
    void setRecord();

    void ShowRecord(QString sql, QString style);

    void setDelete();

    QString selectDB(QString sql);

    void settableView_2(QString style, QString major);

signals:
    void sendMessage(QSqlDatabase thdb, QString style, QString StartT);

    void sendMakeOrAdd(bool tf);

    void sendToPass();

    void sendPassSure();

protected slots:
    void recvDBTNO(QSqlDatabase thdb, QString tno);

    void recvFellOrClass(bool ForC, bool lf);    //判断是奖学金还是课程

    void timeOut();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::ALLDoFellowship *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;
    QStandardItemModel *ticket_model_2;

    QSqlDatabase db;
    QString teacherNo;

    QString curstyle;
    QString curStime;
    QString style;

    FellowShip *fs;
    ClassMessage *cm;

    bool FellOrClass;
    bool MgrorStu;          //学生或管理员
};

#endif // ALLDOFELLOWSHIP_H
