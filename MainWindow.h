#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<base.h>
#include<database_util.h>
#include<updatepwd.h>
#include<student.h>
#include<teacher.h>
#include<admin.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


    void on_btn_login_clicked();

    void on_btn_update_clicked();


private:
    Ui::MainWindow *ui;
    QSharedPointer<updatePwd> msharptr_u;
    QSharedPointer<teacher> mshareptr_t;
    QSharedPointer<student> mshareptr_s;
    QSharedPointer<admin> mshareptr_a;

};
#endif // MAINWINDOW_H
