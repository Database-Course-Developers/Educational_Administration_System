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
        ui->studenPages->setCurrentIndex(4);
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

// 成绩信息子页面
void student::gradePage()
{

}


