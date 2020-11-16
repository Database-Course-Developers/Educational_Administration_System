#include "teacher.h"
#include "ui_teacher.h"
#include<QDebug>
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


void teacher::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int crr_row=ui->listWidget->currentRow();
    if(crr_row==0)ui->stackedWidget->setCurrentIndex(0);
    else if(crr_row==1)ui->stackedWidget->setCurrentIndex(1);
    else if(crr_row==2)ui->stackedWidget->setCurrentIndex(2);
    else if(crr_row==3)ui->stackedWidget->setCurrentIndex(3);
    else if(crr_row==4)ui->stackedWidget->setCurrentIndex(4);
    else ui->stackedWidget->setCurrentIndex(5);
}
