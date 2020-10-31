#include "student.h"
#include "ui_student.h"

student::student(stu cur_student,QWidget *parent) :
    QWidget(parent),cur_student(cur_student),
    ui(new Ui::student)
{
    ui->setupUi(this);
}

student::~student()
{
    delete ui;
}
