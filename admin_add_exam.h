#ifndef ADMIN_ADD_EXAM_H
#define ADMIN_ADD_EXAM_H

#include <QWidget>

namespace Ui {
class admin_add_exam;
}

class admin_add_exam : public QWidget
{
    Q_OBJECT

public:
    explicit admin_add_exam(QWidget *parent = nullptr);
    ~admin_add_exam();

private:
    Ui::admin_add_exam *ui;
};

#endif // ADMIN_ADD_EXAM_H
