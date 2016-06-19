#include "fellowship.h"
#include "ui_fellowship.h"
#include <QDate>
#include <QDebug>
#include <QTimer>
#include <QRegExp>
#include <QThread>
#define WRONG "数据库访问出错!"

extern bool ADD;

FellowShip::FellowShip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FellowShip)
{
    ui->setupUi(this);

    time = false;
    ui->groupBox->hide();
    ui->calendarWidget->hide();
    ui->pushButton->setFocus();

    //事件过滤器
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);
    ui->lineEdit_4->installEventFilter(this);
    ui->year1->installEventFilter(this);
    ui->year2->installEventFilter(this);
    ui->year3->installEventFilter(this);
    ui->year4->installEventFilter(this);
    ui->year1_2->installEventFilter(this);
    ui->year2_2->installEventFilter(this);
    ui->year3_2->installEventFilter(this);
    ui->year4_2->installEventFilter(this);

    //网络工程
    ui->year1_4->installEventFilter(this);
    ui->year2_3->installEventFilter(this);
    ui->year3_3->installEventFilter(this);
    ui->year4_4->installEventFilter(this);
    ui->year1_3->installEventFilter(this);
    ui->year2_4->installEventFilter(this);
    ui->year3_4->installEventFilter(this);
    ui->year4_3->installEventFilter(this);

    //通信工程
    ui->year1_6->installEventFilter(this);
    ui->year2_5->installEventFilter(this);
    ui->year3_5->installEventFilter(this);
    ui->year4_6->installEventFilter(this);
    ui->year1_5->installEventFilter(this);
    ui->year2_6->installEventFilter(this);
    ui->year3_6->installEventFilter(this);
    ui->year4_5->installEventFilter(this);

    //计算机
    ui->year1_8->installEventFilter(this);
    ui->year2_7->installEventFilter(this);
    ui->year3_7->installEventFilter(this);
    ui->year4_8->installEventFilter(this);
    ui->year1_7->installEventFilter(this);
    ui->year2_8->installEventFilter(this);
    ui->year3_8->installEventFilter(this);
    ui->year4_7->installEventFilter(this);

    QTimer *times = new QTimer(this);
    connect(times, SIGNAL(timeout()), this, SLOT(timeOut()));
    times->start(1000);

    SGrade.clear();
    SCount.clear();
    NGrade.clear();
    NCount.clear();
    TGrade.clear();
    TCount.clear();
    CGrade.clear();
    CCount.clear();

    //开始时各年级名额隐藏
    //软件工程
    ui->year1->hide();
    ui->year2->hide();
    ui->year3->hide();
    ui->year4->hide();
    ui->year1_2->hide();
    ui->year2_2->hide();
    ui->year3_2->hide();
    ui->year4_2->hide();

    //网络工程
    ui->year1_4->hide();
    ui->year2_3->hide();
    ui->year3_3->hide();
    ui->year4_4->hide();
    ui->year1_3->hide();
    ui->year2_4->hide();
    ui->year3_4->hide();
    ui->year4_3->hide();

    //通信工程
    ui->year1_6->hide();
    ui->year2_5->hide();
    ui->year3_5->hide();
    ui->year4_6->hide();
    ui->year1_5->hide();
    ui->year2_6->hide();
    ui->year3_6->hide();
    ui->year4_5->hide();

    //计算机
    ui->year1_8->hide();
    ui->year2_7->hide();
    ui->year3_7->hide();
    ui->year4_8->hide();
    ui->year1_7->hide();
    ui->year2_8->hide();
    ui->year3_8->hide();
    ui->year4_7->hide();
}

FellowShip::~FellowShip()
{
    delete ui;
}

void FellowShip::timeOut()
{
    if(ui->yes->isChecked())
    {
        if(ui->calendarWidget->isHidden())
        {
            ui->pushButton->move(151, 553);
            ui->pushButton_2->move(286, 553);
            this->resize(535, 595);
            ui->groupBox->show();
        }
    }
    else
    {
        clearAll();
        if(ui->calendarWidget->isHidden())
        {
            ui->pushButton->move(151, 205);
            ui->pushButton_2->move(286,205);
            this->resize(535, 250);
            ui->groupBox->hide();
        }
    }
}

void FellowShip::recvDBteachNo(QSqlDatabase thdb, QString style, QString startT)
{
    db = thdb;
    Sty = style;
    SaT = startT;
}

