#ifndef MANAGERMESSAGE_H
#define MANAGERMESSAGE_H

#include <QDialog>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class ManagerMessage;
}

class ManagerMessage : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerMessage(QWidget *parent = 0);
    ~ManagerMessage();

private:
    void setMessage();

    QString selectDB(QString sql);

    bool InsertDB(QString sql, QString style);

    void Error(QString message);

protected slots:
    bool eventFilter(QObject * watched,QEvent *event);       //事件过滤器

    void recvDBTNO(QSqlDatabase thdb, QString tno);

    void recvMaorSuper(bool ms);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ManagerMessage *ui;
    QSqlDatabase db;
    QString teacherNo;

    bool super;
    bool MS;           //标记是个人中心还是增加管理员
    QString mm;     //  管理员密码
};

#endif // MANAGERMESSAGE_H
