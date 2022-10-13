#include "breakout.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    breakout w;
    w.setStyleSheet("QWidget {background-color:black;}");       // 배경 색 설정
    w.show();
    return a.exec();
}
