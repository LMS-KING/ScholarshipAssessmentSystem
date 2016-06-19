#include "allscoandhistory.h"
#include "ui_allscoandhistory.h"
#include "passwsure.h"
#include <QTimer>
#include <QMessageBox>

#define WRONG "数据库访问错误!"

QString PAWRE;

AllScoAndHistory::AllScoAndHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllScoAndHistory)
{
    ui->setupUi(this);

    QTimer *time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(timeOut()));
    time->start(100);
}

AllScoAndHistory::~AllScoAndHistory()
{
    delete ui;
}

void AllScoAndHistory::recvDBNo(QSqlDatabase thdb, QString teachN)
{
    db = thdb;
    teacherNo = teachN;
}

void AllScoAndHistory::recvScoOHis(bool tf, bool reset)
{
    ScoOrHis = tf;      //为true是查询成绩
    Reset = reset;

    setBox();
    setRecord();
}

//设置下拉框
void AllScoAndHistory::setBox()
{
    static bool tf = false;     //只设置一次
    if(!tf)
    {
        ui->comboBox->clear();
        ui->comboBox_2->clear();
        ui->comboBox_4->clear();

        QString major = "请选择,软件工程,网络工程,通信工程,计算机科学技术";
        QStringList majors = major.split(",", QString::SkipEmptyParts);
        ui->comboBox->addItems(majors);

        QString sql1 = "select class from GRADE";
        QString grade = "请选择," + selectDB(sql1);
        QStringList grades = grade.split(",", QString::SkipEmptyParts);
        ui->comboBox_2->addItems(grades);

        QString sql2 = "select ClassName from CLASS";
        QSqlQuery query(db);
        query.exec(sql2);
        QString cla = "请选择,";
        while(query.next())
        {
            cla += query.value(0).toString().trimmed() + ",";
        }

        QStringList clas = cla.split(",", QString::SkipEmptyParts);
        ui->comboBox_4->addItems(clas);

        tf = true;
    }

    if(!ScoOrHis)               //查看发放记录
    {
        ui->label_4->show();
        ui->comboBox_3->show();
        ui->comboBox_3->clear();
        ui->label_5->hide();
        ui->comboBox_4->hide();

        ui->pushButton_2->hide();       //恢复资格按钮
        ui->pushButton_3->show();       //取消发放按钮

        QString sql = "select Style from FELLOWSHIP";
        QString style = "请选择," + selectDB(sql);
        QStringList styles = style.split(",", QString::SkipEmptyParts);
        ui->comboBox_3->addItems(styles);
    }
    else
    {
        if(Reset)
        {
            ui->pushButton_2->show();
            ui->label_5->hide();
            ui->comboBox_4->hide();
        }
        else
        {
            ui->pushButton_2->hide();
            ui->label_5->show();
            ui->comboBox_4->show();
        }

        ui->pushButton_3->hide();
        ui->comboBox_3->hide();
        ui->label_4->hide();
    }

    ui->comboBox->setCurrentText("请选择");
    ui->comboBox_2->setCurrentText("请选择");
    ui->comboBox_3->setCurrentText("请选择");
    ui->comboBox_4->setCurrentText("请选择");
}

void AllScoAndHistory::setRecord()
{
    if(ScoOrHis)
    {
        QString sql = NULL;
        if(Reset)               //恢复资格
            sql = tr("select StudentNo, SName, SMajor from STUDENT where SPriority=%1").arg(0);
        else
            sql = "select StudentNo, SName, SMajor from STUDENT";
        ShowRecord(sql, NULL, NULL, NULL);
    }
    else
    {
        QString sql = "select StudentNo, Style, Time from AWARDS where id in(select min(id) from AWARDS group by StudentNo, Style, Time)";
        ShowRecord(sql, "HIS", NULL, NULL);
    }
}

QString AllScoAndHistory::selectDB(QString sql)
{
    QString message = NULL;
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        message += query.value(0).toString().trimmed();
    }
    if(message == NULL)
        message = WRONG;

    return message;
}

