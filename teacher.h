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
    QString fill_zero(QString, int);


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
    void on_colleges_currentIndexChanged(const QString &arg1);
    void on_majors_currentIndexChanged(const QString &arg1);
    void on_classes_currentIndexChanged(const QString &arg1);
    void on_butt_return_clicked();
    void on_search_2_clicked();
    void on_major_3_currentIndexChanged();

    void on_search_student_clicked();

private:
    Ui::teacher *ui;
    QString clg_name;
    QStringList college_name, major_name, class_name, course_name;
};

#endif // TEACHER_H
