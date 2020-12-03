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
    void initial_class(QComboBox*,int);


signals:
    void logout();

private slots:
    void on_listWidget_itemClicked();

    void on_major_2_currentIndexChanged();

    void on_search_3_clicked();

    void grade_search_function(int,int,int,int);

    void on_butt_asc_clicked();

    void on_butt_desc_clicked();

    void on_butt_avg_clicked();


    void on_butt_level_clicked();

    void on_search_5_clicked();


    void on_butt_return_clicked();

    void on_search_2_clicked();



    void on_major_3_currentIndexChanged();

private:
    Ui::teacher *ui;
    QString clg_name;
    QStringList college_no, college_name;//college.append("全部");
    QStringList major_no, major_name;//major.append("全部");
    QStringList course_no, course_name;//course.append("全部");
};

#endif // TEACHER_H
