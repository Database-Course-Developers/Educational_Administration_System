
#include "teacher.h"
#include "ui_teacher.h"
#include<QDebug>
teacher::teacher(tea cur_teacher,QWidget *parent) :
    QWidget(parent),cur_teacher(cur_teacher),
    ui(new Ui::teacher)
{
    ui->setupUi(this);

    initial_personal_info();
    initial_course();
//    警告：Using QCharRef with an index pointing outside the valid range of a QString.
//    The corresponding behavior is deprecated, and will be changed in a future version of Qt.
    initial_student_info();
    //屏幕最大化
    setWindowState(Qt::WindowMaximized);
    ui->stackedWidget->setCurrentIndex(0);

}

teacher::~teacher()
{
    delete ui;
}


void teacher::on_listWidget_itemClicked()
{
    int crr_row=ui->listWidget->currentRow();
    if(crr_row==0)ui->stackedWidget->setCurrentIndex(0);
    else if(crr_row==1)ui->stackedWidget->setCurrentIndex(1);
    else if(crr_row==2)ui->stackedWidget->setCurrentIndex(2);
    else if(crr_row==3)
    {
        ui->major_3->clear();
        ui->stackedWidget->setCurrentIndex(3);
        initial_class(ui->major_3,1);
    }

    else
    {
        ui->major_2->clear();
        ui->stackedWidget->setCurrentIndex(4);
        initial_class(ui->major_2,0);
}
}

// 初始化个人信息界面
void teacher::initial_personal_info(){
    QString sql="select Name from college where CLG='"+cur_teacher.CLG+"'";
    query.exec(sql);
    if(query.next()){
        clg_name=query.value(0).toString();
    }

    ui->ledit_tno->setText(cur_teacher.tno);
    ui->ledit_tname->setText(cur_teacher.tname);
    ui->ledit_sex->setText(cur_teacher.sex);
    ui->ledit_bitrh->setText(cur_teacher.birth);
    ui->ledit_position->setText(cur_teacher.position);
    ui->ledit_tel->setText(cur_teacher.tel);
    ui->ledit_clg->setText(clg_name);
}

QString teacher::print_weektime(QString weektime){// 开课时间-eg.1-12周,14-15周
    QString weektime_output;
    int start,end;
    bool flag=false;
    bool first=true;
    for(int i = 0; i < weektime.length(); i++){
        if(weektime[i] == "1" and flag == false){//连续开课的第一周
            start = i+1;
            flag = true;
        }
        else if(weektime[i] == "0" and flag == true){// 连续开课的最后一周
            end = i+1;
            flag = false;
            if(first == true){
                weektime_output +=QString::number(start)+'-'+QString::number(end)+"周";
                first = false;
            }
            else{
                 weektime_output +=","+QString::number(start)+'-'+QString::number(end)+"周";
            }

        }
    }

    return weektime_output;
}

QStringList teacher::print_daytime(QString daytime){// 上课时间-eg.(1-2节)
    QStringList daytime_output;
    QString temp;
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 5; j++){
            if(daytime[5*i+j] == "1"){
                switch (j) {
                case 0:temp="(1-2节)";break;
                case 1:temp="(3-4节)";break;
                case 2:temp="(5-6节)";break;
                case 3:temp="(7-8节)";break;
                case 4:temp="(9-11节)";break;
                }
                daytime_output.append(temp);//待写入信息-eg.(1-2节)
                daytime_output.append(QString::number(i+1));//记录周几上课-[1,7]
                daytime_output.append(QString::number(j+1));//记录第几节大课-[1,5]
            }
        }
    }
    return daytime_output;
}

