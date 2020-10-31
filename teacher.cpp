#include "teacher.h"
#include "ui_teacher.h"

teacher::teacher(tea cur_teacher,QWidget *parent) :
    QWidget(parent),cur_teacher(cur_teacher),
    ui(new Ui::teacher)
{
    ui->setupUi(this);
}

teacher::~teacher()
{
    delete ui;
}
