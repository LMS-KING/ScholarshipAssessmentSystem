#include "alldofellowship.h"
#include "ui_alldofellowship.h"
#include "passwsure.h"
#include <QTimer>
#include <QDateTime>
#define WRONG "数据库访问错误!"

QString PAWF;
bool ADD;
bool CA;

ALLDoFellowship::ALLDoFellowship(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ALLDoFellowship)
{
    ui->setupUi(this);

    ui->label_2->hide();

    fs = new FellowShip(this);
    cm = new ClassMessage(this);
    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString, QString)), fs, SLOT(recvDBteachNo(QSqlDatabase,QString, QString)));
    connect(this, SIGNAL(sendMakeOrAdd(bool)), fs, SLOT(recvMakeOrAdd(bool)));

    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString,QString)), cm, SLOT(recvDB(QSqlDatabase,QString,QString)));
    connect(this, SIGNAL(sendMakeOrAdd(bool)), cm, SLOT(recvchangeOradd(bool)));
}

ALLDoFellowship::~ALLDoFellowship()
{
    delete ui;
}

void ALLDoFellowship::recvDBTNO(QSqlDatabase thdb, QString tno)
{
    db = thdb;
    teacherNo = tno;
}

void ALLDoFellowship::recvFellOrClass(bool ForC, bool lf)
{
    FellOrClass = ForC;
    MgrorStu = lf;

    if(FellOrClass)     //奖学金
    {
        ui->label->setText("所有奖学金信息");
        ui->label_3->show();
        ui->tableView_2->show();
        ui->label_4->show();
        ui->comboBox->show();

        static bool tf = true;
        if(tf)
        {
            QString major = "请选择,软件工程,网络工程,通信工程,计算机科学技术";
            QStringList majors = major.split(",", QString::SkipEmptyParts);
            ui->comboBox->addItems(majors);

            tf = false;
        }
    }
    else
    {
        ui->label->setText("所有课程信息");
        ui->label_3->hide();
        ui->tableView_2->hide();
        ui->label_4->hide();
        ui->comboBox->hide();
    }

    if(MgrorStu)        //学生查询奖学金情况
    {
        ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
    }

    else                //管理员编辑奖学金
    {
        ui->pushButton->show();
        ui->pushButton_2->show();
        ui->pushButton_3->show();
    }

    setRecord();

    QTimer *time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(timeOut()));
    time->start(100);
}

void ALLDoFellowship::setRecord()
{
    QString sql = NULL;
    if(FellOrClass)
        sql = "select Style, Money, StartTime, EndTime, Count from FELLOWSHIP";
    else
        sql = "select ClassNo, ClassName, SMajor, ClassScore, TeacherNo, Time from CLASS";

    ShowRecord(sql, NULL);
}

void ALLDoFellowship::ShowRecord(QString sql, QString style)
{
    if(FellOrClass)
    {
        ticket_model = new QStandardItemModel();
        ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("类型")));
        ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("金额")));
        ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("申请开始时间")));
        ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("申请截止时间")));
        ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("奖励名额")));

        //设置时间显示列的宽度
        ui->tableView->setColumnWidth(2,150);
        ui->tableView->setColumnWidth(3,150);
    }
    else
    {
        ticket_model = new QStandardItemModel();
        ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("课程号")));
        ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("课程名")));
        ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
        ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("学分")));
        ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("教师编号")));
        ticket_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("年级")));

        //设置时间显示列的宽度
        ui->tableView->setColumnWidth(2,145);
        ui->tableView->setColumnWidth(3,145);
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
        ticket_model->setItem(i, 1, new QStandardItem(query.value(1).toString().trimmed()));
        ticket_model->setItem(i, 2, new QStandardItem(query.value(2).toString().trimmed()));
        ticket_model->setItem(i, 3, new QStandardItem(query.value(3).toString().trimmed()));
        ticket_model->setItem(i, 4, new QStandardItem(query.value(4).toString().trimmed()));

       if(FellOrClass)
        {
            ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed().left(query.value(0).toString().length() - 1)));
       }
       else
       {
           ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
           ticket_model->setItem(i, 5, new QStandardItem(query.value(5).toString().trimmed()));

           ticket_model->item(i, 5)->setTextAlignment(Qt::AlignCenter);
       }

       //设置单元格文本居中
       ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
       ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
       ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
       ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
       ticket_model->item(i, 4)->setTextAlignment(Qt::AlignCenter);

       i++;
    }

    if(i == 0)
    {
        ui->label_2->show();
    }
    else
        ui->label_2->hide();
}

