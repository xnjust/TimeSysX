#include "mainwindow.h"
#include "simpletimer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleTimer w;
//    MainWindow w;
//    w.setWindowTitle(QString("智能车竞赛计时系统"));
//    w.show();
//    w.showFullScreen();
//    w.showMaximized();
    w.showMaximized();
    return a.exec();
}
