#ifndef ADMIN_UPDATE_GRADE_H
#define ADMIN_UPDATE_GRADE_H

#include <QWidget>

namespace Ui {
class admin_update_grade;
}

class admin_update_grade : public QWidget
{
    Q_OBJECT

public:
    explicit admin_update_grade(QWidget *parent = nullptr);
    ~admin_update_grade();

private:
    Ui::admin_update_grade *ui;
};

#endif // ADMIN_UPDATE_GRADE_H
