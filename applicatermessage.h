#ifndef APPLICATERMESSAGE_H
#define APPLICATERMESSAGE_H

#include <QDialog>
#include <QStandardItem>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class ApplicaterMessage;
}

class ApplicaterMessage : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicaterMessage(QWidget *parent = 0);
    ~ApplicaterMessage();

private:
    void setRecord();       //设置信息记录

    void setCombox();

    void ShowRecord(QString sql, QString style, QString major, QString grade, QString fellowstyle);

    QString SelectDB(QString sql);

    void setPersonMessage(QString stuN, QString grade);        //查看个人提交资料

    int selectScore(QString sql, QString style);

    void setScore(QString sql, QString style, QString grade);       //设置单项成绩

    void setPriority();         //取消资格

    QString checkTime(QString grade);

signals:
    void sendDBStu(QSqlDatabase thdb, QString stuN, QString grade);

    void sendDBPhoto(QSqlDatabase thdb, QString stuN, QString grade);

    void sendSureToPass();

    void sendTeacher();

protected slots:
    void recvDbNum(QSqlDatabase thdb, QString teacherN);

    void recvMeageOrSCo(bool tf, bool all);       //查看信息或成绩

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_4_currentTextChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void timeOut();

    void on_pushButton_4_clicked();

private:
    Ui::ApplicaterMessage *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;
    QSqlDatabase db;
    QString teacherNo;
    bool MgSr;      //判断是查信息还是成绩
    bool AllMg;      //判断是不是查询所有人信息
    QString stuNo;
    QString studentNo;
    QString grades;

    float score1;   //德智体成绩
    float score2;
    float score3;
};

#endif // APPLICATERMESSAGE_H
