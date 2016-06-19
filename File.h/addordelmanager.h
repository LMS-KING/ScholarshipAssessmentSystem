#ifndef ADDORDELMANAGER_H
#define ADDORDELMANAGER_H

#include <QDialog>
#include <QStandardItem>
#include "managermessage.h"

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class ADDorDELManager;
}

class ADDorDELManager : public QDialog
{
    Q_OBJECT

public:
    explicit ADDorDELManager(QWidget *parent = 0);
    ~ADDorDELManager();

private:
    void setRecord();

    void ShowRecord(QString sql);

    void checkPass();

    QString selectDB(QString sql);

signals:
    void sendDBTNO(QSqlDatabase thdb, QString tno);

    void sendMaorSuper(bool tf);

    void sendToPass();

    void sendPassSure();

protected slots:
    void recvDBTNO(QSqlDatabase thdb, QString tno);

    void timeOut();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ADDorDELManager *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;
    QSqlDatabase db;
    QString teacherNo;
    QString stno;

    ManagerMessage *mm;     //管理员个人中心
};

#endif // ADDORDELMANAGER_H
