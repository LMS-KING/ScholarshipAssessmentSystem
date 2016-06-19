#include "photo.h"
#include "ui_photo.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>       //输出测试，如：qDebug()<<i;

#include <QPalette>     //调色板类（改变部件背景，字体颜色头文件）
#include <QColor>

#include <QUrl>                    //图片拖放头文件
#include <QList>
#include <QtGui/QPixmap>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QMimeData>

#include <QCursor>              //右键菜单头文件
#include <QMenu>
#include <QAction>

#include <QMouseEvent>          //窗体内拖动图片头文件
#include <QPainter>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPixmap>

#include <QKeyEvent>            //键盘事件头文件
#include <QWheelEvent>

QString Image[1000] = {};
QStringList fileNames;
QMatrix leftmatrix,rightmatrix;
QStringList::Iterator j;
QString path;

photo::photo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::photo)
{
    ui->setupUi(this);
    ui->label_2->hide();

    //setWindowOpacity(0.5);          //窗体透明度

    ui->label->installEventFilter(this);
    ui->label->setAcceptDrops(true); // [[1]]: 使label可接受拖放操作
    ui->label->setScaledContents(true);

    //setMouseTracking(true);        //鼠标跟踪
    ui->pushButton->installEventFilter(this);       //为部件添加事件过滤器
    ui->pushButton_2->installEventFilter(this);
    ui->pushButton_3->installEventFilter(this);
    ui->pushButton_4->installEventFilter(this);
    ui->pushButton_5->installEventFilter(this);
    ui->pushButton_6->installEventFilter(this);
}

photo::~photo()
{
    delete ui;
}

void photo::recvDBStuNo(QSqlDatabase thdb, QString stuno, QString grade)
{
    db = thdb;
    studentNo = stuno;
    grd = grade;

    setResource();
}

void photo::setResource()
{
    QString sql = tr("select Address from DATA where StudentNo='%1' and Time like '%2%'").arg(studentNo).arg(grd);

    QString path = selectDB(sql);
    if(path == NULL)
    {
        ui->label_2->show();
        return;
    }
    else
        ui->label_2->hide();

    fileNames = path.split(",", QString::SkipEmptyParts);

    for(j = fileNames.begin();j < fileNames.end();j++)
    {
        Image[i] = *j;
        i++;
    }

    QString fileName = Image[0];
    pix.load(fileName);
    lg = pix.width();
    wide = pix.height();
   // pix = pix.scaled(851,491);
    ui->label->setPixmap(pix);
    //pix.scaled(ui->label->x(),ui->label->y());
    ui->label->setAlignment(Qt::AlignCenter);      //显示在label中间
    ui->label->resize(pix.width(),pix.height());

    num++;   //记录label中已有图片
}

QString photo::selectDB(QString sql)
{
    QString message = NULL;
    QSqlQuery query(db);
    query.exec(sql);
    while(query.next())
    {
        message += query.value(0).toString().trimmed() + ",";
    }

    return message;
}

bool photo::eventFilter(QObject *watched, QEvent *event)
{
    //屏蔽方向键控制焦点
    if(watched == ui->pushButton || watched == ui->pushButton_2 || watched == ui->pushButton_3 ||
            watched == ui->pushButton_4 || watched == ui->pushButton_5 ||
            watched == ui->pushButton_6 )
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvnet = static_cast<QKeyEvent*>(event);
            if(keyEvnet->key() == Qt::Key_Up || keyEvnet->key() == Qt::Key_Down ||
                    keyEvnet->key() == Qt::Key_Left || keyEvnet->key() == Qt::Key_Right)
            {
                return true;
            }
        }
    }

    //图片拖动
    if (watched == ui->label)
    {
        if (event->type() == QEvent::DragEnter)
        {
            // 当拖放时鼠标进入label时, label接受拖放的动作
            QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
            dee->acceptProposedAction();
            return true;
        }
        else if (event->type() == QEvent::Drop)
        {
            // 当放操作发生后, 取得拖放的数据
             QDropEvent *de = dynamic_cast<QDropEvent *>(event);
             QList<QUrl> urls = de->mimeData()->urls();

             if (urls.isEmpty())
             {
                 return true;
             }
            path = urls.first().toLocalFile();

            // 在label上显示拖放的图片
             QImage image(path); // QImage对I/O优化过, QPixmap对显示优化
             pix.load(path);
             lg = pix.width();
             wide = pix.height();
             if (!image.isNull())
             {
                 image = image.scaled(ui->label->size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
                 ui->label->setPixmap(QPixmap::fromImage(image));
                 ui->label->setAlignment(Qt::AlignCenter);      //显示在label中间
                 num++;   //记录label中已有图片
             }

             return true;
         }
    }

    return QWidget::eventFilter(watched, event);
}

