#include "admin.h"
#include "ui_admin.h"

admin::admin(adm cur_admin,QWidget *parent) :
    QWidget(parent),cur_admin(cur_admin),
    ui(new Ui::admin)
{
    ui->setupUi(this);
    //屏幕最大化
    setWindowState(Qt::WindowMaximized);
    initbox();
}

admin::~admin()
{
    delete ui;
}




//初始化选择框内的选项
void admin::initbox(){
    //初始化学期选择框
    QVector<QString> year=getyear();
    for(QString& s:year){
        ui->cbox_grade_year->addItem(s);
        ui->cbox_grade_year1->addItem(s);

    }
    ui->cbox_grade_year->setCurrentIndex(-1);
    ui->cbox_grade_year1->setCurrentIndex(-1);
    //初始化教室选择框
    QVector<QString> clr=getclassroom();
    for(QString& s:clr){
        ui->cbox_exam_clr->addItem(s);
        ui->cbox_examadd_clr->addItem(s);
    }
    ui->cbox_exam_clr->setCurrentIndex(-1);
    ui->cbox_examadd_clr->setCurrentIndex(-1);

    //初始化学院选择框
    QVector<QString> collegename=getcollegename();
    for(QString&s:collegename){
        ui->cbox_exam_collegename->addItem(s);
    }
    ui->cbox_exam_collegename->setCurrentIndex(-1);

    //日期时间选择框初始化
    QDate date(QDate::currentDate());
    ui->dt_begin_time->setDate(date);
    ui->dt_end_time->setDate(date.addMonths(1));
    ui->dt_examadd_begin->setDate(date);
    ui->dt_examadd_end->setDate(date.addDays(1));
    ui->dt_begin_time->setCalendarPopup(true);
    ui->dt_end_time->setCalendarPopup(true);
    ui->dt_examadd_begin->setCalendarPopup(true);
    ui->dt_examadd_end->setCalendarPopup(true);
    //课务号选择框初始化
    QVector<QString> rcno=getOneItem("select real_course.rcno from real_course,course "
                                     "where real_course.cno=course.cno and is_exam=1 "
                                     "and real_course.rcno not in(select rcno from exam)");
    for(QString&s:rcno){
        ui->cbox_examadd_rcno->addItem(s);
    }
    ui->cbox_examadd_rcno->setCurrentIndex(-1);

    //初始化老师选择框
    QVector<QString> tno=getOneItem("select tno from teacher");
    for(QString&s:tno){
        ui->cbox_examadd_tno1->addItem(s);
        ui->cbox_examadd_tno2->addItem(s);
    }
    ui->cbox_examadd_tno1->setCurrentIndex(-1);
    ui->cbox_examadd_tno2->setCurrentIndex(-1);

    //----教师管理部分----
    ui->cbox_tea_sex->setCurrentIndex(-1);
    ui->cbox_tea_birth_y->setCurrentIndex(-1);
    ui->cbox_tea_birth_m->setCurrentIndex(-1);
    ui->cbox_tea_birth_d->setCurrentIndex(-1);
    ui->cbox_tea_pos->setCurrentIndex(-1);

    //----学生管理部分----
    ui->cbox_stu_sex->setCurrentIndex(-1);
    ui->cbox_stu_birth_y->setCurrentIndex(-1);
    ui->cbox_stu_birth_m->setCurrentIndex(-1);
    ui->cbox_stu_birth_d->setCurrentIndex(-1);

    //----课程管理部分----
    ui->cbox_cou_credits->setCurrentIndex(-1);
    ui->cbox_cou_clg->setCurrentIndex(-1);
    ui->cbox_cou_is_exam->setCurrentIndex(-1);
    ui->cbox_cou_required->setCurrentIndex(-1);

    //----选课管理部分----
    ui->cbox_choose_cou_term->setCurrentIndex(-1);
    for(QString& s:clr){
        ui->cbox_choose_cou_clr->addItem(s);
    }
    ui->cbox_choose_cou_clr->setCurrentIndex(-1);
}
//-------考务 part -------林嘉欣
QString admin::get_exam_querysql(){
    QString sql="select * from exam_view ";
    QString condition="";
    QString rcno=ui->ld_exam_rcno->text();
    QString cno=ui->ld_exam_cno->text();
    QString tno=ui->ld_exam_tno->text();
    QString tno1=ui->ld_exam_tno1->text();
    QString tno2=ui->ld_exam_tno2->text();
    QString clr=ui->cbox_exam_clr->currentText();
    QString collegename=ui->cbox_exam_collegename->currentText();
    QString begin_time=ui->dt_begin_time->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString end_time=ui->dt_end_time->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(rcno.size()){
        condition+=(QString("rcno='"+rcno+"' and " ));
    }
    if(cno.size()){
        condition+=(QString("cno='"+cno+"' and " ));
    }
    if(tno.size()){
        condition+=(QString("tno='"+tno+"' and " ));
    }
    if(tno1.size()){
        condition+=(QString("tno1='"+tno1+"' and " ));
    }
    if(tno2.size()){
        condition+=(QString("tno2='"+tno2+"' and " ));
    }
    if(clr.size()){
        condition+=(QString("clr='"+clr+"' and " ));
    }
    if(collegename.size()){
        condition+=(QString("collegename='"+collegename+"' and " ));
    }
    if(begin_time.size()){
        condition+=(QString("begin_time>='"+begin_time+"' and " ));
    }
    if(end_time.size()){
        condition+=(QString("end_time<='"+end_time+"' and " ));
    }

    if(condition.size()){
        sql=sql+"where "+ condition.left(condition.size()-4 );//-4去掉最后一个and
    }
    return sql;
}
void admin::on_btn_exam_query_clicked()
{
    ui->table_exam->blockSignals(true);
    QSqlQuery query;
    QString sql=get_exam_querysql();
    if(query.exec(sql)){
        ui->table_exam->clearContents();
        ui->table_exam->setRowCount(0);
        while(query.next()){
            QTableWidgetItem* item[9];
            int rowcount=ui->table_exam->rowCount();
            ui->table_exam->insertRow(rowcount);
            for(int i=0;i<9;i++){
                if(i==7||i==8)
                    item[i]=new QTableWidgetItem(query.value(i).toDateTime().toString("yyyy-MM-dd hh:mm::ss"));
                else
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                if(i<3||i==6){
                    item[i]->setFlags(item[i]->flags()&(~Qt::ItemIsEditable));
                }
                ui->table_exam->setItem(rowcount,i,item[i]);

            }
        }
    }else{
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库查找错误！","数据库查找错误，错误信息:"+connectErrorMsg);
    }
    ui->table_exam->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_exam->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->table_exam->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
    ui->table_exam->horizontalHeader()->setSectionResizeMode(8,QHeaderView::ResizeToContents);
    ui->table_exam->blockSignals(false);
}
QString admin::get_exam_addsql(){
    QString rcno=ui->cbox_examadd_rcno->currentText();
    QString tno1=ui->cbox_examadd_tno1->currentText();
    QString tno2=ui->cbox_examadd_tno2->currentText();
    QString clr=ui->cbox_examadd_clr->currentText();
    QString begintime=ui->dt_examadd_begin->dateTime().toString(timeformat);
    QString endtime=ui->dt_examadd_end->dateTime().toString(timeformat);
    if(rcno.size()==0||tno1.size()==0||tno2.size()==0||clr.size()==0
            ||begintime.size()==0||endtime.size()==0){
        return "";
    }
    QString sql=QString::asprintf("insert into  exam values('%s','%s','%s','%s','%s','%s')",
                                  rcno.toStdString().c_str(),begintime.toStdString().c_str(),
                                  endtime.toStdString().c_str(),tno1.toStdString().c_str(),
                                  tno2.toStdString().c_str(),clr.toStdString().c_str());


    return sql;
}
void admin::on_btn_exam_add_clicked()
{
    QString sql=get_exam_addsql();
    if(sql.size()==0){
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }else{
        QSqlQuery query;
        if(query.exec(sql)){
            ui->cbox_examadd_clr->setCurrentIndex(-1);
            ui->cbox_examadd_rcno->setCurrentIndex(-1);
            ui->cbox_examadd_tno1->setCurrentIndex(-1);
            ui->cbox_examadd_tno2->setCurrentIndex(-1);
            QMessageBox::information(nullptr,"添加成功","添加成功");

        }else{

            connectErrorMsg=query.lastError().text();
            QMessageBox::information(nullptr,"数据库插入错误！","数据库插入错误，错误信息:"+connectErrorMsg);
        }
    }
}