void ALLDoFellowship::on_pushButton_clicked()       //编辑信息
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //获得选类型

    int curow1 = ui->tableView->currentIndex().row();
    QModelIndex index1 = model->index(curow1,2);
    QVariant data1 = model->data(index1);
    QString recd1 = data1.toString().trimmed();   //获得选中行开始时间

    if(recd.isEmpty())
    {
        QMessageBox::information(this, "提示", "请选择项目", QMessageBox::Ok);
        return;
    }

    bool MA = true;
    if(FellOrClass)     //奖学金信息
    {
        emit sendMessage(db,recd, recd1);
        emit sendMakeOrAdd(MA);

        fs->setWindowTitle("奖学金");
        fs->setModal(true);
        fs->show();
    }
    else
    {
        emit sendMessage(db,recd, recd1);
        emit sendMakeOrAdd(MA);

        cm->setWindowTitle("课程");
        cm->setModal(true);
        cm->show();
    }
}

QString ALLDoFellowship::selectDB(QString sql)
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

void ALLDoFellowship::settableView_2(QString style, QString major) //设置tableView2，奖学金分配情况
{
    static bool tf = true;
    if(tf)
    {
        ticket_model_2 = new QStandardItemModel();
        ticket_model_2->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("专业")));
        ticket_model_2->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("年级")));
        ticket_model_2->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("总名额")));
        ticket_model_2->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("剩余名额")));

        //设置时间显示列的宽度
        //ui->tableView->setColumnWidth(2,145);
        //ui->tableView->setColumnWidth(3,145);

        //利用setModel()方法将数据模型与QTableView绑定
        ui->tableView_2->setModel(ticket_model_2);
        //整行选择
        ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

         //表不可编辑
        ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView_2->sortByColumn(10,Qt::AscendingOrder);

        tf = false;
    }

    model = ui->tableView_2->model();
    model->removeRows(0, model->rowCount());

    ui->label_3->setText(style + "分配情况：");
    int i = 0;
    QString sql = NULL;
    if(major.isEmpty())
        sql = "select Number,SMajor from ASSIGN where Style='" + style + "'";
    else
        sql = tr("select Number,SMajor from ASSIGN where Style='%1' and SMajor='%2'").arg(style).arg(major);

    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        if(query.value(0).toString().trimmed() == "0000")       //没有分配情况
        {
            QString sql = "select Count from FELLOWSHIP where Style='" + style + ",'";
            QString count = selectDB(sql);
            QMessageBox::information(this, "提示", "该类型不按照专业年级分配，全院名额为：" + count + "剩余名额为：" + count, QMessageBox::Ok);

            break;
        }

        QString sql1 = "select Grade, Count, NowCount from ASSIGN2 where Number='" + query.value(0).toString().trimmed() + "'";
        QSqlQuery query1(db);
        query1.exec(sql1);
        while(query1.next())
        {
            ticket_model_2->setItem(i, 0, new QStandardItem(query.value(1).toString().trimmed()));
            ticket_model_2->setItem(i, 1, new QStandardItem(query1.value(0).toString().trimmed()));
            ticket_model_2->setItem(i, 2, new QStandardItem(query1.value(1).toString().trimmed()));
            ticket_model_2->setItem(i, 3, new QStandardItem(query1.value(2).toString().trimmed()));

            //设置单元格文本居中
            ticket_model_2->item(i, 0)->setTextAlignment(Qt::AlignCenter);
            ticket_model_2->item(i, 1)->setTextAlignment(Qt::AlignCenter);
            ticket_model_2->item(i, 2)->setTextAlignment(Qt::AlignCenter);
            ticket_model_2->item(i, 3)->setTextAlignment(Qt::AlignCenter);

            i++;
        }
    }
}

