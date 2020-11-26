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

void teacher::on_college_currentIndexChanged(int index)
{
    qDebug()<<college_no[index];

//    QString sql="select rcno from real_course where tno='"+cur_teacher.tno+"' and rcno like %"
//            +college_name[index]+"%";
//    query.exec(sql);
//    while(query.next()){

//    }
}










