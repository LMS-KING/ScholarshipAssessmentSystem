#include "applicationfellow.h"
#include "ui_applicationfellow.h"
#include <QTimer>
#include <QDateTime>
#include <QPalette>
#include <QFile>
#include <QProgressBar>
#include <QTextStream>
#define Wrong "操作错误!"
#define Message "请填写项目名称!"
#define CURMessage "当前信息已保存!"
#define BEFORE "请先完成上一个项目信息!"
#define SUCCESSFUL "保存成功!"
#define FAILED "信息不完善!保存失败!"

ApplicationFellow::ApplicationFellow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationFellow)
{
    ui->setupUi(this);

    QTimer *time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(changeTime()));
    time->start(1000);

    //设置表彰信息
    QString bysj = "请选择,校十佳学生,校百优学生";
    QString fifo = "请选择,五四院级优秀团(党)员,五四校级优秀团(党)员,五四省级优秀团(党)员,五四国家级优秀团(党)员";
    QString thregod = "请选择,校级三好学生标兵,校级三好学生,校级优秀学生干部,先进个人";
    QString socialty = "请选择,社会实践及其他非学科竞赛和体育类院级表彰,社会实践及其他非学科竞赛和体育类校级表彰,社会实践及其他非学科竞赛和体育类省级级表彰,社会实践及其他非学科竞赛和体育类国家级表彰";
    QStringList bysjs = bysj.split(",", QString::SkipEmptyParts);
    QStringList fifos = fifo.split(",", QString::SkipEmptyParts);
    QStringList thregods = thregod.split(",", QString::SkipEmptyParts);
    QStringList socialtys = socialty.split(",", QString::SkipEmptyParts);
    ui->comboBox->addItems(bysjs);
    ui->comboBox_2->addItems(fifos);
    ui->comboBox_3->addItems(thregods);
    ui->comboBox_4->addItems(socialtys);

    //年级标志
    classs.clear();
    //表彰时间标志
    time12 = true;
    time34 = true;
    //学科竞赛时间标志
    xktime1 = true;
    xktime2 = true;
    xktime3 = true;
    xktime4 = true;
    xktime5 = true;
    //科技立项时间标志
    kltime1 = true;
    kltime2 = true;
    kltime3 = true;
    //学生会和班级职务时间标志
    sclass1 = true;
    sclass2 = true;
    //证书活动时间标志
    numbers = 0;
    sporttime = 0;
    //选择证书和活动的个数
    //books = 0;
    //activitices = 0;

    fellowstyle = false;

    //开始时隐藏竞赛中的两个
    ui->label_8->hide();
    ui->lineEdit_7->hide();
    ui->comboBox_11->hide();
    ui->label_19->hide();
    ui->Lxkjs4->hide();
    ui->Bxkjs4->hide();
    //
    ui->label_9->hide();
    ui->lineEdit_8->hide();
    ui->comboBox_12->hide();
    ui->label_20->hide();
    ui->Lxkjs5->hide();
    ui->Bxkjs5->hide();
    //设置初始值
    ui->Lxkjs5->setText(Message);

    //开始时隐藏科技立项中的两个
    ui->label_6->hide();
    ui->lineEdit_5->hide();
    ui->comboBox_9->hide();
    ui->label_15->hide();
    ui->Lkjlx2->hide();
    ui->Bkjlx2->hide();
    //
    ui->label_21->hide();
    ui->lineEdit_6->hide();
    ui->comboBox_10->hide();
    ui->label_22->hide();
    ui->Lkjlx3->hide();
    ui->Bkjlx3->hide();

    //隐藏双学位时间开始
    ui->label_31->hide();
    ui->lineEdit_13->hide();
    ui->doubleBt->hide();
    //隐藏献血时间
    ui->label_47->hide();
    ui->lineEdit_25->hide();
    ui->doubleBt_13->hide();

    //隐藏其他证书时间
    ui->label_35->hide();
    ui->label_36->hide();
    ui->label_38->hide();
    ui->label_37->hide();
    ui->label_39->hide();
    ui->lineEdit_15->hide();
    ui->lineEdit_17->hide();
    ui->lineEdit_18->hide();
    ui->lineEdit_19->hide();
    ui->lineEdit_20->hide();
    ui->doubleBt_3->hide();
    ui->doubleBt_4->hide();
    ui->doubleBt_6->hide();
    ui->doubleBt_5->hide();
    ui->doubleBt_7->hide();

    //隐藏活动时间
    ui->label_41->hide();
    ui->label_42->hide();
    ui->label_43->hide();
    ui->label_44->hide();
    ui->label_45->hide();
    ui->lineEdit_16->hide();
    ui->lineEdit_21->hide();
    ui->lineEdit_23->hide();
    ui->lineEdit_24->hide();
    ui->lineEdit_22->hide();
    ui->doubleBt_11->hide();
    ui->doubleBt_12->hide();
    ui->doubleBt_9->hide();
    ui->doubleBt_10->hide();
    ui->doubleBt_8->hide();

    //隐藏CET4,6时间
    ui->label_49->hide();
    ui->lineEdit_26->hide();
    ui->cetbt->hide();

    ui->label_52->hide();
    ui->lineEdit_27->hide();
    ui->cetbt2->hide();

    //隐藏体育赛事信息
    ui->label_60->hide();
    ui->Lxkjs1_5->hide();
    ui->comboBox_23->hide();
    ui->label_59->hide();
    ui->lineEdit_31->hide();
    ui->Bxkjs1_5->hide();

    ui->label_62->hide();
    ui->Lxkjs1_6->hide();
    ui->comboBox_24->hide();
    ui->label_61->hide();
    ui->lineEdit_32->hide();
    ui->Bxkjs1_6->hide();

    //体育工作者
    ui->label_65->hide();
    ui->Lxkjs1_8->hide();
    ui->comboBox_26->hide();
    ui->label_66->hide();
    ui->lineEdit_34->hide();
    ui->Bxkjs1_8->hide();

    ui->label_67->hide();
    ui->Lxkjs1_9->hide();
    ui->comboBox_27->hide();
    ui->label_68->hide();
    ui->lineEdit_35->hide();
    ui->Bxkjs1_9->hide();



    //设置初始值
    ui->Lkjlx2->setText(Message);
    ui->Lkjlx3->setText(Message);

    //开始时时间都是不可选
    //表彰时间
    ui->BTime1->setEnabled(false);
    ui->BTime2->setEnabled(false);
    ui->BTime3->setEnabled(false);
    ui->BTime4->setEnabled(false);
    //学科竞赛时间
    ui->Bxkjs1->setEnabled(false);
    ui->Bxkjs2->setEnabled(false);
    ui->Bxkjs3->setEnabled(false);
    ui->Bxkjs4->setEnabled(false);
    ui->Bxkjs5->setEnabled(false);
    //科技立项时间
    ui->Bkjlx1->setEnabled(false);
    ui->Bkjlx2->setEnabled(false);
    ui->Bkjlx3->setEnabled(false);

    //隐藏体育时间按钮
    ui->Bxkjs1_2->setEnabled(false);
    ui->Bxkjs1_3->setEnabled(false);
    ui->Bxkjs1_4->setEnabled(false);
    ui->Bxkjs1_5->setEnabled(false);
    ui->Bxkjs1_6->setEnabled(false);
    ui->Bxkjs1_7->setEnabled(false);
    ui->Bxkjs1_8->setEnabled(false);
    ui->Bxkjs1_9->setEnabled(false);

    //隐藏日历
    ui->calendarWidget->hide();
    ui->calendarWidget_2->hide();
    ui->tableTime->hide();
    ui->kktimetable->hide();
    ui->stuclasstimetable->hide();
    ui->moretime->hide();
    ui->cettime->hide();
    ui->cettime2->hide();
    ui->sporttime->hide();

    //开始提交按钮灰色
    ui->sure->setEnabled(false);

    //设置焦点
    ui->comboBox_13->setFocus();

    //设置颜色
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_14->setPalette(pe);

    setMessage();       //设置文本说明
}

ApplicationFellow::~ApplicationFellow()
{
    delete ui;
}

void ApplicationFellow::setMessage()        //设置文本说明
{
    //提示职务具体信息,读取TXT文件
    //设置学生会干部类型说明
    QFile file("./message1.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error("Can't open the file1!");
        return;
    }
    QByteArray line = file.readAll();
    QTextCodec *codec = QTextCodec::codecForName("GBK");//指定QString的编码方式
    QString message = codec->toUnicode(line);
    ui->textEdit->setText(message);

    //设置其他证书说明
    QFile file1("./message2.txt");
    if(!file1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error("Can't open the file2!");
        return;
    }
    QByteArray line1 = file1.readAll();
    QTextCodec *codec1 = QTextCodec::codecForName("GBK");//指定QString的编码方式
    QString message1 = codec1->toUnicode(line1);
    ui->otherbook->setText(message1);
    //设置活动说明
    QFile file2("./message3.txt");
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Error("Can't open the file3!");
        return;
    }
    QByteArray line2 = file2.readAll();
    QTextCodec *codec2 = QTextCodec::codecForName("GBK");//指定QString的编码方式
    QString message2 = codec2->toUnicode(line2);
    ui->otherbook_2->setText(message2);
}

void ApplicationFellow::Database(QSqlDatabase sql, QString studentno)  //槽函数，获取已经连接的数据库和当前登录的学生学号
{
    db = sql;
    studentNo = studentno;
    setICM();
}

