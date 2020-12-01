#ifndef STUDENT_H
#define STUDENT_H

#include <QWidget>
#include<base.h>
#include<database_util.h>
namespace Ui {
class student;
}

class student : public QWidget
{
    Q_OBJECT

public:
    explicit student(stu cur_student,QWidget *parent = nullptr);
    ~student();
    stu cur_student;
    void initbox();
    void gradePage();
    void setGradeTable(QString);
    void calGrade();
    void timeTablePage();
    void examPage();

    void initial_personal_info();

signals:
    void logout();
private:
    Ui::student *ui;
};

#endif // STUDENT_H
