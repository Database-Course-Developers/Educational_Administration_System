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
    ~admin();
    adm cur_admin;
signals:
    void logout();
private:
    Ui::admin *ui;
};

#endif // ADMIN_H
