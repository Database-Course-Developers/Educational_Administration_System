#ifndef TEACHER_H
#define TEACHER_H
#include<QApplication>
#include <QWidget>
#include<QListWidgetItem>
#include<base.h>
#include<database_util.h>
#include<QString>
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

signals:
    void logout();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

<<<<<<< HEAD
    void on_college_currentIndexChanged(int index);
=======
    void on_search_3_clicked(bool checked);


    void on_comboBox_3_currentIndexChanged(int index);
>>>>>>> e26cf07d40d2edb5235b0f472d8cd8e87d92abf5

    void on_search_3_clicked(bool checked);


    void on_comboBox_3_currentIndexChanged(int index);

private:
    Ui::teacher *ui;
    QString clg_name;
    QStringList college_no, college_name;//college.append("全部");
    QStringList major_no, major_name;//major.append("全部");
    QStringList course_no, course_name;//course.append("全部");
};

#endif // TEACHER_H
