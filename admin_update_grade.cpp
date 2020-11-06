#include "admin_update_grade.h"
#include "ui_admin_update_grade.h"

admin_update_grade::admin_update_grade(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_update_grade)
{
    ui->setupUi(this);
}

admin_update_grade::~admin_update_grade()
{
    delete ui;
}
