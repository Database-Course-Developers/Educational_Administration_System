#include "student.h"
#include "ui_student.h"
#include<QTextEdit>
#include<database_util.h>
student::student(stu cur_student,QWidget *parent) :
    QWidget(parent),cur_student(cur_student),
    ui(new Ui::student)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    initbox();
}

student::~student()
{
    delete ui;
}

// 初始化学生页面
void student::initbox()
{
    ui->studenPages->setCurrentIndex(0);
    create_view();//创建视图

    //记录学生专业号
    myMJR=cur_student.sno.mid(4,3);
    //记录学生的班级号
    myclass = cur_student.sno.mid(0,8);
    //记录选课的情况
    flag=0;//0代表选课情况没有更改，1代表发生了更改

    // 子页面的初始化
    gradePage();
    timeTablePageInit();
    examPageInit();
    stuInfoPage();
    stuPlanPage();

    // 连接6个按钮和对应子页面
    connect(ui->pBStuInfo, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(1);
    });
    connect(ui->pBStuCho, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(2);
        stuChooselessonPage();
    });
    connect(ui->pBStuPlan, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(3);
        init_stuPlanTable();

    });
    connect(ui->pBStuGrade, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(4);
        ui->tWGrade->clearContents();
        ui->tWGrade->setRowCount(0);
    });
    connect(ui->pBStuTimetable, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(5);
        if(flag == 1) timeTablePage();
    });
    connect(ui->pBStuExam, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(6);
        if(flag == 1) examPage();
    });

    // 返回登陆界面
    connect(ui->pBStuBack, &QPushButton::clicked, [=]()
    {
        emit logout();
        this->close();
    });

}

// 成绩信息子页面 钟子涵
void student::gradePage()
{
    ui->tWGrade->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 成绩信息子页面返回按钮
    connect(ui->pBGradeBack, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(0);
    });
    // 时间下拉框设置
    for (int i = 2017 ; i <= 2020 ;i++)
    {
        ui->cBGradeYear->addItem(QString::number(i));
    }
    ui->cBGradeTerm->addItem(QString::number(1));
    ui->cBGradeTerm->addItem(QString::number(2));

    // 按学生学号、学年、学期查询成绩
    connect(ui->pBSeGraByTime, &QPushButton::clicked, [=]()
    {
        ui->tWGrade->clearContents(); // 清除原有内容

        QString myTime = ui->cBGradeYear->currentText() + ui->cBGradeTerm->currentText();

        QString sqlStr = "select sg.cno, name, credits, grade, bin(required+0) from student_grade sg, course c where "
                         "year = '" + myTime + "' and sg.cno = c.cno";

        setGradeTable(sqlStr);
    });

    // 按课程名模糊查询
    connect(ui->pBSeaGrade, &QPushButton::clicked, [=](){
        QString myLesson = ui->lESeaGraByName->text();
        ui->lESeaGraByName->clear();

        QString sqlStr = "select sg.cno, name, credits, grade, bin(required+0) from student_grade sg, course c where name like '%"
                + myLesson + "%' and c.cno = sg.cno";

        setGradeTable(sqlStr);
    });

    // 获得学生百分制绩点和排名
    calGrade();
}

void student::setGradeTable(QString sqlStr)
{
    QSqlQuery *sqlQuery = new QSqlQuery;
    sqlQuery->prepare( sqlStr );

    if( sqlQuery->exec() )
    {
       //读取查询到的记录
       //QTableWidget需要先设置一共有多少行数据才能正常显示，如果不设置那么默认是0行数据。
       ui->tWGrade->setRowCount(0);
       if(!sqlQuery->size())
       {
           QMessageBox message(QMessageBox::Question,"提示", "没有成绩信息");
           QPushButton *ok = new QPushButton("确定");
           message.addButton(ok,QMessageBox::AcceptRole);
           message.exec();
       }
       while( sqlQuery->next() )
       {
           if(sqlQuery->isNull(3)) continue; // 如果成绩为NULL，则表示该门课还未考试，跳过
           int rowcount = ui->tWGrade->rowCount();
           ui->tWGrade->insertRow(rowcount);

           for(int column = 0; column < 4; column++)
           {
               ui->tWGrade->setItem( rowcount, column, new QTableWidgetItem( sqlQuery->value(column).toString()));
           }
           // 是否为必修课
           if (sqlQuery->value(4).toString() == "1") ui->tWGrade->setItem( rowcount, 4, new QTableWidgetItem("必修"));
           else ui->tWGrade->setItem( rowcount, 4, new QTableWidgetItem("选修"));
           // 是否通过考试
           if(sqlQuery->value(3).toInt() >= 60) ui->tWGrade->setItem( rowcount, 5, new QTableWidgetItem("是"));
           else ui->tWGrade->setItem( rowcount, 5, new QTableWidgetItem("否"));
       }
    }
}

