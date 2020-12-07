#include "admin_choose_cou_detail.h"
#include "ui_admin_choose_cou_detail.h"
QString RCNO;
admin_choose_cou_detail::admin_choose_cou_detail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_choose_cou_detail)
{
    ui->setupUi(this);
}

admin_choose_cou_detail::~admin_choose_cou_detail()
{
    delete ui;
}
//得到学期信息
QString admin_choose_cou_detail::getyear()
{
    QSqlQuery query;
    QString str;
    QString sql = "select year from term";
    query.exec(sql);
    if(query.next())
    {
        str = query.value(0).toString();
    }
    return str;
}
//接收admin页面传达槽函数
void admin_choose_cou_detail::recevieData(QString data)
{
    RCNO = data;
    QString cno = RCNO.mid(15,5);
    QString cls = RCNO.mid(6,8);
    QString sql;
    ui->table_detail->blockSignals(true);
    QSqlQuery query;
    sql = "select distinct sname, s.sno from real_course r, grade g, student s where r.cno=g.cno and s.sno=g.sno and "
          "r.cno='"+cno+"' and g.year='"+getyear()+"' and s.cls='"+cls+"'";
    if(query.exec(sql)){
        if(query.size()){
            ui->table_detail->clearContents();
            ui->table_detail->setRowCount(0);
            while(query.next()){
                QTableWidgetItem* item[2];
                int rowCount=ui->table_detail->rowCount();
                ui->table_detail->insertRow(rowCount);
                int i;
                for(i=0;i<2;i++){
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                    ui->table_detail->setItem(rowCount,i,item[i]);
                  }
                }
            }

        else{
            QMessageBox::information(nullptr,"查询结果","查询结果为空！");
        }
    }
    else{
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库查找错误！","数据库查找错误，错误信息:"+connectErrorMsg);
    }
    ui->table_detail->blockSignals(false);

}
//接收查询语句
QString admin_choose_cou_detail::get_detail_querysql()
{
    QString cno = RCNO.mid(15,5);
    QString cls = RCNO.mid(6,8);
    QString sql="select distinct sname, s.sno from real_course r, grade g, student s where r.cno=g.cno and s.sno=g.sno and ";
    QString condition="";
    QString sname=ui->ld_detail_sname->text();
    QString sno=ui->ld_detail_sno->text();
    if(sname.size()){
        condition+=(QString("sname='"+sname+"' and " ));
    }
    if(sno.size()){
        condition+=(QString("sno='"+sno+"' and " ));
    }
    if(condition.size()){
        sql=sql + condition.left(condition.size()) + "r.cno='"+cno+"' and g.year='"+getyear()+"' and s.cls='"+cls+"'";//-4去掉最后一个and
        return sql;
    }
    else{
        sql = "select distinct sname, s.sno from real_course r, grade g, student s where r.cno=g.cno and s.sno=g.sno and "
              "r.cno='"+cno+"' and g.year='"+getyear()+"' and s.cls='"+cls+"'";
        return sql;
    }
}
//查询操作
void admin_choose_cou_detail::on_btn_detail_query_clicked(){
    ui->table_detail->blockSignals(true);
    QSqlQuery query;
    QString sql=get_detail_querysql();
    if(query.exec(sql)){
        if(query.size()){
            ui->table_detail->clearContents();
            ui->table_detail->setRowCount(0);
            while(query.next()){
                QTableWidgetItem* item[2];
                int rowCount=ui->table_detail->rowCount();
                ui->table_detail->insertRow(rowCount);
                int i;
                for(i=0;i<2;i++){
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                    ui->table_detail->setItem(rowCount,i,item[i]);
                  }
                }
            }
        else{
            QMessageBox::information(nullptr,"查询结果","查询结果为空！");
        }
    }
    else{
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库查找错误！","数据库查找错误，错误信息:"+connectErrorMsg);
    }
    ui->table_detail->blockSignals(false);
}
//------添加操作---

    void admin_choose_cou_detail::on_btn_detail_add_clicked()
{
    QString sname=ui->ld_detail_sname->text();
    QString sno=ui->ld_detail_sno->text();
    QString cno = RCNO.mid(15,5);
    if(sname.size()==0||sno.size()==0)
    {
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }
    else{
        QString sql="insert into grade value ('"+sno+"','"+cno+"','"+getyear()+"',NULL)";
        QSqlQuery query;
        if(query.exec(sql)){
            ui->ld_detail_sno->setText(""); //清空输入框
            ui->ld_detail_sname->setText("");
            QMessageBox::information(nullptr,"添加成功","添加成功！");
        }
        //输出非法信息提示
        else{
            connectErrorMsg=query.lastError().text();
            if(connectErrorMsg.contains("PRIMARY",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的学生已存在，请重新输入！");
            }
            else if(connectErrorMsg.contains("too long for column 'rcno'",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的学生号太长，请重新输入！");
            }
            else if(connectErrorMsg.contains("Incorrect date value",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入非法信息，请重新输入！");
            }
            else{
                QMessageBox::information(nullptr,"数据库插入错误","数据库插入错误，错误信息："+connectErrorMsg);
            }
        }
    }
}
//----删除选课学生信息---
    void admin_choose_cou_detail::on_btn_detail_delete_clicked()
    {
        int row=ui->table_detail->currentRow();
        if(row!=-1){
            QPushButton *okbtn = new QPushButton(QString("确定"));
            QPushButton *cancelbtn = new QPushButton(QString("取消"));
            QMessageBox *mymsgbox = new QMessageBox;

            mymsgbox->setIcon(QMessageBox::Warning);
            mymsgbox->setWindowTitle(QString("提示"));
            mymsgbox->setText(QString("确定删除？"));
            mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
            mymsgbox->addButton(cancelbtn, QMessageBox::RejectRole);
            mymsgbox->show();
            mymsgbox->exec();//阻塞等待用户输入
            if (mymsgbox->clickedButton()==okbtn)//点击了OK按钮
            {
                QString sql="delete from grade where sno='"+ui->table_detail->item(row,1)->text()+"';";
                QSqlQuery query;
                if(query.exec(sql)){
                    ui->table_detail->removeRow(row);
                    QMessageBox::information(nullptr,"删除成功","删除成功");
                }else{
                    connectErrorMsg=query.lastError().text();
                    QMessageBox::information(nullptr,"数据库删除错误！","数据库删除错误，错误信息:"+connectErrorMsg);
                }
            }


        }else{
            QMessageBox::information(nullptr,"未选中","未选中删除行,请选择一行进行删除");
        }
    }
//------重置输入框------
void admin_choose_cou_detail::on_btn_detail_reset_clicked()
{
    ui->ld_detail_sno->setText("");
    ui->ld_detail_sname->setText("");
}
