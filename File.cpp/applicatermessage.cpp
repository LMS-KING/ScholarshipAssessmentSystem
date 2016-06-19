#include "applicatermessage.h"
#include "ui_applicatermessage.h"
#include "passwsure.h"
#include "photo.h"
#include <QDebug>
#include <QDate>
#include <QTimer>
#include <QMessageBox>
#define WRONG "数据库访问错误!"

QString PAW;
float DScore;
float ZScore;
float TScore;

ApplicaterMessage::ApplicaterMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicaterMessage)
{
    ui->setupUi(this);

    QTimer *time = new QTimer(this);
    connect(time,SIGNAL(timeout()), this, SLOT(timeOut()));
    time->start(100);

    MgSr = true;
    ui->label_4->hide();

    score1 = 0.75;
    score2 = 0.20;
    score3 = 0.05;
}

ApplicaterMessage::~ApplicaterMessage()
{
    delete ui;
}

void ApplicaterMessage::recvDbNum(QSqlDatabase thdb, QString teacherN)
{
    db = thdb;
    teacherNo = teacherN;
}

void ApplicaterMessage::recvMeageOrSCo(bool tf, bool all)
{
    MgSr = tf;
    AllMg = all;
    setCombox();
    setRecord();
}

void ApplicaterMessage::setRecord()     //显示信息记录
{
    QString sql = NULL;
    if(AllMg)
    {
        sql = "select StudentNo from STUDENT";
    }
    else
    {
        QString sql1 = "select StudentNo from STUDENT where SPriority="+ QString::number(1);

        QString stu = NULL;
        QSqlQuery query(db);
        query.exec(sql1);
        while(query.next())
        {
            stu += query.value(0).toString().trimmed() + ",";
        }

        QString stus = stu.left(stu.length() - 1);

        if(MgSr)        //查询申请人信息
            sql = tr("select StudentNo, Style, Status, Time from APPLICATION where StudentNo in(%1) and Status='%2'").arg(stus).arg("待审核");
        else            //查询申请人成绩(根据学号和学期去重)
            sql = tr("select StudentNo, Time, Time, Time from APPLICATION where id in(select min(id) from APPLICATION group by StudentNo, Time) and Status='%1'").arg("待审核");
    }

    ShowRecord(sql, "ALL", NULL, NULL, NULL);
}

void ApplicaterMessage::setCombox()     //设置下拉框
{
    static bool tf = true;
    if(tf)
    {
        QString major = "请选择,软件工程,网络工程,通信工程,计算机科学技术";
        QStringList majors = major.split(",", QString::SkipEmptyParts);
        ui->comboBox->addItems(majors);

        QString sql = "select Style from FELLOWSHIP";
        QString style = "请选择," + SelectDB(sql);
        QStringList styles = style.split(",", QString::SkipEmptyParts);
        ui->comboBox_2->addItems(styles);

        QString sql1 = "select class from GRADE";
        QString grade = "请选择," + SelectDB(sql1);
        QStringList grades = grade.split(",", QString::SkipEmptyParts);
        ui->comboBox_3->addItems(grades);

        tf = false;
    }

    if(!MgSr)       //查看成绩
    {
        ui->pushButton_3->hide();
        ui->pushButton_4->hide();
        ui->pushButton->setEnabled(true);
        ui->pushButton->setText("查看分数明细");

        ui->comboBox_4->clear();

        QString mainkey = "请选择,总分主序,德育主序,智育主序,体育主序";
        QStringList mains = mainkey.split(",", QString::SkipEmptyParts);
        ui->comboBox_4->addItems(mains);

        ui->label_5->show();
        ui->comboBox_4->show();
    }
    else
    {
        if(AllMg)       //所有人信息
        {
            ui->label_2->hide();
            ui->comboBox_2->hide();

            ui->pushButton->hide();
            ui->pushButton_3->hide();
            ui->pushButton_4->hide();
        }
        else            //申请人信息
        {
            ui->label_2->show();
            ui->comboBox_2->show();

            ui->pushButton->show();
            ui->pushButton_3->show();
            ui->pushButton_4->show();
        }

        ui->label_5->hide();
        ui->comboBox_4->hide();

        ui->pushButton->setText("查看提交资料");
    }
}

