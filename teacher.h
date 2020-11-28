#ifndef TEACHER_H
#define TEACHER_H
#include<QApplication>
#include <QWidget>
#include<QListWidgetItem>
#include<base.h>
#include<database_util.h>
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
    void on_colleges_currentIndexChanged(const QString &arg1);
    void on_majors_currentIndexChanged(const QString &arg1);
    void on_classes_currentIndexChanged(const QString &arg1);


    void on_search_3_clicked(bool checked);


    void on_comboBox_3_currentIndexChanged(int index);

    //void on_search_3_clicked(bool checked);


    //void on_comboBox_3_currentIndexChanged(int index);





    void on_search_student_clicked();

private:
    Ui::teacher *ui;
    QString clg_name;
    QStringList college_name, major_name, class_name, course_name;
};

#endif // TEACHER_H
