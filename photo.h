#ifndef PHOTO_H
#define PHOTO_H

#include <QDialog>
#include <QPixmap>
//数据库头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <Qtsql>

static int i = 0, k = 0, lg = 0, wide = 0, num = 0;

namespace Ui {
class photo;
}

class photo : public QDialog
{
    Q_OBJECT

public:
    float x,y;

    explicit photo(QWidget *parent = 0);
    ~photo();

private:
    void setResource();

    QString selectDB(QString sql);

protected:

    bool eventFilter(QObject *watched, QEvent *event);

    void contextMenuEvent(QContextMenuEvent *);

    void mousePressEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void wheelEvent(QWheelEvent *event);

    void Wrong(QString message);

protected slots:
    void recvDBStuNo(QSqlDatabase thdb, QString stuno, QString grade);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::photo *ui;
    QPixmap pix;

    QSqlDatabase db;
    QString studentNo;
    QString grd;
};

#endif // PHOTO_H
