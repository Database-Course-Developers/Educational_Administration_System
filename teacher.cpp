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


void teacher::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int crr_row=ui->listWidget->currentRow();
    if(crr_row==0)ui->stackedWidget->setCurrentIndex(0);
    else if(crr_row==1)ui->stackedWidget->setCurrentIndex(1);
    else if(crr_row==2)ui->stackedWidget->setCurrentIndex(2);
    else if(crr_row==3)ui->stackedWidget->setCurrentIndex(3);
    else if(crr_row==4)ui->stackedWidget->setCurrentIndex(4);
    else ui->stackedWidget->setCurrentIndex(5);
}


void teacher::on_search_3_clicked(bool checked)
{
    QString major= ui->major_2->currentText();
    QString course=ui->course_2->currentText();

}


void teacher::on_comboBox_3_currentIndexChanged(int index)
{
    if(ui->comboBox_3->currentIndex()==2)
        ui->lineedit_info->setVisible(false);

}


//void teacher::on_search_3_clicked(bool checked)
//{
//    QString major= ui->major_2->currentText();
//    QString course=ui->course_2->currentText();

//}


//void teacher::on_comboBox_3_currentIndexChanged(int index)
//{
//    if(ui->comboBox_3->currentIndex()==2)
//        ui->lineedit_info->setVisible(false);
//}

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
                "where tno='"+cur_teacher.tno+"' and real_course.cno = course.CNO";
    query.exec(sql);
    QString cour_info;


    while(query.next()){
//        qDebug()<<"\t\tquery.value(1).toString()="<<query.value(1).toChar();

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
        }
    }
}


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
