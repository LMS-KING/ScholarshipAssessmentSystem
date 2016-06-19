#ifndef APPLICATION_H
#define APPLICATION_H

#include <QDialog>
#include <QStandardItem>
#include <QTimer>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class Application;
}

class Application : public QDialog
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = 0);
    ~Application();

private:
    void setRecord();

    void Error(QString message);

    void passwSure();       //密码确认

    void sureDeleteApplication();

signals:
    void sendDBtoBtn(QSqlDatabase thisdb, QString stunNo);

    void sendSureToPass();

    void sendStudentPass();

protected slots:
    void recvSDBStuNO(QSqlDatabase thisdb, QString stuNo);

    void timeOut();

    void recvPassw(QString passw);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Application *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;
    QSqlDatabase db;
    QString studentNo;
};

#endif // APPLICATION_H