void student::calGrade()
{
    QSqlQuery *sqlQuery = new QSqlQuery;

    // 根据学生已参与的所有考试，获得当前总学分（这里为了省事把未通过的学分也算上了。。）
    QString sqlStr0 = "select sum(credits) from grade g, course c where sno = '" + cur_student.sno +
           "' and g.cno = c.cno";
    sqlQuery->prepare(sqlStr0);
    QString totalCredits = "";
    sqlQuery->prepare(sqlStr0);
    if(sqlQuery->exec())
    {
        if(sqlQuery->next())
            totalCredits = sqlQuery->value(0).toString();
    }

    // 根据学生学号，获得学生所在年级和专业，按百分制绩点计算公式，获得所有属于该年级和该专业的学生百分制绩点和排名。
    QString yearMajor = cur_student.sno.mid(0, 7);

    QString sqlStr = "select A.sno, A.graPoint, @rank:=@rank+1 ran from (select sno, sum(credits * grade) / " + totalCredits + " graPoint from grade g, course c where sno like '%" + yearMajor +
        "%' and g.cno = c.cno group by sno order by graPoint desc) A, (SELECT @rank:=0) B";
  //  QString sqlStr = "select A.graPoint, count(A.sno) from (select sno, sum(credits * grade) / " + totalCredits + " graPoint from grade g, course c where sno like '%" + yearMajor +
   //             "%' and g.cno = c.cno group by sno) A where A.graPoint > (select B.graPoint from A B where B.sno = '" + cur_student.sno + "')";

    sqlQuery->prepare(sqlStr);

    // 设置当前学生的百分制绩点和排名。
    if(sqlQuery->exec())
    {
        while(sqlQuery->next())
        {
            if(sqlQuery->value(0).toString() == cur_student.sno)
            {
                ui->labelGradePoint->setText(sqlQuery->value(1).toString().mid(0, 4));
                ui->labelGradeRank->setText(sqlQuery->value(2).toString());
                break;
            }
        }

    }
}

// 课表信息子界面 钟子涵
void student::timeTablePageInit()
{
    ui->tWTimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWTimeTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pBTimeTableBack, &QPushButton::clicked, [=]()
    {
        ui->studenPages->setCurrentIndex(0);
    });
    timeTablePage();
}

