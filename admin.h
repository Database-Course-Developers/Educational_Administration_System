#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include<base.h>
#include<database_util.h>
#include<admin_choose_cou_detail.h>
namespace Ui {
class admin;
}

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(adm curadmin,QWidget *parent = nullptr);
    QString get_grade_querysql();
    QString get_grade_addsql();
    QString get_exam_querysql();
    QString get_exam_addsql();
    QString get_tea_querysql();
    QString get_stu_querysql();
    QString get_cou_querysql();
    QString get_cou_addsql();
    QString get_tea_column_name(int n);
    QString get_stu_column_name(int n);
    QString get_cou_column_name(int n);
    QString get_choose_cou_column_name(int n);
    QString get_choose_cou_querysql();
    void initbox();
    ~admin();
    adm cur_admin;
signals:
    void logout();
    void sendData(QString);
private slots:
    void on_btn_grade_query_clicked();

    void on_table_grade_itemChanged(QTableWidgetItem *item);


    void on_btn_grade_delete_clicked();


    void on_btn_grade_clear_clicked();

    void on_bbtn_grade_add_clicked();

    void on_btn_exam_query_clicked();

    void on_btn_exam_add_clicked();

    void on_btn_exam_delete_clicked();

    void on_btn_exam_clear_clicked();

    void on_table_exam_itemChanged(QTableWidgetItem *item);

    void on_btn_tea_query_clicked();

    void on_table_tea_itemChanged(QTableWidgetItem *item);

    void on_btn_tea_add_clicked();

    void on_btn_tea_delete_clicked();

    void on_btn_tea_reset_clicked();

    void on_btn_stu_query_clicked();

    void on_table_stu_itemChanged(QTableWidgetItem *item);

    void on_btn_stu_add_clicked();


    void on_btn_stu_delete_clicked();

    void on_btn_stu_reset_clicked();

    void on_btn_cou_query_clicked();
    void on_table_cou_itemChanged(QTableWidgetItem *item);
    void on_btn_cou_add_clicked();
    void on_btn_cou_delete_clicked();
    void on_btn_cou_reset_clicked();
    void on_btn_choose_cou_query_clicked();
    void on_btn_choose_cou_reset_clicked();
    void on_table_choose_cou_itemChanged(QTableWidgetItem *item);
    void on_btn_choose_cou_add_clicked();
    void on_btn_choose_cou_delete_clicked();
    void on_btn_choose_cou_turn_clicked();
private:
    Ui::admin *ui;
    admin_choose_cou_detail *a; //为新的界面建立对象

};

#endif // ADMIN_H
