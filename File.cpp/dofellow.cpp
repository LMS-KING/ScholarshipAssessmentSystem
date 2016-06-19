#include "dofellow.h"
#include "ui_dofellow.h"
#include <QMessageBox>
#include <QDateTime>
#define WRONG "数据库访问错误!"

DoFellow::DoFellow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DoFellow)
{
    ui->setupUi(this);

    ui->label_4->hide();

    ticket_model = new QStandardItemModel();
    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
    ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("申请奖学金类型")));
    ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("状态")));
    ticket_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("申请时间")));

    //设置时间显示列的宽度
    //ui->tableView->setColumnWidth(2,145);
    //ui->tableView->setColumnWidth(3,145);

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);
    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);
}

DoFellow::~DoFellow()
{
    delete ui;
}

void DoFellow::Message(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

void DoFellow::recvMessage(QSqlDatabase thdb, QString No)
{
    db = thdb;
    TeacNo = No;

    setcomBox();
    setRecord();
}

void DoFellow::setcomBox()
{
    static bool tf = true;
    if(tf)
    {
        QString major = "请选择,软件工程,网络工程,通信工程,计算机科学技术";
        QStringList majors = major.split(",", QString::SkipEmptyParts);
        ui->comboBox->addItems(majors);

        QString sql1 = "select class from GRADE";
        QString grade = "请选择," + selectDB(sql1);
        QStringList grades = grade.split(",", QString::SkipEmptyParts);
        ui->comboBox_2->addItems(grades);

        QString sql = "select Style from FELLOWSHIP";
        QString style = "请选择," + selectDB(sql);
        QStringList styles = style.split(",", QString::SkipEmptyParts);
        ui->comboBox_3->addItems(styles);

        tf = false;
    }

    ui->comboBox->setCurrentText("请选择");
    ui->comboBox_2->setCurrentText("请选择");
    ui->comboBox_3->setCurrentText("请选择");
}

void DoFellow::setRecord()
{
    QString sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1'").arg("待审核");

    showRecord(sql, NULL);
}

QString DoFellow::selectDB(QString sql)
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

void DoFellow::showRecord(QString sql, QString major)
{
    model = ui->tableView->model();
    model->removeRows(0, model->rowCount());

    int i = 0;
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        QString sql1 = NULL;
        if(major.isEmpty())
            sql1 = "select SName, SMajor from STUDENT where StudentNo='" + query.value(0).toString().trimmed() + "'";
        else
            sql1 = tr("select SName, SMajor from STUDENT where StudentNo='%1' and SMajor='%2'").arg(query.value(0).toString().trimmed()).arg(major);

        QSqlQuery query1(db);
        query1.exec(sql1);
        while(query1.next())
        {
            ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
            ticket_model->setItem(i, 1, new QStandardItem(query1.value(0).toString().trimmed()));
            ticket_model->setItem(i, 2, new QStandardItem(query1.value(1).toString().trimmed()));
            ticket_model->setItem(i, 3, new QStandardItem(query.value(1).toString().trimmed()));
            ticket_model->setItem(i, 4, new QStandardItem(query.value(3).toString().trimmed()));
            ticket_model->setItem(i, 5, new QStandardItem(query.value(2).toString().trimmed()));

            ticket_model->item(i, 4)->setForeground(QBrush(QColor(0,255,0)));

            //设置单元格文本居中
            ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(i, 4)->setTextAlignment(Qt::AlignCenter);
            ticket_model->item(i, 5)->setTextAlignment(Qt::AlignCenter);

            i++;
        }
    }

    if(i == 0)
        ui->label_4->show();
    else
        ui->label_4->hide();
}

//专业改变
void DoFellow::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql = NULL;
        if(ui->comboBox_2->currentText().trimmed() == "请选择" && ui->comboBox_3->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1'").arg("待审核");
        }
        else if(ui->comboBox_2->currentText().trimmed() != "请选择" && ui->comboBox_3->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Time='%2'").arg("待审核").arg(ui->comboBox_2->currentText().left(2).trimmed());
        }
        else if(ui->comboBox_2->currentText().trimmed() == "请选择" && ui->comboBox_3->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Style='%2'").arg("待审核").arg(ui->comboBox_3->currentText().trimmed());
        }
        else if(ui->comboBox_2->currentText().trimmed() != "请选择" && ui->comboBox_3->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Time='%2' and Style='%3'").arg("待审核").arg(ui->comboBox_2->currentText().left(2).trimmed()).arg(ui->comboBox_3->currentText().trimmed());
        }

        showRecord(sql, arg1);
    }
}

