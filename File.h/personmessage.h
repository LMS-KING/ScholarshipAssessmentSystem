#ifndef PERSONMESSAGE_H
#define PERSONMESSAGE_H

#include <QDialog>
//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class PersonMessage;
}

class PersonMessage : public QDialog
{
    Q_OBJECT

public:
    explicit PersonMessage(QWidget *parent = 0);
    ~PersonMessage();

private:
    void setMessage();

    void Error(QString message);

    QString selectDB(QString sql);

private slots:
    bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

    void on_pushButton_clicked();

    void recvDbStuno(QSqlDatabase thisdb, QString stu);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PersonMessage *ui;
    QSqlDatabase db;
    QString studentNo;
};

#endif // PERSONMESSAGE_H
