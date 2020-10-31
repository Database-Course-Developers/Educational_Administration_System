#ifndef UPDATEPWD_H
#define UPDATEPWD_H

#include <QWidget>
#include<database_util.h>
#include<QMessageBox>

namespace Ui {
class updatePwd;
}

class updatePwd : public QWidget
{
    Q_OBJECT

public:
    explicit updatePwd(QWidget *parent = nullptr);
    ~updatePwd();
signals:
    void logout();

private slots:


    void on_btn_update_clicked();

    void on_btn_back_clicked();

private:
    Ui::updatePwd *ui;
};

#endif // UPDATEPWD_H