QString ApplicaterMessage::SelectDB(QString sql)
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

void ApplicaterMessage::setPersonMessage(QString stuN, QString grade)      //查看跟人提交资料
{
    ticket_model = new QStandardItemModel();
    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("项目名称")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("项目时间")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("项目类型")));
    ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("所属学期")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);
    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置时间显示列的宽度
    ui->tableView->setColumnWidth(0,180);
    ui->tableView->setColumnWidth(1,180);
    ui->tableView->setColumnWidth(2,180);
    ui->tableView->setColumnWidth(3,180);
     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);

    //model = ui->tableView->model();
    //model->removeRows(0, model->rowCount());

    QString sql = tr("select Expres, Time,TableName, TimeC from MORALISM_ADD_SCORE where StudentNo='%1' and TimeC like'%2%'").arg(stuN).arg(grade);

    QSqlQuery query(db);
    query.exec(sql);
    int i = 0;
    while(query.next())
    {
        ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
        ticket_model->setItem(i, 1, new QStandardItem(query.value(1).toString().trimmed()));
        if(query.value(2).toString() != "SPORTS_SCORE")
            ticket_model->setItem(i, 2, new QStandardItem("德育加分"));
        else
            ticket_model->setItem(i, 2, new QStandardItem("体育加分"));
        ticket_model->setItem(i, 3, new QStandardItem(query.value(3).toString().trimmed()));

        //设置单元格文本居中
        ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        i++;
    }

    if(i == 0)
        ui->label_4->show();
    else
        ui->label_4->hide();
}

int ApplicaterMessage::selectScore(QString sql, QString style)
{
    int score = 0;
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        if(style == "DY" || style == "TY")       //德育分.体育
        {
            QString sql1 = NULL;
            if(style == "DY")
                sql1= tr("select Score from %1 where Item='%2'").arg(query.value(1).toString().trimmed()).arg(query.value(0).toString().trimmed());
            else if(style == "TY")
                sql1= tr("select Score from SPORTS_SCORE where Item='%1'").arg(query.value(0).toString().trimmed());

            QSqlQuery query1(db);
            query1.exec(sql1);
            while(query1.next())
            {
                score += query1.value(0).toInt();
            }
        }
        else if(style == "ZY")      //智育分
        {
            score += query.value(0).toInt();
        }
    }

    if(score < 0)
    {
        QMessageBox::information(this, "提示", WRONG, QMessageBox::Ok);
    }

    return score;
}

void ApplicaterMessage::setScore(QString sql, QString style, QString grade)     //设置单项成绩，德育，智育，体育
{
    ticket_model = new QStandardItemModel();
    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("项目名称")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("加分值")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("所属学期")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);

    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);

    model = ui->tableView->model();
    model->removeRows(0, model->rowCount());

    if(style.isEmpty())
    {
        ui->label_4->show();
        return;
    }

    int i = 0;
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        if(style == "DY" || style == "TY")
        {
            QString sql1 = NULL;
            if(style == "DY")
                sql1= tr("select Score from %1 where Item='%2'").arg(query.value(3).toString().trimmed()).arg(query.value(0).toString().trimmed());
            else if(style == "TY")
                sql1= tr("select Score from SPORTS_SCORE where Item='%1'").arg(query.value(0).toString().trimmed());

            QSqlQuery query1(db);
            query1.exec(sql1);
            while(query1.next())
            {
                ticket_model->setItem(i, 0, new QStandardItem(query.value(1).toString().trimmed()));
                ticket_model->setItem(i, 1, new QStandardItem(query1.value(0).toString().trimmed()));
                ticket_model->setItem(i, 2, new QStandardItem(query.value(2).toString().trimmed()));

                //设置单元格文本居中
                ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
                ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);

                i++;
            }
        }
        else if(style == "ZY")
        {
            QString sql1 = "select SMajor from STUDENT where StudentNo='" + stuNo + "'";
            QString smajor = SelectDB(sql1);

            QString sql = tr("select ClassName,Time from CLASS where ClassNo='%1' and SMajor='%2' and Time like '%3%'").arg(query.value(0).toString().trimmed()).arg(smajor).arg(grades);

            QSqlQuery query1(db);
            query1.exec(sql);
            while(query1.next())
            {
                ticket_model->setItem(i, 0, new QStandardItem(query1.value(0).toString().trimmed()));
                ticket_model->setItem(i, 1, new QStandardItem(query.value(1).toString().trimmed()));
                ticket_model->setItem(i, 2, new QStandardItem(query1.value(1).toString().trimmed()));

                //设置单元格文本居中
                ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
                ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);

                i++;
            }
        }
    }

    if(i == 0)
    {
        ui->label_4->show();
    }
    else
        ui->label_4->hide();
}

