#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include<base.h>
#include<database_util.h>
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
    void initbox();
    ~admin();
    adm cur_admin;
signals:
    void logout();
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

private:
    Ui::admin *ui;

};

#endif // ADMIN_H
