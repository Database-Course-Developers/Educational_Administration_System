#ifndef ADMIN_UPDATE_EXAM_H
#define ADMIN_UPDATE_EXAM_H

#include <QWidget>

namespace Ui {
class admin_update_exam;
}

class admin_update_exam : public QWidget
{
    Q_OBJECT

public:
    explicit admin_update_exam(QWidget *parent = nullptr);
    ~admin_update_exam();

private:
    Ui::admin_update_exam *ui;
};

#endif // ADMIN_UPDATE_EXAM_H
