#ifndef TEACHER_H
#define TEACHER_H
#include<QApplication>
#include <QWidget>
#include<QComboBox>
#include<QListWidgetItem>
#include<base.h>
#include<database_util.h>
#include<QTableWidgetItem>
#include<QTableWidget>
namespace Ui {
class teacher;
}

class teacher : public QWidget
{
    Q_OBJECT

public:
    explicit teacher(tea cur_teacher,QWidget *parent = nullptr);
    ~teacher();
    tea cur_teacher;
    QSqlQuery query;
    void initial_personal_info();
    void initial_course();
    QString print_weektime(QString);
    QStringList print_daytime(QString);
    void initial_student_info();
    void initial_class(QComboBox*,int);//考试、成绩页面实现QCombobox的可选班级初始化
    void show_butt_submit();//通过按钮的显示与否实现在一定期限内才能更改成绩
    QString fill_zero(QString, int);


signals:
    void logout();

private slots:
    void on_listWidget_itemClicked();//实现页面跳转
    void on_major_2_currentIndexChanged();//实现成绩查询界面根据班级候选框更改科目候选框
    void on_search_3_clicked();//进入成绩信息的查询
    void grade_search_function(int,int,int,int);//实现成绩查询的各功能
    void on_butt_asc_clicked();//实现成绩查询中的升序显示
    void on_butt_desc_clicked();//实现成绩查询中的降序显示
    void on_butt_avg_clicked();//实现成绩查询中的平均分显示
    void on_butt_level_clicked();//跳转到成绩查询中的分数查询页面
    void on_search_5_clicked();//实现成绩查询中的分数段查询
    void on_colleges_currentIndexChanged(const QString &arg1);
    void on_majors_currentIndexChanged(const QString &arg1);
    void on_classes_currentIndexChanged(const QString &arg1);
    void on_butt_return_clicked();//实现由成绩查询中的分数段界面回到成绩查询主页面的跳转
    void on_search_2_clicked();//实现考试信息的查询
    void on_major_3_currentIndexChanged();//实现考试查询界面根据班级候选框更改科目候选框

    void on_search_student_clicked();

    void on_butt_submit_clicked();//提交成绩

private:
    Ui::teacher *ui;
    QString clg_name;
    QStringList college_name, major_name, class_name, course_name;
};

#endif // TEACHER_H
