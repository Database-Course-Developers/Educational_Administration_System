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
        condition+=(QString("sno='"+sno+"' and" ));
    }
    if(cno.size()){
        condition+=(QString("cno='"+cno+"' and" ));
    }
    if(cname.size()){
        condition+=(QString("cname='"+cname+"' and" ));
    }
    if(sname.size()){
        condition+=(QString("sname='"+sname+"' and" ));
    }
    if(year.size()){
        condition+=(QString("year='"+year+"' and" ));
    }
    if(condition.size()){
        int i=3;
        while(i--)
            condition.
    }


}
void admin::on_btn_grade_query_clicked()
{
    QSqlQuery query;
    if(query.exec("select* from grade_view where sname='陈宏丰'")){
        ui->table_grade->clearContents();
        ui->table_grade->setRowCount(0);
        while(query.next()){
            QTableWidgetItem* item[6];
            int rowcount=ui->table_grade->rowCount();
            ui->table_grade->insertRow(rowcount);
            for(int i=0;i<6;i++){
                item[i]=new QTableWidgetItem(query.value(i).toString());
                ui->table_grade->setItem(rowcount,i,item[i]);

            }
        }
    }
}