void ApplicaterMessage::ShowRecord(QString sql,QString style, QString major = NULL, QString grade = NULL, QString fellowstyle = NULL)
{
    if(MgSr)
    {
        if(AllMg)       //所有人信息
        {
            ticket_model = new QStandardItemModel();
            ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
            ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
            ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
            ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("电话")));
            ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("邮箱")));

            //设置列的宽度
            ui->tableView->setColumnWidth(0,150);
            ui->tableView->setColumnWidth(2,150);
            ui->tableView->setColumnWidth(3,170);
            ui->tableView->setColumnWidth(4,175);
        }
        else        //申请人信息
        {
            ticket_model = new QStandardItemModel();
            ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
            ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
            ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
            ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("申请奖学金类型")));
            ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("状态")));
            ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("电话")));
            ticket_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("邮箱")));
            ticket_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("申请学期")));

            //设置列的宽度
            ui->tableView->setColumnWidth(0,110);
            ui->tableView->setColumnWidth(6,110);
            ui->tableView->setColumnWidth(7,110);
        }
    }
    else
    {
        ticket_model = new QStandardItemModel();
        ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
        ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
        ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
        ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("德育成绩")));
        ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("智育成绩")));
        ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("体育成绩")));
        ticket_model->setHorizontalHeaderItem(6, new QStandardItem(QObject::tr("总成绩")));
        ticket_model->setHorizontalHeaderItem(7, new QStandardItem(QObject::tr("学期")));

        //设置时间显示列的宽度
        ui->tableView->setColumnWidth(0,110);
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

    QSqlQuery query(db);

    query.exec(sql);        //sql 查询申请表
    int i = 0;
    QString sql1 = NULL;
    while(query.next())
    {
        QSqlQuery query1(db);
        if(style == "ALL")
            sql1 = tr("select SName, SMajor,STel, SEmail from STUDENT where StudentNo='%1' and SPriority=%2").arg(query.value(0).toString().trimmed()).arg(1);
        else if(style == "MA")  //按专业
        {
            sql1 = tr("select SName, SMajor,STel, SEmail from STUDENT where StudentNo='%1' and SMajor='%2' and SPriority=%3").arg(query.value(0).toString().trimmed()).arg(major).arg(1);
        }

        query1.exec(sql1);
        while(query1.next())
        {
            ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
            ticket_model->setItem(i, 1, new QStandardItem(query1.value(0).toString().trimmed()));
            ticket_model->setItem(i, 2, new QStandardItem(query1.value(1).toString().trimmed()));

            if(MgSr)
            {
                if(AllMg)
                {
                    ticket_model->setItem(i, 3, new QStandardItem(query1.value(2).toString().trimmed()));
                    ticket_model->setItem(i, 4, new QStandardItem(query1.value(3).toString().trimmed()));

                    //设置单元格文本居中
                    ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(i, 4)->setTextAlignment(Qt::AlignCenter);

                    i++;
                    continue;
                }
                else
                {
                    ticket_model->setItem(i, 3, new QStandardItem(query.value(1).toString().trimmed()));
                    ticket_model->setItem(i, 4, new QStandardItem(query.value(2).toString().trimmed()));
                    ticket_model->setItem(i, 5, new QStandardItem(query1.value(2).toString().trimmed()));
                    ticket_model->setItem(i, 6, new QStandardItem(query1.value(3).toString().trimmed()));
                    ticket_model->setItem(i, 7, new QStandardItem(query.value(3).toString().trimmed()));

                    if(query.value(2).toString() == "未批准")
                        ticket_model->item(i, 4)->setForeground(QBrush(QColor(255, 0, 0)));
                    else if(query.value(2).toString() == "已批准")
                    {
                        //设置状态单元格为
                        ticket_model->item(i, 4)->setForeground(QBrush(QColor(0,255,0)));
                    }
                    else if(query.value(2).toString() == "待审核")
                    {
                        //设置状态单元格为
                        ticket_model->item(i, 4)->setForeground(QBrush(QColor(0,0,255)));
                    }
                }
            }
            else        //显示成绩
            {
                //德育分
                QString sqlD = tr("select Item, TableName from MORALISM_ADD_SCORE where StudentNo='%1' and TimeC like '%2%'").arg(query.value(0).toString().trimmed()).arg(query.value(3).toString().trimmed());

                int scoreD = score1 * selectScore(sqlD, "DY");
                ticket_model->setItem(i, 3, new QStandardItem(QString::number(scoreD)));

                //智育成绩
                //根据学号查专业
                QString sqls = "select SMajor from STUDENT where StudentNo='" + query.value(0).toString().trimmed() + "'";
                QString smajor = SelectDB(sqls);
                if(smajor == WRONG)
                {
                    QMessageBox::information(this, "提示", WRONG, QMessageBox::Ok);
                    return;
                }
                //根据专业和年级查课程号
                QString sqlc = tr("select ClassNo from Class where SMajor='%1' and Time like '%2%'").arg(smajor).arg(query.value(3).toString().trimmed());
                QSqlQuery query3(db);
                query3.exec(sqlc);
                int scoreZ = 0;
                while(query3.next())
                {
                    //查学科分数
                    QString sqlZ = tr("select Score from SCORE where StudentNo='%1' and ClassNo='%2'").arg(query.value(0).toString().trimmed()).arg(query3.value(0).toString().trimmed());

                    scoreZ += score2 * selectScore(sqlZ, "ZY");
                }
                ticket_model->setItem(i, 4, new QStandardItem(QString::number(scoreZ)));

                //体育成绩
                QString sqlT = tr("select Item from SPORT_ADD_SCORE where StudentNo='%1' and TimeC like '%2%'").arg(query.value(0).toString().trimmed()).arg(query.value(3).toString().trimmed());
                int scoreT = score3 * selectScore(sqlT, "TY");
                ticket_model->setItem(i, 5, new QStandardItem(QString::number(scoreT)));

                //总分
                ticket_model->setItem(i, 6, new QStandardItem(QString::number(scoreD + scoreZ + scoreT)));

                //学期
                ticket_model->setItem(i, 7, new QStandardItem(query.value(3).toString().trimmed()));
            }

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

    if(i == 0)
    {
        ui->label_4->show();
    }
    else
        ui->label_4->hide();
}

void ApplicaterMessage::on_comboBox_currentTextChanged(const QString &arg1)     //专业改变
{
    if(arg1 != "请选择")
    {
        QString sql1 = NULL;
        if(AllMg)       //所有人信息
        {
            if(ui->comboBox_3->currentText() == "请选择")
                sql1 = "select StudentNo from STUDENT where SMajor='" + arg1 + "'";
            else
            {
                QString mintime = checkTime(ui->comboBox_3->currentText().trimmed());
                int maxyear = mintime.left(4).toInt() + 1;
                QString maxtime = QString::number(maxyear) + mintime.right(5);
                sql1 = tr("select StudentNo from STUDENT where SMajor='%1' and ((convert(date, STime) > CONVERT(varchar,'%2')) and (convert(date, STime) < CONVERT(varchar,'%3')))").arg(arg1).arg(mintime).arg(maxtime);
            }
        }
        else
        {
            if(ui->comboBox_2->currentText() == "请选择" && ui->comboBox_3->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where Status='%1'").arg("待审核");
            }
            else if(ui->comboBox_2->currentText() == "请选择" && ui->comboBox_3->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Time='%1' and Status='%2'").arg(ui->comboBox_3->currentText().left(2)).arg("待审核");
            }
            else if(ui->comboBox_2->currentText() != "请选择" && ui->comboBox_3->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Style='%1' and Status='%2'").arg(ui->comboBox_2->currentText()).arg("待审核");
            }
            else if(ui->comboBox_2->currentText() != "请选择" && ui->comboBox_3->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Style='%1' and Time='%2' and Status='%3'").arg(ui->comboBox_2->currentText()).arg(ui->comboBox_3->currentText().left(2)).arg("待审核");
            }
        }

        ShowRecord(sql1, "MA", arg1, NULL, NULL);
    }
}

//类型改变
void ApplicaterMessage::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql1 = NULL;
        if(ui->comboBox->currentText() == "请选择" && ui->comboBox_3->currentText() == "请选择")
        {
            sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where STyle='%1' and Status='%2'").arg(arg1).arg("待审核");
        }
        else if(ui->comboBox->currentText() == "请选择" && ui->comboBox_3->currentText() != "请选择")
        {
            sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Time='%1' and Style='%2' and Status='%3'").arg(ui->comboBox_3->currentText().left(2)).arg(arg1).arg("待审核");
        }
        else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_3->currentText() == "请选择")
        {
            sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where Style='%1' and Status='%2'").arg(arg1).arg("待审核");

            ShowRecord(sql1, "MA", ui->comboBox->currentText(), NULL, NULL);
            return;
        }
        else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_3->currentText() != "请选择")
        {
            sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Style='%1' and Time='%2' and Status='%3'").arg(arg1).arg(ui->comboBox_3->currentText().left(2)).arg("待审核");

            ShowRecord(sql1, "MA", ui->comboBox->currentText(), NULL, NULL);
            return;
        }

        ShowRecord(sql1, "ALL", NULL, NULL, NULL);
    }
}