//这个函数（整个计时器）
void ApplicationFellow::changeTime()        //计时器
{
    //表彰
    if(ui->comboBox->currentText() != "请选择")    //时间按钮可用
    {
        ui->BTime1->setEnabled(true);
    }
    else
    {
        ui->LTime1->clear();
        ui->BTime1->setEnabled(false);
    }

    if(ui->comboBox_2->currentText() != "请选择")    //时间按钮可用
    {
        if(ui->comboBox->currentText() != "请选择" && ui->LTime1->text().isEmpty())    //如果前一个时间没选，不能选择下一项目
        {
            Error("请选择"+ui->comboBox->currentText()+"奖励时间");
            ui->comboBox_2->setCurrentText("请选择");
            return;
        }
        ui->BTime2->setEnabled(true);
    }
    else
    {
        ui->LTime2->clear();
        ui->BTime2->setEnabled(false);
    }

    if(ui->comboBox_3->currentText() != "请选择")    //时间按钮可用
    {
        if(ui->comboBox_2->currentText() != "请选择" && ui->LTime2->text().isEmpty())
        {
            Error("请选择"+ui->comboBox_2->currentText()+"奖励时间");
            ui->comboBox_3->setCurrentText("请选择");
            return;
        }
        ui->BTime3->setEnabled(true);
    }
    else
    {
        ui->LTime3->clear();
        ui->BTime3->setEnabled(false);
    }

    if(ui->comboBox_4->currentText() != "请选择")    //时间按钮可用
    {
        if(ui->comboBox_3->currentText() != "请选择" && ui->LTime3->text().isEmpty())
        {
            Error("请选择"+ui->comboBox_3->currentText()+"奖励时间");
            ui->comboBox_4->setCurrentText("请选择");
            return;
        }
        ui->BTime4->setEnabled(true);
    }
    else
    {
        ui->LTime4->clear();
        ui->BTime4->setEnabled(false);
    }

    //如果选择双学位，显示时间选择
    if(ui->yes->isChecked())
    {
        ui->label_31->show();
        ui->lineEdit_13->show();
        ui->doubleBt->show();
    }
    else
    {
        ui->label_31->hide();
        ui->lineEdit_13->hide();
        ui->doubleBt->hide();
        ui->lineEdit_13->clear();
    }

    //如果选择献血，显示时间选择
    if(ui->yes_2->isChecked())
    {
        ui->label_47->show();
        ui->lineEdit_25->show();
        ui->doubleBt_13->show();
    }
    else
    {
        ui->label_47->hide();
        ui->lineEdit_25->hide();
        ui->doubleBt_13->hide();
        ui->lineEdit_25->clear();
    }

    //CET4
    if(ui->cet4yes->isChecked())
    {
        ui->label_49->show();
        ui->lineEdit_26->show();
        ui->cetbt->show();
    }
    else
    {
        ui->label_49->hide();
        ui->lineEdit_26->hide();
        ui->lineEdit_26->clear();
        ui->cetbt->hide();
    }

    //CET6
    if(ui->cet6yes->isChecked())
    {
        ui->label_52->show();
        ui->lineEdit_27->show();
        ui->cetbt2->show();
    }
    else
    {
        ui->label_52->hide();
        ui->lineEdit_27->hide();
        ui->lineEdit_27->clear();
        ui->cetbt2->hide();
    }
}

void ApplicationFellow::Error(QString message)      //提示函数
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

void ApplicationFellow::setICM()        //设置下各个拉框
{
    static bool tf = true;
    if(!tf)
        return;

    tf = false;
    QString sql = "select Expre from ICM_SCORE";
    QString mess = "请选择," + MakeDatabase("SN", sql);
    if(mess != Wrong)
    {
        messl = mess.split(",", QString::SkipEmptyParts);
        ui->comboBox_5->addItems(messl);
        ui->comboBox_6->addItems(messl);
        ui->comboBox_7->addItems(messl);
        ui->comboBox_11->addItems(messl);
        ui->comboBox_12->addItems(messl);

        QString temp = "请选择," + messl.at(5)+ "," + messl.at(6)+ "," + messl.at(7)+ "," + messl.at(8)+ "," + messl.at(21)+ "," + messl.at(22);
        messk = temp.split(",", QString::SkipEmptyParts);
        ui->comboBox_8->addItems(messk);
        ui->comboBox_9->addItems(messk);
        ui->comboBox_10->addItems(messk);
    }
    else
    {
        Error("数据库访问出错");
        return;
    }

    //设置职务下拉框
    QString sql2 = "select Expre from POSITION_SCORE";
    QString mess1 = "请选择," + MakeDatabase("SN", sql2);
    if(mess1 != Wrong)
    {
        QStringList positon = mess1.split(",", QString::SkipEmptyParts);
        //科技创新平台
        ui->comboBox_14->insertItem(0,positon.at(0));
        ui->comboBox_14->insertItem(1,positon.at(7));
        ui->comboBox_14->insertItem(2,positon.at(8));
        //学生会
        ui->comboBox_15->insertItem(0,positon.at(0));
        ui->comboBox_15->insertItem(1,positon.at(1));
        ui->comboBox_15->insertItem(2,positon.at(2));
        ui->comboBox_15->insertItem(3,positon.at(3));
        ui->comboBox_15->insertItem(4,positon.at(4));
        //班级
        ui->comboBox_16->insertItem(0,positon.at(0));
        ui->comboBox_16->insertItem(1,positon.at(5));
        ui->comboBox_16->insertItem(2,positon.at(6));
    }
    else
    {
        Error("数据库访问出错");
        return;
    }

    //设置证书活动下拉框
    ui->comboBox_17->insertItem(0,"请选择");
    ui->comboBox_17->insertItem(1,"二级/低级");
    ui->comboBox_17->insertItem(2,"三级/中级");
    ui->comboBox_17->insertItem(3,"四级/高级");
    //设置证书活动
    ui->comboBox_18->insertItem(0,"请选择");
    ui->comboBox_18->insertItem(1,"1");
    ui->comboBox_18->insertItem(2,"2");
    ui->comboBox_18->insertItem(3,"3");
    ui->comboBox_18->insertItem(4,"4");
    ui->comboBox_18->insertItem(5,"5");
    ui->comboBox_19->insertItem(0,"请选择");
    ui->comboBox_19->insertItem(1,"1");
    ui->comboBox_19->insertItem(2,"2");
    ui->comboBox_19->insertItem(3,"3");
    ui->comboBox_19->insertItem(4,"4");
    ui->comboBox_19->insertItem(5,"5");

    //设置体育赛事下拉框
    QString sql3 = "select Expre from SPORTS_SCORE";
    QString mess2 = "请选择," + MakeDatabase("SN", sql3);
    if(mess2 != Wrong)
    {
        QStringList priority = mess2.split(",", QString::SkipEmptyParts);
        //工作者，志愿者
        ui->comboBox_25->insertItem(0, priority.at(0));
        ui->comboBox_25->insertItem(1, priority.at(1));
        ui->comboBox_25->insertItem(2, priority.at(2));

        ui->comboBox_26->insertItem(0, priority.at(0));
        ui->comboBox_26->insertItem(1, priority.at(1));
        ui->comboBox_26->insertItem(2, priority.at(2));

        ui->comboBox_27->insertItem(0, priority.at(0));
        ui->comboBox_27->insertItem(1, priority.at(1));
        ui->comboBox_27->insertItem(2, priority.at(2));

        QString sports = NULL;
        sports += priority.at(0) + ",";
        for(int i = 3; i < priority.size(); i++)
        {
            sports += priority.at(i) + ",";
        }

        QStringList sportwon = sports.split(",", QString::SkipEmptyParts);
        ui->comboBox_20->addItems(sportwon);
        ui->comboBox_21->addItems(sportwon);
        ui->comboBox_22->addItems(sportwon);
        ui->comboBox_23->addItems(sportwon);
        ui->comboBox_24->addItems(sportwon);
    }
}

QString ApplicationFellow::MakeDatabase(QString style, QString sql)    //数据库查询操作，查询下拉框奖励数据在数据库中对应的Item
{
    QString message = NULL;
    QSqlQuery query(db);
    query.exec(sql);
    if(style == "SN")       //查询奖励等次
    {
        while(query.next())
        {
            message += query.value(0).toString().trimmed();
        }
    }
    else if(style == "SC")      //查询数据
    {
        if(query.next())
        {
            message = query.value(0).toString().trimmed();
        }
        else
            message = Wrong;
    }
    else
        message = Wrong;

    return message;
}

bool ApplicationFellow::insertDatabase(QString style, QString sql, QString Item, QString time, QString tablename, QString expre, QString timec)     //数据库插入操作：插入学生德育加分表
{
    if(style == "SIGNAL")       //一般每个学年只能插入一个奖励
    {
        //判断数据是否已存在
        QString sql1 = tr("select id from MORALISM_ADD_SCORE where StudentNo='%1' and Item='%2' and TimeC='%3'").arg(studentNo).arg(Item).arg(timec);
        QString isExits = MakeDatabase("SC", sql1);
        if(isExits != Wrong)
            return true;
    }
    else if(style == "MORE")
    {
        //判断证书或活动是否大于等于5
        QString sql1 = tr("select count(Item) from MORALISM_ADD_SCORE where Expres='%1' or Expres='%2'").arg("献血").arg("其他证书");
        QString isExits = MakeDatabase("SC", sql1);
        if(isExits != Wrong && isExits.toInt() >= 5)     //大于5条记录停止插入
            return true;
    }
    else if(style == "OACV")
    {
        //判断证书或活动是否大于等于5
        QString sql1 = tr("select count(Item) from MORALISM_ADD_SCORE where Expres='%1'").arg(expre);
        QString isExits = MakeDatabase("SC", sql1);
        if(isExits != Wrong && isExits.toInt() >= 5)     //大于5条记录停止插入
            return true;
    }

    QSqlQuery query(db);
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare(sql);
    query.addBindValue(tr("%1").arg(studentNo));
    query.addBindValue(tr("%1").arg(Item));
    query.addBindValue(tr("%1").arg(time));
    query.addBindValue(tr("%1").arg(tablename));
    query.addBindValue(tr("%1").arg(expre));
    query.addBindValue(tr("%1").arg(timec));
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    return true;
}

