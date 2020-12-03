#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // 解决ui预览和实际分辨率不一致
   // if(QT_VERSION>=QT_VERSION_CHECK(5,6,0))
            QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