// 初始化课程表
void teacher::initial_course(){
    QString sql="select rcno, bin(daytime+0), bin(weektime+0), CLR, course.name, credits, "
                "bin(required+0), bin(is_exam+0), hour from real_course, course "
                "where tno='"+cur_teacher.tno+"' and real_course.cno = course.cno";
    query.exec(sql);
    QString cour_info;
    while(query.next()){
        // 0-课务号 1-每周上课时间 2-开课周 3-课室号 4-课程名 5-学分 6-是否必修 7-是否考试 8-学时
        QString daytime=query.value(1).toString();// 上课时间-eg.(1-2节)
        QString weektime=query.value(2).toString();// 开课时间-eg.1-12周

        for(int i=0; i<print_daytime(daytime).length()/3; i++){
            cour_info=query.value(4).toString()+'\n'+print_daytime(daytime)[3*i]+print_weektime(weektime)
                    +'/'+query.value(3).toString()+'/'+cur_teacher.tname+'/'+query.value(0).toString()
                    +'/'+(query.value(6).toString() == "1" ? "必修" : "选修")
                    +'/'+"是否考试:"+(query.value(7).toString() == "1" ? "是" : "否")
                    +"/学时:"+query.value(8).toString()+"/学分:"+query.value(5).toString();

            //把cour_info写进课程表中 周：print_daytime(daytime)[3*i+1] 节：print_daytime(daytime)[3*i+2]
            ui->lesson_table->setItem(print_daytime(daytime)[3*i+2].toInt()-1,
                    print_daytime(daytime)[3*i+1].toInt()-1, new QTableWidgetItem(cour_info));

            //qDebug()<<"\ninfo:\n"<<cour_info;
        }
    }
}

// 初始化学生信息界面
void teacher::initial_student_info(){
    // 筛选出老师教授的学院 专业 课程
    QString sql="select substring(rcno, 11, 2) clg_no, (select name from college where CLG = clg_no), "
                "substring(rcno, 7, 8) major_no, concat(substring(rcno, 9, 2),'级',(select name from major where MJR = substring(rcno, 11, 3)),substring(rcno, 14, 1),'班'), "
                "real_course.cno, course.name from real_course, college, course "
                "where tno='"+cur_teacher.tno+"' and real_course.cno = course.cno";
    query.exec(sql);
    while(query.next()){
        qDebug()<<"new:\n"<<query.value(0).toString()<<'\t'<<query.value(1).toString()<<'\t'
                <<query.value(2).toString()<<'\t'<<query.value(3).toString()<<'\t'
                <<query.value(4).toString()<<'\t'<<query.value(5).toString()<<'\n';
    }




    QString cour_info;
    //QStringList college;
    college_no.append("-1");college_name.append("全部");
    //QStringList major;
    major_no.append("-1");major_name.append("全部");
    //QStringList course;
    course_no.append("-1");course_name.append("全部");
    while(query.next()){
        cour_info=query.value(0).toString();
        college_no.append(cour_info.mid(10,2));
        major_no.append(cour_info.mid(6,8));
        course_no.append(cour_info.mid(15,5));
    }
    college_no.removeDuplicates();
    major_no.removeDuplicates();
    course_no.removeDuplicates();

//    // college 编号->名字
//    for(int i=1; i<college_no.length(); i++){
//        sql="select name from college where CLG='"+college_no[i]+"'";
//        query.exec(sql);
//        if(query.next()){
//            college_name.append(query.value(0).toString());
//            qDebug()<<college_name[i]<<'\t';
//        }
//    }
//    // major 编号->名字
//    for(int i=1; i<major_no.length(); i++){
//        sql="select name from major where MJR='"+(major_no[i].mid(4,3))+"'";
//        query.exec(sql);
//        if(query.next()){
//            major_name.append(major_name[i].mid(2,2)+"级"+query.value(0).toString()+major_name[i].mid(7,1)+"班");
//            qDebug()<<major_name[i]<<'\t';
//        }
//    }
//    // course从编号->名字
//    for(int i=1; i<college_no.length(); i++){
//        sql="select name from course where cno='"+course_no[i]+"'";
//        query.exec(sql);
//        if(query.next()){
//            course_name.append(query.value(0).toString());
//            qDebug()<<course_name[i]<<'\t';
//        }
//    }

//    ui->college->addItems(college_name);
//    ui->major->addItems(major_name);
//    ui->course->addItems(course_name);

}
/*
void teacher::on_college_currentIndexChanged(int index)
{
    qDebug()<<college_no[index];

//    QString sql="select rcno from real_course where tno='"+cur_teacher.tno+"' and rcno like %"
//            +college_name[index]+"%";
//    query.exec(sql);
//    while(query.next()){

//    }
}
*/
//------------------------------------------------------------------成绩信息----梁靖欣------------------------------------------------------
//初始化该老师的班级
void teacher::initial_class(QComboBox*a,int option){

    a->clear();
    QVector<QString> clsno_lst;
    QVector<QString> crsname_lst;
    QVector<QString> clsname_lst;
    QString sql;
    //获取老师的有成绩的对应的班级号
    if(option==0)//option为0-->加载成绩查询
    {  sql="SELECT DISTINCT SUBSTR(RCNO,7,8) FROM real_course WHERE TNO='"+cur_teacher.tno+"' AND CNO IN (SELECT CNO FROM course WHERE is_exam=1)";}
    if(option==1)//option为1-->加载考试查询
    {
        sql="SELECT DISTINCT SUBSTR(RCNO,7,8) FROM exam_view WHERE tno1='"+cur_teacher.tno+"' OR tno2='"+cur_teacher.tno+"'";
    }
    query.exec(sql);
    while(query.next()){
        clsno_lst.append(query.value(0).toString());
    }
    //班级名载入
    for(int i=0;i<clsno_lst.length();i++)
    {

        sql="SELECT DISTINCT `name` FROM _class WHERE CLS='"+clsno_lst[i]+"'";
        qDebug()<<"clsno_lst:"<<clsno_lst[i]<<endl;
        query.exec(sql);
        while(query.next()){
            clsname_lst.append(query.value(0).toString());
        }
    }
     //按班级查询时候选框加载
        for(int i=0;i<clsname_lst.length();i++)
        {
            //qDebug()<<"clsname_lst:"<<clsname_lst[i]<<endl;
            a->addItem(clsname_lst[i]);

        }

}