bool ApplicationFellow::insertDatabasetoSport(QString sql, QString Item, QString time, QString expre, QString timec)
{
    //判断数据是否已存在
    QString sql1 = tr("select id from SPORT_ADD_SCORE where StudentNo='%1' and Item='%2' and TimeC='%3'").arg(studentNo).arg(Item).arg(timec);
    QString isExits = MakeDatabase("SC", sql1);
    if(isExits != Wrong)
        return true;

    QSqlQuery query(db);
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare(sql);
    query.addBindValue(tr("%1").arg(studentNo));
    query.addBindValue(tr("%1").arg(Item));
    query.addBindValue(tr("%1").arg(time));
    query.addBindValue(tr("%1").arg(expre));
    query.addBindValue(tr("%1").arg(timec));
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    return true;
}


void ApplicationFellow::on_save1_clicked()      //保存学生表彰信息
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类型");
        return;
    }

    if(ui->comboBox->currentText() != "请选择")
    {
        if(ui->LTime1->text().isEmpty())
        {
            Error("请选择"+ui->comboBox->currentText()+"奖励时间");
            return;
        }
    }
    if(ui->comboBox_2->currentText() != "请选择")
    {
        if(ui->LTime2->text().isEmpty())
        {
            Error("请选择"+ui->comboBox_2->currentText()+"奖励时间");
            return;
        }
    }
    if(ui->comboBox_3->currentText() != "请选择")
    {
        if(ui->LTime3->text().isEmpty())
        {
            Error("请选择"+ui->comboBox_3->currentText()+"奖励时间");
            return;
        }
    }
    if(ui->comboBox_4->currentText() != "请选择")
    {
        if(ui->LTime4->text().isEmpty())
        {
            Error("请选择"+ui->comboBox_4->currentText()+"奖励时间");
            return;
        }
    }

    //获表彰
    QString temp1 = ui->comboBox->currentText() + ","
    + ui->comboBox_2->currentText() + ","
    + ui->comboBox_3->currentText() + ","
    + ui->comboBox_4->currentText();
    QStringList ql = temp1.split(",", QString::SkipEmptyParts);

    //时间
    QString time1 = ui->LTime1->text() + ","
    + ui->LTime2->text() + ","
    + ui->LTime3->text() + ","
    + ui->LTime4->text();
    QStringList time2 = time1.split(",", QString::SkipEmptyParts);

    //年级信息
    static QStringList grade;
    if(!classs.isEmpty())
        grade += classs.split(",", QString::SkipEmptyParts);

    int savecount = 0;
    int count = ql.size();
    int counts = time2.size() - 1;
    while(count--)
    {
        if(counts < 0)
            break;

        if(ql.at(count) != QString("请选择"))
        {
            QString sql1 = "select Item from HOUOR_SCORE where Expre='"+ql.at(count)+"'";
            QString item = MakeDatabase("SC", sql1);

            if(item == Wrong)
            {
                Error(FAILED);
                return;
            }

            QString sql2 = "insert into MORALISM_ADD_SCORE(StudentNo,Item,Time,TableName,Expres,TimeC) values(?,?,?,?,?,?)";
            if(insertDatabase("SIGNAL", sql2, item,time2.at(counts), "HOUOR_SCORE", ql.at(count), grade.at(counts)))
            {
                savecount++;
                counts--;
            }
        }
    }
    classs.clear();

    if(savecount > 0)
    {
        Error(SUCCESSFUL);
    }
    else
    {
        Error(FAILED);
    }
}

void ApplicationFellow::on_BTime1_clicked()     //表彰第一个时间按钮
{
    ui->calendarWidget->show();
    time12 = true;
}

void ApplicationFellow::on_BTime2_clicked()     //表彰第二个时间按钮
{
    ui->calendarWidget->show();
    time12 = false;
}

void ApplicationFellow::on_BTime3_clicked()     //表彰第三个时间按钮
{
   ui->calendarWidget_2->show();
   time34 = true;
}

void ApplicationFellow::on_BTime4_clicked()     //表彰第四个时间按钮
{
    ui->calendarWidget_2->show();
    time34 = false;
}

void ApplicationFellow::on_calendarWidget_clicked(const QDate &date) //表彰第一个日历
{
    QString selectTime = date.toString("yyyy-MM-dd");
    if(checkTime("OTP", selectTime))
    {
        if(time12)
        {
            ui->LTime1->setText(selectTime);
        }
        else
        {
            ui->LTime2->setText(selectTime);
        }
        ui->calendarWidget->hide();
    }
}

void ApplicationFellow::on_calendarWidget_2_clicked(const QDate &date)  //表彰第二个日历
{
    QString selectTime = date.toString("yyyy-MM-dd");
    if(checkTime("OTP", selectTime))
    {
        if(time34)
        {
            ui->LTime3->setText(selectTime);
        }
        else
        {
            ui->LTime4->setText(selectTime);
        }
        ui->calendarWidget_2->hide();
    }
}

bool ApplicationFellow::checkTime(QString style, QString selectT)
{
    //所选择的时间
    int selectyear = selectT.left(4).toInt();
    int selectmonth = selectT.mid(5,2).toInt();

    //入学时间
    QString sql = "select STime from STUDENT where StudentNo='"+ studentNo +"'";
    QString time = MakeDatabase("SC", sql);
    if(time == Wrong)
    {
        Error("数据库访问出错!");
        return false;
    }
    int bgnyear = time.left(4).toInt();        //入学年份
    int bgnmonth = time.mid(5,2).toInt();        //入学月份，默认9

    //系统当前时间
    QDateTime curtime = QDateTime::currentDateTime();   //获取系统现在的时间
    QString strTime = curtime.toString("yyyy-MM-dd");      //设置系统时间显示格式
    int curyear = strTime.left(4).toInt();   //当前年份
    int curmonth = strTime.mid(5,2).toInt();    //当前月份

    //申请时间必须在当前时间之前
    if(selectT.compare(strTime) > 0)
    {
        Error("奖励时间只能为已经度过的大学任意时间!当前时间为："+ strTime);
        return false;
    }

    if(time.compare(selectT) >= 0)
    {
        Error("入职时间须在入学时间之后!你的入学时间为："+ time);
        return false;
    }

    if(style == "OTP")      //一次性加分项必须为过去一学年时间
    {
        //限定申请时间只能为一个学年
        if(curmonth < bgnmonth)     //当前月份小于开学月份，时间可以为当前时间之前两年
        {
            if(selectyear < curyear - 2 || selectyear > curyear - 1)//时间限定
            {
                Error("奖励时间只能为过去一学年的任意时间!当前时间为："+ strTime);
                return false;
            }
            if(selectyear == curyear - 2)       //月份必须在正常开学时间之后
            {
                if(selectmonth < bgnmonth)
                {
                    Error("奖励时间只能为过去一学年的任意时间!当前时间为："+ strTime);
                    return false;
                }
            }
            if(selectyear == curyear - 1)
            {
                if(selectmonth >= bgnmonth)
                {
                    Error("奖励时间只能为过去一学年的任意时间!当前时间为："+ strTime);
                    return false;
                }
            }
        }
        else
        {
            if(selectyear < curyear - 1)        //时间限定在当前时间之前一年
            {
                Error("奖励时间只能为过去一学年的任意时间!当前时间为："+ strTime);
                return false;
            }
            if(selectyear == curyear - 1)
            {
                if(selectmonth < bgnmonth)
                {
                    Error("奖励时间只能为过去一学年的任意时间!当前时间为："+ strTime);
                    return false;
                }
            }
            else
            {
                if(selectmonth >= bgnmonth)
                {
                    Error("奖励时间只能为过去一学年的任意时间!当前时间为："+ strTime);
                    return false;
                }
            }
        }
    }

    int year = selectyear - bgnyear;
    QString sql1 = tr("select class from GRADE where year=%1").arg(year);
    QString cla = MakeDatabase("SN", sql1);

    if(cla == Wrong)
    {
        Error("请确定你是否仍是在读学生!");
        return false;
    }

    QStringList ql = cla.split(",", QString::SkipEmptyParts);

    if(selectmonth >= bgnmonth)
    {
        classs += ql.at(1) + ",";
    }
    else
    {
        classs += ql.at(0) + ",";
    }

    return true;
}

//检查当前学期
QString ApplicationFellow::curGrade(QString time)
{
    int selectyear = time.left(4).toInt();
    int selectmonth = time.mid(5,2).toInt();

    QString sql = "select STime from STUDENT where StudentNo='"+ studentNo +"'";
    QString times = MakeDatabase("SC", sql);
    if(times == Wrong)
    {
        Error(Wrong);
        return Wrong;
    }
    int bgnyear = times.left(4).toInt();        //入学年份
    int bgnmonth = times.mid(5,2).toInt();        //入学月份，默认9

    int year = selectyear - bgnyear;
    QString sql2 = tr("select class from GRADE where year=%1").arg(year);
    QString cla = MakeDatabase("SN", sql2);

    if(cla == Wrong)
    {
        Error("请确定你是否仍是在读学生!");
        return Wrong;
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

void ApplicationFellow::recvStyle(QString style)        //奖学金类别下拉框
{
    ui->comboBox_13->clear();
    QString styles = "请选择," + style;
    QStringList stylelist = styles.split(",", QString::SkipEmptyParts);
    ui->comboBox_13->addItems(stylelist);
}

void ApplicationFellow::on_pushButton_clicked()     //返回按钮
{
    this->close();
}

void ApplicationFellow::on_comboBox_13_currentTextChanged(const QString &arg1)//文本改变
{
    if(arg1 != "请选择")
    {
        //系统当前时间
        QDateTime curtime = QDateTime::currentDateTime();   //获取系统现在的时间
        QString strTime = curtime.toString("yyyy-MM-dd");      //设置系统时间显示格式

        QString grade = curGrade(strTime);
        if(grade != Wrong)
        {
            QString style = arg1;
            QString sql = tr("select id from APPLICATION where StudentNo='%1' and Style='%2' and Time='%3'").arg(studentNo).arg(style).arg(grade.left(2));
            QString id = MakeDatabase("SC", sql);
            if(id == Wrong)
            {
                ui->label_14->setText("可以申请");
                fellowstyle = true;
                classs.clear();
                return;
            }
            else
            {
                ui->label_14->setText("该类型奖学金你已经申请!你可以取消之前申请然后再次申请!");
                ui->sure->setEnabled(false);
                classs.clear();
            }
        }
        else
        {
            ui->label_14->setText("时间错误!");
        }
    }
    else
    {
        ui->label_14->setText("请选择申请奖学金类型");
    }

    fellowstyle = false;
}

//学科竞赛下拉框内容改变事件
void ApplicationFellow::on_comboBox_5_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->lineEdit->text().isEmpty())
        {
            Error(Message);
            ui->comboBox_5->setCurrentText("请选择");
            ui->Bxkjs1->setEnabled(false);
            return;
        }
        else
            ui->Bxkjs1->setEnabled(true);
    }
    else
    {
        ui->lineEdit->clear();
        ui->Lxkjs1->clear();
        ui->Bxkjs1->setEnabled(false);
    }
}