//年级改变
void ApplicaterMessage::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql1 = NULL;
        if(AllMg)       //所有人信息
        {
            QString mintime = checkTime(arg1);
            int maxyear = mintime.left(4).toInt() + 1;
            QString maxtime = QString::number(maxyear) + mintime.right(5);

            if(ui->comboBox->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo from STUDENT where ((convert(date, STime) > CONVERT(varchar,'%1')) and (convert(date, STime) < CONVERT(varchar,'%2')))").arg(mintime).arg(maxtime);
            }
            else
            {
                sql1 = tr("select StudentNo from STUDENT where SMajor='%1' and ((convert(date, STime) > CONVERT(varchar,'%2')) and (convert(date, STime) < CONVERT(varchar,'%3')))").arg(ui->comboBox->currentText().trimmed()).arg(mintime).arg(maxtime);
            }
        }
        else
        {
            if(ui->comboBox->currentText() == "请选择" && ui->comboBox_2->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where Time='%1' and Status='%2'").arg(arg1.left(2)).arg("待审核");
            }
            else if(ui->comboBox->currentText() == "请选择" && ui->comboBox_2->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Time='%1' and Style='%2' and Status='%3'").arg(arg1.left(2)).arg(ui->comboBox_2->currentText()).arg("待审核");
            }
            else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_2->currentText() == "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where Time='%1' and Status='%2'").arg(arg1.left(2)).arg("待审核");
                ShowRecord(sql1, "MA", ui->comboBox->currentText(), NULL, NULL);
                return;
            }
            else if(ui->comboBox->currentText() != "请选择" && ui->comboBox_2->currentText() != "请选择")
            {
                sql1 = tr("select StudentNo, Style, Status, Time from APPLICATION where  Style='%1' and Time='%2' and Status='%3'").arg(ui->comboBox_2->currentText()).arg(arg1.left(2)).arg("待审核");
                ShowRecord(sql1, "MA", ui->comboBox->currentText(), NULL, NULL);
                return;
            }
        }

        ShowRecord(sql1, "ALL", NULL, NULL, NULL);
    }
}

