#include "addordelmanager.h"
#include "ui_addordelmanager.h"
#include <QMessageBox>
#include <QTimer>
#include "passwsure.h"
#define WRONG "数据库访问错误!"

QString SUPAW;
bool add;

ADDorDELManager::ADDorDELManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ADDorDELManager)
{
    ui->setupUi(this);

    ui->label_2->hide();

    QTimer *time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(timeOut()));
    time->start(100);

    mm = new ManagerMessage(this);
    connect(this, SIGNAL(sendDBTNO(QSqlDatabase,QString)), mm, SLOT(recvDBTNO(QSqlDatabase,QString)));
    connect(this, SIGNAL(sendMaorSuper(bool)), mm, SLOT(recvMaorSuper(bool)));
}

ADDorDELManager::~ADDorDELManager()
{
    delete ui;
}

void ADDorDELManager::recvDBTNO(QSqlDatabase thdb, QString tno)
{
    db = thdb;
    teacherNo = tno;

    setRecord();
}

void ADDorDELManager::timeOut()     //计时器
{
    if(!SUPAW.isEmpty())
    {
        checkPass();
    }
    else
    {
        emit sendToPass();
    }

    if(add)
    {
        setRecord();
        add = false;
    }
}

void ADDorDELManager::setRecord()
{
    QString sql = tr("select TeacherNo, TName, TEmail, TMajor from MANAGER where TManager=%1 and TSuperManager=%2").arg(1).arg(0);

    ShowRecord(sql);
}

void ADDorDELManager::ShowRecord(QString sql)
{
    ticket_model = new QStandardItemModel();
    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("工号")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("姓名")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("专业")));
    ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("邮箱")));

    //设置时间显示列的宽度
    ui->tableView->setColumnWidth(2,145);
    ui->tableView->setColumnWidth(3,145);

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
        ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
        ticket_model->setItem(i, 1, new QStandardItem(query.value(1).toString().trimmed()));
        ticket_model->setItem(i, 2, new QStandardItem(query.value(3).toString().trimmed()));
        ticket_model->setItem(i, 3, new QStandardItem(query.value(2).toString().trimmed()));

        //设置单元格文本居中
        ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        i++;
    }

    if(i == 0)
    {
        ui->label_2->show();
    }
    else
        ui->label_2->hide();
}

void ADDorDELManager::checkPass()
{
    try
    {
        QString sql1 = "select TPassword from MANAGER where TeacherNo='" + teacherNo + "'";

        QString pa = selectDB(sql1);
        if(pa == WRONG)
        {
            QMessageBox::information(this, "提示", WRONG, QMessageBox::Ok);
        }
        else if(pa.compare(SUPAW) != 0)
        {
            QMessageBox::information(this, "提示", "密码错误!", QMessageBox::Ok);
        }
        else
        {
            QString sql = "delete from MANAGER where TeacherNo='" + stno + "'";
            QString del = selectDB(sql);
            if(del != WRONG)
                QMessageBox::information(this, "提示", "删除失败!", QMessageBox::Ok);
            else
            {
                QMessageBox::information(this, "提示", "删除成功!", QMessageBox::Ok);
                emit sendPassSure();
                setRecord();
            }
        }
    }
    catch(std::exception e)
    {
        QMessageBox::information(this, "提示", e.what(), QMessageBox::Ok);
        SUPAW.clear();
        return;
    }

    SUPAW.clear();
    return;
}

QString ADDorDELManager::selectDB(QString sql)
{
    QString message = NULL;
    QSqlQuery query(db);
    query.exec(sql);
    if(query.next())
    {
        message = query.value(0).toString().trimmed();
    }
    else
        message = WRONG;

    return message;
}


void ADDorDELManager::on_pushButton_2_clicked()     //删除管理员
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,0);
    QVariant data = model->data(index);
    QString recd = data.toString().trimmed();   //获得选中行工号

    stno = recd;

    if(recd.isEmpty())
    {
        QMessageBox::information(this, "提示", "请选择管理员", QMessageBox::Ok);
        return;
    }

    PasswSure *paw = new PasswSure(this);
    connect(this, SIGNAL(sendToPass()), paw, SLOT(recvSuper()));
    connect(this, SIGNAL(sendPassSure()), paw, SLOT(closes()));
    paw->setWindowTitle("密码确认");
    paw->setModal(true);
    paw->show();
}

void ADDorDELManager::on_pushButton_clicked()       //增加管理员
{
    bool MS = false;
    emit sendDBTNO(db, teacherNo);
    emit sendMaorSuper(MS);

    mm->setWindowTitle("个人中心");
    mm->setModal(true);
    mm->show();
}

void ADDorDELManager::on_pushButton_3_clicked()
{
    this->close();
}