void ApplicationFellow::on_comboBox_6_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_5->currentText() != "请选择" && ui->Lxkjs1->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->lineEdit_2->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
             ui->Bxkjs2->setEnabled(true);
             return;
        }

        ui->comboBox_6->setCurrentText("请选择");
    }
    else
    {
        ui->lineEdit_2->clear();
        ui->Lxkjs2->clear();
    }

    ui->Bxkjs2->setEnabled(false);
}

void ApplicationFellow::on_comboBox_7_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_6->currentText() != "请选择" && ui->Lxkjs2->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->lineEdit_3->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
             ui->Bxkjs3->setEnabled(true);
             return;
        }

        ui->comboBox_7->setCurrentText("请选择");
    }
    else
    {
        ui->lineEdit_3->clear();
        ui->Lxkjs3->clear();
    }

    ui->Bxkjs3->setEnabled(false);
}

void ApplicationFellow::on_comboBox_11_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_6->currentText() != "请选择" && ui->Lxkjs3->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->lineEdit_7->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
             ui->Bxkjs4->setEnabled(true);
             return;
        }

        ui->comboBox_11->setCurrentText("请选择");
    }
    else
    {
        ui->lineEdit_7->clear();
        ui->Lxkjs4->clear();
    }

    ui->Bxkjs4->setEnabled(false);
}

void ApplicationFellow::on_comboBox_12_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_11->currentText() != "请选择" && ui->Lxkjs4->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->lineEdit_8->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
             ui->Bxkjs5->setEnabled(true);
             return;
        }

        ui->comboBox_12->setCurrentText("请选择");
    }
    else
    {
        ui->lineEdit_8->clear();
        ui->Lxkjs5->clear();
    }

    ui->Bxkjs5->setEnabled(false);
}

void ApplicationFellow::on_comboBox_8_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->lineEdit_4->text().isEmpty())
        {
            Error(Message);
            ui->comboBox_8->setCurrentText("请选择");
        }
        else
        {
             ui->Bkjlx1->setEnabled(true);
             return;
        }
    }
    else
    {
        ui->lineEdit_4->clear();
        ui->Lkjlx1->clear();
    }

    ui->Bkjlx1->setEnabled(false);
}

void ApplicationFellow::on_comboBox_9_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_8->currentText() != "请选择" && ui->Lkjlx1->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->lineEdit_5->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bkjlx2->setEnabled(true);
            return;
        }

        ui->comboBox_9->setCurrentText("请选择");
    }
    else
    {
        ui->lineEdit_5->clear();
        ui->Lkjlx2->clear();
    }

    ui->Bkjlx2->setEnabled(false);
}

void ApplicationFellow::on_comboBox_10_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_9->currentText() != "请选择" && ui->Lkjlx2->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->lineEdit_6->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bkjlx3->setEnabled(true);
            return;
        }

        ui->comboBox_10->setCurrentText("请选择");
    }
    else
    {
        ui->lineEdit_6->clear();
        ui->Lkjlx3->clear();
    }

    ui->Bkjlx3->setEnabled(false);
}


void ApplicationFellow::on_add_clicked()        //增加学科竞赛
{
    if(ui->label_8->isHidden())
    {
        ui->label_8->show();
        ui->lineEdit_7->show();
        ui->comboBox_11->show();
        ui->label_19->show();
        ui->Lxkjs4->show();
        ui->Bxkjs4->show();
    }
    else if(ui->label_9->isHidden())
    {
        //清空初始值
        ui->Lxkjs5->setText("");

        ui->label_9->show();
        ui->lineEdit_8->show();
        ui->comboBox_12->show();
        ui->label_20->show();
        ui->Lxkjs5->show();
        ui->Bxkjs5->show();
    }
    else
    {
        Error("数量已达上限，请选择最优秀奖项填写");
    }
}


void ApplicationFellow::on_add_3_clicked()      //删除学科竞赛
{
    if(ui->label_9->isHidden())
    {
        if(ui->label_8->isHidden())
        {
            Error("其他不可删除，若有误请清空");
            return;
        }
        ui->label_8->hide();
        ui->lineEdit_7->hide();
        ui->lineEdit_17->clear();
        ui->comboBox_11->hide();
        ui->comboBox_11->setCurrentText("请选择");
        ui->label_19->hide();
        ui->Lxkjs4->hide();
        ui->Lxkjs4->clear();
        ui->Bxkjs4->hide();
    }
    else
    {
        //设置初始值
        ui->Lxkjs5->setText(Message);

        ui->label_9->hide();
        ui->lineEdit_8->hide();
        ui->lineEdit_8->clear();
        ui->comboBox_12->hide();
        ui->comboBox_12->setCurrentText("请选择");
        ui->label_20->hide();
        ui->Lxkjs5->hide();
        ui->Lxkjs5->clear();
        ui->Bxkjs5->hide();
    }
}

void ApplicationFellow::on_add_2_clicked()      //增加科技立项
{
    if(ui->label_6->isHidden())
    {
        //清空初始值
        ui->Lkjlx2->setText("");

        ui->label_6->show();
        ui->lineEdit_5->show();
        ui->comboBox_9->show();
        ui->label_15->show();
        ui->Lkjlx2->show();
        ui->Bkjlx2->show();
    }
    else if(ui->label_21->isHidden())
    {
        //清空初始值
        ui->Lkjlx3->setText("");

        ui->label_21->show();
        ui->lineEdit_6->show();
        ui->comboBox_10->show();
        ui->label_22->show();
        ui->Lkjlx3->show();
        ui->Bkjlx3->show();
    }
    else
    {
        Error("数量已达上限，请选择最优秀奖项填写");
    }
}

void ApplicationFellow::on_add_4_clicked()      //删除科技立项
{
    if(ui->label_21->isHidden())
    {
        if(ui->label_6->isHidden())
        {
            Error("其他不可删除，若有误请清空");
            return;
        }
        //设置初始值
        ui->Lkjlx2->setText(Message);

        ui->label_6->hide();
        ui->lineEdit_5->hide();
        ui->lineEdit_5->clear();
        ui->comboBox_9->hide();
        ui->comboBox_9->setCurrentText("请选择");
        ui->label_15->hide();
        ui->Lkjlx2->hide();
        ui->Lkjlx2->clear();
        ui->Bkjlx2->hide();
    }
    else
    {
        //设置初始值
        ui->Lkjlx3->setText(Message);

        ui->label_21->hide();
        ui->lineEdit_6->hide();
        ui->lineEdit_6->clear();
        ui->comboBox_10->hide();
        ui->comboBox_10->setCurrentText("请选择");
        ui->label_22->hide();
        ui->Lkjlx3->hide();
        ui->Lkjlx3->clear();
        ui->Bkjlx3->hide();
    }
}

void ApplicationFellow::setTimeBool(bool &time)     //设置时间标志，判断当前选择哪个竞赛时间
{
    //学科竞赛时间标志
    xktime1 = false;
    xktime2 = false;
    xktime3 = false;
    xktime4 = false;
    xktime5 = false;
    //科技立项时间标志
    kltime1 = false;
    kltime2 = false;
    kltime3 = false;

    time = true;
}

void ApplicationFellow::on_Bxkjs1_clicked()     //学科竞赛第一个时间按钮
{
    ui->tableTime->show();
    setTimeBool(xktime1);
}

void ApplicationFellow::on_Bxkjs2_clicked()     //学科竞赛第二个时间按钮
{
    ui->tableTime->show();
    setTimeBool(xktime2);
}

void ApplicationFellow::on_Bxkjs3_clicked()     //学科竞赛第三个时间按钮
{
    ui->tableTime->show();
    setTimeBool(xktime3);
}

void ApplicationFellow::on_Bxkjs4_clicked()     //学科竞赛第四个时间按钮
{
    ui->tableTime->show();
    setTimeBool(xktime4);
}

void ApplicationFellow::on_Bxkjs5_clicked()     //学科竞赛第五个时间按钮
{
    ui->tableTime->show();
    setTimeBool(xktime5);
}

void ApplicationFellow::on_Bkjlx1_clicked()     //科技立项第一个时间按钮
{
    ui->tableTime->show();
    setTimeBool(kltime1);
}

void ApplicationFellow::on_Bkjlx2_clicked()     //科技立项第二个时间按钮
{
    ui->tableTime->show();
    setTimeBool(kltime2);
}

void ApplicationFellow::on_Bkjlx3_clicked()     //科技立项第三个时间按钮
{
    ui->tableTime->show();
    setTimeBool(kltime3);
}

