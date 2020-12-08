#include<database_util.h>
QString connectErrorMsg="";


bool connectDB() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//添加驱动
    db.setHostName("139.196.127.11");//设置主机名
    db.setDatabaseName("edu_administration");//设置数据库名
    db.setUserName("root");//设置用户名
    //发送连接
    if(!db.open())
    {
        qDebug() << db.lastError();
        connectErrorMsg=db.lastError().text();
        return false;
    }
    return true;
}
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
QVector<QString> getclassroom(){
    QVector<QString> res;
    QString sql="select clr from classroom";
    QSqlQuery query;
    if(query.exec(sql)){
        while(query.next()){
            res.push_back(query.value(0).toString());
        }
    }else{
        QSqlError error=query.lastError().text();
        qDebug()<<error;
    }
    return res;
}
QVector<QString> getcollegename(){
    QVector<QString> res;
    QString sql="select name from college";
    QSqlQuery query;
    if(query.exec(sql)){
        while(query.next()){
            res.push_back(query.value(0).toString());
        }
    }else{
        QSqlError error=query.lastError().text();
        qDebug()<<error;
    }
    return res;
}

QVector<QString> getOneItem(QString sql){
    QVector<QString> res;
    QSqlQuery query;
    if(query.exec(sql)){
        while(query.next()){
            res.push_back(query.value(0).toString());
        }
    }else{
        QSqlError error=query.lastError().text();
        qDebug()<<error;
    }
    return res;
}
void testinsert(){
    QSqlQuery query;
    if(query.exec("insert into tmp values (b'100000100000')")){
        qDebug()<<"i am ok";
    }else{
        QSqlError error=query.lastError().text();
        qDebug()<<error;
    }
}


