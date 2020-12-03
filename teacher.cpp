
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

//---------------------个人信息----董妙君------------------------
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


//---------------------课程查询----董妙君------------------------
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
            end = i;
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

QString teacher::fill_zero(QString in,int len){
    QString tmp;
    for(int k=0; in.length()+k<len; k++){
        tmp[k]='0';
    }
    return tmp+in;
}

// 初始化课程表
void teacher::initial_course(){
    QString sql="select rcno, bin(daytime+0), bin(weektime+0), CLR, course.name, credits, "
                "bin(required+0), bin(is_exam+0), hour from real_course, course "
                "where tno='"+cur_teacher.tno+"' and real_course.cno = course.CNO";
    query.exec(sql);
    QString cour_info;


    while(query.next()){
        // 0-课务号 1-每周上课时间 2-开课周 3-课室号 4-课程名 5-学分 6-是否必修 7-是否考试 8-学时
        QString daytime=query.value(1).toString();// 上课时间-eg.(1-2节)
        QString weektime=query.value(2).toString();// 开课时间-eg.1-12周
        daytime = fill_zero(daytime,35);
        weektime = fill_zero(weektime,20);

        for(int i=0; i<print_daytime(daytime).length()/3; i++){
            cour_info=query.value(4).toString()+'\n'+print_daytime(daytime)[3*i]+print_weektime(weektime)
                    +'/'+query.value(3).toString()+'/'+cur_teacher.tname+'/'+query.value(0).toString()
                    +'/'+(query.value(6).toString() == "1" ? "必修" : "选修")
                    +'/'+"是否考试:"+(query.value(7).toString() == "1" ? "是" : "否")
                    +"/学时:"+query.value(8).toString()+"/学分:"+query.value(5).toString();

            //把cour_info写进课程表中 周：print_daytime(daytime)[3*i+1] 节：print_daytime(daytime)[3*i+2]
            ui->lesson_table->setItem(print_daytime(daytime)[3*i+2].toInt()-1,
                    print_daytime(daytime)[3*i+1].toInt()-1, new QTableWidgetItem(cour_info));
        }
    }
}

//---------------------学生查询----董妙君------------------------
// 初始化学生信息界面
void teacher::initial_student_info(){
    QString sql;

    // 删除上次创建的视图
    sql="drop view cur_teacher_course";
    query.exec(sql);
    sql="drop view cur_teacher_course_with_name";
    query.exec(sql);

    // 从rcno筛选出子串创建视图cur_teacher_course
    sql="create view cur_teacher_course(CLG, MJR, CLS, cno) as "
        "select substring(rcno, 11, 2), substring(rcno, 11, 3) CLS, substring(rcno, 7, 8), cno "
        "from real_course "
        "where tno='"+cur_teacher.tno+"'";
    query.exec(sql);

    // 创建包含学院、专业、班级、课程编号和对应名字的视图cur_teacher_course_with_name
    sql="create view cur_teacher_course_with_name(clg_no, clg_name, mjr_no, mjr_name, cls_no, cls_name, cour_no, cour_name) as "
        "select cur_teacher_course.CLG, college.name, cur_teacher_course.MJR, major.name, cur_teacher_course.CLS, _class.name, cur_teacher_course.cno, course.name "
        "from cur_teacher_course, college, major, _class, course "
        "where college.CLG = cur_teacher_course.CLG and major.MJR = cur_teacher_course.MJR and _class.CLS = cur_teacher_course.CLS and course.CNO = cur_teacher_course.cno";
    query.exec(sql);

    // 创建_name QStringList写入QCombox
    college_name.append("全部");

    sql="select clg_name, mjr_name, cls_name, cour_name from cur_teacher_course_with_name";
    query.exec(sql);
    while(query.next()){
        college_name.append(query.value(0).toString());
    }

    // 去除重复值
    college_name.removeDuplicates();
    ui->colleges->addItems(college_name);
}

void teacher::on_colleges_currentIndexChanged(const QString &arg1)
{
    QString sql;
    major_name.clear();
    major_name.append("全部");

    if(arg1 == "全部"){
        sql="select distinct mjr_name from cur_teacher_course_with_name where clg_name = '"+college_name[1]+"'";
        for(int i=2;i<college_name.length(); i++){
            sql+=" or clg_name = '"+college_name[i]+"'";
        }
    }
    else{
        sql="select distinct mjr_name from cur_teacher_course_with_name where clg_name = '"+arg1+"'";
    }

    query.exec(sql);
    while(query.next()){
        major_name.append(query.value(0).toString());
    }
    ui->majors->clear();
    ui->majors->addItems(major_name);
}

void teacher::on_majors_currentIndexChanged(const QString &arg1)
{
    if(arg1 == ""){
        return;
    }

    QString sql;
    class_name.clear();
    class_name.append("全部");
    if(arg1 == "全部"){
        sql="select distinct cls_name from cur_teacher_course_with_name where mjr_name = '"+major_name[1]+"'";
        for(int i=2;i<major_name.length(); i++){
            sql+=" or mjr_name = '"+major_name[i]+"'";
        }
    }
    else{
        sql="select distinct cls_name from cur_teacher_course_with_name where mjr_name = '"+arg1+"'";
    }

    query.exec(sql);
    while(query.next()){
        class_name.append(query.value(0).toString());
    }
    ui->classes->clear();
    ui->classes->addItems(class_name);
}

