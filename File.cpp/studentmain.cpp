#include "studentmain.h"
#include "ui_studentmain.h"
#include "score.h"
#include "scoreparts.h"
#include <Qtimer>
#include <QTime>
#include <QDate>
#include <QLabel>
#define WRONG "操作失败!"

StudentMain::StudentMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentMain)
{
    ui->setupUi(this);
    ui->lcdNumber->setPalette(Qt::blue);     //时间显示框颜色

    //设置字体
    font.setPixelSize(15);
    ui->textEdit->setFont(font);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changePicture()));
    timer->start(1000); //1s

    ui->statusbar->showMessage(tr("欢迎登计通奖学金评定系统"),1000000);        //临时标签10s

    ui->First->setPixmap(QPixmap(":/picture/F:/下载/qq图像/one.jpg"));  //首页
    ui->Second->setPixmap(QPixmap(":/picture/F:/下载/qq图像/two.jpg"));  //第二页
    ui->Thred->setPixmap(QPixmap(":/picture/F:/下载/qq图像/three.jpg"));    //第三页
}

StudentMain::~StudentMain()
{
    thisdb.close();
    delete ui;
}

void StudentMain::checkUser(int style)
{
    ado = new ALLDoFellowship(this);   //教师编辑奖学金，课程信息，学生查看奖学金详细信息
    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), ado, SLOT(recvDBTNO(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendFellOrClass(bool,bool)), ado, SLOT(recvFellOrClass(bool,bool)));

    if(style == 0)        //学生
    {
        app = new ApplicationFellow(this);      //申请
        record = new Application(this);
        per = new PersonMessage(this);
        connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), app, SLOT(Database(QSqlDatabase,QString)));
        connect(this, SIGNAL(sendStyle(QString)), app, SLOT(recvStyle(QString)));
        connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), record, SLOT(recvSDBStuNO(QSqlDatabase,QString)));
        connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), per, SLOT(recvDbStuno(QSqlDatabase,QString)));
        return;
    }

    setBar(style);

    apper = new ApplicaterMessage(this);        //申请人信息，申请人成绩，所有人信息
    aah = new AllScoAndHistory(this);           //所有人成绩，发放记录，恢复资格
    mm = new ManagerMessage(this);              //个人中心，增删管理员
    aord = new ADDorDELManager(this);           //特权，增删管理员
    df = new DoFellow(this);                    //奖学金发放

    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), apper,SLOT(recvDbNum(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendMgOrSr(bool,bool)), apper, SLOT(recvMeageOrSCo(bool,bool)));

    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), aah, SLOT(recvDBNo(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendScoOrHis(bool, bool)), aah, SLOT(recvScoOHis(bool,bool)));

    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), mm, SLOT(recvDBTNO(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendMaorSuper(bool)), mm, SLOT(recvMaorSuper(bool)));

    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), aord, SLOT(recvDBTNO(QSqlDatabase,QString)));

    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), df, SLOT(recvMessage(QSqlDatabase,QString)));
}

void StudentMain::changePicture()       //更换图片
{
    static int page = 0;
    static int time = 0;
    static int setmeg = 0;
    time++;
    setmeg++;
    ui->stackedWidget->setCurrentIndex(page);
    if(time % 3 == 0)
        page++;
    if(page == 3)
        page = 0;

    if(setmeg == 300)       //五分钟更新一次界面通知
    {
        setMessage(thisdb);
        setmeg = 0;
    }

    QDateTime curtime = QDateTime::currentDateTime();//获取系统现在的时间
    QString strTime = curtime.toString("yyyy-MM-dd hh:mm:ss");//设置系统时间显示格式
    ui->lcdNumber->display(strTime);//在lcdNumber上显示时间
}

void StudentMain::setWelcome(QString username, QString number)      //设置信息
{
    StudentNo = number;

    QLabel * label = new QLabel(this);                          //永久标签
    label->setFrameStyle(QFrame::Box|QFrame::Sunken);
    label->setText(username + ",欢迎登录");
    ui->statusbar->addPermanentWidget(label);
}

void StudentMain::setMessage(QSqlDatabase db)      //设置主界面通知信息
{
    static bool tf = true;
    if(tf)
    {
        thisdb = db;        //数据库绑定
        tf = false;
    }

    try
    {
        //当前时间
        QDateTime curtime = QDateTime::currentDateTime();//获取系统现在的时间
        QString strTime = curtime.toString("yyyy-MM-dd");//设置系统时间显示格式

        QString sql1 = tr("select Style,StartTime, EndTime from FELLOWSHIP where (convert(date, EndTime) > convert(varchar(32), '%1'))").arg(strTime);

        ui->textEdit->setText(Database("FM",sql1));
    }
    catch(std::exception e)
    {
        thisdb.close();
        QMessageBox::information(this,"异常",e.what(),QMessageBox::Ok);
        return;
    }
}