//参数 major 可能为专业或者年级
void AllScoAndHistory::ShowRecord(QString sql, QString style, QString major, QString classname)
{
    if(ScoOrHis)
    {
        ticket_model = new QStandardItemModel();
        ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
        ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
        ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
        ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("课程号")));
        ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("课程名")));
        ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("分数")));
        ticket_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("学分")));
        ticket_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("年级")));
    }
    else
    {
        ticket_model = new QStandardItemModel();
        ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
        ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
        ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
        ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("奖学金类型")));
        ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("状态")));
        ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("申请时间")));
        ticket_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("发放时间")));
    }

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);
    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);

    model = ui->tableView->model();
    model->removeRows(0, model->rowCount());

    int i = 0;

    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        if(style.isEmpty())     //查成绩
        {
            QString sql1 = "select ClassNo, Score from SCORE where StudentNo='"+query.value(0).toString().trimmed() + "'";
            QSqlQuery query1(db);
            query1.exec(sql1);
            while(query1.next())
            {
                QString sql2 = NULL;
                if(major.isEmpty())      //年级为任意
                {
                    if(classname.isEmpty())
                        sql2 = tr("select ClassName, ClassScore, Time from CLASS where ClassNo='%1' and SMajor='%2'").arg(query1.value(0).toString().trimmed()).arg(query.value(2).toString().trimmed());
                    else
                        sql2 = tr("select ClassName, ClassScore, TIme from CLASS where ClassNo='%1' and SMajor='%2' and ClassName='%3'").arg(query1.value(0).toString().trimmed()).arg(query.value(2).toString().trimmed()).arg(classname);
                }
                else                //年级选定
                {
                    if(classname.isEmpty())
                        sql2 = tr("select ClassName, ClassScore, Time from CLASS where ClassNo='%1' and SMajor='%2' and Time='%3'").arg(query1.value(0).toString().trimmed()).arg(query.value(2).toString().trimmed()).arg(major);
                    else
                        sql2 = tr("select ClassName, ClassScore, Time from CLASS where ClassNo='%1' and SMajor='%2' and Time='%3' and ClassName='%4'").arg(query1.value(0).toString().trimmed()).arg(query.value(2).toString().trimmed()).arg(major).arg(classname);
                }

                QSqlQuery query2(db);
                query2.exec(sql2);
                while(query2.next())
                {
                    ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
                    ticket_model->setItem(i, 1, new QStandardItem(query.value(1).toString().trimmed()));
                    ticket_model->setItem(i, 2, new QStandardItem(query.value(2).toString().trimmed()));

                    ticket_model->setItem(i, 3, new QStandardItem(query1.value(0).toString().trimmed()));
                    ticket_model->setItem(i, 4, new QStandardItem(query2.value(0).toString().trimmed()));
                    ticket_model->setItem(i, 5, new QStandardItem(query1.value(1).toString().trimmed()));
                    ticket_model->setItem(i, 6, new QStandardItem(query2.value(1).toString().trimmed()));
                    ticket_model->setItem(i, 7, new QStandardItem(query2.value(2).toString().trimmed()));

                    if(query1.value(1).toInt() <= 60)
                        ticket_model->item(i, 5)->setForeground(QBrush(QColor(255,0,0)));
                    else if(query1.value(1).toInt() >= 90)
                        ticket_model->item(i, 5)->setForeground(QBrush(QColor(0,255,0)));

                    //设置单元格文本居中
                    ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 4)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 5)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 6)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 7)->setTextAlignment(Qt::AlignCenter);
                    i++;
                }
            }
        }
        else if(style == "HIS")     //查询历史发放
        {
            QString sql1 = NULL;
            if(major.isEmpty())     //专业不限
                sql1 = "select SName, SMajor from STUDENT where StudentNo='"+ query.value(0).toString().trimmed() + "'";
            else                //专业限定
                sql1 = tr("select SName, SMajor from STUDENT where StudentNo='%1' and SMajor='%2'").arg(query.value(0).toString().trimmed()).arg(major);

            QSqlQuery query1(db);
            query1.exec(sql1);
            while(query1.next())
            {
                QString sql2 = tr("select Status,Time from APPLICATION where StudentNo='%1' and (Status='%2' or Status='%3') and Style='%4'").arg(query.value(0).toString().trimmed()).arg("已批准").arg("未批准").arg(query.value(1).toString().trimmed());

                QSqlQuery query2(db);
                query2.exec(sql2);
                while(query2.next())
                {
                    ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
                    ticket_model->setItem(i, 1, new QStandardItem(query1.value(0).toString().trimmed()));
                    ticket_model->setItem(i, 2, new QStandardItem(query1.value(1).toString().trimmed()));

                    ticket_model->setItem(i, 3, new QStandardItem(query.value(1).toString().trimmed()));
                    ticket_model->setItem(i, 4, new QStandardItem(query2.value(0).toString().trimmed()));
                    ticket_model->setItem(i, 5, new QStandardItem(query2.value(1).toString().trimmed()));
                    ticket_model->setItem(i, 6, new QStandardItem(query.value(2).toString().trimmed()));

                    if(query2.value(0).toString() == "未批准")
                        ticket_model->item(i, 4)->setForeground(QBrush(QColor(255, 0, 0)));
                    else if(query2.value(0).toString() == "已批准")
                    {
                        //设置状态单元格为green
                        ticket_model->item(i, 4)->setForeground(QBrush(QColor(0,255,0)));
                    }

                    //设置单元格文本居中
                    ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 4)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 5)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 6)->setTextAlignment(Qt::AlignCenter);

                    i++;
                }
            }
        }
    }

    if(i == 0)
    {
        ui->label_3->show();
    }
    else
        ui->label_3->hide();
}

