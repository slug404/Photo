#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF8"));

    MainWindow w;
    w.setWindowTitle("个性日历  www.yin99.com");
    w.setWindowIcon(QIcon(QString("./images/logo.png")));
    w.show();
    
    return a.exec();
}