void student::timeTablePage()
{
    ui->tWTimeTable->clearContents();

    // 获得当前学年
    QString sqlStr0 = "select year from term";
    QSqlQuery *sqlQuery = new QSqlQuery;
    sqlQuery->prepare(sqlStr0);
    QString curYear;
    if(sqlQuery->exec())
    {
        if(sqlQuery->next()) // 要有此句
        {
            curYear = sqlQuery->value(0).toString();
        }
    }
        /*
    // 先获取该学生所在班级
    QString sqlStr0 = "select CLS from student where sno = '" + mySno + "'";
    QSqlQuery *sqlQuery = new QSqlQuery;
    sqlQuery->prepare(sqlStr0);

    QString myClass = "";
    if(sqlQuery->exec())
    {
        if(sqlQuery->next()) // 要有此句
        {
            myClass = sqlQuery->value(0).toString();
        }
    }


    // 根据学生所在班级，利用模糊匹配，得到该学生所在班级本学期开设的课程
    QString sqlStr = "select rcno, c.name, tname, clr, credits, bin(required+0), bin(is_exam+0), co.name, hour, bin(weektime+0), bin(daytime+0) "
                     "from real_course r, course c, teacher t, college co "
                     "where rcno like '%" + myClass + "%' and r.cno = c.cno and r.tno = t.tno and c.clg = co.clg";
    sqlQuery->prepare( sqlStr );
*/
    QString sqlStr = "select rcno, c.name, tname, clr, credits, bin(required+0), bin(is_exam+0), co.name, hour, bin(weektime+0), bin(daytime+0) "
                     "from real_course r, course c, teacher t, college co, student_grade sg "
                     "where rcno like '%" + myclass + "%' and sg.year = '" + curYear +
            "' and sg.cno = r.cno and r.cno = c.cno and r.tno = t.tno and c.clg = co.clg";
    sqlQuery->prepare(sqlStr);
    if( sqlQuery->exec() )
    {
       //读取查询到的记录
       while( sqlQuery->next() )
       {
           QStringList info;
           int column = 0;

           // 整理查询到的记录，获得课程具体信息，显示在课表的对应位置
           QString v1 = "课程性质：";
           QString v2 = "有无考试：";
           QString v3 = "上课周：";
           info << "课务号：" + sqlQuery->value(column++).toString() << "课程名称：" + sqlQuery->value(column++).toString() << "授课老师：" + sqlQuery->value(column++).toString()
                << "课室：" + sqlQuery->value(column++).toString() << "学分：" + sqlQuery->value(column++).toString()
                << v1 + (sqlQuery->value(column++).toString() == "1" ? "必修" : "选修")
                << v2 + (sqlQuery->value(column++).toString() == "1" ? "有" : "无")
                << "开课学院：" + sqlQuery->value(column++).toString() << "总学时：" + sqlQuery->value(column++).toString();

           // weektime，获得上课周
           QString weektime = sqlQuery->value(column++).toString();
           if(weektime.length() != 20)
           {
               for(int i = 20 - weektime.length(); i > 0; i--)
               {
                   weektime = "0" + weektime;
               }
           }
           for(int i = 0; i < weektime.length(); i++)
           {
               if(weektime[i] == '1') v3 += QString::number(i + 1) + " " ;
           }

           info << v3;
           QString in = info.join("\n");

           // daytime，获得每周上课时间，将课程具体信息插入到表格对应位置
           QString daytime = sqlQuery->value(column).toString();
           if(daytime.length() != 35)
           {
               for(int i = 35 - daytime.length(); i > 0; i--)
               {
                   daytime = "0" + daytime;
               }
           }
           for(int i = 0; i < daytime.length(); i++)
           {
               if(daytime[i] == '1')
               {
                   QTextEdit *edit = new QTextEdit();
                   edit->setText(in);
                   edit->setReadOnly(true);
                   ui->tWTimeTable->setCellWidget( i % 5 , i / 5 , edit);
               }
           }
       }
    }
}

// 考试信息子页面 钟子涵
void student::examPageInit()
{
    ui->tWStuExam->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWStuExam->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pBStuExamBack, &QPushButton::clicked, [=](){
        ui->studenPages->setCurrentIndex(0);
    });

    examPage();
}
void student::examPage()
{
    ui->tWStuExam->clearContents();
    // 获得当前学年
    QString sqlStr0 = "select year from term";
    QSqlQuery *sqlQuery = new QSqlQuery;
    sqlQuery->prepare(sqlStr0);
    QString curYear;
    if(sqlQuery->exec())
    {
        if(sqlQuery->next()) // 要有此句
        {
            curYear = sqlQuery->value(0).toString();
        }
    }

    QString sqlStr = "select e.rcno, name, tname, e.clr, begin_time, end_time "
                     "from exam e, real_course r, teacher t, course c, student_grade sg "
                     "where sg.year = '" + curYear + "' and r.rcno like '%" + myclass + "%' and r.cno = sg.cno "
                     "and r.rcno = e.rcno and r.tno = t.tno and r.cno = c.cno";
    sqlQuery->prepare(sqlStr);
    if( sqlQuery->exec() )
    {
       ui->tWStuExam->setRowCount(0);
       //读取查询到的记录
       while( sqlQuery->next() )
       {
           int rowCount = ui->tWStuExam->rowCount();
           ui->tWStuExam->insertRow(rowCount);
           for(int i = 0; i < 4; i++)
           {
               ui->tWStuExam->setItem(rowCount, i, new QTableWidgetItem(sqlQuery->value(i).toString()));
           }
           QString examTime = "";
           QString beginTime = sqlQuery->value(4).toString();
           examTime += beginTime.section(QRegExp("[.T]"), 0, 0) + " " + beginTime.section(QRegExp("[.T]"), 1, 1);
           QString endTime = sqlQuery->value(5).toString();
           examTime += "\n~" + endTime.section(QRegExp("[.T]"), 0, 0) + " " +endTime.section(QRegExp("[.T]"), 1, 1);
           ui->tWStuExam->setItem(rowCount, 4, new QTableWidgetItem(examTime));
       }
    }
}

