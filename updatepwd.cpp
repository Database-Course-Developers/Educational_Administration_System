#include "updatepwd.h"
#include "ui_updatepwd.h"

updatePwd::updatePwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updatePwd)
{
    ui->setupUi(this);
    ui->rbtn_admin->setChecked(true);
    ui->ledit_oldpwd->setEchoMode(QLineEdit::Password);
    ui->ledit_newpwd->setEchoMode(QLineEdit::Password);
    ui->ledit_twoNewPwd->setEchoMode(QLineEdit::Password);



}

updatePwd::~updatePwd()
{
    delete ui;
}


void updatePwd::on_btn_update_clicked()
{
    QString acc=ui->ledit_acc->text();
    QString oldPwd=ui->ledit_oldpwd->text();
    QString newPwd=ui->ledit_newpwd->text();
    QString twoNewPwd=ui->ledit_twoNewPwd->text();
    QString sql;
    QString updateSql;
    if(ui->rbtn_stu->isChecked()){
        sql="select * from student where sno='"+acc.trimmed()+"'";
        updateSql="update student set pwd='"+newPwd+"'where sno='"+ acc+"'";
    }
    else if(ui->rbtn_tea->isChecked()){
        sql="select * from teacher where tno='"+acc.trimmed()+"'";
        updateSql="update teacher set pwd='"+newPwd+"'where tno='"+ acc+"'";
    }else if(ui->rbtn_admin->isChecked()){
        sql="select * from admin where ano='"+acc.trimmed()+"'";
        updateSql="update admin set pwd='"+newPwd+"'where ano='"+ acc+"'";
    }

    if(connectDB()){
        QSqlQuery query;
        if(query.exec(sql)){
            if(query.next()){
                if(oldPwd==query.value("pwd").toString()){
                    QSqlQuery q;
                    if(newPwd!=twoNewPwd){
                        QMessageBox::information(nullptr,"错误","新密码前后不一致");
                    }
                    else{
                        if(q.exec(updateSql)){
                            QMessageBox::information(nullptr,"修改成功","修改成功");
                        }else{
                            connectErrorMsg=q.lastError().text();
                            QMessageBox::information(nullptr,"修改错误","数据库修改错误，错误信息："+connectErrorMsg);
                        }
                    }
                }else{
                    QMessageBox::information(nullptr,"账号或旧密码错误！","账号或密码错误，请重试");
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

void updatePwd::on_btn_back_clicked()
{
    emit logout();
    this->close();
}