//过滤器函数
bool FellowShip::eventFilter(QObject *wacht, QEvent *event)
{
    if(wacht == ui->lineEdit)
    {
        if(event->type() == QEvent::FocusOut)
        {
            QString number = ui->lineEdit->text().trimmed();
            QString sql = "select id from ASSIGN where Number='" + number + "'";
            QString re = SelectDB(sql);
            if(re != WRONG)
            {
                Error("该编号已经存在，请重新编号!");
                ui->lineEdit->clear();
                ui->pushButton->setEnabled(false);
                return false;
            }
        }
    }
    else if(wacht == ui->lineEdit_2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            QString number = ui->lineEdit_2->text().trimmed();
            QString sql = "select id from ASSIGN where Number='" + number + "'";
            QString re = SelectDB(sql);
            if(re != WRONG)
            {
                Error("该编号已经存在，请重新编号!");
                ui->lineEdit_2->clear();
                ui->pushButton->setEnabled(false);
                return false;
            }
        }
    }
    else if(wacht == ui->lineEdit_3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            QString number = ui->lineEdit_3->text().trimmed();
            QString sql = "select id from ASSIGN where Number='" + number + "'";
            QString re = SelectDB(sql);
            if(re != WRONG)
            {
                Error("该编号已经存在，请重新编号!");
                ui->lineEdit_3->clear();
                ui->pushButton->setEnabled(false);
                return false;
            }
        }
    }
    else if(wacht == ui->lineEdit_4)
    {
        if(event->type() == QEvent::FocusOut)
        {
            QString number = ui->lineEdit_4->text().trimmed();
            QString sql = "select id from ASSIGN where Number='" + number + "'";
            QString re = SelectDB(sql);
            if(re != WRONG)
            {
                Error("该编号已经存在，请重新编号!");
                ui->lineEdit_4->clear();
                ui->pushButton->setEnabled(false);
                return false;
            }
        }
    }
    else if(wacht == ui->year1)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1->clear();
                ui->year1->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2->clear();
                ui->year2->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3->clear();
                ui->year3->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4->clear();
                ui->year4->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_2->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_2->clear();
                ui->year1_2->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2_2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_2->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_2->clear();
                ui->year2_2->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3_2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_2->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_2->clear();
                ui->year3_2->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_2->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_2->clear();
                ui->year4_2->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_4)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_4->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_4->clear();
                ui->year1_4->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2_3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_3->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_3->clear();
                ui->year2_3->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3_3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_3->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_3->clear();
                ui->year3_3->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_4)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_4->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_4->clear();
                ui->year4_4->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_3->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_3->clear();
                ui->year1_3->setFocus();
                return false;
            }
        }
     }
    else if(wacht == ui->year2_4)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_4->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_4->clear();
                ui->year2_4->setFocus();
                return false;
            }
        }
     }
    else if(wacht == ui->year3_4)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_4->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_4->clear();
                ui->year3_4->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_3->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_3->clear();
                ui->year4_3->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_6)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_6->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_6->clear();
                ui->year1_6->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2_5)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_5->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_5->clear();
                ui->year2_5->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3_5)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_5->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_5->clear();
                ui->year3_5->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_6)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_6->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_6->clear();
                ui->year4_6->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_5)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_5->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_5->clear();
                ui->year1_5->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2_6)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_6->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_6->clear();
                ui->year2_6->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3_6)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_6->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_6->clear();
                ui->year3_6->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_5)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_5->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_5->clear();
                ui->year4_5->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_8)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_8->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_8->clear();
                ui->year1_8->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2_7)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_7->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_7->clear();
                ui->year2_7->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3_7)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_7->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_7->clear();
                ui->year3_7->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_8)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_8->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_8->clear();
                ui->year4_8->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year1_7)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year1_7->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year1_7->clear();
                ui->year1_7->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year2_8)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year2_8->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year2_8->clear();
                ui->year2_8->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year3_8)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year3_8->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year3_8->clear();
                ui->year3_8->setFocus();
                return false;
            }
        }
    }
    else if(wacht == ui->year4_7)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->year4_7->text().toInt() <= 0)
            {
                Error("奖励名额必须为整数!");
                ui->year4_7->clear();
                ui->year4_7->setFocus();
                return false;
            }
        }
    }

    ui->pushButton->setEnabled(true);
    return QWidget::eventFilter(wacht, event);
}

void FellowShip::recvMakeOrAdd(bool tf)
{
    MA = tf;

    if(MA)      //编辑
    {
        ui->style->setReadOnly(true);       //不可编辑奖学金类型

        QString sql = tr("select Style, Money, StartTime, EndTime, Count from FELLOWSHIP where Style='%1' and StartTime='%2'").arg(Sty + ",").arg(SaT);
        QSqlQuery query(db);
        query.exec(sql);

        if(query.next())
        {
            ui->style->setText(query.value(0).toString().left(query.value(0).toString().length() - 1));
            ui->money->setText(query.value(1).toString());
            ui->Lt1->setText(query.value(2).toString());
            ui->Lt2->setText(query.value(3).toString());
            ui->count->setText(query.value(4).toString());
        }
    }
    else
    {
        ui->style->clear();
        ui->count->clear();
        ui->money->clear();
        ui->Lt1->clear();
        ui->Lt2->clear();
        ui->style->setReadOnly(false);
    }
}

QString FellowShip::SelectDB(QString sql)
{
    QString message = NULL;

    QSqlQuery query(db);
    query.exec(sql);

    if(query.next())
    {
        message = query.value(0).toString().trimmed();
    }
    else
        message = WRONG;

    return message;
}

bool FellowShip::InsertDB(QString sql, QString style)
{
    QSqlQuery query(db);
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare(sql);
    if(style == "NO")
    {
        query.addBindValue(tr("%1").arg(ui->style->text() + ","));
        query.addBindValue(tr("%1").arg(ui->money->text().trimmed()));
        query.addBindValue(tr("%1").arg(ui->Lt1->text().trimmed()));
        query.addBindValue(tr("%1").arg(ui->Lt2->text().trimmed()));
        query.addBindValue(tr("%1").arg(ui->count->text().trimmed()));
    }
    else if(style == "YES")
    {
        query.addBindValue(tr("%1").arg(ui->style->text().trimmed()));
        query.addBindValue(tr("%1").arg("0000"));
    }
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    return true;
}