/******************作者：李家欣************************/
/***************学号：201830580469********************/
/****************************************************/
// 初始化个人信息界面
void student::stuInfoPage(){
    ui->tWTimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWTimeTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pBstuInfoBack, &QPushButton::clicked, [=](){
        ui->studenPages->setCurrentIndex(0);
    });
    QString sql="select s.sno,s.sname,s.sex,s.hometown,s.birth,c.name,c.year,m.name,co.name "
                "from student s, _class c,major m,college co "
                "where s.CLS = c.CLS and c.MJR=m.MJR and m.CLG=co.CLG and s.sno='"+cur_student.sno+"'";
    QSqlQuery query;
    query.exec(sql);
    query.next();//该学生可以登录因此该学生的信息一定存在，所以不设置失败查询
    ui->stu_no->setText(query.value(0).toString());
    ui->stu_name->setText(query.value(1).toString());
    ui->stu_sex->setText(query.value(2).toString());
    ui->stu_home->setText(query.value(3).toString());
    ui->stu_birth->setText(query.value(4).toString());
    ui->stu_class->setText(query.value(5).toString());
    ui->stu_grade->setText(query.value(6).toString());
    ui->stu_major->setText(query.value(7).toString());
    ui->stu_department->setText(query.value(8).toString());
}

//初始化培养计划界面
void student::stuPlanPage(){
    ui->stuPlanTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->pBstuPlanBack, &QPushButton::clicked, [=](){
        ui->studenPages->setCurrentIndex(0);
    });

    //初始化
    init_stuPlanTable();
    get_credits();

    //按课程名查找培养计划中某一课程的具体情况
    connect(ui->pb_stuPlan_search_by_cname, &QPushButton::clicked, [=](){
        QString cname=ui->stuPlan_search_line->text();
        QString sql="select c.cno,c.name,hour,credits,required,year "
                    "from develop_plan d,course c where d.cno=c.cno and MJR='"+myMJR+"' and c.name like '%"+cname+"%'";
        set_stuPlanTable(sql);
        ui->stuPlan_search_line->clear();
    });
}

void student::init_stuPlanTable(){
    QString sql="select c.cno,c.name,hour,credits,required,year "
                "from develop_plan d,course c where d.cno=c.cno and MJR='"+myMJR+"'";
    set_stuPlanTable(sql);
}

void student::set_stuPlanTable(QString sql){
    QSqlQuery query;
    if(query.exec(sql)){
        ui->stuPlanTable->clearContents();
        ui->stuPlanTable->setRowCount(0);
        while(query.next()){
            int rowCount=ui->stuPlanTable->rowCount();
            ui->stuPlanTable->insertRow(rowCount);
            for(int i=0; i<4;i++){
                ui->stuPlanTable->setItem(rowCount,i,new QTableWidgetItem(query.value(i).toString()));
              }
            ui->stuPlanTable->setItem(rowCount,4,new QTableWidgetItem(query.value(4).toString()==1?"必修":"选修"));
            int term=query.value(5).toInt();
            term=(term%10)+(term/10-1)*2;
            ui->stuPlanTable->setItem(rowCount,5,new QTableWidgetItem("第"+QString::number(term)+"学期"));

            //判断课程的修读状态
            QString judge = "select * from student_grade where cno = '"+query.value(0).toString()+"'";
            QSqlQuery sql;
            sql.exec(judge);
            QString status;
            if(sql.size()){
                sql.next();
                if(sql.isNull(3)) status = "正在修读";
                else if(sql.value(3).toInt()>=60) status = "已通过";
                else status="未通过";
            }
            else  status = "未修";
            ui->stuPlanTable->setItem(rowCount,6,new QTableWidgetItem(status));
        }
    }
    else{
        connectErrorMsg=query.lastError().text();
        QMessageBox::information(nullptr,"数据库查询错误！","数据库查询错误，错误信息:"+connectErrorMsg);
    }
}
void student::get_credits(){
    float pass_1=0;//已获得的必修学分
    float nopass_1=0;//未获得的必修学分
    float pass_0=0;//已获得的选修课学分
    float nopass_0=0;//未获得的选修学分
    int num=ui->stuPlanTable->rowCount();
    for(int i=0;i<num;i++){
        float credit=ui->stuPlanTable->item(i,3)->text().toFloat();
        if(ui->stuPlanTable->item(i,4)->text()=="必修"){
            if(ui->stuPlanTable->item(i,6)->text()=="已通过") pass_1+=credit;
            else  nopass_1+=credit;
        }
        else {
            if(ui->stuPlanTable->item(i,6)->text()=="已通过") pass_0+=credit;
            else  nopass_0+=credit;
        }
    }
    ui->lE_pass_0->setText(QString::number(pass_0));
    ui->lE_pass_1->setText(QString::number(pass_1));
    ui->lE_nopass_0->setText(QString::number(nopass_0));
    ui->lE_nopass_1->setText(QString::number(nopass_1));
}

