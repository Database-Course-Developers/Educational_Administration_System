#include<base.h>
QVector<QString> getyear(){
    QVector<QString> res;
    QDate date(QDate::currentDate());
    int year=date.year();
    int month=date.month();
    if(month<9){
        year--;
    }
    int st=year-3;
    for(int i=st;i<=year;i++){
        for(int j=1;j<3;j++)
            res.push_back(QString::asprintf("%d%d",i,j));
    }
    return res;

}