void teacher::on_classes_currentIndexChanged(const QString &arg1)
{
    if(arg1 == ""){
        return;
    }

    QString sql;
    course_name.clear();
    course_name.append("全部");
    if(arg1 == "全部"){
        sql="select distinct cour_name from cur_teacher_course_with_name where cls_name = '"+class_name[1]+"'";
        for(int i=2;i<class_name.length(); i++){
            sql+=" or cls_name = '"+class_name[i]+"'";
        }
    }
    else{
        sql="select distinct cour_name from cur_teacher_course_with_name where cls_name = '"+arg1+"'";
    }

    query.exec(sql);
    while(query.next()){
        course_name.append(query.value(0).toString());
    }
    course_name.removeDuplicates();
    ui->courses->clear();
    ui->courses->addItems(course_name);
}

void teacher::on_search_student_clicked()
{
    ui->student_table->setRowCount(0);

    // 班级->课程->专业->学院
    QString sql;
    QString clg_info=ui->colleges->currentText();
    QString mjr_info=ui->majors->currentText();
    QString cls_info=ui->classes->currentText();
    QString cour_info=ui->courses->currentText();
    int row=0;

    if(cls_info != "全部"){
        sql = "select sno, sname, sex, hometown, birth, cls_name from student, cur_teacher_course_with_name "
              "where student.CLS = cur_teacher_course_with_name.cls_no and CLS in ("
              "select distinct cls_no from cur_teacher_course_with_name "
              "where cls_name = '"+cls_info+"')";
    }
    else if(cour_info != "全部"){
        sql = "select sno, sname, sex, hometown, birth, cls_name from student, cur_teacher_course_with_name "
              "where student.CLS = cur_teacher_course_with_name.cls_no and CLS in ("
              "select distinct cls_no from cur_teacher_course_with_name "
              "where cour_name = '"+cour_info+"')";
    }
    else if(mjr_info != "全部"){
        sql = "select sno, sname, sex, hometown, birth, cls_name from student, cur_teacher_course_with_name "
              "where student.CLS = cur_teacher_course_with_name.cls_no and CLS in ("
              "select distinct cls_no from cur_teacher_course_with_name "
              "where mjr_name = '"+mjr_info+"')";
    }
    else if(clg_info != "全部"){
        sql = "select sno, sname, sex, hometown, birth, cls_name from student, cur_teacher_course_with_name "
              "where student.CLS = cur_teacher_course_with_name.cls_no and CLS in ("
              "select distinct cls_no from cur_teacher_course_with_name "
              "where clg_name = '"+clg_info+"')";
    }
    else{
        sql = "select sno, sname, sex, hometown, birth, cls_name from student, cur_teacher_course_with_name "
              "where student.CLS = cur_teacher_course_with_name.cls_no and CLS in ("
              "select distinct cls_no from cur_teacher_course_with_name)";
    }

    query.exec(sql);
    while(query.next()){
        ui->student_table->insertRow(row);
        for(int i=0; i<6; i++){
            ui->student_table->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
        }
        row++;
    }
}


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
    QVector<QString> tno1_lst;
    QVector<QString> tno2_lst;
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
       QString fulldate0=query.value(2).toString();
       QString fulldate1=query.value(3).toString();
       int curr=ui->exam_table->rowCount();
       ui->exam_table->insertRow(curr);
        ui->exam_table->setItem(curr,0,new QTableWidgetItem(query.value(0).toString()));
       ui->exam_table->setItem(curr,1,new QTableWidgetItem(query.value(1).toString()));
       ui->exam_table->setItem(curr,2,new QTableWidgetItem(clsname));
       ui->exam_table->setItem(curr,3,new QTableWidgetItem(fulldate0.split('T')[0]+"   "+fulldate0.split('T')[1].split('.')[0]));
       ui->exam_table->setItem(curr,4,new QTableWidgetItem(fulldate1.split('T')[0]+"   "+fulldate1.split('T')[1].split('.')[0]));
       ui->exam_table->setItem(curr,5,new QTableWidgetItem(query.value(4).toString()));
       tno1_lst.append( query.value(5).toString());
       tno2_lst.append( query.value(6).toString());

    }
    int curr=0;
    for (int i=0;i<tno1_lst.length();i++)
    {
        sql="SELECT tname FROM teacher WHERE tno='"+tno1_lst[i]+"'";
        query.exec(sql);
        while(query.next())
        {
            ui->exam_table->setItem(curr,6,new QTableWidgetItem(query.value(0).toString()));
        }
        curr++;
    }
    curr=0;
    for (int i=0;i<tno2_lst.length();i++)
    {
        sql="SELECT tname FROM teacher WHERE tno='"+tno2_lst[i]+"'";
        query.exec(sql);
        while(query.next())
        {
            ui->exam_table->setItem(curr,7,new QTableWidgetItem(query.value(0).toString()));
        }
        curr++;
    }
    ui->exam_table->show();
}