//根据数据库的内容获取授课时间
QString student::get_time(QString daytime,QString weektime){
    QString day="";
    QString time;
    //获得上课周
    bool flag=false;
    int start=0;
    int end=0;
    for(int i=20-weektime.length(); i>0;i--)
        daytime='0'+daytime;

    for(int i=0; i<weektime.length();i++){
        if(weektime[i]=="1") {
            if(flag) end++;
            else {
                start=i;
                end=i;
                flag=true;
            }
        }
        if((weektime[i]=="0"&&flag)||(weektime[i]=="1"&&i==weektime.length()-1)){
            day = day + (QString::number(start+1))+"-"+(QString::number(end+1))+"周"+" ";
            flag= false;
        }
    }
    day = day + "/";
    //获得每天的上课时间
    for(int j=35-daytime.length();j>0;j--)
        daytime='0'+daytime;
    for(int i=0; i<daytime.length();i++){
        if(daytime[i]=="1"){
            int t = i%5;
            switch (t) {
            case 0: time = "(1-2节)";break;
            case 1: time = "(3-4节)";break;
            case 2: time = "(5-6节)";break;
            case 3: time = "(7-8节)";break;
            case 4: time = "(9-11节)";break;
            }
         day = day +"周"+(QString::number(int(i/5+1)))+time+" ";
        }
    }

    return day;
}

//初始化学生选课界面
void student::stuChooselessonPage(){
    ui->chooseLessonTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->chooseLessonTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->pBstuChooselessonBack, &QPushButton::clicked, [=](){
        ui->studenPages->setCurrentIndex(0);
    });

    QDate date(QDate::currentDate());
    int year=date.year();
    int month=date.month();
    int time = 1;
    if(month<9){
        year--;
        time--;
    }
    ui->ChooseLesson_time_label->setText(QString::number(year) + "年第" + QString::number(time) + "学期");

    /************************选课时间限定*****************************/
    /********        为了方便演示，暂时删除限定           **************/
   /* if(month!=9||month!=3){
        QMessageBox message(QMessageBox::Question,"提示", "当前非选课时间");
        QPushButton *ok = new QPushButton("确定");
        message.addButton(ok,QMessageBox::AcceptRole);
        connect(ok, &QPushButton::clicked, [=](){
            ui->studenPages->setCurrentIndex(0);
        });
        message.exec();
    }
    else{

    }*/

    QSqlQuery query;

    //排课表是按照班级为单位的，因此先确定学生的班级，再根据班级查找课程安排进行选课
    QString sql = "select r.rcno, c.name, hour,credits,required,t.tname,r.clr,bin(r.daytime+0), bin(r.weektime+0)"
                  " from course c, real_course r,teacher t "
                  "where c.cno=r.cno and required='0' and t.tno=r.tno and rcno like '%" + myclass + "%'";

    query.exec(sql);
    ui->chooseLessonTable->clearContents();
    ui->chooseLessonTable->setRowCount(0);
    if(query.size()){
        while(query.next()){
            QTableWidgetItem* item[10];
            int rowCount=ui->chooseLessonTable->rowCount();
            ui->chooseLessonTable->insertRow(rowCount);
            for(int i=0; i<7;i++){
                if(i==4) item[i]=new QTableWidgetItem("选修");
                else{
                    item[i]=new QTableWidgetItem(query.value(i).toString());
                }
                ui->chooseLessonTable->setItem(rowCount,i,item[i]);
              }
            item[7]=new QTableWidgetItem(get_time(query.value(7).toString(),query.value(8).toString()));
            ui->chooseLessonTable->setItem(rowCount,7,item[7]);

           //判断这门课的状态
           QSqlQuery query1;
           QString cno = ui->chooseLessonTable->item(rowCount,0)->text().mid(15);
           QString sql_statu="select * from student_grade where sno='"+cur_student.sno+"' and cno='"+cno+"'";
           query1.exec(sql_statu);

           //动态放置按钮用来进行选课和退选
           QPushButton *pBtn=new QPushButton();

           if(query1.size()>0) {
               item[8] = new QTableWidgetItem("已选");
               pBtn->setText(QString("退课"));
           }
           else {
               item[8] = new QTableWidgetItem("未选");
               pBtn->setText(QString("选课"));
           }
           connect(pBtn, &QPushButton::clicked, [=](){
               OnBtnClicked(pBtn);
           });

           ui->chooseLessonTable->setItem(rowCount,8,item[8]);
           ui->chooseLessonTable->setCellWidget(rowCount,9,pBtn);
        }
    }
    else {
        QMessageBox message(QMessageBox::Question,"提示", "当前无可选课程，若有疑问请与教务员联系");
        QPushButton *ok = new QPushButton("返回主界面");
        message.addButton(ok,QMessageBox::AcceptRole);
        connect(ok, &QPushButton::clicked, [=](){
            ui->studenPages->setCurrentIndex(0);
        });
        message.exec();
    }
}

