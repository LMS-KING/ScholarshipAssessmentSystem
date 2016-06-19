#include "score.h"
#include "ui_score.h"
#include <QDate>
#include <QMessageBox>
#define WRONG "数据访问出错!"

Score::Score(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Score)
{
    ui->setupUi(this);
    scoreOrM = false;
}

Score::~Score()
{
    delete ui;
}

QString Score::MakeDatabase(QString sql, QString style)
{
    QString message = NULL;
    QSqlQuery query(db);
    query.exec(sql);
    if(style == "SIGNAL")
    {
        if(query.next())
        {
            message = query.value(0).toString();
        }
    }
    else if(style == "MORE")
    {
        while(query.next())
        {
            message += query.value(0).toString().trimmed();
        }
    }
    else
        message = WRONG;

    return message;
}

void Score::recvDbStu(QSqlDatabase thisdb, QString stu)
{
    db = thisdb;
    studenNo = stu;
}

void Score::recvManager(bool tf)
{
    scoreOrM = tf;

    if(tf)
    {
        ui->label->setText("请选择学期：");
        setScore();
        return;
    }

    ui->comboBox->setCurrentText("请选择");
    ui->label->setText("请选择类型：");
    setManager();

}

void Score::setScore()      //设置分数下拉框
{
    ticket_model = new QStandardItemModel();
    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("课程号")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("课程名")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("分数")));
    ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("学分")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);
    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置时间显示列的宽度
    ui->tableView->setColumnWidth(0,120);
    ui->tableView->setColumnWidth(1,120);
    ui->tableView->setColumnWidth(2,120);
    ui->tableView->setColumnWidth(3,120);
     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);

    QDateTime time = QDateTime::currentDateTime();
    QString curtime = time.toString("yyyy-MM-dd");
    int curyear = curtime.left(4).toInt();

    int year = 0;
    QString sql = "select STime from STUDENT where StudentNo='" + studenNo + "'";
    QString startT = MakeDatabase(sql, "SIGNAL");
    if(startT != WRONG)
        year = curyear - startT.left(4).toInt();
    else
    {
        Error(WRONG);
        return;
    }

    QString sql1 = "select class from GRADE where year <=" + QString::number(year);
    QString clas = "请选择," + MakeDatabase(sql1, "MORE");
    if(clas == WRONG)
    {
        Error(WRONG);
        return;
    }

    QStringList years = clas.split(",", QString::SkipEmptyParts);
    ui->comboBox->addItems(years);

}

void Score::setManager()        //设置管理员下拉框
{
    ticket_model = new QStandardItemModel();

    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("编号")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("邮箱")));
    ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("专业")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);
    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置时间显示列的宽度
    ui->tableView->setColumnWidth(0,120);
    ui->tableView->setColumnWidth(1,120);
    ui->tableView->setColumnWidth(2,125);
    ui->tableView->setColumnWidth(3,120);
     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);

    ui->comboBox->insertItem(0, "请选择");
    ui->comboBox->insertItem(1, "管理员");
    ui->comboBox->insertItem(2, "超级管理员");
    ui->comboBox->insertItem(3, "所有管理员");
}

void Score::Error(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}



void Score::on_comboBox_currentTextChanged(const QString &arg1)
{
    model = ui->tableView->model();
    model->removeRows(0, model->rowCount());

    if(scoreOrM)      //查询分数
    {
        if(arg1 != "请选择")
        {
            //查询专业
            QString sql = tr("select SMajor from STUDENT where StudentNo='%1'").arg(studenNo);
            QString major = MakeDatabase(sql, "SIGNAL");
            if(major == WRONG)
            {
                Error(WRONG);
                return;
            }

            int record = 0;
            QString sql1 = tr("select ClassNo, ClassName, ClassScore from CLASS where SMajor='%1' and Time='%2'").arg(major).arg(arg1);
            QSqlQuery query(db);
            query.exec(sql1);
            while(query.next())
            {
                QString sql2 = tr("select Score from SCORE where StudentNo='%1' and ClassNo='%2'").arg(studenNo).arg(query.value(0).toString().trimmed());
                QSqlQuery query1(db);
                query1.exec(sql2);
                if(query1.next())
                {
                    ticket_model->setItem(record, 0, new QStandardItem(query.value(0).toString().trimmed()));
                    ticket_model->setItem(record, 1, new QStandardItem(query.value(1).toString().trimmed()));
                    ticket_model->setItem(record, 2, new QStandardItem(query1.value(0).toString().trimmed()));
                    ticket_model->setItem(record, 3, new QStandardItem(query.value(2).toString().trimmed()));

                    if(query1.value(0).toString().toInt() <= 60)
                        ticket_model->item(record, 2)->setForeground(QBrush(QColor(255, 0, 0)));
                    else if(query1.value(0).toString().toInt() >= 90)
                    {
                        //设置状态单元格为
                        ticket_model->item(record, 2)->setForeground(QBrush(QColor(0,255,0)));
                    }

                    //设置单元格文本居中
                    ticket_model->item(record, 0)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(record, 1)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(record, 2)->setTextAlignment(Qt::AlignCenter);
                    ticket_model->item(record, 3)->setTextAlignment(Qt::AlignCenter);
                }

                record++;
            }

            if(record == 0)
            {
                ui->label_2->show();
            }
            else
                ui->label_2->hide();
            return;
        }

        ui->label_2->show();
        return;
    }

    //查询管理员
    QString sql = NULL;
    if(arg1 != "请选择")
    {
        if(arg1 == "管理员")
            sql = tr("select TeacherNo, TName, TEmail, TMajor from MANAGER where TManager=%1 and TSuperManager=%2").arg(1).arg(0);

        else if(arg1 == "超级管理员")
            sql = tr("select TeacherNo, TName, TEmail, TMajor from MANAGER where TManager=%1 and TSuperManager=%2").arg(1).arg(1);

        else if(arg1 == "所有管理员")
            sql = tr("select TeacherNo, TName, TEmail, TMajor from MANAGER where TManager=%1").arg(1);

        QSqlQuery query(db);
        query.exec(sql);

        int record = 0;
        while(query.next())
        {
            ticket_model->setItem(record, 0, new QStandardItem(query.value(0).toString().trimmed()));
            ticket_model->setItem(record, 1, new QStandardItem(query.value(1).toString().trimmed()));
            ticket_model->setItem(record, 2, new QStandardItem(query.value(2).toString().trimmed()));
            ticket_model->setItem(record, 3, new QStandardItem(query.value(3).toString().trimmed()));

            //设置单元格文本居中
            ticket_model->item(record, 0)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(record, 1)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(record, 2)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(record, 3)->setTextAlignment(Qt::AlignCenter);

            record++;
        }

        if(record == 0)
        {
            ui->label_2->show();
        }
        else
            ui->label_2->hide();
        return;
    }

    ui->label_2->show();
    return;
}

void Score::on_pushButton_clicked()     //返回
{
    this->close();
}
