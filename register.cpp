#include "register.h"
#include "ui_register.h"
#include <QPalette>
#include <QPainter>
#include <QRegExp>
#include <QMessageBox>
#include <QTimer>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    passcount = 0;
    ui->register_2->setEnabled(false);  //信息不完整时不可以注册
    ui->timetable->hide();

    //透明窗体
    this->setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowSystemMenuHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QTimer *timer = new QTimer(this);       //实时判断密码是否可见
    connect(timer,SIGNAL(timeout()),this,SLOT(TimeOut()));//关联定时器满信号和相应的槽函数
    timer->start(100);//定时器启动，其中100表示100ms

    ui->password->setEchoMode(QLineEdit::Password);     //密码输入框为暗码
    ui->password_3->setEchoMode(QLineEdit::Password);   //密码输入框为暗码

    //设置提示标签为红色
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->studentno_2->setPalette(pe);
    ui->password_2->setPalette(pe);
    ui->name_2->setPalette(pe);
    ui->sex_2->setPalette(pe);
    ui->tel_2->setPalette(pe);
    ui->email_2->setPalette(pe);
    ui->major_2->setPalette(pe);
    ui->time_3->setPalette(pe);

    //安装事件过滤器
    ui->studentno->installEventFilter(this);
    ui->password->installEventFilter(this);
    ui->password_3->installEventFilter(this);
    ui->name->installEventFilter(this);
    ui->tel->installEventFilter(this);
    ui->email->installEventFilter(this);

    ui->comboBox->insertItem(0, "男");
    ui->comboBox->insertItem(1, "女");

    QString major = "软件工程,网络工程,通信工程,计算机科学技术";
    QStringList majors = major.split(",", QString::SkipEmptyParts);
    ui->comboBox_2->addItems(majors);
}

Register::~Register()
{
    delete ui;
}

void Register::paintEvent(QPaintEvent *)    //窗口重绘
{
    QPainter painter(this);
    painter.fillRect(rect(),QColor(248,248,255, 255));
    this->setModal(true);
}

bool Register::eventFilter(QObject *watched, QEvent *event)     //事件过滤器
{
    if(watched == ui->studentno)     //账号输入框
    {
        if(event->type() == QEvent::FocusOut)     //控件失去焦点事件
        {
            if(ui->studentno->text().isEmpty())
            {
                ui->studentno_2->setText(tr("账号不能为空"));
                return false;
            }

            QString sno = ui->studentno->text().trimmed();       //待注册的学号
            QString sql = "select SPassword from STUDENT where StudentNo='"+ sno +"'";
            bool tf = Database(sql);      //查询账号是否存在

            if(!tf) //用户名已存在
            {
                 ui->studentno_2->setText(tr("账号已存在!"));
                 return false;
            }
            else
            {
                if(ui->studentno_2->text() == "通过")
                    return true;

                 ui->studentno_2->setText("通过");
                 passcount++;
                 return true;
            }
        }
    }
    else if(watched == ui->password)         //密码输入框
    {
        if(event->type()==QEvent::FocusOut)    //控件失去焦点事件
        {
             if(ui->password->text().isEmpty())
             {
                 ui->password_2->setText(tr("密码不能为空"));
                 return false;
             }
             else
             {
                 QString paw = ui->password->text();
                 QRegExp reg("^[a-zA-Z]\\w{5,31}$");
                 if(reg.exactMatch(paw))
                 {
                     if(ui->password_2->text() == "通过")
                         return true;

                     ui->password_2->setText("通过");
                     passcount++;
                     return true;
                 }
                 else
                 {
                     ui->password_2->setText("字母开头,长度6-32,只包含字符,数字,下划线");
                     return false;
                 }
             }
        }
    }
    else if(watched == ui->password_3)         //密码输入框
    {
        if(event->type()==QEvent::FocusOut)    //控件失去焦点事件
        {
             if(ui->password_3->text().isEmpty())
             {
                 ui->password_2->setText(tr("请再次输入密码"));;
                 return false;
             }
             else
             {
                 if(ui->password_3->text() != ui->password->text())
                 {
                     ui->password_2->setText("输入密码不一致");
                     return false;
                 }
                 else
                 {
                     if(ui->password_2->text() == "通过")
                         return true;

                     ui->password_2->setText("通过");
                     passcount++;
                     return true;
                 }
             }
        }
    }
    else if (watched == ui->name)   //姓名输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->name->text().isEmpty())
            {
                ui->name_2->setText(tr("姓名不能为空"));
                return false;
            }
            else
            {
                QString name = ui->name->text();
                QRegExp reg("^[\u4e00-\u9fa5]+$");      //匹配中文字符串
                if(reg.exactMatch(name))
                {
                    if(ui->name_2->text() == "通过")
                        return true;

                    ui->name_2->setText("通过");
                    passcount++;
                    return true;
                }
                else
                {
                    ui->name_2->setText("请输入身份证姓名");
                    return false;
                }
            }
        }
    }
    else if (watched == ui->tel)   //电话输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->tel->text().isEmpty())
            {
                ui->tel_2->setText(tr("电话不能为空"));
                return false;
            }
            else
            {
                QString tel = ui->tel->text();
                QRegExp reg("^\\d{11}");
                if(reg.exactMatch(tel))
                {
                    if(ui->tel_2->text() == "通过")
                        return true;

                    ui->tel_2->setText(tr("通过"));
                    passcount++;
                    return true;
                }
                else
                {
                    ui->tel_2->setText(tr("电话号码错误"));
                    return false;
                }
            }
        }
    }
    else if (watched == ui->email)   //邮箱输入框
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->email->text().isEmpty())
            {
                ui->email_2->setText(tr("邮箱不能为空"));
                return false;
            }
            else
            {
                QString email = ui->email->text().trimmed();
                QRegExp reg("^(([0-9a-zA-Z]+)|([0-9a-zA-Z]+[_.0-9a-zA-Z-]*[0-9a-zA-Z]+))@([a-zA-Z0-9-]+[.])+([a-zA-Z]{2}|net|NET|com|COM|gov|GOV|mil|MIL|org|ORG|edu|EDU|int|INT)$");
                if(reg.exactMatch(email))
                {
                    QString sql = "select SName from STUDENT where SEmail='"+email+"'";
                    bool tf = Database(sql);        //查询邮箱是否已存在
                    if(tf)
                    {
                        QString sql1 = "select TName from MANAGER where TEmail='"+email+"'";
                        bool tf = Database(sql1);        //查询邮箱是否已存在
                        if(tf)
                        {
                            if(ui->email_2->text() == "通过")
                                return true;

                            ui->email_2->setText("通过");
                            passcount++;
                            return true;
                        }
                        else
                        {
                            ui->email_2->setText("邮件已注册");
                            return false;
                        }
                    }
                    else
                    {
                        ui->email_2->setText("邮件已注册");
                        return false;
                    }
                }
                else
                {
                    ui->email_2->setText("邮件地址错误");
                    return false;
                }
            }
        }
    }

    return QWidget::eventFilter(watched,event);     // 最后将事件交给上层对话框
}

