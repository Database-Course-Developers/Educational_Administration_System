#ifndef ADMIN_CHOOSE_COU_DETAIL_H
#define ADMIN_CHOOSE_COU_DETAIL_H
#include <QWidget>
#include<database_util.h>
extern QString RCNO;
namespace Ui {
class admin_choose_cou_detail;
}

class admin_choose_cou_detail : public QWidget
{
    Q_OBJECT

public:
    explicit admin_choose_cou_detail(QWidget *parent = nullptr);
    ~admin_choose_cou_detail();
    QString get_detail_querysql();
    QString getyear();
    QString getcls();

private:
    Ui::admin_choose_cou_detail *ui;
private slots:
    void recevieData(QString data);
    void on_btn_detail_query_clicked();
    void on_btn_detail_reset_clicked();
    void on_btn_detail_add_clicked();
    void on_btn_detail_delete_clicked();
};

#endif // ADMIN_CHOOSE_COU_DETAIL_H