//专业改变
void AllScoAndHistory::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(!ScoOrHis)       //查询往届发放情况，com2:年级，com3:类型
    {
        if(arg1 != "请选择")
        {
            QString sql1 = NULL;
            if(ui->comboBox_2->currentText() == "请选择" && ui->comboBox_3->currentText() == "请选择")
            {
                sql1 = "select StudentNo, Style, Time from AWARDS where id in(select min(id) from AWARDS group by StudentNo, Style, Time)";
            }
            else if(ui->comboBox_2->currentText() == "请选择" && ui->comboBox_3->currentText() != "请选择")
            {
                sql1 = "select StudentNo, Style, Time from AWARDS where Style='" + ui->comboBox_3->currentText() + "' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)";
            }
            else if(ui->comboBox_2->currentText() != "请选择" && ui->comboBox_3->currentText() == "请选择")
            {
                sql1 = "select StudentNo, Style, Time from AWARDS where Time='" + ui->comboBox_2->currentText().left(2) + "' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)";
            }
            else if(ui->comboBox_2->currentText() != "请选择" && ui->comboBox_3->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Time from AWARDS where Style= '%1' and Time='%2' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(ui->comboBox_3->currentText()).arg(ui->comboBox_2->currentText().left(2));
            }

            ShowRecord(sql1, "HIS", arg1, NULL);
        }
    }
    else
    {
        if(arg1 != "请选择")
        {
            QString claname = ui->comboBox_4->currentText().trimmed();

            ui->comboBox_4->clear();
            QString sql2 = "select ClassName from CLASS where SMajor='" + arg1 + "'";
            QSqlQuery query(db);
            query.exec(sql2);
            QString cla = "请选择,";
            while(query.next())
            {
                cla += query.value(0).toString().trimmed() + ",";
            }

            QStringList clas = cla.split(",", QString::SkipEmptyParts);
            ui->comboBox_4->addItems(clas);

            QString sql1 = NULL;
            if(Reset)
                sql1 = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1' and SPriority=%2").arg(arg1).arg(0);
            else
            {
                sql1 = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1'").arg(arg1);

            }
            if(ui->comboBox_2->currentText() != "请选择")
                ShowRecord(sql1, NULL, ui->comboBox_2->currentText(), NULL);
            else
                ShowRecord(sql1, NULL,NULL, NULL);
        }
    }
}

//类型改变，查询历史发放情况时
void AllScoAndHistory::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql1 = NULL;
        if(ui->comboBox->currentText() == "请选择" && ui->comboBox_2->currentText() == "请选择")
        {
            sql1 = "select StudentNo, Style, Time from AWARDS where Style='" + arg1 + "' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)";
        }
        else if(ui->comboBox->currentText() == "请选择" && ui->comboBox_2->currentText() != "请选择")
        {
            sql1 = tr("select StudentNo, Style, Time from AWARDS where Time='%1' and Style='%2' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(ui->comboBox_2->currentText()).arg(arg1);
        }
        else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_2->currentText() == "请选择")
        {
            sql1 = "select StudentNo, Style, Time from AWARDS where Style='" + arg1 + "' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)";
            ShowRecord(sql1, "HIS", ui->comboBox->currentText(), NULL);
            return;
        }
        else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_2->currentText() != "请选择")
        {
            sql1 = tr("select StudentNo, Style, Time from AWARDS where Style='%1' and Time='%2' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(arg1).arg(ui->comboBox_2->currentText());
            ShowRecord(sql1, "HIS", ui->comboBox->currentText(), NULL);
            return;
        }

        ShowRecord(sql1, "HIS", NULL, NULL);
    }
}