//右键菜单
void photo::contextMenuEvent(QContextMenuEvent *)
{
    QCursor cur=this->cursor();
    QMenu *menu=new QMenu(ui->label);

    QAction *big = menu->addAction(tr("放大"));
    QAction *smal = menu->addAction(tr("缩小"));
    QAction *shang = menu->addAction(tr("上一张"));
    QAction *next = menu->addAction(tr("下一张"));
    QAction *left = menu->addAction(tr("左旋转"));
    QAction *right = menu->addAction(tr("右旋转"));

    connect(big,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_3_clicked())); //放大
    connect(smal,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_4_clicked()));//缩小
    connect(shang,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_clicked()));//上张
    connect(next,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_2_clicked()));//下张
   connect(left,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_6_clicked())); //左旋
   connect(right,SIGNAL(triggered(bool)),this,SLOT(on_pushButton_5_clicked()));//右旋

    menu->exec(cur.pos());
}

//鼠标按下事件
void photo::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //第一步，获取图片。将鼠标位置的部件强制转换为QLabel型
        QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
        if(!child->inherits("QLabel"))
            return;
        QPixmap pixmap = *child->pixmap();

        //第二步，自定义MIME类型
        QByteArray itemData;
        QDataStream dataStream(&itemData,QIODevice::WriteOnly);
        //将图片信息，位置信息保存到字节数组中
        dataStream<<pixmap<<QPoint(event->pos() - child->pos());

        //第三步，将数据放入QMimeData中
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("myImage/jpg",itemData);

        //第四步，将QMimelda中的数据放进QDrag
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->pos() - child->pos());

        //第五步，给原图加阴影
        QPixmap tempPixmap = pixmap;
        QPainter painter;               //绘图
        painter.begin(&tempPixmap);
        //添加透明淡黑色阴影
        painter.fillRect(pixmap.rect(),QColor(127,127,127,127));
        painter.end();
        child->setPixmap(tempPixmap);
        setAcceptDrops(true);
        QLabel *label = new QLabel(this);
        label->setPixmap(pix);
        label->resize(pix.size());


        //第六步，执行拖动操作
        if(drag->exec(Qt::CopyAction | Qt::MoveAction,Qt::CopyAction) == Qt::MoveAction)
            child->close();
        else
        {
            child->show();
            child->setPixmap(pixmap);
        }
    }
}

