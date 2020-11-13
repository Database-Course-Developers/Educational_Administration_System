#ifndef BASE_H
#define BASE_H
#include<QString>
#include<QVector>
//基本类的定义
class stu{
public:
    QString sno;
    QString sname;
    QString sex;
    QString hometown;
    QString birth;
    QString CLS;
    QString pwd;
    stu(QString sno,QString sname,QString sex,QString hometown,QString birth,QString CLS,QString pwd):
        sno(sno),sname(sname),sex(sex),hometown(hometown),birth(birth),CLS(CLS),pwd(pwd)
    {}
    stu(){}
};
class tea{
public:
    QString  tno;
    QString tname;
    QString sex;
    QString pwd;
    QString birth;
    QString position;
    QString tel;
    QString CLG;
    tea(QString tno,QString tname,QString sex,QString pwd,QString birth,QString position,QString tel,QString CLG):
        tno(tno),tname(tname),sex(sex),pwd(pwd),birth(birth),position(position),tel(tel),CLG(CLG)
    {}
    tea(){}
};
class adm{
public:
    QString ano;
    QString pwd;
    adm(QString ano,QString pwd):ano(ano),pwd(pwd){}
    adm(){}
};
extern QString timeformat;
#endif // BASE_H
