#include "login.h"
#include "ui_login.h"
#include "register.h"
#include <QPainter>
#include <QErrorMessage>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);     //密码输入框为暗码

    ui->number->setText("0005");
    ui->password->setText("123456");
    ui->radioButton_2->setChecked(true);

//    ui->number->setText("201316080136");
//    ui->password->setText("ai13143344");
//    ui->radioButton->setChecked(true);


    stu = new StudentMain(this);
    connect(this, SIGNAL(sendUsername(QString,QString)),stu, SLOT(setWelcome(QString,QString)));
    connect(this, SIGNAL(sendFellowMessage(QSqlDatabase)), stu, SLOT(setMessage(QSqlDatabase)));
    connect(this, SIGNAL(sendStuOrMan(int)), stu, SLOT(checkUser(int)));

    //数据库连接
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("DRIVER={SQL SERVER};SERVER=LMS-KING;DATABASE=Fellowship;uid=sa;pwd=521ai3849;");
    if(!db.open())
    {
         QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
         return;
    }
}

Login::Login(QString username)
{

}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *)   //登录窗口重绘
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(248,248,255,200));
    this->setModal(true);
}

void Login::on_login_clicked()      //登录
{
    QString username = ui->number->text();
    QString password = ui->password->text();
    if(username.isEmpty() || password.isEmpty())
    {
        error("提示", "请输入完整信息");
        return;
    }
    //没选择身份
    else if(!ui->radioButton->isChecked() && !ui->radioButton_2->isChecked())
    {
        error("提示", "请选择登录身份");
        return;
    }

    QSqlQuery query(db);
    if(ui->radioButton->isChecked())    //学生登录
    {
        query.exec("select SName from STUDENT where StudentNo ='"+ username +"'");
        if(query.next())
        {
            QMessageBox::information(this,tr("欢迎%1").arg(query.value(0).toString()),
                                         tr("登录成功!"),QMessageBox::Ok);
            //db.close();
            emit sendStuOrMan(0);
            emit sendUsername(query.value(0).toString(),ui->number->text());
            emit sendFellowMessage(db);
            this->close();

            //进入学生主界面
            stu->setWindowTitle("学生端");
            stu->show();
            return;
        }
    }
    else if(ui->radioButton_2->isChecked())     //管理员登录
    {
        query.exec("select TName, TSuperManager from MANAGER where TeacherNo ='"+ username +"'");
        if(query.next())
        {
            QMessageBox::information(this,tr("欢迎%1").arg(query.value(0).toString()),
                                         tr("登录成功!"),QMessageBox::Ok);

            if(query.value(1).toInt() == 1)     //超级管理员
                emit sendStuOrMan(2);
            else
                emit sendStuOrMan(1);
            emit sendUsername(query.value(0).toString(),ui->number->text());
            emit sendFellowMessage(db);
            this->close();

            //进入教师主界面
            stu->setWindowTitle("教师端");
            stu->show();
            return;
        }
    }

    QMessageBox::information(this,"登录失败",
                                     tr("用户名或密码错误!"),QMessageBox::Ok);
    //db.close();
    return;
}

void Login::error(QString title, QString messages)
{
    QErrorMessage *message = new QErrorMessage(this);
    message->setWindowTitle(title);
    message->showMessage(messages);
    return;
}

void Login::setUsername(QString username) //注册成功后返回登录页面，自动设置刚刚注册的用户名
{
    ui->number->setText(username);
}

void Login::on_register_2_clicked()     //注册
{
    Register *reg = new Register();
    reg->setWindowTitle("注册");
    reg->setModal(true);
    reg->show();

    connect(this, SIGNAL(sendDatabaseToRegister(QSqlDatabase)), reg, SLOT(recvDatabaseFromLogin(QSqlDatabase)));
    emit sendDatabaseToRegister(db);
}

void Login::on_out_clicked()        //退出
{
    this->close();
}

