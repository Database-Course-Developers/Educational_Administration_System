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
    QString myclass;
    int flag;
    void initbox();
    void gradePage();
    void setGradeTable(QString);
    void calGrade();
    void timeTablePage();
    void examPage();

    void stuInfoPage();
    void stuChooselessonPage();
    void stuPlanPage();

signals:
    void logout();

private:
    Ui::student *ui;
    QString get_time(QString,QString);
    void set_stuPlanTable(QString);
    void OnBtnClicked(QPushButton *pBtn);
};

#endif // STUDENT_H
