#include "admin_add_exam.h"
#include "ui_admin_add_exam.h"

admin_add_exam::admin_add_exam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_add_exam)
{
    ui->setupUi(this);
}

admin_add_exam::~admin_add_exam()
{
    delete ui;
}
