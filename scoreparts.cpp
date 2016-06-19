#include "scoreparts.h"
#include "ui_scoreparts.h"
#include <QMessageBox>
#include <QPalette>

extern float DScore;
extern float ZScore;
extern float TScore;

ScoreParts::ScoreParts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreParts)
{
    ui->setupUi(this);

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);

    //设置默认值
    ui->lineEdit->setText("75");
    ui->lineEdit_2->setText("20");
    ui->lineEdit_3->setText("5");

    //设置提示标签为红色
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_5->setPalette(pe);
    ui->label_6->setPalette(pe);
    ui->label_7->setPalette(pe);
}

ScoreParts::~ScoreParts()
{
    delete ui;
}

void ScoreParts::Message(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

//事假过滤器
bool ScoreParts::eventFilter(QObject *watch, QEvent *event)
{
    if(watch == ui->lineEdit)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->lineEdit->text().toInt() <= 0)
            {
                ui->label_5->setText("该项值必须为整数!");
                ui->lineEdit->clear();
                return false;
            }
        }
    }
    else if(watch == ui->lineEdit_2)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->lineEdit_2->text().toInt() <= 0)
            {
                ui->label_6->setText("该项值必须为整数!");
                ui->lineEdit_2->clear();
                return false;
            }
        }
    }
    else if(watch == ui->lineEdit_3)
    {
        if(event->type() == QEvent::FocusOut)
        {
            if(ui->lineEdit_3->text().toInt() <= 0)
            {
                ui->label_7->setText("该项值必须为整数!");
                ui->lineEdit_3->clear();
                return false;
            }
        }
    }

    return QWidget::eventFilter(watch, event);
}

void ScoreParts::on_pushButton_clicked()        //确定赋值
{
    float FDS = ui->lineEdit->text().toFloat();
    float FZS = ui->lineEdit_2->text().toFloat();
    float FTS = ui->lineEdit_3->text().toFloat();

    if(FDS + FZS + FTS != 100)
    {
        Message("请确认比例是否是百分制!如50+30+20 = 100");
        return;
    }

    QString DS = "0." + ui->lineEdit->text().trimmed();
    QString ZS = "0." + ui->lineEdit_2->text().trimmed();
    QString TS = "0." + ui->lineEdit_3->text().trimmed();

    DScore = DS.toFloat();
    ZScore = ZS.toFloat();
    TScore = TS.toFloat();

    this->close();
}
