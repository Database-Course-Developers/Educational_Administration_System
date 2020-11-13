#ifndef DATABAE_UTIL_H
#define DATABAE_UTIL_H
//数据库模块
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QString>
#include<QDebug>
#include<QSqlError>
#include<QDate>
#include<QTableWidget>
#include<QMessageBox>

extern QString connectErrorMsg;
bool connectDB();
QVector<QString> getyear();
QVector<QString> getclassroom();
QVector<QString> getcollegename();
QVector<QString> getOneItem(QString sql);


#endif // DATABAE_UTIL_H