//班级框选择信息后，对应科目信息的加载
void teacher::on_major_2_currentIndexChanged()
{

    QString clsname=ui->major_2->currentText();
    QString sql="SELECT CLS FROM _class WHERE `name`='"+clsname+"'";
    QVector<QString> crsnamelst;
    QString clsno;
    query.exec(sql);
    //加载班级号
    while(query.next())
    {
        clsno=query.value(0).toString();
    }
    ui->course_2->clear();
    sql="SELECT `name` FROM course WHERE  is_exam=1 AND CNO IN (SELECT CNO FROM real_course WHERE TNO='"+cur_teacher.tno+"' AND RCNO LIKE '%"+clsno+"%') ";
        query.exec(sql);
        while(query.next())
        {
            qDebug()<<query.value(0).toString();
            ui->course_2->addItem(query.value(0).toString());
        }

}


//成绩信息查询
void teacher::grade_search_function(int desc,int asc,int avg,int level)
{
    ui->score_table->clear();
    ui->score_table->setRowCount(0);
    ui->score_table->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"学号"<<"性别"<<"班级"<<"成绩");
    //升序，降序功能的实现
    QString str_asc="ORDER BY grade ASC";
    QString str_desc="ORDER BY grade DESC";
    if(desc==0)str_desc="";
    if(asc==0)str_asc="";
    QString clsname=ui->major_2->currentText();
    QString crsname=ui->course_2->currentText();
    QVector<QString> sno_lst;
    QString sql;
    QString clsno;
    sql="SELECT CLS FROM _class WHERE `name`='"+clsname+"'";
    query.exec(sql);
    while(query.next())
    {clsno=query.value(0).toString();}
    qDebug()<<"clsno:"<<clsno<<endl;
    //输入框为空，默认查找所有学生时
    if(ui->lineedit_input->text().length()==0)
    {
        //加载该班的姓名、学号、成绩、班级、性别
        sql="SELECT DISTINCT grade_view.sname,grade_view.sno,sex,grade FROM real_course,grade_view,student  WHERE real_course.TNO='"+cur_teacher.tno+"'   AND real_course.CNO=grade_view.cno AND student.sno LIKE'%"+clsno+"%' AND student.sno=grade_view.sno "+str_asc+" "+str_desc;
        query.exec(sql);
        if(query.size()==0)
        {
            QMessageBox::information(NULL, "该班的此科目暂时无成绩信息", "抱歉，目前该班的此科目因为尚未考试等原因，暂无成绩信息。");
        }
        while(query.next())
       {
            int curr=ui->score_table->rowCount();

            ui->score_table->insertRow(curr);
            sno_lst.append(query.value(0).toString());//该班的全部人的学号
            ui->score_table->setItem(curr,0,new QTableWidgetItem(query.value(1).toString()));
            ui->score_table->setItem(curr,1,new QTableWidgetItem(query.value(0).toString()));
            ui->score_table->setItem(curr,4,new QTableWidgetItem(query.value(2).toString()));
            ui->score_table->setItem(curr,3,new QTableWidgetItem(clsname));
            ui->score_table->setItem(curr,2,new QTableWidgetItem(query.value(3).toString()));
        }
        //平均分功能的实现
        if(avg==1)
        {
            sql="SELECT AVG(grade_view.grade)FROM grade_view ,student WHERE grade_view.sno=student.sno AND cname='"+crsname+"'AND grade_view.sno IN(SELECT student.sno FROM student WHERE CLS= '"+clsno+"')";
            query.exec(sql);
            while(query.next())
            {
                qDebug()<<query.value(0).toString();
                ui->label_avg->setText("平均分为：  "+query.value(0).toString());
            }
        }
        //数据分段功能的实现
        if(level==1)
        {
            ui->level_table->clear();
            ui->level_table->setRowCount(0);
            ui->level_table->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"学号"<<"性别"<<"班级"<<"成绩");
            QString upper=ui->lineedit_upper->text();
            QString low=ui->lineedit_low->text();
            sql="SELECT grade_view.sno,grade_view.sname,grade,sex FROM grade_view ,student WHERE grade_view.sno=student.sno AND cname='"+crsname+"'AND grade_view.sno IN(SELECT student.sno FROM student WHERE CLS LIKE '%"+clsno+"%') AND grade_view.grade<="+upper+"  AND grade_view.grade>="+low;
            query.exec(sql);
            while(query.next())
           {
                int curr=ui->level_table->rowCount();
                ui->level_table->insertRow(curr);
                ui->level_table->setItem(curr,0,new QTableWidgetItem(query.value(1).toString()));
                ui->level_table->setItem(curr,1,new QTableWidgetItem(query.value(0).toString()));
                ui->level_table->setItem(curr,4,new QTableWidgetItem(query.value(2).toString()));
                ui->level_table->setItem(curr,3,new QTableWidgetItem(clsname));
                ui->level_table->setItem(curr,2,new QTableWidgetItem(query.value(3).toString()));
            }

        }
    }
    //输入框在输入学号/姓名时
    else
    {

        if(ui->lineedit_input->text().length()>6)//判定输入的为学号时
            sql="SELECT grade_view.sno,grade_view.sname,grade,sex FROM grade_view ,student WHERE grade_view.sno=student.sno AND cname='"+crsname+"'AND grade_view.sno ='"+ui->lineedit_input->text()+"'";
        else//判定输入的为姓名时
            sql="SELECT grade_view.sno,grade_view.sname,grade,sex FROM grade_view ,student WHERE grade_view.sno=student.sno AND cname='"+crsname+"'AND grade_view.sname ='"+ui->lineedit_input->text()+"'";
        //执行查询并加载表格
        query.exec(sql);
        while(query.next())
        {
            int curr=ui->score_table->rowCount();
            ui->score_table->insertRow(curr);
            ui->score_table->setItem(curr,0,new QTableWidgetItem(query.value(1).toString()));
            ui->score_table->setItem(curr,1,new QTableWidgetItem(query.value(0).toString()));
            ui->score_table->setItem(curr,2,new QTableWidgetItem(query.value(3).toString()));
            ui->score_table->setItem(curr,3,new QTableWidgetItem(clsname));
            ui->score_table->setItem(curr,4,new QTableWidgetItem(query.value(2).toString()));
        }
    }
    ui->score_table->show();
    ui->level_table->show();
}
//成绩界面的查询执行按钮
void teacher::on_search_3_clicked()
{

    grade_search_function(0,0,0,0);
}
void teacher::on_butt_asc_clicked()
{
    grade_search_function(0,1,0,0);
}
void teacher::on_butt_desc_clicked()
{
    grade_search_function(1,0,0,0);
}

