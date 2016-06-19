#ifndef ALLSCOANDHISTORY_H
#define ALLSCOANDHISTORY_H

#include <QDialog>
#include <QStandardItem>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class AllScoAndHistory;
}

class AllScoAndHistory : public QDialog
{
    Q_OBJECT

public:
    explicit AllScoAndHistory(QWidget *parent = 0);
    ~AllScoAndHistory();

private:
    void setBox();

    void setRecord();

    QString selectDB(QString sql);

    void ShowRecord(QString sql, QString style, QString major, QString classname);

    void setPriority();

    QString DBStudentNo(QString sql);

    void Message(QString message);

    bool upFellow(QString style, QString stu, QString time, QString major, QString number);        //更新奖学金表

    bool upApplication(QString stuNo, QString style, QString time, QString how, QString major);       //更新申请表

    bool upApplication2(QString stuNo, QString style, QString how, QString time);

    bool upAwards(QString stuNo, QString style, QString time);            //更新奖励表

    bool upAssign2(QString stuNo, QString style, QString time, QString number, QString major); //更新奖学金分配表

signals:
    void sendSureToPass();

    void sendTeacher();

protected slots:
    void recvDBNo(QSqlDatabase thdb, QString teachN);

    void recvScoOHis(bool tf, bool reset);

    void timeOut();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_comboBox_4_currentTextChanged(const QString &arg1);

    void on_pushButton_3_clicked();

private:
    Ui::AllScoAndHistory *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;

    QSqlDatabase db;
    QString teacherNo;
    bool ScoOrHis;       //判断是查询成绩还是历史发放
    bool Reset;          //恢复资格
    QString studentNo;
};

#endif // ALLSCOANDHISTORY_H
