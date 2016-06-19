#ifndef FELLOWSHIP_H
#define FELLOWSHIP_H

#include <QDialog>
#include <QMessageBox>
#include <QMutex>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class FellowShip;
}

class FellowShip : public QDialog
{
    Q_OBJECT

public:
    explicit FellowShip(QWidget *parent = 0);
    ~FellowShip();

private:
    QString SelectDB(QString sql);

    bool InsertDB(QString sql, QString style);

    void Error(QString message);

    bool checkTime(QString times);

    bool checkSoftNumber();     //检查软件工程编号

    bool checkNetNumber();      //检查网络工程编号

    bool checkTelNumber();      //检查通信工程编号

    bool checkCompNumber();     //检查计科专业编号

    void clearAll();            //清空所有

    void ReadMessage();         //统计所有专业年级分配信息

    bool DeleteDB();            //删除原有信息

    bool DoInsertALL();         //调用插入函数

    bool InsertALLDB(QString major, QString number, QStringList grade, QStringList count);         //重新插入所有信息

private slots:
    void on_pushButton_clicked();

    void recvDBteachNo(QSqlDatabase thdb, QString style, QString startT);

    //过滤器函数
    bool eventFilter(QObject *wacht, QEvent *event);

    void recvMakeOrAdd(bool tf);

    void on_T1_clicked();

    void on_T2_clicked();

    void on_pushButton_2_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void timeOut();

    void on_soft1_clicked();

    void on_soft2_8_clicked();

    void on_soft2_clicked();

    void on_soft3_clicked();

    void on_soft4_clicked();

    void on_soft1_2_clicked();

    void on_soft2_2_clicked();

    void on_soft3_2_clicked();

    void on_soft4_2_clicked();

    void on_soft1_3_clicked();

    void on_soft2_4_clicked();

    void on_soft3_3_clicked();

    void on_soft4_3_clicked();

    void on_soft1_4_clicked();

    void on_soft2_3_clicked();

    void on_soft3_4_clicked();

    void on_soft4_4_clicked();

    void on_soft1_5_clicked();

    void on_soft2_6_clicked();

    void on_soft3_5_clicked();

    void on_soft4_5_clicked();

    void on_soft1_6_clicked();

    void on_soft2_5_clicked();

    void on_soft3_6_clicked();

    void on_soft4_6_clicked();

    void on_soft1_7_clicked();

    void on_soft3_7_clicked();

    void on_soft4_7_clicked();

    void on_soft1_8_clicked();

    void on_soft2_7_clicked();

    void on_soft3_8_clicked();

    void on_soft4_8_clicked();

private:
    Ui::FellowShip *ui;
    QSqlDatabase db;
    QString SaT;
    QString Sty;
    bool time;
    bool MA;        //判断是增加还是编辑

    QMutex mu;

    QStringList SGrade;      //软件年级
    QStringList SCount;     //软件数量

    QStringList NGrade;      //网络年级
    QStringList NCount;     //网络数量

    QStringList TGrade;      //通信年级
    QStringList TCount;     //通信数量

    QStringList CGrade;      //计科年级
    QStringList CCount;     //计科数量
};

#endif // FELLOWSHIP_H