void teacher::on_butt_avg_clicked()
{
    grade_search_function(0,0,1,0);
}

void teacher::on_butt_level_clicked()
{

    ui->stackedWidget->setCurrentIndex(5);
    grade_search_function(0,0,0,1);
}

void teacher::on_search_5_clicked()
{
    grade_search_function(0,0,0,1);

}

void teacher::on_butt_return_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
//------------------------------------------------------------------考试信息----梁靖欣------------------------------------------------------

//根据班级加载科目
void teacher::on_major_3_currentIndexChanged()
{
    ui->course_1->clear();
    QString clsname=ui->major_3->currentText();
    QString sql="SELECT CLS FROM _class WHERE `name`='"+clsname+"'";
    QVector<QString> crsnamelst;
    QString clsno;
    query.exec(sql);
    //加载班级号
    while(query.next())
    {
        clsno=query.value(0).toString();
    }
    ui->course_1->clear();
    sql="SELECT `name` FROM course WHERE CNO IN (SELECT cno FROM  exam_view WHERE RCNO LIKE '%"+clsno+"%' AND (tno1='"+cur_teacher.tno+"' OR tno2='"+cur_teacher.tno+"'))";
        query.exec(sql);
        while(query.next())
        {
            qDebug()<<query.value(0).toString();
            ui->course_1->addItem(query.value(0).toString());
        }


}

