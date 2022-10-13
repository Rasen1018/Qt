#include <QPainter>
#include "paddle.h"

#define WIDTH 75                // 패들 길이
#define HEIGHT 18               // 패들 높이

Paddle::Paddle(QWidget *parent)
    : QWidget{parent}
{
    resize(WIDTH, HEIGHT);      /* 패들을 이미지로 처리 */
    //setPixmap(QPixmap("paddle.png").scaled(WIDTH, HEIGHT));
    //setStyleSheet("background-color: rgba(0,0,0,0");
}

void Paddle::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter p(this);
    p.setPen(QColor(Qt::transparent));              // 패들 펜 설정
    p.setBrush(QColor(QColorConstants::Svg::darkturquoise));    // 패들 색깔
    p.drawRoundedRect(0, 0, WIDTH, HEIGHT, 5, 5);   // 둥근 직사각형으로 패들 그리기
}
