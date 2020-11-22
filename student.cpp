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
    });
    connect(ui->pBStuPlan, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(3);
    });
    connect(ui->pBStuGrade, &QPushButton::clicked,
         [=]()
    {
        gradePage();
    });
    connect(ui->pBStuTimetable, &QPushButton::clicked,
         [=]()
    {
        timeTablePage();
    });
    connect(ui->pBStuExam, &QPushButton::clicked,
         [=]()
    {
        examPage();
    });
}

// 成绩信息子页面 钟子涵
void student::gradePage()
{
    ui->tWGrade->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->studenPages->setCurrentIndex(4);
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

// 课表信息子界面 钟子涵
void student::timeTablePage()
{
    QString mySno = cur_student.sno;
    ui->tWTimeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWTimeTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->studenPages->setCurrentIndex(5);

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
           for(int i = 0; i < weektime.length(); i++)
           {
               if(weektime[i] == '1') v3 += QString::number(i + 1) + " " ;
           }

           info << v3;
           qDebug() << "info:" << info ;
           QString in = info.join("\n");

           // daytime，获得每周上课时间，将课程具体信息插入到表格对应位置
           QString daytime = sqlQuery->value(column).toString();
           qDebug() << "daytime:" << daytime;
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
    ui->studenPages->setCurrentIndex(6);
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
    qDebug() << sqlStr;
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
               qDebug() << sqlQuery->value(i);
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
// 2017EEA1000



