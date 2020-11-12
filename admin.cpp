#include "admin.h"
#include "ui_admin.h"

admin::admin(adm cur_admin,QWidget *parent) :
    QWidget(parent),cur_admin(cur_admin),
    ui(new Ui::admin)
{
    ui->setupUi(this);
    initbox();
}

admin::~admin()
{
    delete ui;
}



void admin::initbox(){
    //初始化学期下拉框
    QVector<QString> year=getyear();
    for(QString& s:year){
        ui->cbox_grade_year->addItem(s);
    }
    ui->cbox_grade_year->addItem(QString("无"));

}
QString admin::get_grade_querysql(){
    QString sql="select * from grade_view ";
    QString condition="";
    QString sno=ui->ld_grade_sno->text();
    QString cno=ui->ld_grade_cno->text();
    QString cname=ui->ld_grade_cname->text();
    QString sname=ui->ld_grade_sname->text();
    QString year=ui->cbox_grade_year->currentText();
    if(sno.size()){
        condition+=(QString("sno='"+sno+"' and " ));
    }
    if(cno.size()){
        condition+=(QString("cno='"+cno+"' and " ));
    }
    if(cname.size()){
        condition+=(QString("cname='"+cname+"' and " ));
    }
    if(sname.size()){
        condition+=(QString("sname='"+sname+"' and " ));
    }
    if(year.size()&&year!="无"){
        condition+=(QString("year='"+year+"' and " ));
    }

    if(condition.size()){
        sql=sql+"where "+ condition.left(condition.size()-4 );//-3去掉最后一个and
    }
    return sql;

}
void admin::on_btn_grade_query_clicked()
{
    ui->table_grade->blockSignals(true);
    QSqlQuery query;
    QString sql=get_grade_querysql();
    if(query.exec(sql)){
        ui->table_grade->clearContents();
        ui->table_grade->setRowCount(0);
        while(query.next()){
            QTableWidgetItem* item[6];
            int rowcount=ui->table_grade->rowCount();
            ui->table_grade->insertRow(rowcount);
            for(int i=0;i<6;i++){
                item[i]=new QTableWidgetItem(query.value(i).toString());
                if(i<5){
                    item[i]->setFlags(item[i]->flags()&(~Qt::ItemIsEditable));
                }
                ui->table_grade->setItem(rowcount,i,item[i]);

            }
        }
    }else{
        qDebug()<<query.lastError().text();
    }
    ui->table_grade->blockSignals(false);
}

void admin::on_table_grade_itemChanged(QTableWidgetItem *item)
{
    ui->table_grade->blockSignals(true);
    int row=item->row();
    QString updatesql="update grade set grade ="+(item->text())+
            " where sno='"+ui->table_grade->item(row,0)->text()+"' and "+
            "cno='"+ui->table_grade->item(row,2)->text()+"' and "+
            "year='"+ui->table_grade->item(row,4)->text()+"'";

    QSqlQuery query;
    if(query.exec(updatesql)){
        QMessageBox::information(nullptr,"修改成功","修改成功");
    }else{
        item->setBackground(Qt::red);
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库修改错误！","数据库修改错误，错误信息:"+connectErrorMsg);

    }
    ui->table_grade->blockSignals(false);

}


void admin::on_btn_grade_delete_clicked()
{
    int row=ui->table_grade->currentRow();
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

            QString deletesql="delete from grade where sno='"
                    +ui->table_grade->item(row,0)->text()+"' and "+
                    "cno='"+ui->table_grade->item(row,2)->text()+"' and "+
                    "year='"+ui->table_grade->item(row,4)->text()+"'";
            QSqlQuery query;
            if(query.exec(deletesql)){
                ui->table_grade->removeRow(row);
                QMessageBox::information(nullptr,"删除成功","删除成功");
            }else{
                connectErrorMsg=query.lastError().text();
                QMessageBox::information(nullptr,"数据库修改错误！","数据库修改错误，错误信息:"+connectErrorMsg);
            }
        }
        else{

        }

    }else{
        QMessageBox::information(nullptr,"未选中","未选中删除行,请选择一行进行删除");
    }
}
