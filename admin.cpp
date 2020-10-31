#include "admin.h"
#include "ui_admin.h"

admin::admin(adm cur_admin,QWidget *parent) :
    QWidget(parent),cur_admin(cur_admin),
    ui(new Ui::admin)
{
    ui->setupUi(this);
}

admin::~admin()
{
    delete ui;
}