void ApplicationFellow::on_tableTime_clicked(const QDate &date)     //学科竞赛日历
{
    QString time = date.toString("yyyy-MM-dd");
    if(!checkTime("OTP", time))
        return;

    if(xktime1 == true)
    {
        ui->Lxkjs1->setText(date.toString("yyyy-MM-dd"));
    }
    else if(xktime2 == true)
    {
        ui->Lxkjs2->setText(date.toString("yyyy-MM-dd"));
    }
    else if(xktime3 == true)
    {
        ui->Lxkjs3->setText(date.toString("yyyy-MM-dd"));
    }
    else if(xktime4 == true)
    {
        ui->Lxkjs4->setText(date.toString("yyyy-MM-dd"));
    }
    else if(xktime5 == true)
    {
        ui->Lxkjs5->setText(date.toString("yyyy-MM-dd"));
    }
    else if(kltime1 == true)
    {
        ui->Lkjlx1->setText(date.toString("yyyy-MM-dd"));
    }
    else if(kltime2 == true)
    {
        ui->Lkjlx2->setText(date.toString("yyyy-MM-dd"));
    }
    else if(kltime3 == true)
    {
        ui->Lkjlx3->setText(date.toString("yyyy-MM-dd"));
    }

    ui->tableTime->hide();
}


void ApplicationFellow::on_save2_clicked()      //保存竞赛，科技立项信息
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类型");
        return;
    }

    if(ui->comboBox_8->currentText() != "请选择")
    {
        if(ui->Lkjlx1->text().isEmpty())
        {
            Error("请选择科技立项时间");
            return;
        }
    }

    if(!ui->Lkjlx2->isHidden() && ui->Lkjlx2->text().isEmpty())
    {
        Error("请完成新增科技立项时间或删除新增科技立项");
        return;
    }

    if(!ui->Lkjlx3->isHidden() && ui->Lkjlx3->text().isEmpty())
    {
        Error("请完成新增科技立项时间或删除新增科技立项");
        return;
    }

    //所有奖项等级
    QString priority = ui->comboBox_5->currentText() + "," + ui->comboBox_6->currentText() + "," + ui->comboBox_7->currentText() + "," + ui->comboBox_11->currentText() + "," + ui->comboBox_12->currentText() + "," + ui->comboBox_8->currentText() + "," + ui->comboBox_9->currentText() + "," + ui->comboBox_10->currentText();

    //所有奖项名称
    QString name = ui->lineEdit->text().trimmed() + "," + ui->lineEdit_2->text().trimmed() + "," + ui->lineEdit_3->text().trimmed() + "," + ui->lineEdit_7->text().trimmed() + "," + ui->lineEdit_8->text().trimmed() + "," + ui->lineEdit_4->text().trimmed() + "," + ui->lineEdit_5->text().trimmed() + "," + ui->lineEdit_6->text().trimmed();

    //所有获奖时间
    QString time = ui->Lxkjs1->text() + "," + ui->Lxkjs2->text() + "," + ui->Lxkjs3->text() + "," + ui->Lxkjs4->text() + "," + ui->Lxkjs5->text() + "," + ui->Lkjlx1->text() + "," + ui->Lkjlx2->text() + "," + ui->Lkjlx3->text();

    QStringList Lpriority = priority.split(",", QString::SkipEmptyParts);
    QStringList Lname = name.split(",", QString::SkipEmptyParts);
    QStringList Ltime = time.split(",", QString::SkipEmptyParts);

    //年级信息
    static QStringList grade;
    if(!classs.isEmpty())
    {
        grade += classs.split(",", QString::SkipEmptyParts);
    }

    int savecount = 0;
    int counts = Lpriority.size();
    int count = Lname.size() - 1;
    //数据库查询
    while(counts--)
    {
        if(count < 0)
            break;

        if(Lpriority.at(counts) != QString("请选择"))
        {
            QString sql = "select Item from ICM_SCORE where Expre='"+ Lpriority.at(counts)+"," + "'";
            QString item = MakeDatabase("SC", sql);
            qDebug()<<sql<<item;

            if(item == Wrong)
            {
                Error(FAILED);
                return;
            }
            QString sql2 = "insert into MORALISM_ADD_SCORE(StudentNo,Item,Time,TableName,Expres,TimeC) values(?,?,?,?,?,?)";
           if(insertDatabase("SIGNAL",sql2, item,Ltime.at(count),"ICM_SCORE",Lname.at(count),grade.at(count)))
            {
                savecount++;
                count--;
            }
        }
    }
    classs.clear();

    if(savecount > 0)
    {
        Error(SUCCESSFUL);
    }
    else
    {
        Error(FAILED);
    }
}



void ApplicationFellow::on_Bkj_clicked()        //创新平台职务时间按钮
{
    ui->kktimetable->show();
}

void ApplicationFellow::on_kktimetable_clicked(const QDate &date)//创新平台职务时间日历
{
    QString time = date.toString("yyyy-MM-dd");
    if(checkTime("FOR", time))
    {
        ui->lineEdit_10->setText(time);
        ui->kktimetable->hide();
    }
}

void ApplicationFellow::on_Bxue_clicked()   //学生会职务时间
{
    ui->stuclasstimetable->show();
    sclass1 = true;
    sclass2 = false;
}

void ApplicationFellow::on_Bclass_clicked() //班级职务时间
{
    ui->stuclasstimetable->show();
    sclass1 = false;
    sclass2 = true;
}

//学生会职务，班级职务公用日历
void ApplicationFellow::on_stuclasstimetable_clicked(const QDate &date)
{
    QString time = date.toString("yyyy-MM-dd");
    if(checkTime("FOR", time))
    {
        if(sclass1)
        {
            ui->lineEdit_11->setText(time);
            ui->stuclasstimetable->hide();
        }
        else if(sclass2)
        {
            ui->lineEdit_12->setText(time);
            ui->stuclasstimetable->hide();
        }
        else
        {
            Error("未知目标职务");
            return;
        }
    }
}

void ApplicationFellow::on_pushButton_3_clicked()       //职务信息保存
{
    if(!fellowstyle)
    {
        Error("青选择申请奖学金类型!");
        return;
    }

    //职务名称
    QString name = ui->groupBox_4->title().trimmed() + "," + ui->groupBox_5->title().trimmed() + "," + ui->groupBox_6->title().trimmed();

    //职务等级
    QString position = ui->comboBox_14->currentText() + "," + ui->comboBox_15->currentText() + "," + ui->comboBox_16->currentText();

    //入职时间
    QString time = ui->lineEdit_10->text() + "," + ui->lineEdit_11->text() + "," + ui->lineEdit_12->text();

    QStringList lposition = position.split(",", QString::SkipEmptyParts);
    QStringList ltime = time.split(",", QString::SkipEmptyParts);
    QStringList lname = name.split(",", QString::SkipEmptyParts);
    //入职学期
    static QStringList grade;
    if(!classs.isEmpty())
        grade += classs.split(",", QString::SkipEmptyParts);

    int count = ltime.size() - 1;
    int counts = lposition.size();
    int savecount = 0;      //保存记录数量
    while(counts--)
    {
        if(count < 0)
            break;

        if(lposition.at(counts) != QString("请选择"))
        {   
            QString sql = "select Item from POSITION_SCORE where Expre='"+ lposition.at(counts) + "," + "'";
            QString item = MakeDatabase("SC", sql);
            if(item == Wrong)
            {
                Error(FAILED);
                return;
            }

            QString sql2 = "insert into MORALISM_ADD_SCORE(StudentNo,Item,Time,TableName,Expres,TimeC) values(?,?,?,?,?,?)";
           if(insertDatabase("SIGNAL", sql2, item,ltime.at(count),"POSITION_SCORE",lname.at(count),grade.at(count)))
            {
                savecount++;
                count--;
            }
        }
    }
    classs.clear();

    if(savecount > 0)
    {
        Error(SUCCESSFUL);
    }
    else
    {
        Error(FAILED);
    }
}

//证书个数改变
void ApplicationFellow::on_comboBox_18_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        int books = arg1.toInt();
        switch (books)
        {
        case 1:
            //显示一个时间
            ui->label_35->show();
            ui->lineEdit_15->show();
            ui->doubleBt_3->show();
            //隐藏其他证书时间
            ui->label_36->hide();
            ui->label_38->hide();
            ui->label_37->hide();
            ui->label_39->hide();

            ui->lineEdit_17->hide();
            ui->lineEdit_18->hide();
            ui->lineEdit_19->hide();
            ui->lineEdit_20->hide();
            ui->lineEdit_17->clear();
            ui->lineEdit_18->clear();
            ui->lineEdit_19->clear();
            ui->lineEdit_20->clear();

            ui->doubleBt_4->hide();
            ui->doubleBt_6->hide();
            ui->doubleBt_5->hide();
            ui->doubleBt_7->hide();
            break;
        case 2:
            ui->label_35->show();
            ui->lineEdit_15->show();
            ui->doubleBt_3->show();

            ui->label_36->show();
            ui->lineEdit_17->show();
            ui->doubleBt_4->show();
            //隐藏其他证书时间

            ui->label_38->hide();
            ui->label_37->hide();
            ui->label_39->hide();

            ui->lineEdit_18->hide();
            ui->lineEdit_19->hide();
            ui->lineEdit_20->hide();
            ui->lineEdit_18->clear();
            ui->lineEdit_19->clear();
            ui->lineEdit_20->clear();

            ui->doubleBt_6->hide();
            ui->doubleBt_5->hide();
            ui->doubleBt_7->hide();
            break;
        case 3:
            ui->label_35->show();
            ui->lineEdit_15->show();
            ui->doubleBt_3->show();

            ui->label_36->show();
            ui->lineEdit_17->show();
            ui->doubleBt_4->show();

            ui->label_38->show();
            ui->lineEdit_18->show();
            ui->doubleBt_6->show();
            //隐藏其他证书时间

            ui->label_37->hide();
            ui->label_39->hide();

            ui->lineEdit_19->hide();
            ui->lineEdit_20->hide();
            ui->lineEdit_19->clear();
            ui->lineEdit_20->clear();

            ui->doubleBt_5->hide();
            ui->doubleBt_7->hide();
            break;
        case 4:
            ui->label_35->show();
            ui->lineEdit_15->show();
            ui->doubleBt_3->show();

            ui->label_36->show();
            ui->lineEdit_17->show();
            ui->doubleBt_4->show();

            ui->label_37->show();
            ui->lineEdit_19->show();
            ui->doubleBt_5->show();

            ui->label_38->show();
            ui->lineEdit_18->show();
            ui->doubleBt_6->show();
            //隐藏其他证书时间
            ui->label_39->hide();
            ui->lineEdit_20->hide();
            ui->lineEdit_20->clear();
            ui->doubleBt_7->hide();
            break;
        case 5:
            ui->label_35->show();
            ui->lineEdit_15->show();
            ui->doubleBt_3->show();

            ui->label_36->show();
            ui->lineEdit_17->show();
            ui->doubleBt_4->show();

            ui->label_37->show();
            ui->lineEdit_19->show();
            ui->doubleBt_5->show();

            ui->label_38->show();
            ui->lineEdit_18->show();
            ui->doubleBt_6->show();

            ui->label_39->show();
            ui->lineEdit_20->show();
            ui->doubleBt_7->show();
            break;
        default:
            break;
        }
    }
}