void FellowShip::Error(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

void FellowShip::on_pushButton_clicked()        //确定
{
    if(!ui->yes->isChecked())       //不按照专业年级划分
    {
        if(ui->style->text().isEmpty() || ui->count->text().isEmpty() || ui->money->text().isEmpty() || ui->Lt1->text().isEmpty() || ui->Lt2->text().isEmpty())
        {
            Error("所有项目为必填项目!");
            return;
        }

        if(ui->count->text().toInt() <= 0)
        {
            Error("奖励名额输入错误!");
            return;
        }

        if(ui->money->text().toInt() <= 0)
        {
            Error("奖励金额输入错误!");
            return;
        }

        if(MA)      //编辑，更新
        {
            QString sql = tr("update FELLOWSHIP set Money=%1,StartTime='%2', EndTime='%3',Count=%4 where Style='%5,' and StartTime='%6'").arg(ui->money->text().trimmed()).arg(ui->Lt1->text().trimmed()).arg(ui->Lt2->text()).arg(ui->count->text().trimmed()).arg(Sty).arg(SaT);

            QString up = SelectDB(sql);
            if(up == WRONG)
            {
                QString sql1 = "insert into ASSIGN(Style, Number) values(?,?)";
                InsertDB(sql1, "YES");

                Error("操作成功!");
                ADD = true;
                this->close();
            }
            else
            {
                Error(WRONG);
                ADD = false;
            }
        }
        else        //新增时，类型相同时开始时间不能相同
        {
            QString sql = tr("select Money from FELLOWSHIP where Style='%1,' and StartTime='%2'").arg(ui->style->text().trimmed()).arg(ui->Lt1->text().trimmed());

            QString money = SelectDB(sql);
            if(money != WRONG)
            {
                Error("该奖项已存在，请确定类型和开始时间是否雷同!");
                return;
            }

            QString sql1 = "insert into FELLOWSHIP(Style, Money, StartTime, EndTime, Count) values(?,?,?,?,?)";
            if(InsertDB(sql1, "NO"))
            {
                QString sql1 = "insert into ASSIGN(Style, Number) values(?,?)";
                InsertDB(sql1, "YES");

                Error("操作成功!");
                ADD = true;
                this->close();
            }
            else
            {
                Error(WRONG);
                ADD = false;
            }
        }
    }
    else        //按照专业年级分配，先删除原有记录，再重新插入
    {
        //判断数量
        int total = ui->year1->text().toInt() +
        ui->year2->text().toInt() +
        ui->year3->text().toInt() +
        ui->year4->text().toInt() +
        ui->year1_2->text().toInt() +
        ui->year2_2->text().toInt() +
        ui->year3_2->text().toInt() +
        ui->year4_2->text().toInt() +

        //网络工程
        ui->year1_4->text().toInt() +
        ui->year2_3->text().toInt() +
        ui->year3_3->text().toInt() +
        ui->year4_4->text().toInt() +
        ui->year1_3->text().toInt() +
        ui->year2_4->text().toInt() +
        ui->year3_4->text().toInt() +
        ui->year4_3->text().toInt() +

        //通信工程
        ui->year1_6->text().toInt() +
        ui->year2_5->text().toInt() +
        ui->year3_5->text().toInt() +
        ui->year4_6->text().toInt() +
        ui->year1_5->text().toInt() +
        ui->year2_6->text().toInt() +
        ui->year3_6->text().toInt() +
        ui->year4_5->text().toInt() +

        //计算机
        ui->year1_8->text().toInt() +
        ui->year2_7->text().toInt() +
        ui->year3_7->text().toInt() +
        ui->year4_8->text().toInt() +
        ui->year1_7->text().toInt() +
        ui->year2_8->text().toInt() +
        ui->year3_8->text().toInt() +
        ui->year4_7->text().toInt();

        //总数量
        if(ui->count->text().toInt() != total)
        {
            Error("奖励总名额和各专业名额总数不匹配!");
            return;
        }

        if(MA)      //编辑更新
        {
            QString sql = tr("update FELLOWSHIP set Money=%1,StartTime='%2', EndTime='%3',Count=%4 where Style='%5,' and StartTime='%6'").arg(ui->money->text().trimmed()).arg(ui->Lt1->text().trimmed()).arg(ui->Lt2->text()).arg(ui->count->text().trimmed()).arg(Sty).arg(SaT);
            qDebug()<<sql;

            QString up = SelectDB(sql);
            if(up != WRONG)
            {
                Error("操作失败!");
                return;
            }
        }
        else
        {
        //更新奖学金表
            QString sql1 = "insert into FELLOWSHIP(Style, Money, StartTime, EndTime, Count) values(?,?,?,?,?)";

            if(!InsertDB(sql1, "NO"))
            {
                Error("操作成功!");
                return;
            }
        }

        //统计专业，年级情况
        ReadMessage();
        if(DeleteDB())
        {
            Error("操作成功!");
            ADD = true;
            this->close();
        }
    }
}

void FellowShip::on_T1_clicked()        //开始时间
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();

    ui->label_11->hide();
    ui->yes->hide();
    ui->no->hide();

    time = true;
    if(!ui->yes->isChecked())
    {
        this->resize(535, 330);
    }
    else
        ui->groupBox->hide();

    ui->calendarWidget->show();
}