//年级改变
void DoFellow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql = NULL;
        if(ui->comboBox->currentText().trimmed() == "请选择" && ui->comboBox_3->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Time='%2%='").arg("待审核").arg(arg1.left(2));
        }
        else if(ui->comboBox->currentText().trimmed() != "请选择" && ui->comboBox_3->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Time='%2'").arg("待审核").arg(arg1.left(2));

            showRecord(sql, ui->comboBox->currentText().trimmed());
            return;
        }
        else if(ui->comboBox->currentText().trimmed() == "请选择" && ui->comboBox_3->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Style='%2' and Time='%3'").arg("待审核").arg(ui->comboBox_3->currentText().trimmed()).arg(arg1.left(2));
        }
        else if(ui->comboBox->currentText().trimmed() != "请选择" && ui->comboBox_3->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Time='%2' and Style='%3'").arg("待审核").arg(arg1.left(2)).arg(ui->comboBox_3->currentText().trimmed());

            showRecord(sql, ui->comboBox->currentText().trimmed());
            return;
        }

        showRecord(sql, NULL);
    }
}

//类型改变
void DoFellow::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        QString sql = NULL;
        if(ui->comboBox->currentText().trimmed() == "请选择" && ui->comboBox_2->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Style='%2'").arg("待审核").arg(arg1);
        }
        else if(ui->comboBox->currentText().trimmed() != "请选择" && ui->comboBox_2->currentText().trimmed() == "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Style='%2'").arg("待审核").arg(arg1);

            showRecord(sql, ui->comboBox->currentText().trimmed());
            return;
        }
        else if(ui->comboBox->currentText().trimmed() == "请选择" && ui->comboBox_2->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Style='%2' and Time'%3'").arg("待审核").arg(arg1).arg(ui->comboBox_2->currentText().left(2).trimmed());
        }
        else if(ui->comboBox->currentText().trimmed() != "请选择" && ui->comboBox_2->currentText().trimmed() != "请选择")
        {
            sql = tr("select StudentNo, Style, Time, Status from APPLICATION where Status='%1' and Time='%2' and Style='%3'").arg("待审核").arg(ui->comboBox_2->currentText().left(2).trimmed()).arg(arg1);

            showRecord(sql, ui->comboBox->currentText().trimmed());
            return;
        }

        showRecord(sql, NULL);
    }
}

void DoFellow::on_pushButton_2_clicked()
{
    this->close();
}

//确定发放
void DoFellow::on_pushButton_clicked()
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

    if(recd.isEmpty())
    {
        Message("请选择学生");
        return;
    }

    //当前时间
    QDateTime curtime = QDateTime::currentDateTime();
    QString nowtime = curtime.toString("yyyy-MM-dd");

    QString sql = "select EndTime from FELLOWSHIP where Style='" + recd2 + ",'";
    QString time = selectDB(sql);
    if(time == WRONG)
    {
        Message(WRONG);
        return;
    }

    if(time.compare(nowtime) > 0)
    {
        Message("该类型奖学金申请暂未截止，不可发放!");
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
        if(number == WRONG)
        {
            Message("该专业没有该类型奖学金名额!");
            return;
        }
        if(upFellow(recd2, recd, recd3, recd1, number))
        {
            Message("操作成功!");
            setRecord();
            return;
        }
    }
}

