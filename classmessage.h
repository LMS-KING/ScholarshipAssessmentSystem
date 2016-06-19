#ifndef CLASSMESSAGE_H
#define CLASSMESSAGE_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class ClassMessage;
}

class ClassMessage : public QDialog
{
    Q_OBJECT

public:
    explicit ClassMessage(QWidget *parent = 0);
    ~ClassMessage();

private:
    void setBox();

    QString selectDB(QString sql);

    bool InsertDB(QString sql);

protected slots:
    void recvDB(QSqlDatabase thdb, QString sclaNo, QString smajor);

    void recvchangeOradd(bool tf);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ClassMessage *ui;
    QSqlDatabase db;
    QString classNo;
    QString major;
    bool CorA;
};

#endif // CLASSMESSAGE_H
