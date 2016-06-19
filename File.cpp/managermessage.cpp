#include "managermessage.h"
#include "ui_managermessage.h"
#include <QMessageBox>
#include <QRegExp>
#define WRONG "数据库访问出错!"

extern bool add;
ManagerMessage::ManagerMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagerMessage)
{
    ui->setupUi(this);

    ui->email->installEventFilter(this);
    ui->no->installEventFilter(this);
}

ManagerMessage::~ManagerMessage()
{
    delete ui;
}

void ManagerMessage::recvDBTNO(QSqlDatabase thdb, QString tno)
{
    db = thdb;
    teacherNo = tno;
}

void ManagerMessage::recvMaorSuper(bool ms)
{
    MS = ms;
    if(MS)
    {
        ui->pushButton->setText("修改基本信息");
        ui->pushButton_2->show();

        ui->no->setEnabled(false);
        ui->name->setEnabled(false);
        ui->email->setEnabled(false);
        ui->major->setEnabled(false);

        setMessage();
    }
    else
    {
        ui->pushButton->setText("确定");
        ui->pushButton_2->hide();

        ui->no->clear();
        ui->name->clear();
        ui->email->clear();
    }

    ui->label_6->hide();
    ui->old->hide();
    ui->label_7->hide();
    ui->now->hide();

    static bool tf = true;
    if(tf)
    {
        QString major = "软件工程,网络工程,通信工程,计算机科学技术";
        QStringList majors = major.split(",", QString::SkipEmptyParts);
        ui->major->addItems(majors);
        tf = false;
    }
}

void ManagerMessage::setMessage()
{
    QString sql = "select TPassword, TName, TEmail, TMajor, TSuperManager from MANAGER where TeacherNo='" + teacherNo + "'";

    QSqlQuery query(db);
    query.exec(sql);
    if(query.next())
    {
        mm = query.value(0).toString();     //记录密码

        QString ts = query.value(4).toString();
        if(ts.toInt() == 1)
        {
            super = true;
            ui->label->setText(query.value(1).toString() + ",你是超级管理员!");
        }
        else
        {
            super = false;
            ui->label->setText(query.value(1).toString() + ",你是超理员!");
        }

        ui->no->setText(teacherNo.trimmed());
        ui->name->setText(query.value(1).toString().trimmed());
        ui->email->setText(query.value(2).toString().trimmed());
        ui->major->setCurrentText(query.value(3).toString().trimmed());
    }
    else
    {
        Error("查询失败!");
        this->close();
    }
}

QString ManagerMessage::selectDB(QString sql)
{
    QString message = NULL;

    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        message += query.value(0).toString();
    }

    if(message == NULL)
    {
        message = WRONG;
    }

    return message;
}

bool ManagerMessage::InsertDB(QString sql, QString style)
{
    try
    {
        QSqlQuery query(db);
        QSqlDatabase::database().transaction();   //事务开始
        query.prepare(sql);
        query.addBindValue(tr("%1").arg(ui->no->text().trimmed()));
        if(style == NULL)
            query.addBindValue(tr("%1").arg(mm));
        else
            query.addBindValue("csl123456");
        query.addBindValue(tr("%1").arg(ui->name->text().trimmed()));
        query.addBindValue(tr("%1").arg(ui->email->text().trimmed()));
        query.addBindValue(tr("%1").arg(ui->major->currentText().trimmed()));
        query.addBindValue(tr("True"));
        if(style == NULL)
            query.addBindValue(tr("True"));
        else
            query.addBindValue(tr("False"));
        query.exec();
        QSqlDatabase::database().commit();  //事务结束

    return true;
    }
    catch(std::exception e)
    {
        Error(e.what());
        return false;
    }
}