QString StudentMain::Database(QString style, QString sql)
{
    QString message = NULL;
    QSqlQuery query(thisdb);
    QString sty = NULL;

    if(style == "FM")       //主界面奖学金信息
    {
        int i = 1;
        query.exec(sql);
        while(query.next())
        {
            message += QString::number(i) + ":" + query.value(0).toString().left(query.value(0).toString().length() - 1) + ":" + query.value(1).toString() + "--" + query.value(2).toString() + "可申请!\n\n";
            sty += query.value(0).toString().trimmed();

            i++;
        }

        styles = sty.split(",", QString::SkipEmptyParts);
    }
    else if(style == "PF")      //申请奖学金，查看是否有资格,查看申请记录
    {
        query.exec(sql);
        if(query.next())
        {
            message = query.value(0).toString();
        }
        else
            message = WRONG;
    }
    else
        message = WRONG;

    return message;
}

void StudentMain::on_action_triggered()     //申请奖学金
{
    //判断当前是否发布申请奖学金通知
    if(styles.size() == 0)
    {
        QMessageBox::information(this, "提示", "当前没有可以申请的奖学金!",QMessageBox::Ok);
        return;
    }

    QString sql1 = "select SPriority from STUDENT where StudentNo='"+StudentNo+"'";
    QString iscan = Database("PF", sql1);

    if(iscan == "0")            //没有资格申请
    {
        QMessageBox::information(this,"提示","对不起,你当前没有资格申请,请到信息栏联系管理员!",QMessageBox::Ok);
        return;
    }

    //查看当前有没有该专业和年级的奖学金信息
    QString CanFellow = NULL;
    int size = styles.size();

    QString sql = "select SMajor from STUDENT where StudentNo='" + StudentNo + "'";
    QString major = Database("PF", sql);
    if(major != WRONG)
    {
        while (size--)
        {
            QString sql1 = tr("select Number from ASSIGN where Style='%1' and SMajor='%2'").arg(styles.at(size)).arg(major);

            QString number = Database("PF", sql1);
            if(number != WRONG)
            {
                if(number == "0000")
                {
                    CanFellow += styles.at(size);
                    continue;
                }
                else
                {
                    //当前时间,判断当前学生所处的学期
                    QDateTime curtime = QDateTime::currentDateTime();//获取系统现在的时间
                    QString strTime = curtime.toString("yyyy-MM-dd");//设置系统时间显示格式
                    QString grade = checkTime(strTime);
                    QString sql2 = tr("select Count from ASSIGN2 where number='%1' and Grade='%2'").arg(number).arg(grade.left(2).trimmed());

                    QString count = Database("PF", sql2);
                    if(count.toInt() >= 0)
                    {
                        CanFellow += styles.at(size);
                    }
                }
            }
        }
    }
    else
    {
        QMessageBox::information(this,"提示","对不起,目前没有你可以申请的奖学金信息!",QMessageBox::Ok);
        return;
    }

    if(CanFellow == NULL)
    {
        QMessageBox::information(this, "提示", "对不起,目前没有你可以申请的奖学金信息!",QMessageBox::Ok);
        return;
    }

    app->setWindowTitle("资料完善");
    app->show();
    emit sendMessage(thisdb, StudentNo);       //将已经连接的数据库传递过去
    emit sendStyle(CanFellow);

    return;
}

//判断学生当前所处学期
QString StudentMain::checkTime(QString time)
{
    int selectyear = time.left(4).toInt();
    int selectmonth = time.mid(5,2).toInt();

    QString sql = "select STime from STUDENT where StudentNo='"+ StudentNo +"'";
    QString times = Database("PF", sql);
    if(times == WRONG)
    {
        Error(WRONG);
        return WRONG;
    }
    int bgnyear = times.left(4).toInt();        //入学年份
    int bgnmonth = times.mid(5,2).toInt();        //入学月份，默认9

    int year = selectyear - bgnyear;
    QString cla = NULL;
    QString sql2 = tr("select class from GRADE where year=%1").arg(year);
    QSqlQuery query(thisdb);
    query.exec(sql2);
    while(query.next())
    {
        cla += query.value(0).toString().trimmed();
    }

    if(cla == NULL)
    {
        Error("请确定你是否仍是在读学生!");
        return false;
    }

    QStringList ql = cla.split(",", QString::SkipEmptyParts);

    QString classs = NULL;
    if(selectmonth >= bgnmonth)
    {
        classs += ql.at(1) + ",";
    }
    else
    {
        classs += ql.at(0) + ",";
    }

    return classs;
}

