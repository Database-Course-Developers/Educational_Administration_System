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