void ApplicaterMessage::on_comboBox_4_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(arg1.contains("主序"))     //说明是查看总成绩
        {
            model = ui->tableView->model();
            if(arg1 == "总分主序")
            {
                model->sort(6, Qt::DescendingOrder);
            }
            else if(arg1 == "德育主序")
            {
                model->sort(3, Qt::DescendingOrder);
            }
            else if(arg1 == "智育主序")
            {
                model->sort(4, Qt::DescendingOrder);
            }
            else if(arg1 == "体育主序")
            {
                model->sort(5, Qt::DescendingOrder);
            }
        }
        else    //个人成绩明细
        {
            QString sql = NULL;
            if(arg1 == "德育成绩")
            {
                sql = tr("select Item, Expres, TimeC, TableName from MORALISM_ADD_SCORE where StudentNo='%1' and TimeC like '%2%'").arg(stuNo).arg(grades);
                setScore(sql, "DY", NULL);
                return;
            }
            else if(arg1 == "智育成绩")
            {
                sql = "select ClassNo, Score from SCORE where StudentNo='" + stuNo + "'";
                setScore(sql, "ZY", NULL);
                return;
            }
            else if(arg1 == "体育成绩")
            {
                sql = tr("select Item, Expre, TimeC from SPORT_ADD_SCORE where StudentNo='%1' and TimeC like '%2%'").arg(stuNo).arg(grades);
                setScore(sql, "TY", NULL);
                return;
            }
        }
    }
}