//年级改变
void AllScoAndHistory::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(!ScoOrHis)       //查询发放历史
    {
        if(arg1 != "请选择")
        {
            QString sql1 = NULL;
            if(ui->comboBox->currentText() == "请选择" && ui->comboBox_3->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo, Style, Time from AWARDS where Time like '%1%' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(arg1.left(2));
            }
            else if(ui->comboBox->currentText() == "请选择" && ui->comboBox_3->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Time from AWARDS where Time like '%1%' and Style='%2' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(arg1.left(2)).arg(ui->comboBox_3->currentText());
            }
            else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_3->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo, Style, Time from AWARDS where Time like '%1%' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(arg1.left(2));
                ShowRecord(sql1, "HIS", ui->comboBox->currentText(), NULL);
                return;
            }
            else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_3->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Time from AWARDS where Style='%1' and Time like '%2%' and id in(select min(id) from AWARDS group by StudentNo, Style, Time)").arg(ui->comboBox_3->currentText()).arg(arg1.left(2));
                ShowRecord(sql1, "HIS", ui->comboBox->currentText(), NULL);
                return;
            }

            ShowRecord(sql1, "HIS", NULL, NULL);
        }
    }
    else            //查询成绩
    {
        if(arg1 != "请选择")
        {
            QString sql = NULL;
            if(ui->comboBox->currentText() != "请选择" && ui->comboBox_4->currentText().trimmed() != "请选择")
            {
                if(Reset)
                    sql = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1' and SPriority=%2").arg(ui->comboBox->currentText().trimmed()).arg(0);
                else
                    sql = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1'").arg(ui->comboBox->currentText().trimmed());

                ShowRecord(sql, NULL, arg1, ui->comboBox_4->currentText().trimmed());
                return;
            }
            else if(ui->comboBox->currentText() == "请选择" && ui->comboBox_4->currentText().trimmed() != "请选择")
            {
                if(Reset)
                    sql = "select StudentNo, SName, SMajor from STUDENT where SPriority=" + QString::number(0);
                else
                    sql = "select StudentNo, SName, SMajor from STUDENT";

                ShowRecord(sql, NULL, arg1, ui->comboBox_4->currentText().trimmed());
                return;
            }
            else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_4->currentText().trimmed() == "请选择")
            {
                if(Reset)
                    sql = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1' and SPriority=%2").arg(ui->comboBox->currentText().trimmed()).arg(0);
                else
                   sql = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1'").arg(ui->comboBox->currentText().trimmed());
            }
            else if(ui->comboBox->currentText() == "请选择" && ui->comboBox_4->currentText().trimmed() == "请选择")
            {
                if(Reset)
                    sql = "select StudentNo, SName, SMajor from STUDENT where SPriority=" + QString::number(0);
                else
                    sql = "select StudentNo, SName, SMajor from STUDENT";
            }
            ShowRecord(sql, NULL, arg1, NULL);
        }
    }
}

//课程名改变
void AllScoAndHistory::on_comboBox_4_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql = NULL;
        if(ui->comboBox->currentText().trimmed() == "请选择" && ui->comboBox_2->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, SName, SMajor from STUDENT");
        }
        else if(ui->comboBox->currentText().trimmed() != "请选择" && ui->comboBox_2->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, SName, SMajor from STUDENT where SMajor='%1'").arg(ui->comboBox->currentText().trimmed());

        }
        else if(ui->comboBox->currentText().trimmed() == "请选择" && ui->comboBox_2->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, SName, SMajor from STUDENT");

            ShowRecord(sql, NULL, ui->comboBox_2->currentText().trimmed(), arg1);
            return;
        }
        else if(ui->comboBox->currentText().trimmed() != "请选择" && ui->comboBox_2->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, SName, SMajor from STUDENT where  SMajor='%1'").arg(ui->comboBox->currentText().trimmed());

            ShowRecord(sql, NULL, ui->comboBox_2->currentText().trimmed(), arg1);
            return;
        }

        ShowRecord(sql, NULL, NULL, arg1);
    }
}

//查询
QString AllScoAndHistory::DBStudentNo(QString sql)
{
    QString clano = NULL;     //课程号
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        clano += query.value(0).toString().trimmed() + ",";
    }
    clano = clano.left(clano.length() - 1);

    QString sql2 = tr("select StudentNo from SCORE where ClassNo in (%1)").arg(clano);
    query.exec(sql2);
    QString stuno = NULL;
    while(query.next())
    {
        stuno += query.value(0).toString().trimmed() + ",";
    }

    stuno = stuno.left(stuno.length() - 1);

    return stuno;
}