void StudentMain::on_action_5_triggered()       //申请记录
{
    QString sql = "select id from APPLICATION where StudentNo='" + StudentNo + "'";
    QString id = Database("PF", sql);
    if(id == WRONG)
    {
        Error("你当前没有申请记录!");
        return;
    }

    emit sendMessage(thisdb, StudentNo);
    record->setWindowTitle("申请记录");
    record->setModal(true);
    record->show();
}

void StudentMain::Error(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

//查看奖学金详情
void StudentMain::on_action_4_triggered()
{
    bool FC = true;
    bool LF = true;
    emit sendMessage(thisdb, StudentNo);
    emit sendFellOrClass(FC,LF);

    ado->setWindowTitle("奖学金信息");
    ado->setModal(true);
    ado->show();
}

void StudentMain::on_action_19_triggered()      //关于系统
{
    //
}

void StudentMain::on_action_2_triggered()  //个人信息
{
    emit sendMessage(thisdb, StudentNo);
    per->setWindowTitle("个人主页");
    per->setModal(true);
    per->show();
}

void StudentMain::on_action_3_triggered()       //查看智育成绩
{
    scoOrM = true;
    Score *sco = new Score(this);
    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), sco, SLOT(recvDbStu(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendManager(bool)), sco, SLOT(recvManager(bool)));
    emit sendMessage(thisdb, StudentNo);
    emit sendManager(scoOrM);

    sco->setWindowTitle("成绩记录");
    sco->setModal(true);
    sco->show();
}