void ApplicaterMessage::on_pushButton_clicked()     //查看提交资料
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();

    QString major = ui->comboBox->currentText();    //专业
    QString style = ui->comboBox_2->currentText();  //类型
    QString grade = ui->comboBox_3->currentText();  //年级
    QString score = ui->comboBox_4->currentText();  //排序，分数类型

    static bool tf = true;
    if(tf)          //查看资料/成绩明细
    {
        QModelIndex index = model->index(curow,0);
        QVariant data = model->data(index);
        QString recd = data.toString().trimmed();   //获得选中行的学号

        QModelIndex index1 = model->index(curow,7);
        QVariant data1 = model->data(index1);
        QString recd1 = data1.toString().trimmed();   //获得选中行的学期

        if(recd.isEmpty())
        {
            QMessageBox::information(this, "提示", "请选择学生", QMessageBox::Ok);
            return;
        }

        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);

        ui->pushButton->setText("返回所有记录");
        stuNo = recd;       //进入提交资料界面时如果查照片，默认查该学生
        grades = recd1;      //记录选中学期

        if(MgSr)        //申请人信息
        {
            if(ui->comboBox_3->currentText() == "请选择")
                setPersonMessage(recd, recd1);
            else
                setPersonMessage(recd, recd1);
        }
        else
        {
            ui->label_5->setText("选择类型：");

            ui->comboBox_4->clear();
            QString style = "请选择,德育成绩,智育成绩,体育成绩";
            QStringList styles = style.split(",", QString::SkipEmptyParts);
            ui->comboBox_4->addItems(styles);

            setScore(recd, NULL, recd1);       //不做查询
        }

        tf = false;
    }
    else
    {
        stuNo.clear();          //退出资料查看界面后清空进入前学号
        grades.clear();

        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);

        if(!MgSr)
        {
            ui->pushButton->setText("查看成绩明细");

            ui->comboBox_4->clear();
            QString mainkey = "请选择,总分主序,德育主序,智育主序,体育主序";
            QStringList mains = mainkey.split(",", QString::SkipEmptyParts);
            ui->comboBox_4->addItems(mains);
        }
        else
        {
            ui->pushButton->setText("查看提交资料");
        }

        if(major == "请选择" && style == "请选择" && grade == "请选择")
        {
            setRecord();
        }
        else if(major != "请选择")
        {
            on_comboBox_currentTextChanged(major);
        }
        else if(style != "请选择")
        {
            on_comboBox_2_currentTextChanged(style);
        }
        else if(grade != "请选择")
        {
            on_comboBox_3_currentTextChanged(grade);
        }
        else if(score != "总分排序")
        {

        }

        tf = true;
    }
}

void ApplicaterMessage::on_pushButton_2_clicked()
{
    stuNo.clear();
    grades.clear();

    ui->comboBox->setCurrentText("请选择");
    ui->comboBox_2->setCurrentText("请选择");
    ui->comboBox_3->setCurrentText("请选择");
    ui->comboBox_4->setCurrentText("请选择");

    ui->comboBox->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->comboBox_4->setEnabled(true);

    ui->pushButton->show();
    ui->pushButton_3->show();
    ui->pushButton_4->show();

    this->close();
}

