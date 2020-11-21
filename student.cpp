#include "student.h"
#include "ui_student.h"

student::student(stu cur_student,QWidget *parent) :
    QWidget(parent),cur_student(cur_student),
    ui(new Ui::student)
{
    ui->setupUi(this);
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
        ui->studenPages->setCurrentIndex(5);
    });
    connect(ui->pBStuExam, &QPushButton::clicked,
         [=]()
    {
        ui->studenPages->setCurrentIndex(6);
    });
}

// 成绩信息子页面 2017CSA1000
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

    // 按学生学号、学年、学期查询成绩
    connect(ui->pBSeGraByTime, &QPushButton::clicked, [=]()
    {
        ui->tWGrade->clearContents(); // 清除原有内容

        QString myTime = ui->cBGradeYear->currentText() + ui->cBGradeTerm->currentText();
        QString mySno = cur_student.sno;

        QSqlQuery *sqlQuery = new QSqlQuery;
        QString sqlStr = "select g.cno, name, credits, grade, bin(required+0) from grade g, course c where sno = '" + mySno
                + "' and year = '" + myTime + "' and g.cno = c.cno ";
        sqlQuery->prepare( sqlStr );
        // qDebug() << sqlStr;
        if( sqlQuery->exec() )
        {
           int row = 0;
           //读取查询到的记录
           //QTableWidget需要先设置一共有多少行数据才能正常显示，如果不设置那么默认是0行数据。
           ui->tWGrade->setRowCount(0);
           // qDebug() << sqlQuery->size();
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
    });

}