void FellowShip::on_T2_clicked()
{
    ui->pushButton->hide();
    ui->pushButton_2->hide();

    ui->label_11->hide();
    ui->yes->hide();
    ui->no->hide();

    time = false;
    if(!ui->yes->isChecked())
    {
        this->resize(535, 330);
    }
    else
        ui->groupBox->hide();

    ui->calendarWidget->show();
}

void FellowShip::on_pushButton_2_clicked()      //取消
{
    this->close();
}

void FellowShip::on_calendarWidget_clicked(const QDate &date)
{
    QString times = date.toString("yyyy-MM-dd");
    if(checkTime(times))
    {
        if(time)
        {
            ui->Lt1->setText(times);
        }
        else
            ui->Lt2->setText(times);

        ui->pushButton->show();
        ui->pushButton_2->show();
        ui->calendarWidget->hide();
        if(!ui->yes->isChecked())
            this->resize(535, 250);

        ui->label_11->show();
        ui->yes->show();
        ui->no->show();
        if(ui->yes->isChecked())
            ui->groupBox->show();
    }
}

bool FellowShip::checkTime(QString times)        //时间判断
{
    QDate cur= QDate::currentDate();
    QString curtime = cur.toString("yyyy-MM-dd");

    if(time)        //申请开始时间
    {
        if(times.compare(curtime) < 0)
        {
            Error("开始时间不可在当前时间之前!");
            return false;
        }
    }
    else        //申请截止时间
    {
        if(times.compare(curtime) < 0)
        {
            Error("结束时间不可在当前时间之前!");
            return false;
        }
        else if(times.compare(ui->Lt1->text()) < 0)
        {
            Error("结束时间不可在开始时间之前!");
            return false;
        }
    }
    return true;
}

//检验软件工程编号
bool FellowShip::checkSoftNumber()
{
    static bool tf = true;
    if(tr)
    {
        QString number = ui->lineEdit->text().trimmed();
        QRegExp reg("^[\\d]{4}$");
        if(!reg.exactMatch(number))
        {
            Error("编号必须为四个数字字符");
            ui->lineEdit->clear();
            return false;
        }

        tf = false;
    }

    return true;
}

bool FellowShip::checkNetNumber()
{
    static bool tf = true;
    if(tr)
    {
        QString number = ui->lineEdit_2->text().trimmed();
        QRegExp reg("^[\\d]{4}$");
        if(!reg.exactMatch(number))
        {
            Error("编号必须为四个数字字符");
            ui->lineEdit->clear();
            return false;
        }

        tf = false;
    }

    return true;
}

bool FellowShip::checkTelNumber()
{
    static bool tf = true;
    if(tr)
    {
        QString number = ui->lineEdit_3->text().trimmed();
        QRegExp reg("^[\\d]{4}$");
        if(!reg.exactMatch(number))
        {
            Error("编号必须为四个数字字符");
            ui->lineEdit->clear();
            return false;
        }

        tf = false;
    }

    return true;
}

bool FellowShip::checkCompNumber()
{
    static bool tf = true;
    if(tr)
    {
        QString number = ui->lineEdit_4->text().trimmed();
        QRegExp reg("^[\\d]{4}$");
        if(!reg.exactMatch(number))
        {
            Error("编号必须为四个数字字符");
            ui->lineEdit->clear();
            return false;
        }

        tf = false;
    }

    return true;
}

//清空所有
void FellowShip::clearAll()
{
    //软件工程
    ui->year1->clear();
    ui->year2->clear();
    ui->year3->clear();
    ui->year4->clear();
    ui->year1_2->clear();
    ui->year2_2->clear();
    ui->year3_2->clear();
    ui->year4_2->clear();

    ui->soft1->setChecked(false);
    ui->soft2->setChecked(false);
    ui->soft3->setChecked(false);
    ui->soft4->setChecked(false);
    ui->soft1_2->setChecked(false);
    ui->soft2_2->setChecked(false);
    ui->soft3_2->setChecked(false);
    ui->soft4_2->setChecked(false);

    //网络工程
    ui->year1_4->clear();
    ui->year2_3->clear();
    ui->year3_3->clear();
    ui->year4_4->clear();
    ui->year1_3->clear();
    ui->year2_4->clear();
    ui->year3_4->clear();
    ui->year4_3->clear();

    ui->soft1_3->setChecked(false);
    ui->soft2_4->setChecked(false);
    ui->soft3_3->setChecked(false);
    ui->soft4_3->setChecked(false);
    ui->soft1_4->setChecked(false);
    ui->soft2_3->setChecked(false);
    ui->soft3_4->setChecked(false);
    ui->soft4_4->setChecked(false);


    //通信工程
    ui->year1_6->clear();
    ui->year2_5->clear();
    ui->year3_5->clear();
    ui->year4_6->clear();
    ui->year1_5->clear();
    ui->year2_6->clear();
    ui->year3_6->clear();
    ui->year4_5->clear();

    ui->soft1_5->setChecked(false);
    ui->soft2_6->setChecked(false);
    ui->soft3_5->setChecked(false);
    ui->soft4_5->setChecked(false);
    ui->soft1_6->setChecked(false);
    ui->soft2_5->setChecked(false);
    ui->soft3_6->setChecked(false);
    ui->soft4_6->setChecked(false);

    //计算机
    ui->year1_8->clear();
    ui->year2_7->clear();
    ui->year3_7->clear();
    ui->year4_8->clear();
    ui->year1_7->clear();
    ui->year2_8->clear();
    ui->year3_8->clear();
    ui->year4_7->clear();

    ui->soft1_7->setChecked(false);
    ui->soft2_8->setChecked(false);
    ui->soft3_7->setChecked(false);
    ui->soft4_7->setChecked(false);
    ui->soft1_8->setChecked(false);
    ui->soft2_7->setChecked(false);
    ui->soft3_8->setChecked(false);
    ui->soft4_8->setChecked(false);

    SGrade.clear();
    SCount.clear();
    NGrade.clear();
    NCount.clear();
    TGrade.clear();
    TCount.clear();
    CGrade.clear();
    CCount.clear();
}