void admin::on_btn_exam_delete_clicked()
{
    int row=ui->table_exam->currentRow();
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

            QString deletesql="delete from exam where rcno='"
                    +ui->table_exam->item(row,0)->text()+"'";
            qDebug()<<deletesql;
            QSqlQuery query;
            if(query.exec(deletesql)){
                ui->table_exam->removeRow(row);
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
void admin::on_btn_exam_clear_clicked()
{
    ui->ld_exam_cno->setText("");
    ui->ld_exam_rcno->setText("");
    ui->ld_exam_tno->setText("");
    ui->ld_exam_tno1->setText("");
    ui->ld_exam_tno2->setText("");
    ui->cbox_exam_clr->setCurrentIndex(-1);
    ui->cbox_exam_collegename->setCurrentIndex(-1);

}
void admin::on_table_exam_itemChanged(QTableWidgetItem *item)
{
    ui->table_exam->blockSignals(true);
    int row=item->row();
    QString updatesql=QString::asprintf("update exam set teacher1='%s',teacher2='%s',clr='%s',begin_time='%s',end_time='%s'"
                                        " where rcno='%s'",
                                        ui->table_exam->item(row,3)->text().toStdString().c_str(),
                                        ui->table_exam->item(row,4)->text().toStdString().c_str(),
                                        ui->table_exam->item(row,5)->text().toStdString().c_str(),
                                        ui->table_exam->item(row,7)->text().toStdString().c_str(),
                                        ui->table_exam->item(row,8)->text().toStdString().c_str(),
                                        ui->table_exam->item(row,0)->text().toStdString().c_str());


    QSqlQuery query;
    qDebug()<<updatesql;
    if(query.exec(updatesql)){
        QMessageBox::information(nullptr,"修改成功","修改成功");
    }else{
        item->setBackground(Qt::red);
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库修改错误！","数据库修改错误，错误信息:"+connectErrorMsg);

    }
    ui->table_exam->blockSignals(false);
}


//-------成绩 part--------林嘉欣
//根据输入框获取查询成绩sql语句
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
    if(year.size()){
        condition+=(QString("year='"+year+"' and " ));
    }

    if(condition.size()){
        sql=sql+"where "+ condition.left(condition.size()-4 );//-3去掉最后一个and
    }
    return sql;

}
//根据输入框获取添加成绩sql语句
QString admin::get_grade_addsql(){
    QString sno=ui->ld_grade_sno1->text();
    QString cno=ui->ld_grade_cno1->text();
    QString year=ui->cbox_grade_year1->currentText();
    QString grade=ui->ld_grade_grade1->text();
    if(sno.size()==0||cno.size()==0||year.size()==0||grade.size()==0){
        return "";
    }
    QString sql=QString::asprintf("insert into  grade values('%s','%s','%s',%d)",
                                  sno.toStdString().c_str(),cno.toStdString().c_str(),
                                  year.toStdString().c_str(),grade.toInt());

    return sql;

}
//成绩查询按钮
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
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库查找错误！","数据库查找错误，错误信息:"+connectErrorMsg);
    }
    ui->table_grade->blockSignals(false);
}
//成绩修改信号
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
//成绩删除按钮
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
                QMessageBox::information(nullptr,"数据库删除错误！","数据库删除错误，错误信息:"+connectErrorMsg);
            }
        }


    }else{
        QMessageBox::information(nullptr,"未选中","未选中删除行,请选择一行进行删除");
    }
}
//成绩查询重置按钮
void admin::on_btn_grade_clear_clicked()
{
    ui->ld_grade_cno->setText("");
    ui->ld_grade_sno->setText("");
    ui->ld_grade_cname->setText("");
    ui->ld_grade_sname->setText("");
    ui->cbox_grade_year->setCurrentIndex(-1);
}
//成绩添加按钮
void admin::on_bbtn_grade_add_clicked()
{
    QString sql=get_grade_addsql();
    if(sql.size()==0){
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }else{
        QSqlQuery query;
        if(query.exec(sql)){
            ui->ld_grade_cno1->setText("");
            ui->ld_grade_sno1->setText("");
            ui->ld_grade_grade1->setText("");
            ui->cbox_grade_year1->setCurrentIndex(-1);
            QMessageBox::information(nullptr,"添加成功","添加成功");

        }else{

            connectErrorMsg=query.lastError().text();
            QMessageBox::information(nullptr,"数据库插入错误！","数据库插入错误，错误信息:"+connectErrorMsg);
        }
    }
}

