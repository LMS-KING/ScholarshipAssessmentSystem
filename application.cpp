#include "application.h"
#include "ui_application.h"
#include "passwsure.h"
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QThread>

QString PW = NULL;

Application::Application(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Application)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    timer->start(100);

    ticket_model = new QStandardItemModel();
    ticket_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("学号")));
    ticket_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("申请类型")));
    ticket_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("申请时间")));
    ticket_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("当前状态")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(ticket_model);
    //整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置时间显示列的宽度
    ui->tableView->setColumnWidth(0,110);
    ui->tableView->setColumnWidth(1,110);
    ui->tableView->setColumnWidth(2,110);
    ui->tableView->setColumnWidth(3,110);
     //表不可编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->sortByColumn(10,Qt::AscendingOrder);
}

Application::~Application()
{
    delete ui;
}

void Application::timeOut()     //计时器
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,3);
    QVariant data = model->data(index);

    if(data.toString().trimmed() == "已批准" || data.toString().trimmed() == "未批准")
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
    }
    else if(data.toString().trimmed() == "待审核")
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_3->setEnabled(false);
    }

    if(!PW.isEmpty())
    {
        recvPassw(PW);
    }
    else
        emit sendStudentPass();
}

void Application::recvSDBStuNO(QSqlDatabase thisdb, QString stuNo)//槽函数，db,学号
{
    db = thisdb;
    studentNo = stuNo;

    setRecord();
}

void Application::setRecord()       //设置记录
{
    QString sql = "select StudentNo, Style, Time, Status from APPLICATION where StudentNo='" + studentNo + "'";
    QSqlQuery query(db);
    query.exec(sql);
    int i = 0;
    while(query.next())
    {
        ticket_model->setItem(i, 0, new QStandardItem(query.value(0).toString().trimmed()));
        ticket_model->setItem(i, 1, new QStandardItem(query.value(1).toString().trimmed()));
        ticket_model->setItem(i, 2, new QStandardItem(query.value(2).toString().trimmed()));
        ticket_model->setItem(i, 3, new QStandardItem(query.value(3).toString().trimmed()));

        if(query.value(3).toString() == "未批准")
            ticket_model->item(i, 3)->setForeground(QBrush(QColor(255, 0, 0)));
        else if(query.value(3).toString() == "已批准")
        {
            //设置状态单元格为
            ticket_model->item(i, 3)->setForeground(QBrush(QColor(0,255,0)));
        }
        else if(query.value(3).toString() == "待审核")
        {
            //设置状态单元格为
            ticket_model->item(i, 3)->setForeground(QBrush(QColor(0,0,255)));
        }

        //设置单元格文本居中
        ticket_model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ticket_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);

        i++;
    }
}

void Application::on_pushButton_clicked()       //取消本次申请
{
    passwSure();
}

void Application::passwSure()   //确认密码
{
    PasswSure *paw = new PasswSure(this);
    paw->setWindowTitle("确认密码");
    paw->setModal(true);
    paw->show();
    connect(this, SIGNAL(sendSureToPass()), paw, SLOT(closes()));
    connect(this, SIGNAL(sendStudentPass()), paw, SLOT(recvStudent()));
}

void Application::recvPassw(QString passw)      //槽函数，确认密码
{
    QString sql = "select SPassword from STUDENT where StudentNo='" + studentNo + "'";
    QSqlQuery query(db);
    query.exec(sql);
    if(query.next())
    {
        if(passw.compare(query.value(0).toString()) == 0)
        {
            PW.clear();         //清空全局变量
            sureDeleteApplication();
            return;
        }
    }

    PW.clear();         //清空全局变量
    Error("密码错误!");
}

void Application::sureDeleteApplication()
{
    model = ui->tableView->model();
    int curow = ui->tableView->currentIndex().row();
    QModelIndex index = model->index(curow,1);
    QVariant data = model->data(index);
    QString recd = data.toString();

    int curow2 = ui->tableView->currentIndex().row();
    QModelIndex index2 = model->index(curow2,2);
    QVariant data2 = model->data(index2);
    QString recd2 = data2.toString();       //申请时间

    QString sql = tr("delete from APPLICATION where StudentNo='%1' and Style='%2' and Time='%3'").arg(studentNo).arg(recd).arg(recd2);

    QSqlQuery query(db);
    try
    {
        query.exec(sql);
    }
    catch(std::exception e)
    {
        Error(e.what());
        return;
    }

    model->removeRow(ui->tableView->currentIndex().row());      //实时更新列表
    Error("取消成功!");

    emit sendSureToPass();
}

void Application::on_pushButton_2_clicked()     //返回
{
    this->close();
}

void Application::Error(QString message)        //提示信息
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

void Application::on_pushButton_3_clicked()     //删除申请记录
{
    model = ui->tableView->model();

    int curow1 = ui->tableView->currentIndex().row();
    QModelIndex index1 = model->index(curow1,1);
    QVariant data1 = model->data(index1);
    QString recd1 = data1.toString();         //类型

    int curow2 = ui->tableView->currentIndex().row();
    QModelIndex index2 = model->index(curow2,2);
    QVariant data2 = model->data(index2);
    QString recd2 = data2.toString();       //申请时间

    QString sql = tr("delete from APPLICATION where StudentNo='%1' and Style='%2' and Time='%3'").arg(studentNo).arg(recd1).arg(recd2);

    QSqlQuery query(db);
    try
    {
        query.exec(sql);
    }
    catch(std::exception e)
    {
        Error(e.what());
        return;
    }

    model->removeRow(ui->tableView->currentIndex().row());      //实时更新列表
    Error("删除成功!");
}