void AllScoAndHistory::Message(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

//更新奖学金表
bool AllScoAndHistory::upFellow(QString style, QString stu, QString time, QString major, QString number)
{
    QString sql = tr("select Count from FELLOWSHIP where Style='%1,'").arg(style);
    QString count = selectDB(sql);
    if(count == WRONG)
    {
        Message(WRONG);
        return false;
    }

    if(count.toInt() == 0)
    {
        if(number != "0000")
        {
            if(!upAssign2(stu, style, time, number, major))
            {
                return false;
            }

            //更新申请记录状态,调用upApplication，更新一个之外的所有其他为未批准
            if(!upApplication(stu, style, time, "ALL", NULL))
            {
                Message("操作失败!");
                return false;
            }
        }
        else
        {
            //更新申请记录状态,调用upApplication，更新一个之外的所有其他为未批准
            if(!upApplication2(stu, style, "ALL", time))
            {
                Message("操作失败!");
                return false;
            }
        }
    }
    else
    {
        if(number != "0000")
        {
            if(!upAssign2(stu, style, time, number, major))
            {
                return false;
            }

            //调用upApplication，更新一个为已批准
            if(!upApplication(stu, style, time, "ONE", NULL))
            {
                return false;
            }
        }
        else
        {
            if(!upApplication2(stu, style, "ONE", time))
            {
                Message("操作失败!");
                return false;
            }
        }
    }

    QString sql1 = tr("update FELLOWSHIP set Count=Count+1 where Style='%1,'").arg(style);
    QString up = selectDB(sql1);
    if(up != WRONG)
    {
        Message(WRONG);
        return false;
    }

    return true;
}

//更新申请表
bool AllScoAndHistory::upApplication(QString stuNo, QString style, QString time, QString how, QString major)
{
    //调用upAwards，插入获奖情况
    if(how == "ALL")        //更新除此之外的其他状态为未批准
    {
        QString sql2 = "select StudentNo from STUDENT where SMajor='" + major + "'";
        QString students;
        QSqlQuery query(db);
        query.exec(sql2);
        while(query.next())
        {
            students += "'" + query.value(0).toString().trimmed() + "',";
        }
        students = students.left(students.length() - 1);
        students.remove(stuNo);

        QString sql = tr("update APPLICATION set Status='%1' where StudentNo in(%2) and Style='%3' and Time='%4' and Status='%5'").arg("待审核").arg(students).arg(style).arg(time).arg("未批准");

        QString up = selectDB(sql);
        if(up != WRONG)
            return false;
    }

    QString sql1 = tr("update APPLICATION set Status='%1' where StudentNo='%2' and Style='%3' and Time='%4'").arg("未批准").arg(stuNo).arg(style).arg(time);
    QString up = selectDB(sql1);
    if(up == WRONG)
    {
        if(!upAwards(stuNo, style, time))
            return false;
    }
    else
    {
        return false;
    }

    return true;
}

bool AllScoAndHistory::upApplication2(QString stuNo, QString style, QString how, QString time)
{
    if(how == "ALL")        //更新除此之外的其他状态为未批准
    {
        QString sql = tr("update APPLICATION set Status='%1' where Status='%2' and Style='%3' and StudentNo not in('%4')").arg("待审核").arg("未批准").arg(style).arg(stuNo);
        QString up = selectDB(sql);
        if(up != WRONG)
        {
            return false;
        }
    }

    QString sql1 = tr("update APPLICATION set Status='%1' where StudentNo='%2' and Style='%3' and Status='%4'").arg("未批准").arg(stuNo).arg(style).arg("已批准");
    QString up = selectDB(sql1);
    if(up == WRONG)
    {
        if(!upAwards(stuNo, style, time))
            return true;
    }
    else
    {
        return false;
    }
}

bool AllScoAndHistory::upAwards(QString stuNo, QString style, QString time)
{
    QString sql = tr("delete from AWARDS where StudentNo='%1' and Style='%2' and Time='%3'").arg(stuNo).arg(style).arg(time);
    QString del = selectDB(sql);

    if(del != WRONG)
        return false;

    return true;
}

bool AllScoAndHistory::upAssign2(QString stuNo, QString style, QString time, QString number, QString major)
{
    QString sql = tr("select NowCount from ASSIGN2 where Number='%1' and Grade='%2'").arg(number).arg(time);

    QString nowcount = selectDB(sql);

    if(nowcount.toInt() == 0)
    {
        //调用upApplication，更新一个为已批准
        if(!upApplication(stuNo, style, time, "ALL", major))
        {
            Message("操作失败!");
            return false;
        }
    }
    else
    {
        //调用upApplication，更新一个为已批准
        if(!upApplication(stuNo, style, time, "ONE", major))
        {
            Message("操作失败!");
            return false;
        }
    }

    QString sql1 = tr("update ASSIGN2 set NowCount=NowCount+1 where Number='%1' and Grade='%2'").arg(number).arg(time);
    QString up = selectDB(sql1);
    if(up != WRONG)
    {
        Message(WRONG);
        return false;
    }

    return true;
}

void AllScoAndHistory::on_pushButton_2_clicked()        //恢复资格
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //获得选中行的学号

    if(recd.isEmpty())
    {
        Message("请选择学生");
        return;
    }

    studentNo = recd;

    PasswSure *paw = new PasswSure(this);
    connect(this, SIGNAL(sendSureToPass()), paw, SLOT(closes()));
    connect(this, SIGNAL(sendTeacher()), paw, SLOT(recvTeachRe()));

    paw->setWindowTitle("密码确认");
    paw->setModal(true);
    paw->show();
}

