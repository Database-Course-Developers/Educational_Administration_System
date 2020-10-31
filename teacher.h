#ifndef TEACHER_H
#define TEACHER_H

#include <QWidget>
#include<base.h>
#include<database_util.h>
namespace Ui {
class teacher;
}

class teacher : public QWidget
{
    Q_OBJECT

public:
    explicit teacher(tea cur_teacher,QWidget *parent = nullptr);
    ~teacher();
    tea cur_teacher;
signals:
    void logout();
private:
    Ui::teacher *ui;
};

#endif // TEACHER_H
