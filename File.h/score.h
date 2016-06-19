#ifndef SCORE_H
#define SCORE_H

#include <QDialog>
#include <QStandardItem>

//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

namespace Ui {
class Score;
}

class Score : public QDialog
{
    Q_OBJECT

public:
    explicit Score(QWidget *parent = 0);
    ~Score();

private:
    void setScore();

    void setManager();

    QString MakeDatabase(QString sql, QString style);

    void Error(QString message);

protected slots:
    void recvDbStu(QSqlDatabase thisdb, QString stu);

    void recvManager(bool tf);

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::Score *ui;
    QAbstractItemModel *model;  //查询记录中特定的行列值
    QStandardItemModel *ticket_model;

    QSqlDatabase db;
    QString studenNo;

    bool scoreOrM;      //判断是查分数还是找管理员
};

#endif // SCORE_H
