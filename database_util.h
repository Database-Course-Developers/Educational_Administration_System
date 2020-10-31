#ifndef DATABAE_UTIL_H
#define DATABAE_UTIL_H
//数据库模块
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QString>
#include<QDebug>
#include<QSqlError>


extern QString connectErrorMsg;
bool connectDB();



#endif // DATABAE_UTIL_H
