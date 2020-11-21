#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ledit_pwd->setEchoMode(QLineEdit::Password);
    ui->rbtn_admin->setChecked(true);
    ui->btn_login->setFocus();
    ui->btn_login->setDefault(true);
    this->setMinimumSize(1, 1);
    this->setMaximumSize(950,480);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_login_clicked()
{
    QString acc=ui->ledit_acc->text();
    QString pwd=ui->ledit_pwd->text();
    QString sql;
    if(ui->rbtn_stu->isChecked()){
        sql="select * from student where sno='"+acc.trimmed()+"'";
    }
    else if(ui->rbtn_teacher->isChecked()){
        sql="select * from teacher where tno='"+acc.trimmed()+"'";
    }else if(ui->rbtn_admin->isChecked()){
        sql="select * from admin where ano='"+acc.trimmed()+"'";
    }

    if(connectDB()){
        QSqlQuery query;
        if(query.exec(sql)){
            if(query.next()){
                if(ui->rbtn_stu->isChecked()){
                    stu cur_student(query.value(0).toString(),query.value(1).toString(),query.value(2).toString()
                                    ,query.value(3).toString(),query.value(4).toString(),
                                    query.value(5).toString(),query.value(6).toString());

                    if(pwd!=cur_student.pwd){
                        QMessageBox::information(NULL,"账号或密码错误！","账号或密码错误，请重试");
                    }else{
                        mshareptr_s.reset(new student(cur_student));
                        QObject::connect(mshareptr_s.get(),&student::logout,this,&MainWindow::show);
                        mshareptr_s->show();
                        this->close();
                    }

                }
                else if(ui->rbtn_teacher->isChecked()){
                    tea cur_teacher(query.value(0).toString(),query.value(1).toString(),query.value(2).toString()
                                    ,query.value(3).toString(),query.value(4).toString(),
                                    query.value(5).toString(),query.value(6).toString(),
                                    query.value(7).toString());
                    if(pwd!=cur_teacher.pwd){
                        QMessageBox::information(NULL,"账号或密码错误！","账号或密码错误，请重试");
                    }else{
                        mshareptr_t.reset(new teacher(cur_teacher));
                        QObject::connect(mshareptr_t.get(),&teacher::logout,this,&MainWindow::show);
                        mshareptr_t->show();
                        this->close();
                    }

                }else if(ui->rbtn_admin->isChecked()){
                    adm  cur_admin(query.value(0).toString(),query.value(1).toString());
                    if(pwd!=cur_admin.pwd){
                        QMessageBox::information(NULL,"账号或密码错误！","账号或密码错误，请重试");
                    }else{
                        mshareptr_a.reset(new admin(cur_admin));
                        QObject::connect(mshareptr_a.get(),&admin::logout,this,&MainWindow::show);
                        mshareptr_a->show();
                        this->close();
                    }
                }
            }else{
                QMessageBox::information(nullptr,"账户不存在！","该账户不存在");
            }

        }else{
            connectErrorMsg=query.lastError().text();
            QMessageBox::information(nullptr,"数据库查询错误！","数据库查询错误，错误信息:"+connectErrorMsg);
        }
    }
    else{
        QMessageBox::information(nullptr,"错误！","无法连接到数据库，错误信息："+connectErrorMsg);
    }

}

void MainWindow::on_btn_update_clicked()
{
     msharptr_u.reset(new updatePwd);
     QObject::connect(msharptr_u.get(),&updatePwd::logout,this,&MainWindow::show);
     msharptr_u->show();
     this->close();

}