void ManagerMessage::Error(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

bool ManagerMessage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->email)   //邮箱输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->email->text().isEmpty())
            {
                Error("邮箱不能为空");
                return false;
            }
            else
            {
                QString email = ui->email->text().trimmed();
                QRegExp reg("^(([0-9a-zA-Z]+)|([0-9a-zA-Z]+[_.0-9a-zA-Z-]*[0-9a-zA-Z]+))@([a-zA-Z0-9-]+[.])+([a-zA-Z]{2}|net|NET|com|COM|gov|GOV|mil|MIL|org|ORG|edu|EDU|int|INT)$");
                if(reg.exactMatch(email))
                {
                    QString sql = "select SName from STUDENT where SEmail='"+email+"'";
                    QString tf = selectDB(sql);        //查询邮箱是否已存在
                    if(tf == WRONG)
                    {
                        QString sql1 = "select TName from MANAGER where TEmail='"+email+"'";
                        QString tf = selectDB(sql1);        //查询邮箱是否已存在
                        if(tf == WRONG)
                        {
                            return true;
                        }
                        else
                        {
                            Error("邮件已注册");
                            return false;
                        }
                    }
                    else
                    {
                        Error("邮件已注册");
                        return false;
                    }
                }
                else
                {
                    Error("邮件地址格式错误");
                    return false;
                }
            }
        }
    }
    else if(watched == ui->no)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(!MS)
            {
                QString sql1 = "select TName from MANAGER where TeacherNo='" + ui->no->text() + "'";
                QString name = selectDB(sql1);
                if(name != WRONG)
                {
                    Error("当前工号已存在!");
                    ui->no->setFocus();
                    return false;
                }
            }
        }
    }

    return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void ManagerMessage::on_pushButton_clicked()        //修改基本信息
{
    if(MS)
    {
        static bool tf = true;
        if(tf)
        {
            if(super)       //超级管理员
            {
                ui->no->setEnabled(true);
                ui->major->setEnabled(true);
            }
            ui->name->setEnabled(true);
            ui->email->setEnabled(true);
            ui->pushButton->setText("保存信息");

            tf = false;
        }
        else
        {
            if(ui->no->text().isEmpty() || ui->name->text().isEmpty() || ui->email->text().isEmpty())
            {
                Error("请完善信息!");
                return;
            }

            if(super)
            {
                //判断当前工号是否存在
                if(ui->no->text().trimmed() != teacherNo)
                {
                    QString sql1 = "select TName from MANAGER where TeacherNo='" + ui->no->text() + "'";
                    QString name = selectDB(sql1);
                    if(name != WRONG)
                    {
                        Error("当前工号已存在!");
                        return;
                    }
                }

                //删除原记录
                QString sql = "delete from MANAGER where TeacherNo='" + teacherNo + "'";
                QString del = selectDB(sql);
                if(del != WRONG)
                {
                    Error(WRONG);
                    return;
                }

                QString sql2 = "insert into MANAGER(TeacherNo, TPassword, TName, TEmail, TMajor, TManager, TSuperManager) values(?,?,?,?,?,?,?)";
                if(InsertDB(sql2, NULL))
                {
                    Error("保存成功!");
                }
                else
                {
                    Error(WRONG);
                    return;
                }

                ui->no->setEnabled(false);
                ui->major->setEnabled(false);
            }
            else
            {
                QString sql = tr("update MANAGER set TName='%1', TEmail='%2' where TeacherNo='%3'").arg(ui->name->text().trimmed()).arg(ui->email->text().trimmed()).arg(ui->no->text().trimmed());
                QString up = selectDB(sql);
                if(up != WRONG)
                {
                    Error(WRONG);
                    return;
                }

                Error("保存成功!");
            }

            ui->name->setEnabled(false);
            ui->email->setEnabled(false);
            ui->pushButton->setText("修改基本信息");
            tf = true;
        }
    }
    else
    {
        if(ui->no->text().isEmpty() || ui->name->text().isEmpty() || ui->email->text().isEmpty())
        {
            Error("请完善信息!");
            add = false;
            return;
        }

        QString sql2 = "insert into MANAGER(TeacherNo, TPassword, TName, TEmail, TMajor, TManager, TSuperManager) values(?,?,?,?,?,?,?)";
        if(InsertDB(sql2, "ADD"))
        {
            Error(tr("新增成功!默认密码%1").arg("csl123456"));
            add = true;
        }
        else
        {
            Error(WRONG);
            false;
            return;
        }

        this->close();
    }
}

void ManagerMessage::on_pushButton_2_clicked()      //修改密码
{
    static bool tf = true;
    if(tf)
    {
        ui->pushButton_2->setText("保存密码");

        ui->label_6->show();
        ui->old->show();
        ui->label_7->show();
        ui->now->show();

        tf = false;
    }
    else
    {
        if(ui->old->text().trimmed() != mm)
        {
            Error("原始密码错误!");
            return;
        }

        if(ui->now->text().isEmpty())
        {
            Error("密码不能为空!");
            return;
        }

        QRegExp reg("^[a-zA-Z]\\w{5,31}$");
        if(!reg.exactMatch(ui->now->text().trimmed()))
        {
            Error("密码须字母开头,长度6-32,只包含字符,数字,下划线");
            return;
        }

        if(ui->now->text() == mm)
        {
            Error("新密码不能和原始密码相同!");
            return;
        }

        QString sql = "update MANAGER set TPassword where TeacherNo='" + teacherNo + "'";
        QString up = selectDB(sql);
        if(up != WRONG)
        {
            Error(WRONG);
            return;
        }

        Error("更改密码成功!");

        ui->pushButton_2->setText("修改密码");
        tf = true;
    }
}

void ManagerMessage::on_pushButton_3_clicked()
{
    this->close();
}