void Register::on_register_2_clicked()      //注册
{
    if(UserRegister())
    {
        QMessageBox::information(this,"提示", tr("注册成功!"),QMessageBox::Ok);
        this->close();
        return;
    }

    QMessageBox::information(this,"提示", tr("注册失败!"),QMessageBox::Ok);
}

void Register::on_out_clicked()     //取消
{
    this->close();
}

void Register::TimeOut()
{
    if(ui->repassword->isChecked())
    {
        ui->password->setEchoMode(QLineEdit::Normal);
        ui->password_3->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->password->setEchoMode(QLineEdit::Password);
        ui->password_3->setEchoMode(QLineEdit::Password);
    }

    if(!ui->time->text().isEmpty())
    {
        if(ui->time_3->text() != "通过")
        {
            ui->time_3->setText(tr("通过"));
            passcount++;
        }
    }

    if(passcount >= 7)
        ui->register_2->setEnabled(true);
    else
       ui->register_2->setEnabled(false);

    ui->register_2->setFocus();
}

bool Register::Database(QString sql)
{
    QSqlQuery query(db);
    query.exec(sql);
    if(query.next())
    {
        return false;
    }

    return true;
}

bool Register::UserRegister()       //注册数据库操作
{
       QSqlQuery query(db);
       QSqlDatabase::database().transaction();   //事务开始
       query.prepare("insert into STUDENT(StudentNo,SPassword,SName,SSex,STel,SEmail,SMajor,SPriority,STime) values(?,?,?,?,?,?,?,?,?)");
       query.addBindValue(tr("%1").arg(ui->studentno->text().trimmed()));
       query.addBindValue(tr("%1").arg(ui->password->text().trimmed()));
       query.addBindValue(tr("%1").arg(ui->name->text().trimmed()));
       if(ui->comboBox->currentText() == "男")
          query.addBindValue(tr("false"));
       else
          query.addBindValue(tr("true"));
       query.addBindValue(tr("%1").arg(ui->tel->text().trimmed()));
       query.addBindValue(tr("%1").arg(ui->email->text().trimmed()));
       query.addBindValue(tr("%1").arg(ui->comboBox_2->currentText()));
       query.addBindValue(tr("True"));
       query.addBindValue(tr("%1").arg(ui->time->text()));
       query.exec();
       QSqlDatabase::database().commit();  //事务结束

       return true;
}

void Register::recvDatabaseFromLogin(QSqlDatabase dbl)   //槽函数，获取数据库
{
    db = dbl;
}

void Register::on_time_2_clicked()      //点击选择时间
{
    this->resize(462, 570);
    ui->timetable->show();
}

void Register::on_timetable_clicked(const QDate &date)
{
    QString time = date.toString("yyyy-MM-dd");
    int year = time.left(4).toInt();        //选择时间的年份
    //int month = time.mid(5,2).toInt();      //选择时间的月份
    QDateTime curtime = QDateTime::currentDateTime();   //获取系统现在的时间
    QString strTime = curtime.toString("yyyy-MM-dd");      //设置系统时间显示格式
    int curyear = strTime.left(4).toInt();   //当前年份


    if(strTime.compare(time) < 0)
    {
        QMessageBox::information(this, "提示", "入学时间不能在当前时间之后", QMessageBox::Ok);
        return;
    }
    else if(curyear - year > 11)    //当前年份不能大于入学年份超过11年
    {
        QMessageBox::information(this, "提示", "时间错误", QMessageBox::Ok);
        return;
    }

    ui->time->setText(time);
    this->resize(462, 445);
    ui->timetable->hide();
    ui->register_2->setFocus();
}