//----------课程管理part----------郑晓峰
//根据输入框获取查询课程信息的sql语句
QString admin::get_cou_querysql(){
    QString sql="select * from course ";
    QString condition="";
    QString cno=ui->ld_cou_cno->text();
    QString name=ui->ld_cou_name->text();
    QString credits=ui->cbox_cou_credits->currentText();
    QString required=ui->cbox_cou_required->currentText();
    QString is_exam=ui->cbox_cou_is_exam->currentText();
    QString clg=ui->cbox_cou_clg->currentText();
    QString hour=ui->ld_cou_hour->text();
    if(cno.size()){
        condition+=(QString("cno='"+cno+"' and " ));
    }
    if(name.size()){
        condition+=(QString("name='"+name+"' and " ));
    }
    if(credits.size()){
        condition+=(QString("credits='"+credits+"' and " ));
    }
    if(required.size()){
        if(required == "必修"){
        condition+=(QString("required='1' and " ));}
        else if(required == "选修")
            condition+=(QString("required='0' and "));
    }
    if(is_exam.size()){
        if(is_exam == "是"){
        condition+=(QString("is_exam='1' and " ));}
        else if (is_exam == "否")
            condition+=(QString("is_exam='0' and "));
    }
    if(clg.size()){
        condition+=(QString("clg='"+clg+"' and "));
    }
    if(hour.size()){
        hour+=(QString("hour='"+hour+"' and "));
    }

    if(condition.size()){
        sql=sql+"where "+ condition.left(condition.size()-4 );//-4去掉最后一个and
    }
    return sql;

}

