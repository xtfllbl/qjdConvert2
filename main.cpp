#include <QtGui/QApplication>
#include "qjd_convert_mainwindow.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    //translator.load("convert_zh.qm");       //中文
    translator.load("convert_en.qm");          //英文
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
