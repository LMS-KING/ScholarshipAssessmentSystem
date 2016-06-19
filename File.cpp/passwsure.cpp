#include "passwsure.h"
#include "ui_passwsure.h"
#include <QTimer>

extern QString PW;
extern QString PAW;
extern QString PAWRE;
extern QString PAWF;
extern QString SUPAW;

PasswSure::PasswSure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswSure)
{
    ui->setupUi(this);

    ui->lineEdit->setEchoMode(QLineEdit::Password);

    No = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    timer->start(100);
}

PasswSure::~PasswSure()
{
    delete ui;
}

void PasswSure::on_pushButton_clicked()
{
    if(No == 1)
    {
        PW = ui->lineEdit->text().trimmed();
    }
    else if(No == 2)
    {
        PAW = ui->lineEdit->text().trimmed();
    }
    else if(No == 3)
    {
        PAWRE = ui->lineEdit->text().trimmed();
    }
    else if(No == 4)
    {
        PAWF = ui->lineEdit->text().trimmed();
    }
    else if(No == 5)
    {
        SUPAW = ui->lineEdit->text().trimmed();
    }

    ui->lineEdit->clear();
}

void PasswSure::timeOut()
{
    if(!ui->checkBox->isChecked())
        ui->lineEdit->setEchoMode(QLineEdit::Password);
    else
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
}

void PasswSure::recvStudent()       //学生确认密码
{
    No = 1;
}

void PasswSure::recvTeachOut()
{
    No = 2;
}

void PasswSure::recvTeachRe()
{
    No = 3;
}

void PasswSure::recvFellowDe()
{
    No = 4;
}

void PasswSure::recvSuper()
{
    No = 5;
}

void PasswSure::closes()
{
    this->close();
}