//课程信息查询按钮
void admin::on_btn_cou_query_clicked(){
    ui->table_cou->blockSignals(true);
    QSqlQuery query;
    QString sql=get_cou_querysql();
    if(query.exec(sql)){
        if(query.size()){
            ui->table_cou->clearContents();
            ui->table_cou->setRowCount(0);
            while(query.next()){
                //执行完query.exec()后query是指向结果集外的
                //第一次执行query.next()使query指向结果集的第一条记录
                //接下来的query.next()使query指向下一条记录
                QTableWidgetItem* item[7];
                int rowCount=ui->table_cou->rowCount();
                ui->table_cou->insertRow(rowCount);
                int i;
                for(i=0;i<7;i++){
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                    if(i==0){
                        item[i]->setFlags(item[i]->flags()&(~Qt::ItemIsEditable));//设置第一列课程号为不可编辑
                    }
                    if(i==3){
                        if(item[i]->text()==1)
                            item[i]->setText("必修");
                        else
                            item[i]->setText("选修");
                    }
                    if(i==4){
                        if(item[i]->text()==1)
                            item[i]->setText("是");
                        else
                            item[i]->setText("否");
                    }
                    ui->table_cou->setItem(rowCount,i,item[i]);
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
    ui->table_cou->blockSignals(false);
}
//------修改------
QString admin::get_cou_column_name(int n){
    if(n==0) return QString("cno");
    if(n==1) return QString("name");
    if(n==2) return QString("credits");
    if(n==3) return QString("required");
    if(n==4) return QString("is_exam");
    if(n==5) return QString("clg");
    if(n==6) return QString("hour");
}

//在表格中修改内容
void admin::on_table_cou_itemChanged(QTableWidgetItem *item)
{
    ui->table_cou->blockSignals(true);
    int row=item->row();
    QString column_name=get_cou_column_name(item->column());
    QString updatesql;
    if(item->text() == "必修"){
        updatesql="update course set required=1 where cno='"+ui->table_cou->item(row,0)->text()+"'";
    }
    else if(item->text() == "选修"){
       updatesql="update course set required =0 where cno='"+ui->table_cou->item(row,0)->text()+"'";
    }
    else if(item->text() == "是"){
        updatesql="update course set is_exam =1 where cno='"+ui->table_cou->item(row,0)->text()+"'";
    }
    else if(item->text() == "否"){
        updatesql="update course set is_exam =0 where cno='"+ui->table_cou->item(row,0)->text()+"'";
    }
    else{
        updatesql="update course set "+column_name+
            "='"+item->text()+"' where cno='"+ui->table_cou->item(row,0)->text()+"'";
    }
    QSqlQuery query;
    if(query.exec(updatesql)){
        QMessageBox::information(nullptr,"修改成功","修改成功！");
    }
    else{
        connectErrorMsg=query.lastError().text();
        if(connectErrorMsg.contains("foreign key constraint",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的学院号不存在，请重新输入！");
        }
        else if(connectErrorMsg.contains("too long for column 'required'",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","请输入“必修”或“选修”，请重新输入！");
        }
        else if(connectErrorMsg.contains("too long for column 'is_exam'",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","请输入“是”或“否”，请重新输入！");
        }
        else{
            QMessageBox::information(nullptr,"修改错误","修改错误！错误信息："+connectErrorMsg);
        }
    }

    ui->table_cou->blockSignals(false);
}

//----添加课程信息
void admin::on_btn_cou_add_clicked()
{
    QString cno=ui->ld_cou_cno->text();
    QString name=ui->ld_cou_name->text();
    QString credits=ui->cbox_cou_credits->currentText();
    QString required=ui->cbox_cou_required->currentText();
    QString is_exam=ui->cbox_cou_is_exam->currentText();
    QString hour=ui->ld_cou_hour->text();
    QString clg=ui->cbox_cou_clg->currentText();
    if(cno.size()==0||name.size()==0||credits.size()==0||required.size()==0||is_exam.size()==0||
            hour.size()==0||clg.size()==0){
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }
    else{
        if(required == "必修"){
            required = '1';
        }
        else if(required == "选修")
            required = '0';
        if(is_exam == "是"){
            is_exam = '1';
        }
        else if(is_exam == "否")
            is_exam = '0';
        QString sql="insert into course value ('"+cno+"','"+name+"','"+credits+"',"+required+","+is_exam+",'"+clg+"','"+hour+"')";
        QSqlQuery query;
        if(query.exec(sql)){
            ui->ld_cou_cno->setText(""); //清空输入框
            ui->ld_cou_name->setText("");
            ui->cbox_cou_credits->setCurrentIndex(-1);
            ui->cbox_cou_required->setCurrentIndex(-1);
            ui->cbox_cou_is_exam->setCurrentIndex(-1);
            ui->cbox_cou_clg->setCurrentIndex(-1);
            ui->ld_cou_hour->setText("");
            QMessageBox::information(nullptr,"添加成功","添加成功！");
        }
        //输出非法信息提示
        else{
            connectErrorMsg=query.lastError().text();
            if(connectErrorMsg.contains("PRIMARY",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的课程号已存在，请重新输入！");
            }
            else if(connectErrorMsg.contains("too long for column 'CNO'",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的课程号太长，请重新输入！");
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
//------删除课程信息
void admin::on_btn_cou_delete_clicked()
{
    int row=ui->table_cou->currentRow();
    if(row!=-1){
        QPushButton *okbtn = new QPushButton(QString("确定"));
        QPushButton *cancelbtn = new QPushButton(QString("取消"));
        QMessageBox *confirmbox = new QMessageBox;

        confirmbox->setIcon(QMessageBox::Warning);
        confirmbox->setWindowTitle("提示");
        confirmbox->setText("确认删除？");
        confirmbox->addButton(okbtn,QMessageBox::AcceptRole);
        confirmbox->addButton(cancelbtn,QMessageBox::RejectRole);
        confirmbox->show();

        confirmbox->exec();//使函数“暂停”，等待用户点击按钮

        if(confirmbox->clickedButton()==okbtn){//点击确认按钮
            QString sql="delete from course where cno='"+ui->table_cou->item(row,0)->text()+"';";
            QSqlQuery query;
            if(query.exec(sql)){
                ui->table_cou->removeRow(row);
                QMessageBox::information(nullptr,"删除成功","删除成功！");
            }
            else{
                connectErrorMsg=query.lastError().text();
                QMessageBox::information(nullptr,"数据库删除错误","数据库删除错误，错误信息"+connectErrorMsg);
            }
        }
    }
    else{
        QMessageBox::information(nullptr,"未选中","未选中删除行，请选择一行进行删除");
    }
}
//------重置输入框------
void admin::on_btn_cou_reset_clicked()
{
    ui->ld_cou_cno->setText("");
    ui->ld_cou_name->setText("");
    ui->cbox_cou_clg->setCurrentIndex(-1);
    ui->cbox_cou_credits->setCurrentIndex(-1);
    ui->cbox_cou_is_exam->setCurrentIndex(-1);
    ui->cbox_cou_required->setCurrentIndex(-1);
    ui->ld_cou_hour->setText("");
}

//----------选课管理part----------郑晓峰
//根据输入框获取查询选课信息sql语句
QString admin::get_choose_cou_querysql(){
    QString sql="select r.rcno, r.cno, r.tno, c.name, r.term, bin(r.daytime+0), bin(r.weektime+0), r.clr from real_course r, course c ";
    QString condition="";
    QString rcno=ui->ld_choose_cou_rcno->text();
    QString cno=ui->ld_choose_cou_cno->text();
    QString tno=ui->ld_choose_cou_tno->text();
    QString name=ui->ld_choose_cou_name->text();
    /*QString term=ui->cbox_choose_cou_term->currentText();
    QString daytime=ui->ld_choose_cou_daytime->text();
    QString weektime=ui->cbox_choose_cou_weektime->currentText();
    QString clr=ui->cbox_choose_cou_clr->currentText();*/
    if(rcno.size()){
        condition+=QString("r.rcno='"+rcno+"' and ");
    }
    if(cno.size()){
        condition+=QString("r.cno='"+cno+"' and ");
    }
    if(tno.size()){
        condition+=QString("r.tno='"+tno+"' and ");
    }
    if(name.size()){
        condition+=QString("c.name='"+name+"' and ");
    }
    /*if(term.size()){
        condition+=QString("term='"+term+"' and ");
    }
    if(daytime.size()){
        condition+=QString("daytime='"+daytime+"' and ");
    }
    if(weektime.size()){
        condition+=QString("weektime='"+weektime+"' and ");
    }
    if(clr.size()){
            condition+=QString("clr='"+clr+"' and ");
    }*/
    if(condition.size()){
        sql=sql+"where r.cno=c.cno and "+condition.left(condition.size()-4);//去掉最后一个and和空格
    }
    else{
        sql=sql+"where r.cno=c.cno ";
    }

    return sql;
}
//选课信息查询按钮
void admin::on_btn_choose_cou_query_clicked()
{
    ui->table_choose_cou->blockSignals(true);
    QSqlQuery query;
    QString sql=get_choose_cou_querysql();
    if(query.exec(sql)){
        if(query.size()){
            ui->table_choose_cou->clearContents();
            ui->table_choose_cou->setRowCount(0);
            while(query.next()){
                QTableWidgetItem* item[8];
                int rowCount=ui->table_choose_cou->rowCount();
                ui->table_choose_cou->insertRow(rowCount);
                int i;
                for(i=0;i<8;i++){
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                    if(i==0 || i==1 || i==2 || i==3)
                    {
                        item[i]->setFlags(item[i]->flags()&(~Qt::ItemIsEditable));//设置前四列为不可编辑
                    }
                    if(i==5)
                    {
                        QString daytime = query.value(i).toString();
                        if(daytime.length() != 35)
                        {
                            for(int m = 35 - daytime.length(); m > 0; m--)
                            {
                                daytime = "0" + daytime;
                            }
                        }
                        QString d="";
                        QString time;
                        for(int j = 0; j < daytime.length(); j++)
                        {
                            if(daytime[j]=="1"){
                                int t = j%5;
                                switch (t) {
                                case 0: time = "(1-2)";break;
                                case 1: time = "(3-4)";break;
                                case 2: time = "(5-6)";break;
                                case 3: time = "(7-8)";break;
                                case 4: time = "(9-11)";break;
                                }
                             d = d +"周"+(QString::number(int(j/5+1)))+time+" ";
                            }
                        }
                        item[i]->setText(d);
                    }
                    if(i==6)
                    {
                        int count = 0;
                        QString weektime = query.value(i).toString();
                        if(weektime.length() != 20)
                        {
                            for(int m = 20 - weektime.length(); m > 0; m--)
                            {
                                weektime = "0" + weektime;
                            }
                        }
                        for(int j = 0; j < weektime.length(); j++)
                        {
                            if(weektime[j] == '1') count += 1 ;
                        }
                        QString count_ = QString::number(count);
                        item[i]->setText(count_);
                    }
                    ui->table_choose_cou->setItem(rowCount,i,item[i]);
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
    ui->table_choose_cou->blockSignals(false);

}

//------修改------
QString admin::get_choose_cou_column_name(int n){
    if(n==4) return QString("term");
    if(n==5) return QString("daytime");
    if(n==6) return QString("weektime");
    if(n==7) return QString("clr");
}

//在表格中修改内容
void admin::on_table_choose_cou_itemChanged(QTableWidgetItem *item)
{
    ui->table_choose_cou->blockSignals(true);
    int row=item->row();
    QString column_name=get_choose_cou_column_name(item->column());
    QString updatesql;
    if(item->column()==5 || item->column()==6)
    {
        updatesql="update real_course set "+column_name+
                "=b'"+item->text()+"' where rcno='"+ui->table_choose_cou->item(row,0)->text()+"'";
    }
    else
    {
        updatesql="update real_course set "+column_name+
                "='"+item->text()+"' where rcno='"+ui->table_choose_cou->item(row,0)->text()+"'";
    }
    QSqlQuery query;
    if(query.exec(updatesql)){
        QMessageBox::information(nullptr,"修改成功","修改成功！");
    }
    else{
        connectErrorMsg=query.lastError().text();
        if(connectErrorMsg.contains("foreign key constraint",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的课室号不存在，请重新输入！");
        }
        else if(connectErrorMsg.contains("too long for column" ,Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"数据库插入错误","输入的位数过长，请重新输入！");
        }
        else{
            QMessageBox::information(nullptr,"修改错误","修改错误！错误信息："+connectErrorMsg);
        }
    }

    ui->table_cou->blockSignals(false);
}

//----添加选课课程信息
void admin::on_btn_choose_cou_add_clicked()
{
    QString rcno=ui->ld_choose_cou_rcno1->text();
    QString cno=ui->ld_choose_cou_cno1->text();
    QString tno=ui->ld_choose_cou_tno1->text();
    QString term=ui->cbox_choose_cou_term->currentText();
    QString daytime=ui->ld_choose_cou_daytime->text();
    QString weektime=ui->ld_choose_cou_weektime->text();
    QString clr=ui->cbox_choose_cou_clr->currentText();
    if(rcno.size()==0||cno.size()==0||tno.size()==0||daytime.size()==0||weektime.size()==0||
            clr.size()==0){
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }
    else{

        QString sql="insert into real_course value ('"+rcno+"','"+cno+"','"+tno+"','"+term+"', b'"+daytime+"', b'"+weektime+"','"+clr+"')";
        QSqlQuery query;
        if(query.exec(sql)){
            ui->ld_choose_cou_rcno1->setText(""); //清空输入框
            ui->ld_choose_cou_cno1->setText("");
            ui->cbox_choose_cou_term->setCurrentIndex(-1);
            ui->cbox_choose_cou_clr->setCurrentIndex(-1);
            ui->ld_choose_cou_tno1->setText("");
            ui->ld_choose_cou_daytime->setText("");
            ui->ld_choose_cou_weektime->setText("");
            QMessageBox::information(nullptr,"添加成功","添加成功！");
        }
        //输出非法信息提示
        else{
            connectErrorMsg=query.lastError().text();
            if(connectErrorMsg.contains("PRIMARY",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的课程代号已存在，请重新输入！");
            }
            else if(connectErrorMsg.contains("too long for column 'rcno'",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的课程代号太长，请重新输入！");
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
//删除选课课程按钮
void admin::on_btn_choose_cou_delete_clicked()
{
    int row=ui->table_choose_cou->currentRow();
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

            QString sql="delete from real_course where rcno='"+ui->table_choose_cou->item(row,0)->text()+"';";
            QSqlQuery query;
            if(query.exec(sql)){
                ui->table_choose_cou->removeRow(row);
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
void admin::on_btn_choose_cou_reset_clicked()
{
    ui->ld_choose_cou_rcno->setText("");
    ui->ld_choose_cou_cno->setText("");
    ui->ld_choose_cou_name->setText("");
    ui->ld_choose_cou_tno->setText("");
    ui->cbox_choose_cou_term->setCurrentIndex(-1);
    ui->ld_choose_cou_daytime->setText("");
    ui->ld_choose_cou_weektime->setText("");
    ui->cbox_choose_cou_clr->setCurrentIndex(-1);
}

//---------跳转到选课学生名单界面-----------

void admin::on_btn_choose_cou_turn_clicked()
{
    int row=ui->table_choose_cou->currentRow();
    if(row!=-1){
        QPushButton *okbtn = new QPushButton(QString("确定"));
        QPushButton *cancelbtn = new QPushButton(QString("取消"));
        QMessageBox *mymsgbox = new QMessageBox;

        mymsgbox->setIcon(QMessageBox::Warning);
        mymsgbox->setWindowTitle(QString("提示"));
        mymsgbox->setText(QString("确定进入学生名单页面吗？"));
        mymsgbox->addButton(okbtn, QMessageBox::AcceptRole);
        mymsgbox->addButton(cancelbtn, QMessageBox::RejectRole);
        mymsgbox->show();
        mymsgbox->exec();//阻塞等待用户输入
        if (mymsgbox->clickedButton()==okbtn)//点击了OK按钮
        {
            QString sql= ui->table_choose_cou->item(row,0)->text();
            a = new admin_choose_cou_detail();
            QObject::connect(this,SIGNAL(sendData(QString)), a,SLOT(recevieData(QString)));
            emit sendData(sql);
            a->show();
        }
    }else{
        QMessageBox::information(nullptr,"未选中","未选中要查看的课程,请点击要查看的行");
    }
}






//----------教师管理part----------胡婷婷

//------查询------
//根据输入框获取查询教师信息sql语句
QString admin::get_tea_querysql(){
    QString sql="select * from teacher ";
    QString condition="";
    QString birth_condition="";
    QString tno=ui->ld_tea_no->text();
    QString tname=ui->ld_tea_name->text();
    QString sex=ui->cbox_tea_sex->currentText();
    QString birth_y=ui->cbox_tea_birth_y->currentText();
    QString birth_m=ui->cbox_tea_birth_m->currentText();
    QString birth_d=ui->cbox_tea_birth_d->currentText();
    QString pos=ui->cbox_tea_pos->currentText();
    QString tel=ui->ld_tea_tel->text();
    QString clg=ui->ld_tea_clg->text();
    if(tno.size()){
        condition+=QString("TNO='"+tno+"' and ");
    }
    if(tname.size()){
        condition+=QString("tname='"+tname+"' and ");
    }
    if(sex.size()){
        condition+=QString("sex='"+sex+"' and ");
    }
    //----生日-------
    if(birth_y.size()){
        birth_condition+=QString(birth_y+"%");
    }
    if(birth_m.size()){
        birth_condition+=QString("%-"+birth_m+"-%");
    }
    if(birth_d.size()){
        birth_condition+=QString("%"+birth_d);
    }
    if(birth_condition.size()){
        condition+="birth like '"+birth_condition+"' and ";
    }
    //---------------
    if(pos.size()){
        condition+=QString("position='"+pos+"' and ");
    }
    if(tel.size()){
        condition+=QString("tel='"+tel+"' and ");
    }
    if(clg.size()){
        condition+=QString("CLG='"+clg+"' and ");
    }
    if(condition.size()){
        sql=sql+"where "+condition.left(condition.size()-4);//去掉最后一个and和空格
    }

    return sql;
}

//教师信息查询按钮
void admin::on_btn_tea_query_clicked()
{
    ui->table_tea->blockSignals(true);
    QSqlQuery query;
    QString sql=get_tea_querysql();
    if(query.exec(sql)){
        if(query.size()){
            ui->table_tea->clearContents();
            ui->table_tea->setRowCount(0);
            while(query.next()){
                //执行完query.exec()后query是指向结果集外的
                //第一次执行query.next()使query指向结果集的第一条记录
                //接下来的query.next()使query指向下一条记录
                QTableWidgetItem* item[7];
                int rowCount=ui->table_tea->rowCount();
                ui->table_tea->insertRow(rowCount);
                int i,j;
                for(i=0;i<7;i++){
                    if(i<3) j=i;
                    else j=i+1; //第四项在数据库中是登录密码，不予显示
                    item[i]=new QTableWidgetItem(query.value(j).toString());
                    if(i==0){
                        item[i]->setFlags(item[i]->flags()&(~Qt::ItemIsEditable));//设置第一列教师号为不可编辑
                    }
                    ui->table_tea->setItem(rowCount,i,item[i]);
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
    ui->table_tea->blockSignals(false);

}


//------修改------
//根据item所在列获取对应的列名
QString admin::get_tea_column_name(int n){
    if(n==0) return QString("TNO");
    if(n==1) return QString("tname");
    if(n==2) return QString("sex");
    if(n==3) return QString("birth");
    if(n==4) return QString("position");
    if(n==5) return QString("tel");
    if(n==6) return QString("CLG");
}

//在表格中修改内容
void admin::on_table_tea_itemChanged(QTableWidgetItem *item)
{
    ui->table_tea->blockSignals(true);

    int row=item->row();
    QString column_name=get_tea_column_name(item->column());
    QString updatesql="update teacher set "+column_name+
            "='"+item->text()+"' where TNO='"+ui->table_tea->item(row,0)->text()+"'";

    QSqlQuery query;
    if(query.exec(updatesql)){
        QMessageBox::information(nullptr,"修改成功","修改成功！");
    }
    //输出非法信息提示
    else{
        connectErrorMsg=query.lastError().text();
        if(connectErrorMsg.contains("constraint 'teacher_chk_1'",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的性别是非法信息，请重新输入！");
        }
        else if(connectErrorMsg.contains("Incorrect date value",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的生日信息是非法日期，请重新输入！");
        }
        else if(connectErrorMsg.contains("foreign key constraint",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的学院号不存在，请重新输入！");
        }
        else if(connectErrorMsg.contains("constraint 'teacher_check_tel'",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的手机号须为11位数，请重新输入！");
        }
        else if(connectErrorMsg.contains("constraint 'teacher_check_pos'",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的教师职位不存在，请重新输入！");
        }
        else{
            QMessageBox::information(nullptr,"修改错误","修改错误！错误信息："+connectErrorMsg);
        }
    }

    ui->table_tea->blockSignals(false);
}


//------添加------
void admin::on_btn_tea_add_clicked()
{
    QString tno=ui->ld_tea_no->text();
    QString tname=ui->ld_tea_name->text();
    QString sex=ui->cbox_tea_sex->currentText();
    QString birth_y=ui->cbox_tea_birth_y->currentText();
    QString birth_m=ui->cbox_tea_birth_m->currentText();
    QString birth_d=ui->cbox_tea_birth_d->currentText();
    QString pos=ui->cbox_tea_pos->currentText();
    QString tel=ui->ld_tea_tel->text();
    QString clg=ui->ld_tea_clg->text();
    if(tno.size()==0||tname.size()==0||sex.size()==0||birth_y.size()==0||birth_m.size()==0||
            birth_d.size()==0||pos.size()==0||tel.size()==0||clg.size()==0){
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }
    else{
        QString sql="insert into teacher value ('"+tno+"','"+tname+"','"+sex+"','123456','"+birth_y+"-"+
                birth_m+"-"+birth_d+"','"+pos+"','"+tel+"','"+clg+"')";
        QSqlQuery query;
        if(query.exec(sql)){
            ui->ld_tea_no->setText(""); //清空输入框
            ui->ld_tea_name->setText("");
            ui->cbox_tea_sex->setCurrentIndex(-1);
            ui->cbox_tea_birth_y->setCurrentIndex(-1);
            ui->cbox_tea_birth_m->setCurrentIndex(-1);
            ui->cbox_tea_birth_d->setCurrentIndex(-1);
            ui->cbox_tea_pos->setCurrentIndex(-1);
            ui->ld_tea_tel->setText("");
            ui->ld_tea_clg->setText("");
            QMessageBox::information(nullptr,"添加成功","添加成功！");
        }
        //输出非法信息提示
        else{
            connectErrorMsg=query.lastError().text();
            if(connectErrorMsg.contains("PRIMARY",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的教师号已存在，请重新输入！");
            }
            else if(connectErrorMsg.contains("too long for column 'TNO'",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的教师号太长，请重新输入！");
            }
            else if(connectErrorMsg.contains("Incorrect date value",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的生日信息是非法日期，请重新输入！");
            }
            else if(connectErrorMsg.contains("foreign key constraint",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的学院号不存在，请重新输入！");
            }
            else if(connectErrorMsg.contains("constraint 'teacher_check_tel'",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的手机号须为11位数，请重新输入！");
            }
            else{
                QMessageBox::information(nullptr,"数据库插入错误","数据库插入错误，错误信息："+connectErrorMsg);
            }
        }
    }
}

//------删除----
void admin::on_btn_tea_delete_clicked()
{
    int row=ui->table_tea->currentRow();
    if(row!=-1){
        QPushButton *okbtn = new QPushButton(QString("确定"));
        QPushButton *cancelbtn = new QPushButton(QString("取消"));
        QMessageBox *confirmbox = new QMessageBox;

        confirmbox->setIcon(QMessageBox::Warning);
        confirmbox->setWindowTitle("提示");
        confirmbox->setText("确认删除？");
        confirmbox->addButton(okbtn,QMessageBox::AcceptRole);
        confirmbox->addButton(cancelbtn,QMessageBox::RejectRole);
        confirmbox->show();

        confirmbox->exec();//使函数“暂停”，等待用户点击按钮

        if(confirmbox->clickedButton()==okbtn){//点击确认按钮
            QString sql="delete from teacher where TNO='"+ui->table_tea->item(row,0)->text()+"';";
            QSqlQuery query;
            if(query.exec(sql)){
                ui->table_tea->removeRow(row);
                QMessageBox::information(nullptr,"删除成功","删除成功！");
            }
            else{
                connectErrorMsg=query.lastError().text();
                QMessageBox::information(nullptr,"数据库删除错误","数据库删除错误，错误信息"+connectErrorMsg);
            }
        }
    }
    else{
        QMessageBox::information(nullptr,"未选中","未选中删除行，请选择一行进行删除");
    }
}


//------重置输入框------
void admin::on_btn_tea_reset_clicked()
{
    ui->ld_tea_no->setText("");
    ui->ld_tea_name->setText("");
    ui->cbox_tea_sex->setCurrentIndex(-1);
    ui->cbox_tea_birth_y->setCurrentIndex(-1);
    ui->cbox_tea_birth_m->setCurrentIndex(-1);
    ui->cbox_tea_birth_d->setCurrentIndex(-1);
    ui->cbox_tea_pos->setCurrentIndex(-1);
    ui->ld_tea_tel->setText("");
    ui->ld_tea_clg->setText("");
}


//----------学生管理part----------胡婷婷

//------查询------
//根据输入框获取查询学生信息sql语句
QString admin::get_stu_querysql(){
    QString sql="select * from student ";
    QString condition="";
    QString birth_condition="";
    QString sno=ui->ld_stu_no->text();
    QString sname=ui->ld_stu_name->text();
    QString sex=ui->cbox_stu_sex->currentText();
    QString hometown=ui->ld_stu_hometown->text();
    QString birth_y=ui->cbox_stu_birth_y->currentText();
    QString birth_m=ui->cbox_stu_birth_m->currentText();
    QString birth_d=ui->cbox_stu_birth_d->currentText();
    QString cls=ui->ld_stu_cls->text();
    if(sno.size()){
        condition+=QString("sno='"+sno+"' and ");
    }
    if(sname.size()){
        condition+=QString("sname='"+sname+"' and ");
    }
    if(sex.size()){
        condition+=QString("sex='"+sex+"' and ");
    }
    if(hometown.size()){
        condition+=QString("hometown='"+hometown+"' and ");
    }
    //----生日-------
    if(birth_y.size()){
        birth_condition+=QString(birth_y+"%");
    }
    if(birth_m.size()){
        birth_condition+=QString("%-"+birth_m+"-%");
    }
    if(birth_d.size()){
        birth_condition+=QString("%"+birth_d);
    }
    if(birth_condition.size()){
        condition+="birth like '"+birth_condition+"' and ";
    }
    //---------------
    if(cls.size()){
        condition+=QString("CLS='"+cls+"' and ");
    }
    if(condition.size()){
        sql=sql+"where "+condition.left(condition.size()-4);//去掉最后一个and和空格
    }

    return sql;
}


//学生信息查询按钮
void admin::on_btn_stu_query_clicked()
{
    ui->table_stu->blockSignals(true);
    QSqlQuery query;
    QString sql=get_stu_querysql();
    if(query.exec(sql)){
        if(query.size()){
            ui->table_stu->clearContents();
            ui->table_stu->setRowCount(0);
            while(query.next()){
                QTableWidgetItem* item[6];
                int rowCount=ui->table_stu->rowCount();
                ui->table_stu->insertRow(rowCount);
                for(int i=0;i<6;i++){
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                    if(i==0){
                        item[i]->setFlags(item[i]->flags()&(~Qt::ItemIsEditable));//设置第一列学号为不可编辑
                    }
                    ui->table_stu->setItem(rowCount,i,item[i]);
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
    ui->table_stu->blockSignals(false);
}


//------修改------
QString admin::get_stu_column_name(int n){
    if(n==0) return QString("sno");
    if(n==1) return QString("sname");
    if(n==2) return QString("sex");
    if(n==3) return QString("hometown");
    if(n==4) return QString("birth");
    if(n==5) return QString("CLS");
}

//在表格中修改内容
void admin::on_table_stu_itemChanged(QTableWidgetItem *item)
{
    ui->table_stu->blockSignals(true);

    int row=item->row();
    QString column_name=get_stu_column_name(item->column());
    QString updatesql="update student set "+column_name+
            "='"+item->text()+"' where sno='"+ui->table_stu->item(row,0)->text()+"'";

    QSqlQuery query;
    if(query.exec(updatesql)){
        QMessageBox::information(nullptr,"修改成功","修改成功！");
    }
    else{
        connectErrorMsg=query.lastError().text();
        if(connectErrorMsg.contains("constraint 'student_chk_1'",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的性别是非法信息，请重新输入！");
        }
        else if(connectErrorMsg.contains("Incorrect date value",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的生日信息是非法日期，请重新输入！");
        }
        else if(connectErrorMsg.contains("foreign key constraint",Qt::CaseSensitive)){
            QMessageBox::information(nullptr,"修改错误","输入的班级号不存在，请重新输入！");
        }
        else{
            QMessageBox::information(nullptr,"修改错误","修改错误！错误信息："+connectErrorMsg);
        }
    }

    ui->table_stu->blockSignals(false);
}

//------添加------
void admin::on_btn_stu_add_clicked()
{
    QString sno=ui->ld_stu_no->text();
    QString sname=ui->ld_stu_name->text();
    QString sex=ui->cbox_stu_sex->currentText();
    QString hometown=ui->ld_stu_hometown->text();
    QString birth_y=ui->cbox_stu_birth_y->currentText();
    QString birth_m=ui->cbox_stu_birth_m->currentText();
    QString birth_d=ui->cbox_stu_birth_d->currentText();
    QString cls=ui->ld_stu_cls->text();
    if(sno.size()==0||sname.size()==0||sex.size()==0||birth_y.size()==0||birth_m.size()==0||
            birth_d.size()==0||hometown.size()==0||cls.size()==0){
        QMessageBox::information(nullptr,"错误","添加的信息未填写完全");
    }
    else{
        QString sql="insert into student value ('"+sno+"','"+sname+"','"+sex+"','"+
                hometown+"','"+birth_y+"-"+birth_m+"-"+birth_d+"','"+cls+"','123456')";
        QSqlQuery query;
        if(query.exec(sql)){
            ui->ld_stu_no->setText(""); //清空输入框
            ui->ld_stu_name->setText("");
            ui->cbox_stu_sex->setCurrentIndex(-1);
            ui->ld_stu_hometown->setText("");
            ui->cbox_stu_birth_y->setCurrentIndex(-1);
            ui->cbox_stu_birth_m->setCurrentIndex(-1);
            ui->cbox_stu_birth_d->setCurrentIndex(-1);
            ui->ld_stu_cls->setText("");
            QMessageBox::information(nullptr,"添加成功","添加成功！");
        }
        else{
            connectErrorMsg=query.lastError().text();
            if(connectErrorMsg.contains("PRIMARY",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的学号已存在，请重新输入！");
            }
            else if(connectErrorMsg.contains("too long for column 'TNO'",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的学号太长，请重新输入！");
            }
            else if(connectErrorMsg.contains("Incorrect date value",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的生日信息是非法日期，请重新输入！");
            }
            else if(connectErrorMsg.contains("foreign key constraint",Qt::CaseSensitive)){
                QMessageBox::information(nullptr,"数据库插入错误","输入的班级号不存在，请重新输入！");
            }
            else{
                QMessageBox::information(nullptr,"数据库插入错误","数据库插入错误，错误信息："+connectErrorMsg);
            }
        }
    }
}


//------删除------
void admin::on_btn_stu_delete_clicked()
{
    int row=ui->table_stu->currentRow();
    if(row!=-1){
        QPushButton *okbtn = new QPushButton(QString("确定"));
        QPushButton *cancelbtn = new QPushButton(QString("取消"));
        QMessageBox *confirmbox = new QMessageBox;

        confirmbox->setIcon(QMessageBox::Warning);
        confirmbox->setWindowTitle("提示");
        confirmbox->setText("确认删除？");
        confirmbox->addButton(okbtn,QMessageBox::AcceptRole);
        confirmbox->addButton(cancelbtn,QMessageBox::RejectRole);
        confirmbox->show();

        confirmbox->exec();//使函数“暂停”，等待用户点击按钮

        if(confirmbox->clickedButton()==okbtn){//点击确认按钮
            QString sql="delete from student where sno='"+ui->table_stu->item(row,0)->text()+"';";
            QSqlQuery query;
            if(query.exec(sql)){
                ui->table_stu->removeRow(row);
                QMessageBox::information(nullptr,"删除成功","删除成功！");
            }
            else{
                connectErrorMsg=query.lastError().text();
                QMessageBox::information(nullptr,"数据库删除错误","数据库删除错误，错误信息"+connectErrorMsg);
            }
        }
    }
    else{
        QMessageBox::information(nullptr,"未选中","未选中删除行，请选择一行进行删除");
    }
}

//------重置输入框------
void admin::on_btn_stu_reset_clicked()
{
    ui->ld_stu_no->setText("");
    ui->ld_stu_name->setText("");
    ui->cbox_stu_sex->setCurrentIndex(-1);
    ui->ld_stu_hometown->setText("");
    ui->cbox_stu_birth_y->setCurrentIndex(-1);
    ui->cbox_stu_birth_m->setCurrentIndex(-1);
    ui->cbox_stu_birth_d->setCurrentIndex(-1);
    ui->ld_stu_cls->setText("");
}
