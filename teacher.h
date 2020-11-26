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
signals:
    void logout();
private slots:


    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_search_3_clicked(bool checked);


    void on_comboBox_3_currentIndexChanged(int index);

private:
    Ui::teacher *ui;
};

#endif // TEACHER_H