//更新奖学金表名额
bool DoFellow::upFellow(QString style, QString stu, QString time, QString major, QString number)
{
    QString sql = tr("select Count from FELLOWSHIP where Style='%1,'").arg(style);
    QString count = selectDB(sql);
    if(count == WRONG)
    {
        Message(WRONG);
        return false;
    }

    if(count.toInt() <= 0)
    {
        Message("该类型名额已达上限，请重新操作!");
        return false;
    }

    if(count.toInt() == 1)
    {
        if(number != "0000")
        {
            if(!upAssign2(stu, style, time, number, major))
            {
                return false;
            }

//            //更新申请记录状态,调用upApplication，更新一个之外的所有其他为未批准
//            if(!upApplication(stu, style, time, "ALL", NULL))
//            {
//                Message("操作失败!");
//                return false;
//            }
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

//            //调用upApplication，更新一个为已批准
//            if(!upApplication(stu, style, time, "ONE", NULL))
//            {
//                return false;
//            }
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

    QString sql1 = tr("update FELLOWSHIP set Count=Count-1 where Style='%1,'").arg(style);
    QString up = selectDB(sql1);
    if(up != WRONG)
    {
        Message(WRONG);
        return false;
    }

    return true;
}

//更新奖学金分配表
bool DoFellow::upAssign2(QString stuNo, QString style, QString time, QString number, QString major)
{
    QString sql = tr("select NowCount from ASSIGN2 where Number='%1' and Grade='%2'").arg(number).arg(time);

    QString nowcount = selectDB(sql);
    if(nowcount.toInt() <= 0)
    {
        Message("该类型奖学金该专业名额已用完!");
        return false;
    }

    if(nowcount.toInt() == 1)
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

    QString sql1 = tr("update ASSIGN2 set NowCount=NowCount-1 where Number='%1' and Grade='%2'").arg(number).arg(time);
    QString up = selectDB(sql1);
    if(up != WRONG)
    {
        Message(WRONG);
        return false;
    }

    return true;
}

//检查时间
QString DoFellow::checkTime(QString time, QString stuNo)
{
    int selectyear = time.left(4).toInt();
    int selectmonth = time.mid(5,2).toInt();

    QString sql = "select STime from STUDENT where StudentNo='"+ stuNo +"'";
    QString times = selectDB(sql);
    if(times == WRONG)
    {
        Message(WRONG);
        return WRONG;
    }
    int bgnyear = times.left(4).toInt();        //入学年份
    int bgnmonth = times.mid(5,2).toInt();        //入学月份，默认9

    int year = selectyear - bgnyear;
    QString sql2 = tr("select class from GRADE where year=%1").arg(year);
    QString cla = selectDB(sql2);

    if(cla == NULL)
    {
        Message("请确定你是否仍是在读学生!");
        return false;
    }

    QStringList ql = cla.split(",", QString::SkipEmptyParts);

    QString classs = NULL;
    if(selectmonth >= bgnmonth)
    {
        classs += ql.at(1);
    }
    else
    {
        classs += ql.at(0);
    }

    return classs;
}

//更新申请表,how表示是更新一个还是除了一个之外其他的
bool DoFellow::upApplication(QString stuNo, QString style, QString time, QString how, QString major)
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

        QString sql = tr("update APPLICATION set Status='%1' where StudentNo in(%2) and Style='%3' and Time='%4' and Status='%5'").arg("未批准").arg(students).arg(style).arg(time).arg("待审核");

        QString up = selectDB(sql);
        if(up != WRONG)
            return false;
    }

    QString sql1 = tr("update APPLICATION set Status='%1' where StudentNo='%2' and Style='%3' and Time='%4'").arg("已批准").arg(stuNo).arg(style).arg(time);
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
}

bool DoFellow::upApplication2(QString stuNo, QString style, QString how, QString time)
{
    if(how == "ALL")        //更新除此之外的其他状态为未批准
    {
        QString sql = tr("update APPLICATION set Status='%1' where Status='%2' and Style='%3' and StudentNo not in('%4')").arg("未批准").arg("待审核").arg(style).arg(stuNo);
        QString up = selectDB(sql);
        if(up != WRONG)
        {
            return false;
        }
    }

    QString sql1 = tr("update APPLICATION set Status='%1' where StudentNo='%2' and Style='%3' and Status='%4'").arg("已批准").arg(stuNo).arg(style).arg("待审核");
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

//更新奖励表
bool DoFellow::upAwards(QString stuNo, QString style, QString time)
{
    QString sql = tr("select id from AWARDS where StudentNo='%1' and Style='%2' and Time like '%3%'").arg(stuNo).arg(style).arg(time);
    QString id = selectDB(sql);
    if(id != WRONG)
        return true;

    //当前时间
    QDateTime curtime = QDateTime::currentDateTime();
    QString nowtime = curtime.toString("yyyy-MM-dd");
    QString times = checkTime(nowtime, stuNo);

    //插入操作
    QSqlQuery query(db);
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare("insert into AWARDS(StudentNo,Style,Time) values(?,?,?)");
    query.addBindValue(tr("%1").arg(stuNo));
    query.addBindValue(tr("%1").arg(style));
    query.addBindValue(tr("%1").arg(times));
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    return true;
}


