#include "student.h"
#include "ui_student.h"
# include<QTextEdit>
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

    // 子页面的初始化
    gradePage();
    timeTablePage();
    examPage();
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

    });
    connect(ui->pBStuExam, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(6);
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

    QString mySno = cur_student.sno;

    // 按学生学号、学年、学期查询成绩
    connect(ui->pBSeGraByTime, &QPushButton::clicked, [=]()
    {
        ui->tWGrade->clearContents(); // 清除原有内容

        QString myTime = ui->cBGradeYear->currentText() + ui->cBGradeTerm->currentText();

        QString sqlStr = "select g.cno, name, credits, grade, bin(required+0) from grade g, course c where sno = '" + mySno
                + "' and year = '" + myTime + "' and g.cno = c.cno ";

        setGradeTable(sqlStr);
    });

    // 按课程名模糊查询
    connect(ui->pBSeaGrade, &QPushButton::clicked, [=](){
        QString myLesson = ui->lESeaGraByName->text();
        ui->lESeaGraByName->clear();

        QString sqlStr = "select g.cno, name, credits, grade, bin(required+0) from grade g, course c where name like '%"
                + myLesson + "%' and c.cno = g.cno and sno = '"+ mySno + "'";

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
           if(sqlQuery->value(3).toString() >= "60") ui->tWGrade->setItem( rowcount, 5, new QTableWidgetItem("是"));
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
void student::timeTablePage()
{
    QString mySno = cur_student.sno;
    ui->tWTimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWTimeTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pBTimeTableBack, &QPushButton::clicked, [=]()
    {
        ui->studenPages->setCurrentIndex(0);
    });

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
void student::examPage()
{
    ui->tWStuExam->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWStuExam->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pBStuExamBack, &QPushButton::clicked, [=](){
        ui->studenPages->setCurrentIndex(0);
    });
    // 先获取该学生所在班级
    QString sqlStr0 = "select CLS from student where sno = '" + cur_student.sno + "'";
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

    QString sqlStr = "select e.rcno, name, tname, e.clr, begin_time, end_time from exam e, real_course r, teacher t, course c "
                     "where e.rcno like '%" + myClass + "%' and e.rcno = r.rcno and r.tno = t.tno and r.cno = c.cno";
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
//



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
    ui->stuPlanTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->pBstuPlanBack, &QPushButton::clicked, [=](){
        ui->studenPages->setCurrentIndex(0);
    });
}
//根据数据库的内容获取授课时间
QString student::get_time(QString daytime,QString weektime){
    QString day="";
    QString time;
    //获得上课周
    bool flag=false;
    int start=0;
    int end=0;
    for(int i=20-weektime.length(); i<20;i++){
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
    for(int i=35-daytime.length(); i<35;i++){
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
    ui->chooseLessonTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
    ui->ChooseLesson_time_label->setText(QString::number(year) + "第" + QString::number(time) + "学期");

    QSqlQuery query;

    //排课表是按照班级为单位的，因此先确定学生的班级，再根据班级查找课程安排进行选课
    QString sql_cla="select cls from student where sno='"+cur_student.sno+"'";
    query.exec(sql_cla);
    query.next();
    QString cla = query.value(0).toString();

    QString sql = "select r.rcno, c.name, hour,credits,required,t.tname,r.clr,bin(r.daytime+0), bin(r.weektime+0)"
                  " from course c, real_course r,teacher t "
                  "where c.cno=r.cno and required='0' and t.tno=r.tno and rcno like '%" + cla + "%'";

    query.exec(sql);
    ui->chooseLessonTable->clearContents();
    ui->chooseLessonTable->setRowCount(0);
    while(query.next()){
        QTableWidgetItem* item[8];
        int rowCount=ui->chooseLessonTable->rowCount();
        qDebug()<<rowCount;
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

    }
}