//软件工程，大一
void FellowShip::on_soft1_clicked()
{
    if(checkSoftNumber())
    {
        if(ui->soft1->isChecked())
        {
            ui->year1->show();
        }
        else
        {
            ui->year1->hide();
        }
    }
    else
    {
        ui->soft1->setChecked(false);
        return;
    }
}

//软件工程，大二
void FellowShip::on_soft2_clicked()
{
    if(checkSoftNumber())
    {
        if(ui->soft1->isChecked() && ui->year1->text().isEmpty())
        {
            Error("请完善上个年级信息");
            return;
        }

        if(ui->soft2->isChecked())
        {
            ui->year2->show();
        }
        else
        {
            ui->year2->hide();
        }
    }
    else
    {
        ui->soft2->setChecked(false);
        return;
    }
}

//软件，大三
void FellowShip::on_soft3_clicked()
{
    if(ui->soft2->isChecked() && ui->year2->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkSoftNumber())
    {
        if(ui->soft3->isChecked())
        {
            ui->year3->show();
        }
        else
        {
            ui->year3->hide();
        }
    }
    else
    {
        ui->soft3->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_clicked()
{
    if(ui->soft3->isChecked() && ui->year3->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkSoftNumber())
    {
        if(ui->soft4->isChecked())
        {
            ui->year4->show();
        }
        else
        {
            ui->year4->hide();
        }
    }
    else
    {
        ui->soft4->setChecked(false);
        return;
    }
}

void FellowShip::on_soft1_2_clicked()
{
    if(ui->soft4->isChecked() && ui->year4->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkSoftNumber())
    {
        if(ui->soft1_2->isChecked())
        {
            ui->year1_2->show();
        }
        else
        {
            ui->year1_2->hide();
        }
    }
    else
    {
        ui->soft1_2->setChecked(false);
        return;
    }
}

void FellowShip::on_soft2_2_clicked()
{
    if(ui->soft1_2->isChecked() && ui->year1_2->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkSoftNumber())
    {
        if(ui->soft2_2->isChecked())
        {
            ui->year2_2->show();
        }
        else
        {
            ui->year2_2->hide();
        }
    }
    else
    {
        ui->soft2_2->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_2_clicked()
{
    if(ui->soft2_2->isChecked() && ui->year2_2->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkSoftNumber())
    {
        if(ui->soft3_2->isChecked())
        {
            ui->year3_2->show();
        }
        else
        {
            ui->year3_2->hide();
        }
    }
    else
    {
        ui->soft3_2->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_2_clicked()
{
    if(ui->soft3_2->isChecked() && ui->year3_2->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkSoftNumber())
    {
        if(ui->soft4_2->isChecked())
        {
            ui->year4_2->show();
        }
        else
        {
            ui->year4_2->hide();
        }
    }
    else
    {
        ui->soft4_2->setChecked(false);
        return;
    }
}

//网络工程
void FellowShip::on_soft1_3_clicked()
{
    if(checkNetNumber())
    {
        if(ui->soft1_3->isChecked())
        {
            ui->year1_4->show();
        }
        else
        {
            ui->year1_4->hide();
        }
    }
    else
    {
        ui->soft1_3->setChecked(false);
        return;
    }
}

void FellowShip::on_soft2_4_clicked()
{
    if(ui->soft1_3->isChecked() && ui->year1_4->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft2_4->isChecked())
        {
            ui->year2_3->show();
        }
        else
        {
            ui->year2_3->hide();
        }
    }
    else
    {
        ui->soft2_4->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_3_clicked()
{
    if(ui->soft2_4->isChecked() && ui->year2_3->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft3_3->isChecked())
        {
            ui->year3_3->show();
        }
        else
        {
            ui->year3_3->hide();
        }
    }
    else
    {
        ui->soft3_3->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_3_clicked()
{
    if(ui->soft3_3->isChecked() && ui->year3_3->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft4_3->isChecked())
        {
            ui->year4_4->show();
        }
        else
        {
            ui->year4_4->hide();
        }
    }
    else
    {
        ui->soft4_3->setChecked(false);
        return;
    }
}

void FellowShip::on_soft1_4_clicked()
{
    if(ui->soft4_3->isChecked() && ui->year4_4->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft1_4->isChecked())
        {
            ui->year1_3->show();
        }
        else
        {
            ui->year1_3->hide();
        }
    }
    else
    {
        ui->soft1_4->setChecked(false);
        return;
    }
}

void FellowShip::on_soft2_3_clicked()
{
    if(ui->soft1_4->isChecked() && ui->year1_3->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft2_3->isChecked())
        {
            ui->year2_4->show();
        }
        else
        {
            ui->year2_4->hide();
        }
    }
    else
    {
        ui->soft2_3->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_4_clicked()
{
    if(ui->soft2_3->isChecked() && ui->year2_4->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft3_4->isChecked())
        {
            ui->year3_4->show();
        }
        else
        {
            ui->year3_4->hide();
        }
    }
    else
    {

        ui->soft3_4->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_4_clicked()
{
    if(ui->soft3_4->isChecked() && ui->year3_4->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkNetNumber())
    {
        if(ui->soft4_4->isChecked())
        {
            ui->year4_3->show();
        }
        else
        {
            ui->year4_3->hide();
        }
    }
    else
    {

        ui->soft4_4->setChecked(false);
        return;
    }
}

//通信工程
void FellowShip::on_soft1_5_clicked()
{
    if(checkTelNumber())
    {
        if(ui->soft1_5->isChecked())
        {
            ui->year1_6->show();
        }
        else
        {
            ui->year1_6->hide();
        }
    }
    else
    {

        ui->soft1_6->setChecked(false);
        return;
    }
}

void FellowShip::on_soft2_6_clicked()
{
    if(ui->soft1_5->isChecked() && ui->year1_6->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft2_6->isChecked())
        {
            ui->year2_5->show();
        }
        else
        {
            ui->year2_5->hide();
        }
    }
    else
    {

        ui->soft2_6->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_5_clicked()
{
    if(ui->soft2_6->isChecked() && ui->year2_5->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft3_5->isChecked())
        {
            ui->year3_5->show();
        }
        else
        {
            ui->year3_5->hide();
        }
    }
    else
    {

        ui->soft3_5->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_5_clicked()
{
    if(ui->soft3_5->isChecked() && ui->year3_5->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft4_5->isChecked())
        {
            ui->year4_6->show();
        }
        else
        {
            ui->year4_6->hide();
        }
    }
    else
    {

        ui->soft4_5->setChecked(false);
        return;
    }
}

void FellowShip::on_soft1_6_clicked()
{
    if(ui->soft4_5->isChecked() && ui->year4_6->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft1_6->isChecked())
        {
            ui->year1_5->show();
        }
        else
        {
            ui->year1_5->hide();
        }
    }
    else
    {

        ui->soft1_6->setChecked(false);
        return;
    }
}

void FellowShip::on_soft2_5_clicked()
{
    if(ui->soft1_6->isChecked() && ui->year1_5->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft2_5->isChecked())
        {
            ui->year2_6->show();
        }
        else
        {
            ui->year2_6->hide();
        }
    }
    else
    {

        ui->soft2_5->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_6_clicked()
{
    if(ui->soft2_5->isChecked() && ui->year2_6->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft3_6->isChecked())
        {
            ui->year3_6->show();
        }
        else
        {
            ui->year3_6->hide();
        }
    }
    else
    {

        ui->soft3_6->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_6_clicked()
{
    if(ui->soft3_6->isChecked() && ui->year3_6->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkTelNumber())
    {
        if(ui->soft4_6->isChecked())
        {
            ui->year4_5->show();
        }
        else
        {
            ui->year4_5->hide();
        }
    }
    else
    {

        ui->soft4_6->setChecked(false);
        return;
    }
}

//计科
void FellowShip::on_soft1_7_clicked()
{
    if(checkCompNumber())
    {
        if(ui->soft1_7->isChecked())
        {
            ui->year1_8->show();
        }
        else
        {
            ui->year1_8->hide();
        }
    }
    else
    {

        ui->soft1_7->setChecked(false);
        return;
    }
}

//计科，大二
void FellowShip::on_soft2_8_clicked()
{
    if(ui->soft1_7->isChecked() && ui->year1_8->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft2_8->isChecked())
        {
            ui->year2_7->show();
        }
        else
        {
            ui->year2_7->hide();
        }
    }
    else
    {

        ui->soft2_8->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_7_clicked()
{
    if(ui->soft2_8->isChecked() && ui->year2_7->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft3_7->isChecked())
        {
            ui->year3_7->show();
        }
        else
        {
            ui->year3_7->hide();
        }
    }
    else
    {

        ui->soft3_7->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_7_clicked()
{
    if(ui->soft3_7->isChecked() && ui->year3_7->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft4_7->isChecked())
        {
            ui->year4_8->show();
        }
        else
        {
            ui->year4_8->hide();
        }
    }
    else
    {

        ui->soft4_7->setChecked(false);
        return;
    }
}

void FellowShip::on_soft1_8_clicked()
{
    if(ui->soft4_7->isChecked() && ui->year4_8->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft1_8->isChecked())
        {
            ui->year1_7->show();
        }
        else
        {
            ui->year1_7->hide();
        }
    }
    else
    {

        ui->soft1_8->setChecked(false);
        return;
    }
}

void FellowShip::on_soft2_7_clicked()
{
    if(ui->soft1_8->isChecked() && ui->year1_7->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft2_7->isChecked())
        {
            ui->year2_8->show();
        }
        else
        {
            ui->year2_8->hide();
        }
    }
    else
    {

        ui->soft2_7->setChecked(false);
        return;
    }
}

void FellowShip::on_soft3_8_clicked()
{
    if(ui->soft2_7->isChecked() && ui->year2_8->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft3_8->isChecked())
        {
            ui->year3_8->show();
        }
        else
        {
            ui->year3_8->hide();
        }
    }
    else
    {

        ui->soft3_8->setChecked(false);
        return;
    }
}

void FellowShip::on_soft4_8_clicked()
{
    if(ui->soft3_8->isChecked() && ui->year3_8->text().isEmpty())
    {
        Error("请完善上个年级信息");
        return;
    }

    if(checkCompNumber())
    {
        if(ui->soft4_8->isChecked())
        {
            ui->year4_7->show();
        }
        else
        {
            ui->year4_7->hide();
        }
    }
    else
    {

        ui->soft4_8->setChecked(false);
        return;
    }
}

//统计专业，年级分配情况
void FellowShip::ReadMessage()
{
    //统计软件工程年级，数量分配
    if(!ui->lineEdit->text().isEmpty())
    {
        if(ui->soft1->isChecked())
        {
            SGrade.append(ui->soft1->text());
            SCount.append(ui->year1->text());
        }
        if(ui->soft2->isChecked())
        {
            SGrade.append(ui->soft2->text());
            SCount.append(ui->year2->text());
        }
        if(ui->soft3->isChecked())
        {
            SGrade.append(ui->soft3->text());
            SCount.append(ui->year3->text());
        }
        if(ui->soft4->isChecked())
        {
            SGrade.append(ui->soft4->text());
            SCount.append(ui->year4->text());
        }
        if(ui->soft1_2->isChecked())
        {
            SGrade.append(ui->soft1_2->text());
            SCount.append(ui->year1_2->text());
        }
        if(ui->soft2_2->isChecked())
        {
            SGrade.append(ui->soft2_2->text());
            SCount.append(ui->year2_2->text());
        }
        if(ui->soft3_2->isChecked())
        {
            SGrade.append(ui->soft3_2->text());
            SCount.append(ui->year3_2->text());
        }
        if(ui->soft4_2->isChecked())
        {
            SGrade.append(ui->soft4_2->text());
            SCount.append(ui->year4_2->text());
        }
    }

    //统计网络工程年级，数量分配
    if(!ui->lineEdit_2->text().isEmpty())
    {
        if(ui->soft1_3->isChecked())
        {
            NGrade.append(ui->soft1_3->text());
            NCount.append(ui->year1_4->text());
        }
        if(ui->soft2_4->isChecked())
        {
            NGrade.append(ui->soft2_4->text());
            NCount.append(ui->year2_3->text());
        }
        if(ui->soft3_3->isChecked())
        {
            NGrade.append(ui->soft3_3->text());
            NCount.append(ui->year3_3->text());
        }
        if(ui->soft4_3->isChecked())
        {
            NGrade.append(ui->soft4_3->text());
            NCount.append(ui->year4_4->text());
        }
        if(ui->soft1_4->isChecked())
        {
            NGrade.append(ui->soft1_4->text());
            NCount.append(ui->year1_3->text());
        }
        if(ui->soft2_3->isChecked())
        {
            NGrade.append(ui->soft2_3->text());
            NCount.append(ui->year2_4->text());
        }
        if(ui->soft3_4->isChecked())
        {
            NGrade.append(ui->soft3_4->text());
            NCount.append(ui->year3_4->text());
        }
        if(ui->soft4_4->isChecked())
        {
            NGrade.append(ui->soft4_4->text());
            NCount.append(ui->year4_3->text());
        }
    }

    //统计通信工程年级，数量分配
    if(!ui->lineEdit_3->text().isEmpty())
    {
        if(ui->soft1_5->isChecked())
        {
            TGrade.append(ui->soft1_5->text());
            TCount.append(ui->year1_6->text());
        }
        if(ui->soft2_6->isChecked())
        {
            TGrade.append(ui->soft2_6->text());
            TCount.append(ui->year2_5->text());
        }
        if(ui->soft3_5->isChecked())
        {
            TGrade.append(ui->soft3_5->text());
            TCount.append(ui->year3_5->text());
        }
        if(ui->soft4_5->isChecked())
        {
            TGrade.append(ui->soft4_5->text());
            TCount.append(ui->year4_6->text());
        }
        if(ui->soft1_6->isChecked())
        {
            TGrade.append(ui->soft1_6->text());
            TCount.append(ui->year1_5->text());
        }
        if(ui->soft2_5->isChecked())
        {
            TGrade.append(ui->soft2_5->text());
            TCount.append(ui->year2_6->text());
        }
        if(ui->soft3_6->isChecked())
        {
            TGrade.append(ui->soft3_6->text());
            TCount.append(ui->year3_6->text());
        }
        if(ui->soft4_6->isChecked())
        {
            TGrade.append(ui->soft4_6->text());
            TCount.append(ui->year4_5->text());
        }
    }

    //统计计科年级，数量分配
    if(!ui->lineEdit_4->text().isEmpty())
    {
        if(ui->soft1_7->isChecked())
        {
            CGrade.append(ui->soft1_7->text());
            CCount.append(ui->year1_8->text());
        }
        if(ui->soft2_8->isChecked())
        {
            CGrade.append(ui->soft2_8->text());
            CCount.append(ui->year2_7->text());
        }
        if(ui->soft3_7->isChecked())
        {
            CGrade.append(ui->soft3_7->text());
            CCount.append(ui->year3_7->text());
        }
        if(ui->soft4_7->isChecked())
        {
            CGrade.append(ui->soft4_7->text());
            CCount.append(ui->year4_8->text());
        }
        if(ui->soft1_8->isChecked())
        {
            CGrade.append(ui->soft1_8->text());
            CCount.append(ui->year1_7->text());
        }
        if(ui->soft2_7->isChecked())
        {
            CGrade.append(ui->soft2_7->text());
            CCount.append(ui->year2_8->text());
        }
        if(ui->soft3_8->isChecked())
        {
            CGrade.append(ui->soft3_8->text());
            CCount.append(ui->year3_8->text());
        }
        if(ui->soft4_8->isChecked())
        {
            CGrade.append(ui->soft4_8->text());
            CCount.append(ui->year4_7->text());
        }
    }
}

//删除所有原始信息
bool FellowShip::DeleteDB()
{
    QString style = ui->style->text().trimmed();
    QString sql = "select Number from ASSIGN where Style='" + style +"'";
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        if(query.value(0).toString().trimmed() == "0000")
            continue;

        //删除年级分配
        QString sql1 = "delete from ASSIGN2 where Number='" + query.value(0).toString().trimmed() + "'";
        QString re = SelectDB(sql1);
        if(re != WRONG)
        {
            Error("操作失败!");
            return false;
        }
    }

    //删除专业分配
    QString sql2 = "delete from ASSIGN where Style='" + style + "'";
    QString res = SelectDB(sql2);
    if(res != WRONG)
    {
        Error("操作失败!");
        return false;
    }

    if(DoInsertALL())
        return true;
    else
        return false;
}

bool FellowShip::DoInsertALL()
{
    //插入软件工程分配情况
    if(!ui->lineEdit->text().isEmpty())
    {
        if(!InsertALLDB("软件工程", ui->lineEdit->text().trimmed(), SGrade, SCount))
        {
            Error("操作失败!");
            return false;
        }
    }
    QThread::sleep(0.5);

    //插入网络工程分配情况
    if(!ui->lineEdit_2->text().isEmpty())
    {
        if(!InsertALLDB("网络工程", ui->lineEdit_2->text().trimmed(), NGrade, NCount))
        {
            Error("操作失败!");
            return false;
        }
    }
    QThread::sleep(0.5);

    //插入通信工程分配情况
    if(!ui->lineEdit_3->text().isEmpty())
    {
        if(!InsertALLDB("通信工程", ui->lineEdit_3->text().trimmed(), TGrade, TCount))
        {
            Error("操作失败!");
            return false;
        }
    }
    QThread::sleep(0.5);

    //插入计科工程分配情况
    if(!ui->lineEdit_4->text().isEmpty())
    {
        if(!InsertALLDB("计算机科学技术", ui->lineEdit_4->text().trimmed(), CGrade, CCount))
        {
            Error("操作失败!");
            return false;
        }
    }

    return true;
}

//插入当前所有信息（更新）
bool FellowShip::InsertALLDB(QString major, QString number, QStringList grade, QStringList count)
{
    mu.lock();      //枷锁

    static bool tf = true;
    if(tf)
    {
        //更新fellowship的总数量
        QString sql2 = tr("update FELLOWSHIP set Count=%1 where Style='%2,'").arg(ui->count->text().trimmed()).arg(ui->style->text().trimmed());
        QString up = SelectDB(sql2);
        if(up != WRONG)
        {
            Error("操作失败!");
            return false;
        }

        tf = false;
    }

    QSqlQuery query(db);
    QString sql = "insert into ASSIGN(Style, SMajor, Number) values(?,?,?)";
    //插入专业分配编号
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare(sql);
    query.addBindValue(tr("%1").arg(ui->style->text()));
    query.addBindValue(tr("%1").arg(major));
    query.addBindValue(tr("%1").arg(number));
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    int size = grade.size();
    QSqlQuery query1(db);
    QString sql1 = "insert into ASSIGN2(Number, Grade, Count, NowCount) values(?,?,?,?)";
    while(size--)
    {
        QSqlDatabase::database().transaction();   //事务开始
        query1.prepare(sql1);
        query1.addBindValue(tr("%1").arg(number));
        query1.addBindValue(tr("%1").arg(grade.at(size)));
        query1.addBindValue(tr("%1").arg(count.at(size)));
        query1.addBindValue(tr("%1").arg(count.at(size)));
        query1.exec();
        QSqlDatabase::database().commit();  //事务结束
    }

    mu.unlock();
    return true;
}