//活动证书个数改
void ApplicationFellow::on_comboBox_19_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        int activitices = arg1.toInt();
        switch (activitices)
        {
        case 1:
            //显示一个活动时间
            ui->label_41->show();
            ui->lineEdit_16->show();
            ui->doubleBt_11->show();
            //隐藏其他活动时间
            ui->label_42->hide();
            ui->label_43->hide();
            ui->label_44->hide();
            ui->label_45->hide();

            ui->lineEdit_21->hide();
            ui->lineEdit_23->hide();
            ui->lineEdit_24->hide();
            ui->lineEdit_22->hide();
            ui->lineEdit_21->clear();
            ui->lineEdit_23->clear();
            ui->lineEdit_24->clear();
            ui->lineEdit_22->clear();

            ui->doubleBt_12->hide();
            ui->doubleBt_9->hide();
            ui->doubleBt_10->hide();
            ui->doubleBt_8->hide();
            break;
        case 2:
            ui->label_41->show();
            ui->lineEdit_16->show();
            ui->doubleBt_11->show();

            ui->label_42->show();
            ui->lineEdit_21->show();
            ui->doubleBt_12->show();
            //隐藏其他活动时间

            ui->label_43->hide();
            ui->label_44->hide();
            ui->label_45->hide();

            ui->lineEdit_23->hide();
            ui->lineEdit_24->hide();
            ui->lineEdit_22->hide();
            ui->lineEdit_23->clear();
            ui->lineEdit_24->clear();
            ui->lineEdit_22->clear();

            ui->doubleBt_9->hide();
            ui->doubleBt_10->hide();
            ui->doubleBt_8->hide();
            break;
        case 3:
            ui->label_41->show();
            ui->lineEdit_16->show();
            ui->doubleBt_11->show();

            ui->label_42->show();
            ui->lineEdit_21->show();
            ui->doubleBt_12->show();

            ui->label_43->show();
            ui->lineEdit_23->show();
            ui->doubleBt_9->show();
            //隐藏其他活动时间

            ui->label_44->hide();
            ui->label_45->hide();

            ui->lineEdit_24->hide();
            ui->lineEdit_22->hide();
            ui->lineEdit_24->clear();
            ui->lineEdit_22->clear();

            ui->doubleBt_10->hide();
            ui->doubleBt_8->hide();
            break;
        case 4:
            ui->label_41->show();
            ui->lineEdit_16->show();
            ui->doubleBt_11->show();

            ui->label_42->show();
            ui->lineEdit_21->show();
            ui->doubleBt_12->show();

            ui->label_43->show();
            ui->lineEdit_23->show();
            ui->doubleBt_9->show();

            ui->label_44->show();
            ui->lineEdit_24->show();
            ui->doubleBt_10->show();
            //隐藏其他活动时间
            ui->label_45->hide();

            ui->lineEdit_22->hide();
            ui->lineEdit_22->clear();
            ui->doubleBt_8->hide();
            break;
        case 5:
            ui->label_41->show();
            ui->lineEdit_16->show();
            ui->doubleBt_11->show();

            ui->label_42->show();
            ui->lineEdit_21->show();
            ui->doubleBt_12->show();

            ui->label_43->show();
            ui->lineEdit_23->show();
            ui->doubleBt_9->show();

            ui->label_44->show();
            ui->lineEdit_24->show();
            ui->doubleBt_10->show();

            ui->label_45->show();
            ui->lineEdit_22->show();
            ui->doubleBt_8->show();
            break;
        default:
            break;
        }
    }
}

