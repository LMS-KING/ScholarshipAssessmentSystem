#ifndef DOFELLOW_H
#define DOFELLOW_H

#include <QDialog>
#include <QStandardItem>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class DoFellow;
}

class DoFellow : public QDialog
{
    Q_OBJECT

public:
    explicit DoFellow(QWidget *parent = 0);
    ~DoFellow();

private:
    void Message(QString message);

    void setcomBox();

    void setRecord();

    void showRecord(QString sql, QString major);

    QString selectDB(QString sql);

    bool upFellow(QString style, QString stu, QString time, QString major, QString number);        //更新奖学金表

    bool upApplication(QString stuNo, QString style, QString time, QString how, QString major);       //更新申请表

    bool upApplication2(QString stuNo, QString style, QString how, QString time);

    bool upAwards(QString stuNo, QString style, QString time);            //更新奖励表

    bool upAssign2(QString stuNo, QString style, QString time, QString number, QString major); //更新奖学金分配表

    QString checkTime(QString time, QString stuNo);

protected slots:
    void recvMessage(QSqlDatabase thdb, QString No);

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_3_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DoFellow *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;

    QSqlDatabase db;
    QString TeacNo;
};

#endif // DOFELLOW_H