void ALLDoFellowship::on_pushButton_2_clicked()     //删除
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //获得选类型，课程号

    int curow1 = ui->tableView->currentIndex().row();
    QModelIndex index1 = model->index(curow1,2);
    QVariant data1 = model->data(index1);
    QString recd1 = data1.toString().trimmed();   //获得选中行开始时间，专业

    if(recd.isEmpty())
    {
        QMessageBox::information(this, "提示", "请选择项目", QMessageBox::Ok);
        return;
    }

    if(FellOrClass)
    {
        //查询该奖学金申请截止时间
        QString sql = "select EndTime from FELLOWSHIP where Style='" + recd + ",'";
        QString endtime = selectDB(sql);
        if(endtime == WRONG)
        {
            QMessageBox::information(this, "提示", WRONG, QMessageBox::Ok);
            return;
        }

        QDateTime curtime = QDateTime::currentDateTime();
        QString time = curtime.toString("yyyy-MM-dd");
        if(endtime.compare(time) > 0)
        {
            QMessageBox::information(this, "提示","该奖学金申请暂未截止，不可删除记录!", QMessageBox::Ok);
            return;
        }
     }

    curstyle = recd;
    curStime = recd1;

    PasswSure *paw = new PasswSure(this);
    connect(this,SIGNAL(sendToPass()), paw, SLOT(closes()));
    connect(this, SIGNAL(sendPassSure()), paw, SLOT(recvFellowDe()));

    paw->setWindowTitle("密码确认");
    paw->setModal(true);
    paw->show();
}

void ALLDoFellowship::timeOut()
{
    if(!PAWF.isEmpty())
    {
        QString sql = "select TPassword from MANAGER where TeacherNo='"+ teacherNo +"'";
        QString pass = selectDB(sql);
        if(pass.compare(PAWF) == 0)
        {
            setDelete();
        }
        else
            QMessageBox::information(this, "提示", "密码错误!", QMessageBox::Ok);

        PAWF.clear();
        return;
    }
    else
        emit sendPassSure();

    if(ADD)
    {

       setRecord();
       ADD = false;
    }

    if(CA)
    {
        setRecord();
        CA = false;
    }

    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //选择行类型

    if(style == recd)
        return;
    else
        style = recd;

    if(!recd.isEmpty())
    {
        if(ui->comboBox->currentText() == "请选择")
            settableView_2(style,NULL);
        else
            settableView_2(style,ui->comboBox->currentText().trimmed());
    }
}

void ALLDoFellowship::setDelete()
{
    QString sql1 = NULL;
    if(FellOrClass)
    {
        //更新申请表
        QString sql2 = tr("update APPLICATION set Status='%1' where Style='%2' and (Status='%3' or Status='%4')").arg("未批准").arg(curstyle).arg("待审核").arg("资格已取消");
        QString up = selectDB(sql2);
        if(up != WRONG)
        {
            QMessageBox::information(this, "提示", "操作失败!", QMessageBox::Ok);
            return;
        }

        sql1 = tr("delete from FELLOWSHIP where Style='%1,' and StartTime='%2'").arg(curstyle).arg(curStime);
    }
    else
        sql1 = tr("delete from CLASS where ClassNo='%1' and SMajor='%2'").arg(curstyle).arg(curStime);

    QString del = selectDB(sql1);
    if(del == WRONG)
    {
        QMessageBox::information(this, "提示", "操作成功!", QMessageBox::Ok);
        emit sendToPass();
        setRecord();
    }
    else
        QMessageBox::information(this, "提示", "操作失败!", QMessageBox::Ok);
}



void ALLDoFellowship::on_pushButton_3_clicked()     //增加奖学金
{
    bool MA = false;
    if(FellOrClass)
    {
        emit sendMessage(db, NULL, NULL);
        emit sendMakeOrAdd(MA);

        fs->setWindowTitle("奖学金");
        fs->setModal(true);
        fs->show();
    }
    else
    {
        emit sendMessage(db,NULL, NULL);
        emit sendMakeOrAdd(MA);

        cm->setWindowTitle("课程");
        cm->setModal(true);
        cm->show();
    }
}

void ALLDoFellowship::on_pushButton_4_clicked()
{
    this->close();
}

//选择查看专业分配情况
void ALLDoFellowship::on_comboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        settableView_2(style, arg1);
    }
    else
        settableView_2(style, NULL);
}
