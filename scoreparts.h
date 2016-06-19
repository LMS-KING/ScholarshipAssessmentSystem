#ifndef SCOREPARTS_H
#define SCOREPARTS_H

#include <QDialog>

namespace Ui {
class ScoreParts;
}

class ScoreParts : public QDialog
{
    Q_OBJECT

public:
    explicit ScoreParts(QWidget *parent = 0);
    ~ScoreParts();

private:
    void Message(QString message);

private slots:
    bool eventFilter(QObject *watch, QEvent *event);

    void on_pushButton_clicked();

private:
    Ui::ScoreParts *ui;
};

#endif // SCOREPARTS_H