void StudentMain::on_action_17_triggered()      //联系管理员
{
    scoOrM = false;
    Score *sco = new Score(this);
    connect(this, SIGNAL(sendMessage(QSqlDatabase,QString)), sco, SLOT(recvDbStu(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendManager(bool)), sco, SLOT(recvManager(bool)));
    emit sendMessage(thisdb, StudentNo);
    emit sendManager(scoOrM);

    sco->setWindowTitle("成绩记录");
    sco->setModal(true);
    sco->show();
}

void StudentMain::on_action_14_triggered()      //退出系统
{
    this->close();
}


void StudentMain::closeEvent(QCloseEvent *event)
{
    QMessageBox tip(QMessageBox::Warning,tr("提示"),tr("是否确认退出"),QMessageBox::Yes | QMessageBox::No, NULL);
    if (tip.exec() == QMessageBox::No)
    {
        event->ignore();
        return;
    }
    else
    {
        event->accept();
    }

}

void StudentMain::setBar(int man)           //管理员登录，重设目录栏
{
    ui->menubar->clear();       //清除没登陆前的菜单栏的所有事件

    QMenu *check = new QMenu(ui->menubar);         //添加新菜单
    check = ui->menubar->addMenu(tr("信息查看"));
    QAction *message = check->addAction(tr("申请人信息"));       //添加新事件
    QAction *score = check->addAction(tr("申请人成绩"));     //添加新事件
    QAction *allsocre = check->addAction(tr("所有学生成绩"));     //添加新事件
    QAction *allmessage = check->addAction(tr("所有学生信息"));     //添加新事件
    QAction *priority = check->addAction(tr("恢复申请资格"));     //添加新事件

    QMenu *dowork = new QMenu(ui->menubar);        //添加新菜单
    dowork = ui->menubar->addMenu(tr("奖学金/课程"));
    QAction *classmge = dowork->addAction(tr("录入课程信息"));    //添加新事件
    QAction *fellow = dowork->addAction(tr("编辑奖学金信息"));    //添加新事件
    QAction *newfellow = dowork->addAction(tr("奖学金发放"));    //添加新事件
    QAction *odfellow = dowork->addAction(tr("往届奖学金发放记录"));  //添加新事件

    QMenu *people = new QMenu(ui->menubar);        //添加新菜单
    people = ui->menubar->addMenu(tr("个人中心"));
    QAction *center = people->addAction(tr("个人信息"));      //添加新事件


    if(man == 2)
    {
        QMenu *super = new QMenu(ui->menubar);        //添加新菜单
        super = ui->menubar->addMenu(tr("特权"));
        QAction *superpriority = super->addAction(tr("增设/删除管理员"));  //添加新事件
        //增删管理员
        connect(superpriority, SIGNAL(triggered(bool)), this, SLOT(AddorDEL()));
    }

    QMenu *out = new QMenu(ui->menubar);        //添加新菜单
    out = ui->menubar->addMenu(tr("退出登录"));
    QAction *safeout = out->addAction(tr("安全退出"));      //添加新事件

    //给新添加的事件关联处理函数
    //查看申请人信息
   connect(message,SIGNAL(triggered(bool)),this,SLOT(lookMessageForApplication()));
    //查看申请人成绩
    connect(score, SIGNAL(triggered(bool)), this, SLOT(lookScoreForApplication()));

    //查看所有学生智育成绩
    connect(allsocre, SIGNAL(triggered(bool)), this, SLOT(lookAllScore()));

    //查看往届发放情况
    connect(odfellow, SIGNAL(triggered(bool)), this, SLOT(lookHistory()));

    //查询所有学生信息
    connect(allmessage, SIGNAL(triggered(bool)), this, SLOT(loolAllMessage()));

    //恢复资格
    connect(priority, SIGNAL(triggered(bool)), this, SLOT(ResetPriority()));

    //编辑奖学金信息
    connect(fellow, SIGNAL(triggered(bool)), this, SLOT(Fellow()));

    //录入课程信息
    connect(classmge, SIGNAL(triggered(bool)), this, SLOT(Class()));

    //管理员信息
    connect(center, SIGNAL(triggered(bool)), this, SLOT(ManagerM()));

    //退出
    connect(safeout,SIGNAL(triggered(bool)), this, SLOT(close()));

    //奖学金发放
    connect(newfellow, SIGNAL(triggered(bool)), this, SLOT(Dofew()));
}

//查看申请人信息
void StudentMain::lookMessageForApplication()
{
    bool Mg = true;
    bool Fmg = false;
    emit sendMessage(thisdb, StudentNo);
    emit sendMgOrSr(Mg, Fmg);
    apper->setWindowTitle("申请信息");
    apper->setModal(true);
    apper->show();
}

//查看申请人成绩
void StudentMain::lookScoreForApplication()
{
    bool Mg = false;
    bool Fmg = false;
    emit sendMessage(thisdb, StudentNo);
    emit sendMgOrSr(Mg, Fmg);
    apper->setWindowTitle("成绩信息");
    apper->setModal(true);
    apper->show();

    ScoreParts *sp = new ScoreParts(this);
    sp->setWindowTitle("成绩比例");
    sp->setModal(true);
    sp->show();
}

//查看所有人信息
void StudentMain::loolAllMessage()
{
    bool Mg = true;
    bool Fmg = true;
    emit sendMessage(thisdb, StudentNo);
    emit sendMgOrSr(Mg, Fmg);
    apper->setWindowTitle("基本信息");
    apper->setModal(true);
    apper->show();
}

//查看所有学生智育成绩
void StudentMain::lookAllScore()
{
    bool SH = true;
    bool re = false;
    emit sendMessage(thisdb, StudentNo);
    emit sendScoOrHis(SH, re);
    aah->setWindowTitle("成绩信息");
    aah->setModal(true);
    aah->show();
}

//查看所有发放历史
void StudentMain::lookHistory()
{
    bool SH = false;
    bool re = false;
    emit sendMessage(thisdb, StudentNo);
    emit sendScoOrHis(SH, re);
    aah->setWindowTitle("发放历史");
    aah->setModal(true);
    aah->show();
}

//恢复某人资格
void StudentMain::ResetPriority()
{
    bool SH = true;
    bool re = true;
    emit sendMessage(thisdb, StudentNo);
    emit sendScoOrHis(SH, re);
    aah->setWindowTitle("恢复资格");
    aah->setModal(true);
    aah->show();
}

//编辑奖学金信息
void StudentMain::Fellow()
{
    bool FC = true;
    bool LF = false;
    emit sendMessage(thisdb, StudentNo);
    emit sendFellOrClass(FC, LF);

    ado->setWindowTitle("奖学金信息");
    ado->setModal(true);
    ado->show();
}

//编辑课程信息
void StudentMain::Class()
{
    bool FC = false;
    bool LF = false;
    emit sendMessage(thisdb, StudentNo);
    emit sendFellOrClass(FC,LF);

    ado->setWindowTitle("课程信息");
    ado->setModal(true);
    ado->show();
}

//管理员信息
void StudentMain::ManagerM()
{
    bool MS = true;
    emit sendMessage(thisdb, StudentNo);
    emit sendMaorSuper(MS);

    mm->setWindowTitle("个人中心");
    mm->setModal(true);
    mm->show();
}

//增删管理员
void StudentMain::AddorDEL()
{
    emit sendMessage(thisdb, StudentNo);

    aord->setWindowTitle("管理员中心");
    aord->setModal(true);
    aord->show();
}

//奖学金发放
void StudentMain::Dofew()
{
    emit sendMessage(thisdb, StudentNo);

    df->setWindowTitle("奖学金发放");
    df->setModal(true);
    df->show();
}
