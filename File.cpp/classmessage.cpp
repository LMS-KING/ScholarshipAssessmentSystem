#include "classmessage.h"
#include "ui_classmessage.h"
#include <QMessageBox>
#include <QDebug>
#define WRONG "数据库访问错误!"

extern bool CA;

ClassMessage::ClassMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClassMessage)
{
    ui->setupUi(this);
}

ClassMessage::~ClassMessage()
{
    delete ui;
}

void ClassMessage::recvDB(QSqlDatabase thdb, QString sclaNo, QString smajor)
{
    db = thdb;
    classNo = sclaNo;
    major = smajor;
}

void ClassMessage::recvchangeOradd(bool tf)
{
    CorA = tf;

    setBox();

    if(CorA)        //编辑
    {
        QString sql = "select ClassNo, ClassName, SMajor, ClassScore, TeacherNo, Time from CLASS";
        QSqlQuery query(db);
        query.exec(sql);
        if(query.next())
        {
            ui->classno->setReadOnly(true);
            ui->comboBox_2->setEnabled(false);
            ui->comboBox->setEnabled(false);

            ui->classno->setText(query.value(0).toString().trimmed());
            ui->classname->setText(query.value(1).toString().trimmed());
            ui->comboBox_2->setCurrentText(query.value(2).toString().trimmed());
            ui->score->setText(query.value(3).toString().trimmed());
            ui->teachno->setText(query.value(4).toString().trimmed());
            ui->comboBox->setCurrentText(query.value(5).toString().trimmed());
        }
    }
    else
    {
        ui->classno->setReadOnly(false);
        ui->comboBox_2->setEnabled(true);

        ui->classno->clear();
        ui->classname->clear();
        ui->score->clear();
        ui->teachno->clear();
        ui->comboBox->setEnabled(true);
    }
}

void ClassMessage::setBox()   //设置下拉框
{
    static bool tf = true;
    if(!tf)
        return;

    QString sql = "select class from GRADE";
    QString gra = NULL;
    gra = selectDB(sql);
    QStringList grade = gra.split(",", QString::SkipEmptyParts);
    ui->comboBox->addItems(grade);

    QString major = "软件工程,网络工程,通信工程,计算机科学技术";
    QStringList majors = major.split(",", QString::SkipEmptyParts);
    ui->comboBox_2->addItems(majors);

    tf = false;
}

QString ClassMessage::selectDB(QString sql)
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

bool ClassMessage::InsertDB(QString sql)
{
    QSqlQuery query(db);
    QSqlDatabase::database().transaction();   //事务开始
    query.prepare(sql);
    query.addBindValue(tr("%1").arg(ui->classno->text()));
    query.addBindValue(tr("%1").arg(ui->classname->text().trimmed()));
    query.addBindValue(tr("%1").arg(ui->score->text().trimmed()));
    query.addBindValue(tr("%1").arg(ui->comboBox_2->currentText().trimmed()));
    query.addBindValue(tr("%1").arg(ui->teachno->text().trimmed()));
    query.addBindValue(tr("%1").arg(ui->comboBox->currentText().trimmed()));
    query.exec();
    QSqlDatabase::database().commit();  //事务结束

    return true;
}

void ClassMessage::on_pushButton_clicked()      //确定
{
    if(ui->classno->text().isEmpty() || ui->classname->text().isEmpty() || ui->teachno->text().isEmpty() || ui->score->text().isEmpty())
    {
        QMessageBox::information(this, "提示", "所有项都为必填项!请完善信息!");
        return;
    }

    if(ui->score->text().toInt() <= 0)
    {
        QMessageBox::information(this, "提示", "学分信息有误!");
        return;
    }

    if(CorA)        //编辑
    {
        QString sql = tr("update CLASS set ClassName='%1', ClassScore=%2, TeacherNo='%3', Time='%4' where ClassNo='%5' and SMajor='%6'").arg(ui->classname->text().trimmed()).arg(ui->score->text().trimmed()).arg(ui->teachno->text().trimmed()) .arg(ui->comboBox->currentText().trimmed()).arg(ui->classno->text()).arg(ui->comboBox_2->currentText().trimmed());

        QString up = selectDB(sql);
        if(up != WRONG)
        {
            QMessageBox::information(this, "提示", "操作失败!", QMessageBox::Ok);
            CA = false;
            return;
        }

        QMessageBox::information(this, "提示", "操作成功!", QMessageBox::Ok);
        CA = true;
        this->close();
        return;
    }
    else        //增加
    {
        QString sql = tr("select ClassScore from CLASS where ClassNo='%1' and SMajor='%2' and Time='%3'").arg(ui->classno->text().trimmed()).arg(ui->comboBox_2->currentText().trimmed()).arg(ui->comboBox->currentText().trimmed());

        QString sc = selectDB(sql);
        if(sc != WRONG)
        {
            QMessageBox::information(this, "提示", "该课程已存在，请查看课程号，专业和年级是否有误!", QMessageBox::Ok);
            return;
        }

        QString sql1 = "insert into CLASS(ClassNo, ClassName, ClassScore, SMajor, TeacherNo, Time) values(?,?,?,?,?,?)";
        if(InsertDB(sql1))
        {
            QMessageBox::information(this, "提示", "操作成功!", QMessageBox::Ok);
            CA = true;
            this->close();
        }
        else
        {
            QMessageBox::information(this, "提示", "操作失败!", QMessageBox::Ok);
            CA = false;
        }

        return;
    }
}

void ClassMessage::on_pushButton_2_clicked()        //
{
    this->close();
}