void AllScoAndHistory::timeOut()
{
    if(!PAWRE.isEmpty())
    {
        QString sql = "select TPassword from MANAGER where TeacherNo='" + teacherNo + "'";
        QString passw = selectDB(sql);
        if(PAWRE.compare(passw) == 0)
        {
            setPriority();
        }
        else
            Message("密码错误");
        PAWRE.clear();
        return;
    }
    else
        emit sendTeacher();
}

void AllScoAndHistory::setPriority()
{
    QString sql = tr("update STUDENT set SPriority=%1 where StudentNo='%2'").arg(1).arg(studentNo);
    QString wrong = selectDB(sql);
    if(wrong != WRONG)
    {
        Message("恢复失败!");
        return;
    }
    else
    {
        //更新申请表
        QString sql1 = tr("update APPLICATION set Status='%1' where StudentNo='%2' and Status='%3'").arg("待审核").arg(studentNo).arg("资格已取消");

        QString up = selectDB(sql1);
        if(up != WRONG)
        {
            Message("恢复失败!");
            return;
        }

        Message("恢复成功!");
        emit sendSureToPass();

        setRecord();        //刷新信息
    }
}

void AllScoAndHistory::on_pushButton_clicked()      //返回
{
    ui->pushButton_2->show();

    this->close();
}

void AllScoAndHistory::on_pushButton_3_clicked()        //取消发放
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //获得选中学号

    int curow1 = ui->tableView->currentIndex().row();
    QModelIndex index1 = model->index(curow1,2);
    QVariant data1 = model->data(index1);
    QString recd1 = data1.toString().trimmed();   //获得选中学号专业

    int curow2 = ui->tableView->currentIndex().row();
    QModelIndex index2 = model->index(curow2,3);
    QVariant data2 = model->data(index2);
    QString recd2 = data2.toString().trimmed();   //获得选中类型

    int curow3 = ui->tableView->currentIndex().row();
    QModelIndex index3 = model->index(curow3,5);
    QVariant data3 = model->data(index3);
    QString recd3 = data3.toString().trimmed();   //获得选中申请时间

    int curow4 = ui->tableView->currentIndex().row();
    QModelIndex index4 = model->index(curow4,4);
    QVariant data4 = model->data(index4);
    QString recd4 = data4.toString().trimmed();   //获得选中状态

    if(recd.isEmpty())
    {
        Message("请选择学生!");
        return;
    }

    if(recd4 == "未批准")
    {
        Message("未批准状态不可操作!");
        return;
    }

    QString sql1 = "select Number from ASSIGN where Style='" + recd2 + "'";
    QString number = selectDB(sql1);
    if(number == "0000")
    {
        if(upFellow(recd2, recd, recd3, recd1,number))
        {
            Message("操作成功!");
            setRecord();
            return;
        }
    }
    else
    {
        //获得对应专业的编号
        QString sql2 = tr("select Number from ASSIGN where Style='%1' and SMajor='%2'").arg(recd2).arg(recd1);
        QString number = selectDB(sql2);
        if(upFellow(recd2, recd, recd3, recd1, number))
        {
            Message("操作成功!");
            setRecord();
            return;
        }
    }
}