//考试信息的查询功能
void teacher::on_search_2_clicked()
{
    QVector<QString> clsno_lst;
    QVector<QString> clsname_lst;
    ui->exam_table->clear();
    ui->exam_table->setRowCount(0);
    ui->exam_table->setHorizontalHeaderLabels(QStringList()<<"科目"<<"学院"<<"班级"<<"考试开始时间"<<"考试结束时间"<<"考试地点"<<"监考员1"<<"监考员2");
    QString crsname=ui->course_1->currentText();
    QString clsname=ui->major_3->currentText();
    QString sql="SELECT CLS FROM _class WHERE `name`='"+clsname+"'";
    QString clsno;
    query.exec(sql);
    while(query.next())
    {clsno=query.value(0).toString();}
    qDebug()<<"clsno:"<<clsno<<endl;
    //加载科目、学院、考试开始时间、考试结束时间、考试地点、监考员1、监考员2
    sql="SELECT course.`name`,collegename,begin_time,end_time,clr,tno1,tno2 ,RCNO FROM exam_view,course WHERE (tno1='"+cur_teacher.tno+"' OR tno2='"+cur_teacher.tno+"') AND exam_view.CNO=course.CNO AND rcno LIKE '%"+clsno+"%' AND course.`name`='"+crsname+"'" ;
    query.exec(sql);
    while(query.next())
    {
       int curr=ui->exam_table->rowCount();
       ui->exam_table->insertRow(curr);
        ui->exam_table->setItem(curr,0,new QTableWidgetItem(query.value(0).toString()));
       ui->exam_table->setItem(curr,1,new QTableWidgetItem(query.value(1).toString()));
       ui->exam_table->setItem(curr,2,new QTableWidgetItem(clsname));
       ui->exam_table->setItem(curr,3,new QTableWidgetItem(query.value(2).toString()));
       ui->exam_table->setItem(curr,4,new QTableWidgetItem(query.value(3).toString()));
       ui->exam_table->setItem(curr,5,new QTableWidgetItem(query.value(4).toString()));
       ui->exam_table->setItem(curr,6,new QTableWidgetItem(query.value(5).toString()));
       ui->exam_table->setItem(curr,7,new QTableWidgetItem(query.value(6).toString()));

    }
    ui->exam_table->show();
}


