#include "teacher.h"
#include "ui_teacher.h"
#include<QDebug>
teacher::teacher(tea cur_teacher,QWidget *parent) :
    QWidget(parent),cur_teacher(cur_teacher),
    ui(new Ui::teacher)
{
    ui->setupUi(this);
    //屏幕最大化
    setWindowState(Qt::WindowMaximized);
    ui->stackedWidget->setCurrentIndex(0);
    //initbox();
}

teacher::~teacher()
{
    delete ui;
}


void teacher::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int crr_row=ui->listWidget->currentRow();
    if(crr_row==0)ui->stackedWidget->setCurrentIndex(1);
    else if(crr_row==1)ui->stackedWidget->setCurrentIndex(2);
    else if(crr_row==2)ui->stackedWidget->setCurrentIndex(3);
    else if(crr_row==3)ui->stackedWidget->setCurrentIndex(4);
    else if(crr_row==4)ui->stackedWidget->setCurrentIndex(5);
    else ui->stackedWidget->setCurrentIndex(6);
}


void teacher::on_search_3_clicked(bool checked)
{
    QString major= ui->major_2->currentText();
    QString course=ui->course_2->currentText();

}


void teacher::on_comboBox_3_currentIndexChanged(int index)
{
    if(ui->comboBox_3->currentIndex()==2)
        ui->lineedit_info->setVisible(false);
}