void ApplicaterMessage::on_pushButton_3_clicked()       //取消某人资格
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //获得选中行的学号

    if(recd.isEmpty())
    {
        QMessageBox::information(this, "提示", "请选择学生", QMessageBox::Ok);
        return;
    }

    studentNo = recd;

    PasswSure *paw = new PasswSure(this);
    connect(this, SIGNAL(sendSureToPass()), paw, SLOT(closes()));
    connect(this, SIGNAL(sendTeacher()), paw, SLOT(recvTeachOut()));

    paw->setWindowTitle("密码确认");
    paw->setModal(true);
    paw->show();
}

void ApplicaterMessage::timeOut()
{
    if(!PAW.isEmpty())
    {
        QString sql = "select TPassword from MANAGER where TeacherNo='" + teacherNo + "'";
        QString passw = SelectDB(sql);
        if(PAW.compare(passw) == 0)
        {
            setPriority();
        }
        else
            QMessageBox::information(this, "提示", "密码错误", QMessageBox::Ok);
        PAW.clear();
        return;
    }
    else
       emit sendTeacher();

    if(DScore > 0)
    {
        score1 = DScore;
        score2 = ZScore;
        score3 = TScore;

        DScore = 0;
        ZScore = 0;
        TScore = 0;

        setRecord();
    }
}

void ApplicaterMessage::setPriority()      //取消某人资格
{
    QString sql = tr("update STUDENT set SPriority=%1 where StudentNo='%2'").arg(0).arg(studentNo);
    QString wrong = SelectDB(sql);
    if(wrong == WRONG)
    {
        //将该学生待审核记录全部改为资格取消
        QString sql1 = tr("update APPLICATION set Status='%1' where StudentNo='%2' and Status='%3'").arg("资格已取消").arg(studentNo).arg("待审核");

        QString up = SelectDB(sql1);
        if(up != WRONG)
        {
            QString sql = tr("update STUDENT set SPriority=%1 where StudentNo='%2'").arg(1).arg(studentNo);
            SelectDB(sql);

            QMessageBox::information(this, "提示", "取消失败!", QMessageBox::Ok);
            return;
        }

        QMessageBox::information(this, "提示", "取消成功!", QMessageBox::Ok);
        emit sendSureToPass();

        setRecord();        //刷新信息
    }
    else
    {
        QMessageBox::information(this, "提示", "取消失败!", QMessageBox::Ok);
    }
}

QString ApplicaterMessage::checkTime(QString grade)         //检测学生当前学期
{
    QDate date = QDate::currentDate();
    QString curtime = date.toString("yyyy-MM-dd");
    int month = curtime.mid(5,2).toInt();           //当前月份

    QString schoolT = NULL;
    QString sql = tr("select year from GRADE where class like '%1%'").arg(grade.left(2));
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        int year = curtime.left(4).toInt() - query.value(0).toInt();
        if(month >= 9)
        {
            schoolT = QString::number(year) + "-" + QString::number(07) + curtime.right(3);
            break;
        }
        schoolT = QString::number(year) + "-" + QString::number(07) + curtime.right(3);
    }

    return schoolT;
}

void ApplicaterMessage::on_pushButton_4_clicked()       //查看照片
{
    if(stuNo.isEmpty())
    {
        model = ui->tableView->model();
        int curow = ui->tableView->currentIndex().row();
        QModelIndex index = model->index(curow,0);
        QVariant data = model->data(index);
        QString recd = data.toString().trimmed();   //获得选中行的学号

        if(recd.isEmpty())
        {
            QMessageBox::information(this, "提示", "请选择学生", QMessageBox::Ok);
            return;
        }

        stuNo = recd;
    }

    photo *ph = new  photo(this);
    connect(this, SIGNAL(sendDBPhoto(QSqlDatabase,QString, QString)), ph, SLOT(recvDBStuNo(QSqlDatabase,QString, QString)));

    emit sendDBPhoto(db, stuNo, grades);

    ph->setWindowTitle("照片资料");
    ph->setModal(true);
    ph->show();
}
