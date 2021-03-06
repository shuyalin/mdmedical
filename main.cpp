#include "mdmedical.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mdmedical w;
    w.setFixedSize(1024,768);
#if 1
    int id = QFontDatabase::addApplicationFont("/usr/lib/fonts/DroidSansFallback.ttf");
    QString msyh = QFontDatabase::applicationFontFamilies (id).at(0);
    QFont font(msyh,10);
    qDebug()<<msyh<<endl;
    font.setPointSize(14);
    a.setFont(font);
#endif
//this is for github test
    qInstallMessageHandler(customMessageHandler);
//    w.setGeometry(QRect(30, 30, 720, 450));
    w.setWindowFlags(Qt::FramelessWindowHint);
    //w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    w.show();
    return a.exec();
}
