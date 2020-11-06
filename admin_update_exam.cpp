#include "admin_update_exam.h"
#include "ui_admin_update_exam.h"

admin_update_exam::admin_update_exam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_update_exam)
{
    ui->setupUi(this);
}

admin_update_exam::~admin_update_exam()
{
    delete ui;
}