void ApplicationFellow::on_doubleBt_clicked()   //双学位时间按钮
{
    numbers = 1;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_2_clicked()  //计算机证等级
{
    if(ui->comboBox_17->currentText() == "请选择")
    {
        Error("请选择证件等级!");
        return;
    }
    numbers = 2;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_3_clicked()     //其他证书第一个时间按钮
{
    if(ui->yes_2->isChecked() && ui->lineEdit_25->text().isEmpty())
    {
        Error("请选择献血时间!");
        return;
    }
    numbers = 3;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_4_clicked()     //其他证书第二个时间按钮
{
    if(ui->lineEdit_15->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 4;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_6_clicked()     //其他证书第三个时间按钮
{
    if(ui->lineEdit_17->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 5;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_5_clicked()     //其他证书第四个时间按钮
{
    if(ui->lineEdit_18->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 6;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_7_clicked()     //其他证书第五个时间按钮
{
    if(ui->lineEdit_19->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 7;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_11_clicked()     //其他活动第一个时间按钮
{
    numbers = 8;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_12_clicked()     //其他活动第二个时间按钮
{
    if(ui->lineEdit_16->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 9;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_9_clicked()     //其他活动第三个时间按钮
{
    if(ui->lineEdit_21->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 10;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_10_clicked()     //其他活动第四个时间按钮
{
    if(ui->lineEdit_23->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 11;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_8_clicked()     //其他活动第五个时间按钮
{
    if(ui->lineEdit_24->text().isEmpty())
    {
        Error(BEFORE);
        return;
    }
    numbers = 12;
    ui->moretime->show();
}

void ApplicationFellow::on_doubleBt_13_clicked()      //献血活动时间
{
    numbers = 13;
    ui->moretime->show();
}

void ApplicationFellow::on_moretime_clicked(const QDate &date)
{
    QString time = date.toString("yyyy-MM-dd");
    switch(numbers)
    {
    case 1:
        if(checkTime("FOR", time))
        {
            ui->lineEdit_13->setText(time);
            ui->moretime->hide();
        }
        break;
    case 2:
        if(checkTime("FOR", time))
        {
            ui->lineEdit_14->setText(time);
            ui->moretime->hide();
        }
        break;
    case 3:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_15->setText(time);
            ui->moretime->hide();
        }
        break;
    case 4:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_17->setText(time);
            ui->moretime->hide();
        }
        break;
    case 5:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_18->setText(time);
            ui->moretime->hide();
        }
        break;
    case 6:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_19->setText(time);
            ui->moretime->hide();
        }
        break;
    case 7:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_20->setText(time);
            ui->moretime->hide();
        }
        break;
    case 8:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_16->setText(time);
            ui->moretime->hide();
        }
        break;
    case 9:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_21->setText(time);
            ui->moretime->hide();
        }
        break;
    case 10:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_23->setText(time);
            ui->moretime->hide();
        }
        break;
    case 11:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_24->setText(time);
            ui->moretime->hide();
        }
        break;
    case 12:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_22->setText(time);
            ui->moretime->hide();
        }
    case 13:
        if(checkTime("OTP", time))
        {
            ui->lineEdit_25->setText(time);
            ui->moretime->hide();
        }
        break;
    default:
        break;
    }
}

void ApplicationFellow::on_pushButton_2_clicked()       //保存证书活动信息
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类型!");
        return;
    }

    //保存证书类型信息
    QString majorbook = NULL;
    QString time = NULL;
    if(ui->yes->isChecked())
    {
        majorbook = ui->label_30->text().trimmed() + ",";
        time += ui->lineEdit_13->text() + ",";
    }

    if(ui->comboBox_17->currentText() != "请选择")
    {
            majorbook += ui->label_32->text().trimmed() + ui->comboBox_17->currentText() + ",";
            time += ui->lineEdit_14->text() + ",";
    }

    if(ui->yes_2->isChecked())
    {
        majorbook += ui->label_46->text().trimmed() + ",";
        time += ui->lineEdit_25->text() + ",";
    }

    int book = ui->comboBox_18->currentText().toInt();
    while(book--)
    {
        majorbook += "其他证书";
        majorbook += ",";
    }

    int activit = ui->comboBox_19->currentText().toInt();
    while(activit--)
    {
        majorbook += "其他活动";
        majorbook += ",";
    }

    time += ui->lineEdit_15->text() + "," + ui->lineEdit_17->text() + "," + ui->lineEdit_18->text() + "," + ui->lineEdit_19->text() + "," + ui->lineEdit_20->text() + "," + ui->lineEdit_16->text() + "," + ui->lineEdit_21->text() + "," + ui->lineEdit_24->text() + "," + ui->lineEdit_23->text() + "," + ui->lineEdit_22->text();

    QStringList lmajor = majorbook.split(",", QString::SkipEmptyParts);
    QStringList ltime = time.split(",", QString::SkipEmptyParts);

    static QStringList grade;
    if(!classs.isEmpty())
        grade += classs.split(",", QString::SkipEmptyParts);

    int count = ltime.size();
    int savecount = 0;      //记录保存专业证书

    while(count--)
    {
        if(!ltime.at(count).isEmpty())
        {
            QString sql = "select Item from ACTIVITY_SCORE where Expre='" + lmajor.at(count) + "'";
            QString item = MakeDatabase("SC", sql);
            qDebug()<<sql<<item;

            if(item == Wrong)
            {
                Error(FAILED);
                return;
            }

            QString sql2 = "insert into MORALISM_ADD_SCORE(StudentNo,Item,Time,TableName,Expres,TimeC) values(?,?,?,?,?,?)";
            QString style = NULL;
            if(lmajor.at(count) == "其他证书" || lmajor.at(count) == "献血")  //这三项因年中如果有多次，可累计加分5次
            {
                style = "MORE";
            }
            else if( lmajor.at(count) == "其他活动")
            {
                style = "OACV";
            }
            else
            {
                style = "SIGNAL";
            }

           if(insertDatabase(style, sql2, item,ltime.at(count),"ACTIVITY_SCORE",lmajor.at(count),grade.at(count)))
           {
                savecount++;
           }
        }
    }
    classs.clear();

    if(savecount > 0)
    {
        Error(SUCCESSFUL);
    }
    else
    {
        Error(FAILED);
    }
    return;
}



void ApplicationFellow::on_cetbt_clicked()      //英语四级过四级时间
{
    ui->cettime->show();
}

void ApplicationFellow::on_cettime_clicked(const QDate &date)   //四级时间
{
    QString time = date.toString("yyyy-MM-dd");
    if(checkTime("FOR", time))
    {
        ui->lineEdit_26->setText(time);
        ui->cettime->hide();
    }
}

void ApplicationFellow::on_cetbt2_clicked()     //英语六级过六级时间
{
    ui->cettime2->show();
}

void ApplicationFellow::on_cettime2_clicked(const QDate &date)
{
    QString time = date.toString("yyyy-MM-dd");
    if(checkTime("FOR", time))
    {
        ui->lineEdit_27->setText(time);
        ui->cettime2->hide();
    }
}

void ApplicationFellow::on_cetsave_clicked()    //保存英语
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类别!");
        return;
    }

    if((ui->cet4yes->isChecked() && ui->lineEdit_26->text().isEmpty()) || (ui->cet6yes->isChecked() && ui->lineEdit_27->text().isEmpty()))
    {
        Error("请选择通过英语证书时间!");
        return;
    }

    QString cet = NULL;
    QString time = NULL;
    if(ui->cet4yes->isChecked())
    {
        cet += ui->groupBox_10->title() + ",";
        time += ui->lineEdit_26->text() + ",";
    }

    if(ui->cet6yes->isChecked())
    {
        cet += ui->groupBox_11->title() + ",";
        time += ui->lineEdit_27->text() + ",";
    }

    QStringList lcet = cet.split(",", QString::SkipEmptyParts);
    QStringList ltime = time.split(",", QString::SkipEmptyParts);
     static QStringList grade;
    if(!classs.isEmpty())
    {
        classs.simplified();
        grade += classs.split(",", QString::SkipEmptyParts);
    }

    int count = lcet.size();
    int savecount = 0;
    while(count--)
    {
        QString sql = NULL;
        if(lcet.at(count) == "CET6")
            sql = tr("select Expre from ENGLISH_SCORE where Item='%1' and Time='%2'").arg(lcet.at(count)).arg("大学");
        else if(lcet.at(count) == "CET4")
        {
            sql = tr("select Expre from ENGLISH_SCORE where Item='%1' and Time='%2'").arg(lcet.at(count)).arg(grade.at(count).left(2).trimmed());
        }
        else
        {
            Error("当前没有数据!");
            return;
        }

        QString expre = MakeDatabase("SC", sql);
        if(expre == Wrong)
        {
            Error(FAILED);
            return;
        }

        QString sql2 = "insert into MORALISM_ADD_SCORE(StudentNo,Item,Time,TableName,Expres,TimeC) values(?,?,?,?,?,?)";

       if(insertDatabase("SIGNAL", sql2, lcet.at(count),ltime.at(count),"ENGLISH_SCORE",expre,grade.at(count)))
       {
            savecount++;
       }
    }
    classs.clear();

    if(savecount > 0)
    {
        Error(SUCCESSFUL);
    }
    else
    {
        Error(FAILED);
    }

    return;
}

void ApplicationFellow::on_add_5_clicked()      //增加体育赛事
{
    if(ui->label_62->isHidden())
    {
        if(ui->label_60->isHidden())
        {
            ui->label_60->show();
            ui->Lxkjs1_5->show();
            ui->comboBox_23->show();
            ui->label_59->show();
            ui->lineEdit_31->show();
            ui->Bxkjs1_5->show();
        }
        else
        {
            ui->label_62->show();
            ui->Lxkjs1_6->show();
            ui->comboBox_24->show();
            ui->label_61->show();
            ui->lineEdit_32->show();
            ui->Bxkjs1_6->show();
        }
    }
    else
    {
        Error("记录数量已达上限，请选择最优秀成绩!");
        return;
    }
}

void ApplicationFellow::on_sub_clicked()        //删除体育赛事
{
    if(ui->label_62->isHidden())
    {
        if(ui->label_60->isHidden())
        {
            Error("当前记录不可删除，如有错误清空信息即可!");
            return;
        }

        ui->label_60->hide();
        ui->Lxkjs1_5->hide();
        ui->Lxkjs1_5->clear();
        ui->comboBox_23->hide();
        ui->comboBox_23->setCurrentText("请选择");
        ui->label_59->hide();
        ui->lineEdit_31->hide();
        ui->lineEdit_31->clear();
        ui->Bxkjs1_5->hide();
    }
    else
    {
        ui->label_62->hide();
        ui->Lxkjs1_6->hide();
        ui->Lxkjs1_6->clear();
        ui->comboBox_24->hide();
        ui->comboBox_24->setCurrentText("请选择");
        ui->label_61->hide();
        ui->lineEdit_32->hide();
        ui->lineEdit_32->clear();
        ui->Bxkjs1_6->hide();
    }
}

void ApplicationFellow::on_add1_clicked()       //增加工作信息
{
    if(ui->label_67->isHidden())
    {
        if(ui->label_65->isHidden())
        {
            ui->label_65->show();
            ui->Lxkjs1_8->show();
            ui->comboBox_26->show();
            ui->label_66->show();
            ui->lineEdit_34->show();
            ui->Bxkjs1_8->show();
        }
        else
        {
            ui->label_67->show();
            ui->Lxkjs1_9->show();
            ui->comboBox_27->show();
            ui->label_68->show();
            ui->lineEdit_35->show();
            ui->Bxkjs1_9->show();
        }
    }
    else
    {
        Error("记录数量已达上限，请选择最优秀成绩!");
        return;
    }
}

void ApplicationFellow::on_sub1_clicked()       //删除工作记录
{
    if(ui->label_67->isHidden())
    {
        if(ui->label_65->isHidden())
        {
            Error("当前记录不可删除，如有错误清空信息即可!");
            return;
        }

        ui->label_65->hide();
        ui->Lxkjs1_8->hide();
        ui->Lxkjs1_8->clear();
        ui->comboBox_26->hide();
        ui->comboBox_26->setCurrentText("请选择");
        ui->label_66->hide();
        ui->lineEdit_34->hide();
        ui->lineEdit_34->clear();
        ui->Bxkjs1_8->hide();

    }
    else
    {
        ui->label_67->hide();
        ui->Lxkjs1_9->hide();
        ui->Lxkjs1_9->clear();
        ui->comboBox_27->hide();
        ui->comboBox_27->setCurrentText("请选择");
        ui->label_68->hide();
        ui->lineEdit_35->hide();
        ui->lineEdit_35->clear();
        ui->Bxkjs1_9->hide();
    }
}

void ApplicationFellow::on_Bxkjs1_2_clicked()   //第一个体育赛事获奖时间
{
    ui->sporttime->show();
    sporttime = 1;
}

void ApplicationFellow::on_Bxkjs1_3_clicked()   //第二个体育赛事获奖时间
{
    ui->sporttime->show();
    sporttime = 2;
}

void ApplicationFellow::on_Bxkjs1_4_clicked()   //第三个体育赛事获奖时间
{
    ui->sporttime->show();
    sporttime = 3;
}

void ApplicationFellow::on_Bxkjs1_5_clicked()   //第四个体育赛事获奖时间
{
    ui->sporttime->show();
    sporttime = 4;
}

void ApplicationFellow::on_Bxkjs1_6_clicked()   //第五个体育赛事获奖时间
{
    ui->sporttime->show();
    sporttime = 5;
}

void ApplicationFellow::on_Bxkjs1_7_clicked()   //第一个体育赛事工作者时间
{
    ui->sporttime->show();
    sporttime = 6;
}

void ApplicationFellow::on_Bxkjs1_8_clicked()   //第二个体育赛事工作者时间
{
    ui->sporttime->show();
    sporttime = 7;
}

void ApplicationFellow::on_Bxkjs1_9_clicked()   //第三个体育赛事工作者时间
{
    ui->sporttime->show();
    sporttime = 8;
}

void ApplicationFellow::on_sporttime_clicked(const QDate &date)     //体育日历
{
    QString time = date.toString("yyyy-MM-dd");
    if(checkTime("OTP", time))
    {
        switch (sporttime)
        {
        case 1:
            ui->lineEdit_28->setText(time);
            ui->sporttime->hide();
            break;
        case 2:
            ui->lineEdit_29->setText(time);
            ui->sporttime->hide();
            break;
        case 3:
            ui->lineEdit_30->setText(time);
            ui->sporttime->hide();
            break;
        case 4:
            ui->lineEdit_31->setText(time);
            ui->sporttime->hide();
            break;
        case 5:
            ui->lineEdit_32->setText(time);
            ui->sporttime->hide();
            break;
        case 6:
            ui->lineEdit_33->setText(time);
            ui->sporttime->hide();
            break;
        case 7:
            ui->lineEdit_34->setText(time);
            ui->sporttime->hide();
            break;
        case 8:
            ui->lineEdit_35->setText(time);
            ui->sporttime->hide();
            break;
        default:
            break;
        }
    }
}


void ApplicationFellow::on_comboBox_20_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->Lxkjs1_2->text().isEmpty())
        {
            Error(Message);
            ui->comboBox_20->setCurrentText("请选择");
        }
        else
        {
            ui->Bxkjs1_2->setEnabled(true);
            return;
        }
    }
    else
    {
        ui->Lxkjs1_2->clear();
        ui->lineEdit_28->clear();
    }

    ui->Bxkjs1_2->setEnabled(false);
}

//选择下一个比赛时判断上一个比赛是否信息完整
void ApplicationFellow::on_comboBox_21_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_20->currentText() != "请选择" && ui->lineEdit_28->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->Lxkjs1_3->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bxkjs1_3->setEnabled(true);
            return;
        }

        ui->comboBox_21->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_3->clear();
        ui->lineEdit_29->clear();
    }

    ui->Bxkjs1_3->setEnabled(false);
}

