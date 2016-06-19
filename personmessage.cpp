#include "personmessage.h"
#include "ui_personmessage.h"
#include <QMessageBox>
#include <QRegExp>
#define WRONG "数据库访问错误!"

PersonMessage::PersonMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonMessage)
{
    ui->setupUi(this);

    ui->stu->setEnabled(false);
    ui->name->setEnabled(false);
    ui->tel->setEnabled(false);
    ui->email->setEnabled(false);
    ui->major->setEnabled(false);
    ui->time->setEnabled(false);

    ui->label_8->hide();
    ui->label_9->hide();
    ui->old->hide();
    ui->new_2->hide();

    ui->email->installEventFilter(this);
}

PersonMessage::~PersonMessage()
{
    delete ui;
}

void PersonMessage::recvDbStuno(QSqlDatabase thisdb, QString stu)
{
    db = thisdb;
    studentNo = stu;

    setMessage();
}

void PersonMessage::setMessage()
{
    QString sql = "select StudentNo, SName, STel, SEmail, SMajor, STime from STUDENT where StudentNo='" + studentNo + "'";
    QSqlQuery query(db);
    query.exec(sql);
    if(query.next())
    {
        ui->stu->setText(query.value(0).toString());
        ui->name->setText(query.value(1).toString());
        ui->tel->setText(query.value(2).toString());
        ui->email->setText(query.value(3).toString());
        ui->major->setText(query.value(4).toString());
        ui->time->setText(query.value(5).toString());
    }
}

void PersonMessage::Error(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

QString PersonMessage::selectDB(QString sql)
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

bool PersonMessage::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->email)   //邮箱输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->email->text().isEmpty())
            {
                ("邮箱不能为空");
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

    return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void PersonMessage::on_pushButton_clicked()
{
    static bool tf = true;
    if(tf)
    {
        ui->name->setEnabled(true);
        ui->tel->setEnabled(true);
        ui->email->setEnabled(true);

        ui->pushButton->setText("保存");
        tf = false;
        return;
    }

    if(ui->email->text().isEmpty() || ui->name->text().isEmpty() || ui->tel->text().isEmpty())
    {
        Error("请完善信息!");
        return;
    }

    QString sql = tr("update STUDENT set SName='%1', STel='%2', SEmail='%3' where StudentNo='%4'").arg(ui->name->text().trimmed()).arg(ui->tel->text().trimmed()).arg(ui->email->text().trimmed()).arg(studentNo);

    try
    {
        QSqlQuery query(db);
        query.exec(sql);
    }
    catch(std::exception e)
    {
        QMessageBox::information(this, "提示", e.what(), QMessageBox::Ok);
        return;
    }

    ui->pushButton->setText("修改基本信息");
    ui->name->setEnabled(false);
    ui->tel->setEnabled(false);
    ui->email->setEnabled(false);

    Error("修改信息成功!");
    tf = true;
}

void PersonMessage::on_pushButton_3_clicked()       //修改密码
{
    static bool tf = true;
    if(tf)
    {
        ui->old->clear();
        ui->new_2->clear();

        ui->old->setEchoMode(QLineEdit::Password);
        ui->new_2->setEchoMode(QLineEdit::Password);

        ui->pushButton_3->setText("保存");
        ui->label_8->show();
        ui->label_9->show();
        ui->old->show();
        ui->new_2->show();
        tf = false;
        return;
    }

    QString sql = "select SPassword from STUDENT where StudentNo='" + studentNo + "'";
    QString pas = selectDB(sql);
    if(pas.compare(ui->old->text().trimmed()) != 0)
    {
        Error("原始密码错误!");
        return;
    }

    if(ui->new_2->text().isEmpty())
    {
        Error("密码不能为空!");
        return;
    }

    QRegExp reg("^[a-zA-Z]\\w{5,31}$");
    if(!reg.exactMatch(ui->new_2->text().trimmed()))
    {
        Error("密码须字母开头,长度6-32,只包含字符,数字,下划线");
        return;
    }

    if(ui->old->text().trimmed() == ui->new_2->text().trimmed())
    {
        Error("新密码不能和原始密码相同!");
        return;
    }

    int ret = QMessageBox::information(this, "提示", (tr("修改后密码：%1").arg(ui->new_2->text() + "是否确认修改？")), QMessageBox::Ok, QMessageBox::No);
    if(ret == 0)
    {
        return;
    }

    QString sql2 = "update STUDENT set SPassword='" + ui->new_2->text().trimmed() + "' where StudentNo='" + studentNo + "'";
    QString up = selectDB(sql);
    if(up != WRONG)
    {
        Error(WRONG);
        return;
    }

    ui->pushButton_3->setText("修改密码");
    ui->label_8->hide();
    ui->label_9->hide();
    ui->old->hide();
    ui->new_2->hide();
    Error("修改密码成功!");

    tf = true;
}

void PersonMessage::on_pushButton_2_clicked()
{
    this->close();
}