void photo::dragEnterEvent(QDragEnterEvent *event)      //拖动进入
{
    if(event->mimeData()->hasFormat("myImage/jpg"))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void photo::dragMoveEvent(QDragMoveEvent *event)        //拖动事件
{
    if(event->mimeData()->hasFormat("myImage/jpg"))
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void photo::dropEvent(QDropEvent *event)        //拖动放下事件
{
    if(event->mimeData()->hasFormat("myImage/jpg"))
    {
        QByteArray itemData = event->mimeData()->data("myImage/jpg");
        QDataStream dataStream(&itemData,QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint offset;
        dataStream>>pixmap>>offset; //使用数据流将字节数据读到QPixmap，QPoint变量中
        //新建标签，为其添加图片，并根据图片大小设置标签大小
        QLabel *newLabel = new QLabel(this);
        newLabel->setPixmap(pixmap);
        newLabel->resize(pixmap.size());

        ui->label = newLabel;       //其他所有操作只针对ui->label

        //是图片移动到放下的位置，不设置会默认显示在（0，0）
        newLabel->move(event->pos() - offset);
        newLabel->show();
        newLabel->setAttribute(Qt::WA_DeleteOnClose);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
        event->ignore();
}

void photo::keyPressEvent(QKeyEvent *event)     //键盘事件
{
    switch(event->key())
    {
        case Qt::Key_W:on_pushButton_3_clicked();break; //W键放大

        case Qt::Key_S:on_pushButton_4_clicked();break;     //S键缩小

        case Qt::Key_A:on_pushButton_clicked();break;       //A键上一张

        case Qt::Key_D:on_pushButton_2_clicked();break;     //D键下一张

        case Qt::Key_Up:on_pushButton_3_clicked();break;      //上键放大

        case Qt::Key_Down:on_pushButton_4_clicked();break;     //下键缩小

        case Qt::Key_Left:on_pushButton_clicked();break;       //左键上一张

        case Qt::Key_Right:on_pushButton_2_clicked();break;     //右键下一张

        defult:break;
    }
}

//滚轮事件
void photo::wheelEvent(QWheelEvent *event)
{
}

void photo::Wrong(QString message)
{
    QMessageBox::information(this, "提示", message, QMessageBox::Ok);
    return;
}

void photo::on_pushButton_clicked()     //上一张
{
    if(num == 0)
    {
        Wrong("当前没图哟!");
        return;
    }
    if(k <= 0)
    {
        Wrong("亲,已经是第一张图片了!");
        return;
    }

    ui->label->clear();
    pix.load(Image[--k]);
   // pix = pix.scaled(851,491);
    ui->label->setPixmap(pix);
   // pix.scaled(ui->label->x(),ui->label->y());
    ui->label->setAlignment(Qt::AlignCenter);      //显示在label中间
    ui->label->resize(pix.width(),pix.height());
}

void photo::on_pushButton_2_clicked()       //下一张
{
    if(num == 0)
    {
        Wrong("当前没图哟!");
        return;
    }
    if(k >= i - 1)
    {
        Wrong("亲,已经是最后一张图片了!");
        return;
    }

    ui->label->clear();
    pix.load(Image[++k]);
   // pix = pix.scaled(851,491);
    ui->label->setPixmap(pix);
    //pix.scaled(ui->label->x(),ui->label->y());
    ui->label->setAlignment(Qt::AlignCenter);      //显示在label中间
    ui->label->resize(pix.width(),pix.height());
}

void photo::on_pushButton_3_clicked()               //放大
{
    if(num == 0)
    {
        Wrong("当前没图哟!");
        return;
    }

    x = pix.height();
    y = pix.width();
    pix = pix.scaled(x * 1.2,y * 1.2,Qt::KeepAspectRatioByExpanding);
    ui->label->setPixmap(pix);
    ui->label->setAlignment(Qt::AlignCenter);      //显示在label中间
    ui->label->resize(pix.width(),pix.height());
}

void photo::on_pushButton_4_clicked()           //缩小
{
    if(num == 0)
    {
        Wrong("当前没图哟!");
        return;
    }
    x = pix.height();
    y = pix.width();
    if(x <= wide || y <= lg)
    {
        Wrong("亲,已经是原图了,再小会影响像素啦!");
        return;
    }

    pix = pix.scaled(x/1.2,y/1.2,Qt::KeepAspectRatio);
    ui->label->setPixmap(pix);
    ui->label->setAlignment(Qt::AlignCenter);      //显示在label中间
    ui->label->resize(pix.width(),pix.height());
}

void photo::on_pushButton_5_clicked()               //顺时针90
{
    if(num == 0)
    {
        Wrong("当前没图呦!");
        return;
    }

    rightmatrix.rotate(90);
    ui->label->setPixmap(pix.transformed(rightmatrix,Qt::SmoothTransformation));
}

void photo::on_pushButton_6_clicked()               //逆时针90
{
    if(num == 0)
    {
        Wrong("当前没图呦!");
        return;
    }

    rightmatrix.rotate(-90);
    ui->label->setPixmap(pix.transformed(rightmatrix,Qt::SmoothTransformation));
}

void photo::on_pushButton_7_clicked()       //返回，清空全局变量
{
    path.clear();

    this->close();
}