//选择下一个比赛时判断上一个比赛是否信息完整
void ApplicationFellow::on_comboBox_22_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_21->currentText() != "请选择" && ui->lineEdit_29->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->Lxkjs1_4->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bxkjs1_4->setEnabled(true);
            return;
        }

        ui->comboBox_22->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_4->clear();
        ui->lineEdit_30->clear();
    }

    ui->Bxkjs1_4->setEnabled(false);
}

//选择下一个比赛时判断上一个比赛是否信息完整
void ApplicationFellow::on_comboBox_23_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_22->currentText() != "请选择" && ui->lineEdit_30->text().isEmpty())
        {
            Error(BEFORE);   
        }
        else if(ui->Lxkjs1_5->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bxkjs1_5->setEnabled(true);
            return;
        }
        ui->comboBox_23->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_5->clear();
        ui->lineEdit_31->clear();
    }

    ui->Bxkjs1_5->setEnabled(false);
}

//选择下一个比赛时判断上一个比赛是否信息完整
void ApplicationFellow::on_comboBox_24_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_23->currentText() != "请选择" && ui->lineEdit_31->text().isEmpty())
        {
            Error(BEFORE);
        }
        else if(ui->Lxkjs1_6->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bxkjs1_6->setEnabled(true);
            return;
        }

        ui->comboBox_24->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_6->clear();
        ui->lineEdit_32->clear();
    }

    ui->Bxkjs1_6->setEnabled(false);
}


void ApplicationFellow::on_comboBox_25_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->Lxkjs1_7->text().isEmpty())
        {
            Error(Message);

        }
        else
        {
            ui->Bxkjs1_7->setEnabled(true);
            return;
        }

        ui->comboBox_25->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_7->clear();
        ui->lineEdit_33->clear();
    }

    ui->Bxkjs1_7->setEnabled(false);
}

//选择下一个比赛时判断上一个比赛是否信息完整
void ApplicationFellow::on_comboBox_26_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_25->currentText() != "请选择" && ui->lineEdit_33->text().isEmpty())
        {
            Error(BEFORE);

        }
        else if(ui->Lxkjs1_8->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bxkjs1_8->setEnabled(true);
            return;
        }

        ui->comboBox_26->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_8->clear();
        ui->lineEdit_34->clear();
    }

    ui->Bxkjs1_8->setEnabled(false);
}

//选择下一个比赛时判断上一个比赛是否信息完整
void ApplicationFellow::on_comboBox_27_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->comboBox_26->currentText() != "请选择" && ui->lineEdit_34->text().isEmpty())
        {
            Error(BEFORE);

        }
        else if(ui->Lxkjs1_9->text().isEmpty())
        {
            Error(Message);
        }
        else
        {
            ui->Bxkjs1_9->setEnabled(true);
            return;
        }

        ui->comboBox_27->setCurrentText("请选择");
    }
    else
    {
        ui->Lxkjs1_9->clear();
        ui->lineEdit_35->clear();
    }

    ui->Bxkjs1_9->setEnabled(false);
}

void ApplicationFellow::on_savesport_clicked()      //保存体育加分项
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类型!");
        return;
    }

    //体育赛事等级
    QString pro = ui->comboBox_20->currentText() + "," + ui->comboBox_21->currentText() + "," + ui->comboBox_22->currentText() + "," + ui->comboBox_23->currentText() + "," + ui->comboBox_24->currentText() + "," + ui->comboBox_25->currentText() + "," + ui->comboBox_26->currentText() + "," + ui->comboBox_27->currentText();

    //赛事名称
    QString name = ui->Lxkjs1_2->text().trimmed() + "," + ui->Lxkjs1_3->text().trimmed() + "," + ui->Lxkjs1_4->text().trimmed() + "," + ui->Lxkjs1_5->text().trimmed() + "," + ui->Lxkjs1_6->text().trimmed() + "," + ui->Lxkjs1_7->text().trimmed() + "," + ui->Lxkjs1_8->text().trimmed() + "," + ui->Lxkjs1_9->text().trimmed();

    //体育赛事时间
    QString time = ui->lineEdit_28->text() + "," + ui->lineEdit_29->text() + "," + ui->lineEdit_30->text() + "," + ui->lineEdit_31->text() + "," + ui->lineEdit_32->text() + "," + ui->lineEdit_33->text() + "," + ui->lineEdit_34->text() + "," + ui->lineEdit_35->text();

    //年级信息
    static QStringList grade;
    if(!classs.isEmpty())
    {
        grade += classs.split(",", QString::SkipEmptyParts);
    }
    QStringList lpro = pro.split(",", QString::SkipEmptyParts);
    QStringList ltime = time.split(",", QString::SkipEmptyParts);
    QStringList lname = name.split(",", QString::SkipEmptyParts);

    int count = lpro.size();
    int counts = ltime.size();
    qDebug()<<lpro.size()<<ltime.size()<<counts;

    int savecount = 0;
    while(count--)
    {
        if(counts < 0)
            break;

        if(lpro.at(count) != "请选择")
        {
            QString sql = "select Item from SPORTS_SCORE where Expre='" + lpro.at(count) + "," + "'";
            QString item = MakeDatabase("SC", sql);
            qDebug()<<sql<<item;

            if(item == Wrong)
            {
                Error(FAILED);
                return;
            }

            QString sql2 = "insert into SPORT_ADD_SCORE(StudentNo,Item,Time,Expre,TimeC) values(?,?,?,?,?)";

           if(insertDatabasetoSport(sql2, item,ltime.at(counts - 1),lname.at(counts - 1),grade.at(counts - 1)))
           {
                savecount++;
                counts--;
           }
        }
    }
    classs.clear();

    if(savecount > 0)
    {
        Error(SUCCESSFUL);
    }
    else
    {
        Error(FAILED);
    }

    return;
}

void ApplicationFellow::on_photo_clicked()      //提交照片
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类型!");
        return;
    }

    QStringList filename = QFileDialog::getOpenFileNames(this,tr("文件对话框"),"D:","(*png *jpg)");
    if(filename.size() == 0)
    {
        Error("请选择照片!");
        return;
    }

    QDateTime time = QDateTime::currentDateTime();
    QString curtime = time.toString("yyyy-MM-dd");

    QString grade = curGrade(curtime);

    QProgressBar *pro = new QProgressBar(this);
    pro->show();
    int size = filename.size();
    pro->setMaximum(size);

    QString sql1 = "insert into DATA(StudentNo, Address, Time) values(?,?,?)";
    QSqlQuery query(db);
    for(int it = 0; it < size; it++)
    {
        QString sql = tr("select id from DATA where StudentNo='%1' and Address='%2' and Time='%3'").arg(studentNo).arg(filename.at(it)).arg(grade.left(2));
        QString id = MakeDatabase("SC", sql);
        if(id != Wrong)
            continue;
        QSqlDatabase::database().transaction();   //事务开始
        query.prepare(sql1);
        query.addBindValue(tr("%1").arg(studentNo));
        query.addBindValue(tr("%1").arg(filename.at(it)));
        query.addBindValue(tr("%1").arg(grade.left(2)));
        query.exec();
        pro->setMinimum(it);
        QSqlDatabase::database().commit();  //事务结束
    }

    ui->sure->setEnabled(true);
    Error("提交成功!");
    return;
}

void ApplicationFellow::on_sure_clicked()       //提交申请记录
{
    if(!fellowstyle)
    {
        Error("请选择申请奖学金类型!");
        return;
    }

    QDateTime time = QDateTime::currentDateTime();
    QString curtime = time.toString("yyyy-MM-dd");

    QString grade = curGrade(curtime);

    QString sql = tr("select id from APPLICATION where StudentNo='%1' and Style='%2' and Time='%3'").arg(studentNo).arg(ui->comboBox_13->currentText()).arg(grade.left(2));
    QString id = MakeDatabase("SC", sql);
    if(id != Wrong)
    {
        Error("你今年已经申请该类型奖学金!");
        return;
    }

    QString sql1 = "insert into APPLICATION(StudentNo, Style, Time, Status) values(?,?,?,?)";
    QSqlQuery query(db);
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare(sql1);
    query.addBindValue(tr("%1").arg(studentNo));
    query.addBindValue(ui->comboBox_13->currentText());
    query.addBindValue(tr("%1").arg(grade.left(2)));
    query.addBindValue(tr("待审核"));
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    Error("申请成功!");
    classs.clear();
    this->close();
}

//科技创新平台成员下拉框内容改变事件
void ApplicationFellow::on_comboBox_14_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->lineEdit_9->text().isEmpty() || ui->lineEdit_10->text().isEmpty())
        {
            Error("请完善信息!");
            ui->comboBox_14->setCurrentText("请选择");
            return;
        }
    }
    else
    {
        ui->lineEdit_9->clear();
        ui->lineEdit_10->clear();
    }
}

//学生会下拉框内容改变事件
void ApplicationFellow::on_comboBox_15_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->lineEdit_11->text().isEmpty())
        {
            Error("请完善信息!");
            ui->comboBox_15->setCurrentText("请选择");
            return;
        }
    }
    else
    {
        ui->lineEdit_11->clear();
    }
}

//行政班级下拉框内容改变事件
void ApplicationFellow::on_comboBox_16_currentTextChanged(const QString &arg1)
{
    if(arg1 != "请选择")
    {
        if(ui->lineEdit_12->text().isEmpty())
        {
            Error("请完善信息!");
            ui->comboBox_16->setCurrentText("请选择");
            return;
        }
    }
    else
    {
        ui->lineEdit_12->clear();
    }
}