//选课和退课按钮的相关事件
void student::OnBtnClicked(QPushButton *pBtn){
    flag = 1;//选课情况发生更改，即grade表发生变化
    QModelIndex idx=ui->chooseLessonTable->indexAt(QPoint(pBtn->frameGeometry().x(),pBtn->frameGeometry().y()));
    int p=idx.row();
    QString cno = ui->chooseLessonTable->item(p,0)->text().mid(15);
    QString statu = pBtn->text();
    QSqlQuery query;
    if(statu == "退课"){
        QString deletesql = "delete from grade where cno = '"+cno+"' and sno='"+cur_student.sno+"'";
        if(query.exec(deletesql)){
            pBtn->setText("选课");
            ui->chooseLessonTable->setItem(p,8,new QTableWidgetItem("未选"));
            QMessageBox::information(nullptr,"删除成功","删除成功");
        }else{
            connectErrorMsg=query.lastError().text();
            QMessageBox::information(nullptr,"数据库删除错误！","数据库删除错误，错误信息:"+connectErrorMsg);
        }
    }
    else {
        QString term=ui->ChooseLesson_time_label->text().mid(0,4)+ui->ChooseLesson_time_label->text().mid(6,1);
        QString insertsql = "insert into grade(sno,cno,year) values('"+cur_student.sno+"','"+cno+"','"+term+"')";
        if(query.exec(insertsql)){
            pBtn->setText("退课");
            ui->chooseLessonTable->setItem(p,8,new QTableWidgetItem("已选"));
            QString sql_statu="select * from grade where sno='"+cur_student.sno+"' and cno='"+cno+"'";
            QMessageBox::information(nullptr,"选课成功","选课成功");
        }else{
            connectErrorMsg=query.lastError().text();
            QMessageBox::information(nullptr,"数据库删除错误！","数据库删除错误，错误信息:"+connectErrorMsg);
        }
    }
}

void student::create_view(){
    QSqlQuery drop;
    if(drop.exec("drop view student_grade")) qDebug()<<"删除视图";
    else {
        connectErrorMsg=drop.lastError().text();
        QMessageBox::information(nullptr,"视图删除错误！","视图删除错误，错误信息:"+connectErrorMsg);
    }

    //创建视图(将该学生的选课信息输出到视图中)
   QString create_view = "create view student_grade as "
                          "select sno,cno,year,grade from grade where sno='"+cur_student.sno+"'";
    QSqlQuery create;
    if(create.exec(create_view)) qDebug()<<"视图创建成功";
    else {
        connectErrorMsg=create.lastError().text();
        QMessageBox::information(nullptr,"视图创建错误！","视图创建错误，错误信息:"+connectErrorMsg);
    }
}


