#ifndef APPLICATIONFELLOW_H
#define APPLICATIONFELLOW_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

//数据库
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class ApplicationFellow;
}

class ApplicationFellow : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationFellow(QWidget *parent = 0);
    ~ApplicationFellow();

protected:
    void setICM();      //设置界面中学科竞赛信息

    QString MakeDatabase(QString style, QString sql);     //数据查询

    bool insertDatabase(QString style, QString sql, QString Item, QString time, QString tablename, QString expre, QString timec);        //数据插入

    bool insertDatabasetoSport(QString sql, QString Item, QString time,QString expre, QString timec);        //数据插入


    void Error(QString message);

    void setTimeBool(bool &time);

    void setMessage();

protected slots:
    void Database(QSqlDatabase sql, QString studentno);

    void changeTime();

    bool checkTime(QString style, QString selectT); //加分项分一次性和永久性:OTP, FOR

    QString curGrade(QString time);

    void recvStyle(QString style);

private slots:
    void on_save1_clicked();

    void on_BTime1_clicked();

    void on_BTime2_clicked();

    void on_BTime3_clicked();

    void on_BTime4_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_calendarWidget_2_clicked(const QDate &date);

    void on_pushButton_clicked();

    void on_comboBox_13_currentTextChanged(const QString &arg1);

    void on_add_clicked();

    void on_add_2_clicked();

    void on_Bxkjs1_clicked();

    void on_tableTime_clicked(const QDate &date);

    void on_Bxkjs2_clicked();

    void on_Bxkjs3_clicked();

    void on_Bxkjs4_clicked();

    void on_Bxkjs5_clicked();

    void on_Bkjlx1_clicked();

    void on_Bkjlx2_clicked();

    void on_Bkjlx3_clicked();

    void on_save2_clicked();

    void on_add_3_clicked();

    void on_add_4_clicked();

    void on_Bkj_clicked();

    void on_kktimetable_clicked(const QDate &date);

    void on_Bxue_clicked();

    void on_Bclass_clicked();

    void on_stuclasstimetable_clicked(const QDate &date);

    void on_pushButton_3_clicked();

    void on_doubleBt_clicked();

    void on_doubleBt_2_clicked();

    void on_comboBox_18_currentTextChanged(const QString &arg1);

    void on_comboBox_19_currentTextChanged(const QString &arg1);

    void on_doubleBt_3_clicked();

    void on_doubleBt_4_clicked();

    void on_doubleBt_6_clicked();

    void on_doubleBt_5_clicked();

    void on_doubleBt_7_clicked();

    void on_doubleBt_11_clicked();

    void on_doubleBt_12_clicked();

    void on_doubleBt_9_clicked();

    void on_doubleBt_10_clicked();

    void on_doubleBt_8_clicked();

    void on_moretime_clicked(const QDate &date);

    void on_pushButton_2_clicked();

    void on_doubleBt_13_clicked();

    void on_cetbt_clicked();

    void on_cettime_clicked(const QDate &date);

    void on_cetbt2_clicked();

    void on_cettime2_clicked(const QDate &date);

    void on_cetsave_clicked();

    void on_add_5_clicked();

    void on_sub_clicked();

    void on_add1_clicked();

    void on_sub1_clicked();

    void on_Bxkjs1_2_clicked();

    void on_Bxkjs1_3_clicked();

    void on_Bxkjs1_4_clicked();

    void on_Bxkjs1_5_clicked();

    void on_Bxkjs1_6_clicked();

    void on_Bxkjs1_7_clicked();

    void on_Bxkjs1_8_clicked();

    void on_Bxkjs1_9_clicked();

    void on_sporttime_clicked(const QDate &date);

    void on_comboBox_21_currentTextChanged(const QString &arg1);

    void on_comboBox_22_currentTextChanged(const QString &arg1);

    void on_comboBox_23_currentTextChanged(const QString &arg1);

    void on_comboBox_24_currentTextChanged(const QString &arg1);

    void on_comboBox_26_currentTextChanged(const QString &arg1);

    void on_comboBox_27_currentTextChanged(const QString &arg1);

    void on_savesport_clicked();

    void on_comboBox_20_currentTextChanged(const QString &arg1);

    void on_comboBox_25_currentTextChanged(const QString &arg1);

    void on_comboBox_5_currentTextChanged(const QString &arg1);

    void on_comboBox_6_currentTextChanged(const QString &arg1);

    void on_comboBox_7_currentTextChanged(const QString &arg1);

    void on_comboBox_11_currentTextChanged(const QString &arg1);

    void on_comboBox_12_currentTextChanged(const QString &arg1);

    void on_comboBox_8_currentTextChanged(const QString &arg1);

    void on_comboBox_9_currentTextChanged(const QString &arg1);

    void on_comboBox_10_currentTextChanged(const QString &arg1);

    void on_photo_clicked();

    void on_sure_clicked();

    void on_comboBox_14_currentTextChanged(const QString &arg1);

    void on_comboBox_15_currentTextChanged(const QString &arg1);

    void on_comboBox_16_currentTextChanged(const QString &arg1);

private:
    Ui::ApplicationFellow *ui;
    QSqlDatabase db;
    QString studentNo;
    QStringList messl;      //记录学科竞赛等级
    QStringList messk;      //记录科技立项等级
    //表彰时间标志
    bool time12;
    bool time34;
    //学科竞赛时间标志
    bool xktime1;
    bool xktime2;
    bool xktime3;
    bool xktime4;
    bool xktime5;
    //科技立项时间标志
    bool kltime1;
    bool kltime2;
    bool kltime3;
    //证书活动公用时间标记
    int numbers;
    //体育赛事公用时间
    int sporttime;
    //选择证书和活动的个数
    //int books;
    //int activitices;
    //学生会和班级职务时间标志
    bool sclass1;
    bool sclass2;

    bool fellowstyle;       //是否选中奖学金类型

    QString classs;          //当前学生所属年级
};

#endif // APPLICATIONFELLOW_H
